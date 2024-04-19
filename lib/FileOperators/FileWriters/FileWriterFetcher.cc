#include <iostream>
using namespace std;

#include "FileNames.h"
#include "ParticleFileWriters.h"
#include "MLStoppingPowerFileWriter.h"
#include "RandomForrestTheta.h"
#include "MLFieldTrackingFileWriter.h"
#include "HighZSearchFileWriter.h"

#include "FileWriterFetcher.h"

ParticleFileWriter GetFileWriter(const string& opFileName, int FileType, double DetectorThickness, const string& ModelPath)
{
	if(FileType==-1)
		FileType = GetFileType(opFileName);
	if(ModelPath.empty())
	{
		switch(FileType)
		{
			case 1:
				return PxFileWriter(opFileName, DetectorThickness);
				break;
			case 2:
				return AngFileWriter(opFileName, DetectorThickness);
				break;
			case 5: 
				return FeatFileWriter(opFileName, DetectorThickness);
				break;
			case 6:
				return SpFileWriter(opFileName, DetectorThickness);
				break;
			case 7:
				return SatFileWriter(opFileName, DetectorThickness);
				break;
			case 8:
				return FieldTrackingFileWriter(opFileName, DetectorThickness);
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
				return MLSpFileWriter(opFileName, DetectorThickness, ModelPath);
				break;
			case 8:
				return MLFieldTrackingFileWriter(opFileName, DetectorThickness, ModelPath);
				break;
			case 9:
				return HighZSearchFileWriter(opFileName, DetectorThickness, ModelPath);
				break;
			default:
				cout<<FileType<<" is an unknown ML data type(6:_sp.txt)"<<endl;
				break;
		}
	}
	return ParticleFileWriter();
}

ParticleFileWriter GetEmptyFileWriter(int FileType, double DetectorThickness, const string& ModelPath)
{
	if(ModelPath.empty())
	{
		switch(FileType)
		{
			case 1:
				return PxFileWriter(DetectorThickness);
				break;
			case 2:
				return AngFileWriter(DetectorThickness);
				break;
			case 5: 
				return FeatFileWriter(DetectorThickness);
				break;
			case 6:
				return SpFileWriter(DetectorThickness);
				break;
			case 7:
				return SatFileWriter(DetectorThickness);
				break;
			case 8:
				return FieldTrackingFileWriter(DetectorThickness);
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
				return MLSpFileWriter(DetectorThickness, ModelPath);
				break;
			case 8:
				return MLFieldTrackingFileWriter(DetectorThickness, ModelPath);
				break;
			case 9:
				return HighZSearchFileWriter(DetectorThickness, ModelPath);
				break;
			default:
				cout<<FileType<<" is an unknown ML data type(6:_sp.txt)"<<endl;
				break;
		}
	}
	return ParticleFileWriter();
}



