#pragma once
#include "Game.h"
#include "Texture.h"
#include "ObjectBuffer.h"
#include "RigidBody.h"

#ifndef LAB2_H
#define LAB2_H

class Lab2 : public Game
{
public:
	Lab2(void);
	~Lab2(void);

	void run() override;
	void init(char** argv);
	void initShaders();
	void initTweakBar();
	void initModels();
	void initCube();
	void initTextures();

	void Draw();
	void update(GLuint modelLoc, float timeDelta);
	double time, dt;// Current time and elapsed time
	
	glm::vec3 centroid;
	float speed;
	glm::vec3 direction;
	glm::vec3 forcePoint;
private:
	Shader *shader;
	TwBar *bar; 
	ObjectBuffer* objectBuffer;
	RigidBody *rigidBody;
	GLuint VAO, vertexBuffer;
	GLuint sampler;
};

#endif