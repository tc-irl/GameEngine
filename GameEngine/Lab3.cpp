#include "Lab3.h"

void MouseButtonCB( GLFWwindow*,int button ,int action ,int mods)
{
	TwEventMouseButtonGLFW( button , action );
}

//typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);
void MousePosCB(GLFWwindow*,double x ,double y)
{
	TwEventMousePosGLFW( (int)x, (int)y );
}

//typedef void (* GLFWkeyfun)(GLFWwindow*,int,int,int,int);
void KeyFunCB( GLFWwindow* window,int key,int scancode,int action,int mods)
{
	TwEventKeyGLFW( key , action );
	TwEventCharGLFW( key  , action );
}

//typedef void (* GLFWscrollfun)(GLFWwindow*,double,double);
void MouseScrollCB(  GLFWwindow* window, double x , double y )
{
	TwEventMouseWheelGLFW( (int)y );
}

Lab3::Lab3(void)
{
	basicShader = new Shader();
	textureShader = new Shader();
	window = new Window(800,600,"Lab 3: Collision Detection");
	camera = new Camera(window->GetWindow());
	//objectBuffer = new ObjectBuffer(36);

	speed = 0.001f;
	set = false;

	forcePoint = glm::vec3(1,1,0);
	forceEnabled = false;

	pauseScene = false;

	mode = NONE;
}

void Lab3::init(char** argv)
{
	//Magick::InitializeMagick(*argv);

	initShaders();
	initModels();
	initTextures();
	initTweakBar();
}

void Lab3::initShaders()
{
	basicShader->initShader("../Resources/Shaders/objNoTexture");
	textureShader->initShader("../Resources/Shaders/objWithTexture");
}

void Lab3::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

	bar = TwNewBar("Attributes: ");


	TwEnumVal modeEV[] = { {NONE, "None"}, {SPHERE, "Sphere"}, {AABB, "AABB"}, MULTI,"Multi" };
	TwType modeType;

	// Defining season enum type
	modeType = TwDefineEnum("SeasonType", modeEV, 4);
	// Adding season to bar

	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, " group='Control' label='Pause: '");
	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");
	TwAddVarRW(bar, "B Cube Pos", TW_TYPE_DIR3F, &rigidBodies[0]->position, " group='Bounding' label='Bound P: '");
	TwAddVarRW(bar, "B Cube Rot", TW_TYPE_QUAT4F, &rigidBodies[0]->orientation, " group='Bounding' label='Bound R: '");

	TwAddVarRW(bar, "Force", TW_TYPE_BOOLCPP, &forceEnabled, "group='Object Properties' label='Enable Forces: '");
	TwAddVarRW(bar, "Mode", modeType, &mode, NULL);
	
	//TwAddVarRO(bar, "Plane COM", TW_TYPE_DIR3F, &plane->centroid, " group='Plane' label='Plane COM: '");

	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
}

void Lab3::initModels()
{
	plane = new SimpleMesh(textureShader->GetProgramID(),"../Resources/Models/plane.obj", true, glm::vec3(0,-2,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));

	for(int i = 0; i < MAX; i++)
	{
		rigidBodies[i] = new RigidBody();

		cubes[i] = new SimpleMesh(textureShader->GetProgramID(),"../Resources/Models/cube.obj", true,glm::vec3(RandomNumber(-6,6),RandomNumber(-6,6),RandomNumber(-6,6)),glm::quat(glm::vec3(0,0,0)),glm::vec3(0.5,0.5,0.5));

		rigidBodies[i]->setVertices(cubes[i]->vertices,cubes[i]->points);
		rigidBodies[i]->position = cubes[i]->position;
		rigidBodies[i]->orientation = cubes[i]->orientation;
		rigidBodies[i]->scale = cubes[i]->scale;
		rigidBodies[i]->transVertices(rigidBodies[i]->orientation,rigidBodies[i]->position);

		rigidBodies[i]->getCentreOfMass();
		rigidBodies[i]->calculateDistanceToCOM();

		spheres[i] = new SimpleMesh(basicShader->GetProgramID(),"../Resources/Models/simpleSphere.obj", glm::vec3(1,1,1),false,rigidBodies[i]->centreOfMass,glm::quat(glm::vec3(0,0,0)),glm::vec3(rigidBodies[i]->distanceToCOM * cubes[i]->scale));
		boundingCubes[i] = new SimpleMesh(basicShader->GetProgramID(),"../Resources/Models/cube.obj", glm::vec3(1,1,1),false,rigidBodies[i]->centreOfMass,glm::quat(glm::vec3(0,0,0)),glm::vec3(cubes[i]->scale));
	}
}

void Lab3::initTextures()
{
	plane->initTexture("../Resources/Textures/tile.png");

	for(int i = 0; i < MAX; i++)
	{
		cubes[i]->initTexture("../Resources/Textures/wood.png");
	}
}

void Lab3::run()
{
	double prevTime = glfwGetTime(); 

	while(!window->IsCloseRequested())
	{
		time = glfwGetTime();
		dt = time - prevTime;
		prevTime = time;

		if(pauseScene == true)
		{
			dt = 0;
		}

		window->Update();

		camera->getProjectionMatrix();
		camera->getViewMatrix();

		update();

		TwDraw();

		window->SwapBuffers();
	}
}

void Lab3::update()
{

	camera->computeMatricesFromInputs();
	
	basicShader->UseProgram();
	camera->handleMVP(basicShader->modelLoc,basicShader->viewLoc,basicShader->projLoc);

	for(int i =0; i < MAX; i++)
	{
		spheres[i]->updateColor(glm::vec3(0,1,0));
		boundingCubes[i]->updateColor(glm::vec3(0,1,0));
	}

	if(mode == SPHERE)
	{
		checkForSphereCollision();

		for(int i = 0; i < MAX; i++)
		{
			spheres[i]->position = rigidBodies[i]->centreOfMass;
			spheres[i]->orientation = rigidBodies[i]->orientation;
			spheres[i]->update(basicShader->modelLoc, dt);
			spheres[i]->renderPoly();
		}
	}
	else if(mode == AABB)
	{
		for(int i = 0; i < MAX; i++)
		{
			rigidBodies[i]->calcMinMaxExtents();
		}
		
		checkForBoundingBoxCollison();

		for(int i = 0; i < MAX; i++)
		{
			boundingCubes[i]->position = rigidBodies[i]->centreOfMass;
			boundingCubes[i]->scale = (rigidBodies[i]->maxExtents - rigidBodies[i]->minExtents) * cubes[i]->scale;
			boundingCubes[i]->update(basicShader->modelLoc, dt);
			boundingCubes[i]->renderPoly();
		}
	}
	else if(mode == MULTI)
	{
		/*
		for(int i = 0; i < MAX; i++)
		{
			rigidBodies[i]->calcMinMaxExtents();
		}

		checkForBoundingBoxCollison();

		for(int i = 0; i < MAX/2; i++)
		{
			boundingCubes[0]->position = rigidBodies[0]->centreOfMass;
			boundingCubes[0]->scale = (rigidBodies[0]->maxExtents - rigidBodies[0]->minExtents) * cubes[0]->scale;
			boundingCubes[0]->update(basicShader->modelLoc, dt);
			boundingCubes[0]->renderPoly();
		}

		checkForSphereCollision();

		for(int i = 0; i < MAX/2; i++)
		{
			spheres[0]->position = rigidBodies[1]->centreOfMass;
			spheres[0]->orientation = rigidBodies[1]->orientation;
			spheres[0]->update(basicShader->modelLoc, dt);
			spheres[0]->renderPoly();
		}

		if(checkForSphereToBoxCollision(boundingCubes[0],spheres[0]))
		{
			spheres[0]->updateColor(glm::vec3(1,0,0));
			boundingCubes[0]->updateColor(glm::vec3(1,0,0));
		}
		*/
	}
	

	textureShader->UseProgram();
	camera->handleMVP(textureShader->modelLoc,textureShader->viewLoc,textureShader->projLoc);
	plane->update(textureShader->modelLoc,dt);
	plane->render();
	
	for(int i = 0; i < MAX; i++)
	{
		if(forceEnabled)
		{
			if(!set)
			{
				if(i == MAX - 1)
				{
					set = true;
				}

				rigidBodies[i]->applyForce(glm::vec3(RandomNumber(-0.3, 0.3), RandomNumber(-0.3, 0.3),RandomNumber(-0.3, 0.3)),glm::vec3(RandomNumber(-0.1, 0.1), RandomNumber(-0.1, 0.1),RandomNumber(-0.1, 0.1)));
			}
		}

		rigidBodies[i]->enableGravity();
		rigidBodies[i]->getCentreOfMass();
		rigidBodies[i]->calculateDistanceToCOM();
		rigidBodies[i]->transVertices(rigidBodies[i]->orientation, rigidBodies[i]->position);
		rigidBodies[i]->update(dt);
		rigidBodies[i]->updateModel(textureShader->modelLoc);

		cubes[i]->render();
	}

}

void Lab3::checkForSphereCollision()
{
	if(mode == SPHERE)
	{
		for(int i = 0; i < MAX; i++)
		{
			for(int j = 0; j < MAX; j++)
			{
				if(i != j)
				{
					if(glm::length(spheres[j]->position - spheres[i]->position) <= (spheres[j]->scale.x + spheres[i]->scale.x)) // check for collision with specific spheres.
					{
						spheres[i]->updateColor(glm::vec3(1,0,0));
						spheres[j]->updateColor(glm::vec3(1,0,0));
					}
				}
			}
		}
	}

}


void Lab3::checkForBoundingBoxCollison()
{
	if(mode == AABB)
	{
		for(int i = 0; i < MAX; i++)
		{
			for(int j = 0; j < MAX; j++)
			{
				if(i != j)
				{
					//check the X axis
					if(glm::length(boundingCubes[i]->position.x - boundingCubes[j]->position.x) < boundingCubes[i]->scale.x + boundingCubes[j]->scale.x)
					{
						//check the Y axis

						if(glm::length(boundingCubes[i]->position.y - boundingCubes[j]->position.y) < boundingCubes[i]->scale.y + boundingCubes[j]->scale.y)
						{
							//check the Z axis

							if(glm::length(boundingCubes[i]->position.z - boundingCubes[j]->position.z) < boundingCubes[i]->scale.z + boundingCubes[j]->scale.z)
							{
								boundingCubes[i]->updateColor(glm::vec3(1,0,0));
								boundingCubes[j]->updateColor(glm::vec3(1,0,0));
							}
						}
					}
				}
			}
		}

	}
	

}

// Based on real-time collision detection book - Christer Ericson

//bool Lab3::checkForSphereToBoxCollision(SimpleMesh *cube, SimpleMesh *sphere)
//{
//	float r2 = glm::pow(sphere->scale.x / 2,2.0f);
//
//	float dmin = 0;
//
//	for(int i = 0; i < 3; i++ ) 
//	{
//		if(sphere->position[i] < cube->scale[i])
//		{
//			dmin += glm::pow(sphere->position[i] - cube->scale[i],2.0f);
//		}
//		else if(sphere->position[i] > cube->scale[i] ) 
//		{
//			dmin += glm::pow(sphere->position[i] - cube->scale[i],2.0f);    
//		}
//	}
//
//	return dmin <= r2;
//}



float Lab3::RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}
