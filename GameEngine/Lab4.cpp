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
		cubes[i]->SetPos(glm::vec3(RandomNumber(-4,4),RandomNumber(0,0),RandomNumber(0,0)));
		cubes[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
		cubes[i]->SetScale(glm::vec3(1,1,1));
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
		points[i] = new MeshLoader(basicShader->GetProgramID(),"../resources/models/simpleSphere.obj");
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

		//for(int i = 0; i < MAX; i++)
		//{
		//	for(int j = 0; j < 8; j++)
		//	{
		//		if(i == 0)
		//		{
		//			points[i]->position = rigidBodies[0]->transformedPoints[j];			
		//			points[i]->scale = glm::vec3(0.03,0.03,0.03);
		//			points[i]->SetColor(glm::vec3(1,0,0));
		//			points[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
		//			points[i]->Render();
		//		}
		//		else
		//		{
		//			points[i]->position = rigidBodies[1]->transformedPoints[j];			
		//			points[i]->scale = glm::vec3(0.03,0.03,0.03);
		//			points[i]->SetColor(glm::vec3(1,0,0));
		//			points[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
		//			points[i]->Render();
		//		}
		//	}
		//}

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

	if(mode == AABB)
	{
		for(int i = 0; i < MAX; i++)
		{
			for(int j = 0; j < MAX; j++)
			{
				if(i != j)
				{
					if(checkNarrowPhaseCollision(rigidBodies[i],rigidBodies[j]))
					{
						boundingCubes[i]->SetColor(glm::vec3(0,0,1));
						boundingCubes[j]->SetColor(glm::vec3(0,0,1));
					}
			
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
// And video: http://mollyrocket.com/849

bool Lab4::checkNarrowPhaseCollision(RigidBody *rigidBody1, RigidBody *rigidBody2)
{
	glm::vec3 dir = rigidBody2->position - rigidBody1->position;

	glm::vec3 farA = rigidBody1->GetFarthestPointInDirection(dir);
	glm::vec3 farB = rigidBody2->GetFarthestPointInDirection(-dir);

	glm::vec3 mDiff = farA - farB;

	std::vector<glm::vec3> simplex;
	simplex.push_back(mDiff);

	dir = -mDiff;

	int count = 100;

	while(count > 0)
	{
		farA = rigidBody1->GetFarthestPointInDirection(dir);
		farB = rigidBody2->GetFarthestPointInDirection(-dir);

		mDiff = farA - farB;

		if(glm::dot(mDiff,dir) < 0)
		{
			return false;
		}

		simplex.push_back(mDiff);

		if(simplexContainsOrigin(simplex,dir))
		{
			std::cout << "Colliding" << std::endl;
			return true;
		}

		count--;
	}

	return false;
}

	bool doOnce = false;

bool Lab4::simplexContainsOrigin(std::vector<glm::vec3> &simplex, glm::vec3 &dir)
{
	glm::vec3 A,B,C,D;

	switch(simplex.size())
	{
	case 2: // line
		B = simplex[0];
		A = simplex[1];

		if(glm::dot(B-A, -A) > 0)
		{
			dir = glm::cross(glm::cross(B-A, -A), B-A);
		}
		else
		{
			dir = -A;
		}

		return false;

	case 3: // triangle

		return checkTriangle(simplex, dir);

	case 4:	// tetrahedron
		D = simplex[0];
		C = simplex[1];
		B = simplex[2];
		A = simplex[3];

		//if(!doOnce)
		//{
		//	points[0]->position = D;
		//	points[1]->position = C;
		//	points[2]->position = B;
		//	points[3]->position = A;

		//	points[0]->scale = glm::vec3(0.03,0.03,0.03);
		//	points[1]->scale = glm::vec3(0.03,0.03,0.03);
		//	points[2]->scale = glm::vec3(0.03,0.03,0.03);
		//	points[3]->scale = glm::vec3(0.03,0.03,0.03);

		//	points[0]->SetColor(glm::vec3(1,0,0));
		//	points[1]->SetColor(glm::vec3(1,0,0));
		//	points[2]->SetColor(glm::vec3(1,0,0));
		//	points[3]->SetColor(glm::vec3(1,0,0));

		//	doOnce = true;

		//}

		////points[0]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
		////points[0]->Render();
		////points[1]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
		////points[1]->Render();

		////points[2]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
		////points[2]->Render();
		////points[3]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
		////points[3]->Render();
		////
		glm::vec3 ABC = glm::cross(B-A, C-A);
		glm::vec3 ADB = glm::cross(D-A, B-A);
		glm::vec3 ACD = glm::cross(C-A, D-A);

		if(glm::dot(ABC, -A) > 0)
		{			
			simplex.erase(simplex.begin());

			return checkTriangle(simplex, dir);
		}
		else if(glm::dot(ADB, -A) > 0)
		{
			simplex.erase(simplex.begin() + 1);
			simplex[0] = B;
			simplex[1] = D;

			return checkTriangle(simplex, dir);
		}
		else if(glm::dot(ACD, -A) > 0)
		{
			simplex.erase(simplex.begin() + 2);

			return checkTriangle(simplex, dir);
		}

		return true;
	}
}

bool Lab4::checkTriangle(std::vector<glm::vec3> &simplex,glm::vec3 &dir)
{
	glm::vec3 C = simplex[0];
	glm::vec3 B = simplex[1];
	glm::vec3 A = simplex[2];

	// face
	glm::vec3 ABC = glm::cross(B-A, C-A);

	if(glm::dot(glm::cross(ABC, C-A), -A) > 0) // AC plane 
	{
		if(glm::dot(C-A, -A) > 0) // outside AC edge
		{
			dir = glm::cross(glm::cross(C-A, -A), C-A);
			simplex.erase(simplex.begin() + 1);
		}
		else
		{
			if(glm::dot(B-A, -A) > 0) // outside AB edge
			{
				dir = glm::cross(glm::cross(B-A, -A), B-A);
				simplex.erase(simplex.begin());
			}
			else // outside A
			{
				dir = -A;
				simplex.erase(simplex.begin());
				simplex.erase(simplex.begin());
			}
		}
	}
	else // inside AC 
	{
		if(glm::dot(glm::cross(B-A, ABC), -A) > 0) // AB plane 
		{
			if(glm::dot(B-A, -A) > 0) // outside AB plane
			{
				dir = glm::cross(glm::cross(B-A, -A), B-A);
				simplex.erase(simplex.begin());
			}
			else // outside A
			{
				dir = -A;
				simplex.erase(simplex.begin());
				simplex.erase(simplex.begin());
			}
		}
		else // orthogonal to face
		{
			if(glm::dot(ABC, -A) > 0) // outside face
			{
				dir = ABC;
			}
			else // inside face
			{
				simplex[0] = B;
				simplex[1] = C;

				dir = -ABC;
			}
		}
	}

	return false;
}
//
//glm::vec3 Lab4::GetFarthestPointInDirection(glm::vec3 dir, std::vector<glm::vec3> points)
//{
//	glm::vec3 farthestPoint;
//
//	float farthest = glm::dot(points[0],dir);
//
//	//std::cout << "Point 0 = " << "(" << transformedPoints[0].x << "," << transformedPoints[0].y << "," << transformedPoints[0].z << ")" << std::endl;
//
//	for(int i = 1; i <points.size();i++)
//	{
//		//std::cout << "Point " << i << " = " << "(" << transformedPoints[i].x << "," << transformedPoints[i].y << "," << transformedPoints[i].z << ")" << std::endl;
//		float temp = glm::dot(dir,points[i]);
//
//		if(temp > farthest)
//		{
//			farthest = temp;
//			farthestPoint = points[i];
//		}
//	}
//
//	return farthestPoint;
//}


//bool Lab4::checkNarrowPhaseCollision(RigidBody *rigidBody1, RigidBody *rigidBody2)
//{
//	std::vector<glm::vec3> simplex;
//
//	glm::vec3 dir = rigidBody2->position - rigidBody1->position;
//
//	glm::vec3 furthestA = rigidBody1->GetFarthestPointInDirection(dir);
//	glm::vec3 furthestB = rigidBody2->GetFarthestPointInDirection(-dir);
//
//	glm::vec3 supportPoint = GetSupportPoint(furthestA,furthestB);
//
//	simplex.push_back(supportPoint);
//
//	dir = -supportPoint;
//
//	int count = 0;
//
//	while(count < 100)
//	{
//		furthestA = rigidBody1->GetFarthestPointInDirection(dir);
//		furthestB = rigidBody2->GetFarthestPointInDirection(-dir);
//
//		supportPoint = GetSupportPoint(furthestA,furthestB);
//
//		// make sure that the last point we added actually passed the origin
//		if(glm::dot(supportPoint,dir) < 0)
//		{
//			return NULL;
//		}
//
//		simplex.push_back(supportPoint);
//
//		if(simplexContainsOrigin(simplex,dir))
//		{
//			std::cout << "Colliding" << std::endl;
//			return true;
//		}
//
//		count++;
//	}
//
//	return false;
//}
//
//glm::vec3 Lab4::GetSupportPoint(glm::vec3 furthestA, glm::vec3 furthestB)
//{
//	glm::vec3 C = furthestA - furthestB;
//	return C;
//}
//
//bool Lab4::simplexContainsOrigin(std::vector<glm::vec3> &simplex, glm::vec3 &dir)
//{
//	glm::vec3 a,b,c,d;
//
//	if(simplex.size() == 2)
//	{
//		a = simplex[1];
//		b = simplex[0];
//
//		glm::vec3 ab = b - a;
//		glm::vec3 ao = -a;
//
//		glm::vec3 abPerp = glm::cross(glm::cross(ab,ao),ab);
//
//		if(glm::dot(ab,ao) > 0)
//		{
//			dir = abPerp;
//		}
//		else
//		{
//			dir = ao;
//		}
//
//		return false;
//	}
//	else if(simplex.size() == 3)
//	{
//		return checkTriangle(simplex,dir);
//	}
//
//	else if(simplex.size() == 4)
//	{
//		a = simplex[3];
//		b = simplex[2];
//		c = simplex[1];
//		d = simplex[0];
//
//		glm::vec3 ao = -a;
//
//		glm::vec3 ab = b - a;
//		glm::vec3 ac = c - a;
//		glm::vec3 ad = d - a;
//
//		glm::vec3 abc = glm::cross(ab, ac);
//		glm::vec3 acd = glm::cross(ac, ad);
//		glm::vec3 adb = glm::cross(ad, ab);
//
//		glm::vec3 abPerp = glm::cross(glm::cross(ab,ao),ab);
//		glm::vec3 acPerp = glm::cross(glm::cross(ac,ao),ac);
//		glm::vec3 ab_abc = glm::cross(ab,abc);
//		glm::vec3 abc_ac = glm::cross(abc,ac);
//
//		//case 1
//		if(glm::dot(abc,ao) > 0)
//		{
//			std::cout << "Case 1 Hit" << std::endl;
//
//			simplex.erase(simplex.begin()); // erase d
//
//			return checkTriangle(simplex,dir);
//		}
//		else if(glm::dot(acd,ao) > 0)
//		{
//			simplex.erase(simplex.begin() + 2);
//
//			std::cout << "Case 2 Hit" << std::endl;
//
//			return checkTriangle(simplex,dir);
//		}
//		else if(glm::dot(adb,ao) > 0)
//		{
//			simplex.erase(simplex.begin() + 1);
//			simplex[0] = b;
//			simplex[1] = d;
//
//			std::cout << "Case 3 Hit" << std::endl;
//
//			return checkTriangle(simplex,dir);
//		}
//
//		return true;
//
//	}
//}
//
//bool Lab4::checkTriangle(std::vector<glm::vec3> &simplex,glm::vec3 &dir)
//{
//	glm::vec3 c = simplex[0];
//	glm::vec3 b = simplex[1];
//	glm::vec3 a = simplex[2];
//
//	glm::vec3 ao = -a;
//
//	glm::vec3 ab = b - a;
//	glm::vec3 ac = c - a;
//
//	glm::vec3 abc = glm::cross(ab, ac);
//
//	glm::vec3 abPerp = glm::cross(glm::cross(ab,ao),ab);
//	glm::vec3 acPerp = glm::cross(glm::cross(ac,ao),ac);
//	glm::vec3 ab_abc = glm::cross(ab,abc);
//	glm::vec3 abc_ac = glm::cross(abc,ac);
//
//	if(glm::dot(abc_ac,ao) > 0)
//	{
//		if(glm::dot(ac,ao) > 0)
//		{
//			dir = acPerp;
//			simplex.erase(simplex.begin() + 1);
//		}
//		else
//		{
//			if(glm::dot(ab,ao) > 0)
//			{
//				dir = abPerp;
//				simplex.erase(simplex.begin());
//			}
//			else
//			{
//				dir = ao;
//				simplex.erase(simplex.begin());
//				simplex.erase(simplex.begin());
//			}
//		}
//	}
//	else
//	{
//		if(glm::dot(ab_abc,ao) > 0) // AB plane 
//		{
//			if(glm::dot(ab,ao) > 0)
//			{
//				dir = abPerp;
//				simplex.erase(simplex.begin());
//			}
//			else
//			{
//				dir = ao;
//				simplex.erase(simplex.begin());
//				simplex.erase(simplex.begin());
//			}
//		}
//		else
//		{
//			if(glm::dot(abc, ao) > 0) // outside face
//			{
//				dir = abc;
//			}
//			else // inside face
//			{
//				simplex[0] = b;
//				simplex[1] = c;
//
//				dir = -abc;
//			}
//		}
//	}
//
//	return false;
//
//}