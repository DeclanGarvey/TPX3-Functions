#pragma once
#include <string>
#include <map>
#include <vector>

class TH1D;
class RooUnfoldResponseConstructorClass
{
	private:
		size_t NoOfParticleGroups=2;
		std::vector<int> ParticleTypes={2212,11};
		map<int,int> ParticleTypeToParamPosition;
		
		
		int NoStoppingPowerBins = 108;
		double MinStoppingPower = 3.0;
		double MaxStoppingPower = 30.0;
		double StoppingPowerBinWidth;
		
		
		std::vector<std::vector<double>> EnergyBinEdges = {{15,20,30,40,50,60,70,80,90,100,125,150,175,200,300,400},
								{0,1,2,3,4,5,6}};
								
		//Variables are initialised at run time and related to EnergyBinEdges 
		std::vector<size_t> MatrixStartPoint;
		std::vector<double> MinEnergy;
		std::vector<double> MaxEnergy;
		TH2D* ResponseMatrix;
		TH1D* truth;
		TH1D* measured;
	public:	
		RooUnfoldResponseConstructorClass();
		RooUnfoldResponseConstructorClass(const string& FileName);
		
		void Initialise();
		void AddEntry(int ParticleType, double PrimaryEnergy, double StoppingPower,double weight);
		void AddFile(const std::string& ipFileName, double DetectorThickness);
		RooUnfoldResponse GetRooUnfoldResponse();
};
