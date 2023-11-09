#pragma once

#include <string>
class particle;
//#include "PixelStructure.h"

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
