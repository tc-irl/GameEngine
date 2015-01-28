#include "RenderingLab1.h"

//typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
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


RenderingLab1::RenderingLab1(void)
{
	shader1 = new Shader();
	shader2 = new Shader();
	window = new Window(800,600,"Lab 1: Rendering");
	camera = new Camera(window->GetWindow());
}

void RenderingLab1::init(char** argv)
{

	Magick::InitializeMagick(*argv);

	initShaders();
	initModels();
	initTextures();
	initTweakBar();
}

void RenderingLab1::initShaders()
{
	shader1->CreateProgram();

	shader1->LoadFile("../Resources/Shaders/objWithTexture.vs",vs);
	shader1->AddShader(shader1->GetProgramID(), vs.c_str(), GL_VERTEX_SHADER);

	shader1->LoadFile("../Resources/Shaders/objWithTexture.ps",ps);
	shader1->AddShader(shader1->GetProgramID(), ps.c_str(), GL_FRAGMENT_SHADER);

	shader1->LinkProgram();
	shader1->ValidateProgram();
	shader1->UseProgram();

	shader2->CreateProgram();

	shader2->LoadFile("../Resources/Shaders/objNoTexture.vs",vs);
	shader2->AddShader(shader2->GetProgramID(), vs.c_str(), GL_VERTEX_SHADER);

	shader2->LoadFile("../Resources/Shaders/objNoTexture.ps",ps);
	shader2->AddShader(shader2->GetProgramID(), ps.c_str(), GL_FRAGMENT_SHADER);

	shader2->LinkProgram();
	shader2->ValidateProgram();
	shader2->UseProgram();

	shader1->setMVPUniforms();
	shader2->setMVPUniforms();
}

void RenderingLab1::initModels()
{
	cube = new Mesh("../Resources/Models/cube.obj", true);
	sphere = new Mesh("../Resources/Models/sphere.obj", true, glm::vec3(-3,0,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
	torus = new Mesh("../Resources/Models/torus.obj", true, glm::vec3(3,-1,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
	plane = new Mesh("../Resources/Models/plane.obj", true, glm::vec3(0,-2,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
	pyramid = new Mesh("../Resources/Models/pyramid.obj", true, glm::vec3(-3,3,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
	monkey = new Mesh("../Resources/Models/monkey.obj", true, glm::vec3(0,2,0), glm::quat(glm::vec3(0,0,0)), glm::vec3(1,1,1));
}

void RenderingLab1::initTextures()
{
	cube->initTexture("../Resources/Textures/wood.png",shader1->GetProgramID());
	plane->initTexture("../Resources/Textures/tile.png",shader1->GetProgramID());
	sphere->initTexture("../Resources/Textures/green.jpg",shader1->GetProgramID());
	torus->initTexture("../Resources/Textures/grey.jpg",shader1->GetProgramID());
	pyramid->initTexture("../Resources/Textures/test.png",shader2->GetProgramID());
	monkey->initTexture("../Resources/Textures/orange.jpg",shader1->GetProgramID());

}

void RenderingLab1::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

	bar = TwNewBar("Attributes: ");

	TwAddVarRW(bar, "Camera", TW_TYPE_DIR3F, &camera->position, " label='Camera Pos: '");
	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " label='Camera Rot: '");
	TwAddVarRW(bar, "Plane Position", TW_TYPE_DIR3F, &plane->position, " label='Plane Pos: '");
	TwAddVarRW(bar, "Plane Rot", TW_TYPE_QUAT4F, &plane->orientation, " label='Plane Rot: '");
	TwAddVarRW(bar, "Cube Position", TW_TYPE_DIR3F, &cube->position, " label='Cube Pos: '");
	TwAddVarRW(bar, "Cube Rot", TW_TYPE_QUAT4F, &cube->orientation, " label='Cube Rot: '");

	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
}

void RenderingLab1::run()
{
	double prevTime = glfwGetTime(); 

	while(!window->IsCloseRequested())
	{

		time = glfwGetTime();
		dt = time - prevTime;
		prevTime = time;

		window->Update();

		camera->getProjectionMatrix();
		camera->getViewMatrix();

		update();

		TwDraw();

		window->SwapBuffers();
	}
}

void RenderingLab1::update()
{
	camera->computeMatricesFromInputs();

	shader1->UseProgram();
	camera->handleMVP(shader1->modelLoc,shader1->viewLoc,shader1->projLoc);
	
	plane->update(shader1->modelLoc,dt);
	plane->render();

	cube->update(shader1->modelLoc, dt);
	cube->render();

	sphere->update(shader1->modelLoc, dt);
	sphere->render();

	torus->update(shader1->modelLoc, dt);
	torus->render();

	monkey->update(shader1->modelLoc, dt);
	monkey->render();

	shader2->UseProgram();
	camera->handleMVP(shader2->modelLoc,shader2->viewLoc,shader2->projLoc);

	pyramid->update(shader2->modelLoc, dt);
	pyramid->render();
}
