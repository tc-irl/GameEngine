#include "FPSCamera.h"


FPSCamera::FPSCamera(GLFWwindow* window)
{
	this->window = window;

	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	Yaw = YAW;
	Pitch = PITCH;
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	MovementSpeed = SPEED;
	MouseSensitivity = SENSITIVITY;
	Zoom = ZOOM;

	firstMouse = true;
	constrainPitch = true;
}

void FPSCamera::Update(GLfloat deltaTime)
{
	ProcessKeyboard(deltaTime);
	ProcessMouseMovement();
	ProcessMouseScroll();
	UpdateCameraVectors();
}

glm::mat4 FPSCamera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void FPSCamera::ProcessKeyboard(GLfloat deltaTime)
{
	 GLfloat velocity = MovementSpeed * deltaTime;

	 if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	 {
		Position += Front * velocity;
	 }

	 if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	 {
		 Position -= Front * velocity;
	 }

	 if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	 {
		 Position -= Right * velocity;
	 }

	 if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	 {
		 Position += Right * velocity;
	 }
}

void FPSCamera::ProcessMouseMovement()
{
	glfwGetCursorPos(window, &xpos, &ypos);

	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	xoffset = xpos - lastX;
	yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
}

void FPSCamera::ProcessMouseScroll()
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void FPSCamera::UpdateCameraVectors()
{

	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 FPSCamera::GetProjectionMatrix()
{
	return glm::perspective(Zoom, 4.0f / 3.0f, 0.1f, 1000.0f);
}
