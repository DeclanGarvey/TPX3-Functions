#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include <string>
#include "ReadAngles.h"
#include "FileNames.h"
using namespace std;
/* Local Definitions */
#define MODULE_NAME    "Angle Calculations"
#define MODULE_VERSION "1.0"


static void
sigInterrupt (int sigval)
{
    printf("\nExiting: sigInterrupt %d\n\n", sigval);
	exit(sigval);
}




/*
* usage
* Print out the module usage
*/
static void
usage (void)
{
	fprintf (stderr, "\n\n");
	fprintf (stderr, "-h/?          : Help page.\n");
	fprintf (stderr, "-o <filename> : Output filename. If omitted, it will be output stdout.\n");
	fprintf (stderr, "-t <int>      : Theta method, default=1.{0:'llm',1:'improved llm', 2:'min-max time method', 3:'line fit method', -1:'omit'}\n");
	fprintf (stderr, "-p <int>      : Phi method, default=4.  {0:'llm',1:'improved llm', 2:'min-max time method', 3:'line fit method',\n");
	fprintf (stderr, "		  4:'time-weighted entry/exit method, -1:'omit'}\n");
	fprintf (stderr, "-f <int>      : Specify the input file type, default set according to file ending. {0: ''}\n");
	fprintf (stderr, "-z <double>	: Specify Detector Thickness, default 0.05 cm.\n");
	fprintf (stderr, "-e		: Flag indicates to keep edge clusters in output, by default they are removed.\n");
	fprintf (stderr, "-a            : Show available angle calculation methods for phi and theta. \n");
	fprintf (stderr, "-s 		: Show data file types.\n");
	fprintf (stderr, "\n");
	fprintf (stderr, "Declare input file after all parameters are specified.(input file must be in standard _px.txt format)\n\n");
	exit (1);
}

/*
Main entry point.
*/
int main (int argc, const char *argv[])
{
	int opt; /* Local loop counter */
	
	string opFileName; /* output file name */
	int ipFileType; /* Input file type */
	string ipFileName; /* Input file name */
	
	int theta_method=1, phi_method=4; /*Decides the phi and theta calculation method defaulting to improved llm and time weighted methods*/
	double DetectorThickness = 0.05; /* Detector thickness used in calcalations defaults to 0.05 cm*/
	bool RemoveEdges = true;
	
	/* Set up the signal handler */
	(void)signal (SIGINT, sigInterrupt);
	
	/* Collect the arguments from the command line */
	while ((opt = getopt (argc, (char * const*)argv, "?ho:t:p:f:z:eas")) != EOF)
	{
		/* Process the arguments */
		switch (opt)
		{
			case 'h':
			case '?': /* Print usage and exit */
				usage ();   
				break;
			case 'o': 
				opFileName = optarg;
				break;
			case 't': 
				theta_method = atoi(optarg);
				break;   
			case 'p':
				phi_method = atoi(optarg);
				break;
			case 'f':
				ipFileType = atoi(optarg);
				break;
			case 'z':
				DetectorThickness = atoi(optarg);
				break;
			case 'e':
				RemoveEdges = false;
				break;
			case 's':
				ShowDataFileTypes();
				break;
			case 'a':
				ShowAngleCalculationMethods();
				break;
			default:
				fprintf (stderr, "Unrecognised option [-%c]\n", opt);
				usage();
				break;
		}
	}
	
	/* Process the individual files */
	if (optind >= argc)
	{
        fprintf(stderr, "Expected a input filename argument (%d/%d)\n", optind, argc);
		usage ();
	}
	else
	{
			if ((optind +1) == argc)
			{
				ipFileName = (char*)argv [optind];
				CalculateAngles(ipFileName, opFileName, ipFileType, DetectorThickness, theta_method, phi_method, RemoveEdges);
			}
	}
	return 0;
}
