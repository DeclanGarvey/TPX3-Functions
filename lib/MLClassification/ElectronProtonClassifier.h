#pragma once
#include <string>
#include <vector>

class particle;
//class GeneralisedONNXModel;
#include "GeneralisedONNXModel.h"

class ElectronProtonClassifier : public GeneralisedONNXModel<bool>
{
	public:
		ElectronProtonClassifier(const std::string& ModelPath);
		std::vector<float> CalculateInputFeatures(particle const& p);
};
