#pragma once
#include <string>
#include <memory>

class particle;
class RandomForrestTheta;
class FieldTrackingFileWriter;
class SpFileWriter;
class ParticleFileWriter;

class MLFieldTrackingFileWriter : public FieldTrackingFileWriter
{
	private: 
		std::unique_ptr<RandomForrestTheta> ThetaModel;
	protected:
		double ThetaCalculationMethod(particle const& p);
	public:
		MLFieldTrackingFileWriter(const std::string& opFileName, double DetectorThickness, const std::string& ModelPath);
		MLFieldTrackingFileWriter(double DetectorThickness, const string& ModelPath);
};

class MLSpFileWriter : public SpFileWriter
{
	private: 
		std::unique_ptr<RandomForrestTheta> ThetaModel;
	protected:
		double ThetaCalculationMethod(particle const& p);
	public:
		MLSpFileWriter(const std::string& opFileName, double DetectorThickness, const std::string& ModelPath);
		MLSpFileWriter(double DetectorThickness, const std::string& ModelPath);
};

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
