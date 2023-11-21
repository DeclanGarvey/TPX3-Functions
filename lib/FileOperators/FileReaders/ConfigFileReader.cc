#include <fstream>
using namespace std;

#include "ConfigFileReader.h"

ConfigFileReader::ConfigFileReader(string const& ipFileName)
{
	ipFile.open(ipFileName);
	if(ipFile.good()==false)
		cerr<<"Read Error: Config file could not be opened."<<endl;
}

bool ConfigFileReader::ProcessNextVariable()
{
	if(getline(ipFile, CurrentLine))
	{
		size_t DelimiterPos = CurrentLine.find("=");
		if(DelimiterPos!=string::npos)
		{
			VariableName = CurrentLine.substr(0, DelimiterPos);
			VariableValue = CurrentLine.substr(DelimiterPos + 1);
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


