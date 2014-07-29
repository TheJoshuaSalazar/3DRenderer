#include "ParticleContact.h"


void ParticleContact::resolve(float duration)
{
	resolveVelocity(duration);
	resolveInterpenetration(duration);
}

float ParticleContact::calculateSeparatingVelocity() const
{
	glm::vec3 relativeVelocity = particle[0]->getVelocity();
	if(particle[1]) 
		relativeVelocity -= particle[1]->getVelocity();

	return glm::dot(relativeVelocity, contactNormal);
}

void ParticleContact::resolveVelocity(float duration)
{
	duration;
	float separatingVelocity = calculateSeparatingVelocity();

	/*if(particle[0]->getVelocity().y <= particle[0]->getVelocity().y * 0.1f)
		separatingVelocity = 0;*/

	if(separatingVelocity > 0)
	{
		return;
	}

	//float newSepVelocity = -separatingVelocity;

	glm::vec3 accCausedVelocity = particle[0]->getAccelertion();
	if(particle[1])

	accCausedVelocity -= particle[1]->getAccelertion();
	float accCausedSepVelocity = glm::dot(accCausedVelocity, contactNormal) * duration; 

	if(accCausedSepVelocity < 0)
	{
		separatingVelocity -= accCausedSepVelocity;
	}

	//float deltaVelocity = newSepVelocity - separatingVelocity;

	float totalInverseMass = particle[0]->getMass();

	if(particle[1])
		totalInverseMass += particle[1]->getMass();

	if(totalInverseMass <= 0)
		return;

	float impulse = -separatingVelocity * (restitution+1)/totalInverseMass;
	glm::vec3 impulsePerIMass = contactNormal * impulse;



	particle[0]->setVelocity(particle[0]->getVelocity().x + (impulsePerIMass.x * particle[0]->getMass()),
							 particle[0]->getVelocity().y + (impulsePerIMass.y * particle[0]->getMass()),
							 particle[0]->getVelocity().z + (impulsePerIMass.z * particle[0]->getMass()));


	/*
		particle[0]->setVelocity(particle[0]->getPosition().x,
		-(particle[0]->getAccelertion().y),
		 0);*/


	if(particle[1])
	{
		particle[1]->setVelocity((particle[1]->getVelocity() + impulsePerIMass).x * -particle[1]->getMass(),
								(particle[1]->getVelocity() + impulsePerIMass).x * -particle[1]->getMass(),
								(particle[1]->getVelocity() + impulsePerIMass).z * -particle[1]->getMass());
	}
}

void ParticleContact::resolveInterpenetration(float duration)
{
	duration;

	if(penetration <= 0)
		return;

	float totalInverseMass = particle[0]->getMass();

	if(particle[1])
		totalInverseMass += particle[1]->getMass();

	if(totalInverseMass <= 0)
		return;

	glm::vec3 movePerIMass = contactNormal * (penetration/ totalInverseMass);
	particleMovement[0] = movePerIMass * particle[0]->getMass();

	if(particle[1])
		particleMovement[1] = movePerIMass * -particle[1]->getMass();
	else
		particleMovement[1].null;

	particle[0]->setPosition(particle[0]->getPosition().x + particleMovement[0].x,
							particle[0]->getPosition().y + particleMovement[0].y,
							particle[0]->getPosition().z + particleMovement[0].z);

	if(particle[1])
		particle[1]->setPosition(particle[1]->getPosition().x + particleMovement[1].x,
							particle[1]->getPosition().y + particleMovement[1].y,
							particle[1]->getPosition().z + particleMovement[1].z);
}

void ParticleContactResolver::setIterations(unsigned Iterations)
{
	iterations = Iterations;
}

void ParticleContactResolver::resolveContacts(ParticleContact *contactArray, unsigned numContacts,
												float duration)
{
	unsigned i;

	iterationsUsed = 0;
	while(iterationsUsed < iterations)
	{
		float max = FLT_MAX ;
		unsigned maxIndex = numContacts; 
		for(i = 0; i < numContacts; i++)
		{
			float sepVel = contactArray[i].calculateSeparatingVelocity();
			if(sepVel < max &&
				(sepVel < 0 || contactArray[i].penetration > 0))
			{
				max = sepVel;
				maxIndex = i;
			}
		}

		if(maxIndex == numContacts) 
			break;

		contactArray[maxIndex].resolve(duration);
		glm::vec3 *move = contactArray[maxIndex].particleMovement;
		for(i = 0; i < numContacts; i++)
		{
			if(contactArray[i].particle[0] == contactArray[maxIndex].particle[0])
				contactArray[i].penetration -= glm::dot(move[0], contactArray[i].contactNormal);
			else if(contactArray[i].particle[0] == contactArray[maxIndex].particle[1])
				contactArray[i].penetration -= glm::dot(move[1], contactArray[i].contactNormal);

			if(contactArray[i].particle[1])
			{
				if(contactArray[i].particle[1] == contactArray[maxIndex].particle[0])
					contactArray[i].penetration += glm::dot(move[0], contactArray[i].contactNormal);
				else if (contactArray[i].particle[1] == contactArray[maxIndex].particle[1])
					contactArray[i].penetration += glm::dot(move[1], contactArray[i].contactNormal);
			}
		}

		iterationsUsed++;
	}

}