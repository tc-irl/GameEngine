#pragma once

#include <gl/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
class Shader
{
public:
	Shader(void);
	~Shader(void);

	bool LoadFile(const std::string& fileName, std::string& outShader);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

	void DeleteShader();

	void CreateProgram();
	bool LinkProgram();
	void UseProgram();
	void DeleteProgram();
	void ValidateProgram();

	GLuint GetProgramID();
	GLuint GetShaderID();

private:
	GLint success;
	GLuint numVertices;
	GLchar errorLog[1024], infoLog[1024];

	GLuint shaderProgramID, shaderObj;
};

