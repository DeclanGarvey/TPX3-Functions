#include <iostream>
#include <vector>
using namespace std;

#include "FluenceStoppingPower.h"
#include "RandomForrestTheta.h"
#include "Particle.h"
#include "ClusterFeatures.h"
#include "AngleCalulationFunctions.h"
#include "ParticleFileWriters.h"

#include "MLFieldTrackingFileWriter.h"

MLFieldTrackingFileWriter::MLFieldTrackingFileWriter(const string& opFileName, double DetectorThickness, const string& ModelPath) : FieldTrackingFileWriter(opFileName, DetectorThickness)
{	
	ThetaModel = std::make_unique<RandomForrestTheta>(RandomForrestTheta(ModelPath));
}

bool MLFieldTrackingFileWriter::AddParticle(particle const& p)
{
	if(p.IsEmpty()==false)
	{
		double theta = ThetaModel->PredictFromParticle(p);
		double phi = PhiTimeWeighted(p);
		fprintf(opFile_, "%lf %d %lf %f %lf %lf %lf\n", p.GetMinToA(),p.GetSize(), p.GetEnergy(), theta, phi, LLMLinearity(p), ThetaImprovedLLM(p, DetectorThickness_));
		return true;
	}
	else 
		return false;
}

