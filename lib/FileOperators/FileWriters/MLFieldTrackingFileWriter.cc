#include <iostream>
#include <vector>
using namespace std;

#include "FluenceStoppingPower.h"
#include "RandomForrestTheta.h"
#include "Particle.h"
#include "ClusterFeatures.h"
#include "AngleCalulationFunctions.h"
#include "MorphologicalClassification.h"
#include "ParticleFileWriters.h"

#include "MLFieldTrackingFileWriter.h"

MLFieldTrackingFileWriter::MLFieldTrackingFileWriter(const string& opFileName, double DetectorThickness, const string& ModelPath) : FieldTrackingFileWriter(opFileName, DetectorThickness)
{	
	ThetaModel = std::make_unique<RandomForrestTheta>(RandomForrestTheta(ModelPath));
}

MLFieldTrackingFileWriter::MLFieldTrackingFileWriter(double DetectorThickness, const string& ModelPath) : FieldTrackingFileWriter(DetectorThickness)
{	
	ThetaModel = std::make_unique<RandomForrestTheta>(RandomForrestTheta(ModelPath));
}
bool MLFieldTrackingFileWriter::AddParticle(particle const& p)
{
	if(p.IsEmpty()==false)
	{
		int MorphClass = GetMorphologicalClass(p);
		double theta = ThetaModel->PredictFromParticle(p);
		double phi = PhiTimeWeighted(p);
		fprintf(opFile_, "%lf %d %d %f %lf %lf %lf\n", p.GetMinToA(), MorphClass, p.GetSize(), p.GetEnergy(), theta, phi, ThetaImprovedLLM(p, DetectorThickness_));
		return true;
	}
	else 
		return false;
}

