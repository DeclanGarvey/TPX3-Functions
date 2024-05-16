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

#include "MLParticleFileWriters.h"

MLFieldTrackingFileWriter::MLFieldTrackingFileWriter(const string& opFileName, double DetectorThickness, const string& ModelPath) : FieldTrackingFileWriter(opFileName, DetectorThickness)
{	
	ThetaModel = std::make_unique<RandomForrestTheta>(ModelPath);
}

MLFieldTrackingFileWriter::MLFieldTrackingFileWriter(double DetectorThickness, const string& ModelPath) : FieldTrackingFileWriter(DetectorThickness)
{	
	ThetaModel = std::make_unique<RandomForrestTheta>(ModelPath);
}

double MLFieldTrackingFileWriter::ThetaCalculationMethod(particle const& p)
{
	return ThetaModel->PredictFromParticle(p);
}


MLSpFileWriter::MLSpFileWriter(const string& opFileName, double DetectorThickness, const string& ModelPath) : SpFileWriter(opFileName, DetectorThickness)
{	
	ThetaModel = std::make_unique<RandomForrestTheta>(ModelPath);
}
MLSpFileWriter::MLSpFileWriter(double DetectorThickness, const string& ModelPath) : SpFileWriter(DetectorThickness)
{	
	ThetaModel = std::make_unique<RandomForrestTheta>(ModelPath);
}

double MLSpFileWriter::ThetaCalculationMethod(particle const& p)
{
	return ThetaModel->PredictFromParticle(p);
}
