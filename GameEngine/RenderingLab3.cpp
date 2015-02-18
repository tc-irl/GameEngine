#include "RenderingLab3.h"

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


RenderingLab3::RenderingLab3(void)
{
	skyBoxShader = new Shader();
	reflectionShader = new Shader();
	refractionShader = new Shader();
	refractionDispShader = new Shader();
	combinedShader = new Shader();
	combinedDispShader = new Shader();
	texturedShader = new Shader();
	normalMapShader = new Shader();
	fresnelShader = new Shader();

	window = new Window(800,600,"Lab 3: Rendering");
	camera = new Camera(window->GetWindow());

	pauseScene = false;
}

void RenderingLab3::init(char** argv)
{
	initShaders();
	initRefractions();
	initModels();
	initTextures();
	initLights();
	initTweakBar();
}

void RenderingLab3::initShaders()
{
	skyBoxShader->initShader(skyBoxShader->SKYBOX);
	possibleShaders[Shader::SKYBOX] = skyBoxShader->GetProgramID();

	reflectionShader->initShader(reflectionShader->REFLECTION);
	possibleShaders[Shader::REFLECTION] = reflectionShader->GetProgramID();

	refractionShader->initShader(refractionShader->REFRACTION);
	possibleShaders[Shader::REFRACTION] = refractionShader->GetProgramID();

	refractionDispShader->initShader(refractionDispShader->REFRACTION_DISP);
	possibleShaders[Shader::REFRACTION_DISP] = refractionDispShader->GetProgramID();

	combinedShader->initShader(combinedShader->COMBINED);
	possibleShaders[Shader::COMBINED] = combinedShader->GetProgramID();

	combinedDispShader->initShader(combinedDispShader->COMBINED_DISP);
	possibleShaders[Shader::COMBINED_DISP] = combinedDispShader->GetProgramID();

	texturedShader->initShader(texturedShader->TEXTURED);
	possibleShaders[Shader::TEXTURED] = texturedShader->GetProgramID();

	normalMapShader->initShader(normalMapShader->NORMAL_MAP);
	possibleShaders[Shader::NORMAL_MAP] = normalMapShader->GetProgramID();

	fresnelShader->initShader(fresnelShader->FRESNEL);
	possibleShaders[Shader::FRESNEL] = fresnelShader->GetProgramID();
}

void RenderingLab3::initModels()
{
	teapot = new MeshLoader(skyBoxShader->GetProgramID(), "../Resources/Models/sphere.obj");
	teapot->IsSkyboxActive(false);
	teapot->SetPos(glm::vec3(0,0,0));
	teapot->SetScale(glm::vec3(0.8,0.8,0.8));
	teapot->SetPossibleShaders(possibleShaders);
	teapot->SetRefractionTypes(refractions);

	cube = new MeshLoader(normalMapShader->GetProgramID(), "../Resources/Models/cube.obj");
	cube->IsSkyboxActive(false);
	cube->IsTextureActive(true);
	cube->SetPos(glm::vec3(-3,0,0));
	cube->SetScale(glm::vec3(0.8,0.8,0.8));
	cube->SetPossibleShaders(possibleShaders);
	cube->SetRefractionTypes(refractions);

	skybox = new MeshLoader(skyBoxShader->GetProgramID(),"../Resources/Models/cube2.obj");
	skybox->IsSkyboxActive(true);
	skybox->SetPos(camera->position);
	skybox->SetScale(glm::vec3(100,100,100));
	skybox->SetPossibleShaders(possibleShaders);
}

void RenderingLab3::initTextures()
{
	teapot->SetColor(glm::vec3(1,1,1));
	teapot->SetShaderType(fresnelShader->shaderType);
	teapot->SetTexture("../Resources/Textures/bricks2.jpg");
	teapot->SetNormalTexture("../Resources/Textures/bricks2_normal.png");

	cube->SetColor(glm::vec3(1,1,1));
	cube->SetShaderType(normalMapShader->shaderType);
	cube->SetTexture("../Resources/Textures/bricks2.jpg");
	cube->SetNormalTexture("../Resources/Textures/bricks2_normal.png");

	//head->SetColor(glm::vec3(1,1,1));
	//head->SetShaderType(texturedShader->shaderType);
	//head->SetTexture("../Resources/Textures/lambertian.jpg");

	skybox->SetShaderType(skyBoxShader->shaderType);
	skybox->SetCubeMapTexture("../Resources/Skyboxes/Colloseum/");
}

void RenderingLab3::initLights()
{
	//monkeyLight = new Lighting(orenNayar->GetProgramID());
	//teapotLight = new Lighting(toonShader->GetProgramID());
	//headLight = new Lighting(phongShader->GetProgramID());
	cubeLight = new Lighting(normalMapShader->GetProgramID());
}

void RenderingLab3::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

	bar = TwNewBar("Attributes: ");

	TwEnumVal modeEV[] = {{Shader::ShaderType::SKYBOX, "Skybox"}, {Shader::ShaderType::REFLECTION, "Reflection"},
	{Shader::ShaderType::REFRACTION, "Refraction"}, {Shader::ShaderType::COMBINED, "Combined"}, {Shader::ShaderType::TEXTURED, "Textured"},{Shader::ShaderType::REFRACTION_DISP, "Refraction Disp"},
	{Shader::ShaderType::COMBINED_DISP, "Combined Disp"}, {Shader::ShaderType::FRESNEL, "Fresnel"}, {Shader::ShaderType::NORMAL_MAP, "Normal Map"}};

	TwEnumVal modeEV2[] = {{MeshLoader::RefractionIndex::AIRTOWATER, "Air to Water"}, {MeshLoader::RefractionIndex::AIRTOICE, "Air to Ice"},
	{MeshLoader::RefractionIndex::AIRTOGLASS, "Air to Glass"}, {MeshLoader::RefractionIndex::AIRTODIAMOND, "Air to Diamond"}};

	TwType modeType;
	TwType modeType2;

	modeType = TwDefineEnum("ShaderType", modeEV, 9);
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
	TwAddVarRW(bar, "Teapot RIR", TW_TYPE_FLOAT, &teapot->ratioR,"group='Teapot' step='0.01' label='Refraction R: '");
	TwAddVarRW(bar, "Teapot RIG", TW_TYPE_FLOAT, &teapot->ratioG,"group='Teapot' step='0.01' label='Refraction G: '");
	TwAddVarRW(bar, "Teapot RIB", TW_TYPE_FLOAT, &teapot->ratioB,"group='Teapot' step='0.01' label='Refraction B: '");

	TwAddVarRW(bar, "Cube Position", TW_TYPE_DIR3F, &cube->position, " group='Cube' label='Cube Pos: '");
	TwAddVarRW(bar, "Cube Rot", TW_TYPE_QUAT4F, &cube->orientation, " group='Cube' label='Cube Rot: '");
	TwAddVarRW(bar, "Cube Mode", modeType, &cube->shaderType,"group='Cube' label='Cube Mode: '");
	TwAddVarRW(bar, "Cube Texture", TW_TYPE_BOOLCPP, &cube->useTexture, " group='Cube' label='Cube Textured: '");
	TwAddVarRW(bar, "Cube Normal", TW_TYPE_BOOLCPP, &cube->useNormalTexture, " group='Cube' label='Cube Textured: '");
	TwAddVarRW(bar, "Cube Light Position", TW_TYPE_DIR3F, &cubeLight->position, " group='Cube Lighting' label='Light Direction: '");
	TwAddVarRW(bar, "Cube Diffuse Color", TW_TYPE_COLOR3F, &cubeLight->diffuseColor, " group='Cube Lighting' label='Diffuse Color: '");
	TwAddVarRW(bar, "Cube Diffuse Intensity", TW_TYPE_FLOAT, &cubeLight->diffuseIntensity, "step='0.1' group='Cube Lighting' label='Diffuse Intensity: '");
	TwAddVarRW(bar, "Cube Ambient Color", TW_TYPE_COLOR3F, &cubeLight->ambientColor, " group='Cube Lighting' label='Ambient Color: '");
	TwAddVarRW(bar, "Cube Ambient Intensity", TW_TYPE_FLOAT, &cubeLight->ambientIntensity, "step='0.1' group='Cube Lighting' label='Ambient Intensity: '");
	TwAddVarRW(bar, "Cube Specular Color", TW_TYPE_COLOR3F, &cubeLight->specularColor, " group='Cube Lighting' label='Specular Color: '");
	TwAddVarRW(bar, "Cube Specular Intensity", TW_TYPE_FLOAT, &cubeLight->specularIntensity, "step='0.1' group='Cube Lighting' label='Specular Intensity: '");
	TwAddVarRW(bar, "Cube Specular Shininess", TW_TYPE_FLOAT, &cubeLight->specularShininess, "step='0.1' group='Cube Lighting' label='Specular Shininess: '");
	TwAddVarRW(bar, "Cube Material Roughness", TW_TYPE_FLOAT, &cubeLight->roughness, "step='0.1' group='Cube Lighting' label='Roughness: '");

	//TwAddVarRW(bar, "Head Position", TW_TYPE_DIR3F, &head->position, " group='Head' label='Head Pos: '");
	//TwAddVarRW(bar, "Head Rot", TW_TYPE_QUAT4F, &head->orientation, " group='Head' label='Head Rot: '");
	//TwAddVarRW(bar, "Head Mode", modeType, &head->shaderType,"group='Head' label='Head Mode: '");
	//TwAddVarRW(bar, "Head Texture", TW_TYPE_BOOLCPP, &head->useTexture, " group='Head' label='Head Textured: '");
	//TwAddVarRW(bar, "Head Mat Col", TW_TYPE_COLOR4F, &head->matColor,"group='Head' label='Head Color: '");
	//TwAddVarRW(bar, "Head Ref Factor", TW_TYPE_FLOAT, &head->reflectionFactor, "step='0.1' group='Head' label='Reflection Factor: '");
	//TwAddVarRW(bar, "Head RI", modeType2, &head->refractiveIndex,"group='Head' label='Refraction Mode: '");

	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
}

void RenderingLab3::run()
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

void RenderingLab3::update()
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	camera->computeMatricesFromInputs();

	teapot->Rotate360(dt);
	teapot->UpdateShader();
	teapot->UpdateRefractionIndex();
	teapot->UpdateRefractionIndexRGB();
	teapot->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
	teapot->Render();

	cube->Rotate360(dt);
	cube->UpdateShader();
	UpdateLighting(cube->GetShader(), cubeLight);
	cube->UpdateRefractionIndex();
	cube->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
	cube->Render();

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

void RenderingLab3::UpdateLighting(GLuint shaderID, Lighting *light)
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

void RenderingLab3::initRefractions()
{
	refractions[MeshLoader::AIRTOWATER] = 1.0 / 1.33;
	refractions[MeshLoader::AIRTOICE] = 1.0 / 1.309;
	refractions[MeshLoader::AIRTOGLASS] = 1.0 / 1.52;
	refractions[MeshLoader::AIRTODIAMOND] = 1.0 / 2.42;
}
