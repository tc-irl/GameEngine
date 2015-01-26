#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(GLFWwindow* window);
	~Camera(void);
	
	void computeMatricesFromInputs();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void handleMVP(GLuint modelLoc, GLuint viewLoc, GLuint projLoc);
	void cameraUpdate(glm::vec3 pos, glm::vec3 direction);

	glm::vec3 position;
	float speed;// 3 units / second


private:
	GLFWwindow *window;
	//GLFW_KEY_
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	// Initial position : on +Z
	// Initial horizontal angle : toward -Z
	float horizontalAngle;
	// Initial vertical angle : none
	float verticalAngle;
	// Initial Field of View
	float initialFoV;
	float FoV;
	
	float deltaTime;
	double currentTime;

	float mouseSpeed;

	glm::vec3 direction, right,up;
};

