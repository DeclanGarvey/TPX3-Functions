#pragma once
#include <string>


class ParticleFileReader;

ParticleFileReader GetFileReader(const std::string& ipFileName, int ipFileType);
ParticleFileReader GetEmptyFileReader(int ipFileType);

