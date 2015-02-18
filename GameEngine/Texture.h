#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <gl/glew.h>
#include <SOIL.h>

#include <iostream>

class Texture
{
public:
	Texture(GLenum textureTarget, const std::string &textureName);
	Texture(GLenum textureTarget, const std::string &textureName,const std::string &normal, GLuint gSampler, GLuint nSampler);
	bool Load();
	void Bind(GLenum textureUnit);
	void Bind(GLenum textureUnit, GLuint loc);
	void UnBind();
	bool LoadNormalTexture();
private:
	std::string filename;
	std::string normalName;
	GLenum textureTarget;
	GLuint textureObj;
	GLuint textures[2];
	GLuint nSampler;
	GLuint gSampler;
	//Magick::Image *image;
	//Magick::Blob blob;

};

#endif // !TEXTURE_H
