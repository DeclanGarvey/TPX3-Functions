#pragma once

#include <string>

void ShowAngleCalculationMethods();
void CalculateAngles(const std::string& ipFileName, const std::string& opFileName, int ipFileType, double DetectorThickness, int theta_method, int phi_method, bool RemoveEdges);

