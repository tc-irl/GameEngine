#include "Camera.h"


Camera::Camera(GLFWwindow *window)
{
	this->window = window;
	// Initial position : on +Z
	position = glm::vec3(0, 5, 10); 
	// Initial horizontal angle : toward -Z
	horizontalAngle = 3.14f;
	// Initial vertical angle : none
	verticalAngle = 0.0f;
	// Initial Field of View
	initialFoV = 45.0f;

	speed = 3.0f; // 3 units / second
	mouseSpeed = 0.005f;

	direction = glm::vec3(0,0,-1);

	right = glm::vec3(1,0,0);

	enabled = true;
}

void Camera::computeMatricesFromInputs()
{
	if(glfwGetKey(window,GLFW_KEY_C) == GLFW_PRESS)
	{
		enabled = false;
	}
	else if(glfwGetKey(window,GLFW_KEY_V) == GLFW_PRESS)
	{
		enabled = true;
	}
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if(enabled)
	{
	//	glfwGetCursorPos(window, &xpos, &ypos);
		// Reset mouse position for next frame
		glfwSetCursorPos(window, 1024/2, 768/2);

	
		// Compute new orientation
		horizontalAngle += mouseSpeed * float(1024/2 - xpos );
		verticalAngle   += mouseSpeed * float( 768/2 - ypos );


		// Direction : Spherical coordinates to Cartesian coordinates conversion
			direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle), 
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
			);

		// Right vector
		right = glm::vec3(
			sin(horizontalAngle - 3.14f/2.0f), 
			0,
			cos(horizontalAngle - 3.14f/2.0f)
			);
	
	// Up vector
	}

	up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS)
	{
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS)
	{
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS)
	{
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS)
	{
		position -= right * deltaTime * speed;
	}

	eye = position + direction;

	FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 1000.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
		position,           // Camera is here
		eye,	// and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

glm::mat4 Camera::getViewMatrix()
{
	return ViewMatrix;
}
glm::mat4 Camera::getProjectionMatrix()
{
	return ProjectionMatrix;

}

void Camera::handleMVP(GLuint modelLoc, GLuint viewLoc, GLuint projLoc)
{
	ProjectionMatrix = getProjectionMatrix();
	ViewMatrix = getViewMatrix();



	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &ProjectionMatrix[0][0]);
}

void Camera::cameraUpdate(glm::vec3 pos, glm::vec3 direction)
{
	ViewMatrix = glm::lookAt(pos,direction,up);
}