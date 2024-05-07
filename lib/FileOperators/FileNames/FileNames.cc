#include <string>
#include <stdio.h>
using namespace std;

#include "FileNames.h"

/*
Prints the numbers that correpond to each file type 

These numbers are typical written into the terminal at run time following the flags -f for input file type and -t for output file type
*/
void ShowDataFileTypes()
{
	fprintf (stdout, "Possible Data File Types:\n\n");
	fprintf (stdout, "0   : .root, Standard root file type'\n");
	fprintf (stdout, "1   : _px.txt \n");
	fprintf (stdout, "2   : .root, SATRAM root file type',\n");
	fprintf (stdout, "3   : _sim.txt \n");
	fprintf (stdout, "4   : _feat.txt \n");
	fprintf (stdout, "5   : _sp.txt \n");
	fprintf (stdout, "6   : _sat.txt \n");
	fprintf (stdout, "7   : _tracking.txt\n");
	fprintf (stdout, "8   : _zsearch.txt\n");
	fprintf (stdout, "9   : .root, root file type created by Benny(MoEDAL,Simulation, etc.)',\n");
	fprintf (stdout, "\n\n");
	exit (1);
}

/*
Takes a filename and outputs the particle of on the filename ending

Note: SATRAM and Benny files detected as standard Root files 
*/
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


/*
Outputs file name with thethat specifys the file type i.e. _px.txt, _sim.txt, etc. removed
*/
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

/*
Output the FileName with the directory path removed
*/
string RemoveDirectoryPath(const string& FileName)
{
	for(int i = FileName.length()-1; i>=0; i--)
	{
		if(FileName[i]=='/')
			return FileName.substr(i+1);
	}
	return FileName;
}
/*
Checks ending of FileName that the specifis the file type corresponds to the FileType
*/
bool CheckFileType(const string& FileName, int FileType)
{
	return (
	((FileType!=-1) & (GetFileType(FileName)==FileType)) //File name match the file type specified
	|| 
	((FileType==-1) & (GetFileType(FileName)>=0) & (GetFileType(FileName)<=4)) //FileType is unspecified and any file type is excepted
	|| 
	(FileType==2) & (GetFileType(FileName)==0) //Deals case where SATRAM filetype is specified as it has the same ending a ROOT file types (.root)
	|| 
	((FileType==9) & (GetFileType(FileName)==0)) //Deals case where Benny filetype is specified as it has the same ending a ROOT file types (.root)
	);
}




