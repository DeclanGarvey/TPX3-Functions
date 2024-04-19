#pragma once

#include <vector>
#include "PixelStructure.h"

class particle 
{
	private:
		std::vector<PixelHit> cluster_;
		bool edge_cluster_=false;
		PixelHit MinToAPixel_{-1,-1,-1,-1};
		PixelHit MaxToAPixel_{-1,-1,-1,-1};
		double sum_time = 0.0;
		int size_=0;
		
		
		PixelHit lefttopmost_{300,-1, -1,-1}, rightbottommost_{-1,300, -1,-1};
		PixelHit topleftmost_{300,-1, -1,-1}, bottomrightmost_{-1,300, -1,-1};
		double sum_energy_=0;
		double height_=-1.0;
	public:
		int ParticleType=-1;
		double PrimaryEnergy=-1;
		int MorphologicalClass=-1;
		double phi=-1;
		double theta=-1;
		double StoppingPower=-1;
		double AcquisitionTime=-1;
		int RegionID=-1;
		double SatPosX=0;
		double SatPosY=0;
		double SatAltitude=0;
		double FrameOccupancy=0; 
		
		void Insert(const PixelHit& Hit);
		
		std::vector<PixelHit> GetCluster() const;
		std::vector<PixelHit>::const_iterator begin() const;
		std::vector<PixelHit>::const_iterator end() const;
		
		int GetSize() const;
		double GetHeight() const;
		double GetEnergy() const;
		PixelHit GetRightBottomMost() const;
		PixelHit GetLeftTopMost() const;
		PixelHit GetTopLeftMost() const;
		PixelHit GetBottomRightMost() const;
		double GetMinToA() const;
		PixelHit GetMinToAPixel() const;
		double GetMaxToA() const;
		PixelHit GetMaxToAPixel() const;
		double GetTimeSum() const;
		bool IsEdge() const;
		void Clear();
		double GetDeltaToA() const;
		bool IsEmpty() const;
		PixelHit GetRandomPoint();
		void Translate(const PixelHit& Translation);
		int GetRegionID() const;
		void UpdateMorphologicalClass(int NewMorphologicalClass);
		void UpdatePhi(double NewPhi);
		void UpdateTheta(double NewTheta);
};
