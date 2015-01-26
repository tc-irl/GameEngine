#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

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

	bool gravity;
	bool forceEnabled;

public:

	void calculateVelocity(float speed, glm::vec3 direction) {velocity = speed * direction;}

	void enableGravity();
	void applyForce(glm::vec3 point, glm::vec3 force);

	glm::mat4 getTransformationMatrix();

	void update(float dt);

};

