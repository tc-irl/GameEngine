#include "RigidBody.h"



RigidBody::RigidBody()
{
	position = glm::vec3(0,0,0);
	orientation = glm::quat(glm::vec3(0,0,0));
	scale = glm::vec3(1,1,1);

	gravity = false;

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
	// lab2 - if(forceEnabled) {}
	this->force = force;
	torque = glm::cross(point - centreOfMass, this->force);
}

void RigidBody::update(float dt)
{
	// http://stackoverflow.com/questions/12758143/3d-rigid-body-physics-opengl -> Helped with Inertial tensor, could have implement IBody the way done here
	// https://www.scss.tcd.ie/John.Dingliana/cs7057/cs7057-1415-03a-RigidBodyMotion.pdf -> Helped with most of the requirements. 

	limitBoundary();

	inverseMass = 1.0f / mass;

	linearMomentum += force * dt; // 𝑭Δ𝑡/𝑚  -> multiply be inverse mass in position
	angularMomentum += torque * dt;// Δ𝝎=𝑰−1 𝝉Δ𝑡 -> multiply by inverse body in orientation

	IBody = glm::mat3();

	IBody = glm::mat3(
		1.0f/12.0f * (mass * glm::vec3((scale.y * scale.y) + (scale.z * scale.z),0,0)),
		1.0f/12.0f * (mass * glm::vec3(0,(scale.x * scale.x) + (scale.z * scale.z),0)),
		1.0f/12.0f * (mass * glm::vec3(0,0,(scale.x * scale.x) + (scale.y * scale.y)))); 

	invIBody = glm::inverse(IBody);

	position += (linearMomentum * inverseMass) * dt; 
	orientation *= glm::normalize(glm::quat(invIBody * glm::toMat3(glm::quat(angularMomentum * dt))));
}

glm::mat4 RigidBody::getTransformationMatrix()
{
	glm::mat4 translationM = glm::translate(position);
	glm::mat4 rotationM = glm::toMat4(orientation);
	rotationM = glm::inverse(rotationM);
	glm::mat4 scaleM = glm::scale(scale);

	return translationM * rotationM * scaleM;
}

void RigidBody::transVertices(glm::quat orientation,glm::vec3 position)
{
	transformedPoints.clear();
	transformedVertices.clear();

	for(int i = 0; i < vertices.size(); i++)
	{
		transformedVertices.push_back(vertices[i] * orientation * scale + position);
	}

	for(int i = 0; i < indices.size(); i++)
	{
		transformedPoints.push_back(indices[i] * orientation * scale + position);
	}
}

float RigidBody::calculateDistanceToCOM()
{
	glm::vec3 point = transformedPoints.at(0);
	
	distanceToCOM = glm::length(centreOfMass - point);

	return distanceToCOM;
}

glm::vec3 RigidBody::getCentreOfMass()
{
	GLuint numIndices = vertices.size();
	glm::vec3 *average = new glm::vec3[numIndices];

	float *area = new float[numIndices];
	glm::dvec3 sumOfAR;
	glm::dvec3 sumOfA;

	for(int i = 0; i < numIndices; i+=3)
	{
		glm::vec3 a,b,c; 

		a = transformedVertices[i];
		b = transformedVertices[i+1];
		c = transformedVertices[i+2];

		average[i] = (a + b + c) / 3.0f;
		area[i] = glm::length(glm::cross(b - a, c -a));

		sumOfA += area[i];

		sumOfAR += area[i] * average[i];

	}

	centreOfMass = sumOfAR / sumOfA;

	delete [] area;
	delete [] average;
	return centreOfMass;
}

void RigidBody::setVertices(std::vector<glm::vec3> vertices, std::vector<glm::vec3> indices)
{
	this->vertices = vertices;
	this->indices = indices;
}

void RigidBody::updateModel(GLuint modelLoc)
{
	glm::mat4 model = getTransformationMatrix();

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}

void RigidBody::limitBoundary()
{
	if (position.x < -6)
	{
		linearMomentum.x *= -1.0f;
	}
	if (position.x > 6)
	{
		linearMomentum.x *= -1.0f;
	}

	if (position.y < -6)
	{
		linearMomentum.y *= -1.0f;
	}
	if (position.y > 6)
	{
		linearMomentum.y *= -1.0f;
	}

	if (position.z < -6)
	{
		linearMomentum.z *= -1.0f;
	}
	if (position.z > 6)
	{
		linearMomentum.z *= -1.0f;
	}
}

void RigidBody::calcMinMaxExtents()
{
	// Used: http://en.wikibooks.org/wiki/OpenGL_Programming/Bounding_box to help understand how to calculate min/max extents.

	minExtents = glm::vec3(0,0,0);
	maxExtents = glm::vec3(0,0,0);

	for(int j = 0; j < transformedPoints.size(); j++)
	{
		if(glm::length(transformedPoints[j].x - centreOfMass.x) < minExtents.x) minExtents.x = glm::length(transformedPoints[j].x - centreOfMass.x);
		if(glm::length(transformedPoints[j].x - centreOfMass.x) > maxExtents.x) maxExtents.x = glm::length(transformedPoints[j].x - centreOfMass.x);
		if(glm::length(transformedPoints[j].y - centreOfMass.y) < minExtents.y) minExtents.y = glm::length(transformedPoints[j].y - centreOfMass.y);
		if(glm::length(transformedPoints[j].y - centreOfMass.y) > maxExtents.y) maxExtents.y = glm::length(transformedPoints[j].y - centreOfMass.y);
		if(glm::length(transformedPoints[j].z - centreOfMass.z) < minExtents.z) minExtents.z = glm::length(transformedPoints[j].z - centreOfMass.z);
		if(glm::length(transformedPoints[j].z - centreOfMass.z) > maxExtents.z) maxExtents.z = glm::length(transformedPoints[j].z - centreOfMass.z);
	}
}

void RigidBody::Rotate360(float dt)
{
	orientation *= glm::quat(glm::vec3(0,0,dt/2));
}

glm::vec3 RigidBody::GetFarthestPointInDirection(glm::vec3 direction)
{
	glm::vec3 farthestPoint;
	float farthest;

	farthest = 0;

	for(int i = 0; i <transformedPoints.size();i++)
	{
		float temp = glm::dot(direction,transformedPoints[i]);
		
		if(temp > farthest)
		{
			farthest = temp;
			farthestPoint = transformedPoints[i];
		}
	}

	return farthestPoint;
}

