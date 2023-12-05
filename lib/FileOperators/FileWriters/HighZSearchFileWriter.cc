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

#include "HighZSearchFileWriter.h"

HighZSearchFileWriter::HighZSearchFileWriter(const string& opFileName, double DetectorThickness, const string& ModelPath)
{	
	DetectorThickness_ = DetectorThickness;
	UpdateFileOutput(opFileName);
	ThetaModel = std::make_unique<RandomForrestTheta>(RandomForrestTheta(ModelPath));
}

HighZSearchFileWriter::HighZSearchFileWriter(double DetectorThickness, const string& ModelPath)
{	
	DetectorThickness_ = DetectorThickness;
	ThetaModel = std::make_unique<RandomForrestTheta>(RandomForrestTheta(ModelPath));
}

void HighZSearchFileWriter::UpdateFileOutput(const string& opFileName)
{
	Close();
	if(opFileName.size()>8)
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
		double theta = ThetaModel->PredictFromParticle(p);
		double phi = PhiTimeWeighted(p);
		fprintf(opFile_, "%lf %d %lf %f %lf %lf %lf %d\n", p.GetMinToA(), p.GetSize(), p.GetEnergy(), theta, phi, ThetaImprovedLLM(p, DetectorThickness_), Linearity(p), NumberOfEndPoints(p));
		return true;
	}
	else 
		return false;
}

void HighZSearchFileWriter::Close()
{
	if(opFile_!=NULL)
	{
		fclose(opFile_);
		opFile_=NULL;
	}
}
