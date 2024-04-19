#pragma once
#include <string>

class particle;
class ParticleFileWriter;

ParticleFileWriter GetFileWriter(const std::string& opFileName, int FileType, double DetectorThickness, const string& ModelPath);
ParticleFileWriter GetEmptyFileWriter(int FileType, double DetectorThickness, const string& ModelPath);

