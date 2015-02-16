//#include "Lab4.h"
//
//void MouseButtonCB( GLFWwindow*,int button ,int action ,int mods)
//{
//	TwEventMouseButtonGLFW( button , action );
//}
//
//typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);
//void MousePosCB(GLFWwindow*,double x ,double y)
//{
//	TwEventMousePosGLFW( (int)x, (int)y );
//}
//
//typedef void (* GLFWkeyfun)(GLFWwindow*,int,int,int,int);
//void KeyFunCB( GLFWwindow* window,int key,int scancode,int action,int mods)
//{
//	TwEventKeyGLFW( key , action );
//	TwEventCharGLFW( key  , action );
//}
//
//typedef void (* GLFWscrollfun)(GLFWwindow*,double,double);
//void MouseScrollCB(  GLFWwindow* window, double x , double y )
//{
//	TwEventMouseWheelGLFW( (int)y );
//}
//
//Lab4::Lab4(void)
//{
//	basicShader = new Shader();
//	textureShader = new Shader();
//	window = new Window(800,600,"Lab 3: Collision Detection");
//	camera = new Camera(window->GetWindow());
//
//	speed = 0.001f;
//	set = false;
//
//	forcePoint = glm::vec3(1,1,0);
//	forceEnabled = false;
//	gravityEnabled = false;
//	collisionsEnabled = false;
//
//	pauseScene = false;
//
//	mode = NONE;
//}
//
//void Lab4::init(char** argv)
//{
//	initShaders();
//	initModels();
//	initTextures();
//	initTweakBar();
//}
//
//void Lab4::initShaders()
//{
//	basicShader->initShader(Shader::BASIC);
//	possibleShaders[Shader::BASIC] = basicShader->GetProgramID();
//
//	textureShader->initShader(textureShader->TEXTURED);
//	possibleShaders[Shader::TEXTURED] = textureShader->GetProgramID();
//
//
//}
//
//void Lab4::initTweakBar()
//{
//	TwInit(TW_OPENGL_CORE, NULL);
//	TwWindowSize(window->GetWidth(), window->GetHeight());
//
//	bar = TwNewBar("Attributes: ");
//
//
//	TwEnumVal modeEV[] = { {NONE, "None"}, {SPHERE, "Sphere"}, {AABB, "AABB"}, {AABB_PLANE,"Plane Collision"}, {MULTI,"Multi"}};
//	TwType modeType;
//
//	modeType = TwDefineEnum("SeasonType", modeEV, 5);
//
//	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, " group='Control' label='Pause: '");
//	TwAddVarRW(bar, "Gravity", TW_TYPE_BOOLCPP, &gravityEnabled, " group='Control' label='Gravity: '");
//	TwAddVarRW(bar, "Collisions", TW_TYPE_BOOLCPP, &collisionsEnabled, " group='Control' label='Collisions: '");
//	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
//	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");
//	TwAddVarRW(bar, "B Cube Pos", TW_TYPE_DIR3F, &rigidBodies[0]->position, " group='Bounding' label='Bound P: '");
//	TwAddVarRW(bar, "B Cube Rot", TW_TYPE_QUAT4F, &rigidBodies[0]->orientation, " group='Bounding' label='Bound R: '");
//		TwAddVarRW(bar, "Cube Pos", TW_TYPE_DIR3F, &rigidBodies[1]->position, " group='Bounding' label='Bound P: '");
//		TwAddVarRW(bar, "Cube Rot", TW_TYPE_QUAT4F, &rigidBodies[1]->orientation, " group='Bounding' label='Bound R: '");
//	TwAddVarRW(bar, "F", TW_TYPE_DIR3F, &forcePoint, " group='Bounding' label='Force point: '");
//
//	TwAddVarRW(bar, "Force", TW_TYPE_BOOLCPP, &forceEnabled, "group='Object Properties' label='Enable Forces: '");
//	TwAddVarRW(bar, "Mode", modeType, &mode, NULL);
//
//	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
//	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
//	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
//	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
//}
//
//void Lab4::initModels()
//{
//	plane = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/plane.obj");
//	plane->IsTextureActive(true);
//	plane->SetPos(glm::vec3(0,0,0));
//	plane->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//	plane->SetScale(glm::vec3(10,1,10));
//	plane->SetPossibleShaders(possibleShaders);
//
//	for(int i = 0; i < MAX; i++)
//	{
//		rigidBodies[i] = new RigidBody();
//
//		cubes[i] = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/cube.obj");
//		cubes[i]->IsTextureActive(true);
//		//cubes[i]->SetPos(glm::vec3(RandomNumber(-6,6),RandomNumber(4,6),RandomNumber(-6,6)));
//		cubes[i]->SetPos(glm::vec3(RandomNumber(-4,4),RandomNumber(4,6),RandomNumber(0,0)));
//		cubes[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//		cubes[i]->SetScale(glm::vec3(1,1,1));
//		cubes[i]->SetPossibleShaders(possibleShaders);
//
//		rigidBodies[i]->setVertices(cubes[i]->GetVertices(),cubes[i]->GetPoints());
//		rigidBodies[i]->position = cubes[i]->GetPos();
//		rigidBodies[i]->orientation = cubes[i]->GetOrientation();
//		rigidBodies[i]->scale = cubes[i]->GetScale();
//		rigidBodies[i]->transVertices(rigidBodies[i]->orientation,rigidBodies[i]->position);
//		rigidBodies[i]->getCentreOfMass();
//		rigidBodies[i]->calculateDistanceToCOM();
//
//		spheres[i] = new MeshLoader(basicShader->GetProgramID(),"../Resources/Models/simpleSphere.obj");
//		spheres[i]->SetPos(rigidBodies[i]->centreOfMass);
//		spheres[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//		spheres[i]->SetScale(glm::vec3(rigidBodies[i]->distanceToCOM));
//
//		boundingCubes[i] = new MeshLoader(basicShader->GetProgramID(),"../Resources/Models/cube.obj");
//		boundingCubes[i]->SetPos(rigidBodies[i]->centreOfMass);
//		boundingCubes[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//		boundingCubes[i]->SetScale(glm::vec3(cubes[i]->scale));
//	}
//
//	for(int i = 0; i < MAX + 1; i++)
//	{
//		points[i] = new MeshLoader(basicShader->GetProgramID(),"../Resources/Models/simpleSphere.obj");
//	}
//
//	line = new Line(basicShader->GetProgramID());
//	directionNormal = new Line(basicShader->GetProgramID());
//	
//	std::vector<glm::vec3> temp;
//	temp.push_back(points[0]->position);
//	temp.push_back(points[1]->position);
//
//	std::vector<glm::vec3> temp2;
//	temp2.push_back(plane->position);
//	temp2.push_back(glm::vec3(0,0,0));
//
//	line->GenerateBuffer(temp);
//
//	directionNormal->GenerateBuffer(temp2);
//}
//
//void Lab4::initTextures()
//{
//	plane->SetTexture("../Resources/Textures/tile.png");
//	plane->SetShaderType(textureShader->shaderType);
//
//	for(int i = 0; i < MAX; i++)
//	{
//		cubes[i]->SetShaderType(textureShader->shaderType);
//		cubes[i]->SetTexture("../Resources/Textures/wood.png");
//	}
//}
//
//void Lab4::run()
//{
//	double prevTime = glfwGetTime(); 
//
//	while(!window->IsCloseRequested())
//	{
//		time = glfwGetTime();
//		dt = time - prevTime;
//		prevTime = time;
//
//		if(pauseScene == true)
//		{
//			dt = 0;
//		}
//
//		window->Update();
//
//		update();
//
//		TwDraw();
//
//		window->SwapBuffers();
//	}
//}
//
//void Lab4::update()
//{
//
//	camera->computeMatricesFromInputs();
//
//	basicShader->UseProgram();
//
//	for(int i =0; i < MAX; i++)
//	{
//		spheres[i]->SetColor(glm::vec3(0,1,0));
//		boundingCubes[i]->SetColor(glm::vec3(0,1,0));
//	}
//
//
//	if(mode == SPHERE)
//	{
//		checkForSphereCollision();
//
//		for(int i = 0; i < MAX; i++)
//		{
//			spheres[i]->position = rigidBodies[i]->centreOfMass;
//			spheres[i]->orientation = rigidBodies[i]->orientation;
//			spheres[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//			spheres[i]->RenderPoly();
//		}
//	}
//
//	else if(mode == AABB)
//	{
//		for(int i = 0; i < MAX; i++)
//		{
//			boundingCubes[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//			rigidBodies[i]->calcMinMaxExtents();
//		}
//
//		checkForBoundingBoxCollison();
//
//		for(int i = 0; i < MAX; i++)
//		{
//			boundingCubes[i]->position = rigidBodies[i]->centreOfMass;
//			boundingCubes[i]->scale = (rigidBodies[i]->maxExtents - rigidBodies[i]->minExtents);
//			boundingCubes[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//			boundingCubes[i]->RenderPoly();
//		}
//
//	}
//
//	else if(mode == AABB_PLANE)
//	{
//		for(int i = 0; i < MAX; i++)
//		{
//			boundingCubes[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//			rigidBodies[i]->calcMinMaxExtents();
//		}
//
//		checkForBoundingBoxCollisonWithPlane();
//
//		for(int i = 0; i < MAX; i++)
//		{
//			boundingCubes[i]->position = rigidBodies[i]->centreOfMass;
//			boundingCubes[i]->scale = (rigidBodies[i]->maxExtents - rigidBodies[i]->minExtents);
//			boundingCubes[i]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//			boundingCubes[i]->RenderPoly();
//		}
//	}
//
//	plane->UseProgram();
//	plane->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	plane->Render();
//
//	for(int i = 0; i < MAX; i++)
//	{
//		if(forceEnabled)
//		{
//			if(!set)
//			{
//				if(i == MAX - 1)
//				{
//					set = true;
//				}
//
//				rigidBodies[i]->applyForce(glm::vec3(RandomNumber(-0.3, 0.3), RandomNumber(-0.3, -0.3),RandomNumber(-0.3, 0.3)),glm::vec3(RandomNumber(-0.1, 0.1), RandomNumber(-0.3f, -0.3f),RandomNumber(-0.1, 0.1)));
//			}
//		}
//
//		//rigidBodies[i]->Rotate360(dt);
//
//		if(gravityEnabled == true)
//		{
//			rigidBodies[i]->enableGravity(dt);
//		}
//
//		if(collisionsEnabled)
//		{
//			rigidBodies[i]->collidingEnabled = true;
//		}
//
//		rigidBodies[i]->getCentreOfMass();
//		rigidBodies[i]->calculateDistanceToCOM();
//		rigidBodies[i]->transVertices(rigidBodies[i]->orientation, rigidBodies[i]->position);
//		rigidBodies[i]->update(dt);
//		rigidBodies[i]->updateModel(textureShader->modelLoc);
//
//		cubes[i]->Render();
//	}
//
//}
//
//void Lab4::checkForSphereCollision()
//{
//	if(mode == SPHERE)
//	{
//		for(int i = 0; i < MAX; i++)
//		{
//			for(int j = 0; j < MAX; j++)
//			{
//				if(i != j)
//				{
//					if(glm::length(spheres[j]->position - spheres[i]->position) <= (spheres[j]->scale.x + spheres[i]->scale.x)) // check for collision with specific spheres.
//					{
//						if(checkNarrowPhaseCollision(rigidBodies[i],rigidBodies[j]))
//						{
//							spheres[i]->SetColor(glm::vec3(0,0,1));
//							spheres[j]->SetColor(glm::vec3(0,0,1));
//						}
//						else
//						{
//							spheres[i]->SetColor(glm::vec3(1,0,0));
//							spheres[j]->SetColor(glm::vec3(1,0,0));
//						}
//					}
//				}
//			}
//		}
//	}
//
//}
//
//
//void Lab4::checkForBoundingBoxCollison()
//{
//	//std::cout << "Num points" << rigidBodies[0]->transformedPoints.size() << std::endl;
//
//	if(mode == AABB)
//	{
//		for(int i = 0; i < MAX; i++)
//		{
//			for(int j = 0; j < MAX; j++)
//			{
//				if(i != j)
//				{
//					////////check the X axis
//					if(glm::length(boundingCubes[i]->position.x - boundingCubes[j]->position.x) < boundingCubes[i]->scale.x + boundingCubes[j]->scale.x)
//					{
//						///////check the Y axis
//
//						if(glm::length(boundingCubes[i]->position.y - boundingCubes[j]->position.y) < boundingCubes[i]->scale.y + boundingCubes[j]->scale.y)
//						{
//							///////check the Z axis
//
//							if(glm::length(boundingCubes[i]->position.z - boundingCubes[j]->position.z) < boundingCubes[i]->scale.z + boundingCubes[j]->scale.z)
//							{
//								boundingCubes[i]->SetColor(glm::vec3(1,0,0));
//								boundingCubes[j]->SetColor(glm::vec3(1,0,0));
//
//								if(checkNarrowPhaseCollision(rigidBodies[i],rigidBodies[j]))
//								{
//									boundingCubes[i]->SetColor(glm::vec3(0,0,1));
//									boundingCubes[j]->SetColor(glm::vec3(0,0,1));
//								}
//								else
//								{
//									boundingCubes[i]->SetColor(glm::vec3(1,0,0));
//									boundingCubes[j]->SetColor(glm::vec3(1,0,0));
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//
//float Lab4::RandomNumber(float Min, float Max)
//{
//	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
//}
//
//// Following tutorial were helpful/used for GJK: http://www.dyn4j.org/2010/04/gjk-gilbert-johnson-keerthi/
//// http://in2gpu.com/2014/05/18/gjk-algorithm-3d/
//// http://entropyinteractive.com/2011/04/gjk-algorithm/
//// And video: http://mollyrocket.com/849
//
//
//bool Lab4::checkNarrowPhaseCollision(RigidBody *rigidBody1, RigidBody *rigidBody2)
//{
//	glm::vec3 dir = rigidBody2->position - rigidBody1->position;
//
//	glm::vec3 farA = rigidBody1->GetFarthestPointInDirection(dir);
//	glm::vec3 farB = rigidBody2->GetFarthestPointInDirection(-dir);
//
//	glm::vec3 supportPoint = farA - farB;
//
//	std::vector<glm::vec3> simplex;
//	simplex.push_back(supportPoint);
//
//	dir = -supportPoint;
//
//	int count = 0;
//
//	while(count < 100)
//	{
//		farA = rigidBody1->GetFarthestPointInDirection(dir);
//		farB = rigidBody2->GetFarthestPointInDirection(-dir);
//
//		supportPoint = farA - farB;
//
//		if(glm::dot(supportPoint,dir) < 0)
//		{
//			return false;
//		}
//
//		simplex.push_back(supportPoint);
//
//		if(simplexContainsOrigin(simplex,dir))
//		{
//			return true;
//		}
//
//		count++;
//	}
//
//	return false;
//}
//
//bool Lab4::simplexContainsOrigin(std::vector<glm::vec3> &simplex, glm::vec3 &dir)
//{
//		glm::vec3 a,b,c,d;
//	
//		// Based on video: http://mollyrocket.com/849
//		if(simplex.size() == 2)
//		{
//			a = simplex[1];
//			b = simplex[0];
//	
//			glm::vec3 ab = b - a;
//			glm::vec3 ao = -a;
//	
//			glm::vec3 abPerp = glm::cross(glm::cross(ab,ao),ab);
//	
//			if(glm::dot(ab,ao) > 0)
//			{
//				dir = abPerp;
//			}
//			else
//			{
//				dir = ao;
//			}
//	
//			return false;
//		}
//		else if(simplex.size() == 3)
//		{
//			return checkTriangle(simplex,dir);
//		}
//		// Helpful site of tetrahedron: http://in2gpu.com/2014/05/18/gjk-algorithm-3d/
//		else if(simplex.size() == 4)
//		{
//			a = simplex[3];
//			b = simplex[2];
//			c = simplex[1];
//			d = simplex[0];
//	
//			glm::vec3 ao = -a;
//	
//			glm::vec3 ab = b - a;
//			glm::vec3 ac = c - a;
//			glm::vec3 ad = d - a;
//	
//			glm::vec3 abc = glm::cross(ab, ac);
//			glm::vec3 acd = glm::cross(ac, ad);
//			glm::vec3 adb = glm::cross(ad, ab);
//	
//			glm::vec3 abPerp = glm::cross(glm::cross(ab,ao),ab);
//			glm::vec3 acPerp = glm::cross(glm::cross(ac,ao),ac);
//			glm::vec3 ab_abc = glm::cross(ab,abc);
//			glm::vec3 abc_ac = glm::cross(abc,ac);
//	
//			// In front of abc face
//			if(glm::dot(abc,ao) > 0)
//			{	
//				simplex.erase(simplex.begin()); // erase d
//	
//				return checkTriangle(simplex,dir);
//			}
//			// In front of ACD face
//			else if(glm::dot(acd,ao) > 0)
//			{
//				simplex.erase(simplex.begin() + 2); // erase b
//		
//				return checkTriangle(simplex,dir);
//			}
//			// in front of ADB face
//			else if(glm::dot(adb,ao) > 0)
//			{
//				simplex.erase(simplex.begin() + 1); // erase c
//				simplex[0] = b;
//				simplex[1] = d;
//	
//				return checkTriangle(simplex,dir);
//			}
//
//			// NB. origin cannot be below cbd because we already checked that
//	
//			return true;
//	
//		}
//}
//
//// Based on video: http://mollyrocket.com/849
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
//			simplex.erase(simplex.begin() + 1); // erase b
//		}
//		else
//		{
//			if(glm::dot(ab,ao) > 0)
//			{
//				dir = abPerp;
//				simplex.erase(simplex.begin()); // erase c
//			}
//			else
//			{
//				dir = ao;
//				simplex.erase(simplex.begin()); // erase c
//				simplex.erase(simplex.begin()); // erase b
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
//				simplex.erase(simplex.begin()); // erase c
//			}
//			else
//			{
//				dir = ao;
//				simplex.erase(simplex.begin()); // erase c
//				simplex.erase(simplex.begin()); // erase b
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
//
//void Lab4::checkForBoundingBoxCollisonWithPlane()
//{
//	//std::cout << "Num points" << rigidBodies[0]->transformedPoints.size() << std::endl;
//
//	if(mode == AABB_PLANE)
//	{
//		for(int i = 0; i < MAX; i++)
//		{
//			if(glm::length(boundingCubes[i]->position.y - plane->position.y) <= boundingCubes[i]->scale.y - plane->position.y)
//			{
//				boundingCubes[i]->SetColor(glm::vec3(1,0,0));
//
//				closestPoint = GetClosestPointToPlane(rigidBodies[i]->transformedPoints);
//				planePoint = GetPointOnPlane(closestPoint);
//
//				points[0]->position = closestPoint;
//				points[0]->SetColor(glm::vec3(0,1,0));
//				points[0]->scale = glm::vec3(0.04,0.04,0.04);
//				points[0]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//				points[0]->Render();
//
//				points[1]->position = planePoint;
//				points[1]->SetColor(glm::vec3(0,0,1));
//				points[1]->scale = glm::vec3(0.04,0.04,0.04);
//				points[1]->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//				points[1]->Render();
//
//				//distanceToClosestPoint = GetDistanceToPlane(closestPoint);
//
//				line->SetFromTo(points[0]->position ,points[1]->position );
//				line->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//				line->DrawLine();
//
//				directionNormal->SetFromTo(planePoint,glm::vec3(planePoint.x,3,planePoint.z));
//				directionNormal->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//				directionNormal->DrawLine();
//
//				if(collisionsEnabled)
//				{
//					rigidBodies[i]->collisionResponse(dt,closestPoint,planePoint,glm::vec3(0,1,0));
//				}
//			}
//	
//		}
//	}
//}
//
//// Calculates the distance to plane based on the closest vertex
////float Lab4::GetDistanceToPlane(glm::vec3 point)
////{
////
////}
//
//glm::vec3 Lab4::GetClosestPointToPlane(std::vector<glm::vec3> points)
//{
//	// d = ((v-f).n);
//
//	glm::vec3 normal = glm::vec3(0,1,0);
//	glm::vec3 closestPoint = points[0];
//	float closest = glm::dot((points[0] - glm::vec3(0,0,0)),normal);
//	float temp;
//
//	for(int i = 1; i < points.size(); i++)
//	{
//		temp = glm::dot(points[i] - glm::vec3(0,0,0),normal);
//
//		if(temp < closest)
//		{
//			closest = temp;
//			closestPoint = points[i];
//		}
//	}
//
//	return closestPoint;
//}
//
//glm::vec3 Lab4::GetPointOnPlane(glm::vec3 closestPoint)
//{
//	glm::vec3 pointOnPlane = glm::vec3(0,0,0);
//	glm::vec3 normal = glm::vec3(0,1,0);
//
//	pointOnPlane = closestPoint - (glm::dot((closestPoint - glm::vec3(0,0,0)),normal)) * normal; 
//
//	return pointOnPlane;
//}
