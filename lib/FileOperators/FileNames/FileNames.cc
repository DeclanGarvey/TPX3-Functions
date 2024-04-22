#include <string>
#include <stdio.h>
using namespace std;

#include "FileNames.h"

void ShowDataFileTypes()
{
	fprintf (stderr, "Possible Data File Types:\n\n");
	fprintf (stderr, "0	: .root, Standard root file type'\n");
	fprintf (stderr, "1	: _px.txt \n");
	fprintf (stderr, "2	: .root, SATRAM root file type',\n");
	fprintf (stderr, "3	: _sim.txt \n");
	fprintf (stderr, "4	: _feat.txt \n");
	fprintf (stderr, "5	: _sp.txt \n");
	fprintf (stderr, "6	: _sat.txt \n");
	fprintf (stderr, "7	: _tracking.txt\n");
	fprintf (stderr, "8	: _zsearch.txt\n");
	fprintf (stderr, "9	: .root, root file type created by Benny(MoEDAL,Simulation, etc.)',\n");
	fprintf (stderr, "\n\n");
	exit (1);
}


int GetFileType(const string& ipFileName)
{
	int FileType=-1;
	if(ipFileName.size()>5){
		if(ipFileName.substr(ipFileName.size()-5)==".root")
			FileType=0;
		else if(ipFileName.size()>7){
			if(ipFileName.substr(ipFileName.size()-7)=="_px.txt")
				FileType=1;
			else if(ipFileName.substr(ipFileName.size()-7)=="_sp.txt")
				FileType=5;
			else if(ipFileName.size()>8){
				if(ipFileName.substr(ipFileName.size()-8)=="_sim.txt")
					FileType=3;
				else if(ipFileName.substr(ipFileName.size()-8)=="_sat.txt")
					FileType=6;
				else if(ipFileName.size()>9){
					if(ipFileName.substr(ipFileName.size()-9)=="_feat.txt")
						FileType=4;
					else if(ipFileName.size()>12){
						if(ipFileName.substr(ipFileName.size()-12)=="_zsearch.txt")
							FileType=8;
						else if(ipFileName.size()>13){
							if(ipFileName.substr(ipFileName.size()-13)=="_tracking.txt")
								FileType=7;
						}
					}
				}
			}
		}
	}
	return FileType;
}



string RemoveExtension(const string& FileName)
{
	if(FileName.size()>5){
		if(FileName.substr(FileName.size()-5)==".root")
			return FileName.substr(0,FileName.size()-5);
		else if(FileName.size()>7){
			if(FileName.substr(FileName.size()-7)=="_px.txt")
				return FileName.substr(0,FileName.size()-7);
			else if(FileName.substr(FileName.size()-7)=="_sp.txt")
				return FileName.substr(0,FileName.size()-7);
			else if(FileName.size()>8){
				if(FileName.substr(FileName.size()-8)=="_sim.txt")
					return FileName.substr(0,FileName.size()-8);
				else if(FileName.substr(FileName.size()-8)=="_sat.txt")
					return FileName.substr(0,FileName.size()-8);
				else if(FileName.size()>9){
					if(FileName.substr(FileName.size()-9)=="_feat.txt")
						return FileName.substr(0,FileName.size()-9);
					else if(FileName.size()>12){
						if(FileName.substr(FileName.size()-12)=="_zsearch.txt")
							return FileName.substr(0,FileName.size()-12);
						else if(FileName.size()>13){
							if(FileName.substr(FileName.size()-13)=="_tracking.txt")
								return FileName.substr(0,FileName.size()-13);
						}
					}
				}
			}
		}
	}
	return FileName;
}

string RemoveDirectoryPath(const string& FileName)
{
	for(int i = FileName.length()-1; i>=0; i--)
	{
		if(FileName[i]=='/')
			return FileName.substr(i+1);
	}
	return FileName;
}



