
#pragma once

#include <iostream>
#include <string>

class TFile;
class TTree;
class TList; 
class TIter;
class TSystemFile;
class TH1D;


#include "ParticleGeneratorBaseClass.h"

class particle;

class ParticleFileReader : public ParticleGeneratorBaseClass
{
	protected:
		FILE* ipFile_;
	public:
		virtual ~ParticleFileReader() {}
		//virtual bool AssignParticle(particle& p){return false;};
		virtual void Close() {};
};

ParticleFileReader* GetFileReader(const std::string& ipFileName, int ipFileType);

class PxFileReader : public ParticleFileReader
{
	public:
		PxFileReader(const std::string& ipFileName);
		bool AssignParticle(particle &p);
		void Close() {
			if(ipFile_!=NULL)
			{
				fclose(ipFile_);
				ipFile_=NULL;
			}
		}
};

class AngFileReader : public ParticleFileReader
{
	public:
		AngFileReader(const std::string& ipFileName);
		bool AssignParticle(particle &p);
		void Close() {
			if(ipFile_!=NULL)
			{
				fclose(ipFile_);
				ipFile_=NULL;
			}
		}
};

class SimFileReader : public ParticleFileReader
{
	public:
		SimFileReader(const std::string& ipFileName);
		bool AssignParticle(particle &p);
		void Close() {
			if(ipFile_!=NULL)
			{
				fclose(ipFile_);
				ipFile_=NULL;
			}
		}
};

class ROOTFileReader : public ParticleFileReader
{
	TFile* File_;
	TTree*  Data_; 
	
	int CurrentEntryNo;
	
	int size;
	double time;
	double theta, phi;
	int x[256*256];
	int y[256*256];
	float energy[256*256];
	double PrimaryEnergy;
	int ParticleType;
	public:
		ROOTFileReader(const std::string& ipFileName);
		bool AssignParticle(particle &p);
		void Close();
};

class SATRAMFileReader : public ParticleFileReader
{
	TFile* File_;
	TTree*  dscData_;
	TTree* ClusterFile_;
	double MaxOccupancy_=5;
	double MinOccupancy_ = 0;
	int CurrentClusterEntry;
	int CurrentSatEntry;
	
	double dtime; 
	float SatellitePosition[3];
	
	int size;
	double ctime;
	short x[256*256];
	short y[256*256];
	float energy[256*256];
	float occupancy_;
	short good_region;
	float acq_time;
	public:
		SATRAMFileReader(const std::string& ipFileName);
		bool AssignParticle(particle &p);
		void SetMaximumOccupancy(double NewMaxOccupancy);
		void SetMinimumOccupancy(double NewMaxOccupancy);
		float GetCurrentOccupancy();
		int GetCurrentOrbitRegion();
		double GetAcquisitionTime();
		void Close();
};
class StoppingPowerFileReader
{
	
	FILE* ipFile_=NULL;
	int EntryNumber_=0;
	int Region_=-1;
	int ParticleType_=-1;
	double PrimaryEnergy_;
	double ToA_=-1;
	double AcquisitionTime_;
	double TotalAcquisitionTime_=0;
	int AcquisitionRegion_=-2;
	int Size_;
	double Height_;
	double Theta_;
	double Phi_;
	double Energy_;
	double StoppingPower_;
	
	public:
		StoppingPowerFileReader() {};
		StoppingPowerFileReader(const std::string& ipFileName, int AcquisitionRegion);
		bool ReadNextEntry();
		int GetEntryNumber() const;
		int GetRegionID() const;
		int GetParticleType() const;
		double GetPrimaryEnergy() const;
		double GetStoppingPower() const;
		double GetToA() const;
		double GetAcquisitionTime() const;
		double GetTotalAcquisitionTime() const;
		int GetSize() const;
		double GetHeight() const;
		double GetTheta() const;
		double GetPhi() const;
		double GetEnergy() const;
		void Close() {
			if(ipFile_!=NULL) 
			{
				fclose(ipFile_);
				ipFile_=NULL;
			}
		}
};

class FluenceFileReader
{
	protected:
		FILE* ipFile_;
		double DetectorWidth_;
		double DetectorThickness_;
		double FluencePerFile_;
		double NoFiles_;
	public:
		FluenceFileReader(const std::string& ipFileName);
		double GetTotalFluence();
};

