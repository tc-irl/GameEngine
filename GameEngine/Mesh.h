/**********************************************************************************************/
/* Mesh Loading Class.

Tutorial followed - http://www.nexcius.net/2014/04/13/loading-meshes-using-assimp-in-opengl/ */
/*********************************************************************************************/

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

#ifndef MESH_H
#define MESH_H

#include <iostream>
class Mesh
{

public: 
	
	// We need this struct to handle the case of multiple meshes being in loaded in at once, e.g. a complex model. 
	struct MeshEntry 
	{
		static enum BUFFERS { VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER }; 

		GLuint VAO;
		GLuint VBO[4];

		GLuint numElements;

		MeshEntry(aiMesh *mesh);
		~MeshEntry();

		void render();
	};

	std::vector<MeshEntry*> meshEntries;
public:

	Mesh(const char *filename, bool textureIncluded);
	Mesh::Mesh(const char *filename, bool useTexture, glm::vec3 position, glm::quat orientation, glm::vec3 scale);
	~Mesh(void);
	
	Texture *texture;
	GLuint gSampler;

	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

	bool rotate;

	bool useTexture;
	
	void initTexture(const std::string &filename, GLuint shaderID); // Just using our own textures, rather than loading from file itself.
	void update(GLuint modelLoc, float deltaTime);
	void render();

	glm::mat4 getTransformationMatrix();
};

#endif