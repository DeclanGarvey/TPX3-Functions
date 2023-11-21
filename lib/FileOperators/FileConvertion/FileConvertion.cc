#include <string>
#include <cstdint>
#include <experimental/filesystem> // for C++ 17 #include <filesystem>
namespace fs = std::experimental::filesystem;// using namespace fs = filesystem in c++17
using namespace std;

#include "Particle.h"
#include "DirectoryReaders.h"
#include "ParticleFileReaders.h"
#include "ParticleFileReaderFilter.h"

#include "FileWriterFetcher.h"
#include "ParticleFileWriters.h"
#include "MLStoppingPowerFileWriter.h"
#include "FileNames.h"

#include "FileConvertion.h"

void ConvertTo(const string& ipFileName, const string& opFileName, int ipFileType, int opFileType, double DetectorThickness, bool RemoveEdges,
		 const string& ModelPath, const string& SelectionConfigFilePath)
{
	ParticleFileReader* reader = GetFileReader(ipFileName, ipFileType);
	ParticleFileFilter* filteredreader = new ParticleFileFilter(reader, SelectionConfigFilePath);
	
	ParticleFileWriter* writer = GetFileWriter(opFileName, opFileType, DetectorThickness, ModelPath);
	
	particle p;
	while(filteredreader->AssignParticle(p))
	{
		if((p.IsEdge()==false) | (RemoveEdges==false))
			writer->AddParticle(p);
		
	}
	reader->Close();
	writer->Close();
}

void ConvertDirectoryTo(const string& ipDirectoryName, const string& opDirectoryName, int ipFileType, int opFileType, double DetectorThickness, bool RemoveEdges,
		 	const string& ModelPath, const string& SelectionConfigFilePath)
{
	cout<<ipDirectoryName<<endl<<endl;
	DirectoryReader* dir = new DirectoryReader(ipDirectoryName);
	string ipFileName;
	while(dir->AssignNextFile(ipFileName))
	{
		if( ((ipFileType!=-1) & (GetFileType(ipFileName)==ipFileType)) | 
			((ipFileType==-1) & (GetFileType(ipFileName)>=0) & (GetFileType(ipFileName)<=4)) |
			((ipFileType==3) & (GetFileType(ipFileName)==0)) // Deals with special Case of SATRAM file type 
			)
		{
			if( (opFileType == GetFileType(ipFileName)) &
				experimental::filesystem::equivalent(ipDirectoryName, opDirectoryName))
				{
					cout<<"Processing: "<<ipFileName <<endl;
					string NewipFileName = RemoveExtension(ipFileName)+"_new";
					cout<<" =>Altering output file name to \'"+NewipFileName+"\' as to avoid overwriting itself."<<endl;
					ConvertTo(ipDirectoryName +"/"+ ipFileName, opDirectoryName+"/"+RemoveExtension(NewipFileName), ipFileType, opFileType, DetectorThickness, RemoveEdges, 	
						ModelPath,SelectionConfigFilePath);
				}
			else
			{
				cout<<"Processing: "<<ipFileName<<endl;
				ConvertTo(ipDirectoryName +"/"+ ipFileName, opDirectoryName+"/"+RemoveExtension(ipFileName), ipFileType, opFileType, DetectorThickness, RemoveEdges, 	
					ModelPath,SelectionConfigFilePath);
			}
		}
	}
	
}
