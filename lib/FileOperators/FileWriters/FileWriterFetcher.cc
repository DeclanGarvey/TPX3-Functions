#include <iostream>
using namespace std;

#include "FileNames.h"
#include "ParticleFileWriters.h"
#include "MLStoppingPowerFileWriter.h"
#include "MLFieldTrackingFileWriter.h"
#include "HighZSearchFileWriter.h"

#include "FileWriterFetcher.h"

ParticleFileWriter* GetFileWriter(const string& opFileName, int FileType, double DetectorThickness, const string& ModelPath)
{
	if(FileType==-1)
		FileType = GetFileType(opFileName);
	if(ModelPath.empty())
	{
		switch(FileType)
		{
			case 1:
				return new PxFileWriter(opFileName, DetectorThickness);
				break;
			case 2:
				return new AngFileWriter(opFileName, DetectorThickness);
				break;
			case 5: 
				return new FeatFileWriter(opFileName, DetectorThickness);
				break;
			case 6:
				return new SpFileWriter(opFileName, DetectorThickness);
				break;
			case 7:
				return new SatFileWriter(opFileName, DetectorThickness);
				break;
			case 8:
				return new FieldTrackingFileWriter(opFileName, DetectorThickness);
				break;
			default:
				cout<<FileType<<" is an unknown data type(1:_px.txt, 2:_ang.txt, 5:_feat.txt, 6:_sp.txt)"<<endl;
				break;
		}
	}
	else
	{
		switch(FileType)
		{
			case 6:
				return new MLSpFileWriter(opFileName, DetectorThickness, ModelPath);
				break;
			case 8:
				return new MLFieldTrackingFileWriter(opFileName, DetectorThickness, ModelPath);
				break;
			case 9:
				return new HighZSearchFileWriter(opFileName, DetectorThickness, ModelPath);
				break;
			default:
				cout<<FileType<<" is an unknown ML data type(6:_sp.txt)"<<endl;
				break;
		}
	}
	return new ParticleFileWriter;
}

ParticleFileWriter* GetEmptyFileWriter(int FileType, double DetectorThickness, const string& ModelPath)
{
	if(ModelPath.empty())
	{
		switch(FileType)
		{
			case 1:
				return new PxFileWriter(DetectorThickness);
				break;
			case 2:
				return new AngFileWriter(DetectorThickness);
				break;
			case 5: 
				return new FeatFileWriter(DetectorThickness);
				break;
			case 6:
				return new SpFileWriter(DetectorThickness);
				break;
			case 7:
				return new SatFileWriter(DetectorThickness);
				break;
			case 8:
				return new FieldTrackingFileWriter(DetectorThickness);
				break;
			default:
				cout<<FileType<<" is an unknown data type(1:_px.txt, 2:_ang.txt, 5:_feat.txt, 6:_sp.txt)"<<endl;
				break;
		}
	}
	else
	{
		switch(FileType)
		{
			case 6:
				return new MLSpFileWriter(DetectorThickness, ModelPath);
				break;
			case 8:
				return new MLFieldTrackingFileWriter(DetectorThickness, ModelPath);
				break;
			case 9:
				return new HighZSearchFileWriter(DetectorThickness, ModelPath);
				break;
			default:
				cout<<FileType<<" is an unknown ML data type(6:_sp.txt)"<<endl;
				break;
		}
	}
	return new ParticleFileWriter;
}



