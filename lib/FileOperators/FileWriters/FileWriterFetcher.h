#pragma once
#include <string>

class particle;
class ParticleFileWriter;

std::unique_ptr<ParticleFileWriter> GetFileWriter(const std::string& opFileName, int FileType, double DetectorThickness, const string& ModelPath);
std::unique_ptr<ParticleFileWriter> GetEmptyFileWriter(int FileType, double DetectorThickness, const string& ModelPath);

