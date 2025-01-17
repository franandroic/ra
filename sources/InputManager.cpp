#include "../headers/InputManager.h"

InputManager::InputManager(GLFWwindow *inWindow, int width, int height, SceneGraph *inSceneGraph, SGNode *inCameraNode, SGNode *inCameraMountNode) {

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

    sceneGraph = inSceneGraph;

    x_mouse = 0;

    y_mouse = 0;

    rollDeg = 0.0f;
    rollConstraint = 80.0f;
    pitchDeg = 0.0f;
    pitchConstraint = 45.0f;

    cameraNode = inCameraNode;

    cameraMountNode = inCameraMountNode;

    cameraPitchDeg = 0.0f;
    cameraPitchConstraint = 80.0f;

    reflectorNode = nullptr;

    cameraVelocity = 0.02f;

    vehicleVelocity = 0.025f;

    cameraTurnRate = 1.5f;

    vehicleTurnRate = 0.1f;
    
    bVehicleDestroyed = false;

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
        if (action == GLFW_RELEASE) setInputProfile(InputProfile::FlyingCamera);
    } else if (key == GLFW_KEY_2) {
        if (action == GLFW_RELEASE) setInputProfile(InputProfile::VehicleControl);
    } else if (key == GLFW_KEY_BACKSPACE) {
        if (action == GLFW_RELEASE) selfDestruct();
    }
}

void InputManager::keyCallbackStatic(GLFWwindow *window, int key, int scancode, int action, int mods) {

    InputManager *instance = (InputManager *)glfwGetWindowUserPointer(window);
    instance->keyCallback(window, key, scancode, action, mods);
}

void InputManager::handleInput(Transformable *transformable, bool inFocus) {

    handleKeyboardInput(transformable);
    handleMouseInput(transformable, inFocus, true);

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

void InputManager::handleInput(SGNode *node, bool inFocus) {

    handleKeyboardInput(node->item);
    handleMouseInput(node->item, inFocus, false);

    sceneGraph->moveSubtree(node->name, movementVector);
    
    if (x_mouse < 0) {
        sceneGraph->rotateSubtree(node->name, glm::vec3(0.0f, 1.0f, 0.0f), -cameraTurnRate);
	} else if (x_mouse > 0) {
        sceneGraph->rotateSubtree(node->name, glm::vec3(0.0f, 1.0f, 0.0f), cameraTurnRate);
	}

	if (y_mouse > 0 && cameraPitchDeg > -cameraPitchConstraint) {
        sceneGraph->rotateSubtree(node->name, node->item->getRight(), -cameraTurnRate);
		cameraPitchDeg--;
	} else if (y_mouse < 0 && cameraPitchDeg < cameraPitchConstraint) {
        sceneGraph->rotateSubtree(node->name, node->item->getRight(), cameraTurnRate);
		cameraPitchDeg++;
	}

    if (currentInputProfile == InputProfile::VehicleControl) {
        if (bLeft && rollDeg < rollConstraint) {
            cameraNode->doRotate = false;
            sceneGraph->rotateSubtree(node->name, node->item->getFront(), vehicleTurnRate);
            cameraNode->doRotate = true;
            rollDeg += vehicleTurnRate;
        } else if (!bLeft && rollDeg > 0.0f) {
            cameraNode->doRotate = false;
            sceneGraph->rotateSubtree(node->name, node->item->getFront(), -vehicleTurnRate);
            cameraNode->doRotate = true;
            rollDeg -= vehicleTurnRate;
        } 
        if (bRight && rollDeg > -rollConstraint) {
            cameraNode->doRotate = false;
            sceneGraph->rotateSubtree(node->name, node->item->getFront(), -vehicleTurnRate);
            cameraNode->doRotate = true;
            rollDeg -= vehicleTurnRate;
        } else if (!bRight && rollDeg < 0.0f) {
            cameraNode->doRotate = false;
            sceneGraph->rotateSubtree(node->name, node->item->getFront(), vehicleTurnRate);
            cameraNode->doRotate = true;
            rollDeg += vehicleTurnRate;
        }
        if (bUp && pitchDeg > -pitchConstraint) {
            cameraNode->doRotate = false;
            sceneGraph->rotateSubtree(node->name, node->item->getRight(), -vehicleTurnRate);
            cameraNode->doRotate = true;
            pitchDeg -= vehicleTurnRate;
        } else if (!bUp && pitchDeg < 0.0f) {
            cameraNode->doRotate = false;
            sceneGraph->rotateSubtree(node->name, node->item->getRight(), vehicleTurnRate);
            cameraNode->doRotate = true;
            pitchDeg += vehicleTurnRate;
        }
        if (bDown && pitchDeg < pitchConstraint) {
            cameraNode->doRotate = false;
            sceneGraph->rotateSubtree(node->name, node->item->getRight(), vehicleTurnRate);
            cameraNode->doRotate = true;
            pitchDeg += vehicleTurnRate;
        } else if (!bDown && pitchDeg > 0.0f) {
            cameraNode->doRotate = false;
            sceneGraph->rotateSubtree(node->name, node->item->getRight(), -vehicleTurnRate);
            cameraNode->doRotate = true;
            pitchDeg -= vehicleTurnRate;
        }
    }

    movementVector = glm::vec3(0.0f, 0.0f, 0.0f);
}

void InputManager::handleKeyboardInput(Transformable *transformable) {

    if (currentInputProfile == InputProfile::FlyingCamera) {

        if (bForward) movementVector += -cameraVelocity * transformable->getFront();
        if (bBackward) movementVector += cameraVelocity * transformable->getFront();
        if (bRight) movementVector += cameraVelocity * transformable->getRight();
        if (bLeft) movementVector += -cameraVelocity * transformable->getRight();
        if (bUp) movementVector += cameraVelocity * transformable->getUp();
        if (bDown) movementVector += -cameraVelocity * transformable->getUp();

    } else if (currentInputProfile == InputProfile::VehicleControl) {
        if (bForward) movementVector += vehicleVelocity * transformable->getFront();
        if (bBackward) movementVector += -vehicleVelocity * transformable->getFront();
    }
}

void InputManager::handleMouseInput(Transformable *transformable, bool inFocus, bool calcMat) {

    if (inFocus) {

        x_mouse = 0.0;
        y_mouse = 0.0;
        glfwGetCursorPos(window, &x_mouse, &y_mouse);
        x_mouse = ((2.0 * x_mouse) / windowWidth) - 1.0;
        y_mouse = -1.0 * (((2.0 * y_mouse) / windowHeight) - 1.0);

        if (calcMat) rotationMatrix = transformable->rotateFPS(x_mouse, y_mouse, 85.0f);

        glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
	}
}

void InputManager::addAnimator(Animator *inAnimator) {

    animator = inAnimator;
}

void InputManager::setInputProfile(InputProfile newInputProfile) {

    if (newInputProfile == currentInputProfile) return;

    if (newInputProfile == InputProfile::VehicleControl) {

        sceneGraph->root.children.pop_back();
        cameraMountNode->children.push_back(cameraNode);

        cameraNode->item->setPosition(cameraMountNode->item->getPosition());
        cameraNode->item->setOrientation(-cameraMountNode->item->getFront(), cameraMountNode->item->getUp(), cameraMountNode->item->getRight());

        sceneGraph->moveSubtree(cameraNode->name, 8.0f * cameraNode->item->getUp());
        sceneGraph->moveSubtree(cameraNode->name, 10.0f * cameraNode->item->getFront());
        sceneGraph->rotateSubtree(cameraNode->name, cameraNode->item->getRight(), 25.0f);

    } else {

        cameraMountNode->children.pop_back();
        sceneGraph->root.children.push_back(cameraNode);
    }

    currentInputProfile = newInputProfile;
}

void InputManager::setReflector(SGNode *inReflector) {

    reflectorNode = inReflector;
}

void InputManager::selfDestruct() {

    if (currentInputProfile == InputProfile::FlyingCamera) {

        sceneGraph->destroySubtree(cameraMountNode->name);

        for (int i = 0; i < sceneGraph->detachedNodes.size(); i++) {
            sceneGraph->root.children.push_back(sceneGraph->detachedNodes[i]);
            partDirections.push_back(glm::normalize(glm::vec3(((float) rand() / RAND_MAX) * 2 - 1,
                                                              ((float) rand() / RAND_MAX) * 2 - 1,
                                                              ((float) rand() / RAND_MAX) * 2 - 1)));
        }

        bVehicleDestroyed = true;
    }
}