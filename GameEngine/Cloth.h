#pragma once
#include "Particle.h"
#include "Constraint.h"
#include "Triangle.h"
#include <gl/glew.h>
#include <vector>
#include <iostream>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>


#define CONSTRAINT_ITERATIONS 15 // how many iterations of constraint satisfaction each frame (more is rigid, less is soft)
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

class Cloth
{
private:
	int numParticlesHeight;
	int numParticlesWidth;
	int numParticles;

	float width;
	float height;

	std::vector<Particle> particles;
	std::vector<Particle*> orderedParticles;
	std::vector<glm::vec3> orderedColors;
	std::vector<Constraint> constraints;
	std::vector<Triangle> triangles;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> colors;

	GLuint vao, vbo;
	GLuint vSize, cSize, nSize;
	GLuint modelLoc, viewLoc, projLoc;

	GLuint shaderID;

public:

	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

public:
	Cloth(float width, float height, int numParticlesHeight, int numParticlesWidth);
	~Cloth(void);

	void SetPos(glm::vec3 position){this->position = position;}
	glm::vec3 GetPos(){return position;}

	void SetOrientation(glm::quat orientation) {this->orientation = orientation;}
	glm::quat GetOrientation() {return orientation;}

	void SetScale(glm::vec3 scale) {this->scale = scale;}
	glm::vec3 GetScale() {return scale;}

	glm::mat4 GetTransformationMatrix();

	void SetShader(GLuint shaderID) {this->shaderID = shaderID;}
	Particle *GetParticle(int x, int y) { return &particles[y*numParticlesWidth + x]; }
	void SetConstraint(Particle *p1, Particle *p2) { constraints.push_back(Constraint(p1,p2)); }

	glm::vec3 CalcNormal(Particle *p1, Particle *p2, Particle *p3);

	void ApplyForceToTriangle(Particle *p1,Particle *p2,Particle *p3, glm::vec3 direction);

	void AddForce(glm::vec3 direction);
	void AddWind(glm::vec3 direction);
	void AddBallCollision(glm::vec3 centre, float radius);
	void AddPlaneCollision(glm::vec3 planePos);

	void TimeStep();
	void GenerateBuffer();

	void Update(glm::mat4 view, glm::mat4 proj);
	void DrawTriangle();
	void DrawCloth();
	float RandomNumber(float Min, float Max);
	void AddSelfCollision();
};

