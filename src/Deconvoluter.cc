#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include <iostream>

#include "ResponseConstruction.h"
#include "Deconvolution.h"
#include "FileNames.h"
using namespace std;
/* Local Definitions */
#define MODULE_NAME    "File Convertor Model"
#define MODULE_VERSION "1.0"


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
	fprintf (stderr, "-d            : Flag indicates input is directory of file.\n");
	fprintf (stderr, "-r <filename> : Specify directory of the response matrix used by algorithm, default './Response.root'\n");
	fprintf (stderr, "-i <int>      : Indicate the measurement region chosen specified in _sp.txt files column 1, default -1 => take all measurment regions.\n");
	fprintf (stderr, "-z <double>   : Specify Detector Thickness, default 0.05 cm\n");
	fprintf (stderr, "-p <filename> : Specify directory at which to save image of response matrix, empty by default => response matrix image not saved.\n");
	fprintf (stderr, "-s            : Show data file types.");
	fprintf (stderr, "\n");
	fprintf (stderr, "NOTE: Deconvolution algorithm only takes _sp.txt files as input type.\n\n");
	fprintf (stderr, "Declare input file after all parameters are specified.\n\n");
	exit (1);
}

/*
 * Main entry point.
 */
int main (int argc, const char *argv[])
{
	int opt;                      /* Local loop counter */

	string opFileName;            /* output file name */
    	string ipFileName; 		/* Input file name */
	string ResponseFileName = "./Response.root";
	string ResponseMatrixImageSaveDirectory;
	double DetectorThickness = 0.05;
	bool InputIsDirectory=false;
	int RegionID=-1;
	/* Collect the arguments from the command line */
	while ((opt = getopt (argc, (char * const*)argv, "?ho:do:t:f:ez:psr:i:")) != EOF)
	{
		/* Process the arguments */
		switch (opt)
		{
			case 'h':
			case '?': /* Print usage and exit */
				usage ();   
				break;
			case 'o': 
				opFileName.assign(optarg);
				break; 
			case 'd':
				InputIsDirectory = true;
				break;
			case 'r':
				ResponseFileName.assign(optarg);
				break;
			case 'i':
				RegionID = atoi(optarg);
				break;
			case 'z':
				DetectorThickness = atof(optarg);
				break;
			case 'p':
				ResponseMatrixImageSaveDirectory.assign(optarg);
				break;
			case 's':
				ShowDataFileTypes();
				break;
			default:
				fprintf (stderr, "Unrecognised option [-%c]\n", opt);
				usage();
				break;
		}
	}
	if(ResponseFileName.empty())
		ResponseFileName = "Response.root";
	if(opFileName.empty())
		opFileName = "results";
	/* Process the individual files */
	if (optind >= argc)
	{
        cout<<"Expected a input filename argument ("<<optind<<"/"<<argc<<")"<<endl;
		usage ();
	}
	else
	{
		if ((optind +1) == argc)
		{
			ipFileName.assign(argv[optind]);
			if(ResponseMatrixImageSaveDirectory.empty()==false)
				PrintUnfoldingMatrix(ResponseFileName, ResponseMatrixImageSaveDirectory);
				
			if(InputIsDirectory)
				DeconvolutionDirectory(ResponseFileName, ipFileName, opFileName,DetectorThickness,RegionID);
			else
				Deconvolution(ResponseFileName, ipFileName, opFileName,DetectorThickness,RegionID);
			
			
		}
		else
		{
			fprintf (stdout, "No action specified\n");
		}
	}
	return 0;
}
