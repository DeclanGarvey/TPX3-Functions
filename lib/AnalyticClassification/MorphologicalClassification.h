#pragma once

class particle;

int GetMorphologicalClass(particle const& p);

namespace Dots
{
	const int MaximumSize=2;
}
namespace SmallBlobs
{
	const int MaximumSize=4;
	const int MaximumDimension=1.0;
}
namespace HeavyBlobs
{
	const int MinimumInnerCount=4;
	const double MinimumInnerToOuterRatio = 0.5;
	const double MaximumRadiusDeviation = 1.2;
}
namespace HeavyTracks
{
	const int MinimumInnerCount=4;
	const double MinimumInnerToOuterRatio = 0.3;
	const double MinimumRadiusDeviation = 1.2;
}
namespace StraightTracks
{
	const double MinimumLinearity = 0.9;
	const int MinimumInLinePixels = 4;//Originally was 20 in line pixels
}
namespace ZeroIncidentParticles
{
 	const int MinimumPrimarySize=5;
 	const int MaximumPrimaryDimension=2;
}
