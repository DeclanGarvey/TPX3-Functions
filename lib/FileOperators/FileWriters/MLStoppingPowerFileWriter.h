#pragma once
#include <string>
#include <memory>

class particle;
class RandomForrestTheta;
class SpFileWriter;

class MLSpFileWriter : public SpFileWriter
{
	private: 
		std::unique_ptr<RandomForrestTheta> ThetaModel;
	public:
		MLSpFileWriter(const std::string& opFileName, double DetectorThickness, const std::string& ModelPath);
		bool AddParticle(particle const& p);
};

