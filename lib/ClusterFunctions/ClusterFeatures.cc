#include <iostream>
#include <fstream>
#include <assert.h>
#include <math.h>
using namespace std;

#include "TMath.h"

#include "ParticleFileReaders.h"
#include "AngleCalulationFunctions.h"
#include "Particle.h"
#include "ClusterFeatures.h"
#include "FittingVariables.h"

#include "TH1D.h"

vector<double> LineFit(particle const& p)
{
	double meanx=0, meany=0, meanxy=0, meanx2=0,meany2=0;
	double e_ratio;
	double varx, vary;
	if(p.GetSize()>=2)
	{
		for (auto i = p.begin(); i != p.end(); i++ ) 
		{
			e_ratio = (i->energy)/p.GetEnergy();
			meanx  += i->x*e_ratio;
			meany  += i->y*e_ratio;
			meanxy += i->x*i->y*e_ratio;
			meanx2 += i->x*i->x*e_ratio;
			meany2 += i->y*i->y*e_ratio;
		}
		varx = meanx2 - meanx*meanx;
		vary = meany2 - meany*meany;
		vector<double> mc;
		if(varx<vary)
		{
			mc.push_back(1);//y
			mc.push_back((meanxy - meany*meanx)/vary);
			mc.push_back(meanx - mc[1]*meany);
			
		}
		else 
		{
			mc.push_back(0);//x
			mc.push_back((meanxy - meanx*meany)/varx);
			mc.push_back(meany - mc[1]*meanx);
		}
		return mc;
	}
	else
	{
		vector<double> mc{0.0, 0.0, p.begin()->x};
		return mc;
	}
}

double PixelsInLine(particle const&p, int XFuncOrYYFunc, PixelHit const& p1, PixelHit const& p2)
{
	double n=0;
	double m_dx = (p2.x - p1.x);
	double m_dy = (p2.y - p1.y);
	for (auto i = p.begin(); i != p.end(); i++ ) 
	{
		double dist = abs( m_dx * (i->y - p1.y) - m_dy * (i->x - p1.x));
		if(dist<=1.0)//?
			n+=1;
	}
	return n;
}


double Linearity(particle const& p)
{
	if(p.GetSize()>2)
	{
		double linearity=0;
		
		// implementing d = |Ay + Bx + C|/sqrt(A*A+B*B)
		auto mc = LineFit(p);
		double denom = sqrt(1+mc[1]*mc[1]);
		for (auto i = p.begin(); i != p.end(); i++ ) 
		{
			if(mc[0]==0)
			{
				if(abs(i->y - (i->x*mc[1] + mc[2]))<=0.7071*denom)
					linearity+=1;
			}
			else
			{
				if(abs(i->x - (i->y*mc[1] + mc[2]))<=0.7071*denom)
					linearity+=1;
			}
		}
		return linearity/p.GetSize();
	}
	else
	{
		return 1;
	}
}

double LLMLinearity(particle const& p)
{
	if(p.GetSize()>2)
	{
		double HoriLinearity = PixelsInLine(p, 0, p.GetRightBottomMost(), p.GetLeftTopMost());
		double VertLinearity = PixelsInLine(p, 1, p.GetBottomRightMost(), p.GetTopLeftMost());
		
		if(HoriLinearity>=VertLinearity)
			return HoriLinearity/p.GetSize();
		else
			return VertLinearity/p.GetSize();
	}
	else
	{
		return 1;
	}
}

particle RotateToNormal(particle const& p)
{
	
	double e_ratio;
	double oldmeanx=0, oldmeany=0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		e_ratio = (i->energy)/p.GetEnergy();
		oldmeanx  += i->x*e_ratio;
		oldmeany  += i->y*e_ratio;
	}
	
	double varx, vary, covxy;
	double meanx=0, meany=0, meanxy=0, meanx2=0,meany2=0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		double x = i->x - oldmeanx;
		double y = i->y - oldmeany;
		e_ratio = (i->energy)/p.GetEnergy();
		meanx  += x*e_ratio;
		meany  += y*e_ratio;
		meanxy += x*y*e_ratio;
		meanx2 += x*x*e_ratio;
		meany2 += y*y*e_ratio;
	}
	varx = meanx2 - meanx*meanx;
	vary = meany2 - meany*meany;
	covxy = meanxy - meanx*meany;
    	
    	particle RotatedP;
    	if(p.GetSize()<=1)
    	{
    		auto i=p.begin();
		RotatedP.Insert(*i);
    	}
    	else if(varx==0)
    	{
    		for (auto i=p.begin(); i != p.end(); i++ ) 
		{
			RotatedP.Insert(PixelHit{i->y, i->x, i->time, i->energy});
		}
    	}
    	else
    	{
		double sumvars = varx + vary;
		double diffvars = varx - vary;
		double discriminant = diffvars*diffvars + 4*covxy*covxy;
		double sqrtdiscr = sqrt(discriminant);
		// eigenvalues
		double lambdaplus = (sumvars + sqrtdiscr)/2;
		double lambdaminus = (sumvars - sqrtdiscr)/2;
		//eigenvectors - these are the components of the two vectors
		double aplus = varx + covxy - lambdaminus;
		double bplus = vary + covxy - lambdaminus;
		
		double aminus = varx + covxy - lambdaplus;
		double bminus = vary + covxy - lambdaplus;
		
		double denomPlus = sqrtf(aplus*aplus + bplus*bplus);
		double denomMinus= sqrtf(aminus*aminus + bminus*bminus);
		aplus = aplus/denomPlus;
		bplus = bplus/denomPlus;
		if(denomMinus!=0)
		{
			aminus = aminus/denomMinus;
			bminus = bminus/denomMinus;
		}
		for (auto i=p.begin(); i != p.end(); i++ ) 
		{
			RotatedP.Insert(PixelHit{aplus*i->x + bplus*i->y,
						aminus*i->x + bminus*i->y,
						i->time,
						i->energy});
		}
	}
	
	return RotatedP;
}

vector<double> BoxStds(particle const& p)
{
	double meanx=0,meany=0,meanx2=0, meany2=0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		meanx  += i->x;
		meany  += i->y;
		meanx2 += i->x*i->x;
		meany2 += i->y*i->y;
	}
	meanx = meanx/p.GetSize();
	meany = meany/p.GetSize();
	meanx2 = meanx2/p.GetSize();
	meany2 = meany2/p.GetSize();
	vector<double> ans{sqrt((meanx2-meanx*meanx)/(p.GetSize()-1)), sqrt((meany2 - meany*meany)/(p.GetSize()-1))};
	return ans;
}


vector<double> WeightedBoxStds(particle const& p)
{
	double meanx=0,meany=0,meanx2=0, meany2=0, e_ratio;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		e_ratio = (i->energy)/p.GetEnergy();
		meanx  += i->x*e_ratio;
		meany  += i->y*e_ratio;
		meanx2 += i->x*i->x*e_ratio;
		meany2 += i->y*i->y*e_ratio;
	}
	vector<double> ans{meanx2-meanx*meanx, meany2 - meany*meany};
	return ans;
}

vector<double> BoxDimensions(particle const& p)
{
	double minx=256,maxx=-1,miny=256, maxy=-1;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		if(i->x<minx)
			minx=i->x;
		if(i->x>maxx)
			maxx=i->x;
		if(i->y<miny)
			miny=i->y;
		if(i->y>maxy)
			maxy=i->y;
	}
	vector<double> ans{maxx-minx,maxy-miny};
	return ans;
}

double StandardDeviationTime(particle const& p)
{
	double mean=0,mean2=0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		//e_ratio = (i->energy)/p.GetEnergy();
		mean  += i->time;
		mean2 += i->time*i->time;
	}
	mean = mean/p.GetSize();
	mean2 = mean2/p.GetSize();
	return sqrt(mean2-mean*mean);
}

double StandardDeviationEnergy(particle const& p)
{
	double mean=0,mean2=0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		mean  += i->energy;
		mean2 += i->energy*i->energy;
	}
	mean = mean/p.GetSize();
	mean2 = mean2/p.GetSize();
	return sqrt(mean2-mean*mean);
}

double EnergySizeRatio(particle const& p, double EnergyCut)
{
	double LowEnergySize=0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		if(i->energy<EnergyCut)
			LowEnergySize++;
		
	}
	return LowEnergySize/p.GetSize();
}

double Length(particle const& p)
{
	if(p.GetSize()>=2)
	{
		double len=0;
		if(GetFuncType(p))
			len = sqrt(pow(p.GetRightBottomMost().x - p.GetLeftTopMost().x, 2.0) + pow(p.GetRightBottomMost().y - p.GetLeftTopMost().y, 2.0));
		else
			len = sqrt(pow(p.GetTopLeftMost().x - p.GetBottomRightMost().x, 2.0) + pow(p.GetTopLeftMost().y - p.GetBottomRightMost().y, 2.0));
		return len;
	}
	else
	{
		return 0;
	}
}

double NearestNeighbours8Fold(particle const& p)
{
	int n = 0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		for(int dx = -1; dx<2;dx++)
		{
			for(int dy = -1; dy<2;dy++)
			{
				if( (dx==0) & (dy==0) )
					continue;
				for (auto j=i+1; j != p.end(); j++ ) 
				{
					if(( (i->x+dx) == j->x) & ((i->y+dy) == j->y))
						n++;
				}
			}
		
		}
	}
	return (2.0*n)/p.GetSize();
}

double NearestNeighbours4Fold(particle const& p)
{
	int n = 0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		for (auto j=i+1; j != p.end(); j++ ) 
		{
			if(( (i->x) == j->x) & ((i->y+1) == j->y))
				n++;
			if(( (i->x) == j->x) & ((i->y-1) == j->y))
				n++;
			if(( (i->x+1) == j->x) & ((i->y) == j->y))
				n++;
			if(( (i->x-1) == j->x) & ((i->y) == j->y))
				n++;
		}
	}
	return (2.0*n)/p.GetSize();
}

double NumberOfInnerPixels(particle const& p)
{
	int InnerPixels=0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		int n = 0;
		for (auto j=p.begin(); j != p.end(); j++ ) 
		{
			if(( (i->x) == j->x) & ((i->y+1) == j->y))
				n++;
			if(( (i->x) == j->x) & ((i->y-1) == j->y))
				n++;
			if(( (i->x+1) == j->x) & ((i->y) == j->y))
				n++;
			if(( (i->x-1) == j->x) & ((i->y) == j->y))
				n++;
		}
		if(n==4)
			InnerPixels++;
	}
	return InnerPixels;
}

double DiameterOfCluster(particle const& p)
{ 
	return 2.*sqrt( p.GetSize() / M_PI); 
}

double PixelDistance(PixelHit const& p1, PixelHit const& p2)
{
	return sqrt(pow(p1.x - p2.x, 2.0) + pow(p1.y - p2.y, 2.0)); // 0.00003025 = (55 micometers)^2
}

double MaximumDistance(particle const& p)
{
	double Distances[6];
	Distances[0] = PixelDistance(p.GetRightBottomMost(), p.GetLeftTopMost());
	Distances[1] = PixelDistance(p.GetRightBottomMost(), p.GetTopLeftMost());
	Distances[2] = PixelDistance(p.GetRightBottomMost(), p.GetBottomRightMost());
	Distances[3] = PixelDistance(p.GetBottomRightMost(), p.GetLeftTopMost());
	Distances[4] = PixelDistance(p.GetTopLeftMost(), p.GetLeftTopMost());
	Distances[5] = PixelDistance(p.GetTopLeftMost(), p.GetBottomRightMost());
	double m=-1;
	for( double dist : Distances)
		if(m<dist)
			m=dist;
	
	return m;
}

int NumberOfEndPoints(particle const& p)
{
	int EndPointNumber = 0;
	
	int Neighbours[8];
	int NumberOfNeighbours,NumberofTransitions;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		for(int ii=0;ii<8;ii++) Neighbours[ii] = 0;
		NumberOfNeighbours=0;
		for (auto j=p.begin(); j != p.end(); j++ ) 
		{
			if(( (i->x) == j->x) & ((i->y+1) == j->y))
			{
				Neighbours[0]=1;
				NumberOfNeighbours++;
			}
			
			if(( (i->x) == j->x+1) & ((i->y+1) == j->y))
			{
				Neighbours[1]=1;
				NumberOfNeighbours++;
			}

			if(( (i->x) == j->x+1) & ((i->y) == j->y))
			{
				Neighbours[2]=1;
				NumberOfNeighbours++;
			}
			if(( (i->x) == j->x+1) & ((i->y-1) == j->y))
			{
				Neighbours[3]=1;
				NumberOfNeighbours++;
			}
			if(( (i->x) == j->x) & ((i->y-1) == j->y))
			{
				Neighbours[4]=1;
				NumberOfNeighbours++;
			}
			if(( (i->x) == j->x-1) & ((i->y-1) == j->y))
			{
				Neighbours[5]=1;
				NumberOfNeighbours++;
			}
			if(( (i->x) == j->x-1) & ((i->y) == j->y))
			{
				Neighbours[6]=1;
				NumberOfNeighbours++;
			}
			if(( (i->x) == j->x-1) & ((i->y+1) == j->y))
			{
				Neighbours[7]=1;
				NumberOfNeighbours++;
			}
		}
		
		NumberofTransitions = 0;
		for(int ii=0;ii<7;ii++)
			NumberofTransitions += static_cast<int>((Neighbours[ii+1] -Neighbours[ii])>0);
		
		if((NumberofTransitions<=1) & (NumberOfNeighbours<=3))
			EndPointNumber++;
	}
	return  EndPointNumber;
}

