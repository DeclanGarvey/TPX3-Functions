
#pragma once

#include <string>

class TList; 
class TIter;
class TSystemFile;

class particle;

class DirectoryReader 
{
	private:
		std::string ipDirectory_;
		TList *FileList_; 
		TIter* Iterator_;
		TSystemFile *CurrentFile_;
		bool IsEmpty_;
	public:
		DirectoryReader(const std::string& ipDirectoryName);
		bool AssignNextFile(std::string& FileName);
		bool IsEmpty(){return IsEmpty_;}
		void RestartIteration();
};

