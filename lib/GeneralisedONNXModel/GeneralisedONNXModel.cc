#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Particle.h"
#include "ClusterFeatures.h"

#include "GeneralisedONNXModel.h"
using namespace std;

/*
This the generalised version of object used to read and use ML models saved as .onnx files through this project
*/


/*
Takes in a std::vector and converts it to an ORT::Value vector as output which can be input into an ONNX model

	MemInfo: ONNX memory allocation and management (consult ONNX documentation)
	data: the vector that will be converted to ORT::Value
	shape: the shape of the vector to be output
*/
template <typename T>
Ort::Value ValueToTensor(const Ort::MemoryInfo& MemInfo , std::vector<T> &data, const std::vector<int64_t> &shape)
{
	auto tensor = Ort::Value::CreateTensor<T>(MemInfo, data.data(), data.size(), shape.data(), shape.size());
	return tensor;
}

/*
Constructs the onnx model and necessary variables
	ModelPath specifies the location the ML .onnx model is saved
*/
template <typename OutputDataType>
GeneralisedONNXModel<OutputDataType>::GeneralisedONNXModel(const string& ModelPath)
{
		Environment_ = std::make_unique<Ort::Env>(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, EnvironmentName_.c_str());
		MemoryInfo_ = std::make_unique<Ort::MemoryInfo>(Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault));
		
		Ort::SessionOptions SessionOptions;
        	SessionOptions.SetIntraOpNumThreads(1); 
        	Session_ = std::make_unique<Ort::Session>(*Environment_, ModelPath.c_str(), SessionOptions);
        	
        	
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
		
		for (std::size_t i = 0; i < Session_->GetInputCount(); i++)
			InputNames_.emplace_back(Session_->GetInputNameAllocated(i, allocator).get());
		for (std::size_t i = 0; i < Session_->GetOutputCount(); i++)
			OutputNames_.emplace_back(Session_->GetOutputNameAllocated(i, allocator).get());
		
		InputNamesChar_ = std::vector<const char *>(InputNames_.size(), nullptr);
        	OutputNamesChar_ = std::vector<const char *>(OutputNames_.size(), nullptr);
        	std::transform(std::begin(InputNames_), std::end(InputNames_), std::begin(InputNamesChar_),
                       [&](const std::string &str)
                       { return str.c_str(); });

        	std::transform(std::begin(OutputNames_), std::end(OutputNames_), std::begin(OutputNamesChar_),
                       [&](const std::string &str)
                       { return str.c_str(); });
}

/*
Take in the vector of inputs used by the ML model and outputs the prediction
NOTE: The shape of the input depends of the particular that was loaded on construction
*/
template<typename OutputDataType>
OutputDataType GeneralisedONNXModel<OutputDataType>::PredictFromFeatures(vector<float> Inputs)
{
	vector<Ort::Value> InputTensor;
	InputTensor.push_back(ValueToTensor<float>(*MemoryInfo_, Inputs, InputDims_));//MemoryInfo_??
	std::vector<Ort::Value> Output = Session_->Run(Ort::RunOptions{nullptr}, InputNamesChar_.data(), InputTensor.data(),
                                      InputNamesChar_.size(), OutputNamesChar_.data(), OutputNamesChar_.size());
	return  *Output[0].GetTensorMutableData<OutputDataType>();
}

/*
Takes in a particle and outputs the prediction of the model
*/
template<typename OutputDataType>
OutputDataType GeneralisedONNXModel<OutputDataType>::PredictFromParticle(particle const& p)
{
	vector<float> Inputs = CalculateInputFeatures(p);
	OutputDataType Outputs = PredictFromFeatures(Inputs);
	return Outputs;
}

/*
Allows for the adjusting of Batch size
*/
template<typename OutputDataType>
void GeneralisedONNXModel<OutputDataType>::AdjustBatchSize(int64_t BatchSize)
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


template class GeneralisedONNXModel<float>;
template class GeneralisedONNXModel<int>;
template class GeneralisedONNXModel<bool>;

