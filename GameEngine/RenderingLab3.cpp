//#include "RenderingLab3.h"
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
//RenderingLab3::RenderingLab3(void)
//{
//	skyBoxShader = new Shader();
//	reflectionShader = new Shader();
//	refractionShader = new Shader();
//	refractionDispShader = new Shader();
//	combinedShader = new Shader();
//	combinedDispShader = new Shader();
//	texturedShader = new Shader();
//	normalMapShader = new Shader();
//	fresnelShader = new Shader();
//	normalFresnel = new Shader();
//
//	window = new Window(800,600,"Lab 3: Rendering");
//	camera = new Camera(window->GetWindow());
//
//	pauseScene = false;
//}
//
//void RenderingLab3::init(char** argv)
//{
//	initShaders();
//	initRefractions();
//	initModels();
//	initTextures();
//	initLights();
//	initTweakBar();
//}
//
//void RenderingLab3::initShaders()
//{
//	skyBoxShader->initShader(skyBoxShader->SKYBOX);
//	possibleShaders[Shader::SKYBOX] = skyBoxShader->GetProgramID();
//
//	reflectionShader->initShader(reflectionShader->REFLECTION);
//	possibleShaders[Shader::REFLECTION] = reflectionShader->GetProgramID();
//
//	refractionShader->initShader(refractionShader->REFRACTION);
//	possibleShaders[Shader::REFRACTION] = refractionShader->GetProgramID();
//
//	refractionDispShader->initShader(refractionDispShader->REFRACTION_DISP);
//	possibleShaders[Shader::REFRACTION_DISP] = refractionDispShader->GetProgramID();
//
//	combinedShader->initShader(combinedShader->COMBINED);
//	possibleShaders[Shader::COMBINED] = combinedShader->GetProgramID();
//
//	combinedDispShader->initShader(combinedDispShader->COMBINED_DISP);
//	possibleShaders[Shader::COMBINED_DISP] = combinedDispShader->GetProgramID();
//
//	texturedShader->initShader(texturedShader->TEXTURED);
//	possibleShaders[Shader::TEXTURED] = texturedShader->GetProgramID();
//
//	normalMapShader->initShader(normalMapShader->NORMAL_MAP);
//	possibleShaders[Shader::NORMAL_MAP] = normalMapShader->GetProgramID();
//
//	fresnelShader->initShader(fresnelShader->FRESNEL);
//	possibleShaders[Shader::FRESNEL] = fresnelShader->GetProgramID();
//
//	normalFresnel->initShader(normalFresnel->NORMAL_FRESNEL);
//	possibleShaders[Shader::NORMAL_FRESNEL] = normalFresnel->GetProgramID();
//}
//
//void RenderingLab3::initModels()
//{
//	teapot = new MeshLoader(skyBoxShader->GetProgramID(), "../Resources/Models/tea2.obj");
//	teapot->IsSkyboxActive(false);
//	teapot->SetPos(glm::vec3(0,0,0));
//	teapot->SetScale(glm::vec3(0.7,0.7,0.7));
//	teapot->SetOrientation(glm::quat(glm::vec3(0,0,0)));
//	teapot->SetPossibleShaders(possibleShaders);
//	teapot->SetRefractionTypes(refractions);
//
//	cube = new MeshLoader(normalMapShader->GetProgramID(), "../Resources/Models/cube.obj");
//	cube->IsSkyboxActive(false);
//	cube->IsTextureActive(true);
//	cube->SetPos(glm::vec3(-3,0,0));
//	cube->SetScale(glm::vec3(0.8,0.8,0.8));
//	cube->SetPossibleShaders(possibleShaders);
//	cube->SetRefractionTypes(refractions);
//
//	car = new MeshLoader(normalMapShader->GetProgramID(), "../Resources/Models/minicooper.obj");
//	car->IsSkyboxActive(false);
//	car->IsTextureActive(true);
//	car->SetPos(glm::vec3(3,0,0));
//	car->SetScale(glm::vec3(0.3,0.3,0.3));
//	car->SetPossibleShaders(possibleShaders);
//	car->SetRefractionTypes(refractions);
//
//	skybox = new MeshLoader(skyBoxShader->GetProgramID(),"../Resources/Models/cube2.obj");
//	skybox->IsSkyboxActive(true);
//	skybox->SetPos(camera->position);
//	skybox->SetScale(glm::vec3(100,100,100));
//	skybox->SetPossibleShaders(possibleShaders);
//}
//
//void RenderingLab3::initTextures()
//{
//	teapot->SetColor(glm::vec3(1,1,1));
//	teapot->SetShaderType(skyBoxShader->shaderType);
//	teapot->SetTexture("../Resources/Textures/bricks2.jpg");
//	teapot->SetNormalTexture("../Resources/Textures/bricks2_normal.png");
//
//	cube->SetColor(glm::vec3(1,1,1));
//	cube->SetShaderType(normalMapShader->shaderType);
//	cube->SetTexture("../Resources/Textures/bricks2.jpg");
//	cube->SetNormalTexture("../Resources/Textures/bricks2_normal.png");
//
//	car->SetColor(glm::vec3(1,1,1));
//	car->SetShaderType(normalMapShader->shaderType);
//	car->SetTexture("../Resources/Textures/stripe.png");
//	car->SetNormalTexture("../Resources/Textures/stripes_NRM.jpg");
//
//	skybox->SetShaderType(skyBoxShader->shaderType);
//	skybox->SetCubeMapTexture("../Resources/Skyboxes/Colloseum/");
//}
//
//void RenderingLab3::initLights()
//{
//	//teapotLight = new Lighting(orenNayar->GetProgramID());
//	//teapotLight = new Lighting(toonShader->GetProgramID());
//	carLight = new Lighting(normalMapShader->GetProgramID());
//	cubeLight = new Lighting(normalMapShader->GetProgramID());
//	teapotLight = new Lighting(normalFresnel->GetProgramID());
//}
//
//void RenderingLab3::initTweakBar()
//{
//	TwInit(TW_OPENGL_CORE, NULL);
//	TwWindowSize(window->GetWidth(), window->GetHeight());
//
//	bar = TwNewBar("Attributes: ");
//
//	TwEnumVal modeEV[] = {{Shader::ShaderType::SKYBOX, "Skybox"}, {Shader::ShaderType::REFLECTION, "Reflection"},
//	{Shader::ShaderType::REFRACTION, "Refraction"}, {Shader::ShaderType::COMBINED, "Combined"}, {Shader::ShaderType::TEXTURED, "Textured"},{Shader::ShaderType::REFRACTION_DISP, "Refraction Disp"},
//	{Shader::ShaderType::COMBINED_DISP, "Combined Disp"}, {Shader::ShaderType::FRESNEL, "Fresnel"}, {Shader::ShaderType::NORMAL_MAP, "Normal Map"},
//	{Shader::ShaderType::NORMAL_FRESNEL, "Normal Fresnel"} };
//
//	TwEnumVal modeEV2[] = {{MeshLoader::RefractionIndex::AIRTOWATER, "Air to Water"}, {MeshLoader::RefractionIndex::AIRTOICE, "Air to Ice"},
//	{MeshLoader::RefractionIndex::AIRTOGLASS, "Air to Glass"}, {MeshLoader::RefractionIndex::AIRTODIAMOND, "Air to Diamond"}};
//
//	TwType modeType;
//	TwType modeType2;
//
//	modeType = TwDefineEnum("ShaderType", modeEV, 10);
//	modeType2 = TwDefineEnum("RefractionType",modeEV2, 4);
//
//	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, "label='Pause Scene: '");
//	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
//	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");
//
//	TwAddVarRW(bar, "Teapot Position", TW_TYPE_DIR3F, &teapot->position, " group='Teapot' label='Teapot Pos: '");
//	TwAddVarRW(bar, "Teapot Rot", TW_TYPE_QUAT4F, &teapot->orientation, " group='Teapot' label='Teapot Rot: '");
//	TwAddVarRW(bar, "Teapot Scale", TW_TYPE_DIR3F, &teapot->scale, " group='Teapot' label='Teapot Scale: '");
//	TwAddVarRW(bar, "Teapot Mode", modeType, &teapot->shaderType,"group='Teapot' label='Teapot Mode: '");
//	TwAddVarRW(bar, "Teapot Texture", TW_TYPE_BOOLCPP, &teapot->useTexture, " group='Teapot' label='Teapot Textured: '");
//	TwAddVarRW(bar, "Teapot Mat Col", TW_TYPE_COLOR4F, &teapot->matColor,"group='Teapot' label='Teapot Color: '");
//	TwAddVarRW(bar, "Teapot Ref Factor", TW_TYPE_FLOAT, &teapot->reflectionFactor, "step='0.1' group='Teapot' label='Reflection Factor: '");
//	TwAddVarRW(bar, "Teapot RI", modeType2, &teapot->refractiveIndex,"group='Teapot' label='Refraction Mode: '");
//	TwAddVarRW(bar, "Teapot RIR", TW_TYPE_FLOAT, &teapot->ratioR,"group='Teapot' step='0.01' label='Refraction R: '");
//	TwAddVarRW(bar, "Teapot RIG", TW_TYPE_FLOAT, &teapot->ratioG,"group='Teapot' step='0.01' label='Refraction G: '");
//	TwAddVarRW(bar, "Teapot RIB", TW_TYPE_FLOAT, &teapot->ratioB,"group='Teapot' step='0.01' label='Refraction B: '");
//	TwAddVarRW(bar, "Teapot Light Position", TW_TYPE_DIR3F, &teapotLight->position, " group='Teapot Lighting' label='Light Direction: '");
//	TwAddVarRW(bar, "Teapot Diffuse Color", TW_TYPE_COLOR3F, &teapotLight->diffuseColor, " group='Teapot Lighting' label='Diffuse Color: '");
//	TwAddVarRW(bar, "Teapot Diffuse Intensity", TW_TYPE_FLOAT, &teapotLight->diffuseIntensity, "step='0.1' group='Teapot Lighting' label='Diffuse Intensity: '");
//	TwAddVarRW(bar, "Teapot Ambient Color", TW_TYPE_COLOR3F, &teapotLight->ambientColor, " group='Teapot Lighting' label='Ambient Color: '");
//	TwAddVarRW(bar, "Teapot Ambient Intensity", TW_TYPE_FLOAT, &teapotLight->ambientIntensity, "step='0.1' group='Teapot Lighting' label='Ambient Intensity: '");
//	TwAddVarRW(bar, "Teapot Specular Color", TW_TYPE_COLOR3F, &teapotLight->specularColor, " group='Teapot Lighting' label='Specular Color: '");
//	TwAddVarRW(bar, "Teapot Specular Intensity", TW_TYPE_FLOAT, &teapotLight->specularIntensity, "step='0.1' group='Teapot Lighting' label='Specular Intensity: '");
//	TwAddVarRW(bar, "Teapot Specular Shininess", TW_TYPE_FLOAT, &teapotLight->specularShininess, "step='0.1' group='Teapot Lighting' label='Specular Shininess: '");
//	TwAddVarRW(bar, "Teapot Material Roughness", TW_TYPE_FLOAT, &teapotLight->roughness, "step='0.1' group='Teapot Lighting' label='Roughness: '");
//
//	TwAddVarRW(bar, "Cube Position", TW_TYPE_DIR3F, &cube->position, " group='Cube' label='Cube Pos: '");
//	TwAddVarRW(bar, "Cube Rot", TW_TYPE_QUAT4F, &cube->orientation, " group='Cube' label='Cube Rot: '");
//	TwAddVarRW(bar, "Cube Scale", TW_TYPE_DIR3F, &cube->scale, " group='Cube' label='Cube Scale: '");
//	TwAddVarRW(bar, "Cube Mode", modeType, &cube->shaderType,"group='Cube' label='Cube Mode: '");
//	TwAddVarRW(bar, "Cube Texture", TW_TYPE_BOOLCPP, &cube->useTexture, " group='Cube' label='Cube Textured: '");
//	TwAddVarRW(bar, "Cube Normal", TW_TYPE_BOOLCPP, &cube->useNormalTexture, " group='Cube' label='Cube Textured: '");
//	TwAddVarRW(bar, "Cube Ref Factor", TW_TYPE_FLOAT, &cube->reflectionFactor, "step='0.1' group='Cube' label='Reflection Factor: '");
//	TwAddVarRW(bar, "Cube RI", modeType2, &cube->refractiveIndex,"group='Cube' label='Refraction Mode: '");
//	TwAddVarRW(bar, "Cube RIR", TW_TYPE_FLOAT, &cube->ratioR,"group='Cube' step='0.01' label='Refraction R: '");
//	TwAddVarRW(bar, "Cube RIG", TW_TYPE_FLOAT, &cube->ratioG,"group='Cube' step='0.01' label='Refraction G: '");
//	TwAddVarRW(bar, "Cube RIB", TW_TYPE_FLOAT, &cube->ratioB,"group='Cube' step='0.01' label='Refraction B: '");
//	TwAddVarRW(bar, "Cube Light Position", TW_TYPE_DIR3F, &cubeLight->position, " group='Cube Lighting' label='Light Direction: '");
//	TwAddVarRW(bar, "Cube Diffuse Color", TW_TYPE_COLOR3F, &cubeLight->diffuseColor, " group='Cube Lighting' label='Diffuse Color: '");
//	TwAddVarRW(bar, "Cube Diffuse Intensity", TW_TYPE_FLOAT, &cubeLight->diffuseIntensity, "step='0.1' group='Cube Lighting' label='Diffuse Intensity: '");
//	TwAddVarRW(bar, "Cube Ambient Color", TW_TYPE_COLOR3F, &cubeLight->ambientColor, " group='Cube Lighting' label='Ambient Color: '");
//	TwAddVarRW(bar, "Cube Ambient Intensity", TW_TYPE_FLOAT, &cubeLight->ambientIntensity, "step='0.1' group='Cube Lighting' label='Ambient Intensity: '");
//	TwAddVarRW(bar, "Cube Specular Color", TW_TYPE_COLOR3F, &cubeLight->specularColor, " group='Cube Lighting' label='Specular Color: '");
//	TwAddVarRW(bar, "Cube Specular Intensity", TW_TYPE_FLOAT, &cubeLight->specularIntensity, "step='0.1' group='Cube Lighting' label='Specular Intensity: '");
//	TwAddVarRW(bar, "Cube Specular Shininess", TW_TYPE_FLOAT, &cubeLight->specularShininess, "step='0.1' group='Cube Lighting' label='Specular Shininess: '");
//	TwAddVarRW(bar, "Cube Material Roughness", TW_TYPE_FLOAT, &cubeLight->roughness, "step='0.1' group='Cube Lighting' label='Roughness: '");
//
//	TwAddVarRW(bar, "Car Position", TW_TYPE_DIR3F, &car->position, " group='Car' label='Car Pos: '");
//	TwAddVarRW(bar, "Car Rot", TW_TYPE_QUAT4F, &car->orientation, " group='Car' label='Car Rot: '");
//	TwAddVarRW(bar, "Car Scale", TW_TYPE_DIR3F, &car->scale, " group='Car' label='Car Scale: '");
//	TwAddVarRW(bar, "Car Mode", modeType, &car->shaderType,"group='Car' label='Car Mode: '");
//	TwAddVarRW(bar, "Car Texture", TW_TYPE_BOOLCPP, &car->useTexture, " group='Car' label='Car Textured: '");
//	TwAddVarRW(bar, "Car Normal", TW_TYPE_BOOLCPP, &car->useNormalTexture, " group='Car' label='Car Textured: '");
//	TwAddVarRW(bar, "Car Ref Factor", TW_TYPE_FLOAT, &car->reflectionFactor, "step='0.1' group='Car' label='Reflection Factor: '");
//	TwAddVarRW(bar, "Car RI", modeType2, &car->refractiveIndex,"group='Car' label='Refraction Mode: '");
//	TwAddVarRW(bar, "Car RIR", TW_TYPE_FLOAT, &car->ratioR,"group='Car' step='0.01' label='Refraction R: '");
//	TwAddVarRW(bar, "Car RIG", TW_TYPE_FLOAT, &car->ratioG,"group='Car' step='0.01' label='Refraction G: '");
//	TwAddVarRW(bar, "Car RIB", TW_TYPE_FLOAT, &car->ratioB,"group='Car' step='0.01' label='Refraction B: '");
//	TwAddVarRW(bar, "Car Light Position", TW_TYPE_DIR3F, &carLight->position, " group='Car Lighting' label='Light Direction: '");
//	TwAddVarRW(bar, "Car Diffuse Color", TW_TYPE_COLOR3F, &carLight->diffuseColor, " group='Car Lighting' label='Diffuse Color: '");
//	TwAddVarRW(bar, "Car Diffuse Intensity", TW_TYPE_FLOAT, &carLight->diffuseIntensity, "step='0.1' group='Car Lighting' label='Diffuse Intensity: '");
//	TwAddVarRW(bar, "Car Ambient Color", TW_TYPE_COLOR3F, &carLight->ambientColor, " group='Car Lighting' label='Ambient Color: '");
//	TwAddVarRW(bar, "Car Ambient Intensity", TW_TYPE_FLOAT, &carLight->ambientIntensity, "step='0.1' group='Car Lighting' label='Ambient Intensity: '");
//	TwAddVarRW(bar, "Car Specular Color", TW_TYPE_COLOR3F, &carLight->specularColor, " group='Car Lighting' label='Specular Color: '");
//	TwAddVarRW(bar, "Car Specular Intensity", TW_TYPE_FLOAT, &carLight->specularIntensity, "step='0.1' group='Car Lighting' label='Specular Intensity: '");
//	TwAddVarRW(bar, "Car Specular Shininess", TW_TYPE_FLOAT, &carLight->specularShininess, "step='0.1' group='Car Lighting' label='Specular Shininess: '");
//	TwAddVarRW(bar, "Car Material Roughness", TW_TYPE_FLOAT, &carLight->roughness, "step='0.1' group='Car Lighting' label='Roughness: '");
//
//	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
//	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
//	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
//	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
//}
//
//void RenderingLab3::run()
//{
//	double prevTime = glfwGetTime(); 
//
//	while(!window->IsCloseRequested())
//	{
//		time = glfwGetTime();
//		dt = time - prevTime;
//		prevTime = time;
//
//		if(pauseScene)
//		{
//			dt = 0;
//		}
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
//void RenderingLab3::update()
//{
//	GLint OldCullFaceMode;
//	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
//	GLint OldDepthFuncMode;
//	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);
//
//	camera->computeMatricesFromInputs();
//
//	teapot->Rotate360(dt);
//	teapot->UpdateShader();
//	teapot->UpdateRefractionIndex();
//	teapot->UpdateRefractionIndexRGB();
//	UpdateLighting(teapot->GetShader(), teapotLight);
//	teapot->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	teapot->Render();
//
//	car->Rotate360(dt);
//	car->UpdateShader();
//	UpdateLighting(car->GetShader(), carLight);
//	car->UpdateRefractionIndex();
//	car->UpdateRefractionIndexRGB();
//	car->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	car->Render();
//
//	cube->Rotate360(dt);
//	cube->UpdateShader();
//	UpdateLighting(cube->GetShader(), cubeLight);
//	cube->UpdateRefractionIndex();
//	cube->UpdateRefractionIndexRGB();
//	cube->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	cube->Render();
//
//	glCullFace(GL_FRONT);
//	glDepthFunc(GL_LEQUAL);
//	glDepthMask(false);
//
//	skybox->UseProgram();
//	skybox->cameraPos = camera->position;
//	skybox->UpdateShader();
//	skybox->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	skybox->RenderCubeMap();
//
//	glDepthMask(true);
//	glCullFace(OldCullFaceMode); 
//	glDepthFunc(OldDepthFuncMode);
//}
//
//void RenderingLab3::UpdateLighting(GLuint shaderID, Lighting *light)
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
//
//void RenderingLab3::initRefractions()
//{
//	refractions[MeshLoader::AIRTOWATER] = 1.0 / 1.33;
//	refractions[MeshLoader::AIRTOICE] = 1.0 / 1.309;
//	refractions[MeshLoader::AIRTOGLASS] = 1.0 / 1.52;
//	refractions[MeshLoader::AIRTODIAMOND] = 1.0 / 2.42;
//}
