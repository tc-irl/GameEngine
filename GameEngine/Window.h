#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#define GLFW_CDECL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <AntTweakBar.h>

class Window
{
public:
	Window(int width, int height, const std::string& title);
	~Window(void);


	void SwapBuffers();
	void Update();
	float GetAspect();
	int GetHeight();
	int GetWidth();
	glm::vec2 GetCentre();
	bool IsCloseRequested();

	GLFWwindow* GetWindow();

private:
	int width;
	int height;
	std::string title;

	GLFWwindow* window;

	bool isCloseRequested;
};

#endif
