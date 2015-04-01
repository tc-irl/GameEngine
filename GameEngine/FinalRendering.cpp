#include "FinalRendering.h"

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


FinalRendering::FinalRendering(void)
{
	skyBoxShader = new Shader();
	texturedShader = new Shader();
	normalMapShader = new Shader();
	reliefMapShader = new Shader();
	pomShader = new Shader();
	pomShadowShader = new Shader();

	window = new Window(800,600,"Lab 3: Rendering");
	camera = new Camera(window->GetWindow());

	pauseScene = false;
}

void FinalRendering::init(char** argv)
{
	initShaders();
	initRefractions();
	initModels();
	initTextures();
	initLights();
	initTweakBar();
}

void FinalRendering::initShaders()
{
	skyBoxShader->initShader(skyBoxShader->SKYBOX);
	possibleShaders[Shader::SKYBOX] = skyBoxShader->GetProgramID();

	texturedShader->initShader(texturedShader->TEXTURED);
	possibleShaders[Shader::TEXTURED] = texturedShader->GetProgramID();

	normalMapShader->initShader(normalMapShader->NORMAL_MAP);
	possibleShaders[Shader::NORMAL_MAP] = normalMapShader->GetProgramID();

	pomShader->initShader(pomShader->POM);
	possibleShaders[Shader::POM] = pomShader->GetProgramID();

	reliefMapShader->initShader(reliefMapShader->RELIEF_MAP);
	possibleShaders[Shader::RELIEF_MAP] = reliefMapShader->GetProgramID();
}

void FinalRendering::initModels()
{
	plane = new MeshLoader(texturedShader->GetProgramID(),"../Resources/Models/plane.obj");
	plane->IsTextureActive(true);
	plane->SetPos(glm::vec3(0,0,0));
	plane->SetOrientation(glm::quat(glm::vec3(0,0,0)));
	plane->SetScale(glm::vec3(10,1,10));
	plane->SetPossibleShaders(possibleShaders);

	cube = new MeshLoader(normalMapShader->GetProgramID(), "../Resources/Models/newCube.obj");
	cube->IsTextureActive(true);
	cube->SetPos(glm::vec3(-3,3,0));
	cube->SetScale(glm::vec3(0.8,0.8,0.8));
	cube->SetPossibleShaders(possibleShaders);
	cube->SetRefractionTypes(refractions);

	//skybox = new Skybox(skyBoxShader->GetProgramID(),"../Resources/Models/cube.obj");
	//skybox->SetPos(camera->position);
	//skybox->SetScale(glm::vec3(100,100,100));
	//skybox->SetPossibleShaders(possibleShaders);
}

void FinalRendering::initTextures()
{
	plane->SetTexture("../Resources/Textures/tile.png");
	plane->SetShaderType(texturedShader->shaderType);

	cube->SetColor(glm::vec3(1,1,1));
	cube->SetShaderType(normalMapShader->shaderType);
	cube->SetTexture("../Resources/Textures/wood2.png");
	cube->SetNormalTexture("../Resources/Textures/wood_relief.png");
	cube->SetHeightTexture("../Resources/Textures/wood_height.png");

	//skybox->SetShaderType(skyBoxShader->shaderType);
	//skybox->SetCubeMapTexture("../Resources/Skyboxes/Colloseum/");
}

void FinalRendering::initLights()
{
	cubeLight = new Lighting(normalMapShader->GetProgramID());
}

void FinalRendering::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

	bar = TwNewBar("Attributes: ");

	TwEnumVal modeEV[] = {{Shader::ShaderType::SKYBOX, "Skybox"}, {Shader::ShaderType::TEXTURED, "Textured"},
	{Shader::ShaderType::NORMAL_MAP, "Normal Map"}, {Shader::ShaderType::POM, "POM"},{Shader::ShaderType::POM_SHADOW, "POM w/ Shadow"}, 
	{Shader::ShaderType::RELIEF_MAP, "Relief Map"}, {Shader::ShaderType::RELIEF_MAP_SHADOW, "Relief Map w/ Shadow"}};
	

	//TwEnumVal modeEV2[] = {{MeshLoader::RefractionIndex::AIRTOWATER, "Air to Water"}, {MeshLoader::RefractionIndex::AIRTOICE, "Air to Ice"},
	//{MeshLoader::RefractionIndex::AIRTOGLASS, "Air to Glass"}, {MeshLoader::RefractionIndex::AIRTODIAMOND, "Air to Diamond"}};

	TwType modeType;
	//TwType modeType2;

	modeType = TwDefineEnum("ShaderType", modeEV, 7);
	//modeType2 = TwDefineEnum("RefractionType",modeEV2, 4);

	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, "label='Pause Scene: '");
	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");

	TwAddVarRW(bar, "Cube Position", TW_TYPE_DIR3F, &cube->position, " group='Cube' label='Cube Pos: '");
	TwAddVarRW(bar, "Cube Rot", TW_TYPE_QUAT4F, &cube->orientation, " group='Cube' label='Cube Rot: '");
	TwAddVarRW(bar, "Cube Scale", TW_TYPE_DIR3F, &cube->scale, " group='Cube' label='Cube Scale: '");
	TwAddVarRW(bar, "Cube Mode", modeType, &cube->shaderType,"group='Cube' label='Cube Mode: '");
	TwAddVarRW(bar, "Cube Texture", TW_TYPE_BOOLCPP, &cube->useTexture, " group='Cube' label='Cube Textured: '");
	TwAddVarRW(bar, "Cube Normal", TW_TYPE_BOOLCPP, &cube->useNormalTexture, " group='Cube' label='Cube Normal: '");
	TwAddVarRW(bar, "Cube Height", TW_TYPE_BOOLCPP, &cube->useHeightTexture, " group='Cube' label='Cube Height: '");
	TwAddVarRW(bar, "Parallax Scale", TW_TYPE_FLOAT, &cubeLight->parallaxScale, " step='0.01'group='Parallax Settings' label='Parallax Scale: '");
	TwAddVarRW(bar, "Parallax Bias", TW_TYPE_FLOAT, &cubeLight->bias, " step='0.01'group='Parallax Settings' label='Bias: '");
	TwAddVarRW(bar, "Cube Light Position", TW_TYPE_DIR3F, &cubeLight->position, " group='Cube Lighting' label='Light Direction: '");
	TwAddVarRW(bar, "Cube Diffuse Color", TW_TYPE_COLOR3F, &cubeLight->diffuseColor, " group='Cube Lighting' label='Diffuse Color: '");
	TwAddVarRW(bar, "Cube Diffuse Intensity", TW_TYPE_FLOAT, &cubeLight->diffuseIntensity, "step='0.1' group='Cube Lighting' label='Diffuse Intensity: '");
	TwAddVarRW(bar, "Cube Ambient Color", TW_TYPE_COLOR3F, &cubeLight->ambientColor, " group='Cube Lighting' label='Ambient Color: '");
	TwAddVarRW(bar, "Cube Ambient Intensity", TW_TYPE_FLOAT, &cubeLight->ambientIntensity, "step='0.1' group='Cube Lighting' label='Ambient Intensity: '");
	TwAddVarRW(bar, "Cube Specular Color", TW_TYPE_COLOR3F, &cubeLight->specularColor, " group='Cube Lighting' label='Specular Color: '");
	TwAddVarRW(bar, "Cube Specular Intensity", TW_TYPE_FLOAT, &cubeLight->specularIntensity, "step='0.1' group='Cube Lighting' label='Specular Intensity: '");
	TwAddVarRW(bar, "Cube Specular Shininess", TW_TYPE_FLOAT, &cubeLight->specularShininess, "step='0.1' group='Cube Lighting' label='Specular Shininess: '");
	TwAddVarRW(bar, "Cube Material Roughness", TW_TYPE_FLOAT, &cubeLight->roughness, "step='0.1' group='Cube Lighting' label='Roughness: '");

	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
}

void FinalRendering::run()
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

void FinalRendering::update()
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	camera->computeMatricesFromInputs();

	plane->UseProgram();
	plane->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
	plane->Render();

	cube->Rotate360(dt);
	cube->UpdateShader();
	UpdateLighting(cube->GetShader(), cubeLight);
	cube->UpdateRefractionIndex();
	cube->UpdateRefractionIndexRGB();
	cube->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
	cube->Render();
	
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(false);

	//skybox->UseProgram();
	//skybox->cameraPos = camera->position;
	//skybox->UpdateShader();
	//skybox->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
	//skybox->RenderCubeMap();

	glDepthMask(true);
	glCullFace(OldCullFaceMode); 
	glDepthFunc(OldDepthFuncMode);
}

void FinalRendering::UpdateLighting(GLuint shaderID, Lighting *light)
{
	light->SetShader(shaderID);
	light->SetEye(camera->eye);
	light->SetCameraPosition(camera->position);
	light->SetEyeDir();
	light->SetToonLight();
	light->SetDirectionalLight();
	light->SetAmbientLight();
	light->SetSpecularLight();
	light->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
}

void FinalRendering::initRefractions()
{
	refractions[MeshLoader::AIRTOWATER] = 1.0 / 1.33;
	refractions[MeshLoader::AIRTOICE] = 1.0 / 1.309;
	refractions[MeshLoader::AIRTOGLASS] = 1.0 / 1.52;
	refractions[MeshLoader::AIRTODIAMOND] = 1.0 / 2.42;
}
