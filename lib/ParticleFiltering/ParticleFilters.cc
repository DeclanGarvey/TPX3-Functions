#include <functional>
#include <iostream>
#include <string>
using namespace std;

#include "Particle.h"

#include "ParticleFilters.h"

function<bool(const particle& p)> GetFilter(const string& FilterName, const string& FilterValue)
{
	if(FilterName=="MinimumSize")
		return CreateMinimumSizeFilter(stoi(FilterValue));
	else if(FilterName=="MaximumSize")
		return CreateMaximumSizeFilter(stoi(FilterValue));
	else if(FilterName=="MinimumEnergy")
		return CreateMinimumEnergyFilter(stod(FilterValue));
	else if(FilterName=="MaximumEnergy")
		return CreateMaximumEnergyFilter(stod(FilterValue));
	else 
	{
		cout<<"Warning: Unknown filter type found \'"<<FilterName<<"\' is being ommited"<<endl;
		return [](const particle&) { return true; };
	}
}

function<bool(const particle&)> CreateMinimumSizeFilter(int MinimumSize)
{
	return [MinimumSize](const particle& p)
	{
		return p.GetSize()>MinimumSize;
	};
}

function<bool(const particle&)> CreateMaximumSizeFilter(int MaximumSize)
{
	return [MaximumSize](const particle& p)
	{
		return p.GetSize()<MaximumSize;
	};
}


function<bool(const particle&)> CreateMinimumEnergyFilter(double MinimumEnergy)
{
	return [MinimumEnergy](const particle& p)
	{
		return p.GetEnergy()>MinimumEnergy;
	};
}

function<bool(const particle&)> CreateMaximumEnergyFilter(double MaximumEnergy)
{
	return [MaximumEnergy](const particle& p)
	{
		return p.GetEnergy()<MaximumEnergy;
	};
}
