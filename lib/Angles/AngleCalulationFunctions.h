
#pragma once 

class Particle;
struct PixelHit;

particle Skeletonise(particle const& p, double a, double b);
double NormalizeIncidentAngle(double theta);
double Distance(PixelHit const& p1, PixelHit const& p2);
bool GetFuncType(particle const& p);

double ThetaLLM(particle const& p, double d);
double ThetaImprovedLLM(particle const& p, double d);
double ThetaMinMax(particle const& p, double d);
double ThetaLineFit(particle const& p, double d);

std::pair<bool,bool> Flipxy(particle const& p);
double TwoPointPhi(PixelHit const& p1, PixelHit const& p2);

double PhiLLMVert(particle const& p);
double PhiLLMHori(particle const& p);
double PhiImprovedLLM(particle const& p);
double PhiMinMax(particle const& p);
double PhiLineFit(particle const& p);
double PhiTimeWeighted(particle const& p);
