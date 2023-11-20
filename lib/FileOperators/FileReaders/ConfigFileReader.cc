#include <fstream>
using namespace std;

#include "ConfigFileReader.h"

ConfigFileReader::ConfigFileReader(string const& ipFileName)
{
	ipFile.open(FileName);
	if(ipFile.good())
		ProcessNextVariable();
	else
		cerr<<"Read Error: Config file could not be opened."<<endl;
}

bool ConfigFileReader::ProcessNextVariable()
{
	if(getline(ipFile, CurrentLine))
	{
		int DelimiterPos = line.find("=");
		if(DelimiterPos!=string::npos)
		{
			string VariableName = line.substr(0, delimiterPos);
			string VariableValue = line.substr(delimiterPos + 1);
		}
		else 
			cout<<"Line = \'"<<CurrentLine<<"\' does not containt \'=\' and is being ommited."<<endl;
		return true;
	}
	else
		return false;
}

string ConfigFileReader::GetCurrentVariableName()
{
	return VariableName;
}


string ConfigFileReader::GetCurrentVariableValue()
{
	return VariableValue;
}


