#pragma once

/************************************************************************/
/* The particle system used here was based on the tutorial from: 
http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/ 

The Tweak bar and how to integrate it can be found at: 

http://anttweakbar.sourceforge.net/doc/tools:anttweakbar
*/
/************************************************************************/
#ifndef LAB1_H
#define LAB1_H
#define GLFW_CDECL

#include "Game.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "TextureLoader.h"
#include <AntTweakBar.h>
#include <glm/gtx/rotate_vector.hpp>
#include <memory>
#include <algorithm>

#define MaxParticles 100000

class Lab1 : public Game
{
public:
	Lab1(void);
	~Lab1(void);

	float floorY;
	float bounceFactor;

	void run() override;
	void init();
	void initTweakBar();
	void initShaders() override;
	void initParticleSystem();
	bool initTextures();
	int findUnusedParticle();
	void sortParticles();
	void updateParticles();
	void drawParticles();
	void particleControl();
	double time, dt;// Current time and enlapsed time

private: 
	Shader *particleShader,*modelShader;
	std::string vs2,ps2;
	TwBar *bar;    
	
	GLuint VertexArrayID;
	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;
		
	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

	unsigned int gParticleTexture;

	// CPU representation of a particle
	struct Particle
	{
		glm::vec3 pos, speed;
		unsigned char r,g,b,a; // Color
		float size;
		float weight;
		float angle;
		float life; // Remaining life of the particle. if <0 : dead and unused.
		float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

		bool operator<(const Particle& that) const {
			// Sort in reverse order : far particles drawn first.
			return this->cameradistance > that.cameradistance;
		}
	};

	Particle ParticlesContainer[MaxParticles];
	int LastUsedParticle;
	int ParticlesCount;

	GLuint CameraRight_worldspace_ID;
	GLuint CameraUp_worldspace_ID;
	GLuint ViewProjMatrixID;
	GLuint TextureID;
	GLuint Texture;
	bool gravityEnabled;
	bool attractorPoint;
	bool attractorPoint2;
	bool attractorPoint3;
	bool automated;
	double rotSpeed;

	glm::vec3 attractorPos[3];
	float distance;
	glm::vec3 maindir,maindir2,maindir3;
	float spread;

	int currentAttractor;
};

#endif