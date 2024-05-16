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
std::function<bool(particle&)> CreateMinimumHeightFilter(double MinimumHeight);
std::function<bool(particle&)> CreateMaximumHeightFilter(double MaximumHeight);
std::function<bool(particle&)> CreatePrimaryEnergyFilter(double PrimaryEnergy);
std::function<bool(particle&)> CreateMinimumPrimaryEnergyFilter(double MinimumPrimaryEnergy);
std::function<bool(particle&)> CreateMaximumPrimaryEnergyFilter(double MaximumPrimaryEnergy);
std::function<bool(particle&)> CreateMinimumThetaFilter(std::string MinimumThetaInfo);
std::function<bool(particle&)> CreateMaximumThetaFilter(std::string MaximumThetaInfo);
std::function<bool(particle&)> CreateMinimumPredictedThetaFilter(std::string MinimumThetaInfo);
std::function<bool(particle&)> CreateMaximumPredictedThetaFilter(std::string MaximumThetaInfo);
std::function<bool(particle&)> CreateMinimumPhiFilter(std::string MinimumPhiInfo);
std::function<bool(particle&)> CreateMaximumPhiFilter(std::string MaximumPhiInfo);
std::function<bool(particle&)> CreateMinimumPredictedPhiFilter(std::string MinimumPhiInfo);
std::function<bool(particle&)> CreateMaximumPredictedPhiFilter(std::string MaximumPhiInfo);
std::function<bool(particle&)> CreateMorphologicalClassFilter(int MorphologicalClass);
std::function<bool(particle&)> CreateRegionIDFilter(int RegionID);
std::function<bool(particle&)> CreateMinimumStoppingPowerFilter(std::string MinimumStoppingPowerInfo);
std::function<bool(particle&)> CreateMaximumStoppingPowerFilter(std::string MaximumStoppingPowerInfo);
std::function<bool(particle&)> CreateMinimumEndPointsFilter(int MinimumEndPoints);
std::function<bool(particle&)> CreateMaximumEndPointsFilter(int MaximumEndPoints);
std::function<bool(particle&)> CreateLIPsFilter(bool KeepLIPs);
std::function<bool(particle&)> CreateMaximumAcquisitionTimeFilter(double MaximumAcquisitionTime);

std::function<bool(particle&)> CreateMorphologicalClassRemovalFilter(int MorphologicalClass);
