#pragma once
#include <glm/glm.hpp>
#include <gl/glew.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <iostream>

#define MAX 30

class RigidBody
{
public:
	RigidBody();
	~RigidBody() {};

	float mass;
	float inverseMass;

	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

	glm::mat3 IBody;
	glm::mat3 invIBody;

	glm::vec3 velocity;
	glm::vec3 linearMomentum;
	
	glm::vec3 angularVelocity;
	glm::vec3 angularMomentum;

	glm::vec3 force;
	glm::vec3 torque;

	glm::vec3 centreOfMass;
	std::vector<glm::vec3> indices;
	std::vector<glm::vec3> vertices;

	std::vector<glm::vec3> transformedPoints;
	std::vector<glm::vec3> transformedVertices;

	glm::vec3 minExtents, maxExtents;

	bool gravity;
	bool forceEnabled;
	float distanceToCOM;
public:

	void calculateVelocity(float speed, glm::vec3 direction) {velocity = speed * direction;}
	void setVertices(std::vector<glm::vec3> vertices, std::vector<glm::vec3> indices);

	void enableGravity();
	void applyForce(glm::vec3 point, glm::vec3 force);

	glm::mat4 getTransformationMatrix();

	void transVertices(glm::quat orientation, glm::vec3 position);
	glm::vec3 getCentreOfMass();
	float calculateDistanceToCOM();
	void calcMinMaxExtents();
	void limitBoundary();

	void update(float dt);
	void updateModel(GLuint modelLoc);
	float initialDistanceToCOM();



};

