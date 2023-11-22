#include <math.h>
#include <iostream>

using namespace std;

#include "Particle.h"
#include "ClusterFeatures.h"
#include "MorphologicalClassification.h"

int GetMorphologicalClass(particle const& p)
{
	if(p.GetSize()>Dots::MaximumSize)
	{
		if( (abs(p.GetRightBottomMost().x- p.GetLeftTopMost().x)>SmallBlobs::MaximumXYDimension) & (abs(p.GetTopLeftMost().y - p.GetBottomRightMost().y)>SmallBlobs::MaximumXYDimension) )
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
						if(p.GetSize()>CurlyTracks::MaximumSize)
							return 3; //Accounting for misclassification of volcano affect
						else
							return 5; //Curly Tracks
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

