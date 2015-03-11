#pragma once

#include <scene.h> // Assimp file
#include <mesh.h> // Assimp file
#include <Importer.hpp>
#include <postprocess.h>
#include <vector>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


struct Tex
{
	GLuint id; 
	string type;
	aiString path;
};

class Mesh
{
public:

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Tex> textures;

	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Tex> textures);
	void Draw(GLuint shaderID);

private:
	GLuint VAO, VBO, EBO;

	void setupMesh();
};
