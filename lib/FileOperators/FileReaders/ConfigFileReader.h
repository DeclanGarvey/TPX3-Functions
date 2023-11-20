#pragma once
#include <fstream>
#include <string>

class ConfigFileReader 
{
	private:
		std::ifstream ipFile;
		std::string CurrentLine;
		std::string VariableName;
		std::string VariableValue;
	public:
		ConfigFileReader(std::string const& ipFileName);
		bool ProcessNextVariable();
		std::string GetCurrentVariableName();
		std::string GetCurrentVariableValue();
		void Close() {ipFile.close()};
};

