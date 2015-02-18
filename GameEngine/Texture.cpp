#include "Texture.h"


Texture::Texture(GLenum textureTarget, const std::string &textureName)
{
	this->textureTarget = textureTarget;
	this->filename = textureName;
}

Texture::Texture(GLenum textureTarget, const std::string &textureName, const std::string &normal, GLuint gSampler, GLuint nSampler)
{
	this->textureTarget = textureTarget;
	this->filename = textureName;
	this->normalName = normal;
	this->gSampler = gSampler;
	this->nSampler = nSampler;

	//LoadNormalTexture();
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

void Texture::UnBind()
{
	glDisable(GL_TEXTURE_2D);
	glBindTexture(textureTarget, 0);
}

//bool Texture::LoadNormalTexture()
//{
//	int width,height;
//
//	glGenTextures(2, textures);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(textureTarget, textures[0]);
//	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
//	glTexImage2D(textureTarget, 0 , GL_RGBA, width,height,0,GL_RGBA, GL_UNSIGNED_BYTE,image);
//	SOIL_free_image_data(image);
//	glUniform1i(gSampler,0);
//
//	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(textureTarget, textures[1]);
//	image = SOIL_load_image(normalName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
//	glTexImage2D(textureTarget, 0 , GL_RGBA, width,height,0,GL_RGBA, GL_UNSIGNED_BYTE,image);
//	SOIL_free_image_data(image);
//	glUniform1i(nSampler,1);
//
//	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
//
//	//glBindTexture(textureTarget, 0);
//
//	return true;
//}

