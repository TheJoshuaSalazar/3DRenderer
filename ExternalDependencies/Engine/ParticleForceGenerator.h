#pragma once 
#ifndef PARTICLE_FORCE_GENERATOR
#define PARTICLE_FORCE_GENERATOR
#include "PhysicsParticle.h"
#include <glm\glm.hpp>
#include "ExportHeader.h"
#include <vector>

class ENGINE_SHARED ParticleForceGenerator
{

public:
	virtual void updateForce(PhysicsParticle *particle, float duration) = 0;
};


class ENGINE_SHARED ParticleForceRegistry
{
protected:
	struct ParticleForceRegistration
	{
		PhysicsParticle *particle;
		ParticleForceGenerator *fg;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;

public:
	void add(PhysicsParticle* particle, ParticleForceGenerator *fg);
	void remove(PhysicsParticle* particle, ParticleForceGenerator *fg);
	void clear();
	void updateForces(float duration);
	
};

class ENGINE_SHARED ParticleGravity : public ParticleForceGenerator
{
	glm::vec3 gravity;

public:
	//ParticleGravity(const glm::vec3 &gravity);
	void setGravity(glm::vec3 setGravity);
	virtual void updateForce(PhysicsParticle *particle, float duration);
};

class ENGINE_SHARED ParticleDrag : public ParticleForceGenerator
{
	float k1;
	float k2;

public:
	//ParticleDrag(float k1, float k2);
	void updateKforces(float kOne, float  kTwo);
	virtual void updateForce(PhysicsParticle* particle, float duration);
};

class ENGINE_SHARED ParticleSpring : public ParticleForceGenerator
{
	PhysicsParticle *other;
	float springConstant;
	float restLength;

public:
	void setSpring(PhysicsParticle *Other, float SpringConstant, float RealLength);
	virtual void updateForce(PhysicsParticle* particle, float duration);
};

class ENGINE_SHARED ParticleAnchoredSpring : public ParticleForceGenerator
{
	glm::vec3 *anchor;
	float springConstant;
	float restLength;

public:
	void setAnchor(glm::vec3 *Anchor, float SpringConstant, float RestLength);
	virtual void updateForce(PhysicsParticle *particle, float duration);
};

class ENGINE_SHARED ParticleBungee : public ParticleForceGenerator
{
	PhysicsParticle *other;
	float springConstant;
	float restLength;

public:
	void setBungee(PhysicsParticle *Other, float SpringConstant, float RealLength);
	virtual void updateForce(PhysicsParticle* particle, float duration);
};

class ENGINE_SHARED ParticleBuoyancy : public ParticleForceGenerator
{
	float maxDepth;
	float volume;
	float waterHeight;
	float liquidDensity;

public:
	void setBuoyancy(float MaxDepth, float Volume, float WaterHeight, float LiquidDensity);
	virtual void updateForce(PhysicsParticle* particle, float duration);
};


#endif