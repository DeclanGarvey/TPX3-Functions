#pragma once
#include <string>
#include <memory>

class particle;
class RandomForrestTheta;

class HighZSearchFileWriter : public ParticleFileWriter
{
	private: 
		std::unique_ptr<RandomForrestTheta> ThetaModel;
	public:
		HighZSearchFileWriter(const std::string& opFileName, double DetectorThickness, const std::string& ModelPath);
		HighZSearchFileWriter(double DetectorThickness, const string& ModelPath);
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
};
