#include "PixelStructure.h"

void PixelHit::operator -=(PixelHit const& p)
{
	this->x = this->x - p.x;
	this->y = this->y - p.y;
	this->time = this->time - p.time;
	this->energy = this->energy - p.energy;
}
bool PixelHit::operator ==(PixelHit const& p)
{
	if((this->x==p.x) & (this->y==p.y)
	& (this->time==p.time) & (this->energy==p.energy))
		return true;
	else 
		return false;
}
