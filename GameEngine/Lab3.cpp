//#include "Lab3.h"
//
//void MouseButtonCB( GLFWwindow*,int button ,int action ,int mods)
//{
//	TwEventMouseButtonGLFW( button , action );
//}
//
////typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);
//void MousePosCB(GLFWwindow*,double x ,double y)
//{
//	TwEventMousePosGLFW( (int)x, (int)y );
//}
//
////typedef void (* GLFWkeyfun)(GLFWwindow*,int,int,int,int);
//void KeyFunCB( GLFWwindow* window,int key,int scancode,int action,int mods)
//{
//	TwEventKeyGLFW( key , action );
//	TwEventCharGLFW( key  , action );
//}
//
////typedef void (* GLFWscrollfun)(GLFWwindow*,double,double);
//void MouseScrollCB(  GLFWwindow* window, double x , double y )
//{
//	TwEventMouseWheelGLFW( (int)y );
//}
//
//Lab3::Lab3(void)
//{
//	basicShader = new Shader();
//	textureShader = new Shader();
//	window = new Window(800,600,"Lab 3: Collision Detection");
//	camera = new Camera(window->GetWindow());
//	//objectBuffer = new ObjectBuffer(36);
//
//	speed = 0.001f;
//	set = false;
//
//	forcePoint = glm::vec3(1,1,0);
//	forceEnabled = false;
//
//	pauseScene = false;
//
//	mode = NONE;
//}
//
//void Lab3::init(char** argv)
//{
//	//Magick::InitializeMagick(*argv);
//
//	initShaders();
//	initModels();
//	initTextures();
//	initTweakBar();
//}
//
//void Lab3::initShaders()
//{
//	basicShader->initShader(Shader::BASIC);
//	possibleShaders[Shader::BASIC] = basicShader->GetProgramID();
//
//	textureShader->initShader(textureShader->TEXTURED);
//	possibleShaders[Shader::TEXTURED]  = textureShader->GetProgramID();
//}
//
//void Lab3::initTweakBar()
//{
//	TwInit(TW_OPENGL_CORE, NULL);
//	TwWindowSize(window->GetWidth(), window->GetHeight());
//
//	bar = TwNewBar("Attributes: ");
//
//
//	TwEnumVal modeEV[] = { {NONE, "None"}, {SPHERE, "Sphere"}, {AABB, "AABB"}, {MULTI,"Multi"}};
//	TwType modeType;
//
//	modeType = TwDefineEnum("SeasonType", modeEV, 4);
//
//	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, " group='Control' label='Pause: '");
//	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
//	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");
//	TwAddVarRW(bar, "B Cube Pos", TW_TYPE_DIR3F, &rigidBodies[0]->position, " group='Bounding' label='Bound P: '");
//	TwAddVarRW(bar, "B Cube Rot", TW_TYPE_QUAT4F, &rigidBodies[0]->orientation, " group='Bounding' label='Bound R: '");
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
//void Lab3::initModels()
//{
//	plane = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/plane.obj");
//	plane->IsTextureActive(true);
//	plane->SetPos(glm::vec3(0,-2,0));
//	plane->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//	plane->SetScale(glm::vec3(1,1,1));
//	plane->SetPossibleShaders(possibleShaders);
//
//	for(int i = 0; i < MAX; i++)
//	{
//		rigidBodies[i] = new RigidBody();
//
//		cubes[i] = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/cube.obj");
//		cubes[i]->IsTextureActive(true);
//		cubes[i]->SetPos(glm::vec3(RandomNumber(-6,6),RandomNumber(-6,6),RandomNumber(-6,6)));
//		cubes[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//		cubes[i]->SetScale(glm::vec3(0.5,0.5,0.5));
//		cubes[i]->SetPossibleShaders(possibleShaders);
//
//		rigidBodies[i]->setVertices(cubes[i]->GetVertices(),cubes[i]->GetPoints());
//		rigidBodies[i]->position = cubes[i]->GetPos();
//		rigidBodies[i]->orientation = cubes[i]->GetOrientation();
//		rigidBodies[i]->scale = cubes[i]->GetScale();
//		rigidBodies[i]->transVertices(rigidBodies[i]->orientation,rigidBodies[i]->position);
//
//		rigidBodies[i]->getCentreOfMass();
//		rigidBodies[i]->calculateDistanceToCOM();
//
//		spheres[i] = new MeshLoader(basicShader->GetProgramID(),"../Resources/Models/simpleSphere.obj");
//		spheres[i]->SetPos(rigidBodies[i]->centreOfMass);
//		spheres[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//		spheres[i]->SetScale(glm::vec3(rigidBodies[i]->distanceToCOM * cubes[i]->scale));
//
//		boundingCubes[i] = new MeshLoader(basicShader->GetProgramID(),"../Resources/Models/cube.obj");
//		boundingCubes[i]->SetPos(rigidBodies[i]->centreOfMass);
//		boundingCubes[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//		boundingCubes[i]->SetScale(glm::vec3(cubes[i]->scale));
//	}
//}
//
//void Lab3::initTextures()
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
//void Lab3::run()
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
//
//		update();
//
//		TwDraw();
//
//		window->SwapBuffers();
//	}
//}
//
//void Lab3::update()
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
//	else if(mode == AABB)
//	{
//		for(int i = 0; i < MAX; i++)
//		{
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
//				rigidBodies[i]->applyForce(glm::vec3(RandomNumber(-0.3, 0.3), RandomNumber(-0.3, 0.3),RandomNumber(-0.3, 0.3)),glm::vec3(RandomNumber(-0.1, 0.1), RandomNumber(-0.1, 0.1),RandomNumber(-0.1, 0.1)));
//			}
//		}
//
//		rigidBodies[i]->enableGravity();
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
//void Lab3::checkForSphereCollision()
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
//						spheres[i]->SetColor(glm::vec3(1,0,0));
//						spheres[j]->SetColor(glm::vec3(1,0,0));
//
//						//collidingPairs.push_back(std::make_pair(rigidBodies[i],rigidBodies[j]));
//					}
//				}
//			}
//		}
//	}
//
//}
//
//
//void Lab3::checkForBoundingBoxCollison()
//{
//	if(mode == AABB)
//	{
//		for(int i = 0; i < MAX; i++)
//		{
//			for(int j = 0; j < MAX; j++)
//			{
//				if(i != j)
//				{
//					//check the X axis
//					if(glm::length(boundingCubes[i]->position.x - boundingCubes[j]->position.x) < boundingCubes[i]->scale.x + boundingCubes[j]->scale.x)
//					{
//						//check the Y axis
//
//						if(glm::length(boundingCubes[i]->position.y - boundingCubes[j]->position.y) < boundingCubes[i]->scale.y + boundingCubes[j]->scale.y)
//						{
//							//check the Z axis
//
//							if(glm::length(boundingCubes[i]->position.z - boundingCubes[j]->position.z) < boundingCubes[i]->scale.z + boundingCubes[j]->scale.z)
//							{
//								boundingCubes[i]->SetColor(glm::vec3(1,0,0));
//								boundingCubes[j]->SetColor(glm::vec3(1,0,0));
//
//								//collidingPairs.push_back(std::make_pair(rigidBodies[i],rigidBodies[j]));
//							}
//						}
//					}
//				}
//			}
//		}
//
//	}
//	
//}
//
//// Based on real-time collision detection book - Christer Ericson
//
//
//float Lab3::RandomNumber(float Min, float Max)
//{
//	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
//}
//
////void Lab3::sweepAndPrune()
////{
////	axisX.clear();
////	axisY.clear();
////	axisZ.clear();
////
////	for(int i = 0; i < MAX; i++)
////	{
////		float minX = rigidBodies[i]->minExtents.x;
////		float maxX = rigidBodies[i]->maxExtents.x;
////
////		float minY = rigidBodies[i]->minExtents.y;
////		float maxY = rigidBodies[i]->maxExtents.y;
////
////		float minZ = rigidBodies[i]->minExtents.z;
////		float maxZ = rigidBodies[i]->maxExtents.z;
////
////		axisX.push_back(glm::vec2(minX,maxX));
////		axisY.push_back(glm::vec2(minY,maxY));
////		axisZ.push_back(glm::vec2(minZ,maxZ));
////
////	std::sort(axisX.begin(),axisX.end());
////		std::sort(axisY.begin(),axisY.end());
////		std::sort(axisZ.begin(),axisZ.end());
////
////		for (int i=0; i<MAX; i++)
////		{
////			for (int j = i + 1; j<MAX; j++)
////			{
////				/*if (axisX[i].x < axisX[i].x)
////				{
////					break;
////				}*/
////
////				//std::cout << axisX[i].y << std::endl;
////				
////				/*
////				if (((axisX[i].y > axisX[j].x) && (axisX[i].y < axisX[j].y) ) &&
////					((axisY[i].y > axisY[j].x) && (axisY[i].y < axisY[j].y)) && 
////					((axisZ[i].y > axisZ[j].x) && (axisZ[i].y < axisZ[j].y)))
////				{
////					boundingCubes[i]->updateColor(glm::vec3(1,0,0));
////					boundingCubes[j]->updateColor(glm::vec3(1,0,0));
////				}
////				*/
////			}
////		}
////	 }
////}