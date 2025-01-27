#include "../headers/FPSManager.h"

FPSManager::FPSManager(GLFWwindow *glfwwindow, int FPS, std::string title) {

	window = glfwwindow;
	targetFPS = FPS;
	windowTitle = title;
	currentFrameTime = 0.0;
	previousFrameTime = 0.0;
	deltaTime = 0.0;
	targetFrameTime = 0.0;
	frameCounter = 0;
}

int FPSManager::getTargetFPS() {

	return targetFPS;
}

void FPSManager::setTargetFPS(int FPS) {

	targetFPS = FPS;
	targetFrameTime = 1.0 / targetFPS;
}

double FPSManager::maintainFPS() {

	frameCounter++;

	currentFrameTime = glfwGetTime();
	deltaTime = currentFrameTime - previousFrameTime;

	if (deltaTime < targetFrameTime) glfwWaitEventsTimeout(targetFrameTime - deltaTime);

	if (frameCounter == targetFPS / 10) {
		frameCounter = 0;
		glfwSetWindowTitle(window, (windowTitle + " FPS: " + std::to_string(targetFPS)).c_str());
	}

	previousFrameTime = glfwGetTime();

	return deltaTime;
}