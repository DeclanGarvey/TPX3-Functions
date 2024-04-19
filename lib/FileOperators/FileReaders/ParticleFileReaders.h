
#pragma once

#include <iostream>
#include <string>
#include <map>

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
		FILE* ipFile_=nullptr;
	public:
		virtual void UpdateFileInput(const std::string& ipFileName) {};
		virtual void Close() 
		{
			if(ipFile_!=nullptr)
			{
				fclose(ipFile_);
				ipFile_=nullptr;
			}
		}
		virtual ~ParticleFileReader() {Close();}
};

class PxFileReader : public ParticleFileReader
{
	public:
		PxFileReader() {};
		PxFileReader(const std::string& ipFileName){ UpdateFileInput(ipFileName); }
		void UpdateFileInput(const std::string& ipFileName);
		bool AssignParticle(particle &p);
};

class AngFileReader : public ParticleFileReader
{
	public:
		AngFileReader() {}
		AngFileReader(const std::string& ipFileName){ UpdateFileInput(ipFileName); }
		void UpdateFileInput(const std::string& ipFileName);
		bool AssignParticle(particle &p);
};

class SimFileReader : public ParticleFileReader
{
	public:
		SimFileReader() {}
		SimFileReader(const std::string& ipFileName){ UpdateFileInput(ipFileName); }
		void UpdateFileInput(const std::string& ipFileName);
		bool AssignParticle(particle &p);
};

class ROOTFileReader : public ParticleFileReader
{
	TFile* File_=nullptr;
	TTree*  Data_=nullptr; 
	
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
		ROOTFileReader() {}
		ROOTFileReader(const std::string& ipFileName){ UpdateFileInput(ipFileName); }
		void UpdateFileInput(const std::string& ipFileName);
		bool AssignParticle(particle &p);
		void Close();
};

class SATRAMFileReader : public ParticleFileReader
{
	TFile* File_=nullptr;
	TTree*  dscData_=nullptr;
	TTree* ClusterFile_=nullptr;
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
		SATRAMFileReader() {}
		SATRAMFileReader(const std::string& ipFileName){ UpdateFileInput(ipFileName); }
		void UpdateFileInput(const std::string& ipFileName);
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
	
	FILE* ipFile_=nullptr;
	int EntryNumber_=0;
	int Region_=-1;
	int ParticleType_=-1;
	double PrimaryEnergy_;
	double ToA_=-1;
	double MaximumFrameAcquisitionTime_= -1;
	double AcquisitionTime_;
	double TotalAcquisitionTime_=0;
	int AcquisitionRegion_=-2;
	int Size_;
	double Height_;
	double Theta_;
	double Phi_;
	double Energy_;
	double StoppingPower_;
	std::map<double, double> FrameAcquisitionTimeHistory_;
	public:
		StoppingPowerFileReader() {}
		StoppingPowerFileReader(const std::string& ipFileName, int AcquisitionRegion);
		bool ReadNextEntry();
		void SetMaximumFrameAcquisitionTime(double NewMaximumFrameAcquisitionTime);
		int GetEntryNumber() const;
		int GetRegionID() const;
		int GetParticleType() const;
		double GetPrimaryEnergy() const;
		double GetStoppingPower() const;
		double GetToA() const;
		double GetAcquisitionTime() const;
		double GetTotalAcquisitionTime() const;
		std::map<double,double> GetFrameAcquisitionTimeHistory() const;
		int GetSize() const;
		double GetHeight() const;
		double GetTheta() const;
		double GetPhi() const;
		double GetEnergy() const;
		void Close() 
		{
			if(ipFile_!=nullptr)
			{
				fclose(ipFile_);
				ipFile_=nullptr;
			}
		}
};

class BennyFileReader : public ParticleFileReader
{
	TFile* File_=nullptr;
	TTree*  Data_=nullptr; 
	
	int CurrentEntryNo;
	
	int size;
	
	double theta, phi;
	short x[256*256];//int for Stefan SATRAM Simulations
	short y[256*256];//int for Stefan SATRAM Simulations
	float energy[256*256];
	double time[256*256];
	double PrimaryEnergy;
	int ParticleType;
	public:
		BennyFileReader() {}
		BennyFileReader(const std::string& ipFileName){ UpdateFileInput(ipFileName); }
		void UpdateFileInput(const std::string& ipFileName);
		bool AssignParticle(particle &p);
		void Close();
};

