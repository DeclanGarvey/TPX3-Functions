
#pragma once

#include <iostream>
#include <string>
#include <functional>

class particle;

class ParticleFilter 
{
	private:
		std::vector<std::function<bool(particle& p)>> ParticleFilters_;
	public:
		ParticleFilter(const std::string& ConfigFileFilters);
		ParticleFilter() {};
		
		void AddFilter(std::function<bool(particle& p)> NewFilter);
		void AddFilter(const std::string& VariableFilter, const std::string& Value);
		void GenerateFiltersFromConfig(const std::string& ConfigFileFilters);
		
		bool operator() (particle& p);
};
