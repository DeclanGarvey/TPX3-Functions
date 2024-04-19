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
#include "ParticleGeneratorBaseClass.h"
#include "ParticleFileWriters.h"
#include "MLStoppingPowerFileWriter.h"
#include "FileNames.h"

#include "FileConvertion.h"

void WriteParticleInformation(ParticleGeneratorBaseClass reader, ParticleFileWriter writer, bool RemoveEdges)
{
	particle p;
	while(reader.AssignParticle(p))
	{
		if((p.IsEdge()==false) | (RemoveEdges==false))
			writer.AddParticle(p);
		
	}
}

void ConvertTo(const string& ipFileName, const string& opFileName, int ipFileType, int opFileType, double DetectorThickness, bool RemoveEdges,
		 const string& ModelPath, const string& SelectionConfigFilePath)
{
	ParticleFileReader reader = GetFileReader(ipFileName, ipFileType);
	ParticleFileFilter filteredreader(&reader, SelectionConfigFilePath);
	
	ParticleFileWriter writer = GetFileWriter(opFileName, opFileType, DetectorThickness, ModelPath);
	
	WriteParticleInformation(filteredreader, writer, RemoveEdges);
	
	//reader.Close();
	//writer.Close();
}

void ConvertDirectoryTo(const string& ipDirectoryName, const string& opDirectoryName, int ipFileType, int opFileType, double DetectorThickness, bool RemoveEdges,
		 	const string& ModelPath, const string& SelectionConfigFilePath)
{
	string ipFileName;
	ParticleFileWriter writer = GetEmptyFileWriter(opFileType, DetectorThickness, ModelPath);
	ParticleFileReader reader = GetEmptyFileReader(ipFileType);
	ParticleFileFilter filteredreader(&reader, SelectionConfigFilePath);
	
	DirectoryReader dir(ipDirectoryName);
	while(dir.AssignNextFile(ipFileName))
	{
		if( ((ipFileType!=-1) & (GetFileType(ipFileName)==ipFileType)) | 
			((ipFileType==-1) & (GetFileType(ipFileName)>=0) & (GetFileType(ipFileName)<=4)) |
			((ipFileType==3) & (GetFileType(ipFileName)==0)) | ((ipFileType==10) & (GetFileType(ipFileName)==0)) // Deals with special Case of SATRAM file type 
			)
		{
			if( (opFileType == GetFileType(ipFileName)) &
				experimental::filesystem::equivalent(ipDirectoryName, opDirectoryName))
			{
				cout<<"Processing: "<<ipFileName <<endl;
				string NewipFileName = RemoveExtension(ipFileName)+"_new";
				cout<<" =>Altering output file name to \'"+NewipFileName+"\' as to avoid overwriting itself."<<endl;
				
				reader.UpdateFileInput(ipDirectoryName +"/"+ ipFileName);
				writer.UpdateFileOutput(opDirectoryName+"/"+RemoveExtension(NewipFileName));
				
				WriteParticleInformation(filteredreader, writer, RemoveEdges);
			}
			else
			{
				cout<<"Processing: "<<ipFileName<<endl;
				reader.UpdateFileInput(ipDirectoryName +"/"+ ipFileName);
				writer.UpdateFileOutput(opDirectoryName+"/"+RemoveExtension(ipFileName));
				
				WriteParticleInformation(filteredreader, writer, RemoveEdges);
			}
		}
	}
	
}
