#include "PhysicsParticle.h"

void PhysicsParticle::integrate(float duration)
{
	if(inverseMass <= 0.0f)
		return;

	position += velocity * duration;
	glm::vec3 resultingAcc = acceleration;
	resultingAcc += forceAccum * inverseMass;
	velocity += resultingAcc*duration;
	velocity *= glm::pow(damping, duration);

	clearAccumulator();
}

void PhysicsParticle::clearAccumulator()
{
	forceAccum = glm::vec3();
}

void PhysicsParticle::addForce(const glm::vec3 &force)
{
	forceAccum += force;
}

bool PhysicsParticle::hasFiniteMass()
{
	return inverseMass >= 0.0f;
}

float PhysicsParticle::getMass()
{
	return (float)(1/inverseMass);
}

glm::vec3 PhysicsParticle::getVelocity()
{
	return velocity;
}

glm::vec3 PhysicsParticle::getPosition()
{
	return position;
}

glm::vec3 PhysicsParticle::getAccelertion()
{
	return acceleration;
}

void PhysicsParticle::setMass(float mass)
{
	inverseMass = mass;
}

void PhysicsParticle::setVelocity(float x, float y, float z)
{
	velocity.x = x;
	velocity.y = y;
	velocity.z = z;
}

void PhysicsParticle::setAcceleration(float x, float y, float z)
{
	acceleration.x = x;
	acceleration.y = y;
	acceleration.z = z;
}

void PhysicsParticle::setDamping(float d)
{
	damping = d;
}

void PhysicsParticle::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void PhysicsParticle::setForceAcc(glm::vec3 forceAcc)
{
	forceAccum = forceAcc;
}
