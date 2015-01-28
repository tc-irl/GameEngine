#include "Texture.h"


Texture::Texture(GLenum textureTarget, const std::string &filename)
{
	this->textureTarget = textureTarget;
	this->filename = filename;
}

bool Texture::Load()
{
	try 
	{
		image.read(filename);
		image.write(&blob, "RGBA");
	}
	catch (Magick::Error& Error)
	{
		std::cout << "Error loading texture '" << filename << "': " << Error.what() << std::endl;
		return false;
	}

	glGenTextures(1, &textureObj);
	glBindTexture(textureTarget, textureObj);
	glTexImage2D(textureTarget, 0 , GL_RGBA, image.columns(),image.rows(),0,GL_RGBA, GL_UNSIGNED_BYTE,blob.data());

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

