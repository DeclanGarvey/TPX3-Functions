#pragma once
#include <string>


void MLClassificationFromDirectory(const std::string& ClassifierModelPath, const std::string& SpectroscopeModelPath, const std::string& ipDirectory, const std::string& opDirectory,  int ipFileType, int RegionID);
