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

void PrintUnfoldingMatrix(const string& ResponseFileName, const string& ResponseMatrixImageSaveDirectory)
{
	RooUnfoldResponse* response = new RooUnfoldResponse();
	TFile* f = new TFile(ResponseFileName.c_str(), "READ");
	f->Print();
	f->GetObject("Response", response);
	f->Close();
	
	auto Matrix = response->Hresponse();
	
	TCanvas* c = new TCanvas("","");
	Matrix->Draw("COLZ");
	c->SetLogz(1);
	c->Update();
	c->SaveAs(ResponseMatrixImageSaveDirectory.c_str());
}

	

