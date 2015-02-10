#include "CubeMapTexture.h"


CubeMapTexture::CubeMapTexture(GLenum textureTarget,const char* directory)
{
	this->textureTarget = textureTarget;
	this->directory = directory;
}

CubeMapTexture::~CubeMapTexture(void)
{
}

bool CubeMapTexture::LoadCubeMap()
{
	int width,height;

	glGenTextures(1, &textureObj);
	glBindTexture(textureTarget, textureObj);

	std::string right = std::string(directory) + "right.jpg";
	std::string left = std::string(directory) + "left.jpg";
	std::string up = std::string(directory) + "top.jpg";
	std::string down = std::string(directory) + "bottom.jpg";
	std::string back = std::string(directory) + "back.jpg";
	std::string front = std::string(directory) + "front.jpg";

	textureObj = SOIL_load_OGL_cubemap(right.c_str(),left.c_str(), up.c_str(), down.c_str(), back.c_str(), front.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	//glTexImage2D(textureTarget, 0 , GL_RGBA, width,height,0,GL_RGBA, GL_UNSIGNED_BYTE,image);

	glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);   
	glBindTexture(textureTarget, 0);

	return true;
}

void CubeMapTexture::Bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(textureTarget, textureObj);
}

void CubeMapTexture::UnBind()
{
	glBindTexture(textureTarget, 0);
}
