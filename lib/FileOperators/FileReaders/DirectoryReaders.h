
#pragma once
#include <string>
#include <filesystem>

class particle;

class DirectoryReader 
{
	private:
		std::string ipDirectoryName_;
		std::filesystem::directory_iterator DirectoryIterator_; 
		std::filesystem::directory_entry CurrentFileEntry_;
		bool FileExists=false;
	public:
		DirectoryReader(const std::string& ipDirectoryName);
		bool AssignNextFile(std::string& FileName);
		void RestartIteration();
};

