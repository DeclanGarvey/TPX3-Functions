#pragma once

#include <string>

void ShowDataFileTypes();
int GetFileType(const std::string& ipFileName);
std::string RemoveExtension(const std::string& FileName);
std::string RemoveDirectoryPath(const std::string& FileName);

