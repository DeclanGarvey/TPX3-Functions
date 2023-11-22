#include <iostream>
using namespace std;

#include "FluenceStoppingPower.h"
#include "FileNames.h"
#include "Particle.h"
#include "AngleCalulationFunctions.h"
#include "ClusterFeatures.h"
#include "MorphologicalClassification.h"
#include "ParticleFileWriters.h"

PxFileWriter::PxFileWriter(const string& opFileName, double DetectorThickness)
{
	opInitFile_ = fopen((opFileName+".ini").c_str(),"w");
	fprintf(opInitFile_, "[Measurement]\n");
	fprintf(opInitFile_, "PxFile=%s\n", (RemoveDirectoryPath(opFileName) + "_px.txt").c_str());
	fprintf(opInitFile_, "ClFile=%s\n", (RemoveDirectoryPath(opFileName) + "_cl.txt").c_str());
	fprintf(opInitFile_, "Version=0\nFormat=txt\n");
	fclose(opInitFile_);
	opClFile_ = fopen((opFileName+"_cl.txt").c_str(),"w");
	opPxFile_ = fopen((opFileName+"_px.txt").c_str(),"w");
	PxLineNo_=0; 
	PxByteNo_=0;
	DetectorThickenss_ = DetectorThickness;
}



bool PxFileWriter::AddParticle(particle const& p)
{
	if(p.IsEmpty()==false)
	{
		fprintf(opClFile_, "%lf %d %d %d\n", p.GetMinToA(), p.GetSize(), PxLineNo_, PxByteNo_);
		for (auto pixel : p.GetCluster()) 
		{
			fprintf(opPxFile_, "%.0lf %.0lf %.6lf %.6lf\n", pixel.x, pixel.y, pixel.time, pixel.energy);
			PxLineNo_++;
		}
		fprintf(opPxFile_, "#\n");
		PxLineNo_++;
		PxByteNo_ = ftell(opPxFile_);
		return true;
	}
	else 
		return false;
}

void PxFileWriter::AddInitInfo(const string&  ipInfo)
{
	fprintf(opInitFile_, "%s\n", ipInfo.c_str());
}

void PxFileWriter::Close()
{
	fclose(opClFile_);
	fclose(opPxFile_);
}

AngFileWriter::AngFileWriter(const string& opFileName, double DetectorThickness)
{	
	DetectorThickenss_ = DetectorThickness;
	if(opFileName.size()>8)
	{
		
		if(opFileName.substr(opFileName.size()-8)!="_ang.txt")
			opFile_ = fopen((opFileName + "_ang.txt").c_str(),"w");
		else
			opFile_ = fopen((opFileName).c_str(), "w");
	}
	else 
		opFile_ = fopen((opFileName+ "_ang.txt").c_str(),"w");
}

bool AngFileWriter::AddParticle(particle const& p)
{
	if(p.IsEmpty()==false)
	{
		fprintf(opFile_, "theta=%lf,phi=%lf\n", p.theta, p.phi);
		for (auto pixel : p.GetCluster()) 
		{
			fprintf(opFile_, "%.0lf %.0lf %.6lf %.6lf\n", pixel.x, pixel.y, pixel.time, pixel.energy);
		}
		return true;
	}
	else 
		return false;
}

void AngFileWriter::Close()
{
	fclose(opFile_);
}


SpFileWriter::SpFileWriter(const string& opFileName, double DetectorThickness)
{	
	DetectorThickness_ = DetectorThickness;
	opFile_ = fopen((RemoveExtension(opFileName)+ "_sp.txt").c_str(),"w");
}

bool SpFileWriter::AddParticle(particle const& p)
{
	if(p.IsEmpty()==false)
	{
		double theta = ThetaImprovedLLM(p, DetectorThickness_);
		double phi =  PhiTimeWeighted(p);
		double sp = StoppingPower(p.GetEnergy(), theta, DetectorThickness_); 
		double AcquisitionTime_ = p.AcquisitionTime;
		fprintf(opFile_, "%d %d %lf %lf %lf %d %lf %lf %lf %lf %lf\n", p.GetRegionID(), p.ParticleType, p.PrimaryEnergy, p.GetMinToA(), AcquisitionTime_,
										 p.GetSize(), p.GetEnergy(), p.GetHeight(), theta, phi, sp);
		return true;
	}
	else 
		return false;
}

void SpFileWriter::Close()
{
	fclose(opFile_);
}



FeatFileWriter::FeatFileWriter(const string& opFileName, double DetectorThickness)
{	
	DetectorThickenss_ = DetectorThickness;
	if(opFileName.size()>9)
	{
		
		if(opFileName.substr(opFileName.size()-9)!="_feat.txt")
			opFile_ = fopen((opFileName + "_feat.txt").c_str(),"w");
		else
			opFile_ = fopen((opFileName).c_str(), "w");
	}
	else 
		opFile_ = fopen((opFileName+ "_feat.txt").c_str(),"w");
}

bool FeatFileWriter::AddParticle(particle const& p)
{
	if(p.IsEmpty()==false)
	{
		auto rotated = RotateToNormal(p);
		auto dims = BoxDimensions(rotated);
		auto stds = WeightedBoxStds(rotated);
		
		auto dimsxy = BoxDimensions(p);
		
		double theta = ThetaImprovedLLM(p, DetectorThickenss_);
		double sp = StoppingPower(p.GetEnergy(), theta, DetectorThickenss_); 
		double weight = FleunceContribution(theta, DetectorThickenss_);
		
		fprintf(opFile_, "%d %lf %lf %lf %lf %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
				 p.GetRegionID(), p.PrimaryEnergy, p.theta, p.phi, p.GetMinToA(), p.GetSize(), p.GetEnergy(), StandardDeviationEnergy(p), NearestNeighbours8Fold(p),stds[0], stds[1],
				 dims[1], dims[0], PhiLineFit(p), PhiTimeWeighted(p), sp, weight, theta,dimsxy[0],dimsxy[1], Linearity(p));		 
				 
		return true;
	}
	else 
		return false;
}

void FeatFileWriter::Close()
{
	fclose(opFile_);
}

SatFileWriter::SatFileWriter(const string& opFileName, double DetectorThickness)

{	
	DetectorThickenss_ = DetectorThickness;
	if(opFileName.size()>8)
	{
		if(opFileName.substr(opFileName.size()-8)!="_sat.txt")
			opFile_ = fopen((opFileName + "_sat.txt").c_str(),"w");
		else
			opFile_ = fopen((opFileName).c_str(), "w");
	}
	else 
		opFile_ = fopen((opFileName+ "_sat.txt").c_str(),"w");
}



bool SatFileWriter::AddParticle(particle const& p)
{
	if(p.IsEmpty()==false)
	{	

		fprintf(opFile_, "%lf %d %lf %lf %lf %lf %lf %lf\n",

				 p.GetMinToA(), p.GetSize(), p.GetEnergy(), p.AcquisitionTime,p.FrameOccupancy, p.SatPosX, p.SatPosY, p.SatAltitude);		 
		return true;
	}
	else 
		return false;
}

void SatFileWriter::Close()
{
	fclose(opFile_);
}


FieldTrackingFileWriter::FieldTrackingFileWriter(const string& opFileName, double DetectorThickness)
{	
	DetectorThickness_ = DetectorThickness;
	opFile_ = fopen((RemoveExtension(opFileName)+ "_tracking.txt").c_str(),"w");
}

bool FieldTrackingFileWriter::AddParticle(particle const& p)
{
	if(p.IsEmpty()==false)
	{
		int MorphClass = GetMorphologicalClass(p);
		double theta = ThetaImprovedLLM(p, DetectorThickness_);
		double phi = PhiTimeWeighted(p);
		fprintf(opFile_, "%lf %d %d %lf %lf %lf %lf\n", p.GetMinToA(), MorphClass, p.GetSize(), p.GetEnergy(), theta, phi, ThetaImprovedLLM(p, DetectorThickness_));
		return true;
	}
	else 
		return false;
}

void FieldTrackingFileWriter::Close()
{
	fclose(opFile_);
}
