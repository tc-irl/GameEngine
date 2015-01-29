#include "Window.h"

Window::Window(int width, int height, const std::string& title)
{
	this->width = width;
	this->height = height;
	this->title = title;
	
	isCloseRequested = false;

	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width,height,title.c_str(),NULL,NULL);

	if(window == NULL)
	{
		std::cout << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK) 
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	glEnable(GLFW_REPEAT);
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR);
	glfwSetCursorPos(window, 1024/2, 768/2);
	

	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
}

Window::~Window(void)
{
	glfwDestroyWindow(window);
}


void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::Update()
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		isCloseRequested = true;
	}

	if(glfwGetKey(window, GLFW_KEY_F1 ) == GLFW_PRESS)
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else if((glfwGetKey(window, GLFW_KEY_F2 ) == GLFW_PRESS))
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	}

	glfwPollEvents();
}

float Window::GetAspect()
{
	return (float)width / (float)height;
}

int Window::GetHeight()
{
	return height;
}

int Window::GetWidth()
{
	return width;
}

glm::vec2 Window::GetCentre()
{
	return glm::vec2((float)width/2.0f, (float)height/2.0f);
}

GLFWwindow* Window::GetWindow()
{
	return window;
}

bool Window::IsCloseRequested()
{
	return isCloseRequested;
}
