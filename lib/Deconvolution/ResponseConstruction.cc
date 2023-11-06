#include <fstream>
#include <string>
#include <iomanip>
#include <map>
using namespace::std;

#include "TFile.h"
#include "TCanvas.h"

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"

#include "DirectoryReaders.h"
#include "ParticleFileReaders.h"
#include "FileNames.h"
#include "RooUnfoldResponseConstructorClass.h"
#include "ResponseConstruction.h"

void ConstructResponse(const string&  ipDirectory, const string&  opDirectory,  const string& ConfigurationDirectory, double DetectorThickness)
{	
	RooUnfoldResponseConstructorClass Constructor;
	Constructor.SetResponseConstrutionParameters(ConfigurationDirectory);
	Constructor.Initialise();
	
	Constructor.AddFile(ipDirectory, DetectorThickness);
	
	RooUnfoldResponse r = Constructor.GetRooUnfoldResponse();
	TFile* f = new TFile(opDirectory.c_str(), "RECREATE");
	f->WriteObject(&r, "Response");
	f->Write();
	f->Close();
}

void ResponseConstructionFromDirectory(const string&  ipDirectory, const string&  opDirectory,  const string& ConfigurationDirectory, double DetectorThickness)
{
	RooUnfoldResponseConstructorClass Constructor;
	Constructor.SetResponseConstrutionParameters(ConfigurationDirectory);
	Constructor.Initialise();
	
	DirectoryReader* dir = new DirectoryReader(ipDirectory);
	string ParticleFileName;
	while(dir->AssignNextFile(ParticleFileName))
	{
		DirectoryReader* dir = new DirectoryReader(ipDirectory+ "/" +ParticleFileName);
		string ipFileName;
		while(dir->AssignNextFile(ipFileName))
		{
			if(GetFileType(ipFileName)==6)
			{
				cout<<"Processing: "<<ipFileName<<endl;
				Constructor.AddFile(ipDirectory + "/"+ParticleFileName+"/"+ipFileName,DetectorThickness);
			}
		}
	}
	
	RooUnfoldResponse r = Constructor.GetRooUnfoldResponse();
	TFile* f = new TFile(opDirectory.c_str(), "RECREATE");
	f->WriteObject(&r, "Response");
	f->Write();
	f->Close();
}

void PrintUnfoldingMatrix(const string& ResponseFileName)//, const string& opFileName)
{
	RooUnfoldResponse* response = new RooUnfoldResponse();
	TFile* f = new TFile(ResponseFileName.c_str(), "READ");
	f->Print();
	f->GetObject("Response", response);
	f->Close();
	
	auto Matrix = response->Hresponse();
	
	/*FILE* opFile = fopen(opFileName.c_str(), "w");
	fprintf(opFile, "%d %lf %lf %d\n", Matrix->GetNbinsX(), Matrix->GetXaxis()->GetXmin(), Matrix->GetXaxis()->GetXmax(), Matrix->GetNbinsY());
	for(int i=1;i<=Matrix->GetNbinsY();i++)
	{
		for(int j=1;j<=Matrix->GetNbinsX();j++)
			fprintf(opFile, "%lf ", Matrix->GetBinContent(j,i));
		fprintf(opFile, "\n");
	}*/
	TCanvas* c = new TCanvas("","");
	Matrix->Draw("COLZ");
	c->SetLogz(1);
	c->Update();
	c->SaveAs("Response.png");
}

	

