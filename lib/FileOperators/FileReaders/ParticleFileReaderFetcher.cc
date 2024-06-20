using namespace std;

#include "ParticleFileReaders.h"
#include "FileNames.h"
#include "ParticleFileReaderFetcher.h"

/*
Creates a particular instance of particle file reader 
	ipFileName: Specifies the location of the filename that intended to read
	ipFileType: Specifies partcular type of file reader needed if set to -1, code attempts to detect file type based of on file name ending, Note: doesn't work SATRAM and Benny filetypes
*/
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
		case 8:
			return make_unique<BennyFileReader>(ipFileName);
			break;
		default:
			cout<<ipFileType<<" is an unknown data type(0: .root(standard), 1:_px.txt, 2:_ang.txt, 3: .root(SATRAM), 4:_sim.txt)"<<endl;
			break;
	}
	return make_unique<ParticleFileReader>();
}

/*
Creates an empty particle file reader the particular type of file reader is specified by ipFileType
*/
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
		case 8:
			return make_unique<BennyFileReader>();
			break;
		default:
			cout<<ipFileType<<" is an unknown data type(0: .root(standard), 1:_px.txt, 2:_ang.txt, 3: .root(SATRAM), 4:_sim.txt)"<<endl;
			break;
	}
	return make_unique<ParticleFileReader>();
}

