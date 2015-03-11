#pragma once

#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL.h>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "Mesh.h"
class Model
{
public:
	Model(void);
	~Model(void);

	Model(GLchar* path);
	GLint TextureFromFile(const char* path, string directory);
	
	void SetPos(glm::vec3 position){this->position = position;}
	glm::vec3 GetPos(){return position;}

	void SetOrientation(glm::quat orientation) {this->orientation = orientation;}
	glm::quat GetOrientation() {return orientation;}

	void SetScale(glm::vec3 scale) {this->scale = scale;}
	glm::vec3 GetScale() {return scale;}

	void Update(glm::mat4 view, glm::mat4 proj,  GLuint shaderID,  float deltaTime);
	void Draw(GLuint shaderID);

	glm::mat4 GetTransformationMatrix();


private:

	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

	vector<Mesh> meshes;
	string directory; 
	vector<Tex> textures_loaded;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	vector<Tex> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

};

