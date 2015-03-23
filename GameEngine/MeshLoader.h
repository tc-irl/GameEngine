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

class MeshLoader
{
public:
	static enum BUFFERS { VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, NORMAL_MAP_BUFFER}; 

	typedef enum RefractionIndex{AIRTOWATER, AIRTOICE, AIRTOGLASS, AIRTODIAMOND};
	RefractionIndex refractiveIndex, refractiveIndexR,refractiveIndexG,refractiveIndexB;

	MeshLoader(GLuint initialShaderID, const char* filename);
	~MeshLoader(void);

	void LoadMesh(const char* filename);

	void SetTexture(const char* filename);
	std::string GetTexture() {return textureName;}
	void SetCubeMapTexture(const char* directory);
	void SetColor(glm::vec3 color);
	void SetShader(GLuint shaderID) {this->shaderID = shaderID; SetAttributesAndUniforms();}
	GLuint GetShader(){return shaderID;}
	void IsTextureActive(bool useTexture) {this->useTexture = useTexture;}
	void IsNormalActive(bool useNormalTexture) {this->useNormalTexture = useNormalTexture;}
	void IsHeightMapActive(bool useHeightTexture) {this->useHeightTexture = useHeightTexture;}
	void IsSkyboxActive(bool drawSkyBox) {this->drawSkyBox = drawSkyBox;}

	void SetPos(glm::vec3 position){this->position = position;}
	glm::vec3 GetPos(){return position;}

	void SetOrientation(glm::quat orientation) {this->orientation = orientation;}
	glm::quat GetOrientation() {return orientation;}

	void SetScale(glm::vec3 scale) {this->scale = scale;}
	glm::vec3 GetScale() {return scale;}

	void SetShaderType(Shader::ShaderType shaderType) {this->shaderType = shaderType;}
	Shader::ShaderType GetShaderType() {return shaderType;}

	void SetCurrentRatio(float ratio);
	void SetCurrentRatio(float ratioR, float ratioG,float ratioB);
	std::vector<glm::vec3> GetVertices() { return vertices; }
	std::vector<glm::vec3> GetPoints() { return points; }

	void SetPossibleShaders(std::map<Shader::ShaderType, GLuint> possibleShaders) { this->possibleShaders = possibleShaders;} 
	void SetRefractionTypes(std::map<RefractionIndex, float> refractions) { this->refractions = refractions;} 
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
	void Render();
	void RenderPoly();
	void RenderCubeMap();
	void UpdateRefractionIndex();

	void DrawLine();
	void DefineLine(glm::vec3 point1, glm::vec3 point2);
	void UpdateRefractionIndexRGB();
	void SetNormalTexture(const char* filename);
	void SetNormalTexture(const char* texture, const char* normal);
	void EnableDispersion();
	void BindTexture();
	void UnBindTexture();
	void BindNormal();
	void UnBindNormal();
	void SetHeightTexture(const char* textureName);
public:
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

	glm::vec3 color;
	glm::vec3 previousColor;
	glm::vec4 matColor;

	float reflectionFactor;

	Shader::ShaderType shaderType;

	std::map <Shader::ShaderType, GLuint> possibleShaders;
	std::map <RefractionIndex, float> refractions;
	bool useNormalTexture;
	bool useTexture;
	bool drawSkyBox;
	bool dispersion;

	glm::vec3 cameraPos;


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> temp;

	GLuint lineVao,lineVbo;

	float ratioR;
	float ratioG;
	float ratioB;

private:
	GLuint VAO, VBO[4];

	std::vector<glm::vec4> colors;

	GLuint vSize, cSize, numElements;

	Texture *texture, *normalTexture, *heightTexture;
	CubeMapTexture *cubeTexture;
	GLuint gSampler,cSampler, vColor, vEye, normalMap;
	GLuint shaderID, drawSky, camPos;
	GLuint reflectFactor, materialColor;
	GLuint ratioID;

	std::string filename;
	GLuint modelLoc, viewLoc, projLoc;
	std::string textureName;
	GLuint ratioRID;
	GLuint ratioGID;
	GLuint ratioBID;
	GLuint nSampler, hSampler;
	std::string normalTextureName;
	GLuint disperionID;
	const char* normalName;
	bool useHeightTexture;
};

