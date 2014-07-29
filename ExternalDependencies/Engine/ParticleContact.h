#pragma once
#ifndef PARTICLE_CONTACT	
#define PARTICLE_CONTACT	
#include "PhysicsParticle.h"
#include "glm\glm.hpp"
#include "ExportHeader.h"

class ENGINE_SHARED ParticleContact
{
public:
	PhysicsParticle* particle[2];
	float restitution;
	glm::vec3 contactNormal;
	float penetration;
	glm::vec3 particleMovement[2];

void resolve(float duration);
float calculateSeparatingVelocity() const;
void resolveVelocity(float duration);
void resolveInterpenetration(float duration);
void resovleRestingContact(float duration);

protected:
		

private:

};

class ENGINE_SHARED ParticleContactResolver
{
protected:
	unsigned iterations;
	unsigned iterationsUsed;

public:
	void setIterations(unsigned Iterations);
	void resolveContacts(ParticleContact *contactArray, unsigned numContacts, float duration);

};

#endif

