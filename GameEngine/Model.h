#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
// GL Includes
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL.h>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include "Mesh.h"

using namespace std;

class Model
{
public:
	Model(GLchar *path,GLuint shaderProgramID);
	~Model(void);

	void render();
private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	
	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint TextureFromFile(const char* path, string directory);
	GLuint shaderProgramID; 
};

