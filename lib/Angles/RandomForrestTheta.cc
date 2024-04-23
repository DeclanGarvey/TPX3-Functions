#include <vector>
#include "Particle.h"
#include "AngleCalulationFunctions.h"
#include "ClusterFeatures.h"
//#include "GeneralisedONNXModel.h"

#include "RandomForrestTheta.h"
using namespace std;
#include <iostream>



/*
Takes a particle cluster as input and outputs the feature vector necesary for Random Forrest Theta Calculation based on Improved algorithms for Master Thesis: Advanced Methodology for Radiation Field Decomposition with Hybrid Pixel Detectors https://dspace.cvut.cz/bitstream/handle/10467/108693/F4-DP-2023-Garvey-Declan-MastersThesis_DeclanGarvey.pdf

*/
vector<float> RandomForrestTheta::CalculateInputFeatures(particle const& p)
{
	auto skel = Skeletonise(p,0,5); 
	particle RotatedCluster = RotateToNormal(skel);
	auto BoxDim = BoxDimensions(RotatedCluster);
	auto BoxStds = WeightedBoxStds(RotatedCluster);
	float LLMTheta = static_cast<float>(ThetaImprovedLLM(skel, 0.05));
	vector<float> x{static_cast<float>(skel.GetSize()), static_cast<float>(BoxDim[0]), static_cast<float>(BoxDim[1]),
			 static_cast<float>(BoxStds[1]), static_cast<float>(BoxStds[0]),
			  LLMTheta};
	//for(int i=0;i<x.size();i++)
	//	cout<<x[i]<<" ";
	//cout<<endl;
	return x;
}

