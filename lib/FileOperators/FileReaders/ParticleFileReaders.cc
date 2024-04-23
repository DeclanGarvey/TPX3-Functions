#include <math.h>
#include <map>
using namespace std;

#include "TTree.h"
#include "TSystemDirectory.h"
#include "TFile.h"
#include "TH1.h"

#include "Particle.h"
#include "FileNames.h"
#include "FittingVariables.h"
#include "FluenceStoppingPower.h"
#include "ParticleFilter.h"

#include "ParticleFileReaders.h"

void ParticleFileReader::AddParticleFiltersFromConfig(const std::string& FilterConfig)
{
	Filter_=ParticleFilter(FilterConfig);
}
void ParticleFileReader::AddParticleFiltersFromConfig(ParticleFilter& Filter)
{
	Filter_ = ParticleFilter();
}
void ParticleFileReader::AddParticleFilter(const std::string& VariableFilter, const std::string& Value)
{
	Filter_.AddFilter(VariableFilter, Value);
}

PxFileReader::PxFileReader(const std::string& ipFileName)
{ 
	UpdateFileInput(ipFileName);
	
}

void PxFileReader::UpdateFileInput(const string& ipFileName)
{
	Close();
	ipFile_ =fopen((ipFileName).c_str(),"r");
	
}

bool PxFileReader::AssignParticle(particle &p)
{
	
	PixelHit hit;
	char hashtag;
	do
	{
		p.Clear();
		while(fscanf(ipFile_, "%lf %lf %lf %lf\n", &hit.x, &hit.y, &hit.time, &hit.energy)==4)
		{
			p.Insert(hit);
		}
		if(fscanf(ipFile_, "%c\n", &hashtag)==EOF)
		{
			Close();
			return false;
		}
	}
	while(Filter_(p)==false);
	return true;
}


void SimFileReader::UpdateFileInput(const string& ipFileName)
{
	Close();
	ipFile_ =fopen((ipFileName).c_str(),"r");
}

bool SimFileReader::AssignParticle(particle &p)
{
	PixelHit hit;
	int s;
	do
	{
		p.Clear();
		if(fscanf(ipFile_, "ParticleType=%d,Size=%d,Energy=%lf,Theta=%lf,Phi=%lf\n", &p.ParticleType, &s, &p.PrimaryEnergy, &p.theta, &p.phi)==EOF)
		{
			Close();
			return false;
		}
		
		while(fscanf(ipFile_, "%lf %lf %lf %lf\n", &hit.x, &hit.y, &hit.time, &hit.energy)==4)
		{
			p.Insert(hit);
		}
	}
	while(Filter_(p)==false);
	return true;
}


void ROOTFileReader::UpdateFileInput(const string& ipFileName)
{
	Close();

	File_ = new TFile(ipFileName.c_str(), "OPEN");
	Data_ = (TTree*)  File_->Get("clusteredData");
	
	Data_->SetBranchAddress("countervalue_keV", energy);
	Data_->SetBranchAddress("PixX", x);
	Data_->SetBranchAddress("PixY", y);
	Data_->SetBranchAddress("clstrSize", &size);
	Data_->SetBranchAddress("theta_deg", &theta);
	Data_->SetBranchAddress("phi_deg", &phi);
	Data_->SetBranchAddress("primaryEnergy_keV",&PrimaryEnergy);
	//Data_->SetBranchAddress("Coincidences", Concidences)
	if(RemoveDirectoryPath(ipFileName).substr(0,6)=="proton")
		ParticleType = 2212;
	else if(RemoveDirectoryPath(ipFileName).substr(0,8)=="electron")
		ParticleType = 11;
	else 
		ParticleType = -1;
	CurrentEntryNo=0;
}
bool ROOTFileReader::AssignParticle(particle &p)
{
	do
	{
		p.Clear();
		if(CurrentEntryNo>=Data_->GetEntries())
		{
			Close();
			return false;
		}
		Data_->GetEntry(CurrentEntryNo);
		p.theta = theta * (M_PI/180); 	// deg -> Rad as required by algorithms
		p.phi = phi * (M_PI/180);	// deg -> Rad as required by algorithms
		p.PrimaryEnergy = PrimaryEnergy*1e-3;// keV -> MeV
		p.ParticleType = ParticleType;
		for(int j=0;j<size;j++)
		{
			p.Insert(PixelHit{static_cast<double>(x[j]), static_cast<double>(y[j]), 0, energy[j]});
		}
		CurrentEntryNo++;
	}
	while(Filter_(p)==false);
	return true;
}
void ROOTFileReader::Close() 
{
	if(File_!=nullptr)
	{
		File_->Close();
		File_= nullptr;
	}
}


void SATRAMFileReader::UpdateFileInput(const string& ipFileName)
{
	Close();
	
	File_ = new TFile(ipFileName.c_str(), "OPEN");
	dscData_ = (TTree*)  File_->Get("dscData");
	ClusterFile_ = (TTree*) File_->Get("clusterFile");
	
	dscData_->SetBranchAddress("Start_time",&dtime);
	dscData_->SetBranchAddress("satellite_position", SatellitePosition);
	ClusterFile_->SetBranchAddress("clstrSize",&size);
	ClusterFile_->SetBranchAddress("Start_time",&ctime);
	ClusterFile_->GetBranch("CounterValue_keV")->SetAddress(energy);
	ClusterFile_->SetBranchAddress("PixX", x);
	ClusterFile_->SetBranchAddress("PixY", y);
	ClusterFile_->SetBranchAddress("clstrRegion",&good_region);
	ClusterFile_->SetBranchAddress("Occupation",&occupancy_);
	ClusterFile_->SetBranchAddress("Acq_time",&acq_time);
	CurrentClusterEntry=0;
	CurrentSatEntry=0;

}

bool SATRAMFileReader::AssignParticle(particle &p)
{
	p.Clear();
	do	
	{
		if( (CurrentClusterEntry>=ClusterFile_->GetEntries()) | (CurrentSatEntry>=dscData_->GetEntries()))
		{
			Close();
			return false;
		}
		
		ClusterFile_->GetEntry(CurrentClusterEntry);
		dscData_->GetEntry(CurrentSatEntry);
		while( dtime != ctime )
		{
			CurrentSatEntry++;
			if(CurrentSatEntry>=dscData_->GetEntries())
				break;
			else
				dscData_->GetEntry(CurrentSatEntry);
		}
		for(int j=0;j<size;j++)
		{
			auto t = PixelHit{static_cast<double>(x[j]), static_cast<double>(y[j]), ctime, energy[j]};
			p.Insert(t);
		}
		CurrentClusterEntry++;
	}while((occupancy_<MinOccupancy_) |(occupancy_>MaxOccupancy_) | (good_region==0) | (Filter_(p)==false));
	
	p.RegionID = GetCurrentOrbitRegion();
	p.AcquisitionTime = acq_time;
	p.SatPosX = SatellitePosition[1];
	p.SatPosY = SatellitePosition[2];
	p.SatAltitude = SatellitePosition[0];
	p.FrameOccupancy = occupancy_;
	
	return true;
	
}
void SATRAMFileReader::SetMaximumOccupancy(double NewMaxOccupancy)
{
	MaxOccupancy_=NewMaxOccupancy;
}

void SATRAMFileReader::SetMinimumOccupancy(double NewMinOccupancy)
{
	MinOccupancy_=NewMinOccupancy;
}
float SATRAMFileReader::GetCurrentOccupancy()
{
	return occupancy_;
}
int SATRAMFileReader::GetCurrentOrbitRegion()
{
	if((SatellitePosition[2]<-12) & (SatellitePosition[2]>-40) & (SatellitePosition[1]>-70) & (SatellitePosition[1]<-25))
	{
		return 3; //Position Corresponding Approximately to the South Atlantic Anomally
	}
	else if ((SatellitePosition[2]<17) & (SatellitePosition[2]>-62) & (SatellitePosition[1]>-145) & (SatellitePosition[1]<45))
	{
		return -1; //Cloud around the SAA
	}
	else if((SatellitePosition[2]>40) & (SatellitePosition[2]<75))
	{
		return 2; //Position Corresponding Approximately to the Northern Horn
	}
	else if((SatellitePosition[2]<-40))
	{
		return 1; //Position Corresponding Approximately to the Southern Horn
	}
	else
		return 0; //Position Outside of any high flux region
}
double SATRAMFileReader::GetAcquisitionTime()
{
	return acq_time;
}

void SATRAMFileReader::Close()
{
	if(File_!=nullptr)
	{
		File_->Close();
		File_= nullptr;
	}
}

StoppingPowerFileReader::StoppingPowerFileReader(const string& ipFileName, int AcquisitionRegion)
{
	ipFile_ =fopen((ipFileName).c_str(),"r");
	AcquisitionRegion_ = AcquisitionRegion;
}
bool StoppingPowerFileReader::ReadNextEntry()
{
	double PreviousToA = ToA_;
	do
		if(fscanf(ipFile_, "%d %d %lf %lf %lf %d %lf %lf %lf %lf %lf\n",
				 &Region_, &ParticleType_, &PrimaryEnergy_, &ToA_, &AcquisitionTime_, &Size_, &Energy_,&Height_, &Theta_, &Phi_, &StoppingPower_)==11)
		{
			EntryNumber_++;
		}
		else
		{
			Close();
			return false;
		}
	while( (abs(AcquisitionTime_)<1e-4) | ((AcquisitionTime_>MaximumFrameAcquisitionTime_) & (MaximumFrameAcquisitionTime_!=-1)) | 
		(((AcquisitionRegion_==-2) | (Region_==AcquisitionRegion_))==false)  | 
		((Size_<=2) & (Height_>1000)) | (Height_>1500));
	
	// Note: AcquisitionTime_<1e-6 was an error found in root data files and hence these frames are neglected 
	// whilst AcquisitionTime_=-1 indicates the acquisition time is not relevent and the frame should still be considered, commonly used for simulation data or Timepix3 data
	if(PreviousToA!=ToA_){
		if(AcquisitionTime_!=-1)
		{
			TotalAcquisitionTime_ += AcquisitionTime_;
			
			//Rounding to 4 significant figures
			double RoundFactor = pow(10.0, 4 - ceil(log10(AcquisitionTime_))); 
	    		double RoundedAcquisitionTime = round(AcquisitionTime_ * RoundFactor) / RoundFactor;
	    		
			if(FrameAcquisitionTimeHistory_.find(RoundedAcquisitionTime)== FrameAcquisitionTimeHistory_.end())
				FrameAcquisitionTimeHistory_.insert({RoundedAcquisitionTime, 1});
			else
				FrameAcquisitionTimeHistory_[RoundedAcquisitionTime] = FrameAcquisitionTimeHistory_[RoundedAcquisitionTime] + 1;
		}
	}
	return true;
}
void StoppingPowerFileReader::SetMaximumFrameAcquisitionTime(double NewMaximumFrameAcquisitionTime)
{
	MaximumFrameAcquisitionTime_ = NewMaximumFrameAcquisitionTime;
}

int StoppingPowerFileReader::GetEntryNumber() const
{
	return EntryNumber_;
}
int StoppingPowerFileReader::GetRegionID() const
{
	return Region_;
}
int StoppingPowerFileReader::GetParticleType() const
{
	return ParticleType_;
}
double StoppingPowerFileReader::GetPrimaryEnergy() const
{
	return PrimaryEnergy_;
}
double StoppingPowerFileReader::GetStoppingPower() const
{
	return StoppingPower_;
}
double StoppingPowerFileReader::GetToA() const
{
	return ToA_;
}
double StoppingPowerFileReader::GetAcquisitionTime() const
{
	return AcquisitionTime_;
}
double StoppingPowerFileReader::GetTotalAcquisitionTime() const
{
	return TotalAcquisitionTime_;
}
map<double,double> StoppingPowerFileReader::GetFrameAcquisitionTimeHistory() const 
{
	return FrameAcquisitionTimeHistory_;
}
int StoppingPowerFileReader::GetSize() const
{
	return Size_;
}
double StoppingPowerFileReader::GetHeight() const
{
	return Height_;
}
double StoppingPowerFileReader::GetTheta() const
{
	return Theta_;
}
double StoppingPowerFileReader::GetPhi() const
{
	return Phi_;
}
double StoppingPowerFileReader::GetEnergy() const
{
	return Energy_;
}

void BennyFileReader::UpdateFileInput(const string& ipFileName)
{
	Close();

	File_ = new TFile(ipFileName.c_str(), "OPEN");
	Data_ = (TTree*)  File_->Get("clusteredData");
	
	Data_->SetBranchAddress("ToA", &time);//!!!!!
	//Data_->SetBranchAddress("countervalue_keV", energy);
	Data_->SetBranchAddress("ToT_keV", energy);
	Data_->SetBranchAddress("PixX", x);
	Data_->SetBranchAddress("PixY", y);
	
	Data_->SetBranchAddress("clstrSize", &size);
	
	//Data_->SetBranchAddress("theta_deg", &theta);
	//Data_->SetBranchAddress("phi_deg", &phi);
	//Data_->SetBranchAddress("primaryEnergy_keV",&PrimaryEnergy);
	if(RemoveDirectoryPath(ipFileName).substr(0,6)=="proton")
		ParticleType = 2212;
	else if(RemoveDirectoryPath(ipFileName).substr(0,8)=="electron")
		ParticleType = 11;
	else 
		ParticleType = -1;
	CurrentEntryNo=0;
}

bool BennyFileReader::AssignParticle(particle &p)
{
	
	do
	{
		p.Clear();
		if(CurrentEntryNo>=Data_->GetEntries())
		{
			Close();
			return false;
		}
		Data_->GetEntry(CurrentEntryNo);
		p.theta = theta * (M_PI/180); // deg -> Rad as required by algorithms
		p.phi = phi * (M_PI/180);				  // deg -> Rad as required by algorithms
		p.PrimaryEnergy = PrimaryEnergy*1e-3;// keV -> MeV
		p.ParticleType = ParticleType;
		for(int j=0;j<size;j++)
		{
			p.Insert(PixelHit{static_cast<double>(x[j]), static_cast<double>(y[j]), time[j], energy[j]});
		}
		CurrentEntryNo++;
	}
	while(Filter_(p)==false);
	return true;
}

void BennyFileReader::Close() 
{
	if(File_!=nullptr)
	{
		File_->Close();
		File_= nullptr;
	}
}

