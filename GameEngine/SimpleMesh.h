#pragma once

#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <scene.h> // Assimp file
#include <mesh.h> // Assimp file
#include <Importer.hpp>
#include <postprocess.h>
#include <vector>
#include "Texture.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class SimpleMesh
{
public:
	static enum BUFFERS { VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER }; 
	
	SimpleMesh(GLuint shaderID, const char *filename, glm::vec3 color, bool useTexture);
	SimpleMesh(GLuint shaderID, const char *filename, bool textureIncluded);
	SimpleMesh(GLuint shaderID, const char *filename, glm::vec3 color, bool useTexture, glm::vec3 position, glm::quat orientation, glm::vec3 scale);
	SimpleMesh(GLuint shaderID, const char *filename, bool useTexture, glm::vec3 position, glm::quat orientation, glm::vec3 scale);
	~SimpleMesh(void);
	
	GLuint VAO;
	GLuint VBO[4];

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> points;
	std::vector<glm::vec4> colors;


	glm::vec3 color;

	GLuint numElements;

	Texture *texture;
	GLuint gSampler, vColor;
	GLuint shaderID;

	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

	bool useTexture;
	float distanceToCOM; 
	glm::vec3 centroid;
	GLuint vSize, cSize;

	void initTexture(const char* filename); // Just using our own textures, rather than loading from file itself.
	void updateColor(glm::vec3 color);
	void LoadMesh(const char *filename);

	void verticesToPoints(std::vector<glm::vec3> vert);
	void update(GLuint modelLoc, float deltaTime);
	void update(glm::mat4 model, GLuint modelLoc, float deltaTime);
	void render();
	void renderPoly();
	

	GLuint numIndices;

	glm::mat4 getTransformationMatrix();
};

