#include <fstream>
#include <vector>
#include <string>
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
	string CommentRemovedLine; 
	while(CommentRemovedLine.empty())
	{
		if(getline(ipFile, CurrentLine))
		{
			size_t DelimiterPos = CurrentLine.find("#");
			CommentRemovedLine = CurrentLine.substr(0, DelimiterPos);
			
			if(CommentRemovedLine.find_first_not_of(" ") != std::string::npos)
			{
				DelimiterPos = CommentRemovedLine.find("=");
				if(DelimiterPos!=string::npos)
				{
					VariableName = CommentRemovedLine.substr(0, DelimiterPos);
					VariableValue = CommentRemovedLine.substr(DelimiterPos + 1);
					VariableValueVector.clear();
				}
				else 
					cout<<"Line = \'"<<CurrentLine<<"\' does not containt \'=\' and is being ommited."<<endl;
			}
		}
		else
			return false;
	}
	return true;
}

const string& ConfigFileReader::GetCurrentVariableName()
{
	return VariableName;
}


const string& ConfigFileReader::GetCurrentVariableValue()
{
	return VariableValue;
}

const std::vector<string>& ConfigFileReader::GetCurrentVariableAsVector()
{
	if(VariableValueVector.empty())
	{
		string buffer{""};
		for(auto n : VariableValue)
		{
			if((n==',') & (n!=' ')) 
				buffer+=n;
			else if (((n == ',') | (n==' ')) & (buffer != "")) 
			{
				VariableValueVector.push_back(buffer); 
				buffer = ""; 
			}
		}
	}
	return VariableValueVector;
}

