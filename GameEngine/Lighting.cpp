#include "Lighting.h"


Lighting::Lighting(GLuint shaderID)
{
	position = glm::vec3(0,0,0);
	orientation = glm::quat(glm::vec3(0,0,0));
	this->shaderID = shaderID;
}

Lighting::Lighting(GLuint shaderID, glm::vec3 position, glm::quat orientation)
{
	this->position = position;
	this->orientation = orientation;
	this->shaderID = shaderID;

	direction = glm::vec3 (0, 0,-1);
}

void Lighting::setToonLight(glm::vec3 direction)
{
	this->direction = direction;

	GLuint lightDirLoc = glGetUniformLocation(shaderID, "vLightDir");

	glUniform3f(lightDirLoc, direction.x,direction.y,direction.z);
}



glm::mat4 Lighting::getTransformationMatrix()
{
	glm::mat4 translationM = glm::translate(direction);
	glm::mat4 rotationM = glm::toMat4(orientation);

	return translationM * rotationM;
}

void Lighting::update(GLuint modelLoc, float deltaTime)
{
	glm::mat4 model;

	model = getTransformationMatrix();

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}

void Lighting::setDirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity)
{
	this->direction = direction;

	GLuint diffuseColorID = glGetUniformLocation(shaderID, "diffuseColor");
	GLuint diffuseIntensityID = glGetUniformLocation(shaderID, "diffuseIntensity");
	GLuint diffuseDirectionID = glGetUniformLocation(shaderID, "vLightDir");

	glUseProgram(shaderID);
	glUniform3f(diffuseColorID, color.r, color.g, color.b);
	glUniform1f(diffuseIntensityID, intensity);
	glUniform3f(diffuseDirectionID, direction.x, direction.y, direction.z);
}

void Lighting::setAmbientLight(glm::vec3 color, float intensity)
{
	GLuint ambientColorID = glGetUniformLocation(shaderID, "ambientColor");
	GLuint ambientIntensityID = glGetUniformLocation(shaderID, "ambientIntensity");

	glUseProgram(shaderID);
	glUniform3f(ambientColorID, color.r, color.g, color.b);
	glUniform1f(ambientIntensityID, intensity);
}

void Lighting::setSpecularLight(glm::vec3 color, float intensity, float shininess)
{

	GLuint specularColorID = glGetUniformLocation(shaderID, "specularColor");
	GLuint specularIntensityID = glGetUniformLocation(shaderID, "specularIntensity");
	GLuint specularShininessID = glGetUniformLocation(shaderID, "specularShininess");

	glUseProgram(shaderID);
	glUniform3f(specularColorID, color.r, color.g, color.b);
	glUniform1f(specularIntensityID, intensity);
	glUniform1f(specularShininessID, shininess);
}
