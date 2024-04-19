#pragma once
#include <string>

class particle;

class ParticleFileWriter
{
	protected:
		FILE* opFile_=nullptr;
		double DetectorThickness_ =0.05;
	public:
		virtual bool AddParticle(particle const& p) {return false;}
		virtual void UpdateFileOutput(const std::string& opFileName) {};
		virtual bool AddParticleWithRegionID(particle const& p, int RegionID){return false;};
		virtual void Close() 
		{
			if(opFile_!=nullptr)
			{
				fclose(opFile_);
				opFile_=nullptr;
			}
		}
		virtual ~ParticleFileWriter() { Close(); }
};


class PxFileWriter : public ParticleFileWriter
{
	private:
		FILE* opInitFile_ = nullptr;
		FILE* opClFile_ = nullptr;
		
		int PxLineNo_, PxByteNo_;
	public:
		PxFileWriter(const std::string&  opFileName, double DetectorThickness);
		PxFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
		void AddInitInfo(const std::string&  ipInfo);
		void Close();
};

class AngFileWriter : public ParticleFileWriter
{
	public:
		AngFileWriter(const std::string& opFileName, double DetectorThickness);
		AngFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
};

class SpFileWriter : public ParticleFileWriter
{
	public:
		SpFileWriter(const std::string& opFileName, double DetectorThickness);
		SpFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
};

class FeatFileWriter : public ParticleFileWriter
{
	public:
		FeatFileWriter(const std::string& opFileName, double DetectorThickness);
		FeatFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
};

class SatFileWriter : public ParticleFileWriter
{
	private: 
		int Region_=-2;
		double CurrentFrameTime_=-1;
		double Occupation_=0;
		double EnergyDeposition_=0;
		double AcqusitionTime_=0;
		double ClusterCounts_=0;
		double SatPosX_ = -1;
		double SatPosY_ = -1;
		double SatAltitude_ = -1;
		int EntryNumber=-1;
	public:
		SatFileWriter(const std::string& opFileName, double DetectorThickness);
		SatFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}

		void UpdateFileOutput(const std::string& opFileName);

		bool AddParticle(particle const& p);
};
class FieldTrackingFileWriter : public ParticleFileWriter
{
	public:
		FieldTrackingFileWriter(const std::string& opFileName, double DetectorThickness);
		FieldTrackingFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
};
