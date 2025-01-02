#include "../headers/InputManager.h"

InputManager::InputManager(GLFWwindow *inWindow, int width, int height) {

    window = inWindow;

    currentInputProfile = InputProfile::FlyingCamera;

    bForward = false;

    bBackward = false;

    bLeft = false;

    bRight = false;

    bUp = false;

    bDown = false;

    movementVector = glm::vec3(0.0f, 0.0f, 0.0f);

    rotationMatrix = glm::mat4(1.0f);

    windowWidth = width;

    windowHeight = height;

    bSetToAnimate = false;

    bAnimating = false;

    glfwSetWindowUserPointer(inWindow, this);
    glfwSetKeyCallback(inWindow, keyCallbackStatic);

}

void InputManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) bForward = true;
		else if (action == GLFW_RELEASE) bForward = false;
	} else if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) bBackward = true;
		else if (action == GLFW_RELEASE) bBackward = false;
	} else if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) bLeft = true;
		else if (action == GLFW_RELEASE) bLeft = false;
	} else if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) bRight = true;
		else if (action == GLFW_RELEASE) bRight = false;
	} else if (key == GLFW_KEY_Q) {
		if (action == GLFW_PRESS) bUp = true;
		else if (action == GLFW_RELEASE) bUp = false;
	} else if (key == GLFW_KEY_E) {
		if (action == GLFW_PRESS) bDown = true;
		else if (action == GLFW_RELEASE) bDown = false;
	} else if (key == GLFW_KEY_ENTER) {
        if (action == GLFW_PRESS) {
            if (bAnimating) bAnimating = false;
            else bSetToAnimate = true;
        }
        else if (action == GLFW_RELEASE) bAnimating = true;
    } else if (key == GLFW_KEY_1) {
        if (action == GLFW_RELEASE) currentInputProfile = InputProfile::FlyingCamera;
    } else if (key == GLFW_KEY_2) {
        if (action == GLFW_RELEASE) currentInputProfile = InputProfile::VehicleControl;
    }
}

void InputManager::keyCallbackStatic(GLFWwindow *window, int key, int scancode, int action, int mods) {

    InputManager *instance = (InputManager *)glfwGetWindowUserPointer(window);
    instance->keyCallback(window, key, scancode, action, mods);
}

void InputManager::handleInput(Transformable *transformable, bool inFocus) {

    handleKeyboardInput(transformable);
    handleMouseInput(transformable, inFocus);

    transformable->globalMove(movementVector);
    transformable->rotate(rotationMatrix);

    movementVector = glm::vec3(0.0f, 0.0f, 0.0f);
    rotationMatrix = glm::mat4(1.0f);

    if (bSetToAnimate) {
        animator->moveToStartingPosition();
        bSetToAnimate = false;
    }

    if (bAnimating) {
	    bAnimating = animator->animate();
    }
}

void InputManager::handleKeyboardInput(Transformable *transformable) {

    if (currentInputProfile == InputProfile::FlyingCamera) {
        if (bForward) movementVector += -0.002f * transformable->getFront();
        if (bBackward) movementVector += 0.002f * transformable->getFront();
        if (bRight) movementVector += 0.002f * transformable->getRight();
        if (bLeft) movementVector += -0.002f * transformable->getRight();
        if (bUp) movementVector += 0.002f * transformable->getUp();
        if (bDown) movementVector += -0.002f * transformable->getUp();
    }
}

void InputManager::handleMouseInput(Transformable *transformable, bool inFocus) {

    if (inFocus) {
        if (currentInputProfile == InputProfile::FlyingCamera) {
            double x_mouse = 0.0;
            double y_mouse = 0.0;
            glfwGetCursorPos(window, &x_mouse, &y_mouse);
            x_mouse = ((2.0 * x_mouse) / windowWidth) - 1.0;
            y_mouse = -1.0 * (((2.0 * y_mouse) / windowHeight) - 1.0);

            rotationMatrix = transformable->rotateFPS(x_mouse, y_mouse, 180.0f);

            glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
        }
	}
}

void InputManager::addAnimator(Animator *inAnimator) {

    animator = inAnimator;
}