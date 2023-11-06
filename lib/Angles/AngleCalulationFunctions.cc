#include <iostream>
#include <fstream>
#include <assert.h>
#include <unordered_map> 
#include <math.h>

#include <PixelStructure.h>
#include "Particle.h"
#include <ClusterFeatures.h>

#include <AngleCalulationFunctions.h>
using namespace std;

//double d=0.05; //Thickness of detector in cm used in all theta 
double UD=80; //Deplection across detector used in the "theta_line_fit_method"
double mu=450;//Hole mobility in silicon used in the "theta_line_fit_method"

double SkelParameters[2] = {0,10};

/*
Takes a particle as input and returns the skeletonised particle.
*/
particle Skeletonise(particle const& p,double a,double b)
{
	particle skel;
	for (auto i = p.begin(); i != p.end(); i++ ) 
	{
		if( i->energy >= (a*p.GetHeight() + b) )
			skel.Insert(*i);
	}
	return skel;
}
/*
Converts range of incident from 0 --> (pi/2)
Neccessary as algorithm in this project require input [0,pi/2], also this is the range of physically indiguishable in Timepix3
*/
double NormalizeIncidentAngle(double theta)
{
	if(theta>M_PI)
	{
		theta += -M_PI;
	}
	if(theta>M_PI_2)
	{
		theta= M_PI - theta;
	}
	return theta;
}

/*
Takes two pixel coordinates and calcultes the distanc between them in cm.
*/
double Distance(PixelHit const& p1, PixelHit const& p2)
{
	return sqrt((0.00003025)*(pow(p1.x - p2.x, 2.0) + pow(p1.y - p2.y, 2.0))); // 0.00003025 = (55 micometers)^2
}

/*
By means of least square line fitting determines if the clusters is predominantly horizontal or vertical, returning true or false respectively in each case.
*/
bool GetFuncType(particle const& p)
{
	assert(p.GetSize()>=0);
	
	double meanx=0, meany=0, meanxy=0, meanx2=0,meany2=0;
	if(p.GetSize()>=2)
	{
		for (auto i = p.begin(); i != p.end(); i++ ) 
		{
			double e_ratio = (i->energy)/p.GetEnergy();
			meanx  += i->x*e_ratio;
			meany  += i->y*e_ratio;
			meanxy += i->x*i->y*e_ratio;
			meanx2 += i->x*i->x*e_ratio;
			meany2 += i->y*i->y*e_ratio;
		}
		double varx = meanx2 - meanx*meanx;
		double vary = meany2 - meany*meany;
		if(varx<vary)
		{
			return false;
		}
		else 
		{
			return true;
		}
	}
	else{return true;}
}

/*
Calculates an approximation of the theta incident angle by finding the distance between the right-bottom most pixel and left-top most pixel. 
*/
double ThetaLLM(particle const& p, double d)
{
	if(p.GetSize()>2)
	{
		double len = Distance(p.GetRightBottomMost(), p.GetLeftTopMost());
		return atan(len/d);
	}
	else 
		return 0;
}

/*
Calculates an approximation of the theta incident angle by first determining if the cluster is predominantly (i) horizontal or (ii) vertical then 
in each case determines the theta value by, 
(i)  Finding the distance between the right-bottom most pixel and left-top most pixel. 
(ii) Finding the distance between the bottom-right most pixel and top-left most pixel. 
*/
double ThetaImprovedLLM(particle const& p, double d)
{
	if(p.GetSize()>2)
	{
		//double XLike = PixelsInLine(p, 0, p.GetRightBottomMost(), p.GetLeftTopMost());
		//double YLike = PixelsInLine(p, 1, p.GetBottomRightMost(), p.GetTopLeftMost());d
		
		double len=0;
		if(GetFuncType(p))
			len = Distance(p.GetRightBottomMost(), p.GetLeftTopMost());
		else
			len = Distance(p.GetTopLeftMost(), p.GetBottomRightMost());
		return atan(len/d);
	}
	else
	{
		return 0;
	}
}

/*
Calculates an approximation of the theta incident angle by finding the distance between the minimum ToA pixel ("exit" pixel) and max ToA pixel ("entry" pixel). 
*/
double ThetaMinMax(particle const& p, double d)
{
	if(p.GetSize()>2)
	{
		double len = Distance(p.GetMinToAPixel(), p.GetMaxToAPixel());
		return atan(len/d);
	}
	else 
		return 0;
}

/*
Calculates an approximation of theta by using drift time equation to reconstruct the 3D path of radiation from which a line is fitted and theta value determined.
*/
double ThetaLineFit(particle const& p, double d)
{
	if(p.GetSize()>=2)
	{	
		double meanx=0, meany=0, meanxy=0, meanx2=0,meany2=0;
		
		//Original equation: ( (d*(UB+UD))/(2*UD) ) * (1 - exp(-2*UD*mu*((i->second).time - p.get_toa())*1e-9 / (d*d))) 
		//it was found to be more accurate to apply the normalisation factor which assumes the radiation travels completely through the detector
		double normalisation_factor = d / (p.GetMaxToA()-p.GetMinToA());//(1 - exp(-2*UD*mu*(p.GetMaxToA() - p.GetMinToA())*1e-9 / (d*d)));
		for (auto i = p.begin(); i != p.end(); i++ ) 
		{
			double x = normalisation_factor * (i->time-p.GetMinToA());//(1 - exp(-2*UD*mu*(i->time - p.GetMinToA())*1e-9 / (d*d)));
			double y = Distance(p.GetMinToAPixel(), *i);
			
			double e_ratio = (i->energy)/p.GetEnergy();
			meanx  += x*e_ratio;
			meany  += y*e_ratio;
			meanxy += x*y*e_ratio;
			meanx2 += x*x*e_ratio;
			meany2 += y*y*e_ratio;
		}
		return atan(abs((meanxy - meanx*meany)/(meanx2 - meanx*meanx)));
	}
	else 
	{
		return 0;
	}
}

/*
Calculates whether correction factors are neccessary in the line fit phi method.
*/
pair<bool,bool> Flipxy(particle const& p)
{
	double meanx=0, meany=0, meanxy=0, meanx2=0,meany2=0;
	double e_ratio;
	double varx, cov;

	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		double x = i->x;
		double y = i->time-p.GetMinToA();
		e_ratio = (i->energy)/p.GetEnergy();
		
		meanx  += x*e_ratio;
		meany  += y*e_ratio;
		meanxy += x*y*e_ratio;
		meanx2 += x*x*e_ratio;
		meany2 += y*y*e_ratio;
	}
	varx = meanx2 - meanx*meanx;
	cov = meanxy - meanx*meany;
	bool flipx = ((varx/cov) <0.0);
	meanx=0, meany=0, meanxy=0, meanx2=0,meany2=0;
	for (auto i=p.begin(); i != p.end(); i++ ) 
	{
		double x = i->y;//length(min_pos, pos);
		double y = i->time - p.GetMinToA();
		e_ratio = (i->energy)/p.GetEnergy();
		meanx  += x*e_ratio;
		meany  += y*e_ratio;
		meanxy += x*y*e_ratio;
		meanx2 += x*x*e_ratio;
		meany2 += y*y*e_ratio;
	}
	varx = meanx2 - meanx*meanx;
	cov = meanxy - meanx*meany;
	bool flipy = ((varx/cov) <0.0);
	return pair<bool,bool>(flipx, flipy);
}

/*
Calculates the phi value between two input pixel postion.
*/
double TwoPointPhi(PixelHit const& p1, PixelHit const& p2)
{
	double deltax = p2.y - p1.y;
	double deltay = p2.x - p1.x;
	double phi = atan2(deltax, deltay);
	if(phi<0)
		phi += 2*M_PI;
	//double phi = atan2(abs(deltay)/abs(deltax));
	
	/*if((deltax<0.0) & (deltay>0.0))
		phi = M_PI - phi; //(2*3.14159) - phi;
	else if((deltay<0.0)&(deltax<0.0))
	{
		phi =  M_PI + phi;
	}
	else if((deltay<0.0) & (deltax>0.0))
	{
		phi =  (2*M_PI) - phi;
	}*/
	//if(phi<0)
	//	phi += 2*M_PI;
	return phi;
}

/*
Calculates an approximation of the phi incident angle by finding the phi angle between the right-bottom most pixel and left-top most pixel.
Determining which is the entry and exit pixel by ToA information.
*/
double PhiLLMVert(particle const& p)
{
	if(p.GetTopLeftMost().time>p.GetBottomRightMost().time)
		return TwoPointPhi(p.GetTopLeftMost(), p.GetBottomRightMost());
	else 
		return TwoPointPhi(p.GetBottomRightMost(), p.GetTopLeftMost());
}
double PhiLLMHori(particle const& p)
{
	if(p.GetLeftTopMost().time>p.GetRightBottomMost().time)
		return TwoPointPhi(p.GetLeftTopMost(), p.GetRightBottomMost());
	else 
		return TwoPointPhi(p.GetRightBottomMost(), p.GetLeftTopMost());
}
/*
Calculates an approximation of the phi incident angle by first determining if the cluster is predominantly (i) horizontal or (ii) vertical then 
in each case determines the phi value by, 
(i)  Finding the distance between the right-bottom most pixel and left-top most pixel. 
(ii) Finding the distance between the bottom-right most pixel and top-left most pixel. 
Determining which is the entry and exit pixel by ToA information.
*/
double PhiImprovedLLM(particle const& p)
{
	if(p.GetSize()>1)
	{
		if(GetFuncType(p))
			if(p.GetLeftTopMost().time>p.GetRightBottomMost().time)
				return TwoPointPhi(p.GetLeftTopMost(), p.GetRightBottomMost());
			else 
				return TwoPointPhi(p.GetRightBottomMost(), p.GetLeftTopMost());
		else
			if(p.GetTopLeftMost().time>p.GetBottomRightMost().time)
				return TwoPointPhi(p.GetTopLeftMost(), p.GetBottomRightMost());
			else 
				return TwoPointPhi(p.GetBottomRightMost(), p.GetTopLeftMost());
	}
	else 
		return -1;
}

/*
Calculates an approximation of the theta incident angle by finding the distance between the minimum ToA pixel ("exit" pixel) and max ToA pixel ("entry" pixel). 
*/
double PhiMinMax(particle const& p)
{
	return TwoPointPhi(p.GetMaxToAPixel(), p.GetMinToAPixel());
}

/*
Calculates an approximation of phi by energy weighted 2D line fitting the x-y positons of the pixels.
*/
double PhiLineFit(particle const& p)
{
	
	double e_ratio, varx, vary, covxy;
	double meanx=0, meany=0, meanxy=0, meanx2=0,meany2=0;
	
	for (auto i=p.begin(); i != p.end(); i++ ) 
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
	covxy = meanxy - meanx*meany;
	
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
	double denomPlus = sqrt(aplus*aplus + bplus*bplus);
	double denomMinus= sqrtf(aminus*aminus + bminus*bminus);
	//cout<<aplus*aminus+bminus*bplus<<endl;
	//aplus = aplus/denomPlus;
	//bplus = bplus/denomPlus;
	
	double angle=0;
	pair<bool,bool> fxy = Flipxy(p);
	if(denomPlus>=denomMinus)
	{
		angle = atan2(bplus,aplus);
	}
	else
	{	
		angle = atan2(bminus,aminus) + M_PI_2 * (1 + 2*(bminus<0));
	}
	
	angle = atan2(bplus,aplus);
	/*if(fxy.first & fxy.second)
		if(angle>=M_PI-1)
			angle = 2*M_PI - angle;
	if(fxy.first==false & fxy.second==false)
		if(angle<=M_PI_2+1)
			angle += M_PI;
	if(fxy.first & fxy.second==false)
	{
		if(angle<=M_PI_2+1)
			angle += M_PI;//angle+=3*M_PI_2;
		if(angle<=M_PI+1)
			angle += M_PI;
	}*/
	if(angle<0)
		angle+=M_PI;
	
	if(fxy.first){
		if(fxy.second==false)
			angle+=M_PI;
		if(abs(angle-M_PI)<1e-1)
			angle+=M_PI;
		
	}
	else if(fxy.first==false)
	{
		if(fxy.second==false)
			angle+=M_PI;
		if(abs(angle-2*M_PI)<1e-1)
			angle-=M_PI;
		if(abs(angle)<1e-1)
			angle+=M_PI;
		
	}
	return angle;
	/*else if(fxy.second)
		if(angle>M_PI)
			return angle-M_PI;
		else 
			return angle;
	else 
		if(angle<M_PI)
			return angle+M_PI;
		else
			return angle;*/
	return 0;//angle;
		//else
		//	return 0;}
		//if(angle<M_PI)
		//	angle+=M_PI;
	/*if(fxy.first & fxy.second)
		if(angle<0)
			return angle+7*M_PI_4;
		else if(angle>M_PI_2)
			return angle - M_PI;
		else 
			return angle;
	
	//else if((fxy.first) & (fxy.second==false))
		//return angle;
		if(angle<0)
			return -angle;
		else if(angle<M_PI_2)
			return 2*M_PI - angle;
		else
			return angle;
		else if(angle>M_PI)
			return angle;
		else if(angle<M_PI_2)
			return angle + 7*M_PI_4;
		else
			return angle + M_PI;*/
	//else
	//	return 0;
	/*else if((fxy.first==false) & (fxy.second))
		if(angle<0)
			return M_PI + angle;
		else
			return angle;
	else //if((fxy.first==false) & (fxy.second==false))
		if(angle<0)
			return angle + 3*M_PI_2;
		else
			return angle + M_PI;*/
	//double angle = acos(bplus);
	
}

/*
Calculates an approxmation of phi by finding the weighted average "entry" and "exit" positions, 
by using the time interval from my minimum ToA value and maximum ToA value respectively
*/
double PhiTimeWeighted(particle const& p)
{
	auto skel = Skeletonise(p,SkelParameters[0],SkelParameters[1]);
	if(skel.GetSize()<=2)
	{
		skel = p;
	}
	if(skel.GetSize()>2)
	{
		PixelHit entry={0,0,0,0}, exit={0,0,0,0};
		double TimeSum =0, InverseTimeSum=0;
		for (auto i=skel.begin(); i != skel.end(); i++ ) 
		{
			TimeSum += i->time - skel.GetMinToA();
			InverseTimeSum += skel.GetMaxToA() - i->time;
		}
		for (auto i=skel.begin(); i != skel.end(); i++ ) 
		{
			double weight = (i->time - skel.GetMinToA())/(TimeSum);
			entry.x += i->x*weight;
			entry.y += i->y*weight;
			entry.time+=i->time*weight;
			
			weight = (skel.GetMaxToA() - i->time)/(InverseTimeSum);
			exit.x += i->x*weight;
			exit.y += i->y*weight;
			exit.time+=i->time*weight;
			
		}
		if(entry==exit)
			return -1;
		else 
			if(entry.time>exit.time)
				return TwoPointPhi(entry, exit);
			else
				return TwoPointPhi(exit, entry);
	}
	else if(skel.GetSize()==2)
	{
		if((skel.begin()->time)>((skel.begin()+1)->time))
			return TwoPointPhi(*skel.begin(), *(skel.begin()+1));
		else if((skel.begin()->time)<((skel.begin()+1)->time))
			return TwoPointPhi(*(skel.begin()+1), *skel.begin());
		return -1;
	}
	else 
		return -1;
}
