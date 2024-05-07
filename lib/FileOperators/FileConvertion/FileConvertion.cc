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

/*
Reads in all particles from "reader" and adds them to "writer" 

Essential reading from 1 file and writing to another

RemoveEdges set to True if you wish to remove all edge cluster from the output file 
*/
void WriteParticleInformation(unique_ptr<ParticleFileReader>&& reader, unique_ptr<ParticleFileWriter>&& writer, bool RemoveEdges)
{
	particle p;
	while(reader->AssignParticle(p))
	{
		if((p.IsEdge()==false) || (RemoveEdges==false))
			writer->AddParticle(p);
		
	}
}

/*
Takes in:
	ipFileName: The location of the file you wish read
	opFileName: The location where you wish to write the new particle file
	ipFileType: The type of file you wish to read, automatically by file ending ipFile is set to -1
	DectorThickness:
	RemoveEdges: set True if you wish to remove all edge cluster from the output file
	ModelPath: Specifies the path to the ML model you wish to calculate theta with (must a .onnx file), set to "" empty if you wish to implement no ML
	SelectionConfigFilePath: Specify the path to the configuration file where cluster cuts are defined i.e. Minimum size, Maximum Energy depostion, etc.

Reads the particle file at locatin ipFileName and outputs the new particle file to the location opFileName
*/
void ConvertTo(const string& ipFileName, const string& opFileName, int ipFileType, int opFileType, double DetectorThickness, bool RemoveEdges,
		 const string& ModelPath, const string& SelectionConfigFilePath)
{
	unique_ptr<ParticleFileReader> reader = GetEmptyFileReader(ipFileType);
	reader->AddParticleFiltersFromConfig(SelectionConfigFilePath);
	unique_ptr<ParticleFileWriter>  writer = GetEmptyFileWriter(opFileType, DetectorThickness, ModelPath);
	
	WriteParticleInformation(std::move(reader), std::move(writer), RemoveEdges);
}


/*
Takes in:
	ipDirectoryName: The location of the directory containing the particle files you wish read
	opDirectoryName: The location of the directory where you wish to write the new particles files
	ipFileType: The type of file you wish to read, automatically by file ending ipFile is set to -1
	DectorThickness:
	RemoveEdges: set True if you wish to remove all edge cluster from the output file
	ModelPath: Specifies the path to the ML model you wish to calculate theta with (must a .onnx file), set to "" empty if you wish to implement no ML
	SelectionConfigFilePath: Specify the path to the configuration file where cluster cuts are defined i.e. Minimum size, Maximum Energy depostion, etc.

Reads all particle files of type ipFileType found at locatin ipDirectoryName and outputs the new particle files to the location opDirectoryName
*/
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
		if(CheckFileType(ipFileName, ipFileType))
		{
			cout<<"Processing: "<<ipFileName <<endl;
			if( CheckFileType(ipFileName, opFileType) &
				filesystem::equivalent(ipDirectoryName, opDirectoryName)) // Prevents of overwriting of files by adding extension to the output file name
			{
				cout<<" =>Altering output file name to \'"+ipFileName+"_new\' as to avoid overwriting itself."<<endl;
				ipFileName = RemoveExtension(ipFileName)+"_new";
			}
			
			reader->UpdateFileInput(ipDirectoryName +"/"+ ipFileName);
			writer->UpdateFileOutput(opDirectoryName+"/"+RemoveExtension(ipFileName));
			
			WriteParticleInformation(std::move(reader), std::move(writer), RemoveEdges);
		}
	}
	
}
