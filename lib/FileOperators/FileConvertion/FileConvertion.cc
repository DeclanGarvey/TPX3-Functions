#include <string>
#include <cstdint>
#include <filesystem> 
using namespace std;

#include "Particle.h"
#include "DirectoryReaders.h"
#include "ParticleFileReaderFetcher.h"
#include "ParticleFileReaders.h"

#include "FileWriterFetcher.h"
#include "ParticleFileWriters.h"
#include "MLParticleFileWriters.h"
#include "FileNames.h"

#include "FileConvertion.h"

void WriteParticleInformation(unique_ptr<ParticleFileReader>&& reader, unique_ptr<ParticleFileWriter>&& writer, bool RemoveEdges)
{
	particle p;
	while(reader->AssignParticle(p))
	{
		if((p.IsEdge()==false) || (RemoveEdges==false))
			writer->AddParticle(p);
		
	}
}

void ConvertTo(const string& ipFileName, const string& opFileName, int ipFileType, int opFileType, double DetectorThickness, bool RemoveEdges,
		 const string& ModelPath, const string& SelectionConfigFilePath)
{
	unique_ptr<ParticleFileReader> reader = GetEmptyFileReader(ipFileType);
	reader->AddParticleFiltersFromConfig(SelectionConfigFilePath);
	unique_ptr<ParticleFileWriter>  writer = GetEmptyFileWriter(opFileType, DetectorThickness, ModelPath);
	
	WriteParticleInformation(std::move(reader), std::move(writer), RemoveEdges);
}

void ConvertDirectoryTo(const string& ipDirectoryName, const string& opDirectoryName, int ipFileType, int opFileType, double DetectorThickness, bool RemoveEdges,
		 	const string& ModelPath, const string& SelectionConfigFilePath)
{
	string ipFileName;
	unique_ptr<ParticleFileReader> reader = GetEmptyFileReader(ipFileType);
	reader->AddParticleFiltersFromConfig(SelectionConfigFilePath);
	unique_ptr<ParticleFileWriter>  writer = GetEmptyFileWriter(opFileType, DetectorThickness, ModelPath);
	
	
	DirectoryReader dir(ipDirectoryName);
	while(dir.AssignNextFile(ipFileName))
	{
		if( ((ipFileType!=-1) & (GetFileType(ipFileName)==ipFileType)) || 
			((ipFileType==-1) & (GetFileType(ipFileName)>=0) & (GetFileType(ipFileName)<=4)) ||
			((ipFileType==2) & (GetFileType(ipFileName)==0)) || ((ipFileType==9) & (GetFileType(ipFileName)==0)) // Deals with special Case of SATRAM file type 
			)
		{
			if( (opFileType == GetFileType(ipFileName)) &
				filesystem::equivalent(ipDirectoryName, opDirectoryName))
			{
				cout<<"Processing: "<<ipFileName <<endl;
				string NewipFileName = RemoveExtension(ipFileName)+"_new";
				cout<<" =>Altering output file name to \'"+NewipFileName+"\' as to avoid overwriting itself."<<endl;
				
				reader->UpdateFileInput(ipDirectoryName +"/"+ ipFileName);
				writer->UpdateFileOutput(opDirectoryName+"/"+RemoveExtension(NewipFileName));
				
				WriteParticleInformation(std::move(reader), std::move(writer), RemoveEdges);
			}
			else
			{
				cout<<"Processing: "<<ipFileName<<endl;
				reader->UpdateFileInput(ipDirectoryName +"/"+ ipFileName);
				writer->UpdateFileOutput(opDirectoryName+"/"+RemoveExtension(ipFileName));
				
				WriteParticleInformation(std::move(reader), std::move(writer), RemoveEdges);
			}
		}
	}
	
}
