using namespace std;

#include "Particle.h"
#include "ParticleFileReaders.h"
#include "ConfigFileReader.h"
#include "ParticleFilterFunctionals.h"

#include "ParticleFilter.h"

ParticleFilter::ParticleFilter(const string& ConfigFileFilters)
{
	GenerateFiltersFromConfig(ConfigFileFilters);
}

void ParticleFilter::AddFilter(const std::string& VariableFilter, const std::string& Value)
{
	ParticleFilters_.push_back(GetFilter(VariableFilter, Value));
}

void ParticleFilter::AddFilter(function<bool(particle& p)> NewFilter)
{
	ParticleFilters_.push_back(NewFilter);
}

void ParticleFilter::GenerateFiltersFromConfig(const string& ConfigFileFilters)
{
	if(ConfigFileFilters.empty()==false)
	{
		ConfigFileReader* configreader = new ConfigFileReader(ConfigFileFilters);
		while(configreader->ProcessNextVariable())
		{
			AddFilter(GetFilter(configreader->GetCurrentVariableName(), configreader->GetCurrentVariableValue()));
		}
	}
}

bool ParticleFilter::operator() (particle& p)
{
	bool KeepParticle=true;
	for(size_t i=0;(KeepParticle) & (i<ParticleFilters_.size());i++)
	{
		KeepParticle = ParticleFilters_[i](p);
	}
	return KeepParticle;
}

