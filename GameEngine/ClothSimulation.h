#pragma once

#include "Game.h"
#include "MeshLoader.h"
#include "Texture.h"
#include "Cloth.h"
#include "RigidBody.h"
#include <string>

using namespace std;

class ClothSimulation : public Game
{
public:
	typedef enum Mode {NONE, CLOTH};

	ClothSimulation(void);
	~ClothSimulation(void);

	void run() override;
	void init(char **argv);
	void initShaders();
	void initTweakBar();
	void initModels();
	void initTextures();
	void update();
	float RandomNumber(float Min, float Max);
	
	Cloth *cloth;
	MeshLoader *plane;
	Shader *basicShader, *textureShader;
	std::map <Shader::ShaderType, GLuint> possibleShaders;

	Mode mode;

	bool pauseScene;
	double time, dt;// Current time and elapsed time

};

