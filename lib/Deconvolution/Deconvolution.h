#pragma once
#include <string>

void Deconvolution(const std::string&  ResponseFileName, const std::string& ipDirectory, const std::string& opDirectory,double DetectorThickness, int RegionID);
void DeconvolutionDirectory(const std::string&  ResponseFileName, const std::string& ipDirectory, const std::string& opDirectory,double DetectorThickness, int RegionID);
//void UnfoldWithBackground(const std::string& ResponseFileName, const std::string& ipDirectory, const std::string& opDirectory, const std::string& BackgroundFile);
//void UnfoldStoppingPowerFile(const std::string& ResponseFileName, const std::string& ipDirectory, const std::string& opDirectory, int Region, bool Merge);
//void UnfoldStoppingPowerFile(const std::string& ResponseFileName, const std::string& ipDirectory, const std::string& opDirectory, int Region);
