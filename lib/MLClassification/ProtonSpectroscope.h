#pragma once
#include <string>
#include <vector>

class particle;
//class GeneralisedONNXModel;
#include "GeneralisedONNXModel.h"

class ProtonSpectroscope : public GeneralisedONNXModel<float>
{
	public:
		ProtonSpectroscope(const std::string& ModelPath);
		std::vector<float> CalculateInputFeatures(particle const& p);
};
