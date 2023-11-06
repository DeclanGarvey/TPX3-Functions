#pragma once
#include <string>
#include <map>
#include <vector>


void ConstructResponse(const std::string&  ipDirectory, const std::string&  opDirectory,  const std::string& ConfigurationDirectory, double DetectorThickness);
void ResponseConstructionFromDirectory(const std::string& ipDirectory, const std::string& opDirectory, const std::string& ConfigurationDirectory, double DetectorThickness);
void PrintUnfoldingMatrix(const std::string& ResponseFileName);//, const std::string& opFileName);
