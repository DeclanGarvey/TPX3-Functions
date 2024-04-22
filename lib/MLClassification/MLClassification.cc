
#include <string>
#include <map>
using namespace::std;

#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "FileNames.h"
#include "DirectoryReaders.h"
#include "ParticleFileReaders.h"
#include "ParticleFilters.h"
#include "ParticleFileReaderFilter.h"
#include "Particle.h"
#include "ElectronProtonClassifier.h"
#include "ProtonSpectroscope.h"
#include "FluenceStoppingPower.h"

#include "MLClassification.h"


//void Deconvolution(const string& ResponseFileName, const string& ipDirectory, const string& opDirectory, double DetectorThickness, int RegionID)
//{	
	/*RooUnfoldResponse* response = new RooUnfoldResponse();
	TFile* f = new TFile(ResponseFileName.c_str(), "READ");
	f->Print();
	f->GetObject("Response", response);
	f->Close();
	auto Matrix = response->Hresponse();	
	double NoStoppingPowerBins = Matrix->GetNbinsX();
	double MinStoppingPowerBin = Matrix->GetXaxis()->GetXmin();
	double MaxStoppingPowerBin = Matrix->GetXaxis()->GetXmax();
	double StoppingPowerBinWidth = (MaxStoppingPowerBin - MinStoppingPowerBin)/NoStoppingPowerBins;
	double TotalAcquisitionTime=0;
	TH1D* Measured = new TH1D ("meas", "Test Measured", NoStoppingPowerBins, MinStoppingPowerBin, MaxStoppingPowerBin);
	Measured->SetBinErrorOption(TH1::kPoisson2);
	string ipFileName;
	//auto Dir = DirectoryReader(ipDirectory);
	//while(Dir.AssignNextFile(ipFileName))
	//{
		//if(GetFileType(ipFileName)==6)
		//{
	cout<<"Processing: " << ipDirectory<<endl;
	//auto reader = StoppingPowerFileReader(ipDirectory+ipFileName, RegionID);
	//		while(reader.ReadNextEntry())
	//		{
	ifstream reader(ipDirectory.c_str());
	double element;
	for(int i=0;i<NoStoppingPowerBins;i++) (reader >> element)
	{
		reader >> element;
		Measured->Fill((reader.GetStoppingPower()-MinStoppingPowerBin)/StoppingPowerBinWidth,element);
	}
	for(int i=0;i<NoStoppingPowerBins;i++) (reader >> element)
	{
		reader >> element;
		Measured->SetBinError((reader.GetStoppingPower()-MinStoppingPowerBin)/StoppingPowerBinWidth, element);
	}
	reader.Close();
		}
	}

	if(TotalAcquisitionTime!=0)
		Measured->Scale(1.0/TotalAcquisitionTime);
	auto PoissonError = Measured->GetSumw2(); 
	int NBins=Measured->GetNbinsX();
	for (int i = 1; i <= NBins; i++)
	{
		
	}
	RooUnfoldBayes unfold(&(*response), Measured,5);
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
	for (int i = 2; i <= NBins; i++)
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
	for (int i = 2; i <= NBins; i++)
	{
		fprintf(HistFile, ",%lf", Measured->GetBinError(i));
	}
	fclose(HistFile);
	
	TCanvas *c = new TCanvas("test","test");
	Matrix->Draw("COLZ");
	Matrix->SetStats(0);
	c->Update();
	c->SaveAs("test1.png");
	
	gPad->SetLogy();
	Measured->Draw("E1");
	Measured->SetStats(0);
	c->Update();
	c->SaveAs("test3.png");

	
	gPad->SetLogy();
	hUnfold->Draw("E1");
	hUnfold->SetStats(0);
	c->Update();
	c->SaveAs("test2.png");
	
	cout<<"Effective Measurment Time: "<< TotalAcquisitionTime<<endl;*/
	
//}

void MLClassificationFromDirectory(const string& ClassifierModelPath, const string& SpectroscopeModelPath, const string& ipDirectory, const string& opDirectory, int ipFileType, int RegionID)
{	
	auto Classifier = std::make_unique<ElectronProtonClassifier>(ClassifierModelPath);
	auto Spectroscope = std::make_unique<ProtonSpectroscope>(SpectroscopeModelPath);
	double TotalAcquisitionTime=0;
	map<double,double> FrameAcquisitionTimeHistory;
	
	int ElectronCounts=0, ProtonCounts=0;
	double ProtonEnergyBins[]={10,15,20,30,40,50,60,70,80,90,100,125,150,175,200,300,400};
	TH1D* ProtonSpectrum = new TH1D ("Spectrum", "Spectrum", 16,ProtonEnergyBins);
	ProtonSpectrum->SetBinErrorOption(TH1::kPoisson2);
	
	ParticleFileReader reader = GetEmptyFileReader(ipFileType);
	ParticleFileFilter filteredreader(&reader);//reader, SelectionConfigFilePath);
	filteredreader.AddFilter(GetFilter("RegionID", to_string(RegionID)));
	filteredreader.AddFilter(GetFilter("MaximumAcquisitionTime", "2e-2"));
	filteredreader.AddFilter(GetFilter("MaximumHeight", "1000"));
	filteredreader.AddFilter(GetFilter("MinimumSize", "2"));
	/*(abs(AcquisitionTime_)<1e-4) | ((AcquisitionTime_>MaximumFrameAcquisitionTime_) & (MaximumFrameAcquisitionTime_!=-1)) | 
		(((AcquisitionRegion_==-2) | (Region_==AcquisitionRegion_))==false)  | 
		((Size_<=2) & (Height_>1000)) | (Height_>1500)*/
	
	particle p;
	string ipFileName;
	DirectoryReader Dir = DirectoryReader(ipDirectory);
	double PreviousToA=-1;
	while(Dir.AssignNextFile(ipFileName))
	{
		if( ((ipFileType!=-1) & (GetFileType(ipFileName)==ipFileType)) | 
			((ipFileType==-1) & (GetFileType(ipFileName)>=0) & (GetFileType(ipFileName)<=4)) |
			((ipFileType==3) & (GetFileType(ipFileName)==0)) | ((ipFileType==10) & (GetFileType(ipFileName)==0)) // Deals with special Case of SATRAM file type 
			)
		{
			cout<<"Processing: " << ipFileName<<endl;
			reader.UpdateFileInput(ipDirectory +"/"+ ipFileName);
			while(filteredreader.AssignParticle(p))
			{
				auto ParticleType = Classifier->PredictFromParticle(p);
				//cout<<ParticleType<<endl;
				//cout<<RegionID<<" "<<p.AcquisitionTime<<" "<<p.RegionID<<endl;
				if(round(ParticleType)==0)
					ElectronCounts++;
				else {
					ProtonCounts++;
					//cout<<"here"<<endl;
					auto e =Spectroscope->PredictFromParticle(p);
					cout<<e<<endl;
					//if(e<50)
					//	cout<<e<<endl;
					ProtonSpectrum->Fill(e);
				}
			}
			
			if(p.GetMinToA()!=PreviousToA) // Implies a new acquisition frame
			{
				TotalAcquisitionTime+=p.AcquisitionTime;
				PreviousToA = p.GetMinToA();
			}
			cout<<ElectronCounts<<" "<<ProtonCounts<<endl;
			reader.Close();
		}
	}
	ProtonSpectrum->Scale(1.0,"WIDTH");
	if(TotalAcquisitionTime!=0)
		ProtonSpectrum->Scale(1.0/TotalAcquisitionTime);
	
	FILE* HistFile;
	HistFile = fopen((opDirectory +"_mlclass.txt").c_str(),"w");
	int NBins=ProtonSpectrum->GetNbinsX();
	fprintf(HistFile, "%d,",ElectronCounts);
	PrintHistogram(HistFile, ProtonSpectrum);
	
	cout<<"========="<<NBins<<endl;
	fprintf(HistFile, "%lf,", sqrt(ElectronCounts));
	fprintf(HistFile, "%lf", ProtonSpectrum->GetBinError(1));
	for (int i = 2; i <= NBins; i++)
	{
		fprintf(HistFile, ",%lf", ProtonSpectrum->GetBinError(i));
	}
	fclose(HistFile);

	TCanvas *c = new TCanvas("test","test");
	gPad->SetLogy();
	ProtonSpectrum->Draw("E1");
	ProtonSpectrum->SetStats(0);
	c->Update();
	c->SaveAs("Spectrum1.png");

	
	/*gPad->SetLogy();
	hUnfold->Draw("E1");
	hUnfold->SetStats(0);
	c->Update();
	c->SaveAs("test2.png");
	
	cout<<endl<<endl;;
	cout<<"==============================================="<<endl;
	cout<<"Acquisition Time [s], Number of Frames Recorded"<<endl;
	cout<<"==============================================="<<endl;
	for(auto& it : FrameAcquisitionTimeHistory)
		cout<<it.first<< ", "<< it.second<<endl;
	
		
	cout<<endl<<"=>Effective Measurment Time: "<< TotalAcquisitionTime<<endl<<endl;*/
}

