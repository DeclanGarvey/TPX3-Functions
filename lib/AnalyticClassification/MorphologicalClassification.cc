#include <math.h>
#include <iostream>

using namespace std;

#include "Particle.h"
#include "ClusterFeatures.h"
#include "AngleCalulationFunctions.h"
#include "MorphologicalClassification.h"

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
							return 6; //Accounting for misclassification of volcano affect
						else if(ContainsInner8FoldPixel(PrimaryTrack))
							return 7;
						else
						{
							//cout<<abs(p.GetRightBottomMost().x- p.GetLeftTopMost().x)<< " "<<abs(p.GetTopLeftMost().y - p.GetBottomRightMost().y)<<endl;
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

