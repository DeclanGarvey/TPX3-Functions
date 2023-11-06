#pragma once

#include <string>
class TH1D;

double StoppingPower(double EnergyDeposition, double theta, double d);
double LandauGaussianFunction(double *x, double *par);
double FleunceContribution(double theta, double d);
double PhiFluenceContribution(double theta,double phi);
double TheoreticalThetaHitDistribution(double *theta, double *par);
TH1D* GetStoppingPowerFluenceCurve(const std::string& ipFileName,  int ipFileType, double d, const std::string&  Name, const std::string&  Title, int NoBins, double XLow, double XHigh);
void NormaliseCurve(TH1D* Curve);
void SubtractCurves(TH1D* Curve1, TH1D* Curve2);
void PrintHistogram(std::FILE* opFile, TH1D* Curve);
