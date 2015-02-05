#include "Lighting.h"


Lighting::Lighting(GLuint shaderID)
{
	position = glm::vec3(0,0,-1);
	orientation = glm::quat(glm::vec3(0,0,0));
	this->shaderID = shaderID;

	diffuseIntensity = 0.6f;
	ambientIntensity = 0.2f;
	specularIntensity = 0.8f;
	specularShininess = 64.0f;

	diffuseColor = glm::vec3(1,1,1);
	ambientColor = glm::vec3(1,1,1);
	specularColor = glm::vec3(1,1,1);

	roughness = 1.0f;
	SetAttributesAndUniforms();
}

void Lighting::SetToonLight()
{
	glUniform3f(lightDirLoc, position.x,position.y,position.z);
}


glm::mat4 Lighting::GetTransformationMatrix()
{
	glm::mat4 translationM = glm::translate(position);
	glm::mat4 rotationM = glm::toMat4(orientation);

	return translationM * rotationM;
}

void Lighting::Update(glm::mat4 view, glm::mat4 proj, float deltaTime)
{
	glm::mat4 model;

	model = GetTransformationMatrix();

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}

void Lighting::SetDirectionalLight()
{
	glUniform3f(diffuseColorID, diffuseColor.r, diffuseColor.g, diffuseColor.b);
	glUniform1f(diffuseIntensityID, diffuseIntensity);
	glUniform3f(diffuseDirectionID, position.x, position.y, position.z);
}

void Lighting::SetAmbientLight()
{
	glUniform3f(ambientColorID, ambientColor.r, ambientColor.g, ambientColor.b);
	glUniform1f(ambientIntensityID, ambientIntensity);
}

void Lighting::SetSpecularLight()
{
	glUniform3f(specularColorID, specularColor.r, specularColor.g, specularColor.b);
	glUniform1f(specularIntensityID, specularIntensity);
	glUniform1f(specularShininessID, specularShininess);
	glUniform1f(roughnessID, roughness);
}

void Lighting::SetAttributesAndUniforms()
{
	modelLoc = glGetUniformLocation(shaderID, "model");
	viewLoc = glGetUniformLocation(shaderID, "view");
	projLoc = glGetUniformLocation(shaderID, "projection");
	vEye = glGetUniformLocation(shaderID, "vEye");
	diffuseColorID = glGetUniformLocation(shaderID, "diffuseColor");
	diffuseIntensityID = glGetUniformLocation(shaderID, "diffuseIntensity");
	diffuseDirectionID = glGetUniformLocation(shaderID, "vLightDir");
	ambientColorID = glGetUniformLocation(shaderID, "ambientColor");
	ambientIntensityID = glGetUniformLocation(shaderID, "ambientIntensity");
	lightDirLoc = glGetUniformLocation(shaderID, "vLightDir");
	specularColorID = glGetUniformLocation(shaderID, "specularColor");
	specularIntensityID = glGetUniformLocation(shaderID, "specularIntensity");
	specularShininessID = glGetUniformLocation(shaderID, "specularShininess");
	roughnessID = glGetUniformLocation(shaderID, "roughness");
}

void Lighting::UpdateLights()
{
	SetShader(possibleShaders[shaderType]);
	UseProgram();
}

void Lighting::SetShader(GLuint shaderID)
{
	this->shaderID = shaderID; 

	SetAttributesAndUniforms();
}

void Lighting::SetEyeDir()
{
	glUniform3f(vEye, eye.x, eye.y, eye.z);
}
