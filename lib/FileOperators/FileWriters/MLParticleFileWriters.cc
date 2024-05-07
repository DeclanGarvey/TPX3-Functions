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

HighZSearchFileWriter::HighZSearchFileWriter(const string& opFileName, double DetectorThickness, const string& ModelPath)
{	
	DetectorThickness_ = DetectorThickness;
	UpdateFileOutput(opFileName);
	ThetaModel = std::make_unique<RandomForrestTheta>(ModelPath);
}

HighZSearchFileWriter::HighZSearchFileWriter(double DetectorThickness, const string& ModelPath)
{	
	DetectorThickness_ = DetectorThickness;
	ThetaModel = std::make_unique<RandomForrestTheta>(ModelPath);
}

void HighZSearchFileWriter::UpdateFileOutput(const string& opFileName)
{
	Close();
	if(opFileName.size()>12)
	{
		if(opFileName.substr(opFileName.size()-12)!="_zsearch.txt")
			opFile_ = fopen((opFileName + "_zsearch.txt").c_str(),"w");
		else
			opFile_ = fopen((opFileName).c_str(), "w");
	}
	else 
		opFile_ = fopen((opFileName+ "_zsearch.txt").c_str(),"w");
}

bool HighZSearchFileWriter::AddParticle(particle const& p)
{
	if(p.IsEmpty()==false)
	{
		auto skel = Skeletonise(p,0.626,-103.5);
		
		double theta = ThetaModel->PredictFromParticle(p);
		double phi = PhiTimeWeighted(skel);//Skeletonise(p,0.0,5));
		
		auto rotated = RotateToNormal(p);
		//auto dims = BoxDimensions(rotated);
		auto stds = WeightedBoxStds(rotated);
		
		fprintf(opFile_, "%d %lf %lf %d %lf %lf %lf %lf %lf %d %lf\n", GetMorphologicalClass(p), p.GetMinToA(), p.PrimaryEnergy,  p.GetSize(),p.GetEnergy(),p.GetHeight(), theta, phi, ThetaImprovedLLM(skel, DetectorThickness_), NumberOfEndPoints(p), Linearity(p));
		return true;
	}
	else 
		return false;
}
