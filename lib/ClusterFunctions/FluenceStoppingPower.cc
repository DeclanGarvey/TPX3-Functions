#include <iostream>
#include <fstream>
#include <assert.h>
#include <math.h>
using namespace std;

#include <TMath.h>
#include <TH1D.h>

#include "ParticleFileReaderFetcher.h"
#include "ParticleFileReaders.h"
#include "AngleCalulationFunctions.h"
#include "Particle.h"
#include "FittingVariables.h"

#include "FluenceStoppingPower.h"

double StoppingPower(double EnergyDeposition, double theta, double d)
{
	double len = d/abs(cos(theta));
	return EnergyDeposition*(1e-3)/(len*DetectorProperties::SiliconDensity); //MeV cm^{-1] g^{-1}
}

double LandauGaussian(double *x, double *par) 
{

      double deltax=0.01;
      double ans=0;
      for(double xt=0.0; xt<=10.0; xt+=deltax) {
         ans += par[0] * TMath::Landau(xt,par[1],par[2]) * TMath::Gaus(x[0]-xt,par[3],par[4])*deltax;
      }

      return ans;
}

double FleunceContribution(double theta, double d)
{
	return 1.0/abs(1.408*(1.408 - d*abs(tan(theta)))*cos(theta));
}

double PhiFluenceContribution(double theta,double phi)
{
	return 1.0/abs((1.408-0.05*tan(theta)*cos(phi))*(1.408 - 0.05*tan(theta)*sin(phi))*cos(theta));
}

double TheoreticalThetaHitDistribution(double *theta, double *par)
{
	return par[0]*abs(1.408*(1.408 - 0.05*tan(theta[0]))*cos(theta[0])*sin(theta[0]));
}

TH1D* GetStoppingPowerFluenceCurve(const string& ipFileName, int ipFileType, double d, const string&  Name, const string&  Title, int NoBins, double XLow, double XHigh)
{
	TH1D* Curve = new TH1D(Name.c_str(), Title.c_str(), NoBins, XLow, XHigh);
	particle p;
	ParticleFileReader reader = GetFileReader(ipFileName,ipFileType);
	while(reader.AssignParticle(p))
	{
		double theta = ThetaImprovedLLM(p,d);
		double sp = StoppingPower(p.GetEnergy(), theta, d); 
		double weight = FleunceContribution(theta, d);
		if((p.IsEdge()==false) & (p.IsEmpty()==false) & (sp>XLow) & (sp<XHigh))
		{
			Curve->Fill(sp, weight);
		}
	}
	reader.Close();
	return Curve;
}

void NormaliseCurve(TH1D* Curve)
{
	double sum = 0;
	for(int i=1;i<=Curve->GetNbinsX();i++)
		sum += Curve->GetBinContent(i);
	Curve->Scale(1.0/sum);
}

void SubtractCurves(TH1D* Curve1, TH1D* Curve2)
{
	for(int i=1;i<=Curve1->GetNbinsX();i++)
		Curve1->SetBinContent(i, Curve1->GetBinContent(i) - Curve2->GetBinContent(i));
}

void PrintHistogram(FILE* opFile, TH1D* Curve)
{
	
	fprintf(opFile, "%lf", Curve->GetBinContent(1));
	int NBins=Curve->GetNbinsX();
	cout<<"Number of Bins:"<<NBins<<endl;
	for(int i=2; i<=NBins;i++)
		fprintf(opFile, ",%lf", Curve->GetBinContent(i));
	fprintf(opFile, "\n");
}
