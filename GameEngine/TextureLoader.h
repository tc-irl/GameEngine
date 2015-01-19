/** @file shaderLoader.h
*  @brief useful functions for shader management
*
*	@author Bartlomiej Filipek 
*/


#pragma once
#include <gl/glew.h>
\
namespace textureLoader
{
    bool loadTexture(const char *filename, GLuint *outTexId);
} // namespace textureLoader
