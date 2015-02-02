#include "Texture.h"


Texture::Texture(GLenum textureTarget, const std::string &filename)
{
	this->textureTarget = textureTarget;
	this->filename = filename;
}

bool Texture::Load()
{
	int width,height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

	glGenTextures(1, &textureObj);
	glBindTexture(textureTarget, textureObj);
	glTexImage2D(textureTarget, 0 , GL_RGBA, width,height,0,GL_RGBA, GL_UNSIGNED_BYTE,image);

	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
	glBindTexture(textureTarget, 0);

	return true;
}

void Texture::Bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(textureTarget, textureObj);
}

