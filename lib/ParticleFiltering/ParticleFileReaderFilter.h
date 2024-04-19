
#pragma once

#include <iostream>
#include <string>
#include <functional>


class ParticleFileReader;
class particle;

#include "ParticleGeneratorBaseClass.h"

class ParticleFileFilter : public ParticleGeneratorBaseClass
{
	private:
		ParticleFileReader* reader;
		std::vector<std::function<bool(particle& p)>> ParticleFilters_;
	public:
		ParticleFileFilter(ParticleFileReader* reader, const std::string& ConfigFileFilters);
		ParticleFileFilter(ParticleFileReader* reader);
		ParticleFileFilter(const std::string& ConfigFileFilters);
		
		void AddFilter(std::function<bool(particle& p)> NewFilter);
		void operator() (ParticleFileReader* NewReader);
		void GenerateFiltersFromConfig(const std::string& ConfigFileFilters);
		bool AssignParticle(particle& p);
		~ParticleFileFilter();
};
