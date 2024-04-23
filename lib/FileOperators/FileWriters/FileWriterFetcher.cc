#include <iostream>
using namespace std;

#include "FileNames.h"
#include "ParticleFileWriters.h"
#include "RandomForrestTheta.h"
#include "MLParticleFileWriters.h"

#include "FileWriterFetcher.h"

unique_ptr<ParticleFileWriter> GetFileWriter(const string& opFileName, int FileType, double DetectorThickness, const string& ModelPath)
{
	if(FileType==-1)
		FileType = GetFileType(opFileName);
	if(ModelPath.empty())
	{
		switch(FileType)
		{
			case 1:
				return make_unique<PxFileWriter>(opFileName, DetectorThickness);
				break;
			case 4: 
				return make_unique<FeatFileWriter>(opFileName, DetectorThickness);
				break;
			case 5:
				return make_unique<SpFileWriter>(opFileName, DetectorThickness);
				break;
			case 6:
				return make_unique<SatFileWriter>(opFileName, DetectorThickness);
				break;
			case 7:
				return make_unique<FieldTrackingFileWriter>(opFileName, DetectorThickness);
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
			case 5:
				return make_unique<MLSpFileWriter>(opFileName, DetectorThickness, ModelPath);
				break;
			case 7:
				return make_unique<MLFieldTrackingFileWriter>(opFileName, DetectorThickness, ModelPath);
				break;
			case 8:
				return make_unique<HighZSearchFileWriter>(opFileName, DetectorThickness, ModelPath);
				break;
			default:
				cout<<FileType<<" is an unknown ML data type(6:_sp.txt)"<<endl;
				break;
		}
	}
	return make_unique<ParticleFileWriter>();
}

unique_ptr<ParticleFileWriter> GetEmptyFileWriter(int FileType, double DetectorThickness, const string& ModelPath)
{
	if(ModelPath.empty())
	{
		switch(FileType)
		{
			case 1:
				return make_unique<PxFileWriter>(DetectorThickness);
				break;
			case 4: 
				return make_unique<FeatFileWriter>(DetectorThickness);
				break;
			case 5:
				return make_unique<SpFileWriter>(DetectorThickness);
				break;
			case 6:
				return make_unique<SatFileWriter>(DetectorThickness);
				break;
			case 7:
				return make_unique<FieldTrackingFileWriter>(DetectorThickness);
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
			case 5:
				return make_unique<MLSpFileWriter>(DetectorThickness, ModelPath);
				break;
			case 7:
				return make_unique<MLFieldTrackingFileWriter>(DetectorThickness, ModelPath);
				break;
			case 8:
				return make_unique<HighZSearchFileWriter>(DetectorThickness, ModelPath);
				break;
			default:
				cout<<FileType<<" is an unknown ML data type(6:_sp.txt)"<<endl;
				break;
		}
	}
	return make_unique<ParticleFileWriter>();
}



