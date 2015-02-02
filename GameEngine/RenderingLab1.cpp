//#include "RenderingLab1.h"
//
//typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
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
//RenderingLab1::RenderingLab1(void)
//{
//	basicShader = new Shader();
//	textureShader = new Shader();
//	toonShader = new Shader();
//	toonTexturedShader = new Shader();
//	phongShader = new Shader();
//	diffuseShader = new Shader();
//
//	window = new Window(800,600,"Lab 1: Rendering");
//	camera = new Camera(window->GetWindow());
//}
//
//void RenderingLab1::init(char** argv)
//{
//	initShaders();
//	initModels();
//	initTextures();
//	initLights();
//	initTweakBar();
//}
//
//void RenderingLab1::initShaders()
//{
//	basicShader->initShader("../Resources/Shaders/objNoTexture");
//	textureShader->initShader("../Resources/Shaders/objWithTexture");
//	toonShader->initShader("../Resources/Shaders/toon");
//	toonTexturedShader->initShader("../Resources/Shaders/toonTextured");
//	diffuseShader->initShader("../Resources/Shaders/diff");
//	phongShader->initShader("../Resources/Shaders/phong");
//
//}
//
//void RenderingLab1::initModels()
//{
//	//cube = new SimpleMesh(basicShader->GetProgramID(), "../Resources/Models/cube.obj", glm::vec3(1.0,0.0,0.0), false);
//	cube = new SimpleMesh(basicShader->GetProgramID(), "../Resources/Models/cube.obj",glm::vec3(1,0,0),false);
//	plane = new SimpleMesh(textureShader->GetProgramID(),"../Resources/Models/plane.obj", true, glm::vec3(0,-2,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
//
//	/*sphere = new Mesh("../Resources/Models/sphere.obj", false, glm::vec3(-3,0,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
//	torus = new Mesh("../Resources/Models/torus.obj", true, glm::vec3(3,-1,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
//	plane = new Mesh("../Resources/Models/plane.obj", true, glm::vec3(0,-2,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
//	pyramid = new Mesh("../Resources/Models/pyramid.obj", false, glm::vec3(-3,3,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
//	monkey = new Mesh("../Resources/Models/monkey.obj", false, glm::vec3(-1,2,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
//	goldenMonkey = new Mesh("../Resources/Models/monkey.obj", true, glm::vec3(3,2,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
//	teapot = new Mesh("../Resources/Models/teapot.obj", true, glm::vec3(0,7,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));*/
//}
//
//void RenderingLab1::initTextures()
//{
//	cube->initTexture("../Resources/Textures/wood.png");
//	plane->initTexture("../Resources/Textures/tile.png");
//	//sphere->initTexture("../Resources/Textures/green.jpg",textureShader->GetProgramID());
//	//torus->initTexture("../Resources/Textures/green.jpg");
//	//pyramid->initTexture("../Resources/Textures/test.png",basicShader->GetProgramID());
//	//monkey->initTexture("../Resources/Textures/orange.jpg",textureShader->GetProgramID());
//	//goldenMonkey->initTexture("../Resources/Textures/gold.png");
//	//teapot->initTexture("../Resources/Textures/white.jpg");
//}
//
//void RenderingLab1::initLights()
//{
//	toonLightDirection = glm::vec3(0,0,-1);
//
//	ambientIntensity = 0.2f;
//	directionalIntensity = 0.6f;
//	specularIntensity = 0.8f;
//	specularShininess = 64.0f;
//
//	directionalLightColor = glm::vec3(1,1,1);
//	ambientColor = glm::vec3(1,1,1);
//	specularColor = glm::vec3(1,1,1);
//
//	direction = glm::vec3(0, 0, -1);
//
//	light = new Lighting(toonShader->GetProgramID(), glm::vec3(0,0,-1),glm::quat(glm::vec3(0,0,0)));
//	diffuseLight = new Lighting(diffuseShader->GetProgramID(), glm::vec3(0,0,-1),glm::quat(glm::vec3(0,0,0)));
//	phongLight = new Lighting(phongShader->GetProgramID(), glm::vec3(0,0,-1),glm::quat(glm::vec3(0,0,0)));
//}
//
//void RenderingLab1::initTweakBar()
//{
//	TwInit(TW_OPENGL_CORE, NULL);
//	TwWindowSize(window->GetWidth(), window->GetHeight());
//
//	bar = TwNewBar("Attributes: ");
//
//	//TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
//	//TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");
//	//TwAddVarRW(bar, "Plane Position", TW_TYPE_DIR3F, &plane->position, " group='Plane' label='Plane Pos: '");
//	//TwAddVarRW(bar, "Plane Rot", TW_TYPE_QUAT4F, &plane->orientation, " group='Plane' label='Plane Rot: '");
//	//TwAddVarRW(bar, "Cube Position", TW_TYPE_DIR3F, &cube->position, " group='Cube' label='Cube Pos: '");
//	//TwAddVarRW(bar, "Cube Rot", TW_TYPE_QUAT4F, &cube->orientation, " group='Cube' label='Cube Rot: '");
//	//TwAddVarRW(bar, "Torus Position", TW_TYPE_DIR3F, &torus->position, " group='Torus' label='Torus Pos: '");
//	//TwAddVarRW(bar, "Torus Rot", TW_TYPE_QUAT4F, &torus->orientation, " group='Torus' label='Torus Rot: '");
//	//TwAddVarRW(bar, "Monkey Position", TW_TYPE_DIR3F, &monkey->position, " group='Monkey' label='Monkey Pos: '");
//	//TwAddVarRW(bar, "Monkey Rot", TW_TYPE_QUAT4F, &monkey->orientation, " group='Monkey' label='Monkey Rot: '");
//	//TwAddVarRW(bar, "Pyramid Position", TW_TYPE_DIR3F, &pyramid->position, " group='Pyramid' label='Pyramid Pos: '");
//	//TwAddVarRW(bar, "Pyramid Rot", TW_TYPE_QUAT4F, &pyramid->orientation, " group='Pyramid' label='Pyramid Rot: '");
//	//TwAddVarRW(bar, "Sphere Position", TW_TYPE_DIR3F, &sphere->position, " group='Sphere' label='Sphere Pos: '");
//	//TwAddVarRW(bar, "Sphere Rot", TW_TYPE_QUAT4F, &sphere->orientation, " group='Sphere' label='Sphere Rot: '");
//	//TwAddVarRW(bar, "Golden M Position", TW_TYPE_DIR3F, &goldenMonkey->position, " group='Golden M' label='Golden M  Pos: '");
//	//TwAddVarRW(bar, "Golden M Rot", TW_TYPE_QUAT4F, &goldenMonkey->orientation, " group='Golden M' label='Golden M  Rot: '");
//	//TwAddVarRW(bar, "Teapot Position", TW_TYPE_DIR3F, &teapot->position, " group='Teapot' label='Teapot Position: '");
//	//TwAddVarRW(bar, "Teapot Rot", TW_TYPE_QUAT4F, &teapot->orientation, " group='Teapot' label='Teapot Rot: '");
//	//TwAddVarRW(bar, "Light Position", TW_TYPE_DIR3F, &toonLightDirection, " group='Lighting' label='Light Direction: '");
//	//TwAddVarRW(bar, "Phong Light Position", TW_TYPE_DIR3F, &direction, " group='Lighting' label='Phong Light Direction: '");
//	//TwAddVarRW(bar, "Diffuse Color", TW_TYPE_COLOR3F, &directionalLightColor, " group='Diffuse Lighting' label='Diffuse Color: '");
//	//TwAddVarRW(bar, "Diffuse Intensity", TW_TYPE_FLOAT, &directionalIntensity, "step='0.1' group='Diffuse Lighting' label='Diffuse Intensity: '");
//	//TwAddVarRW(bar, "Ambient Color", TW_TYPE_COLOR3F, &ambientColor, " group='Ambient Lighting' label='Ambient Color: '");
//	//TwAddVarRW(bar, "Ambient Intensity", TW_TYPE_FLOAT, &ambientIntensity, "step='0.1' group='Ambient Lighting' label='Ambient Intensity: '");
//	//TwAddVarRW(bar, "Specular Color", TW_TYPE_COLOR3F, &specularColor, " group='Specular Lighting' label='Specular Color: '");
//	//TwAddVarRW(bar, "Specular Intensity", TW_TYPE_FLOAT, &specularIntensity, "step='0.1' group='Specular Lighting' label='Specular Intensity: '");
//	//TwAddVarRW(bar, "Specular Shininess", TW_TYPE_FLOAT, &specularShininess, "step='0.1' group='Specular Lighting' label='Specular Shininess: '");
//
//	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
//	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
//	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
//	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
//}
//
//void RenderingLab1::run()
//{
//	double prevTime = glfwGetTime(); 
//
//	while(!window->IsCloseRequested())
//	{
//		time = glfwGetTime();
//		dt = time - prevTime;
//		prevTime = time;
//
//		window->Update();
//
//		camera->getProjectionMatrix();
//		camera->getViewMatrix();
//
//		update();
//
//		TwDraw();
//
//		window->SwapBuffers();
//	}
//}
//
//void RenderingLab1::update()
//{
//	/* Basic Shader */
//	camera->computeMatricesFromInputs();
//
//	basicShader->UseProgram();
//	camera->handleMVP(basicShader->modelLoc,basicShader->viewLoc,basicShader->projLoc);
//
//	cube->update(basicShader->modelLoc, dt);
//	cube->render();
//
//	//pyramid->update(basicShader->modelLoc, dt);
//	//pyramid->render();
//
//	/* Simple Texture Shader */
//
//	textureShader->UseProgram();
//	camera->handleMVP(textureShader->modelLoc,textureShader->viewLoc,textureShader->projLoc);
//
//	plane->update(textureShader->modelLoc,dt);
//	plane->render();
//
//
//	/* Toon Shader */
//	
//	//toonShader->UseProgram();
//	//camera->handleMVP(toonShader->modelLoc,toonShader->viewLoc,toonShader->projLoc);
//
//	//monkey->update(toonShader->modelLoc, dt);
//	//monkey->render();
//
//	//light->update(toonShader->GetProgramID(),dt);
//	//light->setToonLight(toonLightDirection);
//
//	/* Toon Textured */
//
//	//toonTexturedShader->UseProgram();
//	//camera->handleMVP(toonTexturedShader->modelLoc,toonTexturedShader->viewLoc,toonTexturedShader->projLoc);
//
//	//torus->update(toonTexturedShader->modelLoc, dt);
//	//torus->render();
//
//	//teapot->update(toonTexturedShader->modelLoc, dt);
//	//teapot->render();
//
//	//light->update(toonTexturedShader->GetProgramID(),dt);
//	//light->setToonLight(toonLightDirection);
//
//	///* Diffuse shader */
//
//	//diffuseShader->UseProgram();
//	//camera->handleMVP(diffuseShader->modelLoc,diffuseShader->viewLoc,diffuseShader->projLoc);
//
//	//sphere->update(diffuseShader->modelLoc, dt);
//	//sphere->render();
//
//	//diffuseLight->update(diffuseShader->GetProgramID(),dt);
//
//	//diffuseLight->setDirectionalLight(direction, directionalLightColor, directionalIntensity);
//	//diffuseLight->setAmbientLight(ambientColor, ambientIntensity);
//
//	///* Phong shader */
//	//phongShader->UseProgram();
//	//camera->handleMVP(phongShader->modelLoc,phongShader->viewLoc,phongShader->projLoc);
//
//	//goldenMonkey->update(phongShader->modelLoc, dt);
//	//goldenMonkey->render();
//
//	//phongLight->update(phongShader->GetProgramID(),dt);
//
//	//phongLight->setDirectionalLight(direction, directionalLightColor, directionalIntensity);
//	//phongLight->setAmbientLight(ambientColor, ambientIntensity);
//	//phongLight->setSpecularLight(specularColor,specularIntensity,specularShininess);
//}
//
//
