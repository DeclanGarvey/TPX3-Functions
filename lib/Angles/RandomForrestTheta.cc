#include <vector>
#include "Particle.h"
#include "AngleCalulationFunctions.h"
#include "ClusterFeatures.h"
//#include "GeneralisedONNXModel.h"

#include "RandomForrestTheta.h"
using namespace std;




vector<float> RandomForrestTheta::CalculateInputFeatures(particle const& p)
{
	//if(p.GetSize()!=0)
	//{
		//auto skel = Skeletonise(p, 0.626, -103.25);
		auto skel = Skeletonise(p,0,5);
		particle RotatedCluster = RotateToNormal(skel);
		auto BoxDim = BoxDimensions(RotatedCluster);
		auto BoxStds = WeightedBoxStds(RotatedCluster);
		float LLMTheta = static_cast<float>(ThetaImprovedLLM(skel, 0.05));
		vector<float> x{static_cast<float>(skel.GetSize()), static_cast<float>(BoxDim[0]), static_cast<float>(BoxDim[1]),
				 static_cast<float>(BoxStds[1]), static_cast<float>(BoxStds[0]),
				  LLMTheta};
		return x;
	//}
	//else 
	//{
	//	vector<float> x{0, 0, 0, 0,0,0};
	//	return x;
	//}
}

