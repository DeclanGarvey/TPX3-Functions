#include <iostream>
using namespace std;

#include "FluenceStoppingPower.h"
#include "RandomForrestTheta.h"
#include "Particle.h"
#include "ClusterFeatures.h"
#include "AngleCalulationFunctions.h"
#include "ParticleFileWriters.h"

#include "MLStoppingPowerFileWriter.h"

MLSpFileWriter::MLSpFileWriter(const string& opFileName, double DetectorThickness, const string& ModelPath) : SpFileWriter(opFileName, DetectorThickness)
{	
	ThetaModel = std::make_unique<RandomForrestTheta>(ModelPath);
}
MLSpFileWriter::MLSpFileWriter(double DetectorThickness, const string& ModelPath) : SpFileWriter(DetectorThickness)
{	
	ThetaModel = std::make_unique<RandomForrestTheta>(ModelPath);
}

bool MLSpFileWriter::AddParticle(particle const& p)
{
	if(p.IsEmpty()==false)
	{
		float theta = ThetaModel->PredictFromParticle(p);
		double phi = PhiLineFit(p);
		double sp = StoppingPower(p.GetEnergy(), theta, DetectorThickness_); 
		double AcquisitionTime_ = p.AcquisitionTime;
		fprintf(opFile_, "%d %d %lf %lf %lf %d %lf %lf %lf %lf %lf\n", p.GetRegionID(), p.ParticleType, p.PrimaryEnergy, p.GetMinToA(), AcquisitionTime_,
										 p.GetSize(), p.GetEnergy(), p.GetHeight(), theta, phi, sp);
		return true;
	}
	else 
		return false;
}
