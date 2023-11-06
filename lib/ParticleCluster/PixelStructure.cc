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
/*bool position::operator ==( position const& p) const
{
    return (x == p.x) && ( y == p.y);
}
bool position::operator <( position const& pos) const
{
    return (x < pos.x) || ( x == pos.x && y < pos.y );
}
bool position::operator >( position const& pos) const
{
    return (x > pos.x) || ( x == pos.x && y > pos.y );
}
bool position::operator <( double const& num) const
{
    return (x < num) && ( y < num);
}
bool position::operator >( double const& num) const
{
    return (x > num) && ( y > num);
}

bool value::operator ==( value const& val) const
{
    return (time == val.time) && ( energy == val.energy);
}*/
