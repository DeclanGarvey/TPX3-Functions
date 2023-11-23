#include <functional>
#include <iostream>
#include <string>
using namespace std;

#include "Particle.h"
#include "AngleCalulationFunctions.h"
#include "MorphologicalClassification.h"
#include "FluenceStoppingPower.h"

#include "ParticleFilters.h"

function<bool(particle& p)> GetFilter(const string& FilterName, const string& FilterValue)
{
	if(FilterName=="MinimumSize")
		return CreateMinimumSizeFilter(stoi(FilterValue));
	else if(FilterName=="MaximumSize")
		return CreateMaximumSizeFilter(stoi(FilterValue));
	else if(FilterName=="MinimumEnergy")
		return CreateMinimumEnergyFilter(stod(FilterValue));
	else if(FilterName=="MaximumEnergy")
		return CreateMaximumEnergyFilter(stod(FilterValue));
	else if(FilterName=="PrimaryEnergy")	
		return CreatePrimaryEnergyFilter(stod(FilterValue));
	else if(FilterName=="MinimumPrimaryEnergy")
		return CreateMinimumPrimaryEnergyFilter(stod(FilterValue));
	else if(FilterName=="MaximumPrimaryEnergy")
		return CreateMaximumPrimaryEnergyFilter(stod(FilterValue));
	else if(FilterName=="MinimumTheta")
		return CreateMinimumThetaFilter(FilterValue);
	else if(FilterName=="MaximumTheta")
		return CreateMaximumThetaFilter(FilterValue);
	else if(FilterName=="MinimumPhi")
		return CreateMinimumPhiFilter(FilterValue);
	else if(FilterName=="MaximumPhi")
		return CreateMaximumPhiFilter(FilterValue);
	else if(FilterName=="MorphologicalClass")
		return CreateMorphologicalClassFilter(stoi(FilterValue));
	else if(FilterName=="RegionID")
		return CreateRegionIDFilter(stoi(FilterValue));
	else if(FilterName=="MinimumStoppingPower")
		return CreateMinimumStoppingPowerFilter(FilterValue);
	else if(FilterName=="MaximumStoppingPower")
		return CreateMaximumStoppingPowerFilter(FilterValue);
	else 
	{
		cout<<"Warning: Unknown filter type found \'"<<FilterName<<"\' is being ommited along with value \'"<<FilterValue<<"\'."<<endl;
		return [](const particle&) { return true; };
	}
}

function<bool(particle&)> CreateMinimumSizeFilter(int MinimumSize)
{
	return [MinimumSize](particle& p)
	{
		return p.GetSize()>MinimumSize;
	};
}

function<bool(particle&)> CreateMaximumSizeFilter(int MaximumSize)
{
	return [MaximumSize](particle& p)
	{
		return p.GetSize()<MaximumSize;
	};
}


function<bool(particle&)> CreateMinimumEnergyFilter(double MinimumEnergy)
{
	return [MinimumEnergy](particle& p)
	{
		return p.GetEnergy()>MinimumEnergy;
	};
}

function<bool(particle&)> CreateMaximumEnergyFilter(double MaximumEnergy)
{
	return [MaximumEnergy](particle& p)
	{
		return p.GetEnergy()<MaximumEnergy;
	};
}

function<bool(particle&)> CreatePrimaryEnergyFilter(double PrimaryEnergy)
{
	return [PrimaryEnergy](particle& p)
	{
		return p.PrimaryEnergy==PrimaryEnergy;
	};
}

function<bool(particle&)> CreateMinimumPrimaryEnergyFilter(double MinimumPrimaryEnergy)
{
	return [MinimumPrimaryEnergy](particle& p)
	{
		return p.PrimaryEnergy>MinimumPrimaryEnergy;
	};
}

function<bool(particle&)> CreateMaximumPrimaryEnergyFilter(double MaximumPrimaryEnergy)
{
	return [MaximumPrimaryEnergy](particle& p)
	{
		return p.PrimaryEnergy<MaximumPrimaryEnergy;
	};
}

function<bool(particle&)> CreateMinimumThetaFilter(string MinimumThetaInfo)
{
	size_t DelimiterPos = MinimumThetaInfo.find("@");
	double MinimumTheta;
	double DetectorThickness=0.05;
	if(DelimiterPos!=string::npos)
	{
		MinimumTheta = stod(MinimumThetaInfo.substr(0, DelimiterPos));
		DetectorThickness = stod(MinimumThetaInfo.substr(DelimiterPos + 1));
	}
	else 
		MinimumTheta = stod(MinimumThetaInfo);
	return [MinimumTheta,DetectorThickness](particle& p)
	{
		if(p.theta==-1)
			p.theta = ThetaImprovedLLM(p, DetectorThickness);
		return p.theta>MinimumTheta;
	};
}


function<bool(particle&)> CreateMaximumThetaFilter(string MaximumThetaInfo)
{
	size_t DelimiterPos = MaximumThetaInfo.find("@");
	double MaximumTheta;
	double DetectorThickness=0.05;
	if(DelimiterPos!=string::npos)
	{
		MaximumTheta = stod(MaximumThetaInfo.substr(0, DelimiterPos));
		DetectorThickness = stod(MaximumThetaInfo.substr(DelimiterPos + 1));
	}
	else 
		MaximumTheta = stod(MaximumThetaInfo);
	return [MaximumTheta,DetectorThickness] (particle& p)
	{
		if(p.theta==-1)
			p.theta = ThetaImprovedLLM(p, DetectorThickness);
		return p.theta<MaximumTheta;
	};
}

function<bool(particle&)> CreateMinimumPhiFilter(string MinimumPhiInfo)
{
	size_t DelimiterPos = MinimumPhiInfo.find("@");
	double MinimumPhi;
	double DetectorThickness=0.05;
	if(DelimiterPos!=string::npos)
	{
		MinimumPhi = stod(MinimumPhiInfo.substr(0, DelimiterPos));
		DetectorThickness = stod(MinimumPhiInfo.substr(DelimiterPos + 1));
	}
	else 
		MinimumPhi = stod(MinimumPhiInfo);
	return [MinimumPhi,DetectorThickness] (particle& p)
	{
		if(p.phi==-1)
			p.phi = PhiTimeWeighted(p);
		return p.phi>MinimumPhi;
	};
}


function<bool(particle&)> CreateMaximumPhiFilter(string MaximumPhiInfo)
{
	size_t DelimiterPos = MaximumPhiInfo.find("@");
	double MaximumPhi;
	double DetectorThickness=0.05;
	if(DelimiterPos!=string::npos)
	{
		MaximumPhi = stod(MaximumPhiInfo.substr(0, DelimiterPos));
		DetectorThickness = stod(MaximumPhiInfo.substr(DelimiterPos + 1));
	}
	else 
		MaximumPhi = stod(MaximumPhiInfo);
	return [MaximumPhi,DetectorThickness] (particle& p)
	{
		if(p.phi==-1)
			p.phi = PhiTimeWeighted(p);
		return p.phi<MaximumPhi;
	};
}

function<bool(particle&)> CreateMorphologicalClassFilter(int MorphologicalClass)
{
	return [MorphologicalClass](particle& p)
	{
		if(p.MorphologicalClass==-1)
			p.MorphologicalClass = GetMorphologicalClass(p);
		return p.MorphologicalClass==MorphologicalClass;
	};
}

function<bool(particle&)> CreateRegionIDFilter(int RegionID)
{
	return [RegionID](particle& p)
	{
		if((p.RegionID==-1) | (RegionID==-1))
			return true;
		else
			return p.RegionID==RegionID;
	};
}

function<bool(particle&)> CreateMinimumStoppingPowerFilter(string MinimumStoppingPowerInfo)
{
	size_t DelimiterPos = MinimumStoppingPowerInfo.find("@");
	double MinimumStoppingPower;
	double DetectorThickness=0.05;
	if(DelimiterPos!=string::npos)
	{
		MinimumStoppingPower = stod(MinimumStoppingPowerInfo.substr(0, DelimiterPos));
		DetectorThickness = stod(MinimumStoppingPowerInfo.substr(DelimiterPos + 1));
	}
	else 
		MinimumStoppingPower = stod(MinimumStoppingPowerInfo);
	return [MinimumStoppingPower,DetectorThickness] (particle& p)
	{
		if(p.StoppingPower==-1)
		{
			if(p.theta==-1)
				p.theta = ThetaImprovedLLM(p, DetectorThickness);
			
			p.StoppingPower = StoppingPower(p.GetEnergy(), p.theta, DetectorThickness);
		}
		return p.StoppingPower>MinimumStoppingPower;
	};
}

function<bool(particle&)> CreateMaximumStoppingPowerFilter(string MaximumStoppingPowerInfo)
{
	size_t DelimiterPos = MaximumStoppingPowerInfo.find("@");
	double MaximumStoppingPower;
	double DetectorThickness=0.05;
	if(DelimiterPos!=string::npos)
	{
		MaximumStoppingPower = stod(MaximumStoppingPowerInfo.substr(0, DelimiterPos));
		DetectorThickness = stod(MaximumStoppingPowerInfo.substr(DelimiterPos + 1));
	}
	else 
		MaximumStoppingPower = stod(MaximumStoppingPowerInfo);
	return [MaximumStoppingPower,DetectorThickness] (particle& p)
	{
		if(p.StoppingPower==-1)
		{
			if(p.theta==-1)
				p.theta = ThetaImprovedLLM(p, DetectorThickness);
			
			p.StoppingPower = StoppingPower(p.GetEnergy(), p.theta, DetectorThickness);
		}
		return p.StoppingPower<MaximumStoppingPower;
	};
}


