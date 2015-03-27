#pragma once

#include "Particle.h"

class Constraint
{
private:
	float initialDistance;
	float currentDistance;

public:
	Particle * p1;
	Particle * p2;
public:
	Constraint(Particle *p1, Particle *p2);
	~Constraint(void) {};

	void SetInitialDistance(float initialDistance) {this->initialDistance = initialDistance;}
	float GetInitialDistance() {return initialDistance;}

	void SetCurrentDistance(float currentDistance) {this->currentDistance = currentDistance;}
	float GetCurrentDistance() {return currentDistance;}

	void HandleContraint();


	bool CompareParticles(Particle *p);
};

