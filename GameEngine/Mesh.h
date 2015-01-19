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

struct Texture 
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
	vector<Texture> textures;
	
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	void setupMesh();
	void render(GLuint shaderProgram);
private:

	GLuint VAO, VBO, EBO;

};

