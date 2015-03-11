#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD, BACKWARD, LEFT, RIGHT
};

const GLfloat YAW = - 90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;

class FPSCamera
{
public:
	FPSCamera(GLFWwindow* window);
	~FPSCamera(void);

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat Yaw, Pitch, MovementSpeed, MouseSensitivity, Zoom;


	GLFWwindow* window;
	bool firstMouse, constrainPitch;

	double xoffset, yoffset;
	double xpos, ypos;
	double lastY;
	double lastX;
	void Update(GLfloat deltaTime);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void ProcessKeyboard(GLfloat deltaTime);
	void ProcessMouseMovement();
	void ProcessMouseScroll();
	void UpdateCameraVectors();
};

