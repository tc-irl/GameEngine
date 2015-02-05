#pragma once
#include <glm/glm.hpp>
#include <gl/glew.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <map>
#include "Shader.h"

class Lighting
{
public:
	Lighting(GLuint shaderID);
	~Lighting(void);

	glm::vec3 position;
	glm::quat orientation;
	
	glm::vec3 toonColor; 
	glm::vec3 toonLightDirection;
	// diffuse
	glm::vec3 diffuseColor;
	glm::vec3 diffuseDirection;
	float diffuseIntensity;

	// ambient
	glm::vec3 ambientColor;
	float ambientIntensity;

	//specular
	glm::vec3 specularColor;
	float specularIntensity;
	float specularShininess; 

	float roughness;
	GLuint shaderID;
	Shader::ShaderType shaderType;

	std::map<Shader::ShaderType, GLuint> possibleShaders;
	GLuint specularColorID, specularIntensityID, specularShininessID, lightDirLoc, ambientColorID, ambientIntensityID;
	GLuint diffuseColorID, diffuseIntensityID, diffuseDirectionID, roughnessID;
	GLuint modelLoc, viewLoc, projLoc, vEye;

	glm::vec3 eye;
public:
	void UseProgram(){glUseProgram(shaderID);}
	void Update(glm::mat4 view, glm::mat4 proj, float deltaTime);
	void UpdateLights();

	void SetShaderType(Shader::ShaderType shaderType) {this->shaderType = shaderType;}
	void SetShader(GLuint shaderID);
	void SetPos(glm::vec3 position){this->position = position;}
	void SetOrientation(glm::quat){this->orientation = orientation;}
	void SetEye(glm::vec3 eye) {this->eye = eye;}

	void SetEyeDir();
	void SetToonDirection(glm::vec3 direction){this->toonLightDirection = direction;}
	void SetDiffuseDirection(glm::vec3 direction){this->diffuseDirection = direction;}

	void SetToonLight();
	void SetDirectionalLight();
	void SetAmbientLight();
	void SetSpecularLight();
	glm::mat4 GetTransformationMatrix();

	void SetPossibleShaders(std::map<Shader::ShaderType, GLuint> possibleShaders) { this->possibleShaders = possibleShaders;} 
	void SetAttributesAndUniforms();
	
};

