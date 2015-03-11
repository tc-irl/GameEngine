#pragma once
#include "Particle.h"
#include "Constraint.h"
#include <vector>

#define CONSTRAINT_ITERATIONS 15 // how many iterations of constraint satisfaction each frame (more is rigid, less is soft)

class Cloth
{
private:
	int numParticlesHeight;
	int numParticlesWidth;
	int numParticles;

	std::vector<Particle> particles;
	std::vector<Constraint> constraints;
public:
	Cloth(float width, float height, int numParticlesHeight, int numParticlesWidth);
	~Cloth(void);

	Particle *GetParticle(int x, int y) { return &particles[y*numParticlesWidth + x]; }
	void SetConstraint(Particle *p1, Particle *p2) { constraints.push_back(Constraint(p1,p2)); }

	glm::vec3 CalcNormal(Particle *p1, Particle *p2, Particle *p3);

	void ApplyForceToTriangle(Particle *p1,Particle *p2,Particle *p3, glm::vec3 direction);

	void AddForce(glm::vec3 direction);
	void AddWind(glm::vec3 direction);
	void AddBallCollision(glm::vec3 centre, float radius);

	void TimeStep();
	void DrawTriangle();
	void DrawCloth();
};

