#pragma once
class particle;

#include "ParticleGeneratorBaseClass.h"

class ParticleGeneratorBaseClass
{
	public:
		virtual bool AssignParticle(particle& p){return false;};
};
