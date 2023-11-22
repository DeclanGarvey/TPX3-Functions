#include <fstream>
#include <string>
#include <iomanip>
#include <map>
using namespace::std;

#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"

#include "FileNames.h"
#include "ConfigFileReader.h"
#include "FluenceStoppingPower.h"
#include "ParticleFileReaders.h"
#include "RooUnfoldResponseConstructorClass.h"


void RooUnfoldResponseConstructorClass::SetResponseConstrutionParameters(const string& FileName)
{
	ConfigFileReader* configreader = new ConfigFileReader(FileName);
	while(configreader->ProcessNextVariable())
	{
		if(configreader->GetCurrentVariableName()=="NoStopingPowerBins")
		{
			NoStoppingPowerBins = stoi(configreader->GetCurrentVariableValue());
		}
		else if(configreader->GetCurrentVariableName()=="MinStoppingPower")
		{
			MinStoppingPower = stod(configreader->GetCurrentVariableValue());
		}
		else if(configreader->GetCurrentVariableName()=="MaxStoppingPower")
		{
			MaxStoppingPower = stod(configreader->GetCurrentVariableValue());
		}
		else if(configreader->GetCurrentVariableName()=="ParticleTypes")
		{
			ParticleTypes.clear();
			for(string Value : configreader->GetCurrentVariableAsVector())
			{
				ParticleTypes.push_back(stoi(Value)); 
			}
		}
		else if(configreader->GetCurrentVariableName()=="NoEnergyBins")
		{
			NoEnergyBins.clear();
			TotalNoEnergyBins=0;
			for(string Value : configreader->GetCurrentVariableAsVector())
			{
				NoEnergyBins.push_back(stoi(Value)); 
				TotalNoEnergyBins+=NoEnergyBins[-1];
			}
		}
		else if(configreader->GetCurrentVariableName()=="MinEnergyBin")
		{
			MinEnergy.clear();
			for(string Value : configreader->GetCurrentVariableAsVector())
			{
				MinEnergy.push_back(stod(Value)); 
			}
		}
		else if(configreader->GetCurrentVariableName()=="MaxEnergyBin")
		{
			MaxEnergy.clear();
			for(string Value : configreader->GetCurrentVariableAsVector())
			{
				MaxEnergy.push_back(stod(Value)); 
			}
		}
		else
			cout<<"Unrecongised configuration parameter: "<<configreader->GetCurrentVariableName()<<endl;
	}
	EnergyBinWidth.clear();
	StoppingPowerBinWidth = (MaxStoppingPower-MinStoppingPower)/NoStoppingPowerBins;
	for (size_t i = 0, max = ParticleTypes.size(); i != max; ++i)
	{
		ParticleTypeToParamPosition[ParticleTypes[i]]=i;
		EnergyBinWidth.push_back((MaxEnergy[i] - MinEnergy[i])/NoEnergyBins[i]);
		
	}
}

void RooUnfoldResponseConstructorClass::Initialise()
{
	ResponseMatrix = new TH2D("response", "response",NoStoppingPowerBins,MinStoppingPower, MaxStoppingPower,TotalNoEnergyBins, 0, TotalNoEnergyBins);
	int MatrixPosition=0;
	for (size_t i = 0, max = ParticleTypes.size(); i != max; ++i)
	{	
		MatrixPositions.push_back(MatrixPosition);
		MatrixPosition+=NoEnergyBins[i];
	}
	ValsOutsideRange = new double[TotalNoEnergyBins];
	truth = new TH1D("truth", "truth", TotalNoEnergyBins,0, TotalNoEnergyBins);
}

void RooUnfoldResponseConstructorClass::AddEntry(int ParticleType, double PrimaryEnergy, double StoppingPower,double weight)
{
	if(ParticleTypeToParamPosition.find(ParticleType)!=ParticleTypeToParamPosition.end())
	{    
		int ParamPosition = ParticleTypeToParamPosition[ParticleType];
		if((PrimaryEnergy<MaxEnergy[ParamPosition]) & (PrimaryEnergy>MinEnergy[ParamPosition]))
		{
			int EnergyBin = floor((PrimaryEnergy-MinEnergy[ParamPosition])/EnergyBinWidth[ParamPosition]) + MatrixPositions[ParamPosition];
			if((StoppingPower<MaxStoppingPower) & (StoppingPower>MinStoppingPower))
			{
				
				ResponseMatrix->Fill(StoppingPower,EnergyBin,weight);
			}
			else 
			{
				ValsOutsideRange[EnergyBin]++;
			}
		}
	}
	else{
		fprintf(stdout, "Unrecongised ParticleType %d, not being recorded\n", ParticleType);
	}
}
void RooUnfoldResponseConstructorClass::AddFile(const string& ipFileName, double DetectorThickness)
{
	auto reader = new StoppingPowerFileReader(ipFileName, -2);
	while(reader->ReadNextEntry())
	{
		//if((reader->GetSize()>1) & (reader->GetHeight()<5000))
		//{
		//double weight = FleunceContribution(reader->GetTheta(), DetectorThickness);
		AddEntry(reader->GetParticleType(), reader->GetPrimaryEnergy(), reader->GetStoppingPower(),1);// weight);
		//}
	}
	
	TFile* f = new TFile((RemoveExtension(ipFileName)+".root").c_str());
	TH1F* FileTruth = (TH1F*)f->Get("E_source");
	TCanvas* c = new TCanvas("t","t");
	FileTruth->Draw("HIST");
	c->Update();
	c->SaveAs("tt.png");
	
	TAxis* xaxis = FileTruth->GetXaxis();
	int NBinsX = FileTruth->GetNbinsX();
	for(int i=1;i<=NBinsX;i++)
	{
		//cout<<xaxis->GetBinCenter(i)<<" "<<FileTruth->GetBinContent(i)<<endl;
		int ParamPosition = ParticleTypeToParamPosition[reader->GetParticleType()];
		if((xaxis->GetBinCenter(i)<MaxEnergy[ParamPosition]) & (xaxis->GetBinCenter(i)>MinEnergy[ParamPosition]))
		{
			int EnergyBin = floor((xaxis->GetBinCenter(i)-MinEnergy[ParamPosition])/EnergyBinWidth[ParamPosition]) + MatrixPositions[ParamPosition];
			truth->Fill(EnergyBin,FileTruth->GetBinContent(i));//weight);
		}
	}
	reader->Close();
	f->Close();
}

RooUnfoldResponse RooUnfoldResponseConstructorClass::GetRooUnfoldResponse()
{
	
	TH1D* measured = new TH1D("measured", "measured", NoStoppingPowerBins,MinStoppingPower, MaxStoppingPower);
	TH1D* test = new TH1D("test", "test", TotalNoEnergyBins,0, TotalNoEnergyBins);
	int GroupStart=0;
	double sum;
	int NBinsX = ResponseMatrix->GetNbinsX();
	for(int group = 0; group<NoOfParticleGroups;group++)
	{
		
		for(int j=0;j<NoEnergyBins[group];j++)
		{
			sum =0;
			for(int i=1;i<=NBinsX;i++)
			{
				sum+=ResponseMatrix->GetBinContent(i,j + GroupStart);
				measured->AddBinContent(i,ResponseMatrix->GetBinContent(i,j + GroupStart));
			}
			test->SetBinContent(j + GroupStart,sum);
		}
		GroupStart += NoEnergyBins[group];
	}
	TCanvas* c = new TCanvas("","");
	test->Draw("HIST");
	c->Update();
	c->SaveAs("detected.png");
	
	truth->Draw("HIST");
	c->Update();
	c->SaveAs("truth.png");
	
	measured->Draw("HIST");
	c->Update();
	c->SaveAs("measured.png");
	return RooUnfoldResponse( measured, truth, ResponseMatrix);
}


	

