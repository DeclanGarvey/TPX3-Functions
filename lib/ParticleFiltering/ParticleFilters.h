#pragma once
#include <functional>
#include <string>
#include <ostream>
class particle;

std::function<bool(const particle& p)> GetFilter(const std::string& FilterName, const std::string& FilterValue);

std::function<bool(const particle&)> CreateMinimumSizeFilter(int MinimumSize);
std::function<bool(const particle&)> CreateMaximumSizeFilter(int MaximumSize);
std::function<bool(const particle&)> CreateMinimumEnergyFilter(double MinimumEnergy);
std::function<bool(const particle&)> CreateMaximumEnergyFilter(double MaximumEnergy);
