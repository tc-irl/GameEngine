#include "ClothSimulation.h"

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


ClothSimulation::ClothSimulation(void)
{
	basicShader = new Shader();
	textureShader = new Shader();
	cloth = new Cloth();
	window = new Window(800,600,"Cloth Simulation");
	camera = new Camera(window->GetWindow());

	pauseScene = false;
	
	mode = NONE;
}

void ClothSimulation::init(char **argv)
{
	initShaders();
	initModels();
	initTextures();
	initTweakBar();
}

void ClothSimulation::initShaders()
{
	basicShader->initShader(Shader::BASIC);
	possibleShaders[Shader::BASIC] = basicShader->GetProgramID();

	textureShader->initShader(textureShader->TEXTURED);
	possibleShaders[Shader::TEXTURED] = textureShader->GetProgramID();
}

void ClothSimulation::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

	bar = TwNewBar("Attributes: ");

	//TwEnumVal modeEV[] = { {NONE, "None"}, {SPHERE, "Sphere"}, {AABB, "AABB"}, {AABB_PLANE,"Plane Collision"}, {MULTI,"Multi"}};
	//TwType modeType;

	//modeType = TwDefineEnum("SeasonType", modeEV, 5);

	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, " group='Control' label='Pause: '");
	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");

	//TwAddVarRW(bar, "Mode", modeType, &mode, NULL);

	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
}

void ClothSimulation::initModels()
{
	plane = new MeshLoader(textureShader->GetProgramID(),"../Resources/Models/plane.obj");
	plane->IsTextureActive(true);
	plane->SetPos(glm::vec3(0,0,0));
	plane->SetOrientation(glm::quat(glm::vec3(0,0,0)));
	plane->SetScale(glm::vec3(10,1,10));
	plane->SetPossibleShaders(possibleShaders);
}

void ClothSimulation::initTextures()
{
	plane->SetTexture("../Resources/Textures/tile.png");
	plane->SetShaderType(textureShader->shaderType);
}

float ClothSimulation::RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

void ClothSimulation::run()
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

void ClothSimulation::update()
{
	camera->computeMatricesFromInputs();

	plane->UseProgram();
	plane->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
	plane->Render();
}
