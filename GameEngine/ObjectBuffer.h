#pragma once;
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string> 
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

#define MAX 36

class ObjectBuffer
{
public:

	glm::vec3 vertices[MAX];
	glm::vec3 colors[MAX];
	glm::vec3 transformedVertices[MAX];
	glm::vec3 centroid;
	std::vector<glm::vec3> indices;
public:
	ObjectBuffer() {};
	ObjectBuffer(GLuint numVertices);
	~ObjectBuffer() {};

	void transVertices(glm::quat orientation,glm::vec3 position);

	GLuint GenerateVBO(glm::vec3 vertices[], glm::vec3 colors[]);
	GLuint GenerateVBO(std::vector<glm::vec3> vertices, glm::vec3 colors[]);

	void LinkBufferToShader(GLuint shaderProgramID);
	GLuint GetVBO();
	GLuint GetVAO();
	void DeleteVBO();
	void DeleteVAO();
	void Update(glm::mat4 transformMatrix);
	void Draw();

	glm::vec3 getCentreOfMass();

	void verticesToPoints(std::vector<glm::vec3> & vertices);

private:
	GLuint VBO,VAO;
	//glm::mat4 colors;
	GLuint numVertices;
};

