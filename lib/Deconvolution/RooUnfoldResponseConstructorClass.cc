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

/*

*/
RooUnfoldResponseConstructorClass::RooUnfoldResponseConstructorClass()
{
	Initialise();
}

RooUnfoldResponseConstructorClass::RooUnfoldResponseConstructorClass(const string& FileName)
{
	if(FileName.empty()==false)
	{
		ConfigFileReader* configreader = new ConfigFileReader(FileName);
		while(configreader->ProcessNextVariable())
		{
			if(configreader->GetCurrentVariableName()=="NoStoppingPowerBins")
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
				cout<<configreader->GetCurrentVariableName()<<endl;
				cout<<configreader->GetCurrentVariableValue()<<endl;
				for(string Value : configreader->GetCurrentVariableAsVector())
				{
					ParticleTypes.push_back(stoi(Value)); 
				}
			}
			else if(configreader->GetCurrentVariableName()=="EnergyBinEdges")
			{
				for(string StringVector : configreader->GetCurrentVariableAsVector(' '))
				{
					cout<<StringVector<<endl;
					vector<double> EnergyBinEdgeVector;
					double value;
					
					istringstream (StringVector) >> value;
					EnergyBinEdgeVector.push_back(value);
				}
			}
			else
				cout<<"Unrecongised configuration parameter: "<<configreader->GetCurrentVariableName()<<endl;
		}
			
	}
	Initialise();
}

void RooUnfoldResponseConstructorClass::Initialise()
{
	NoOfParticleGroups = ParticleTypes.size();
	if(NoOfParticleGroups!=EnergyBinEdges.size())
		cerr<<"Error: ParticlesTypes and EnergyBinEdges are different"<<endl;
	
	StoppingPowerBinWidth = (MaxStoppingPower-MinStoppingPower)/NoStoppingPowerBins;
	
	MinEnergy.clear();
	MaxEnergy.clear();
	MatrixStartPoint.clear();
	size_t TotalNoEnergyBinEdges=1;
	for(size_t i=0;i<NoOfParticleGroups;i++)
	{
		ParticleTypeToParamPosition[ParticleTypes[i]]=i;
		MinEnergy.push_back(EnergyBinEdges[i][0]);
		MaxEnergy.push_back(EnergyBinEdges[i][EnergyBinEdges[i].size()-1]);
		TotalNoEnergyBinEdges+=EnergyBinEdges[i].size()-1;
		cout<<MinEnergy[i]<<" "<<MaxEnergy[i]<<endl;
	}
	MatrixStartPoint.push_back(0);
	for (size_t i = 1; i<NoOfParticleGroups; ++i)
		MatrixStartPoint.push_back(MaxEnergy[i-1]-MinEnergy[i-1]);
	
	double MatrixBinEdges[TotalNoEnergyBinEdges];
	int CurrentRow=1;
	MatrixBinEdges[0]=0;
	for (size_t i = 0; i<ParticleTypes.size(); ++i)
	{
		for (size_t j = 1; j<EnergyBinEdges[i].size(); ++j)
		{
			MatrixBinEdges[CurrentRow] = (EnergyBinEdges[i][j] + MatrixStartPoint[i]) - MinEnergy[i];
			CurrentRow++;
		}
	}
	for(size_t i=0;i<TotalNoEnergyBinEdges;i++)
		cout<<MatrixBinEdges[i]<<" ";
	cout<<endl;
	for (size_t i = 0; i<NoOfParticleGroups; ++i)
		cout<<MatrixStartPoint[i]<<" ";
	cout<<endl;
	ResponseMatrix = new TH2D("response", "response",NoStoppingPowerBins,MinStoppingPower, MaxStoppingPower,TotalNoEnergyBinEdges-1, MatrixBinEdges);
	truth = new TH1D("truth", "truth", TotalNoEnergyBinEdges-1,MatrixBinEdges);
	measured = new TH1D("measured", "measured", NoStoppingPowerBins,MinStoppingPower, MaxStoppingPower);
}

void RooUnfoldResponseConstructorClass::AddEntry(int ParticleType, double PrimaryEnergy, double StoppingPower,double weight)
{
	if(ParticleTypeToParamPosition.find(ParticleType)!=ParticleTypeToParamPosition.end())
	{    
		int ParamPosition = ParticleTypeToParamPosition[ParticleType];
		if((PrimaryEnergy<MaxEnergy[ParamPosition]) & (PrimaryEnergy>MinEnergy[ParamPosition]))
		{
			if((StoppingPower<MaxStoppingPower) & (StoppingPower>MinStoppingPower))
			{
				ResponseMatrix->Fill(StoppingPower, (PrimaryEnergy - MinEnergy[ParamPosition]) + MatrixStartPoint[ParamPosition],weight);
			}
		}
	}
	else{
		//fprintf(stdout, "Unrecongised ParticleType %d, not being recorded\n", ParticleType);
	}
}
void RooUnfoldResponseConstructorClass::AddFile(const string& ipFileName, double DetectorThickness)
{
	auto reader = new StoppingPowerFileReader(ipFileName, -2);
	while(reader->ReadNextEntry())
	{
		//double weight = EnergyBin FleunceContribution(reader->GetTheta(), DetectorThickness);
		AddEntry(reader->GetParticleType(), reader->GetPrimaryEnergy(), reader->GetStoppingPower(),1);// weight);
	}
	
	TFile* f = new TFile((RemoveExtension(ipFileName)+".root").c_str());
	TH1F* FileTruth = (TH1F*)f->Get("E_source");
	
	TAxis* xaxis = FileTruth->GetXaxis();
	int NBinsX = FileTruth->GetNbinsX();
	for(int i=1;i<=NBinsX;i++)
	{
		int ParamPosition = ParticleTypeToParamPosition[reader->GetParticleType()];
		if((xaxis->GetBinCenter(i)<MaxEnergy[ParamPosition]) & (xaxis->GetBinCenter(i)>MinEnergy[ParamPosition]))
		{
			truth->Fill(xaxis->GetBinCenter(i) + MatrixStartPoint[ParamPosition]-MinEnergy[ParamPosition],FileTruth->GetBinContent(i));
		}
	}
	reader->Close();
	f->Close();
}

RooUnfoldResponse RooUnfoldResponseConstructorClass::GetRooUnfoldResponse()
{
	
	measured = ResponseMatrix->ProjectionX("measured");
	TCanvas* c = new TCanvas("","");
	
	truth->Draw("HIST");
	c->Update();
	c->SaveAs("truth.png");
	
	measured->Draw("HIST");
	c->Update();
	c->SaveAs("measured.png");
	
	ResponseMatrix->Draw("COLZ");
	c->Update();
	c->SaveAs("Response.png");
	return RooUnfoldResponse( measured, truth, ResponseMatrix);
}
