#include <math.h>
#include <iostream>

using namespace std;

#include "Particle.h"
#include "ClusterFeatures.h"
#include "AngleCalulationFunctions.h"
#include "MorphologicalClassification.h"

//This function takes in particle class and outputs the Morphological classification based on T. Holy et al. “Pattern recognition of tracks induced by individual quanta of ionizing radiation in Medipix2 silicon detector”. In: Nuclear Instruments and Methods in Physics Research Section A: Accelerators, Spectrometers, Detectors and Associated Equipment 591.1 (2008). 
//Updates to account for some speical case scenarios
int GetMorphologicalClass(particle const& p)
{
	if(p.GetSize()>Dots::MaximumSize)
	{
		if( (abs(p.GetRightBottomMost().x- p.GetLeftTopMost().x)>SmallBlobs::MaximumDimension) | (abs(p.GetTopLeftMost().y - p.GetBottomRightMost().y)>SmallBlobs::MaximumDimension) )
		{
			
			double inner = NumberOfInnerPixels(p);
			double InnerToOuter = inner/(p.GetSize() - inner);
			double diameter = DiameterOfCluster(p);
			double maxdist = MaximumDistance(p);
			if( (inner<HeavyBlobs::MinimumInnerCount) | (InnerToOuter<HeavyBlobs::MinimumInnerToOuterRatio) | (maxdist >= (HeavyBlobs::MaximumRadiusDeviation*diameter)) )
			{
				if( (inner<HeavyTracks::MinimumInnerCount) | (InnerToOuter<HeavyTracks::MinimumInnerToOuterRatio) | (maxdist < (HeavyTracks::MinimumRadiusDeviation*diameter)) )
				{
					double lin = Linearity(p);
					if( (lin<StraightTracks::MinimumLinearity) | (lin*p.GetSize()<StraightTracks::MinimumInLinePixels) )
					{
						auto PrimaryTrack = Skeletonise(p,0,5);
						if( (abs(PrimaryTrack.GetRightBottomMost().x- PrimaryTrack.GetLeftTopMost().x)<=ZeroIncidentParticles::MaximumPrimaryDimension)
							 & (abs(PrimaryTrack.GetTopLeftMost().y - PrimaryTrack.GetBottomRightMost().y)<=ZeroIncidentParticles::MaximumPrimaryDimension)
							 & (ContainsInner4FoldPixel(PrimaryTrack)) )
							return 6; //Lowly Ionsiing particles at low angle sof incidence
						else if(ContainsInner8FoldPixel(PrimaryTrack))
							return 7; //Medium->Highly ionising particles at low angles of incidence
						else
						{
							return 5; //Curly Tracks
						}
					}
					else
						return 4; //Straight Tracks
				}
				else
					return 3; //Heavy Tracks
			}
			else 
				return 2; //Heavy Blobs
		}
		else
			return 1; //Small Blobs
	}
	else 
		return 0; //Dots
}

