#include "RigidBody.h"



RigidBody::RigidBody()
{
	position = glm::vec3(0,0,0);
	orientation = glm::quat(glm::vec3(0,0,0));
	scale = glm::vec3(1,1,1);

	gravity = false;
	forceEnabled = false;

	mass = 1.0f; 
	inverseMass = 1.0f;

	velocity = glm::vec3();
	linearMomentum = glm::vec3();

	angularVelocity = glm::vec3();
	angularMomentum = glm::vec3();

	force = glm::vec3();
	torque = glm::vec3();
	IBody = glm::mat3();
	invIBody = glm::mat3();
}

void RigidBody::enableGravity()
{
	if(gravity)
	{
		force += glm::vec3(0.0f,-9.81f, 0.0f);
	}	
}

void RigidBody::applyForce(glm::vec3 point, glm::vec3 force)
{
	if(forceEnabled)
	{
		this->force = force;
		torque = glm::cross(point - centreOfMass, this->force);
	}
}

void RigidBody::update(float dt)
{
	// http://stackoverflow.com/questions/12758143/3d-rigid-body-physics-opengl -> Helped with Inertial tensor, could have implement IBody the way done here
	// https://www.scss.tcd.ie/John.Dingliana/cs7057/cs7057-1415-03a-RigidBodyMotion.pdf -> Helped with most of the requirements. 

	inverseMass = 1.0f / mass;

	centreOfMass += position;

	linearMomentum += force * (dt / 4); // 𝑭Δ𝑡/𝑚  -> multiply be inverse mass in position
	angularMomentum += torque * (dt / 4); // Δ𝝎=𝑰−1 𝝉Δ𝑡 -> multiply by inverse body in orientation

	//position += velocity * mass;
	//orientation = orientation * glm::quat(velocity);

	IBody = glm::mat3();

	IBody = glm::mat3(
		1.0f/12.0f * (mass * glm::vec3((scale.y * scale.y) + (scale.z * scale.z),0,0)),
		1.0f/12.0f * (mass * glm::vec3(0,(scale.x * scale.x) + (scale.z * scale.z),0)),
		1.0f/12.0f * (mass * glm::vec3(0,0,(scale.x * scale.x) + (scale.y * scale.y)))); 

	invIBody = glm::inverse(IBody);


	position += (linearMomentum * inverseMass) * dt; 

	//angularVelocity = invIBody * angluarMomentum 
	//glm::toMat3(glm::quat(angularMomentum * dt));
	//orientation = glm::normalize(glm::quat(angularVelocity) * orientation);

	orientation *= glm::normalize(glm::quat(invIBody * glm::toMat3(glm::quat(angularMomentum * dt))));

}

glm::mat4 RigidBody::getTransformationMatrix()
{
	glm::mat4 translationM = glm::translate(position);
	glm::mat4 rotationM = glm::toMat4(orientation);
	glm::mat4 scaleM = glm::scale(scale);

	return translationM * rotationM * scaleM;
}