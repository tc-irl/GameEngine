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
	normalFresnel = new Shader();

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

	normalFresnel->initShader(normalFresnel->NORMAL_FRESNEL);
	possibleShaders[Shader::NORMAL_FRESNEL] = normalFresnel->GetProgramID();
}

void RenderingLab3::initModels()
{
	sphere = new MeshLoader(skyBoxShader->GetProgramID(), "../Resources/Models/sphere.obj");
	sphere->IsSkyboxActive(false);
	sphere->SetPos(glm::vec3(0,0,0));
	sphere->SetScale(glm::vec3(0.8,0.8,0.8));
	sphere->SetPossibleShaders(possibleShaders);
	sphere->SetRefractionTypes(refractions);

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
	sphere->SetColor(glm::vec3(1,1,1));
	sphere->SetShaderType(skyBoxShader->shaderType);
	sphere->SetTexture("../Resources/Textures/bricks2.jpg");
	sphere->SetNormalTexture("../Resources/Textures/bricks2_normal.png");

	cube->SetColor(glm::vec3(1,1,1));
	cube->SetShaderType(normalMapShader->shaderType);
	cube->SetTexture("../Resources/Textures/bricks2.jpg");
	cube->SetNormalTexture("../Resources/Textures/bricks2_normal.png");

	skybox->SetShaderType(skyBoxShader->shaderType);
	skybox->SetCubeMapTexture("../Resources/Skyboxes/Colloseum/");
}

void RenderingLab3::initLights()
{
	//sphereLight = new Lighting(orenNayar->GetProgramID());
	//sphereLight = new Lighting(toonShader->GetProgramID());
	//headLight = new Lighting(phongShader->GetProgramID());
	cubeLight = new Lighting(normalMapShader->GetProgramID());
	sphereLight = new Lighting(normalFresnel->GetProgramID());
}

void RenderingLab3::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

	bar = TwNewBar("Attributes: ");

	TwEnumVal modeEV[] = {{Shader::ShaderType::SKYBOX, "Skybox"}, {Shader::ShaderType::REFLECTION, "Reflection"},
	{Shader::ShaderType::REFRACTION, "Refraction"}, {Shader::ShaderType::COMBINED, "Combined"}, {Shader::ShaderType::TEXTURED, "Textured"},{Shader::ShaderType::REFRACTION_DISP, "Refraction Disp"},
	{Shader::ShaderType::COMBINED_DISP, "Combined Disp"}, {Shader::ShaderType::FRESNEL, "Fresnel"}, {Shader::ShaderType::NORMAL_MAP, "Normal Map"},
	{Shader::ShaderType::NORMAL_FRESNEL, "Normal Fresnel"} };

	TwEnumVal modeEV2[] = {{MeshLoader::RefractionIndex::AIRTOWATER, "Air to Water"}, {MeshLoader::RefractionIndex::AIRTOICE, "Air to Ice"},
	{MeshLoader::RefractionIndex::AIRTOGLASS, "Air to Glass"}, {MeshLoader::RefractionIndex::AIRTODIAMOND, "Air to Diamond"}};

	TwType modeType;
	TwType modeType2;

	modeType = TwDefineEnum("ShaderType", modeEV, 10);
	modeType2 = TwDefineEnum("RefractionType",modeEV2, 4);

	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, "label='Pause Scene: '");
	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");

	TwAddVarRW(bar, "sphere Position", TW_TYPE_DIR3F, &sphere->position, " group='sphere' label='sphere Pos: '");
	TwAddVarRW(bar, "sphere Rot", TW_TYPE_QUAT4F, &sphere->orientation, " group='sphere' label='sphere Rot: '");
	TwAddVarRW(bar, "sphere Mode", modeType, &sphere->shaderType,"group='sphere' label='sphere Mode: '");
	TwAddVarRW(bar, "sphere Texture", TW_TYPE_BOOLCPP, &sphere->useTexture, " group='sphere' label='sphere Textured: '");
	TwAddVarRW(bar, "sphere Mat Col", TW_TYPE_COLOR4F, &sphere->matColor,"group='sphere' label='sphere Color: '");
	TwAddVarRW(bar, "sphere Ref Factor", TW_TYPE_FLOAT, &sphere->reflectionFactor, "step='0.1' group='sphere' label='Reflection Factor: '");
	TwAddVarRW(bar, "sphere RI", modeType2, &sphere->refractiveIndex,"group='sphere' label='Refraction Mode: '");
	TwAddVarRW(bar, "sphere RIR", TW_TYPE_FLOAT, &sphere->ratioR,"group='sphere' step='0.01' label='Refraction R: '");
	TwAddVarRW(bar, "sphere RIG", TW_TYPE_FLOAT, &sphere->ratioG,"group='sphere' step='0.01' label='Refraction G: '");
	TwAddVarRW(bar, "sphere RIB", TW_TYPE_FLOAT, &sphere->ratioB,"group='sphere' step='0.01' label='Refraction B: '");
	TwAddVarRW(bar, "sphere Light Position", TW_TYPE_DIR3F, &sphereLight->position, " group='Cube Lighting' label='Light Direction: '");
	TwAddVarRW(bar, "sphere Diffuse Color", TW_TYPE_COLOR3F, &sphereLight->diffuseColor, " group='Cube Lighting' label='Diffuse Color: '");
	TwAddVarRW(bar, "sphere Diffuse Intensity", TW_TYPE_FLOAT, &sphereLight->diffuseIntensity, "step='0.1' group='Cube Lighting' label='Diffuse Intensity: '");
	TwAddVarRW(bar, "sphere Ambient Color", TW_TYPE_COLOR3F, &sphereLight->ambientColor, " group='Cube Lighting' label='Ambient Color: '");
	TwAddVarRW(bar, "sphere Ambient Intensity", TW_TYPE_FLOAT, &sphereLight->ambientIntensity, "step='0.1' group='Cube Lighting' label='Ambient Intensity: '");
	TwAddVarRW(bar, "sphere Specular Color", TW_TYPE_COLOR3F, &sphereLight->specularColor, " group='Cube Lighting' label='Specular Color: '");
	TwAddVarRW(bar, "sphere Specular Intensity", TW_TYPE_FLOAT, &sphereLight->specularIntensity, "step='0.1' group='Cube Lighting' label='Specular Intensity: '");
	TwAddVarRW(bar, "sphere Specular Shininess", TW_TYPE_FLOAT, &sphereLight->specularShininess, "step='0.1' group='Cube Lighting' label='Specular Shininess: '");
	TwAddVarRW(bar, "sphere Material Roughness", TW_TYPE_FLOAT, &sphereLight->roughness, "step='0.1' group='Cube Lighting' label='Roughness: '");

	TwAddVarRW(bar, "Cube Position", TW_TYPE_DIR3F, &cube->position, " group='Cube' label='Cube Pos: '");
	TwAddVarRW(bar, "Cube Rot", TW_TYPE_QUAT4F, &cube->orientation, " group='Cube' label='Cube Rot: '");
	TwAddVarRW(bar, "Cube Mode", modeType, &cube->shaderType,"group='Cube' label='Cube Mode: '");
	TwAddVarRW(bar, "Cube Texture", TW_TYPE_BOOLCPP, &cube->useTexture, " group='Cube' label='Cube Textured: '");
	TwAddVarRW(bar, "Cube Normal", TW_TYPE_BOOLCPP, &cube->useNormalTexture, " group='Cube' label='Cube Textured: '");
	TwAddVarRW(bar, "Cube Ref Factor", TW_TYPE_FLOAT, &cube->reflectionFactor, "step='0.1' group='Cube' label='Reflection Factor: '");
	TwAddVarRW(bar, "Cube RI", modeType2, &cube->refractiveIndex,"group='Cube' label='Refraction Mode: '");
	TwAddVarRW(bar, "Cube RIR", TW_TYPE_FLOAT, &cube->ratioR,"group='Cube' step='0.01' label='Refraction R: '");
	TwAddVarRW(bar, "Cube RIG", TW_TYPE_FLOAT, &cube->ratioG,"group='Cube' step='0.01' label='Refraction G: '");
	TwAddVarRW(bar, "Cube RIB", TW_TYPE_FLOAT, &cube->ratioB,"group='Cube' step='0.01' label='Refraction B: '");
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

	sphere->Rotate360(dt);
	sphere->UpdateShader();
	sphere->UpdateRefractionIndex();
	sphere->UpdateRefractionIndexRGB();
	UpdateLighting(sphere->GetShader(), sphereLight);
	sphere->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
	sphere->Render();

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
