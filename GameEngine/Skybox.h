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
#include "Shader.h"
#include "CubeMapTexture.h"
#include <map>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class Skybox
{
public:
	static enum BUFFERS { VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, NORMAL_MAP_BUFFER}; 

	Skybox(GLuint initialShaderID, const char* filename);
	~Skybox(void);

	void LoadMesh(const char* filename);

	void SetCubeMapTexture(const char* directory);
	void SetShader(GLuint shaderID) {this->shaderID = shaderID; SetAttributesAndUniforms();}
	GLuint GetShader(){return shaderID;}

	void SetPos(glm::vec3 position){this->position = position;}
	glm::vec3 GetPos(){return position;}

	void SetOrientation(glm::quat orientation) {this->orientation = orientation;}
	glm::quat GetOrientation() {return orientation;}

	void SetScale(glm::vec3 scale) {this->scale = scale;}
	glm::vec3 GetScale() {return scale;}

	void SetShaderType(Shader::ShaderType shaderType) {this->shaderType = shaderType;}
	Shader::ShaderType GetShaderType() {return shaderType;}

	std::vector<glm::vec3> GetVertices() { return vertices; }
	std::vector<glm::vec3> GetPoints() { return points; }

	void SetPossibleShaders(std::map<Shader::ShaderType, GLuint> possibleShaders) { this->possibleShaders = possibleShaders;} 
	void SetAttributesAndUniforms();
	void SetSkyBox();
	glm::mat4 GetTransformationMatrix();

	void SetCameraPos(glm::vec3 position) { cameraPos = position; }
	void UseProgram(){ glUseProgram(shaderID); }

	void Rotate360(float dt); // for rendering
	void VerticesToPoints(std::vector<glm::vec3> vert);
	void Update(glm::mat4 view, glm::mat4 proj, float deltaTime);
	void UpdateShader();
	//void InitMaterials(cons)

	void RenderCubeMap();

	void BindTexture();
	void UnBindTexture();

public:
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

	Shader::ShaderType shaderType;

	std::map <Shader::ShaderType, GLuint> possibleShaders;

	glm::vec3 cameraPos;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> temp;

	float reflectionFactor;
	glm::vec4 matColor;

private:
	GLuint VAO, VBO[4];

	GLuint vSize, numElements;

	CubeMapTexture *cubeTexture;
	GLuint gSampler,cSampler, vColor, vEye, normalMap;
	GLuint shaderID, drawSky, camPos;
	GLuint reflectFactor, materialColor;
	GLuint ratioID;

	std::string filename;
	GLuint modelLoc, viewLoc, projLoc;
	std::string textureName;
};

