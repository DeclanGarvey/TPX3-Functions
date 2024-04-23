#pragma once
#include <string>
#include <memory>

class ParticleFileReader;

std::unique_ptr<ParticleFileReader> GetFileReader(const std::string& ipFileName, int ipFileType);
std::unique_ptr<ParticleFileReader> GetEmptyFileReader(int ipFileType);

