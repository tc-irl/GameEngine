#include "Shader.h"

Shader::Shader()
{
	success = 0;
}

bool Shader::LoadFile(const std::string& fileName, std::string& outShader)
{
	std::ifstream file(fileName);

	if(!file.is_open())
	{
		std::cout << "Error Loading file: " << fileName << " - impossible to open file" << std::endl;
		return false;
	}

	if(file.fail())
	{
		std::cout << "Error Loading file: " << fileName << std::endl;
		return false;
	}
	else
	{
		std::cout << "Success loading file: : " << fileName << std::endl;
	}

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	outShader = stream.str();

	return true;
}

void Shader::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	shaderObj = glCreateShader(ShaderType);

	if (shaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}
	else
	{
		std::cout << "Shader type created successfully: " << ShaderType << std::endl;
	}

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(shaderObj, 1, (const GLchar**)&pShaderText, NULL);
	// compile the shader and check for errors
	glCompileShader(shaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	
	if (!success) 
	{
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, infoLog);

		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, shaderObj);
}

void Shader::CreateProgram()
{
	shaderProgramID = glCreateProgram();

	if (shaderProgramID == 0) 
	{
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}
	else
	{
		std::cout << "Program created." << std::endl;
	}
}

bool Shader::LinkProgram()
{
	glLinkProgram(shaderProgramID);

	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);

	if (success == 0) 
	{
		glGetProgramInfoLog(shaderProgramID, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}
	else
	{
		std::cout << "Program Linked" << std::endl;
	}

	return true;
}

void Shader::UseProgram()
{
	glUseProgram(shaderProgramID);
}


void Shader::ValidateProgram()
{
	glValidateProgram(shaderProgramID);

	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &success);

	if (!success) 
	{
		glGetProgramInfoLog(shaderProgramID, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}
	else
	{
		std::cout << "Valid Program! " << std::endl;
	}
}

GLuint Shader::GetProgramID()
{
	return shaderProgramID;
}

GLuint Shader::GetShaderID()
{
	return shaderObj;
}

Shader::~Shader(void)
{
	glDeleteShader(shaderObj);
	glDeleteProgram(shaderProgramID);
}

void Shader::setMVPUniforms()
{
	modelLoc = glGetUniformLocation(shaderProgramID, "model");
	viewLoc = glGetUniformLocation(shaderProgramID, "view");
	projLoc = glGetUniformLocation(shaderProgramID, "projection");
}

GLuint Shader::getUniform(std::string name)
{
	return glGetUniformLocation(shaderProgramID, name.c_str());
}

GLuint Shader::getAttribute(std::string name)
{
	return glGetAttribLocation(shaderProgramID,name.c_str());
}

void Shader::initShader(ShaderType shaderType)
{
	this->shaderType = shaderType;

	CreateProgram();

	//if(shaderType == BASIC){ filename = "../Resources/Shaders/objNoTexture";}
	//else if(shaderType == TEXTURED){filename = "../Resources/Shaders/objWithTexture";}
	//else if(shaderType == PHONG){filename = "../Resources/Shaders/phong";}
	//else if(shaderType == TOON){filename = "../Resources/Shaders/toon";}
	//else if(shaderType == DIFFUSE){filename = "../Resources/Shaders/diff";}
	//else if(shaderType == PHONG_TEXTURED) {filename = "../Resources/Shaders/phongTextured";}
	//else if(shaderType == TOON_TEXTURED) {filename = "../Resources/Shaders/toonTextured";}
	//else if(shaderType == OREN_NAYAR) {filename = "../Resources/Shaders/orennayar";}
	//else if(shaderType == OREN_TEXTURED) {filename = "../Resources/Shaders/orenTextured";}
	//else if(shaderType == SKYBOX) {filename = "../Resources/Shaders/skybox";}

	//if(shaderType == SKYBOX) {filename = "../Resources/Shaders/cube";}
	//else if(shaderType == REFLECTION) {filename = "../Resources/Shaders/reflect";}
	//else if(shaderType == REFRACTION) {filename = "../Resources/Shaders/refract";}
	//else if(shaderType == REFRACTION_DISP) {filename = "../Resources/Shaders/refractDispersion";}
	//else if(shaderType == COMBINED) {filename = "../Resources/Shaders/combined";}
	//else if(shaderType == COMBINED_DISP) {filename = "../Resources/Shaders/combinedDispersion";}
	//else if(shaderType == TEXTURED){filename = "../Resources/Shaders/objWithTexture";}
	//else if(shaderType == NORMAL_MAP){filename = "../Resources/Shaders/normalMap";}
	//else if(shaderType == FRESNEL){filename = "../Resources/Shaders/fresnel";}
	//else if(shaderType == NORMAL_FRESNEL){filename = "../Resources/Shaders/normalMapFresnel";}
	//else if(shaderType == SKIN{filename = "../Resources/Shaders/skinShader";}
	//if(shaderType == MODEL){ filename = "../Resources/Shaders/objWithTexture";}
	//else if(shaderType == PARTICLE){filename = "../Resources/Shaders/Particle";}

//	if(shaderType == SKIN){ filename = "../Resources/Shaders/skinShader";}

	//if(shaderType == BASIC){ filename = "../Resources/Shaders/cloth";}
	//else if(shaderType == TEXTURED){filename = "../Resources/Shaders/objWithTexture";}

	if(shaderType == BASIC){ filename = "../Resources/Shaders/cloth";}
	else if(shaderType == TEXTURED){filename = "../Resources/Shaders/objWithTexture";}
	else if(shaderType == NORMAL_MAP){filename = "../Resources/Shaders/normalMap";}
	else if(shaderType == RELIEF_MAP){filename = "../Resources/Shaders/reliefMap";}
	else if(shaderType == RELIEF_MAP_SHADOW){filename = "../Resources/Shaders/reliefMapShadow";}
	else if(shaderType == POM){filename = "../Resources/Shaders/pom";}
	else if(shaderType == POM_SHADOW){filename = "../Resources/Shaders/pomShadow";}
	else if(shaderType == SKYBOX){filename = "../Resources/Shaders/cube";}

	LoadFile(filename + ".vs",vs);
	AddShader(shaderProgramID, vs.c_str(), GL_VERTEX_SHADER);

	LoadFile(filename + ".ps",ps);
	AddShader(shaderProgramID, ps.c_str(), GL_FRAGMENT_SHADER);

	LinkProgram();
	//ValidateProgram();
	UseProgram();

	setMVPUniforms();
}
