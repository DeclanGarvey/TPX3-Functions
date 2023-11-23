#include <tgmath.h>
#include <stdlib.h>     /* srand, rand */
#include <iostream>
//#include "PixelStructure.h"
#include "Particle.h"
using namespace std;

/*
Inserts a pixel with position (x, y) and value (time, energy) into particle object saving a number of variables for later quick access, reducing compuational intensity later on.
*/
void particle::Insert(const PixelHit& Hit)
{
	cluster_.push_back(Hit);
	if((Hit.x<1.0) | (Hit.x>254.0) | (Hit.y<1.0) | (Hit.y>254.0))
	{
		edge_cluster_=true;
	}
	
	if((Hit.time < MinToAPixel_.time) | (MinToAPixel_.time == -1.0))
	{
		MinToAPixel_ = Hit;
	}
	if(Hit.time > MaxToAPixel_.time)
	{
		MaxToAPixel_ = Hit;
	}
	if(Hit.energy > height_)
	{
		height_=Hit.energy;
	}
	
	if( (Hit.x < lefttopmost_.x) || ( (Hit.x == lefttopmost_.x) && (Hit.y > lefttopmost_.y) ) )
	{
		lefttopmost_ = Hit;
	}
	if( (Hit.x > rightbottommost_.x) || ( (Hit.x == rightbottommost_.x) && (Hit.y < rightbottommost_.y) ) )
	{
		rightbottommost_ = Hit;
	}
	if( (Hit.y > topleftmost_.y) || ( (Hit.y == topleftmost_.y) && (Hit.x < topleftmost_.x )) )
	{
		topleftmost_ = Hit;
	}
	if( (Hit.y < bottomrightmost_.y) || ( (Hit.y == bottomrightmost_.y) && (Hit.x > bottomrightmost_.x ) ))
	{
		bottomrightmost_ = Hit;
	}
	
	sum_energy_+=Hit.energy;
	sum_time += Hit.time;
	size_+=1;
	//std::cout<<size_<<endl;
}


vector<PixelHit> particle::GetCluster() const
{
	return cluster_;
}


void particle::Clear()
{
	//if(size_>0)
	//{
		RegionID = 0.0;
		sum_energy_=0.0;
		height_=-1.0;
		MinToAPixel_ = {-1,-1,-1,-1};
		MaxToAPixel_ = {-1,-1,-1,-1};
		size_=0.0;
		sum_time = 0.0;
		cluster_.clear();
		edge_cluster_=false;
		lefttopmost_ = {300,-1, -1,-1};
		rightbottommost_ = {-1,300, -1,-1};
		topleftmost_ = {300,-1, -1,-1};
		bottomrightmost_ = {-1,300, -1,-1};
		ParticleType=-1;
		PrimaryEnergy=-1;
		MorphologicalClass=-1;
		phi=-1;
		theta=-1;
		StoppingPower=-1;
		AcquisitionTime=0;
	//}
}
vector<PixelHit>::const_iterator particle::begin() const 
{
	return cluster_.begin(); 
}
vector<PixelHit>::const_iterator particle::end() const 
{
	return cluster_.end(); 
}
int particle::GetSize() const
{
	return size_;
}
bool particle::IsEdge() const
{
	return edge_cluster_;
}
double particle::GetEnergy() const 
{
	return sum_energy_;
}
double particle::GetHeight() const
{
	return height_;
}
PixelHit particle::GetRightBottomMost() const
{
	return rightbottommost_;
}
PixelHit particle::GetLeftTopMost() const
{
	return lefttopmost_;
}
PixelHit particle::GetTopLeftMost() const
{
	return topleftmost_;
}
PixelHit particle::GetBottomRightMost() const
{
	return bottomrightmost_;
}
PixelHit particle::GetMinToAPixel() const 
{
	return MinToAPixel_;
}
double particle::GetMinToA() const
{
	return MinToAPixel_.time;
}
PixelHit particle::GetMaxToAPixel() const 
{
	return MaxToAPixel_;
}
double particle::GetMaxToA() const
{
	return MaxToAPixel_.time;
}
double particle::GetTimeSum() const 
{
	return sum_time;
}
double particle::GetDeltaToA() const
{
	return MaxToAPixel_.time - MinToAPixel_.time;
}
bool particle::IsEmpty() const
{
	return (size_==0);
}
PixelHit particle::GetRandomPoint()
{
	return cluster_[rand() % size_ + 1];
}

int particle::GetRegionID() const
{
	return RegionID;
}

void particle::Translate(const PixelHit& Translation)
{
	for(auto& i : cluster_) 
		i -= Translation;
}

void particle::UpdateMorphologicalClass(int NewMorphologicalClass)
{
	MorphologicalClass = NewMorphologicalClass;
}
void particle::UpdatePhi(double NewPhi)
{
	phi=NewPhi;
}
void particle::UpdateTheta(double NewTheta)
{
	theta=NewTheta;
}



