#pragma once
#include <functional>
#include <string>
#include <ostream>
class particle;

std::function<bool(particle& p)> GetFilter(const std::string& FilterName, const std::string& FilterValue);

std::function<bool(particle&)> CreateMinimumSizeFilter(int MinimumSize);
std::function<bool(particle&)> CreateMaximumSizeFilter(int MaximumSize);
std::function<bool(particle&)> CreateMinimumEnergyFilter(double MinimumEnergy);
std::function<bool(particle&)> CreateMaximumEnergyFilter(double MaximumEnergy);
std::function<bool(particle&)> CreatePrimaryEnergyFilter(double PrimaryEnergy);
std::function<bool(particle&)> CreateMinimumPrimaryEnergyFilter(double MinimumPrimaryEnergy);
std::function<bool(particle&)> CreateMaximumPrimaryEnergyFilter(double MaximumPrimaryEnergy);
std::function<bool(particle&)> CreateMinimumThetaFilter(std::string MinimumThetaInfo);
std::function<bool(particle&)> CreateMaximumThetaFilter(std::string MaximumThetaInfo);
std::function<bool(particle&)> CreateMinimumPhiFilter(std::string MinimumPhiInfo);
std::function<bool(particle&)> CreateMaximumPhiFilter(std::string MaximumPhiInfo);
std::function<bool(particle&)> CreateMorphologicalClassFilter(int MorphologicalClass);
std::function<bool(particle&)> CreateRegionIDFilter(int RegionID);

