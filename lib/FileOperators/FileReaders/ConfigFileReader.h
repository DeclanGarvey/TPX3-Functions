#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class ConfigFileReader 
{
	private:
		std::ifstream ipFile;
		std::string CurrentLine;
		std::string VariableName;
		std::string VariableValue;
		std::vector<std::string> VariableValueVector;
	public:
		ConfigFileReader(std::string const& ipFileName);
		bool ProcessNextVariable();
		const std::string& GetCurrentVariableName();
		const std::string& GetCurrentVariableValue();
		const std::vector<std::string>& GetCurrentVariableAsVector();
		const std::vector<string>& GetCurrentVariableAsVector(char Seperator);
		void Close() {ipFile.close();};
};

