#pragma once

#include <gl/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class Shader
{
public:

	//typedef enum ShaderType {NONE, BASIC, TEXTURED, PHONG, TOON, DIFFUSE, PHONG_TEXTURED, TOON_TEXTURED, OREN_NAYAR, OREN_TEXTURED}; ////// --> Lab 1
	//typedef enum ShaderType {SKYBOX, REFLECTION, REFRACTION, REFRACTION_DISP, COMBINED,COMBINED_DISP, TEXTURED, NORMAL_MAP,FRESNEL, NORMAL_FRESNEL,SKIN};
	//typedef enum ShaderType {MODEL, PARTICLE}; // Physics Lab 1
	typedef enum ShaderType {NONE, BASIC, TEXTURED, NORMAL_MAP,SKYBOX, RELIEF_MAP, RELIEF_MAP_SHADOW, POM, POM_SHADOW};

	ShaderType shaderType;

	Shader();
	~Shader(void);

	bool LoadFile(const std::string& fileName, std::string& outShader);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

	void DeleteShader();

	void initShader(ShaderType shaderType);
	void CreateProgram();
	bool LinkProgram();
	void UseProgram();
	void DeleteProgram();
	void ValidateProgram();

	GLuint GetProgramID();
	GLuint GetShaderID();

	void setMVPUniforms();
	GLuint getUniform(std::string name);
	GLuint getAttribute(std::string name);

	GLuint modelLoc, viewLoc, projLoc;

	std::string vs, ps;
	std::string filename;

private:
	GLint success;
	GLuint numVertices;
	GLchar errorLog[1024], infoLog[1024];

	GLuint shaderProgramID, shaderObj;

};

