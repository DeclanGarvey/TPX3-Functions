#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include <iostream>

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
	fprintf (stderr, "-d            : Flag indicates input is directory of files.\n");
	fprintf (stderr, "-f <int>      : (recommended) Specify the input file type, default set according to file ending.\n");
	fprintf (stderr, "                NOTE: SATRAM file type only used if specified(3).\n");
	fprintf (stderr, "-t <int>      : Specify the ouput file type. default for, case file input: './results', case directory input: ipDirectory\n");
	fprintf (stderr, "-z <double>   : Specify Detector Thickness, default 0.05 cm\n");
	fprintf (stderr, "-e            : Flag indicates to keep edge clusters in output, by default they are removed.\n");
	fprintf (stderr, "-s            : Show data file types.");
	fprintf (stderr, "\n");
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
	string ResponseFileName;
	//int ipFileType = -1;
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
			if(InputIsDirectory)
				DeconvolutionDirectory(ResponseFileName, ipFileName, opFileName,DetectorThickness,RegionID);
			else
				Deconvolution(ResponseFileName, ipFileName, opFileName,DetectorThickness,RegionID);
			/*if(InputIsDirectory)
			{
				if (opFileName.empty())
				{
					opFileName = ipFileName;
				}
				ConvertDirectoryTo(ipFileName, opFileName, ipFileType, opFileType, DetectorThickness, RemoveEdges);
			}
			else
			{
				if (opFileName.empty())
				{
					opFileName.assign("./results");
				}
				ConvertTo(ipFileName, opFileName, ipFileType, opFileType, DetectorThickness, RemoveEdges);
			}*/
			
			
		}
		else
		{
			fprintf (stdout, "No action specified\n");
		}
	}
	return 0;
}
