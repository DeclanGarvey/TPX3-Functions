
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
		std::vector<std::function<bool(const particle& p)>> ParticleFilters_;
	public:
		ParticleFileFilter(ParticleFileReader* reader, const std::string& ConfigFileFilters);
		void GenerateFiltersFromConfig(const std::string& ConfigFileFilters);
		bool AssignParticle(particle& p);
};
