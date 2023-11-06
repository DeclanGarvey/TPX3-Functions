#pragma once

struct PixelHit
{
	double x;
	double y;
	double time;
	double energy;
	
	void operator -=(PixelHit const& p);
	bool operator ==(PixelHit const& p);
};
