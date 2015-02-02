#pragma once
#include "Game.h"
#include "SimpleMesh.h"
#include "Texture.h"
#include "RigidBody.h"

#include <list>

#define MAX 30
class Lab3 : public Game
{


public:

	Lab3(void);
	~Lab3(void);

	typedef enum Mode {NONE, SPHERE, AABB, MULTI};

	void run() override;
	void init(char** argv);
	void initShaders();
	void initTweakBar();
	void initModels();
	void initCube();
	void initTextures();
	void update();
	void checkForSphereCollision();
	void checkForBoundingBoxCollison();
	bool checkForSphereToBoxCollision(SimpleMesh *cube, SimpleMesh *sphere);
	float RandomNumber(float Min, float Max);

	glm::vec3 centroid;
	float speed;
	glm::vec3 direction;
	glm::vec3 forcePoint;

	glm::vec3 newColor;
	double time, dt;// Current time and elapsed time

	SimpleMesh *plane;
	SimpleMesh *cubes[MAX], *spheres[MAX], *boundingCubes[MAX];
	RigidBody *rigidBodies[MAX];

	Shader *basicShader,*textureShader;
	std::vector<glm::vec3> transformedVertices;

	bool pauseScene;
	bool forceEnabled;
	bool set;

	Mode mode;
};

