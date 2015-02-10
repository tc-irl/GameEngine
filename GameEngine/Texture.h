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
	Texture(GLenum textureTarget, const std::string &filename);

	bool Load();
	void Bind(GLenum textureUnit);
	void UnBind();
private:
	std::string filename;
	GLenum textureTarget;
	GLuint textureObj;
	//Magick::Image *image;
	//Magick::Blob blob;

};

#endif // !TEXTURE_H
