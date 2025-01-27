#pragma once

#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>
#include "GLFW/glfw3.h"

class FPSManager {

private:

	GLFWwindow *window;

	std::string windowTitle;

	int targetFPS;

	double currentFrameTime;

	double previousFrameTime;

	double deltaTime;

	double targetFrameTime;

	int frameCounter;

public:

	FPSManager(GLFWwindow *glfwwindow, int FPS, std::string title);

	int getTargetFPS();

	void setTargetFPS(int FPS);
	
	double maintainFPS();
};