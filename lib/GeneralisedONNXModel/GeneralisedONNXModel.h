#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <onnxruntime_cxx_api.h>
class particle;

template <typename OutputDataType>
class GeneralisedONNXModel
{
	protected:
		std::string EnvironmentName_ = "ONNXModel";
	//private:
		std::unique_ptr<Ort::Env> Environment_;
		std::unique_ptr<Ort::Session> Session_;
		std::unique_ptr<Ort::MemoryInfo> MemoryInfo_;
		
		bool DynamicBatchSize_=false;
		int64_t BatchSize_ = 1;
		std::vector<int64_t> InputDims_;
		std::vector<int64_t> OutputDims_;
		std::vector<const char*> InputNames_;
		std::vector<const char*> OutputNames_;
	public:
		GeneralisedONNXModel(const std::string& ModelPath);

		OutputDataType PredictFromFeatures(std::vector<float> Inputs);
		OutputDataType PredictFromParticle(particle const& p);
		void AdjustBatchSize(int64_t BatchSize);
		
		virtual std::vector<float> CalculateInputFeatures(particle const& p) = 0;
		// Explicitly delete copy constructor and copy assignment operator
		/*GeneralisedONNXModel(const GeneralisedONNXModel&) = default;
		GeneralisedONNXModel& operator=(const GeneralisedONNXModel&) =  = default;
		*/
		virtual ~GeneralisedONNXModel()
		{
			for (const char* name : InputNames_) 
			    delete[] name; // Deallocate memory
			for (const char* name : OutputNames_) 
			    delete[] name; // Deallocate memory
		}
		
		

};
