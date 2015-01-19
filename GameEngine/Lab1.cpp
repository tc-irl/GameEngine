#include "Lab1.h"
/************************************************************************/
/* The particle system used here was based on the tutorial from: 
http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/ 

The Tweak bar and how to integrate it can be found at: 

http://anttweakbar.sourceforge.net/doc/tools:anttweakbar
*/
/************************************************************************/


/*
Create new ones.
This function searches linearly in ParticlesContainer, except that it starts at the last known place, so this function usually returns immediately:
*/
int Lab1::findUnusedParticle()
{
	for(int i=LastUsedParticle; i<MaxParticles; i++)
	{
		if (ParticlesContainer[i].life < 0)
		{
			LastUsedParticle = i;
			return i;
		}
	}

	for(int i=0; i<LastUsedParticle; i++)
	{
		if (ParticlesContainer[i].life < 0)
		{
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

// function to sort the particles. They are sorted in reverse order : far particles drawn first.
// The Z buffer doesn't work as you might hope for transparent polygons.
//The problem is that the Z buffer prevents OpenGL from drawing pixels that are behind things that have already been drawn. 
//Generally, that's pretty convenient, but when the thing in front is translucent, you need to see the things that are behind it.

void Lab1::sortParticles()
{
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}


Lab1::Lab1(void)
{
	particleShader = new Shader();
	modelShader = new Shader();
	window = new Window(800,600,"Lab 1: Particle Systems");
	camera = new Camera(window->GetWindow());
	LastUsedParticle = 0;
	floorY = 0;
	bounceFactor = 1.0f;
	gravityEnabled = false;
	attractorPoint = false;
	automated = false;
	attractorPos[0] = glm::vec3(-5,10,0);
	attractorPos[1] = glm::vec3(0,10,-5);
	attractorPos[2] = glm::vec3(7,9,0);
	currentAttractor = 0;
	spread = 2.5f;

	rotSpeed = 0;
}


void Lab1::init()
{
	initShaders();
	initTweakBar();
	initTextures();
	initParticleSystem();
	//initModels();
}

void Lab1::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

    bar = TwNewBar("Attributes: ");

	    // Add 'time' to 'bar': it is a read-only (RO) variable of type TW_TYPE_DOUBLE, with 1 precision digit
	TwAddVarRO(bar, "Num Particles", TW_TYPE_INT32, &ParticlesCount, " label='Particles' precision=0.1 help='Time (in seconds).' ");
	TwAddVarRO(bar, "Spread", TW_TYPE_FLOAT, &spread, " label='Spread (+/-)'");
	TwAddVarRO(bar, "Gravity", TW_TYPE_BOOLCPP, &gravityEnabled, " label='Gravity (G/H)'");

	TwAddVarRO(bar, "Attractor Point", TW_TYPE_BOOLCPP, &attractorPoint, "group ='Point 1 (0) (F3/F4)' label='Point 1:'");
	TwAddVarRO(bar, "Point1 X", TW_TYPE_FLOAT, &attractorPos[0].x, "group ='Point 1 (0) (F3/F4)' label='X:'");
	TwAddVarRO(bar, "Point1 Y", TW_TYPE_FLOAT, &attractorPos[0].y, "group ='Point 1 (0) (F3/F4)' label='Y:'");
	TwAddVarRO(bar, "Point1 Z", TW_TYPE_FLOAT, &attractorPos[0].z, "group ='Point 1 (0) (F3/F4)' label='Z:'");

	TwAddVarRO(bar, "Attractor Point 2", TW_TYPE_BOOLCPP, &attractorPoint2, "group ='Point 2 (1) (F5/F6)' label='Point 2:'");
	TwAddVarRO(bar, "Point2 X", TW_TYPE_FLOAT,  &attractorPos[1].x, "group ='Point 2 (1) (F5/F6)'  label='X:'");
	TwAddVarRO(bar, "Point2 Y", TW_TYPE_FLOAT,  &attractorPos[1].y, "group ='Point 2 (1) (F5/F6)'  label='Y:'");
	TwAddVarRO(bar, "Point2 Z", TW_TYPE_FLOAT, &attractorPos[1].z, "group ='Point 2 (1) (F5/F6)'  label='Z:'");

	TwAddVarRO(bar, "Attractor Point 3", TW_TYPE_BOOLCPP, &attractorPoint3, "group ='Point 3 (2) (F7/F8)' label='Point 3:'");
	TwAddVarRO(bar, "Point3 X", TW_TYPE_FLOAT,  &attractorPos[2].x, "group ='Point 3 (2) (F7/F8)' label='X:'");
	TwAddVarRO(bar, "Point3 Y", TW_TYPE_FLOAT,  &attractorPos[2].y, "group ='Point 3 (2) (F7/F8)' label='Y:'");
	TwAddVarRO(bar, "Point3 Z", TW_TYPE_FLOAT,  &attractorPos[2].z, "group ='Point 3 (2) (F7/F8)' label='Z:'");

	// after GLFW initialization
	// directly redirect GLFW events to AntTweakBar
	glfwSetMouseButtonCallback(window->GetWindow(),(GLFWmousebuttonfun)TwEventMouseButtonGLFW);
	glfwSetCursorPosCallback(window->GetWindow(),(GLFWcursorposfun)TwEventMousePosGLFW);
	glfwSetScrollCallback(window->GetWindow(),(GLFWscrollfun)TwEventMouseWheelGLFW);
	glfwSetKeyCallback(window->GetWindow(),(GLFWkeyfun)TwEventKeyGLFW);
	glfwSetCharCallback(window->GetWindow(),(GLFWcharfun)TwEventCharGLFW);
}

void Lab1::initShaders()
{
	// Particle Shader //

	particleShader->CreateProgram();

	particleShader->LoadFile("../Resources/Shaders/Particle.vertexshader",vs);
	particleShader->AddShader(particleShader->GetProgramID(), vs.c_str(), GL_VERTEX_SHADER);

	particleShader->LoadFile("../Resources/Shaders/Particle.fragmentshader",ps);
	particleShader->AddShader(particleShader->GetProgramID(), ps.c_str(), GL_FRAGMENT_SHADER);

	particleShader->LinkProgram();
	particleShader->ValidateProgram();
	particleShader->UseProgram();

	// Model Shader //

	modelShader->CreateProgram();

	modelShader->LoadFile("../Resources/Shaders/simpleVertexShader.vs",vs2);
	modelShader->AddShader(modelShader->GetProgramID(), vs2.c_str(), GL_VERTEX_SHADER);

	modelShader->LoadFile("../Resources/Shaders/simpleFragmentShader.ps",ps2);
	modelShader->AddShader(modelShader->GetProgramID(), ps2.c_str(), GL_FRAGMENT_SHADER);

	modelShader->LinkProgram();
	modelShader->ValidateProgram();
	modelShader->UseProgram();

}

void Lab1::initModels()
{
	box = new Model("../Resources/Models/Monkey/monkey_with_bones_y_up.dae",modelShader->GetProgramID());
}


void Lab1::initParticleSystem()
{
	CameraRight_worldspace_ID  = glGetUniformLocation(particleShader->GetProgramID(), "CameraRight_worldspace");
	CameraUp_worldspace_ID  = glGetUniformLocation(particleShader->GetProgramID(), "CameraUp_worldspace");
	ViewProjMatrixID = glGetUniformLocation(particleShader->GetProgramID(), "VP");

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	g_particule_color_data  = new GLubyte[MaxParticles * 4];

	for(int i=0; i<MaxParticles; i++)
	{
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}
	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = { 
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	//creating the buffers

	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles

	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

bool Lab1::initTextures()
{
	// Load particle texture
	if (!textureLoader::loadTexture("../Resources/Models/particle.png", &Texture))
		return false;
}


void Lab1::run()
{
	double prevTime = glfwGetTime(); 

	while(!window->IsCloseRequested())
	{
		// calculating delta time
		time = glfwGetTime();
		dt = time - prevTime;
		prevTime = time;

		window->Update();
		glUseProgram(particleShader->GetProgramID());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		GLuint modelLoc = glGetUniformLocation(modelShader->GetProgramID(), "model");
		GLuint viewLoc = glGetUniformLocation(modelShader->GetProgramID(), "view");
		GLuint projLoc = glGetUniformLocation(modelShader->GetProgramID(), "projection");
		
		camera->computeMatricesFromInputs();
		camera->handleMVP(modelLoc,viewLoc,projLoc);

		camera->getProjectionMatrix();
		camera->getViewMatrix();

		updateParticles();
		
		TwDraw();
		drawParticles();

		glDisable(GL_BLEND);
		window->SwapBuffers();
	}
}

void Lab1::updateParticles()
{

	particleControl(); // function to handle keyboard control over particles

	glm::vec3 CameraPosition(glm::inverse(camera->getViewMatrix())[3]);
	glm::mat4 ViewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix();
	std::string name = "myTextureSampler";

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1f(glGetUniformLocation(particleShader->GetProgramID(), name.c_str()), 0);

	glUniform3f(CameraRight_worldspace_ID, camera->getViewMatrix()[0][0], camera->getViewMatrix()[1][0], camera->getViewMatrix()[2][0]);
	glUniform3f(CameraUp_worldspace_ID   , camera->getViewMatrix()[0][1], camera->getViewMatrix()[1][1], camera->getViewMatrix()[2][1]);

	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);


	// Generate 10000 new particles for each period of delta time,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)((float)dt*10000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);

	for(int i=0; i<newparticles; i++)
	{
		int particleIndex = findUnusedParticle();
		ParticlesContainer[particleIndex].life = 10.0f; // This particle will live 7 seconds.
		ParticlesContainer[particleIndex].pos = glm::vec3(0,0,-20.0f); // position of the fountain
	
		// Generate a random direction; 
		glm::vec3 randomdir = glm::vec3(
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f
			);

		if(!automated)
		{
			/* Series of if-else statements checking for which attractor points are turned on at any point. 
			   If multiple attractors are turned on, then split the direction of the particles to the different attractors using mod. 
			*/

			if(attractorPoint && !attractorPoint2 && !attractorPoint3)
			{
				ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;
			}
			else if(attractorPoint && attractorPoint2 && !attractorPoint3)
			{
				if(i % 2 == 0)
				{
					ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;
				}
				else
				{
					ParticlesContainer[particleIndex].speed = maindir2 + randomdir*spread;
				}
			}
			else if(attractorPoint && !attractorPoint2 && attractorPoint3)
			{
				if(i % 2 == 0)
				{
					ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;
				}
				else
				{
					ParticlesContainer[particleIndex].speed = maindir3 + randomdir*spread;
				}
			}
			else if (!attractorPoint && attractorPoint2 && !attractorPoint3)
			{
				ParticlesContainer[particleIndex].speed = maindir2 + randomdir*spread;
			}
			else if(!attractorPoint && attractorPoint2 && attractorPoint3)
			{
				if(i % 2 == 0)
				{
					ParticlesContainer[particleIndex].speed = maindir2 + randomdir*spread;
				}
				else
				{
					ParticlesContainer[particleIndex].speed = maindir3 + randomdir*spread;
				}
			}
			else if(!attractorPoint && !attractorPoint2 && attractorPoint3)
			{
				ParticlesContainer[particleIndex].speed = maindir3 + randomdir*spread;

			}
			else if(attractorPoint && attractorPoint2 && attractorPoint3)
			{
				if(i % 3 == 0)
				{
					ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;
				}
				else if(i % 3 == 1)
				{
					ParticlesContainer[particleIndex].speed = maindir2 + randomdir*spread;
				}
				else
				{
					ParticlesContainer[particleIndex].speed = maindir3 + randomdir*spread;
				}
			}
			else
			{
				ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;
			}

		}
		else
		{
			/* basic rotation effect when space bar is hit, just rotates about the y axis and changes with delta time */ 

			rotSpeed -= dt;

			if(rotSpeed < 0)
			{
				rotSpeed = 100;
			}

			ParticlesContainer[particleIndex].speed = glm::rotateY(maindir,(float)(360 * rotSpeed/10)) + randomdir * spread; 

		}

		// Generating a random color and random size

		ParticlesContainer[particleIndex].r = rand() % 256;
		ParticlesContainer[particleIndex].g = rand() % 256;
		ParticlesContainer[particleIndex].b = rand() % 256;
		ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

		ParticlesContainer[particleIndex].size = (rand()%1000)/2000.0f + 0.1f;

	}

	ParticlesCount = 0;

	for(int i=0; i<MaxParticles; i++)
	{

		Particle& p = ParticlesContainer[i]; // shortcut

		if(p.life > 0.0f)
		{

			// Decrease life
			p.life -= 0.01;

			if (p.life > 0.0f)
			{
				if (p.pos.y < floorY)
				{
					//x(t + Dt) = x(t) + f (x,t)Dt

					p.pos.y = 0;
					p.speed.y *= -0.6;
				}
				else
				{
					if(gravityEnabled)
					{
						p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)dt * 0.5f;
					}
					else
					{
						p.speed += (float)dt * 0.5f;
					}

					p.pos += p.speed * (float)dt;
				}

				// calculate distance between particle and camera;

				p.cameradistance = glm::length( p.pos - CameraPosition );

				// Fill the GPU buffer
				g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
				g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
				g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;

				g_particule_position_size_data[4*ParticlesCount+3] = p.size;

				g_particule_color_data[4*ParticlesCount+0] = p.r;
				g_particule_color_data[4*ParticlesCount+1] = p.g;
				g_particule_color_data[4*ParticlesCount+2] = p.b;
				g_particule_color_data[4*ParticlesCount+3] = p.a;

			}
			else
			{
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.cameradistance = -1.0f;
			}

			ParticlesCount++;

		}
	}

	sortParticles();

	// Buffer orphaning: Buffer Object Streaming is the process of updating buffer objects frequently with new data while using those buffers
	// The first way is to call glBufferData​ with a NULL​ pointer, and the exact same size and usage hints it had before. 
	//This allows the implementation to simply reallocate storage for that buffer object under-the-hood. 
	//Since allocating storage is (likely) faster than the implicit synchronization, you gain significant performance advantages over synchronization. 
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);
}


void Lab1::drawParticles()
{
	// bind the buffers and draw the particles

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1


	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);
}

void Lab1::particleControl()
{
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		automated = true;
	}
	else if(glfwGetKey(window->GetWindow(), GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		automated = false;
	}

	if (glfwGetKey(window->GetWindow(), GLFW_KEY_MINUS) == GLFW_PRESS)
	{
		if(spread - 0.1f >= 0 )
		{
			spread -= 0.1f;
		}
	}
	else if (glfwGetKey(window->GetWindow(), GLFW_KEY_KP_ADD) == GLFW_PRESS)
	{
		spread += 0.1f;
	}

	if (glfwGetKey(window->GetWindow(), GLFW_KEY_0) == GLFW_PRESS)
	{
		currentAttractor = 0;
	}
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
	{
		currentAttractor = 1;
	}
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
	{
		currentAttractor = 2;
	}

	if(attractorPoint || attractorPoint2 || attractorPoint3)
	{
		if (glfwGetKey(window->GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			attractorPos[currentAttractor].x -= 0.1f;
		}
		if (glfwGetKey(window->GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			attractorPos[currentAttractor].x += 0.1f;
		}

		if (glfwGetKey(window->GetWindow(), GLFW_KEY_UP) == GLFW_PRESS)
		{
			attractorPos[currentAttractor].y += 0.1f;
		}
		if (glfwGetKey(window->GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			attractorPos[currentAttractor].y -= 0.1f;
		}

		if (glfwGetKey(window->GetWindow(), GLFW_KEY_I) == GLFW_PRESS)
		{
			attractorPos[currentAttractor].z -= 0.1f;
		}
		if (glfwGetKey(window->GetWindow(), GLFW_KEY_K) == GLFW_PRESS)
		{
			attractorPos[currentAttractor].z += 0.1f;
		}

		maindir = attractorPos[0];
		maindir2 = attractorPos[1];
		maindir3 = attractorPos[2];
	}
	else
	{
		maindir = glm::vec3(0.0f, 10.0f, 0.0f);
	}

	if (glfwGetKey(window->GetWindow(), GLFW_KEY_F3) == GLFW_PRESS)
	{
		attractorPoint = true;
	}
	else if (glfwGetKey(window->GetWindow(), GLFW_KEY_F4) == GLFW_PRESS)
	{
		attractorPoint = false;
	}

	if (glfwGetKey(window->GetWindow(), GLFW_KEY_F5) == GLFW_PRESS)
	{
		attractorPoint2 = true;
	}
	else if (glfwGetKey(window->GetWindow(), GLFW_KEY_F6) == GLFW_PRESS)
	{
		attractorPoint2 = false;
	}

	if (glfwGetKey(window->GetWindow(), GLFW_KEY_F7) == GLFW_PRESS)
	{
		attractorPoint3 = true;
	}
	else if (glfwGetKey(window->GetWindow(), GLFW_KEY_F8) == GLFW_PRESS)
	{
		attractorPoint3 = false;
	}

	// Move forward
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_G ) == GLFW_PRESS)
	{
		gravityEnabled = true;
	}
	else if (glfwGetKey(window->GetWindow(), GLFW_KEY_H ) == GLFW_PRESS)
	{
		gravityEnabled = false;
	}
}
