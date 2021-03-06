//#include "ClothSimulation.h"
//
///************************************************************************/
///* Tutorial used: http://cg.alexandra.dk/?p=147                          */
///************************************************************************/
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
//
//ClothSimulation::ClothSimulation(void)
//{
//	basicShader = new Shader();
//	textureShader = new Shader();
//	//cloth = new Cloth(14, 10, 33,27); 
//	cloth = new Cloth(14,10,55,45);
//	window = new Window(800,600,"Cloth Simulation");
//	camera = new Camera(window->GetWindow());
//
//	pauseScene = false;
//	
//	windDirection = glm::vec3(0.5f,0,0.2f);
//	forceDirection = glm::vec3(0,-0.2f,0);
//	cloth->mode = Cloth::CLOTH_HANGING;
//}
//
//void ClothSimulation::init(char **argv)
//{
//	initShaders();
//	initModels();
//	initTextures();
//	initLights();
//	initTweakBar();
//}
//
//void ClothSimulation::initShaders()
//{
//	basicShader->initShader(Shader::BASIC);
//	possibleShaders[Shader::BASIC] = basicShader->GetProgramID();
//
//	textureShader->initShader(textureShader->TEXTURED);
//	possibleShaders[Shader::TEXTURED] = textureShader->GetProgramID();
//}
//
//void ClothSimulation::initTweakBar()
//{
//	TwInit(TW_OPENGL_CORE, NULL);
//	TwWindowSize(window->GetWidth(), window->GetHeight());
//
//	bar = TwNewBar("Attributes: ");
//
//	TwEnumVal modeEV[] = { {Cloth::CLOTH_HANGING, "Cloth Hanging"}, {Cloth::CLOTH_DROPPING, "Cloth Dropping"}};
//	TwType modeType;
//
//	modeType = TwDefineEnum("modeType", modeEV, 2);
//
//	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, " group='Control' label='Pause: '");
//	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
//	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");
//	TwAddVarRW(bar, "Wind Rot", TW_TYPE_DIR3F, &windDirection, " group='Forces' label='Wind Direction: '");
//	TwAddVarRW(bar, "Force Rot", TW_TYPE_DIR3F, &forceDirection, " group='Forces' label='Force Direction: '");
//	TwAddVarRW(bar, "Ball Pos", TW_TYPE_DIR3F, &ball->position, " group='Ball' label='Ball Position: '");
//	TwAddVarRW(bar, "Cloth Pos", TW_TYPE_DIR3F, &cloth->position, " group='Cloth' label='Cloth Position: '");
//	TwAddVarRW(bar, "Cloth Rot", TW_TYPE_DIR3F, &cloth->orientation, " group='Cloth' label='Cloth Rotation: '");
//
//	TwAddVarRW(bar, "Fixed", TW_TYPE_BOOLCPP, &cloth->movable, " group='Cloth' label='Cloth Fixed: '");
//	TwAddVarRW(bar, "Force", TW_TYPE_BOOLCPP, &cloth->forceEnabled, " group='Cloth' label='Force: '");
//	TwAddVarRW(bar, "Wind", TW_TYPE_BOOLCPP, &cloth->windEnabled, " group='Cloth' label='Wind: '");
//	TwAddVarRW(bar, "Collisions", TW_TYPE_BOOLCPP, &cloth->selfCollisionEnabled, " group='Cloth' label='Collisions: '");
//	TwAddVarRW(bar, "Reset", TW_TYPE_BOOLCPP, &cloth->reset, " group='Cloth' label='Reset: '");
//	TwAddVarRW(bar, "Light Position", TW_TYPE_DIR3F, &light->position, " group='Teapot Lighting' label='Light Direction: '");
//	TwAddVarRW(bar, "Light Diffuse Color", TW_TYPE_COLOR3F, &light->diffuseColor, " group='Teapot Lighting' label='Diffuse Color: '");
//	TwAddVarRW(bar, "Light Diffuse Intensity", TW_TYPE_FLOAT, &light->diffuseIntensity, "step='0.1' group='Teapot Lighting' label='Diffuse Intensity: '");
//	TwAddVarRW(bar, "Light Ambient Color", TW_TYPE_COLOR3F, &light->ambientColor, " group='Teapot Lighting' label='Ambient Color: '");
//	TwAddVarRW(bar, "Light Ambient Intensity", TW_TYPE_FLOAT, &light->ambientIntensity, "step='0.1' group='Teapot Lighting' label='Ambient Intensity: '");
//	TwAddVarRW(bar, "Light Specular Color", TW_TYPE_COLOR3F, &light->specularColor, " group='Teapot Lighting' label='Specular Color: '");
//	TwAddVarRW(bar, "Light Specular Intensity", TW_TYPE_FLOAT, &light->specularIntensity, "step='0.1' group='Teapot Lighting' label='Specular Intensity: '");
//	TwAddVarRW(bar, "Light Specular Shininess", TW_TYPE_FLOAT, &light->specularShininess, "step='0.1' group='Teapot Lighting' label='Specular Shininess: '");
//	TwAddVarRW(bar, "Light Material Roughness", TW_TYPE_FLOAT, &light->roughness, "step='0.1' group='Teapot Lighting' label='Roughness: '");
//
//
//	TwAddVarRW(bar, "Mode", modeType, &cloth->mode, NULL);
//
//	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
//	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
//	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
//	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
//}
//
//void ClothSimulation::initModels()
//{
//	plane = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/plane.obj");
//	plane->IsTextureActive(true);
//	plane->SetPos(glm::vec3(0,-14,0));
//	plane->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//	plane->SetScale(glm::vec3(10,1,10));
//	plane->SetPossibleShaders(possibleShaders);
//
//	ball = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/simpleSphere.obj");
//	ball->IsTextureActive(true);
//	ball->SetPos(glm::vec3(-2.85,-5,-0.15));
//	ball->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//	ball->SetScale(glm::vec3(1,1,1));
//	ball->SetPossibleShaders(possibleShaders);
//
//
//	//for(int i = 0; i < MAX; i++)
//	//{
//	//	balls[i] = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/simpleSphere.obj");
//	//	balls[i]->IsTextureActive(true);
//	//	balls[i]->SetPos(glm::vec3(i,-5,5.3));
//	//	balls[i]->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//	//	balls[i]->SetScale(glm::vec3(2,2,2));
//	//	balls[i]->SetPossibleShaders(possibleShaders);
//	//}
//
//	//balls[0]->SetScale(glm::vec3(0.3,0.3,0.3));
//	//balls[1]->SetScale(glm::vec3(0.5,0.5,0.5));
//	//balls[2]->SetScale(glm::vec3(0.75,0.75,0.75));
//	//balls[2]->SetPos(glm::vec3(6.24,-5,4.88));
//	//balls[0]->SetPos(glm::vec3(4.93,-10,0.10));
//	//balls[1]->SetPos(glm::vec3(9.7,-7.44,6.38));
//
//	//clothModel = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/cloth.obj");
//	//clothModel->IsTextureActive(true);
//	//clothModel->SetPos(glm::vec3(0,5,0));
//	//clothModel->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//	//clothModel->SetScale(glm::vec3(1,1,1));
//	//clothModel->SetPossibleShaders(possibleShaders);
//
//	cloth->SetShader(basicShader->GetProgramID());
//	cloth->GenerateBuffer();
//}
//
//void ClothSimulation::initTextures()
//{
//	plane->SetTexture("../Resources/Textures/tile.png");
//	plane->SetShaderType(textureShader->shaderType);
//
//	ball->SetTexture("../Resources/Textures/red.png");
//	ball->SetShaderType(textureShader->shaderType);
//
//	//for(int i = 0; i < MAX; i++)
//	//{
//	//	balls[i]->SetTexture("../Resources/Textures/red.png");
//	//	balls[i]->SetShaderType(textureShader->shaderType);
//	//}
//
//	//ball2->SetTexture("../Resources/Textures/red.png");
//	//ball2->SetShaderType(textureShader->shaderType);
//
//	//ball3->SetTexture("../Resources/Textures/red.png");
//	//ball3->SetShaderType(textureShader->shaderType);
//
//	//ball4->SetTexture("../Resources/Textures/red.png");
//	//ball4->SetShaderType(textureShader->shaderType);
//	//clothModel->SetTexture("../Resources/Textures/beige.png");
//	//clothModel->SetShaderType(textureShader->shaderType);
//}
//
//float ClothSimulation::RandomNumber(float Min, float Max)
//{
//	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
//}
//
//void ClothSimulation::run()
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
//void ClothSimulation::update()
//{s
//	camera->computeMatricesFromInputs();
//
//	basicShader->UseProgram();
//
//	cloth->AddForce(forceDirection * TIME_STEPSIZE2);
//	cloth->AddWind(windDirection * TIME_STEPSIZE2);
//	cloth->TimeStep(); // calculate the particle positions of the next frame
//	cloth->AddPlaneCollision(plane->GetPos());
//	cloth->AddBallCollision(ball->GetPos(), ball->GetScale().x + 0.1);
//	cloth->AddSelfCollision();
//
//	//cloth->AddTearing(ball->GetPos(), ball->GetScale().x);
//
//	UpdateLighting(basicShader->GetProgramID(), light);
//	cloth->Update(camera->getViewMatrix(), camera->getProjectionMatrix());
//	cloth->DrawCloth();
//
//	plane->UseProgram();
//	plane->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	plane->Render();
//
//	ball->UseProgram();
//	ball->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	ball->Render();
//
//	if (glfwGetKey(window->GetWindow(), GLFW_KEY_ENTER ) == GLFW_PRESS)
//	{
//		cloth->movable = true;
//	}
//
//	if (glfwGetKey(window->GetWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
//	{
//		cloth->windEnabled = true;
//	}
//
//	if (glfwGetKey(window->GetWindow(), GLFW_KEY_N) == GLFW_PRESS)
//	{
//		cloth->selfCollisionEnabled = true;
//	}
//
//	if (glfwGetKey(window->GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
//	{
//		ball->position.x -= 0.05;
//	}
//
//	if (glfwGetKey(window->GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
//	{
//		ball->position.x += 0.05;
//	}
//
//	if (glfwGetKey(window->GetWindow(), GLFW_KEY_UP) == GLFW_PRESS)
//	{
//		ball->position.y += 0.05;
//	}
//
//	if (glfwGetKey(window->GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
//	{
//		ball->position.y -= 0.05;
//	}
//
//	if (glfwGetKey(window->GetWindow(), GLFW_KEY_I) == GLFW_PRESS)
//	{
//		ball->position.z -= 0.05;
//	}
//
//	if (glfwGetKey(window->GetWindow(), GLFW_KEY_K) == GLFW_PRESS)
//	{
//		ball->position.zs += 0.05;
//	}
//}
//
//void ClothSimulation::initLights()
//{
//	light = new Lighting(basicShader->GetProgramID());
//}
//
//void ClothSimulation::UpdateLighting(GLuint shaderID, Lighting *light)
//{
//	light->SetEye(camera->eye);
//	light->SetShader(shaderID);
//	light->SetEyeDir();
//	light->SetToonLight();
//	light->SetDirectionalLight();
//	light->SetAmbientLight();
//	light->SetSpecularLight();
//	light->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//}
