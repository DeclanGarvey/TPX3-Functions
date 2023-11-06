#include <iostream>
using namespace std;

#include "FileNames.h"
#include "ParticleFileWriters.h"
#include "MLStoppingPowerFileWriter.h"
#include "MLFieldTrackingFileWriter.h"

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
			default:
				cout<<opFileName<<" is an unknown data type(1:_px.txt, 2:_ang.txt, 5:_feat.txt, 6:_sp.txt)"<<endl;
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
			default:
				cout<<opFileName<<" is an unknown ML data type(6:_sp.txt)"<<endl;
				break;
		}
	}
	return new ParticleFileWriter;
}


