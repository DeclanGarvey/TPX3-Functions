#pragma once
#include <string>
#include <memory>

class particle;
class RandomForrestTheta;
class FieldTrackingFileWriter;

class MLFieldTrackingFileWriter : public FieldTrackingFileWriter
{
	private: 
		std::unique_ptr<RandomForrestTheta> ThetaModel;
	public:
		MLFieldTrackingFileWriter(const std::string& opFileName, double DetectorThickness, const std::string& ModelPath);
		bool AddParticle(particle const& p);
};

