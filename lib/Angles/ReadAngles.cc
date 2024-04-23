#include <assert.h>
#include <string>

#include "ParticleFileReaderFetcher.h"
#include "ParticleFileReaders.h"
#include "FileNames.h" 

#include "Particle.h"
#include "PixelStructure.h"
#include "AngleCalulationFunctions.h"
#include "ReadAngles.h"
using namespace std;

/*
Prints available angle calculation functions developed for Improved algorithms for determination of particle directions in space with Timepix3, Journal of Instrumentation https://arxiv.org/pdf/2111.00624.pdf
*/
void ShowAngleCalculationMethods()
{
	fprintf (stderr, "Possible Theta Calculation Methods:\n\n");
	fprintf (stderr, "0	: Simple LLM Algorithm'\n");
	fprintf (stderr, "1	: Improved LLM Algorithm \n");
	fprintf (stderr, "2	: Min-Max Algorithm'\n");
	fprintf (stderr, "3	: ToA Gradient Algorithm\n");
	fprintf (stderr, "-1	: Theta not calculated \n");
	fprintf (stderr, "\n");
	fprintf (stderr, "Possible Phi Calculation Methods:\n\n");
	fprintf (stderr, "0	: Simple LLM Algorithm'\n");
	fprintf (stderr, "1	: Improved LLM Algorithm \n");
	fprintf (stderr, "2	: Min-Max Algorithm'\n");
	fprintf (stderr, "3	: ToA Gradient Algorithm\n");
	fprintf (stderr, "4	: Time-weighted averaging\n");
	fprintf (stderr, "-1	: Phi not calculated \n");
	fprintf (stderr, "\n\n");
	fprintf (stderr, "Algorithms based on Improved algorithms for determination of particle directions in space with Timepix3,\n");
	fprintf (stderr, "Journal of Instrumentation https://arxiv.org/pdf/2111.00624.pdf\n\n");

	exit (1);
}
/*
Functions takees in an 
1: input file path 
2: output file path
3: File type of input based on /lib/fileoperators/FileNames/FileNames.cc
4: Detector thickness in cm,  0.05 cm is used
5: Theta calculation method
6: Phi calculation method
*/
void CalculateAngles(const string& ipFileName, const string& opFileName, int ipFileType, double DetectorThickness, int theta_method, int phi_method, bool RemoveEdges)
{
	FILE* opf;
	if (opFileName.empty())
		opf = fopen(opFileName.c_str(), "w");
	else 
		opf = stdout;
	
	particle p;	 		//empty particle class
	double theta, phi;
	
	unique_ptr<ParticleFileReader> reader = GetFileReader(ipFileName, ipFileType);
	while(reader->AssignParticle(p))
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
