#ifndef PHYSICS_PARTICLE
#define PHYSICS_PARTICLE
#include "glm\glm.hpp"
#include "ExportHeader.h"

class ENGINE_SHARED PhysicsParticle
{
protected:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 forceAccum;
	float damping;
	float inverseMass;


public:
	void integrate(float duration);
	void clearAccumulator();
	void addForce(const glm::vec3 &force);
	bool hasFiniteMass();
	float getMass();
	glm::vec3 getVelocity();
	glm::vec3 getAccelertion();
	glm::vec3 getPosition();
	void setMass(float mass);
	void setForceAcc(glm::vec3 forceAcc);
	void setVelocity(float x, float y, float z);
	void setAcceleration(float x, float y, float z);
	void setDamping(float damping);
	void setPosition(float x, float y, float z);
	
};

#endif

