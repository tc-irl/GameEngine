#pragma once
#include <glm/glm.hpp>
#include <gl/glew.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>


class Lighting
{
public:
	Lighting(GLuint shaderID);
	Lighting(GLuint shaderID, glm::vec3 position, glm::quat orientation);
	~Lighting(void);

	glm::vec3 position;
	glm::quat orientation;

	glm::vec3 direction; 
	GLuint shaderID;

public:
	//void init(GLuint shaderID);
	void update(GLuint modelLoc, float deltaTime);

	void setToonLight(glm::vec3 direction);
	void setDirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity);
	void setAmbientLight(glm::vec3 color, float intensity);
	void setSpecularLight(glm::vec3 color, float intensity, float shininess);
	glm::mat4 getTransformationMatrix();
	
	//void update();
	/*
	struct BaseLight

	{
		glm::vec3 color;
		float AmbientIntensity;
		float DiffuseIntensity;

		BaseLight()
		{
			Color = Vector3f(0.0f, 0.0f, 0.0f);
			AmbientIntensity = 0.0f;
			DiffuseIntensity = 0.0f;
		}
	};

	struct DirectionalLight : public BaseLight
	{        
		Vector3f Direction;

		DirectionalLight()
		{
			Direction = Vector3f(0.0f, 0.0f, 0.0f);
		}
	};
	*/

};

