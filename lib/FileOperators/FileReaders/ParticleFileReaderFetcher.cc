using namespace std;

#include "ParticleFileReaders.h"
#include "FileNames.h"
#include "ParticleFileReaderFetcher.h"


unique_ptr<ParticleFileReader> GetFileReader(const string& ipFileName, int ipFileType)
{
	if(ipFileType==-1)
		ipFileType = GetFileType(ipFileName);
	switch(ipFileType)
	{
		case 0:
			return make_unique<ROOTFileReader>(ipFileName);
			break;
		case 1:
			return make_unique<PxFileReader>(ipFileName);
			break;
		case 2:
			return make_unique<SATRAMFileReader>(ipFileName);
			break;
		case 3:
			return make_unique<SimFileReader>(ipFileName);
			break;
		case 9:
			return make_unique<BennyFileReader>(ipFileName);
			break;
		default:
			cout<<ipFileType<<" is an unknown data type(0: .root(standard), 1:_px.txt, 2:_ang.txt, 3: .root(SATRAM), 4:_sim.txt)"<<endl;
			break;
	}
	return make_unique<ParticleFileReader>();
}

unique_ptr<ParticleFileReader> GetEmptyFileReader(int ipFileType)
{
	switch(ipFileType)
	{
		case 0:
			return make_unique<ROOTFileReader>();
			break;
		case 1:
			return make_unique<PxFileReader>();
			break;
		case 2:
			return make_unique<SATRAMFileReader>();
			break;
		case 3:
			return make_unique<SimFileReader>();
			break;
		case 9:
			return make_unique<BennyFileReader>();
			break;
		default:
			cout<<ipFileType<<" is an unknown data type(0: .root(standard), 1:_px.txt, 2:_ang.txt, 3: .root(SATRAM), 4:_sim.txt)"<<endl;
			break;
	}
	return make_unique<ParticleFileReader>();
}

