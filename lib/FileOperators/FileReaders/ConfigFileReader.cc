#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include "ConfigFileReader.h"

/*
ConfigFileReader a particular text file type where you it reads in each line in the following format
VariableName: VariableValue

anything on a line following a "#" is ignored as a comment
*/



/*
Constructs the ConfigFileReader object by opening the text file found at the location ipFileName
*/
ConfigFileReader::ConfigFileReader(string const& ipFileName)
{
	ipFile.open(ipFileName);
	if(ipFile.good()==false)
		cerr<<"Read Error: Config file could not be opened."<<endl;
}


/*
Reads the next line fo the text file
*/
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

/*
Returns the VariableName of the current line in the config text file
*/
const string& ConfigFileReader::GetCurrentVariableName()
{
	return VariableName;
}

/*
Returns the VariableValue of the current line in the config text file
*/
const string& ConfigFileReader::GetCurrentVariableValue()
{
	return VariableValue;
}

/*
Constructs and returns a string vector from the VariableValue
used when VariableVale is of the form
%s,%s,%s,...
or 
%s %s %s ...

the specific seperator type is whatever is found VariableValue
*/
const std::vector<string>& ConfigFileReader::GetCurrentVariableAsVector()
{
	if(VariableValueVector.empty())
	{
		string buffer{""};
		for(auto n : VariableValue)
		{
			if((n!=',') & (n!=' ')) 
				buffer+=n;
			else if (((n == ',') | (n==' ')) & (buffer != ""))
			{
				VariableValueVector.push_back(buffer); 
				buffer = ""; 
			}
		}
		if(buffer != "")
			VariableValueVector.push_back(buffer); 
	}
	return VariableValueVector;
}

/*
Constructs and returns a string vector from the VariableValue
used when VariableVale is of the form
%s,%s,%s,...
or 
%s %s %s ...

the specific seperator type is take as input all other seperator types are not detected
*/
const std::vector<string>& ConfigFileReader::GetCurrentVariableAsVector(char Seperator)
{
	if(VariableValueVector.empty())
	{
		string buffer{""};
		for(auto n : VariableValue)
		{
			if(n!=Seperator) 
				buffer+=n;
			else if ((n == Seperator) & (buffer != "")) 
			{
				VariableValueVector.push_back(buffer); 
				buffer = ""; 
			}
		}
		if(buffer != "")
			VariableValueVector.push_back(buffer); 
	}
	return VariableValueVector;
}
