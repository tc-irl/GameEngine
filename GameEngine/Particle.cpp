#include "Particle.h"


Particle::Particle(glm::vec3 position, int ID)
{
	this->position = position;
	this->ID =ID;

	previousPos = position;

	acceleration = glm::vec3(0,0,0);
	mass = 3.0f;
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

void Particle::CalculateNextPosition(glm::vec3 clothPos, glm::quat clothOrientation, glm::vec3 clothScale)
{
	//transformedVertices.push_back(vertices[i] * orientation * scale + position);

	if(!fixed)
	{
		glm::vec3 temp = position;
		position = position + (position-previousPos) * (1.0f - DAMPING) + acceleration * TIME_STEPSIZE2;
		previousPos = temp;
		acceleration = glm::vec3(0,0,0);
	}
}

glm::vec3 Particle::GetNextPosition()
{
	glm::vec3 test = position + (position-previousPos) * (1.0f - DAMPING) + acceleration * TIME_STEPSIZE2;
	
	return test;
}

void Particle::OffsetPosition(glm::vec3 velocity)
{
	if(!fixed)
	{
		position += velocity;
	}
}
