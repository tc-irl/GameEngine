#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Particle.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class Triangle
{
public:
	Triangle(GLuint shaderID,Particle *p1, Particle *p2, Particle *p3);
	~Triangle(void) {};

	void GenerateBuffer();
	void SetColor(glm::vec3 color);
	void SetP1(Particle *p1) {this->p1 = p1;}
	void SetP2(Particle *p2) {this->p2 = p2;}
	void SetP3(Particle *p3) {this->p3 = p3;}

	glm::mat4 GetTransformationMatrix();

	void Update();
	void Draw();
public:
	
	glm::vec3 color; 

	std::vector<glm::vec3> points;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> normals;

	Particle *p1; 
	Particle *p2;
	Particle *p3;

	GLuint vao, vbo;
	GLuint vSize, cSize, nSize;
	GLuint modelLoc, viewLoc, projLoc;

	GLuint shaderID;

};

