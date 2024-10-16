#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "InputProfile.h"
#include "Transformable.h"
#include "Animator.h"

class InputManager {

private:

    GLFWwindow *window;

    bool bForward;

    bool bBackward;

    bool bLeft;

    bool bRight;

    bool bUp;
    
    bool bDown;

    glm::vec3 movementVector;

    glm::mat4 rotationMatrix;

    int windowWidth;

    int windowHeight;

    Animator *animator;

    bool bSetToAnimate;

    bool bAnimating;

public:

    InputProfile currentInputProfile;

    Transformable *selectedTransformable;

    InputManager(GLFWwindow *inWindow, int width, int height, InputProfile profile);

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void keyCallbackStatic(GLFWwindow *window, int key, int scancode, int action, int mods);

    void handleInput(Transformable *transformable, bool inFocus);

    void handleKeyboardInput(Transformable *transformable);

    void handleMouseInput(Transformable *transformable, bool inFocus);

    void addAnimator(Animator *inAnimator);

};