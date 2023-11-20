#pragma once
#include <string>

void Deconvolution(const std::string&  ResponseFileName, const std::string& ipDirectory, const std::string& opDirectory,double DetectorThickness, int RegionID);
void DeconvolutionDirectory(const std::string&  ResponseFileName, const std::string& ipDirectory, const std::string& opDirectory,double DetectorThickness, int RegionID);
