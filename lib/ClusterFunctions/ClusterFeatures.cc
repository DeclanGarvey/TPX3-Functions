#include <iostream>
#include <fstream>
#include <assert.h>
#include <math.h>
#include <bits/stdc++.h>
using namespace std;

#include "TMath.h"

#include "ParticleFileReaders.h"
#include "AngleCalulationFunctions.h"
#include "Particle.h"
#include "ClusterFeatures.h"
#include "FittingVariables.h"

#include "TH1D.h"

/*
Applies ToT weighted line fit to the particle cluster 

returns double vector of [Function type(0=>x, 1=>y) based on majority orientation, m, c) 

y: f(x) = m*x + c
x: f(y) = m*y + c
*/
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

/*
Calculates the number of the cluster "p" contained with in a line drawn from point p1 to point p2

line calculated by connected points p1 and p2
considered inline if perpendicular distance is less than sqrt(2) = 0.7071 (diagonal with of a pixel)
*/
double PixelsInLine(particle const&p, int XFuncOrYYFunc, PixelHit const& p1, PixelHit const& p2)
{
	double n=0;
	double m_dx = (p2.x - p1.x);
	double m_dy = (p2.y - p1.y);
	for (auto i = p.begin(); i != p.end(); i++ ) 
	{
		double dist = abs( m_dx * (i->y - p1.y) - m_dy * (i->x - p1.x));
		if(dist<=0.7071)
			n+=1;
	}
	return n;
}

/*
Calculates the percentage of pixels of the primary track contained within the energy weighted line fit of the cluster
*/
double Linearity(particle const& p)
{
	auto skel = Skeletonise(p,0,5);
	if(skel.GetSize()>2)
	{
		double linearity=0;
		auto mc = LineFit(skel);
		double denom = sqrt(1+mc[1]*mc[1]);
		for (auto i = skel.begin(); i != skel.end(); i++ ) 
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
		return linearity/skel.GetSize();
	}
	else
	{
		return 1;
	}
}

/*
Calculates the percentage of pixels of the primary track contained within line drawn between the improved LLM points 
*/
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

/*
Returns the particle cluster rotated to the horizontal based on the energy weighted line fit  
*/
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
    	if(p.GetSize()==1)
    	{
    		auto i=p.begin();
		RotatedP.Insert(*i);
    	}
    	else if(p.GetSize()==2)
    	{
    		auto i=p.begin();
		RotatedP.Insert(PixelHit{1,
					0,
					i->time,
					i->energy});
		i++;
		RotatedP.Insert(PixelHit{2,
						0,
						i->time,
						i->energy});
    	}
    	else
    	{
    		double angle1 = atan2(covxy,varx);
    		double angle2 = atan2(covxy,vary);
    		double c,s;
    		if(vary<=varx)
    		{
    			c = cos(-angle1);
    			s = sin(-angle1);
    		}
    		else 
    		{
    			c = sin(-angle2);
    			s = cos(-angle2);
    		}
    		
    		for (auto i=p.begin(); i != p.end(); i++ ) 
		{
			RotatedP.Insert(PixelHit{c*(i->x-oldmeanx) - s*(i->y-oldmeany),
						 s*(i->x-oldmeanx) + c*(i->y-oldmeany),
						i->time,
						i->energy});
		}
    			
    	}
	return RotatedP;
}

/*

*/
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
	if(p.GetSize()>1)
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
	else
	{
		vector<double> ans{0,0};
		return ans;
	}
		
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
int ElectronCloudSize(particle const& p)
{
	int Cloud=0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		if(i->energy <= (0.626*p.GetHeight() - 103.75 ))
		{
			//if(i->energy>5)
				Cloud++;
		}
	}
	return Cloud;
}
double ElectronCloudEnergy(particle const& p)
{
	double Cloud=0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		//if(i->energy <= (10))
		{
			if(i->energy>5)
				Cloud+=i->energy;
		}
	}
	return Cloud;
}
double EnergyOfLine(particle const& p)
{
	if(p.GetSize()>2)
	{
		double LineEnergy = 0;
		double LineSize = 0;
		
		// implementing d = |Ay + Bx + C|/sqrt(A*A+B*B)
		auto mc = LineFit(p);
		double denom = sqrt(1+mc[1]*mc[1]);
		double EnergySum=0;
		for (auto i = p.begin(); i != p.end(); i++ ) 
		{
			if(i->energy>10)//(0.626*p.GetHeight() - 103.75 ))
			{
				if(mc[0]==0)
				{
					if(abs(i->y - (i->x*mc[1] + mc[2]))<=1.0*denom)
					{
						LineEnergy+=i->energy;
						LineSize+=1;
					}
				}
				else
				{
					if(abs(i->x - (i->y*mc[1] + mc[2]))<=1.0*denom)
					{
						LineEnergy+=i->energy;
						LineSize+=1;
					}
				}
				EnergySum+=i->energy;
			}
		}
		return LineEnergy;
	}
	else
	{
		return 1;

	}
}

int NumberOfEndPoints(particle const& p)
{
	int EndPointNumber = 0;
	
	int Neighbours[8];
	int NumberOfNeighbours,NumberofTransitions;
	bool IsEndPoint[p.GetSize()] = {false};
	bool AlreadyNextToEndPoint;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{	
		if( i->energy >= 5)//(i->energy) >= 5)
		{
			for(int ii=0;ii<8;ii++) Neighbours[ii] = 0;
			NumberOfNeighbours=0;
			AlreadyNextToEndPoint=false;
			int jIteratorPosition=0;
			for (auto j=p.begin(); j != p.end(); j++, jIteratorPosition++) 
			{
				if( j->energy >= 5 )//(i->energy) >= 5)
				{
					if(( (i->x) == j->x) & ((i->y+1) == j->y))
					{
						if(IsEndPoint[jIteratorPosition])
							AlreadyNextToEndPoint=true;
						Neighbours[0]=1;
						NumberOfNeighbours++;
					}
					
					else if(( (i->x+1) == j->x) & ((i->y+1) == j->y))
					{
						if(IsEndPoint[jIteratorPosition])
							AlreadyNextToEndPoint=true;
						Neighbours[1]=1;
						NumberOfNeighbours++;
					}

					else if(( (i->x+1) == j->x) & ((i->y) == j->y))
					{
						if(IsEndPoint[jIteratorPosition])
							AlreadyNextToEndPoint=true;
						Neighbours[2]=1;
						NumberOfNeighbours++;
					}
					else if(( (i->x+1) == j->x) & ((i->y-1) == j->y))
					{
						if(IsEndPoint[jIteratorPosition])
							AlreadyNextToEndPoint=true;
						Neighbours[3]=1;
						NumberOfNeighbours++;
					}
					else if(( (i->x) == j->x) & ((i->y-1) == j->y))
					{
						if(IsEndPoint[jIteratorPosition])
							AlreadyNextToEndPoint=true;
						Neighbours[4]=1;
						NumberOfNeighbours++;
					}
					else if(( (i->x-1) == j->x) & ((i->y-1) == j->y))
					{
						if(IsEndPoint[jIteratorPosition])
							AlreadyNextToEndPoint=true;
						Neighbours[5]=1;
						NumberOfNeighbours++;
					}
					else if(( (i->x-1) == j->x) & ((i->y) == j->y))
					{
						if(IsEndPoint[jIteratorPosition])
							AlreadyNextToEndPoint=true;
						Neighbours[6]=1;
						NumberOfNeighbours++;
					}
					else if(( (i->x-1) == j->x) & ((i->y+1) == j->y))
					{
						if(IsEndPoint[jIteratorPosition])
							AlreadyNextToEndPoint=true;
						Neighbours[7]=1;
						NumberOfNeighbours++;
					}
				}
			}
		
		
			NumberofTransitions = 0;
			for(int ii=0;ii<7;ii++)
				NumberofTransitions += static_cast<int>((Neighbours[ii+1] -Neighbours[ii])>0);
			NumberofTransitions += static_cast<int>((Neighbours[0] -Neighbours[7])>0);
			if((NumberofTransitions==1) & (NumberOfNeighbours<=3))
			{
				IsEndPoint[distance(p.begin(), i)] = true;
				if(AlreadyNextToEndPoint==false)
					EndPointNumber++;
			}
		}
	}
	return  EndPointNumber;
}

double MedianEnergy(particle const& p)
{
	double MedianPosition = p.GetSize()/2.0;
	vector<double> EnergyValues;
	for (auto j=p.begin(); j != p.end(); j++) 
	{
		EnergyValues.push_back(j->energy);
	}
	sort(EnergyValues.begin(), EnergyValues.end());
	
	if( fmod(MedianPosition,2) !=0)
		return (EnergyValues[static_cast<int>(MedianPosition-0.5)] + EnergyValues[static_cast<int>(MedianPosition+0.5)])/2;
	else 
		return EnergyValues[static_cast<int>(MedianPosition)];
}

double LineProjectionGradient(particle const& p)
{
	
	if(p.GetSize()>2)
	{
		auto mc = LineFit(p);
		double denom = 1.0/sqrt(1+mc[1]*mc[1]);
		
		particle LineProjected;
		for (auto i = p.begin(); i != p.end(); i++ ) 
		{
			if((i->energy>10))// & (i->energy>(0.626*p.GetHeight() - 103.75 )))
			{
				if(mc[0]==0)
				{
					double distance = (i->y - (i->x*mc[1] + mc[2]))*denom;
					double proj_x = i->x - distance * denom;
					double proj_y = i->y - distance * mc[1] * denom;
					LineProjected.Insert(PixelHit{sqrt(proj_x*proj_x + proj_y*proj_y),0, i->time, i->energy});
				}
				else
				{
					double distance = (i->x - (i->y*mc[1] + mc[2]))*denom;
					double proj_y = i->y - distance * denom;
					double proj_x = i->x - distance * mc[1] * denom;
					LineProjected.Insert(PixelHit{sqrt(proj_x*proj_x + proj_y*proj_y),0, i->time, i->energy});
				}
			}
		}
		double meanx=0, meany=0, meanxy=0, meany2=0;
		double vary;
		for (auto i = LineProjected.begin(); i != LineProjected.end(); i++ ) 
		{
			meanx  += i->x*i->x;
			meany  += i->energy;
			meanxy += i->x*i->energy;
			meany2 += i->energy*i->energy;
		}
		vary = meany2 - meany*meany;
		return abs( (meanxy - meany*meanx)/vary );
	}
	else if(p.GetSize()==2)
	{
		return (p.begin()->energy + (p.begin()+1)->energy)/ sqrt(pow(p.begin()->x - (p.begin()+1)->x,2) + pow(p.begin()->y + (p.begin()+1)->y,2));

	}
	else
	{
		return 0;

	}
}


double HaloDistance(particle const& p)
{
	if(p.GetSize()>2)
	{
		double MeanDistance = 0;
		// implementing d = |Ay + Bx + C|/sqrt(A*A+B*B)
		auto mc = LineFit(p);
		double denom = sqrt(1+mc[1]*mc[1]);
		double EnergySum=0;
		for (auto i = p.begin(); i != p.end(); i++ ) 
		{
			if((i->energy>10)& ( (i->time - p.GetMinToA())>2))// & (i->energy>(0.626*p.GetHeight() - 103.75 )))
			{
				double d;
				if(mc[0]==0)
					d = abs(i->y - (i->x*mc[1] + mc[2]))/denom;
				else
					d = abs(i->x - (i->y*mc[1] + mc[2]))/denom;
				if(d>=2.0)
				{
					MeanDistance += d*(i->time - p.GetMinToA());
					EnergySum += i->time - p.GetMinToA();
				}
			}
		}
		MeanDistance /= EnergySum;
		return MeanDistance;
	}
	else
	{
		return 0;

	}
}

bool ContainsInner8FoldPixel(particle const& p)
{
	int NumberOfNeighbours;
	bool ContainsCube=false;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		NumberOfNeighbours=0;
		for (auto j=p.begin(); j != p.end(); j++) 
		{
			if(( (i->x) == j->x) & ((i->y+1) == j->y))
			{
				NumberOfNeighbours++;
			}
			
			else if(( (i->x+1) == j->x) & ((i->y+1) == j->y))
			{
				NumberOfNeighbours++;
			}

			else if(( (i->x+1) == j->x) & ((i->y) == j->y))
			{
				NumberOfNeighbours++;
			}
			else if(( (i->x+1) == j->x) & ((i->y-1) == j->y))
			{
				NumberOfNeighbours++;
			}
			else if(( (i->x) == j->x) & ((i->y-1) == j->y))
			{
				NumberOfNeighbours++;
			}
			else if(( (i->x-1) == j->x) & ((i->y-1) == j->y))
			{
				NumberOfNeighbours++;
			}
			else if(( (i->x-1) == j->x) & ((i->y) == j->y))
			{
				NumberOfNeighbours++;
			}
			else if(( (i->x-1) == j->x) & ((i->y+1) == j->y))
			{
				NumberOfNeighbours++;
			}
		}
		if(NumberOfNeighbours==8)
		{
			ContainsCube=true;
			break;
		}
		
	}
	return  ContainsCube;
}

bool ContainsInner4FoldPixel(particle const& p)
{
	int NumberOfNeighbours;
	bool ContainsCross=false;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		NumberOfNeighbours=0;
		for (auto j=p.begin(); j != p.end(); j++) 
		{
			if(( (i->x) == j->x) & ((i->y+1) == j->y))
			{
				NumberOfNeighbours++;
			}
			else if(( (i->x+1) == j->x) & ((i->y) == j->y))
			{
				NumberOfNeighbours++;
			}
			else if(( (i->x) == j->x) & ((i->y-1) == j->y))
			{
				NumberOfNeighbours++;
			}
			else if(( (i->x-1) == j->x) & ((i->y) == j->y))
			{
				NumberOfNeighbours++;
			}
		}
		if(NumberOfNeighbours==4)
		{
			ContainsCross=true;
			break;
		}
		
	}
	return  ContainsCross;
}




