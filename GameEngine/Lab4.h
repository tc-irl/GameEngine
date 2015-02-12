#pragma once
#include "Game.h"
#include "SimpleMesh.h"
#include "MeshLoader.h"
#include "Texture.h"
#include "RigidBody.h"

#include <map>
#include <list>
#include <string>

#define MAX 2

//using namespace glm;

class Lab4 : public Game
{
public:

	Lab4(void);
	~Lab4(void);

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
	void sweepAndPrune();
	float RandomNumber(float Min, float Max);
	bool checkNarrowPhaseCollision(RigidBody *, RigidBody *);
	glm::vec3 calculateMinkowskiDifference(RigidBody *rigidBody1, RigidBody *rigidBody2);
	glm::vec3 GetSupportPoint(RigidBody *rigidBody1, RigidBody *rigidBody2, glm::vec3 direction);
	bool simplexContainsOrigin(std::vector<glm::vec3> simplex, glm::vec3 dir);

	glm::vec3 centroid;
	float speed;
	glm::vec3 direction;
	glm::vec3 forcePoint;

	glm::vec3 newColor;
	double time, dt;// Current time and elapsed time

	MeshLoader *plane;
	MeshLoader *cubes[MAX], *spheres[MAX], *boundingCubes[MAX];
	MeshLoader *points[MAX * 8];
	RigidBody *rigidBodies[MAX];

	Shader *basicShader,*textureShader;
	std::vector<glm::vec3> transformedVertices;
	std::map <Shader::ShaderType, GLuint> possibleShaders;

	std::vector<std::pair<int, int>> collidingPairs;
	std::vector<std::pair<RigidBody *, RigidBody *>> collidingBodies;
	std::vector<glm::vec3> simplex;
	glm::vec3 dir;

	std::vector<glm::vec2> axisX, axisY, axisZ;  

	bool pauseScene;
	bool forceEnabled;
	bool set;

	Mode mode;

	struct Colliders
	{
		int objectID;
		int collidingObject;
	};

	std::vector<Colliders*> collidingPairB;
};

