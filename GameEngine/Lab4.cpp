#include "Lab4.h"

void MouseButtonCB( GLFWwindow*,int button ,int action ,int mods)
{
	TwEventMouseButtonGLFW( button , action );
}

typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);
void MousePosCB(GLFWwindow*,double x ,double y)
{
	TwEventMousePosGLFW( (int)x, (int)y );
}

typedef void (* GLFWkeyfun)(GLFWwindow*,int,int,int,int);
void KeyFunCB( GLFWwindow* window,int key,int scancode,int action,int mods)
{
	TwEventKeyGLFW( key , action );
	TwEventCharGLFW( key  , action );
}

typedef void (* GLFWscrollfun)(GLFWwindow*,double,double);
void MouseScrollCB(  GLFWwindow* window, double x , double y )
{
	TwEventMouseWheelGLFW( (int)y );
}

Lab4::Lab4(void)
{
	basicShader = new Shader();
	textureShader = new Shader();
	window = new Window(800,600,"Lab 3: Collision Detection");
	camera = new Camera(window->GetWindow());

	speed = 0.001f;
	set = false;

	forcePoint = glm::vec3(1,1,0);
	forceEnabled = false;

	pauseScene = false;

	mode = NONE;
}

void Lab4::init(char** argv)
{
	initShaders();
	initModels();
	initTextures();
	initTweakBar();
}

void Lab4::initShaders()
{
	basicShader->initShader(Shader::BASIC);
	possibleShaders[Shader::BASIC] = basicShader->GetProgramID();

	textureShader->initShader(textureShader->TEXTURED);
	possibleShaders[Shader::TEXTURED] = textureShader->GetProgramID();
}

void Lab4::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

	bar = TwNewBar("Attributes: ");


	TwEnumVal modeEV[] = { {NONE, "None"}, {SPHERE, "Sphere"}, {AABB, "AABB"}, MULTI,"Multi" };
	TwType modeType;

	modeType = TwDefineEnum("SeasonType", modeEV, 4);

	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, " group='Control' label='Pause: '");
	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");
	TwAddVarRW(bar, "B Cube Pos", TW_TYPE_DIR3F, &rigidBodies[0]->position, " group='Bounding' label='Bound P: '");
	TwAddVarRW(bar, "B Cube Rot", TW_TYPE_QUAT4F, &rigidBodies[0]->orientation, " group='Bounding' label='Bound R: '");
	TwAddVarRW(bar, "B Cube Pos2", TW_TYPE_DIR3F, &rigidBodies[1]->position, " group='Bounding' label='Bound P2: '");
	TwAddVarRW(bar, "B Cube Rot2", TW_TYPE_QUAT4F, &rigidBodies[1]->orientation, " group='Bounding' label='Bound R2: '");


	TwAddVarRW(bar, "Force", TW_TYPE_BOOLCPP, &forceEnabled, "group='Object Properties' label='Enable Forces: '");
	TwAddVarRW(bar, "Mode", modeType, &mode, NULL);

	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
}

void Lab4::initModels()
{
	plane = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/plane.obj");
	plane->IsTextureActive(true);
	plane->SetPos(glm::vec3(0,-2,0));
	plane->SetOrientation(glm::quat(glm::vec3(0,0,0)));
	plane->SetScale(glm::vec3(1,1,1));
	plane->SetPossibleShaders(possibleShaders);

	for(int i = 0; i < MAX; i++)
	{
		rigidBodies[i] = new RigidBody();

		cubes[i] = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/cube.obj");
		cubes[i]->IsTextureActive(true);
		//cubes[i]->SetPos(glm::vec3(RandomNumber(-6,6),RandomNumber(-6,6),RandomNumber(-6,6)));
		cubes[i]->SetPos(glm::vec3(RandomNumber(-2,2),RandomNumber(0,0),RandomNumber(0,0)));
		cubes[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
		cubes[i]->SetScale(glm::vec3(0.5,0.5,0.5));
		cubes[i]->SetPossibleShaders(possibleShaders);

		rigidBodies[i]->setVertices(cubes[i]->GetVertices(),cubes[i]->GetPoints());
		rigidBodies[i]->position = cubes[i]->GetPos();
		rigidBodies[i]->orientation = cubes[i]->GetOrientation();
		rigidBodies[i]->scale = cubes[i]->GetScale();
		rigidBodies[i]->transVertices(rigidBodies[i]->orientation,rigidBodies[i]->position);
		rigidBodies[i]->getCentreOfMass();
		rigidBodies[i]->calculateDistanceToCOM();

		spheres[i] = new MeshLoader(basicShader->GetProgramID(),"../Resources/Models/simpleSphere.obj");
		spheres[i]->SetPos(rigidBodies[i]->centreOfMass);
		spheres[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
		spheres[i]->SetScale(glm::vec3(rigidBodies[i]->distanceToCOM));

		boundingCubes[i] = new MeshLoader(basicShader->GetProgramID(),"../Resources/Models/cube.obj");
		boundingCubes[i]->SetPos(rigidBodies[i]->centreOfMass);
		boundingCubes[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
		boundingCubes[i]->SetScale(glm::vec3(cubes[i]->scale));
	}


	for(int i = 0; i < MAX * 8; i++)
	{
		points[i] = new MeshLoader(basicShader->GetProgramID(),"../resources/models/simplesphere.obj");
	}
}

void Lab4::initTextures()
{
	plane->SetTexture("../Resources/Textures/tile.png");
	plane->SetShaderType(textureShader->shaderType);

	for(int i = 0; i < MAX; i++)
	{
		cubes[i]->SetShaderType(textureShader->shaderType);
		cubes[i]->SetTexture("../Resources/Textures/wood.png");
	}
}

void Lab4::run()
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

		update();

		TwDraw();

		window->SwapBuffers();
	}
}

void Lab4::update()
{

	camera->computeMatricesFromInputs();

	basicShader->UseProgram();

	for(int i =0; i < MAX; i++)
	{
		spheres[i]->SetColor(glm::vec3(0,1,0));
		boundingCubes[i]->SetColor(glm::vec3(0,1,0));
	}

	if(mode == SPHERE)
	{
		checkForSphereCollision();

		for(int i = 0; i < MAX; i++)
		{
			spheres[i]->position = rigidBodies[i]->centreOfMass;
			spheres[i]->orientation = rigidBodies[i]->orientation;
			spheres[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
			spheres[i]->RenderPoly();
		}
	}
	else if(mode == AABB)
	{
		for(int i = 0; i < MAX; i++)
		{
			boundingCubes[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
			rigidBodies[i]->calcMinMaxExtents();
		}

		checkForBoundingBoxCollison();

		for(int i = 0; i < MAX; i++)
		{
			boundingCubes[i]->position = rigidBodies[i]->centreOfMass;
			boundingCubes[i]->scale = (rigidBodies[i]->maxExtents - rigidBodies[i]->minExtents);
			boundingCubes[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
			boundingCubes[i]->RenderPoly();
		}

		for(int i = 0; i < MAX; i++)
		{
			for(int j = 0; j < 8; j++)
			{
				points[i]->position = rigidBodies[i]->transformedPoints[j];			
				points[i]->scale = glm::vec3(0.03,0.03,0.03);
				points[i]->SetColor(glm::vec3(1,0,0));
				points[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
				points[i]->Render();
			}
		}

	}

	plane->UseProgram();
	plane->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
	plane->Render();

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

		//rigidBodies[i]->Rotate360(dt);
		rigidBodies[i]->enableGravity();
		rigidBodies[i]->getCentreOfMass();
		rigidBodies[i]->calculateDistanceToCOM();
		rigidBodies[i]->transVertices(rigidBodies[i]->orientation, rigidBodies[i]->position);
		rigidBodies[i]->update(dt);
		rigidBodies[i]->updateModel(textureShader->modelLoc);

		cubes[i]->Render();
	}

}

void Lab4::checkForSphereCollision()
{
	//collidingPairs.clear();
	//collidingPairB.clear();

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
						spheres[i]->SetColor(glm::vec3(1,0,0));
						spheres[j]->SetColor(glm::vec3(1,0,0));

						//collidingPairs.push_back(std::make_pair(i,j));

// 						Colliders *temp = new Colliders(); 
// 						temp->objectID = i;
// 						temp->collidingObject = j;
// 						collidingPairB.push_back(temp);
					}
				}
			}
		}
	}

}


void Lab4::checkForBoundingBoxCollison()
{
	//std::cout << "Num points" << rigidBodies[0]->transformedPoints.size() << std::endl;

	collidingPairs.clear();

	if(mode == AABB)
	{
		for(int i = 0; i < MAX; i++)
		{
			for(int j = 0; j < MAX; j++)
			{
				if(i != j)
				{
					////////check the X axis
					if(glm::length(boundingCubes[i]->position.x - boundingCubes[j]->position.x) < boundingCubes[i]->scale.x + boundingCubes[j]->scale.x)
					{
						///////check the Y axis

						if(glm::length(boundingCubes[i]->position.y - boundingCubes[j]->position.y) < boundingCubes[i]->scale.y + boundingCubes[j]->scale.y)
						{
							///////check the Z axis

							if(glm::length(boundingCubes[i]->position.z - boundingCubes[j]->position.z) < boundingCubes[i]->scale.z + boundingCubes[j]->scale.z)
							{
								//boundingCubes[i]->SetColor(glm::vec3(1,0,0));
								//boundingCubes[j]->SetColor(glm::vec3(1,0,0));

								if(checkNarrowPhaseCollision(rigidBodies[i],rigidBodies[j]))
								{
									boundingCubes[i]->SetColor(glm::vec3(0,0,1));
									boundingCubes[j]->SetColor(glm::vec3(0,0,1));
								}
							}
						}
					}
				}
			}
		}

	}

}


float Lab4::RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

// Following tutorial was used for GJK: http://www.dyn4j.org/2010/04/gjk-gilbert-johnson-keerthi/

bool Lab4::checkNarrowPhaseCollision(RigidBody *rigidBody1, RigidBody *rigidBody2)
{
	simplex.clear();

	dir = rigidBody2->position - rigidBody1->position;

	simplex.push_back(GetSupportPoint(rigidBody1,rigidBody2,dir));

	dir = -dir;

	while(true)
	{
		simplex.push_back(GetSupportPoint(rigidBody1,rigidBody2,dir));
		
		// make sure that the last point we added actually passed the origin
		if(glm::dot(simplex.back(),dir) <= 0)
		{
			return false;
		}
		else
		{
			if(simplexContainsOrigin(simplex,dir))
			{
				return true;
			}
		}
	}
}

glm::vec3 Lab4::GetSupportPoint(RigidBody *rigidBody1, RigidBody *rigidBody2, glm::vec3 dir)
{
	glm::vec3 A = rigidBody1->GetFarthestPointInDirection(dir);
	glm::vec3 B = rigidBody2->GetFarthestPointInDirection(-dir);

	return A - B;
}

bool Lab4::simplexContainsOrigin(std::vector<glm::vec3> simplex, glm::vec3 dir)
{
	glm::vec3 a,b,c,d;
	glm::vec3 ab, ac, ao, abc;
	glm::vec3 abPerp, acPerp, adPerp, bcPerp, bdPerp, cdPerp;


	if(simplex.size() == 2)
	{
		std::cout << "Got to 2 simplex" << std::endl;
		a = simplex.back();
		ao = -a;
		// Line segment
		b = simplex.at(0);
		ab = b - a;

		abPerp = glm::cross(glm::cross(ab,ao),ab);

		if(glm::dot(ab,ao) > 0)
		{
			// direction perpendicular to ab
			dir = abPerp;
		}
		else
		{
			// direction from a to origin
			dir = ao;
		}

		return false;
	}
	else if(simplex.size() == 3)
	{
		std::cout << "Got to 3 simplex" << std::endl;
		// Triangle 
		a = simplex.at(2);
		ao = -a;

		b = simplex.at(1);
		c = simplex.at(0);

		ab = b - a;
		ac = c - a;
		abc = glm::cross(ab, ac);

		abPerp = glm::cross(glm::cross(ab,ao),ab);
		acPerp = glm::cross(glm::cross(ac,ao),ac);

		if(glm::dot(glm::cross(abc,ac),ao) > 0)
		{
			if(glm::dot(ac,ao) > 0)
			{
				dir = acPerp;
				simplex.erase(simplex.begin() + 1);
			}
			else
			{
				if(glm::dot(ab,ao) > 0)
				{
					dir = abPerp;
					simplex.erase(simplex.begin());
				}
				else
				{
					dir = ao;
					simplex.erase(simplex.begin());
					simplex.erase(simplex.begin());
				}
			}
		}
		else
		{
			if(glm::dot(glm::cross(ab, abc),ao) > 0)
			{
				if(glm::dot(ab,ao) > 0)
				{
					dir = abPerp;
					simplex.erase(simplex.begin());
				}
				else
				{
					dir = ao;
					simplex.erase(simplex.begin());
					simplex.erase(simplex.begin());
				}
			}
			else
			{
				if(glm::dot(abc,ao) > 0)
				{
					dir = abc;
				}
				else
				{
					dir = -abc;
					return true;
				}
			}
		}

		//return false;
	}
	else if(simplex.size() == 4)
	{
		// Tetrahedron 
		a = simplex.at(3);
		b = simplex.at(2);
		c = simplex.at(1);
		d = simplex.at(0);


	}
	
	return false;
}



/************************************************************************/
/* 
//  based on video from: http://mollyrocket.com/849

	if(dot(cross(abc,ac),ao) > 0)
	{
		if(dot(ac) > 0)
		{
			dir = cross(cross(ac, ao), ac);
			simplex.remove(b);
		}
		else
		{
			if(dot(ab) > 0)
			{
				dir = cross(cross(ab,ao),ab);
				simplex.erase(simplex.begin());
			}
			else
			{
				direction = ao;
				simplex.erase(simplex.begin());
				simplex.erase(simplex.begin());
			}
		}
	}
	else
	{
		if(dot(cross(ab, abc) > 0)
		{
			if(dot(ab) > 0)
			{
				dir = cross(cross(ab,ao),ab);
				simplex.erase(simplex.begin());
			}
			else
			{
				dir = ao;
				simplex.erase(simplex.begin());
				simplex.erase(simplex.begin());
			}
		}
		else
		{
			if(dot(abc) > 0)
			{
				dir = abc;
			}
			else
			{
				dir = -abc;
			}
		}
	}

*************************************************************************/

/*
abPerp = glm::cross(glm::cross(ac,ab),ab);
acPerp = glm::cross(glm::cross(ab,ac),ac);

// check ab region
if(glm::dot(abPerp,ao) > 0)
{
	simplex.erase(simplex.begin()); // remove c
	dir = abPerp;
}
else
{
	if(glm::dot(acPerp,ao) > 0)
	{
		simplex.erase(simplex.begin() + 1); // remove b
		dir = acPerp;
	}
	else
	{
		return true; 
	}
}
*/