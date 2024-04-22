#include <string>
#include <vector>
#include "Particle.h"
#include "AngleCalulationFunctions.h"
#include "ClusterFeatures.h"
#include "FluenceStoppingPower.h"

#include "ElectronProtonClassifier.h"
using namespace std;

ElectronProtonClassifier::ElectronProtonClassifier(const std::string& ModelPath) : GeneralisedONNXModel(ModelPath)
{
	EnvironmentName_ = "ElectronClassifier";
}

vector<float>  ElectronProtonClassifier::CalculateInputFeatures(particle const& p)
{
	//if(p.GetSize()!=0)
	//{
		//auto skel = Skeletonise(p, 0.626, -103.25);
		auto skel = Skeletonise(p,0,5);
		
		particle RotatedCluster = RotateToNormal(skel);
		auto BoxDim = BoxDimensions(RotatedCluster);
		auto BoxStds = WeightedBoxStds(RotatedCluster);
		float LLMTheta = static_cast<float>(ThetaImprovedLLM(skel, 0.03));
		//float sp = StoppingPower(p.GetEnergy(), LLMTheta,0.03);
		vector<float> x{static_cast<float>(skel.GetSize()),static_cast<float>(p.GetEnergy()), static_cast<float>(LLMTheta),// static_cast<float>(BoxStds[0]), static_cast<float>(BoxStds[1]),
				 static_cast<float>(BoxDim[1]), static_cast<float>(BoxDim[0])};//, static_cast<float>(StandardDeviationEnergy(skel))};
		return x;
	//}
	//else 
	//{
	//	vector<float> x{0, 0, 0, 0,0,0};
	//	return x;
	//}
}
