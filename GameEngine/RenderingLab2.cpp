#include "RenderingLab2.h"

typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);

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


RenderingLab2::RenderingLab2(void)
{
	skyBoxShader = new Shader();
	reflectionShader = new Shader();
	refractionShader = new Shader();
	combinedShader = new Shader();

	window = new Window(800,600,"Lab 2: Rendering");
	camera = new Camera(window->GetWindow());

	pauseScene = false;


}

void RenderingLab2::init(char** argv)
{
	initShaders();
	initRefractions();
	initModels();
	initTextures();
	//initRefractions();
	initLights();
	initTweakBar();
}

void RenderingLab2::initShaders()
{
	skyBoxShader->initShader(skyBoxShader->SKYBOX);
	possibleShaders[Shader::SKYBOX] = skyBoxShader->GetProgramID();

	reflectionShader->initShader(reflectionShader->REFLECTION);
	possibleShaders[Shader::REFLECTION] = reflectionShader->GetProgramID();

	refractionShader->initShader(refractionShader->REFRACTION);
	possibleShaders[Shader::REFRACTION] = refractionShader->GetProgramID();

	combinedShader->initShader(combinedShader->COMBINED);
	possibleShaders[Shader::COMBINED] = combinedShader->GetProgramID();
}

void RenderingLab2::initModels()
{
	teapot = new MeshLoader(skyBoxShader->GetProgramID(), "../Resources/Models/cube.obj");
	teapot->IsSkyboxActive(false);
	teapot->SetPos(glm::vec3(0,0,0));
	teapot->SetScale(glm::vec3(0.8,0.8,0.8));
	teapot->SetPossibleShaders(possibleShaders);
	teapot->SetRefractionTypes(refractions);

	skybox = new MeshLoader(skyBoxShader->GetProgramID(),"../Resources/Models/cube.obj");
	skybox->IsSkyboxActive(true);
	skybox->SetPos(camera->position);
	skybox->SetScale(glm::vec3(100,100,100));
	skybox->SetPossibleShaders(possibleShaders);
}

void RenderingLab2::initTextures()
{
	teapot->SetColor(glm::vec3(1,1,1));
	teapot->SetShaderType(skyBoxShader->shaderType);
	teapot->SetTexture("../Resources/Textures/bricks.jpg");

	skybox->SetShaderType(skyBoxShader->shaderType);
	skybox->SetCubeMapTexture("../Resources/Skyboxes/Sky1/");
}

void RenderingLab2::initLights()
{
	//monkeyLight = new Lighting(orenNayar->GetProgramID());
	//teapotLight = new Lighting(toonShader->GetProgramID());
	//headLight = new Lighting(phongShader->GetProgramID());
}

void RenderingLab2::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

	bar = TwNewBar("Attributes: ");

	TwEnumVal modeEV[] = {{Shader::ShaderType::SKYBOX, "Skybox"}, {Shader::ShaderType::REFLECTION, "Reflection"},
	{Shader::ShaderType::REFRACTION, "Refraction"}, {Shader::ShaderType::COMBINED, "Combined"}};

	TwEnumVal modeEV2[] = {{MeshLoader::RefractionIndex::AIRTOWATER, "Air to Water"}, {MeshLoader::RefractionIndex::AIRTOICE, "Air to Ice"},
	{MeshLoader::RefractionIndex::AIRTOGLASS, "Air to Glass"}, {MeshLoader::RefractionIndex::AIRTODIAMOND, "Air to Diamond"}};

	TwType modeType;
	TwType modeType2;

	modeType = TwDefineEnum("ShaderType", modeEV, 4);
	modeType2 = TwDefineEnum("RefractionType",modeEV2, 4);

	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, "label='Pause Scene: '");
	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");

	TwAddVarRW(bar, "Teapot Position", TW_TYPE_DIR3F, &teapot->position, " group='Teapot' label='Teapot Pos: '");
	TwAddVarRW(bar, "Teapot Rot", TW_TYPE_QUAT4F, &teapot->orientation, " group='Teapot' label='Teapot Rot: '");
	TwAddVarRW(bar, "Teapot Mode", modeType, &teapot->shaderType,"group='Teapot' label='Teapot Mode: '");
	TwAddVarRW(bar, "Teapot Texture", TW_TYPE_BOOLCPP, &teapot->useTexture, " group='Teapot' label='Teapot Textured: '");
	TwAddVarRW(bar, "Teapot Mat Col", TW_TYPE_COLOR4F, &teapot->matColor,"group='Teapot' label='Teapot Color: '");
	TwAddVarRW(bar, "Teapot Ref Factor", TW_TYPE_FLOAT, &teapot->reflectionFactor, "step='0.1' group='Teapot' label='Reflection Factor: '");
	TwAddVarRW(bar, "Teapot RI", modeType2, &teapot->refractiveIndex,"group='Teapot' label='Refraction Mode: '");

	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
}

void RenderingLab2::run()
{
	double prevTime = glfwGetTime(); 

	while(!window->IsCloseRequested())
	{
		time = glfwGetTime();
		dt = time - prevTime;
		prevTime = time;

		if(pauseScene)
		{
			dt = 0;
		}
		window->Update();

		update();

		TwDraw();

		window->SwapBuffers();
	}
}

void RenderingLab2::update()
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	camera->computeMatricesFromInputs();


	teapot->Rotate360(dt);
	teapot->UpdateShader();
	teapot->UpdateRefractionIndex();
	teapot->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
	teapot->Render();

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(false);

	skybox->UseProgram();
	skybox->cameraPos = camera->position;
	skybox->UpdateShader();
	skybox->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
	skybox->RenderCubeMap();

	glDepthMask(true);
	glCullFace(OldCullFaceMode); 
    glDepthFunc(OldDepthFuncMode);
}

void RenderingLab2::UpdateLighting(GLuint shaderID, Lighting *light)
{
	light->SetEye(camera->eye);
	light->SetShader(shaderID);
	light->SetEyeDir();
	light->SetToonLight();
	light->SetDirectionalLight();
	light->SetAmbientLight();
	light->SetSpecularLight();
	light->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
}

void RenderingLab2::initRefractions()
{
	refractions[MeshLoader::AIRTOWATER] = 1.0 / 1.33;
	refractions[MeshLoader::AIRTOICE] = 1.0 / 1.309;
	refractions[MeshLoader::AIRTOGLASS] = 1.0 / 1.52;
	refractions[MeshLoader::AIRTODIAMOND] = 1.0 / 2.42;
}
