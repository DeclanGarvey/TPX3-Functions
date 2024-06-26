#pragma once

#include <string>
class particle;
//#include "PixelStructure.h"

std::vector<double> LineFit(particle const& p);
double PixelsInLine(particle const&p, int XFuncOrYYFunc, PixelHit const& p1, PixelHit const& p2);
double Linearity(particle const& p);
double LLMLinearity(particle const& p);
particle RotateToNormal(particle const& p);
std::vector<double> BoxStds(particle const& p);
std::vector<double> WeightedBoxStds(particle const& p);
std::vector<double> BoxDimensions(particle const& p);
double StandardDeviationTime(particle const& p);
double StandardDeviationEnergy(particle const& p);
double EnergySizeRatio(particle const& p, double EnergyCut);
double Length(particle const& p);
double NearestNeighbours8Fold(particle const& p);
double NearestNeighbours4Fold(particle const& p);
double NumberOfInnerPixels(particle const& p);
double DiameterOfCluster(particle const& p);
double PixelDistance(PixelHit const& p1, PixelHit const& p2);
double MaximumDistance(particle const& p);
int ElectronCloudSize(particle const& p);
double ElectronCloudEnergy(particle const& p);
double EnergyOfLine(particle const& p);
int NumberOfEndPoints(particle const& p);
double MedianEnergy(particle const& p);
double LineProjectionGradient(particle const& p);
double HaloDistance(particle const& p);
bool ContainsInner8FoldPixel(particle const& p);
bool ContainsInner4FoldPixel(particle const& p);
