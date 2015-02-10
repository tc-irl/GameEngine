#pragma once
#include <string>
#include <gl/glew.h>
#include <SOIL.h>
#include <vector>
#include <iostream>

class CubeMapTexture
{
public:
	CubeMapTexture(GLenum textureTarget, const char* directory);
	~CubeMapTexture(void);

	bool LoadCubeMap();
	void Bind(GLenum textureUnit);
	void UnBind();

public:
	GLenum textureTarget;
	GLuint textureObj;
	const char *directory;
};

