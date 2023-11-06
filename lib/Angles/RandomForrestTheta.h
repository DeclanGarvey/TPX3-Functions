#pragma once
#include <string>
#include <vector>
#include <memory>
#include <onnxruntime_cxx_api.h>
class particle;


class RandomForrestTheta
{
	public:
		RandomForrestTheta(const std::string& ModelPath);
		std::vector<float> CalculateInputFeatures(particle const& p);
		float PredictFromFeatures(std::vector<float> Inputs);
		float PredictFromParticle(particle const& p);
		void AdjustBatchSize(int64_t BatchSize);
	private:
		
		std::unique_ptr<Ort::Env> Environment_;
		std::unique_ptr<Ort::Session> Session_;
		std::unique_ptr<Ort::MemoryInfo> MemoryInfo_;
		
		bool DynamicBatchSize_=false;
		int64_t BatchSize_ = 1;
		std::vector<int64_t> InputDims_;
		std::vector<int64_t> OutputDims_;
		std::vector<const char*> InputNames_;
		std::vector<const char*> OutputNames_;
};
