
#include <string>
using namespace::std;

#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"

#include "FileNames.h"
#include "DirectoryReaders.h"
#include "ParticleFileReaders.h"


#include "Particle.h"
#include "FittingVariables.h"
#include "AngleCalulationFunctions.h"
#include "FluenceStoppingPower.h"
#include "ResponseConstruction.h"
#include "Deconvolution.h"


void Deconvolution(const string& ResponseFileName, const string& ipDirectory, const string& opDirectory, double DetectorThickness, int RegionID)
{	
	RooUnfoldResponse* response = new RooUnfoldResponse();
	TFile* f = new TFile(ResponseFileName.c_str(), "READ");
	f->Print();
	f->GetObject("Response", response);
	f->Close();
	auto Matrix = response->Hresponse();	
	double NoStoppingPowerBins = Matrix->GetNbinsX();
	double MinStoppingPowerBin = Matrix->GetXaxis()->GetXmin();
	double MaxStoppingPowerBin = Matrix->GetXaxis()->GetXmax();
	TH1D* Measured = new TH1D ("meas", "Test Measured", NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);

	auto reader = StoppingPowerFileReader(ipDirectory, RegionID);
	while(reader.ReadNextEntry())
	{
		double weight = FleunceContribution(reader.GetTheta(), DetectorThickness);
		Measured->Fill(reader.GetStoppingPower(), weight);
	}
	
	
	RooUnfoldBayes unfold(&(*response), Measured);
	TH1D* hUnfold= (TH1D*) unfold.Hunfold(RooUnfolding::kCovToys);
	auto cov = unfold.Eunfold();
	//unfold.Print();
	
	TCanvas *c = new TCanvas("test","test");
	Matrix->Draw("COLZ");
	c->Update();
	c->SaveAs("test1.png");
	
	hUnfold->Draw("E1");
	c->Update();
	c->SaveAs("test2.png");
	
	Measured->Draw("hist");
	c->Update();
	c->SaveAs("test3.png");
	
	cov.Draw("COLZ");
	c->Update();
	c->SaveAs("test4.png");
	
	
	
}

void DeconvolutionDirectory(const string& ResponseFileName, const string& ipDirectory, const string& opDirectory, double DetectorThickness, int RegionID)
{	
	RooUnfoldResponse* response = new RooUnfoldResponse();
	TFile* f = new TFile(ResponseFileName.c_str(), "READ");
	f->Print();
	f->GetObject("Response", response);
	f->Close();
	auto Matrix = response->Hresponse();	
	double NoStoppingPowerBins = Matrix->GetNbinsX();
	double MinStoppingPowerBin = Matrix->GetXaxis()->GetXmin();
	double MaxStoppingPowerBin = Matrix->GetXaxis()->GetXmax();
	
	double TotalAcquisitionTime=0;
	TH1D* Measured = new TH1D ("meas", "Test Measured", NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);
	Measured->SetBinErrorOption(TH1::kPoisson2);
	string ipFileName;
	auto Dir = DirectoryReader(ipDirectory);
	while(Dir.AssignNextFile(ipFileName))
	{
		if(GetFileType(ipFileName)==6)
		{
			cout<<"Processing: " << ipFileName<<endl;
			auto reader = StoppingPowerFileReader(ipDirectory+ipFileName, RegionID);
			while(reader.ReadNextEntry())
			{
				//double weight = FleunceContribution(reader.GetTheta(), DetectorThickness);
				Measured->Fill(reader.GetStoppingPower(), 1);//weight);
			}
			TotalAcquisitionTime+=reader.GetTotalAcquisitionTime();
			reader.Close();
		}
	}
	/*int NBins=Measured->GetNbinsX();
	auto PoissonError = Measured->GetSumw2(); 
	if(TotalAcquisitionTime!=0)
	{
		for(int i=0;i<=NBins;i++)
			Measured->SetBinContent(i,Measured->GetBinContent(i)/TotalAcquisitionTime);
	}
	
	
	for (int i = 0; i <=NBins; i++)
	{
		Measured->SetBinError(i, PoissonError->At(i)/TotalAcquisitionTime + Measured->GetBinContent(i)*0.13);
	}*/
	if(TotalAcquisitionTime!=0)
		Measured->Scale(1.0/TotalAcquisitionTime);
	auto PoissonError = Measured->GetSumw2(); 
	int NBins=Measured->GetNbinsX();
	for (int i = 0; i < NBins; i++)
	{
		Measured->SetBinError(i, sqrt(PoissonError->At(i)) + Measured->GetBinContent(i)*0.13);
	}
	RooUnfoldBayes unfold(&(*response), Measured);
	//unfold.smooth();
	TH1D* hUnfold = (TH1D*) unfold.Hunfold(RooUnfolding::kCovariance);
	auto CovMatrixDiag = unfold.EunfoldV(RooUnfolding::kCovariance);
	//auto CovMatrix = errorMatrix * errorMatrix.T();
	FILE* UnfoldFile;
	UnfoldFile = fopen((opDirectory +"_unfold.txt").c_str(),"w");
	PrintHistogram(UnfoldFile, hUnfold);
	NBins=hUnfold->GetNbinsX();
	double statisticalError = CovMatrixDiag(1);
	
	fprintf(UnfoldFile, "%lf", statisticalError);
	for (int i = 2; i < NBins; i++)
	{
		statisticalError = CovMatrixDiag(i);///TotalAcquisitionTime;
		fprintf(UnfoldFile, ",%lf", statisticalError);
	}
	fclose(UnfoldFile);
	
	FILE* HistFile;
	HistFile = fopen((opDirectory +"_hist.txt").c_str(),"w");
	NBins=Measured->GetNbinsX();
	PrintHistogram(HistFile, Measured);
	fprintf(HistFile, "%lf", Measured->GetBinError(1));
	for (int i = 2; i < NBins; i++)
	{
		fprintf(HistFile, ",%lf", Measured->GetBinError(i));
	}
	fclose(HistFile);
	
	TCanvas *c = new TCanvas("test","test");
	Matrix->Draw("COLZ");
	Matrix->SetStats(0);
	c->Update();
	c->SaveAs("test1.png");
	
	
	Measured->Draw("E1");
	Measured->SetStats(0);
	c->Update();
	c->SaveAs("test3.png");
	
	//CovMatrixDiag.Draw("COLZ");
	//c->Update();
	//c->SaveAs("test4.png");
	gPad->SetLogy();
	hUnfold->Draw("E1");
	hUnfold->SetStats(0);
	c->Update();
	c->SaveAs("test2.png");
	
	cout<<"Effective Measurment Time: "<< TotalAcquisitionTime<<endl;
	
}

void DeconvolutionDirectoryRegionSubtraction(const string& ResponseFileName, const string& ipDirectory, const string& opDirectory, double DetectorThickness, int RegionID)
{	
/*	RooUnfoldResponse* response = new RooUnfoldResponse();
	TFile* f = new TFile(ResponseFileName.c_str(), "READ");
	f->Print();
	f->GetObject("Response", response);
	f->Close();
	auto Matrix = response->Hresponse();	
	double NoStoppingPowerBins = Matrix->GetNbinsX();
	double MinStoppingPowerBin = Matrix->GetXaxis()->GetXmin();
	double MaxStoppingPowerBin = Matrix->GetXaxis()->GetXmax();
	TH1D* Measured = new TH1D ("meas", "Test Measured", NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);
	string ipFileName;
	auto Dir = DirectoryReader(ipDirectory);
	while(Dir.AssignNextFile(ipFileName))
	{
		if(GetFileType(ipFileName)==6)
		{
			cout<<"Processing: " << ipFileName<<endl;
			auto reader = StoppingPowerFileReader(ipDirectory+ipFileName);
			while(reader.ReadNextEntry())
			{
				//if(((RegionID==-1) | (reader.GetRegionID()==RegionID)) & (reader.GetTheta()<1.53) & (reader.GetSize()>1))
				//{
				if(reader.GetRegionID()==0)
					Measured0->Fill(reader.GetStoppingPower(), reader.GetWeighting());
				if(reader.GetRegionID()==3)
					Measured3->Fill(reader.GetStoppingPower(), reader.GetWeighting());
				//}
			}
			reader.Close();
		}
	}

	RooUnfoldBayes unfold(&(*response), Measured,5);
	TH1D* hUnfold= (TH1D*) unfold.Hunfold(RooUnfolding::kCovToys);
	auto cov = unfold.Eunfold();
	
	FILE* UnfoldFile;
	UnfoldFile = fopen((opDirectory +"_unfold.txt").c_str(),"w");
	PrintHistogram(UnfoldFile, hUnfold);
	fclose(UnfoldFile);
	
	FILE* HistFile;
	HistFile = fopen((opDirectory +"_hist.txt").c_str(),"w");
	PrintHistogram(HistFile, Measured);
	fclose(HistFile);
	
	TCanvas *c = new TCanvas("test","test");
	Matrix->Draw("COLZ");
	c->Update();
	c->SaveAs("test1.png");
	
	hUnfold->Draw("E1");
	c->Update();
	c->SaveAs("test2.png");
	
	Measured->Draw("hist");
	c->Update();
	c->SaveAs("test3.png");
	
	cov.Draw("COLZ");
	c->Update();
	c->SaveAs("test4.png");*/
	
}

void UnfoldStoppingPowerFile(const string& ResponseFileName, const string& ipDirectory, const string& opDirectory, int Region, bool Merge)
{	
	/*double d=0.05;
	RooUnfoldResponse* response = new RooUnfoldResponse();
	TFile* f = new TFile(ResponseFileName.c_str(), "READ");
	f->Print();
	f->GetObject("Response", response);
	f->Close();
	auto Matrix = response->Hresponse();	
	int NoStoppingPowerBins = Matrix->GetNbinsX();
	double MinStoppingPowerBin = Matrix->GetXaxis()->GetXmin();
	double MaxStoppingPowerBin = Matrix->GetXaxis()->GetXmax();
	
	int NoEnergyBins = Matrix->GetNbinsY();
	double MinEnergyBin = Matrix->GetYaxis()->GetXmin();
	double MaxEnergyBin = Matrix->GetYaxis()->GetXmax();
	FILE* HistFile;
	FILE* UnfoldFile;
	if(Merge)
	{
		HistFile = fopen((opDirectory +"_hist.txt").c_str(),"w");
		UnfoldFile = fopen((opDirectory +"_unfold.txt").c_str(),"w");
	}
	DirectoryReader* dir = new DirectoryReader(ipDirectory);
	string ipFileName;
	TH1D* Measured = new TH1D ("meas", "Test Measured", NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);
	StoppingPowerFileReader* reader = new StoppingPowerFileReader();
	reader->ChangeMeasuringRegion(Region);
	while(dir->AssignNextFile(ipFileName))
	{
		
		if(GetFileType(ipFileName)==5)
		{
			cout<<ipFileName<<endl;
			if(Merge==false)
			{
				cout<<opDirectory +"/"+RemoveExtension(ipFileName)+"_hist.txt"<<endl;
				HistFile = fopen((opDirectory +"/"+RemoveExtension(ipFileName)+"_hist.txt").c_str(),"w");
				UnfoldFile = fopen((opDirectory +"/"+RemoveExtension(ipFileName)+"_unfold.txt").c_str(),"w");
			}
			//
			cout<<ipDirectory +"/"+ipFileName<<endl;
			reader->AssignNewFile(ipDirectory +"/"+ipFileName);
			//cout<<opDirectory +"/"+RemoveExtension(ipFileName)+"_hist.txt"<<endl;
			//reader->SetMinStoppingPower(8.0);
			//reader->SetFluencePerHistogram(50000);
			//reader->SetMinToA(7.9e+7);reader->SetToASteps(219998420000.0);
			//reader->SetMinToA(3.9e4);reader->SetToASteps(5999999220.0);
			reader->SetFluencePerHistogram(100000000);
			
			while(reader->AssignNextHistogram(Measured))
			{
				if(reader->GoalFluenceReached() | (reader->GoalDeltaToAReached()))
				{
					RooUnfoldBayes unfold(&(*response), Measured,5);
					TH1D* hUnfold= (TH1D*) unfold.Hunfold();		
					fprintf(HistFile, "%lf %lf %lf %d %lf %lf ", reader->GetMinToA(), reader->GetMaxToA(),reader->GetRecordedFluence(), NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);
					PrintHistogram(HistFile, Measured);
					fprintf(HistFile, "\n");
					fprintf(UnfoldFile, "%lf %lf %lf %d %lf %lf %d %lf %lf ",  reader->GetMinToA(), reader->GetMaxToA(), reader->GetRecordedFluence(),
									SpectroscopeFitting::ElectronNoEnergyBins, 
									abs(SpectroscopeFitting::ElectronMinEnergyBin/SpectroscopeFitting::ElectronProtonEnergyBinRatio),
									abs(SpectroscopeFitting::ElectronMaxEnergyBin/SpectroscopeFitting::ElectronProtonEnergyBinRatio), 
									SpectroscopeFitting::ProtonNoEnergyBins,  SpectroscopeFitting::ProtonMinEnergyBin, SpectroscopeFitting::ProtonMaxEnergyBin);
					PrintHistogram(UnfoldFile, hUnfold);
					fprintf(UnfoldFile, "\n");
					Measured->Reset();
				}
			}
			if(Merge==false)
			{
				RooUnfoldBayes unfold(&(*response), Measured,2);
				TH1D* hUnfold= (TH1D*) unfold.Hunfold(RooUnfolding::kCovToys);		
				fprintf(HistFile, "%lf %lf %lf %d %lf %lf ", reader->GetMinToA(), reader->GetMaxToA(),reader->GetRecordedFluence(), NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);
				PrintHistogram(HistFile, Measured);
				fprintf(HistFile, "\n");
				fprintf(UnfoldFile, "%lf %lf %lf %d %lf %lf %d %lf %lf ",  reader->GetMinToA(), reader->GetMaxToA(), reader->GetRecordedFluence(),
								SpectroscopeFitting::ElectronNoEnergyBins, 
								abs(SpectroscopeFitting::ElectronMinEnergyBin/SpectroscopeFitting::ElectronProtonEnergyBinRatio),
								abs(SpectroscopeFitting::ElectronMaxEnergyBin/SpectroscopeFitting::ElectronProtonEnergyBinRatio), 
								SpectroscopeFitting::ProtonNoEnergyBins,  SpectroscopeFitting::ProtonMinEnergyBin, SpectroscopeFitting::ProtonMaxEnergyBin);
				PrintHistogram(UnfoldFile, hUnfold);
				fprintf(UnfoldFile, "\n");
				Measured->Reset();
				if(HistFile!=NULL)
				{
					fclose(HistFile);
					fclose(UnfoldFile);
				}
			}
		}
	}
	if(Merge)
	{
		//RooUnfoldBayes unfold(&(*response), Measured,10);
		RooUnfoldBayes unfold(&(*response), Measured,2);
		TH1D* hUnfold= (TH1D*) unfold.Hunfold(RooUnfolding::kCovToys);		
		fprintf(HistFile, "%lf %lf %lf %d %lf %lf ", reader->GetMinToA(), reader->GetMaxToA(),reader->GetRecordedFluence(), NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);
		PrintHistogram(HistFile, Measured);
		fprintf(HistFile, "\n");
		fprintf(UnfoldFile, "%lf %lf %lf %d %lf %lf %d %lf %lf ",  reader->GetMinToA(), reader->GetMaxToA(), reader->GetRecordedFluence(),
						SpectroscopeFitting::ElectronNoEnergyBins, 
						abs(SpectroscopeFitting::ElectronMinEnergyBin/SpectroscopeFitting::ElectronProtonEnergyBinRatio),
						abs(SpectroscopeFitting::ElectronMaxEnergyBin/SpectroscopeFitting::ElectronProtonEnergyBinRatio), 
						SpectroscopeFitting::ProtonNoEnergyBins,  SpectroscopeFitting::ProtonMinEnergyBin, SpectroscopeFitting::ProtonMaxEnergyBin);
		PrintHistogram(UnfoldFile, hUnfold);
		fprintf(UnfoldFile, "\n");
		Measured->Reset();
	}
	if(HistFile!=NULL)
	{
		fclose(HistFile);
		fclose(UnfoldFile);	
	}*/

}


/*void UnfoldStoppingPowerFile(const string& ResponseFileName, const string& ipDirectory, const string& opDirectory, int Region,bool Combine)
{	
	double d=0.05;
	RooUnfoldResponse* response = new RooUnfoldResponse();
	TFile* f = new TFile(ResponseFileName.c_str(), "READ");
	f->Print();
	f->GetObject("Response", response);
	f->Close();
	auto Matrix = response->Hresponse();	
	
	int NoEnergyBins = Matrix->GetNbinsY();
	double MinEnergyBin = Matrix->GetYaxis()->GetXmin();
	double MaxEnergyBin = Matrix->GetYaxis()->GetXmax();
	TCanvas *c = new TCanvas("test","test");
	Matrix->Draw("COLZ");
	c->Update();
	c->SaveAs("test1.png");
	
	DirectoryReader* dir = new DirectoryReader(ipDirectory);
	string ipFileName;
	FILE* HistFile = fopen((opDirectory +"_hist.txt").c_str(),"w");
	FILE* UnfoldFile = fopen((opDirectory +"_unfold.txt").c_str(),"w");
	TH1D* Measured = new TH1D ("meas", "Test Measured", NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);
	StoppingPowerFileReader* reader = new StoppingPowerFileReader();
	reader->ChangeMeasuringRegion(Region);
	while(dir->AssignNextFile(ipFileName))
	{
		if(GetFileType(ipFileName)==5)
		{
			cout<<ipFileName<<endl;
			reader->AssignNewFile(ipDirectory +"/"+ipFileName);
			//cout<<opDirectory +"/"+RemoveExtension(ipFileName)+"_hist.txt"<<endl;
			//reader->SetMinStoppingPower(8.0);
			reader->SetFluencePerHistogram(10000000);
			while(reader->AssignNextHistogram(Measured))
			{
				RooUnfoldBayes unfold(&(*response), Measured);
				TH1D* hUnfold= (TH1D*) unfold.Hunfold(RooUnfolding::kCovToys);		
				fprintf(HistFile, "%lf %lf %lf %d %lf %lf ", reader->GetMinToA(), reader->GetMaxToA(),reader->GetRecordedFluence(), NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);
				PrintHistogram(HistFile, Measured);
				fprintf(HistFile, "\n");
				fprintf(UnfoldFile, "%lf %lf %lf %d %lf %lf %d %lf %lf ",  reader->GetMinToA(), reader->GetMaxToA(), reader->GetRecordedFluence(),
								SpectroscopeFitting::ElectronNoEnergyBins, 
								abs(SpectroscopeFitting::ElectronMinEnergyBin/SpectroscopeFitting::ElectronProtonEnergyBinRatio),
								abs(SpectroscopeFitting::ElectronMaxEnergyBin/SpectroscopeFitting::ElectronProtonEnergyBinRatio), 
								SpectroscopeFitting::ProtonNoEnergyBins,  SpectroscopeFitting::ProtonMinEnergyBin, SpectroscopeFitting::ProtonMaxEnergyBin);
				PrintHistogram(UnfoldFile, hUnfold);
				fprintf(UnfoldFile, "\n");
				Measured->Reset();
			}
		}
	}
	
	RooUnfoldBayes unfold(&(*response), Measured);
	TH1D* hUnfold= (TH1D*) unfold.Hunfold(RooUnfolding::kCovToys);		
	fprintf(HistFile, "%lf %lf %lf %d %lf %lf ", reader->GetMinToA(), reader->GetMaxToA(),reader->GetRecordedFluence(), NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);
	PrintHistogram(HistFile, Measured);
	fprintf(HistFile, "\n");
	fprintf(UnfoldFile, "%lf %lf %lf %d %lf %lf %d %lf %lf ",  reader->GetMinToA(), reader->GetMaxToA(), reader->GetRecordedFluence(),
					SpectroscopeFitting::ElectronNoEnergyBins, 
					abs(SpectroscopeFitting::ElectronMinEnergyBin/SpectroscopeFitting::ElectronProtonEnergyBinRatio),
					abs(SpectroscopeFitting::ElectronMaxEnergyBin/SpectroscopeFitting::ElectronProtonEnergyBinRatio), 
					SpectroscopeFitting::ProtonNoEnergyBins,  SpectroscopeFitting::ProtonMinEnergyBin, SpectroscopeFitting::ProtonMaxEnergyBin);
	PrintHistogram(UnfoldFile, hUnfold);
	fprintf(UnfoldFile, "\n");
	fclose(HistFile);
	fclose(UnfoldFile);
	
}*/
