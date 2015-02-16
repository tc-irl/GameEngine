#pragma once
#include "Shader.h";
#include "MeshLoader.h"

class Line
{
public:
	Line(GLuint shaderID);
	~Line(void);
	
	void GenerateBuffer(std::vector<glm::vec3> points);
	
	void SetColor(glm::vec4 color);
	void SetFromTo(glm::vec3 from, glm::vec3 to);

	glm::mat4 GetTransformationMatrix();
	void Update(glm::mat4 view, glm::mat4 proj, float deltaTime);
	void DrawLine();


public:

	std::vector<glm::vec3> points;
	std::vector<glm::vec4> color;

	GLuint vao, vbo;
	GLuint vSize, cSize;
	GLuint modelLoc, viewLoc, projLoc;

	GLuint shaderID;
};

