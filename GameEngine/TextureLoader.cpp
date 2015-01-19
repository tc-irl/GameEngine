/** @file shaderLoader.cpp
*  @brief useful functions for shader management
*
*	@author Bartlomiej Filipek 
*/

#include "TextureLoader.h"
#include <assert.h>
#include <SOIL.h>
#include <iostream>
#include <gl/glew.h>

#pragma comment(lib, "soil")

namespace textureLoader
{
    bool loadTexture(const char *filename, GLuint *outTexId)
    {
        assert(outTexId);
        *outTexId = SOIL_load_OGL_texture
        (
        filename,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        /*SOIL_FLAG_GL_MIPMAPS | */SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS
        );

		if (*outTexId == 0)
		{
			std::cout << ("Cannot load texture: %s", filename) << std::endl;
		}

        return *outTexId != 0;
    }

} // namespce textureLoader