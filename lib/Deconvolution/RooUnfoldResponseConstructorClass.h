#pragma once
#include <string>
#include <map>
#include <vector>

class TH1D;
class RooUnfoldResponseConstructorClass
{
	private:
		TH1D* truth;
		int NoOfParticleGroups=2;
		std::vector<int> ParticleTypes={2212,11};
		int NoStoppingPowerBins = 54;//60;//
		double MinStoppingPower = 3.0;//0;//
		double MaxStoppingPower = 30.0;
		double StoppingPowerBinWidth=(MaxStoppingPower-MinStoppingPower)/NoStoppingPowerBins;
		
		std::vector<int> NoEnergyBins= {77, 5};
		int TotalNoEnergyBins=77+5;
		std::vector<double> MinEnergy= {15, 0};
		std::vector<double> MaxEnergy= {400, 5};
		std::vector<double> EnergyBinWidth= {5,1};
		double* ValsOutsideRange;
		
		
		std::vector<double> SimSizes={0.9825*1000000,2000000};//We are assuming flat spectrum simulaiton!!
		map<int,int> ParticleTypeToParamPosition;
		std::vector<double> MatrixPositions;
		TH2D* ResponseMatrix;
	public:	
		void SetResponseConstrutionParameters(const string& FileName);
		void Initialise();
		void AddEntry(int ParticleType, double PrimaryEnergy, double StoppingPower,double weight);
		void AddFile(const std::string& ipFileName, double DetectorThickness);
		RooUnfoldResponse GetRooUnfoldResponse();
};
