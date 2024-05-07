using namespace std;
#include <filesystem>
#include <iostream>
#include "DirectoryReaders.h"

/*
DirectoryReader iterates through all the files found in a directory at runtime outputing the File names within that directory
*/


/*
Constructs the DirectoryReader that iterates through all the filenames found at location ipDirectoryName
*/
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

/*
Reads the next file name found in the directory and assigns the file name to the variable FileName 
*/
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

/*
Brings the iterator back the begining of the directory allowing for another iteration through the file names
*/
void DirectoryReader::RestartIteration()
{
	if(FileExists)
		DirectoryIterator_ =  filesystem::directory_iterator(ipDirectoryName_);
}


