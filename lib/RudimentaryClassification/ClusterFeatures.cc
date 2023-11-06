#include <iostream>
#include <fstream>
#include <assert.h>
#include <math.h>
using namespace std;

#include "TMath.h"

#include "ParticleFileReaders.h"
#include "AngleCalulationFunctions.h"
#include "Particle.h"
#include "ClusterFeatures.h"
#include "FittingVariables.h"

#include "TH1D.h"

int GetRudementaryClass(particle const& p)
{
	if(p.GetSize()>2)
	{
		if( (abs(p.GetRightBottomMost().x- p.GetLeftTopMost().x)>1) & (abs(p.GetTopLeftMost().y - p.GetBottomRightMost().y)>1)
		{
			double lin = LLMLinearity(p);
			if((lin<0.9) | (lin*p.GetSize()<5))
			{
				
			}
			else
				return 2
		}
		else
			return 1;
	}
	else 
		return 0;
}

