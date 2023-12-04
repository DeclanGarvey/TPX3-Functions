#pragma once
#include <string>

class particle;

class ParticleFileWriter
{
	public:
		virtual ~ParticleFileWriter() {}
		virtual bool AddParticle(particle const& p) {return false;}
		virtual void UpdateFileOutput(const std::string& opFileName) {};
		virtual bool AddParticleWithRegionID(particle const& p, int RegionID){return false;};
		virtual void Close() {};
};


class PxFileWriter : public ParticleFileWriter
{
	private:
		FILE* opInitFile_=NULL;
		FILE* opClFile_=NULL;
		FILE* opPxFile_=NULL;
		int PxLineNo_, PxByteNo_;
		double DetectorThickness_;
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
	private:
		FILE* opFile_=NULL;
		double DetectorThickness_;
	public:
		AngFileWriter(const std::string& opFileName, double DetectorThickness);
		AngFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
		void Close();
};

class SpFileWriter : public ParticleFileWriter
{
	protected: 
		FILE* opFile_=NULL;
		double DetectorThickness_;
	public:
		SpFileWriter(const std::string& opFileName, double DetectorThickness);
		SpFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
		void Close();
};

class FeatFileWriter : public ParticleFileWriter
{
	private: 
		FILE* opFile_=NULL;
		double DetectorThickness_;
	public:
		FeatFileWriter(const std::string& opFileName, double DetectorThickness);
		FeatFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
		void Close();
};

class SatFileWriter : public ParticleFileWriter
{
	private: 
		FILE* opFile_=NULL;
		double DetectorThickness_;
	public:
		SatFileWriter(const std::string& opFileName, double DetectorThickness);
		SatFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
		void Close();
};

class FieldTrackingFileWriter : public ParticleFileWriter
{
	protected: 
		FILE* opFile_=NULL;
		double DetectorThickness_;
	public:
		FieldTrackingFileWriter(const std::string& opFileName, double DetectorThickness);
		FieldTrackingFileWriter(double DetectorThickness){DetectorThickness_=DetectorThickness;}
		
		void UpdateFileOutput(const std::string& opFileName);
		
		bool AddParticle(particle const& p);
		void Close();
};
