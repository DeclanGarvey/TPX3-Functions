#pragma once
#include <string>
#include <vector>

class particle;
//class GeneralisedONNXModel;
#include "GeneralisedONNXModel.h"
/*

*/
class RandomForrestTheta : public GeneralisedONNXModel<float>
{
	public:
		RandomForrestTheta(const RandomForrestTheta&) = default;
   		RandomForrestTheta(RandomForrestTheta&&) = default;
    
		RandomForrestTheta(const std::string& ModelPath) : GeneralisedONNXModel<float>(ModelPath) {}
		std::vector<float> CalculateInputFeatures(particle const& p);
};

