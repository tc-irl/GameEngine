#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessary OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <cimport.h>

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Textures
{
	GLuint id;
	string type;
	aiString path;
}; 

class Mesh
{
public:
public:

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Textures> textures;
	
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Textures> textures);
	void setupMesh();
	void render(GLuint shaderProgram);
	void Update(GLuint modelLoc, glm::mat4 modelTransform);

private:

	GLuint VAO, VBO, EBO;

};

