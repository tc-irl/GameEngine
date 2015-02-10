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
//	toonTextured = new Shader();
//	phongShader = new Shader();
//	phongTextured = new Shader();
//	diffuseShader = new Shader();
//	orenNayar = new Shader();
//	orenTextured = new Shader();
//
//
//	window = new Window(800,600,"Lab 1: Rendering");
//	camera = new Camera(window->GetWindow());
//
//	pauseScene = false;
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
//	basicShader->initShader(Shader::BASIC);
//	possibleShaders[Shader::BASIC] = basicShader->GetProgramID();
//
//	textureShader->initShader(textureShader->TEXTURED);
//	possibleShaders[Shader::TEXTURED]  = textureShader->GetProgramID();
//
//	toonShader->initShader(toonShader->TOON);
//	possibleShaders[Shader::TOON]  = toonShader->GetProgramID();
//
//	toonTextured->initShader(toonTextured->TOON_TEXTURED);
//	possibleShaders[Shader::TOON_TEXTURED] = toonTextured->GetProgramID();
//
//	phongShader->initShader(phongShader->PHONG);
//	possibleShaders[Shader::PHONG]= phongShader->GetProgramID();
//
//	phongTextured->initShader(phongTextured->PHONG_TEXTURED);
//	possibleShaders[Shader::PHONG_TEXTURED] = phongTextured->GetProgramID();
//
//	diffuseShader->initShader(diffuseShader->DIFFUSE);
//	possibleShaders[Shader::DIFFUSE] = diffuseShader->GetProgramID();
//
//	orenNayar->initShader(orenNayar->OREN_NAYAR);
//	possibleShaders[Shader::OREN_NAYAR] = orenNayar->GetProgramID();
//
//	orenTextured->initShader(orenNayar->OREN_TEXTURED);
//	possibleShaders[Shader::OREN_TEXTURED] = orenTextured->GetProgramID();
//
//}
//
//void RenderingLab1::initModels()
//{
//	plane = new MeshLoader(textureShader->GetProgramID(), "../Resources/Models/plane.obj");
//	plane->IsTextureActive(true);
//	plane->SetPos(glm::vec3(0,-2,0));
//
//	teapot = new MeshLoader(toonShader->GetProgramID(), "../Resources/Models/teapot.obj");
//	teapot->SetPos(glm::vec3(3,0,0));
//	teapot->SetScale(glm::vec3(0.8,0.8,0.8));
//	teapot->SetPossibleShaders(possibleShaders);
//
//	monkey = new MeshLoader(phongShader->GetProgramID(), "../Resources/Models/monkey.obj");
//	monkey->SetPos(glm::vec3(-3,0,0));
//	monkey->SetPossibleShaders(possibleShaders);
//
//	head = new MeshLoader(phongShader->GetProgramID(), "../Resources/Models/head2.obj");
//	head->SetPos(glm::vec3(0,3,0));
//	head->SetPossibleShaders(possibleShaders);
//}
//
//void RenderingLab1::initTextures()
//{
//	plane->SetColor(glm::vec3(1,1,1));
//	plane->SetShaderType(textureShader->shaderType);
//	plane->SetTexture("../Resources/Textures/tile.png");
//
//	monkey->SetColor(glm::vec3(1,0,0));
//	monkey->SetShaderType(phongShader->shaderType);
//	monkey->SetTexture("../Resources/Textures/orange.jpg");
//
//	teapot->SetColor(glm::vec3(1,1,1));
//	teapot->SetShaderType(toonShader->shaderType);
//	teapot->SetTexture("../Resources/Textures/bricks.jpg");
//
//	head->SetColor(glm::vec3(1,1,1));
//	head->SetShaderType(orenNayar->shaderType);
//	head->SetTexture("../Resources/Textures/lambertian.jpg");
//}
//
//void RenderingLab1::initLights()
//{
//	monkeyLight = new Lighting(orenNayar->GetProgramID());
//	teapotLight = new Lighting(toonShader->GetProgramID());
//	headLight = new Lighting(phongShader->GetProgramID());
//}
//
//void RenderingLab1::initTweakBar()
//{
//	TwInit(TW_OPENGL_CORE, NULL);
//	TwWindowSize(window->GetWidth(), window->GetHeight());
//
//	bar = TwNewBar("Attributes: ");
//
//	TwEnumVal modeEV[] = {{Shader::ShaderType::NONE, "None"}, {Shader::ShaderType::BASIC, "Basic"}, {Shader::ShaderType::PHONG, "Phong"}, {Shader::ShaderType::TOON,"Toon" 
//	}, {Shader::ShaderType::DIFFUSE, "Diffuse"}, {Shader::ShaderType::TEXTURED, "Textured"}, 
//	{Shader::ShaderType::TOON_TEXTURED, "Toon Textured"}, {Shader::ShaderType::PHONG_TEXTURED, "Phong Textured"}, {Shader::ShaderType::OREN_NAYAR, "Oren Nayar"},  {Shader::ShaderType::OREN_TEXTURED, "Oren Textured"}};
//
//	TwType modeType;
//
//	modeType = TwDefineEnum("ShaderType", modeEV, 10);
//
//	TwAddVarRW(bar, "Pause", TW_TYPE_BOOLCPP, &pauseScene, "label='Pause Scene: '");
//	TwAddVarRW(bar, "Camera Position", TW_TYPE_DIR3F, &camera->position, " group='Camera' label='Camera Pos: '");
//	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " group='Camera' label='Camera Rot: '");
//
//	TwAddVarRW(bar, "Monkey Position", TW_TYPE_DIR3F, &monkey->position, " group='Monkey' label='Monkey Pos: '");
//	TwAddVarRW(bar, "Monkey Rot", TW_TYPE_QUAT4F, &monkey->orientation, " group='Monkey' label='Monkey Rot: '");
//	TwAddVarRW(bar, "Monkey Mode", modeType, &monkey->shaderType,"group='Monkey' label='Monkey Mode: '");
//	TwAddVarRW(bar, "Monkey Color", TW_TYPE_COLOR3F, &monkey->color,"group='Monkey' label='Monkey Mode: '");
//	TwAddVarRW(bar, "Monkey Texture", TW_TYPE_BOOLCPP, &monkey->useTexture, " group='Monkey' label='Monkey Textured: '");
//	TwAddVarRW(bar, "Light Position", TW_TYPE_DIR3F, &monkeyLight->position, " group='Monkey Lighting' label='Light Direction: '");
//	TwAddVarRW(bar, "Diffuse Color", TW_TYPE_COLOR3F, &monkeyLight->diffuseColor, " group='Monkey Lighting' label='Diffuse Color: '");
//	TwAddVarRW(bar, "Diffuse Intensity", TW_TYPE_FLOAT, &monkeyLight->diffuseIntensity, "step='0.1' group='Monkey Lighting' label='Diffuse Intensity: '");
//	TwAddVarRW(bar, "Ambient Color", TW_TYPE_COLOR3F, &monkeyLight->ambientColor, " group='Monkey Lighting' label='Ambient Color: '");
//	TwAddVarRW(bar, "Ambient Intensity", TW_TYPE_FLOAT, &monkeyLight->ambientIntensity, "step='0.1' group='Monkey Lighting' label='Ambient Intensity: '");
//	TwAddVarRW(bar, "Specular Color", TW_TYPE_COLOR3F, &monkeyLight->specularColor, " group='Monkey Lighting' label='Specular Color: '");
//	TwAddVarRW(bar, "Specular Intensity", TW_TYPE_FLOAT, &monkeyLight->specularIntensity, "step='0.1' group='Monkey Lighting' label='Specular Intensity: '");
//	TwAddVarRW(bar, "Specular Shininess", TW_TYPE_FLOAT, &monkeyLight->specularShininess, "step='0.1' group='Monkey Lighting' label='Specular Shininess: '");
//	TwAddVarRW(bar, "Material Roughness", TW_TYPE_FLOAT, &monkeyLight->roughness, "step='0.1' group='Monkey Lighting' label='Roughness: '");
//
//	TwAddVarRW(bar, "Teapot Position", TW_TYPE_DIR3F, &teapot->position, " group='Teapot' label='Teapot Pos: '");
//	TwAddVarRW(bar, "Teapot Rot", TW_TYPE_QUAT4F, &teapot->orientation, " group='Teapot' label='Teapot Rot: '");
//	TwAddVarRW(bar, "Teapot Mode", modeType, &teapot->shaderType,"group='Teapot' label='Teapot Mode: '");
//	TwAddVarRW(bar, "Teapot Color", TW_TYPE_COLOR3F, &teapot->color,"group='Teapot' label='Teapot Color: '");
//	TwAddVarRW(bar, "Teapot Texture", TW_TYPE_BOOLCPP, &teapot->useTexture, " group='Teapot' label='Teapot Textured: '");
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
//	TwAddVarRW(bar, "Head Position", TW_TYPE_DIR3F, &head->position, " group='Head' label='Head Pos: '");
//	TwAddVarRW(bar, "Head Rot", TW_TYPE_QUAT4F, &head->orientation, " group='Head' label='Head Rot: '");
//	TwAddVarRW(bar, "Head Mode", modeType, &head->shaderType,"group='Head' label='Head Mode: '");
//	TwAddVarRW(bar, "Head Color", TW_TYPE_COLOR3F, &head->color,"group='Head' label='Head Color: '");
//	TwAddVarRW(bar, "Head Texture", TW_TYPE_BOOLCPP, &head->useTexture, " group='Head' label='Head Textured: '");
//	TwAddVarRW(bar, "Head Light Position", TW_TYPE_DIR3F, &headLight->position, " group='Head Lighting' label='Head Direction: '");
//	TwAddVarRW(bar, "Head Diffuse Color", TW_TYPE_COLOR3F, &headLight->diffuseColor, " group='Head Lighting' label='Diffuse Color: '");
//	TwAddVarRW(bar, "Head Diffuse Intensity", TW_TYPE_FLOAT, &headLight->diffuseIntensity, "step='0.1' group='Head Lighting' label='Diffuse Intensity: '");
//	TwAddVarRW(bar, "Head Ambient Color", TW_TYPE_COLOR3F, &headLight->ambientColor, " group='Head Lighting' label='Ambient Color: '");
//	TwAddVarRW(bar, "Head Ambient Intensity", TW_TYPE_FLOAT, &headLight->ambientIntensity, "step='0.1' group='Head Lighting' label='Ambient Intensity: '");
//	TwAddVarRW(bar, "Head Specular Color", TW_TYPE_COLOR3F, &headLight->specularColor, " group='Head Lighting' label='Specular Color: '");
//	TwAddVarRW(bar, "Head Specular Intensity", TW_TYPE_FLOAT, &headLight->specularIntensity, "step='0.1' group='Head Lighting' label='Specular Intensity: '");
//	TwAddVarRW(bar, "Head Specular Shininess", TW_TYPE_FLOAT, &headLight->specularShininess, "step='0.1' group='Head Lighting' label='Specular Shininess: '");
//	TwAddVarRW(bar, "Head Material Roughness", TW_TYPE_FLOAT, &headLight->roughness, "step='0.1' group='Head Lighting' label='Roughness: '");
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
//void RenderingLab1::update()
//{
//	camera->computeMatricesFromInputs();
//
//	plane->UseProgram();
//	plane->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	plane->Render();
//
//	monkey->Rotate360(dt);
//	monkey->UpdateShader();
//	UpdateLighting(monkey->GetShader(), monkeyLight);
//	monkey->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	monkey->Render();
//
//	teapot->Rotate360(dt);
//	teapot->UpdateShader();
//	UpdateLighting(teapot->GetShader(), teapotLight);
//	teapot->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	teapot->Render();
//
//	head->Rotate360(dt);
//	head->UpdateShader();
//	UpdateLighting(head->GetShader(), headLight);
//	head->Update(camera->getViewMatrix(),camera->getProjectionMatrix(),dt);
//	head->Render();
//
//}
//
//void RenderingLab1::UpdateLighting(GLuint shaderID, Lighting *light)
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
