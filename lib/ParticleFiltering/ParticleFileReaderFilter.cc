using namespace std;

#include "Particle.h"
#include "ParticleFileReaders.h"
#include "ConfigFileReader.h"
#include "ParticleFilters.h"

#include "ParticleFileReaderFilter.h"

ParticleFileFilter::ParticleFileFilter(ParticleFileReader* reader, const std::string& ConfigFileFilters) : reader(reader) 
{ 
	GenerateFiltersFromConfig(ConfigFileFilters);
}

ParticleFileFilter::ParticleFileFilter(ParticleFileReader* reader) : reader(reader) 
{
	
}

ParticleFileFilter::ParticleFileFilter(const string& ConfigFileFilters)
{
	GenerateFiltersFromConfig(ConfigFileFilters);
}
void ParticleFileFilter::operator() (ParticleFileReader* NewReader)
{
	reader->Close();
	reader = NewReader;
}

void ParticleFileFilter::AddFilter(function<bool(particle& p)> NewFilter)
{
	ParticleFilters_.push_back(NewFilter);
}

void ParticleFileFilter::GenerateFiltersFromConfig(const string& ConfigFileFilters)
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

bool ParticleFileFilter::AssignParticle(particle& p)
{
	bool KeepParticle=false;
	bool NotEndOfFile=true;
	while((NotEndOfFile) & (KeepParticle==false))
	{
		NotEndOfFile = reader->AssignParticle(p);
		KeepParticle=true;
		for(size_t i=0;(KeepParticle) & (i<ParticleFilters_.size());i++)
		{
			KeepParticle = ParticleFilters_[i](p);
			
		}
	}
	return NotEndOfFile;
}
ParticleFileFilter::~ParticleFileFilter()
{
	delete reader;
}
