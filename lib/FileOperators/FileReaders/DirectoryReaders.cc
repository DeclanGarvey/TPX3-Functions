using namespace std;
#include <filesystem>
#include <iostream>
#include "DirectoryReaders.h"

DirectoryReader::DirectoryReader(const string& ipDirectoryName)
{
	ipDirectoryName_=ipDirectoryName;
	FileExists = filesystem::exists(ipDirectoryName);
	if (FileExists) 
    	{
    		DirectoryIterator_ =  filesystem::directory_iterator(ipDirectoryName_);
    	}
    	else
	{
		std::cout << "Directory does not exist: " << ipDirectoryName << std::endl;
    	}
    	
}
bool DirectoryReader::AssignNextFile(string& FileName)
{
	do
	{
		if(DirectoryIterator_!= filesystem::directory_iterator())
		{
				CurrentFileEntry_ = *DirectoryIterator_;
				FileName = CurrentFileEntry_.path().filename();
				DirectoryIterator_++;
		}
		else
		{
			return false;
		}
	}
	while((FileName==".") | (FileName==".."));
	return true;
}
void DirectoryReader::RestartIteration()
{
	if(FileExists)
		DirectoryIterator_ =  filesystem::directory_iterator(ipDirectoryName_);
}


