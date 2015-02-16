#pragma once
#include "Game.h"
#include "SimpleMesh.h"
#include "MeshLoader.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Line.h"

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

	typedef enum Mode {NONE, SPHERE, AABB, AABB_PLANE, MULTI};

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
	glm::vec3 GetSupportPoint(glm::vec3 farA, glm::vec3 farB);
	bool simplexContainsOrigin(std::vector<glm::vec3> &simplex, glm::vec3 &dir);
	bool checkEdge(std::vector<glm::vec3> &simplex, glm::vec3 &dir);
	bool checkTriangle(std::vector<glm::vec3> &simplex,glm::vec3 &dir);
	bool checkTetrahedron(std::vector<glm::vec3> & simplex, glm::vec3 & dir);

	void checkForBoundingBoxCollisonWithPlane();
	float GetDistanceToPlane(glm::vec3 point); 
	bool checkBoxToPlaneCollision();
	glm::vec3 GetClosestPointToPlane(std::vector<glm::vec3> points);
	glm::vec3 GetPointOnPlane(glm::vec3 closestPoint);
	void DrawLine();
	void DefineLine(glm::vec3 point1, glm::vec3 point2);

	float distanceToClosestPoint;
	glm::vec3 closestPoint;
	glm::vec3 planePoint;

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

	MeshLoader *sphere;

	Line *line;
	Line *directionNormal;

	Shader *basicShader,*textureShader;
	std::vector<glm::vec3> transformedVertices;
	std::map <Shader::ShaderType, GLuint> possibleShaders;

	std::vector<std::pair<int, int>> collidingPairs;
	std::vector<std::pair<RigidBody *, RigidBody *>> collidingBodies;
	std::vector<glm::vec3> simplex;

	std::vector<glm::vec2> axisX, axisY, axisZ;  

	bool pauseScene;
	bool forceEnabled;
	bool gravityEnabled;
	bool collisionsEnabled;
	bool set;

	Mode mode;

	struct Colliders
	{
		int objectID;
		int collidingObject;
	};

	std::vector<Colliders*> collidingPairB;
};

