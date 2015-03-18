#pragma once

#include <glm/glm.hpp>

#define DAMPING 0.01f // how much to damp the cloth simulation each frame
#define TIME_STEPSIZE2 0.5f*0.5f // how large time step each particle takes each frame

class Particle
{
private:
	bool fixed;
	float mass;

	glm::vec3 position;
	glm::vec3 previousPos;
	glm::vec3 acceleration;
	glm::vec3 normal;

public:
	Particle() {};
	Particle(glm::vec3 position);
	~Particle(void) {};

	void MakeUnmovable() {fixed = true;}
	void SetFixed(bool fixed) {this->fixed = fixed;}
	bool GetFixed() {return fixed;}

	void SetPos(glm::vec3 position) {this->position = position;}
	glm::vec3 GetPos() {return position;}

	glm::vec3 GetPreviousPos() {return previousPos;}

	void SetAcceleration(glm::vec3 acceleration) {this->acceleration = acceleration;}
	glm::vec3 GetAcceleration() {return acceleration;}
	void ResetAcceleration() {acceleration = glm::vec3(0,0,0);}

	void SetMass(float mass) {this->mass = mass;}
	float GetMass() {return mass;}

	void SetNormal(glm::vec3 normal) {this->normal = normal;}
	glm::vec3 GetNormal() {return normal;}
	void ResetNormal() {normal = glm::vec3(0,0,0);}

	void AddToNormal(glm::vec3 normal);
	void AddForce(glm::vec3 force);
	void CalculateNextPosition();
	void OffsetPosition(glm::vec3 velocity);


};

