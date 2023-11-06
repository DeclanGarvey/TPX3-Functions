using namespace std;

#include "TSystemDirectory.h"
#include "TFile.h"

#include "DirectoryReaders.h"

DirectoryReader::DirectoryReader(const string& ipDirectoryName)
{
	TSystemDirectory dir((ipDirectoryName).c_str(), (ipDirectoryName).c_str());
	
	FileList_ = dir.GetListOfFiles(); 
	if (FileList_) 
	{
		IsEmpty_=false;
		Iterator_ = new TIter(FileList_);  
	}else{
		IsEmpty_=true;
	}
}
bool DirectoryReader::AssignNextFile(string& FileName)
{
	do
	{
		if( (CurrentFile_=(TSystemFile*)Iterator_->operator()()) )
		{
				//cout<<FileName<<" "<<(FileName!=".") <<" "<<(FileName!="..")<<endl;
				FileName = CurrentFile_->GetName();
		}
		else
		{
			FileName.clear();
			return false;
		}
	}
	while((FileName==".") | (FileName==".."));
	return true;
}
void DirectoryReader::RestartIteration()
{
	if (FileList_) 
		Iterator_ = new TIter(FileList_);  
}


