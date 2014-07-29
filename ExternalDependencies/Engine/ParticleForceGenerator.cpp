#include "ParticleForceGenerator.h"

void ParticleForceRegistry::updateForces(float duration)
{
	Registry::iterator i = registrations.begin();
	for(; i != registrations.end(); i++)
	{
		i->fg->updateForce(i->particle, duration);
	}
}

void ParticleForceRegistry::add(PhysicsParticle* particle, ParticleForceGenerator *fg)
{
	ParticleForceRegistration registration;
	registration.particle = particle;
	registration.fg = fg;
	registrations.push_back(registration);
}

void ParticleGravity::setGravity(glm::vec3 setGravity)
{
	gravity = setGravity;
}

void ParticleGravity::updateForce(PhysicsParticle* particle, float duration)
{
	duration;

	if(!particle->hasFiniteMass()) return;

	particle->addForce(gravity * particle->getMass());
}

void ParticleDrag::updateKforces(float kOne, float kTwo)
{
	k1 = kOne;
	k2 = kTwo;
}

void ParticleDrag::updateForce(PhysicsParticle* particle, float duration)
{
	duration;
	glm::vec3 force;
	force = particle->getVelocity();

	float dragCoeff = glm::sqrt((force.x * force.x) + (force.y * force.y) + (force.z * force.z));
	dragCoeff = (k2 * dragCoeff) + (k1 * (dragCoeff * dragCoeff));

	glm::normalize(force);
	force *= -dragCoeff;
	particle->addForce(force);
}

void ParticleSpring::setSpring(PhysicsParticle* Other, float SpringConstant, float RestLength)
{
	other = Other;
	springConstant = SpringConstant;
	restLength = RestLength;
}

void ParticleSpring::updateForce(PhysicsParticle* particle, float duration)
{
	duration;
	glm::vec3 force = particle->getPosition();
	force -= other->getPosition();

	float magnitude = glm::sqrt((force.x * force.x) + (force.y * force.y) + (force.z * force.z));
	magnitude = glm::abs(magnitude - restLength);
	magnitude *= springConstant;

	glm::normalize(force);
	force *= -magnitude;
	particle->addForce(force);
}

void ParticleAnchoredSpring::setAnchor(glm::vec3 *Anchor, float SpringConstant, float RestLength)
{
	anchor = Anchor;
	springConstant = SpringConstant;
	restLength = RestLength;
}

void ParticleAnchoredSpring::updateForce(PhysicsParticle* particle, float duration)
{
	duration;
	glm::vec3 force = particle->getPosition();
	force -= *anchor;

	float magnitude = glm::sqrt((force.x * force.x) + (force.y * force.y) + (force.z * force.z));
	magnitude = (restLength - magnitude) * springConstant;

	glm::normalize(force);
	force *= magnitude;
	particle->addForce(force);
}

void ParticleBungee::setBungee(PhysicsParticle* Other, float SpringConstant, float RestLength)
{
	other = Other;
	springConstant = SpringConstant;
	restLength = RestLength;
}

void ParticleBungee::updateForce(PhysicsParticle* particle, float duration)
{
	duration;
	glm::vec3 force = particle->getPosition();
	force -= other->getPosition();

	float magnitude = glm::sqrt((force.x * force.x) + (force.y * force.y) + (force.z * force.z));
	if(magnitude <= restLength)
		return;

	magnitude = springConstant * (restLength - magnitude);

	glm::normalize(force);
	force *= magnitude;
	particle->addForce(force);
}

void ParticleBuoyancy::setBuoyancy(float MaxDepth, float Volume, float WaterHeight, float LiquidDensity)
{
	maxDepth = MaxDepth;
	volume = Volume;
	waterHeight = WaterHeight;
	liquidDensity = LiquidDensity;
}

void ParticleBuoyancy::updateForce(PhysicsParticle* particle, float duration)
{
	duration;
	float depth = particle->getPosition().y;

	if(depth >= waterHeight + maxDepth)
		return;
	glm::vec3 force(0,0,0);
	if(depth <= waterHeight - maxDepth)
	{
		force.y = liquidDensity * volume;
		particle->addForce(force);
		return;
	}

	force.y = liquidDensity * volume *
		(depth - maxDepth - waterHeight) / 2 * maxDepth;
	particle->addForce(force);
}