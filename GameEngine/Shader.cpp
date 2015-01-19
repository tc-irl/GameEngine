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
