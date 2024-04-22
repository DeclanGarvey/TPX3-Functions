using namespace std;

#include "ParticleFileReaders.h"
#include "FileNames.h"
#include "ParticleFileReaderFetcher.h"


ParticleFileReader GetFileReader(const string& ipFileName, int ipFileType)
{
	if(ipFileType==-1)
		ipFileType = GetFileType(ipFileName);
	switch(ipFileType)
	{
		case 0:
			return ROOTFileReader(ipFileName);
			break;
		case 1:
			return PxFileReader(ipFileName);
			break;
		case 2:
			return SATRAMFileReader(ipFileName);
			break;
		case 3:
			return SimFileReader(ipFileName);
			break;
		case 9:
			return BennyFileReader(ipFileName);
			break;
		default:
			cout<<ipFileType<<" is an unknown data type(0: .root(standard), 1:_px.txt, 2:_ang.txt, 3: .root(SATRAM), 4:_sim.txt)"<<endl;
			break;
	}
	return ParticleFileReader();
}

ParticleFileReader GetEmptyFileReader(int ipFileType)
{
	switch(ipFileType)
	{
		case 0:
			return ROOTFileReader();
			break;
		case 1:
			return PxFileReader();
			break;
		case 2:
			return SATRAMFileReader();
			break;
		case 3:
			return SimFileReader();
			break;
		case 9:
			return BennyFileReader();
			break;
		default:
			cout<<ipFileType<<" is an unknown data type(0: .root(standard), 1:_px.txt, 2:_ang.txt, 3: .root(SATRAM), 4:_sim.txt)"<<endl;
			break;
	}
	return ParticleFileReader();
}

