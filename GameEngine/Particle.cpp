#include "Particle.h"


Particle::Particle(glm::vec3 position)
{
	this->position = position;
	previousPos = position;

	acceleration = glm::vec3(0,0,0);
	mass = 1.0f;
	fixed = false;
	normal = glm::vec3(0,0,0);
}

void Particle::AddToNormal(glm::vec3 normal)
{
	this->normal += glm::normalize(normal);
}

void Particle::AddForce(glm::vec3 force)
{
	acceleration += force / mass;
}

void Particle::CalculateNextPosition()
{
	if(!fixed)
	{
		glm::vec3 temp = position;
		position = position + (position-previousPos) * (1.0f - DAMPING) + acceleration * TIME_STEPSIZE2;
		previousPos = temp;
		acceleration = glm::vec3(0,0,0);
	}
}

void Particle::OffsetPosition(glm::vec3 velocity)
{
	if(!fixed)
	{
		position += velocity;
	}
}
