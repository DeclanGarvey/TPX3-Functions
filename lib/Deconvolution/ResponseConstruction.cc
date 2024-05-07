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

/*
Function that takes in the input filename, output directory, configuration directory, and detector thickness

Using the configuration(default parameters if ConfigurationDirectory is empty) and the data provided at ipFileName creates a Stopping Power-Primary Energy BayesResponse Matrix.
The created Response matrix is than saved to opDirectory

This functions is used for a single file of data
*/
void ConstructResponse(const string&  ipFileName, const string&  opDirectory,  const string& ConfigurationDirectory, double DetectorThickness)
{	
	RooUnfoldResponseConstructorClass Constructor(ConfigurationDirectory);
	
	Constructor.AddFile(ipFileName, DetectorThickness);
	
	RooUnfoldResponse r = Constructor.GetRooUnfoldResponse();
	TFile* f = new TFile(opDirectory.c_str(), "RECREATE");
	f->WriteObject(&r, "Response");
	f->Write();
	f->Close();
}

/*
Function that takes in the input directory, output directory, configuration directory, and detector thickness

Using the configuration(default parameters if ConfigurationDirectory is empty) and the data provided at ipDirectory creates a Stopping Power-Primary Energy BayesResponse Matrix.
The created Response matrix is than saved to opDirectory

This function is used for a directory of files of data typically in strurture
ParticleType1
	->ParticleType1DataFile1
	->ParticleType1DataFile2
	...
ParticleType2
	->ParticleType2DataFile1
	->ParticleType2DataFile2
	...
...
*/
void ResponseConstructionFromDirectory(const string&  ipDirectory, const string&  opDirectory,  const string& ConfigurationDirectory, double DetectorThickness)
{
	RooUnfoldResponseConstructorClass Constructor(ConfigurationDirectory);
	
	DirectoryReader* dir = new DirectoryReader(ipDirectory);
	string ParticleFileName;
	while(dir->AssignNextFile(ParticleFileName))
	{
		DirectoryReader* dir = new DirectoryReader(ipDirectory+ "/" +ParticleFileName);
		string ipFileName;
		while(dir->AssignNextFile(ipFileName))
		{
			if(CheckFileType(ipFileName, 5))
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

/*
Takes in the file name of the Response Matrix and saves root graph of the response to matrix to the directory provided by ResponseMatrixImageSaveDirectory
*/
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

	

