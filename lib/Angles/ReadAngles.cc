#include <assert.h>
#include <string>

//#include "TCollection.h"
#include "ParticleFileReaders.h"
#include "FileNames.h" 

#include "Particle.h"
#include "PixelStructure.h"
#include "AngleCalulationFunctions.h"
#include "ReadAngles.h"
using namespace std;

void CalculateAngles(const string& ipFileName, const string& opFileName, int ipFileType, double DetectorThickness, int theta_method, int phi_method, bool RemoveEdges)
{
	FILE* opf;
	if (opFileName.empty())
		opf = fopen(opFileName.c_str(), "w");
	else 
		opf = stdout;
	
	particle p;	 		//empty particle class
	double theta, phi;
	ParticleFileReader reader;
	if(ipFileType==-1)
		ipFileType = GetFileType(ipFileName);
	reader = GetFileReader(ipFileName, ipFileType);
	while(reader.AssignParticle(p))
	{	
		if( (p.IsEmpty()==false) & ((p.IsEdge()==false) | (RemoveEdges==false)) )
		{	
			switch (theta_method)
			{
				case 0: 
					theta = ThetaLLM(p, DetectorThickness);
					break;   
				case 1:
					theta = ThetaImprovedLLM(p, DetectorThickness);
					break;   
				case 2:
					theta = ThetaMinMax(p, DetectorThickness);
					break; 
				case 3:
					theta = ThetaLineFit(p, DetectorThickness);
					break; 	
				case -1:
					theta = -1;
					break;
				default:
					fprintf (stderr, "Unrecognised theta_method [-%c]\n", theta_method);
					break;
			}
			
			switch (phi_method)
			{
				case 0:
					phi = PhiLLMHori(p);
					break;   
				case 1:
					phi = PhiImprovedLLM(p);
					break;   
				case 2:
					phi = PhiMinMax(p);
					break; 
				case 3:
					phi = PhiLineFit(p);
					break; 	
				case 4:
					phi = PhiTimeWeighted(p);
					break; 	
				case -1:
					phi = -1;
					break;
				default:
					fprintf (stderr, "Unrecognised theta_method [-%c]\n", theta_method);
					break;
			}
			
			fprintf(opf, "%lf, %lf\n", theta, phi); //Outputs calculated theta and phi information
		}
		p.Clear(); //Clears all data and resets all variables associated with the current particles
	}
}
