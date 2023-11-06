#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Particle.h"
#include "AngleCalulationFunctions.h"
#include "ClusterFeatures.h"

#include "RandomForrestTheta.h"
using namespace std;

template <typename T>
Ort::Value ValueToTensor(const Ort::MemoryInfo& MemInfo , std::vector<T> &data, const std::vector<int64_t> &shape)
{
	auto tensor = Ort::Value::CreateTensor<T>(MemInfo, data.data(), data.size(), shape.data(), shape.size());
	return tensor;
}

RandomForrestTheta::RandomForrestTheta(const string& ModelPath)
{
		
		Environment_ = std::make_unique<Ort::Env>(Ort::Env(ORT_LOGGING_LEVEL_WARNING, "RandomForrestThetaModel"));
		Ort::SessionOptions SessionOptions;
        	SessionOptions.SetIntraOpNumThreads(1); 
        	Session_ = std::make_unique<Ort::Session>(Ort::Session(*Environment_, ModelPath.c_str(), SessionOptions));
        	MemoryInfo_ = std::make_unique<Ort::MemoryInfo>(Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault));
        	
        	
        	Ort::TypeInfo inputTypeInfo = Session_->GetInputTypeInfo(0);
   		auto inputTensorInfo = inputTypeInfo.GetTensorTypeAndShapeInfo();
        	InputDims_ = inputTensorInfo.GetShape();
        	Ort::TypeInfo outputTypeInfo = Session_->GetOutputTypeInfo(0);
   		auto outputTensorInfo = outputTypeInfo.GetTensorTypeAndShapeInfo();
        	OutputDims_ = outputTensorInfo.GetShape();
		if (InputDims_.at(0) == -1)
		{
			DynamicBatchSize_ = true;
			InputDims_.at(0) = BatchSize_;
			OutputDims_.at(0) = BatchSize_;
		}
		
		Ort::AllocatorWithDefaultOptions allocator;
		string InputName = Session_->GetInputNameAllocated(0, allocator).get();
		InputNames_.push_back(strdup(InputName.c_str()));
		string outputName = Session_->GetOutputNameAllocated(0, allocator).get();
		OutputNames_.push_back(strdup(outputName.c_str()));
}

vector<float> RandomForrestTheta::CalculateInputFeatures(particle const& p)
{
	particle RotatedCluster = RotateToNormal(p);
	auto BoxDim = BoxDimensions(RotatedCluster);
	auto BoxStds = WeightedBoxStds(RotatedCluster);
	float LLMTheta = static_cast<float>(ThetaImprovedLLM(p, 0.05));
	vector<float> x{static_cast<float>(p.GetSize()), static_cast<float>(BoxDim[0]), static_cast<float>(BoxDim[1]), static_cast<float>(BoxStds[1]),static_cast<float>(BoxStds[0]), LLMTheta};
	return x;
}

float RandomForrestTheta::PredictFromFeatures(vector<float> Inputs)
{
	vector<Ort::Value> InputTensor;
	InputTensor.push_back(ValueToTensor<float>(*MemoryInfo_, Inputs, InputDims_));//MemoryInfo_??
	std::vector<Ort::Value> Output = Session_->Run(Ort::RunOptions{nullptr}, InputNames_.data(), InputTensor.data(),
                                      InputNames_.size(), OutputNames_.data(), OutputNames_.size());
	
	return *Output[0].GetTensorMutableData<float>();
}

float RandomForrestTheta::PredictFromParticle(particle const& p)
{
	vector<float> Inputs = CalculateInputFeatures(p);
	float Outputs = PredictFromFeatures(Inputs);
	return Outputs;
}

void RandomForrestTheta::AdjustBatchSize(int64_t BatchSize)
{
	if(DynamicBatchSize_)
	{
		BatchSize_ = BatchSize;
		InputDims_.at(0) = BatchSize_;
		OutputDims_.at(0) = BatchSize_;
	}
	else
		cout<<"Model does not have dynamic batch size so batch size, batch size = "<< BatchSize_<<endl;
}

