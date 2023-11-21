
#pragma once
#include <string>

void ConvertTo(const std::string& ipFileName, const std::string& opFileName, int ipFileType, int opFileType, double DetectorThickness, bool RemoveEdges,
		 const std::string& ModelPath, const std::string& SelectionConfigFilePath);
void ConvertDirectoryTo(const std::string& ipDirectoryName, const std::string& opFileName, int ipFileType, int opFileType, double DetectorThickness, bool RemoveEdges,
			 const std::string& ModelPath, const std::string& SelectionConfigFilePath);
