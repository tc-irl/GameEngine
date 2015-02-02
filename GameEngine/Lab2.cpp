#include "Lab2.h"

////typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
//void MouseButtonCB( GLFWwindow*,int button ,int action ,int mods)
//{
//	TwEventMouseButtonGLFW( button , action );
//}
//
////typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);
//void MousePosCB(GLFWwindow*,double x ,double y)
//{
//	TwEventMousePosGLFW( (int)x, (int)y );
//}
//
////typedef void (* GLFWkeyfun)(GLFWwindow*,int,int,int,int);
//void KeyFunCB( GLFWwindow* window,int key,int scancode,int action,int mods)
//{
//	TwEventKeyGLFW( key , action );
//	TwEventCharGLFW( key  , action );
//}
//
////typedef void (* GLFWscrollfun)(GLFWwindow*,double,double);
//void MouseScrollCB(  GLFWwindow* window, double x , double y )
//{
//	TwEventMouseWheelGLFW( (int)y );
//}


Lab2::Lab2(void)
{
	shader = new Shader();
	window = new Window(800,600,"Lab 2: Rigid Body Motion");
	camera = new Camera(window->GetWindow());
	objectBuffer = new ObjectBuffer(36);
	rigidBody = new RigidBody();

	speed = 0.0001f;

	forcePoint = glm::vec3(1,1,0);
}

void Lab2::init(char** argv)
{
	initShaders();
	initCube();
	//initModels();
	initTweakBar();
	//initTextures();

}

void Lab2::initShaders()
{
	shader->initShader("../Resources/Shaders/diffuse");
}

void Lab2::run() 
{
	double prevTime = glfwGetTime(); 

	while(!window->IsCloseRequested())
	{
		// calculating delta time
		time = glfwGetTime();
		dt = time - prevTime;
		prevTime = time;

		window->Update();

		glUseProgram(shader->GetProgramID());

		GLuint modelLoc = glGetUniformLocation(shader->GetProgramID(), "model");
		GLuint viewLoc = glGetUniformLocation(shader->GetProgramID(), "view");
		GLuint projLoc = glGetUniformLocation(shader->GetProgramID(), "projection");

		camera->computeMatricesFromInputs();
		camera->handleMVP(modelLoc,viewLoc,projLoc);

		camera->getProjectionMatrix();
		camera->getViewMatrix();

		update(modelLoc,(float)dt);

		Draw();
		TwDraw();

		window->SwapBuffers();
	}
}

void Lab2::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

	bar = TwNewBar("Attributes: ");

	TwAddVarRW(bar, "Camera", TW_TYPE_DIR3F, &camera->position, " label='Camera Pos: '");

	TwAddVarRW(bar, "Mass", TW_TYPE_FLOAT, &rigidBody->mass, " group='Object Properties' label='Mass: '");
	TwAddVarRW(bar, "Gravity", TW_TYPE_BOOLCPP, &rigidBody->gravity, "group='Object Properties' label='Gravity: '");
	TwAddVarRW(bar, "Force", TW_TYPE_BOOLCPP, &rigidBody->forceEnabled, "group='Object Properties' label='Enable Forces: '");
	//TwAddVarRW(bar, "Speed", TW_TYPE_FLOAT, &speed, "group='Object Properties' precision=4 label='Speed: '");
	//TwAddVarRW(bar, "Direction", TW_TYPE_DIR3F, &direction, "group='Object Properties' label='Direction: '");
	TwAddVarRW(bar, "Position", TW_TYPE_DIR3F, &rigidBody->position, "group='Object Properties' label='Position: '");
	TwAddVarRW(bar, "Orientation", TW_TYPE_QUAT4F, &rigidBody->orientation, "group='Object Properties' label='Orientation: '");
	TwAddVarRW(bar, "Force Point", TW_TYPE_DIR3F, &forcePoint, "group='Object Properties' label='Force point: '");
	TwAddVarRW(bar, "Direction", TW_TYPE_DIR3F, &direction, "group='Object Properties' label='Force Direction: '");
	TwAddVarRO(bar, "COM", TW_TYPE_DIR3F, &rigidBody->centreOfMass, "group='Object Properties' label='COM: '");

	TwAddVarRW(bar, "Vertex 0", TW_TYPE_DIR3F,&objectBuffer->transformedVertices[0], "group='Vertex Pos' label='Vertex 0:'");
	TwAddVarRW(bar, "Vertex 1", TW_TYPE_DIR3F,&objectBuffer->transformedVertices[1], "group='Vertex Pos' label='Vertex 1:'");
	TwAddVarRW(bar, "Vertex 2", TW_TYPE_DIR3F,&objectBuffer->transformedVertices[2], "group='Vertex Pos' label='Vertex 2:'");
	TwAddVarRW(bar, "Vertex 3", TW_TYPE_DIR3F,&objectBuffer->transformedVertices[3], "group='Vertex Pos' label='Vertex 3:'");
	TwAddVarRW(bar, "Vertex 4", TW_TYPE_DIR3F,&objectBuffer->transformedVertices[4], "group='Vertex Pos' label='Vertex 4:'");
	TwAddVarRW(bar, "Vertex 5", TW_TYPE_DIR3F,&objectBuffer->transformedVertices[5], "group='Vertex Pos' label='Vertex 5:'");
	TwAddVarRW(bar, "Vertex 6", TW_TYPE_DIR3F,&objectBuffer->transformedVertices[6], "group='Vertex Pos' label='Vertex 6:'");
	TwAddVarRW(bar, "Vertex 7", TW_TYPE_DIR3F,&objectBuffer->transformedVertices[7], "group='Vertex Pos' label='Vertex 7:'");


	// after GLFW initialization
	// directly redirect GLFW events to AntTweakBar
	//glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
	//glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
	//glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
	//glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
}

void Lab2::initCube()
{
	// Colored cube tutorial used: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/

	glm::vec3 g_vertex_buffer_data[] = 
	{
		glm::vec3(-1.0f,-1.0f,-1.0f), // triangle 1 : begin
		glm::vec3(-1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f), // triangle 1 : end
		glm::vec3(1.0f, 1.0f,-1.0f), // triangle 2 : begin
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f), // triangle 2 : end
		glm::vec3(1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(1.0f,-1.0f,-1.0f),
		glm::vec3(1.0f, 1.0f,-1.0f),
		glm::vec3(1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f),
		glm::vec3(1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f, 1.0f),
		glm::vec3(1.0f,-1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f,-1.0f,-1.0f),
		glm::vec3(1.0f, 1.0f,-1.0f),
		glm::vec3(1.0f,-1.0f,-1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f,-1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f,-1.0f, 1.0f)
	};

	glm::vec3 g_color_buffer_data[] = 
	{
		glm::vec3(0.583f,  0.771f,  0.014f),
		glm::vec3(0.609f,  0.115f,  0.436f),
		glm::vec3(0.327f,  0.483f,  0.844f),
		glm::vec3(0.822f,  0.569f,  0.201f),
		glm::vec3(0.435f,  0.602f,  0.223f),
		glm::vec3(0.310f,  0.747f,  0.185f),
		glm::vec3(0.597f,  0.770f,  0.761f),
		glm::vec3(0.559f,  0.436f,  0.730f),
		glm::vec3(0.359f,  0.583f,  0.152f),
		glm::vec3(0.483f,  0.596f,  0.789f),
		glm::vec3(0.559f,  0.861f,  0.639f),
		glm::vec3(0.195f,  0.548f,  0.859f),
		glm::vec3(0.014f,  0.184f,  0.576f),
		glm::vec3(0.771f,  0.328f,  0.970f),
		glm::vec3(0.406f,  0.615f,  0.116f),
		glm::vec3(0.676f,  0.977f,  0.133f),
		glm::vec3(0.971f,  0.572f,  0.833f),
		glm::vec3(0.140f,  0.616f,  0.489f),
		glm::vec3(0.997f,  0.513f,  0.064f),
		glm::vec3(0.945f,  0.719f,  0.592f),
		glm::vec3(0.543f,  0.021f,  0.978f),
		glm::vec3(0.279f,  0.317f,  0.505f),
		glm::vec3(0.167f,  0.620f,  0.077f),
		glm::vec3(0.347f,  0.857f,  0.137f),
		glm::vec3(0.055f,  0.953f,  0.042f),
		glm::vec3(0.714f,  0.505f,  0.345f),
		glm::vec3(0.783f,  0.290f,  0.734f),
		glm::vec3(0.722f,  0.645f,  0.174f),
		glm::vec3(0.302f,  0.455f,  0.848f),
		glm::vec3(0.225f,  0.587f,  0.040f),
		glm::vec3(0.517f,  0.713f,  0.338f),
		glm::vec3(0.053f,  0.959f,  0.120f),
		glm::vec3(0.393f,  0.621f,  0.362f),
		glm::vec3(0.673f,  0.211f,  0.457f),
		glm::vec3(0.820f,  0.883f,  0.371f),
		glm::vec3(0.982f,  0.099f,  0.879f)
	};

	std::vector<glm::vec3> temp; 

	for(int i = 0; i < 36; i++)
	{
		temp.push_back(g_vertex_buffer_data[i]);
	}

	objectBuffer->GenerateVBO(temp,g_color_buffer_data);
	objectBuffer->LinkBufferToShader(shader->GetProgramID());
}

void Lab2::Draw()
{
	objectBuffer->Draw();
}

void Lab2::update(GLuint modelLoc, float timeDelta)
{
	glm::mat4 model;

	model = rigidBody->getTransformationMatrix();
	objectBuffer->transVertices(rigidBody->orientation,rigidBody->position);
	
	centroid = objectBuffer->getCentreOfMass();
	rigidBody->centreOfMass = centroid;

	rigidBody->applyForce(forcePoint,direction);
	rigidBody->enableGravity();
	rigidBody->update(dt);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}