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


#include "ParticleFileReaders.h"


ParticleFileReader GetFileReader(const string& ipFileName, int ipFileType)
{
	if(ipFileType==-1)
		ipFileType = GetFileType(ipFileName);
	switch(ipFileType)
	{
		case 0:
			return ROOTFileReader(ipFileName);
			break;
		case 1:
			return PxFileReader(ipFileName);
			break;
		case 2:
			return AngFileReader(ipFileName);
			break;
		case 3:
			return SATRAMFileReader(ipFileName);
			break;
		case 4:
			return SimFileReader(ipFileName);
			break;
		case 10:
			return BennyFileReader(ipFileName);
			break;
		default:
			cout<<ipFileType<<" is an unknown data type(0: .root(standard), 1:_px.txt, 2:_ang.txt, 3: .root(SATRAM), 4:_sim.txt)"<<endl;
			break;
	}
	return ParticleFileReader();
}

ParticleFileReader GetEmptyFileReader(int ipFileType)
{
	switch(ipFileType)
	{
		case 0:
			return ROOTFileReader();
			break;
		case 1:
			return PxFileReader();
			break;
		case 2:
			return AngFileReader();
			break;
		case 3:
			return SATRAMFileReader();
			break;
		case 4:
			return SimFileReader();
			break;
		case 10:
			return BennyFileReader();
			break;
		default:
			cout<<ipFileType<<" is an unknown data type(0: .root(standard), 1:_px.txt, 2:_ang.txt, 3: .root(SATRAM), 4:_sim.txt)"<<endl;
			break;
	}
	return ParticleFileReader();
}


void PxFileReader::UpdateFileInput(const string& ipFileName)
{
	Close();
	ipFile_ =fopen((ipFileName).c_str(),"r");
}

bool PxFileReader::AssignParticle(particle &p)
{
	p.Clear();
	PixelHit hit;
	while(fscanf(ipFile_, "%lf %lf %lf %lf\n", &hit.x, &hit.y, &hit.time, &hit.energy)==4)
	{
		p.Insert(hit);
	}
	char hashtag;
	if(fscanf(ipFile_, "%c\n", &hashtag)!=EOF){
		//if (hashtag!='#') {fprintf(stderr, "What is?   %c\n", hashtag); throw std::runtime_error("input file error, does not follow standard _px file format, # should follow all clusters.");}
		return true;
	}
	else			
	{
		Close();
		return false;
	}
}


void AngFileReader::UpdateFileInput(const string& ipFileName)
{
	Close();
	ipFile_ =fopen((ipFileName).c_str(),"r");
}

bool AngFileReader::AssignParticle(particle &p)
{
	p.Clear();
	bool IsEnd;
	IsEnd = (fscanf(ipFile_, "theta=%lf,phi=%lf\n", &p.theta, &p.phi)!=EOF);
	if(IsEnd)
	{
		PixelHit hit;
		while(fscanf(ipFile_, "%lf %lf %lf %lf\n", &hit.x, &hit.y, &hit.time, &hit.energy)==4)
		{
			p.Insert(hit);
		}
		return true;
	}
	else			
	{
		Close();
		return false;
	}
}


void SimFileReader::UpdateFileInput(const string& ipFileName)
{
	Close();
	ipFile_ =fopen((ipFileName).c_str(),"r");
}

bool SimFileReader::AssignParticle(particle &p)
{
	p.Clear();
	bool IsEnd;
	int s;
	IsEnd = (fscanf(ipFile_, "ParticleType=%d,Size=%d,Energy=%lf,Theta=%lf,Phi=%lf\n", &p.ParticleType, &s, &p.PrimaryEnergy, &p.theta, &p.phi)!=EOF);
	if(IsEnd)
	{
		PixelHit hit;
		while(fscanf(ipFile_, "%lf %lf %lf %lf\n", &hit.x, &hit.y, &hit.time, &hit.energy)==4)
		{
			p.Insert(hit);
		}
		return true;
	}
	else			
	{
		Close();
		return false;
	}
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
	
	p.Clear();
	if(CurrentEntryNo<Data_->GetEntries())
	{
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
		return true;
	}
	return false;
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
			break;
		
		//cout<<occupancy_<<" "<<good_region<<endl;
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
		CurrentClusterEntry++;
	}while((occupancy_<MinOccupancy_) |(occupancy_>MaxOccupancy_) | (good_region==0));
	p.RegionID = GetCurrentOrbitRegion();
	p.AcquisitionTime = acq_time;
	p.SatPosX = SatellitePosition[1];
	p.SatPosY = SatellitePosition[2];
	p.SatAltitude = SatellitePosition[0];
	p.FrameOccupancy = occupancy_;
	//cout<<acq_time<<endl;
	if( (CurrentClusterEntry<ClusterFile_->GetEntries()) & (CurrentSatEntry<dscData_->GetEntries()))
	{
		
		for(int j=0;j<size;j++)
		{
			auto t = PixelHit{static_cast<double>(x[j]), static_cast<double>(y[j]), ctime, energy[j]};
			p.Insert(t);
		}
		return true;
	}
	
	return false;
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
	
	p.Clear();
	if(CurrentEntryNo<Data_->GetEntries())
	{
		Data_->GetEntry(CurrentEntryNo);
		p.theta = theta * (M_PI/180); // deg -> Rad as required by algorithms
		p.phi = phi * (M_PI/180);				  // deg -> Rad as required by algorithms
		p.PrimaryEnergy = PrimaryEnergy*1e-3;// keV -> MeV
		p.ParticleType = ParticleType;
		for(int j=0;j<size;j++)
		{
			//p.Insert(PixelHit{static_cast<double>(x[j]), static_cast<double>(y[j]), 0, energy[j]});
			p.Insert(PixelHit{static_cast<double>(x[j]), static_cast<double>(y[j]), time[j], energy[j]});
		}
		CurrentEntryNo++;
		return true;
	}
	return false;
}

void BennyFileReader::Close() 
{
	if(File_!=nullptr)
	{
		File_->Close();
		File_= nullptr;
	}
}

