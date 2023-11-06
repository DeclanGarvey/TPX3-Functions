#pragma once
#include <string>

class particle;

class ParticleFileWriter
{
	public:
		virtual ~ParticleFileWriter() {}
		virtual bool AddParticle(particle const& p) {return false;}
		virtual bool AddParticleWithRegionID(particle const& p, int RegionID){return false;};
		virtual void Close() {};
};


class PxFileWriter : public ParticleFileWriter
{
	private:
		FILE* opInitFile_;
		FILE* opClFile_;
		FILE* opPxFile_;
		int PxLineNo_, PxByteNo_;
		double DetectorThickenss_;
	public:
		PxFileWriter(const std::string&  opFileName, double DetectorThickness);
		bool AddParticle(particle const& p);
		void AddInitInfo(const std::string&  ipInfo);
		void Close();
};

class AngFileWriter : public ParticleFileWriter
{
	private:
		FILE* opFile_;
		double DetectorThickenss_;
	public:
		AngFileWriter(const std::string& opFileName, double DetectorThickness);
		bool AddParticle(particle const& p);
		void Close();
};

class SpFileWriter : public ParticleFileWriter
{
	protected: 
		FILE* opFile_;
		double DetectorThickness_;
	public:
		SpFileWriter(const std::string& opFileName, double DetectorThickness);
		bool AddParticle(particle const& p);
		void Close();
};

class FeatFileWriter : public ParticleFileWriter
{
	private: 
		FILE* opFile_;
		double DetectorThickenss_;
	public:
		FeatFileWriter(const std::string& opFileName, double DetectorThickness);
		bool AddParticle(particle const& p);
		void Close();
};

class SatFileWriter : public ParticleFileWriter
{
	private: 
		FILE* opFile_;
		double DetectorThickenss_;
	public:
		SatFileWriter(const std::string& opFileName, double DetectorThickness);
		bool AddParticle(particle const& p);
		void Close();
};

class FieldTrackingFileWriter : public ParticleFileWriter
{
	protected: 
		FILE* opFile_;
		double DetectorThickness_;
	public:
		FieldTrackingFileWriter(const std::string& opFileName, double DetectorThickness);
		bool AddParticle(particle const& p);
		void Close();
};
