#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "InputProfile.h"
#include "Transformable.h"
#include "Animator.h"
#include "SceneGraph.h"
#include "SGNode.h"

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

    SceneGraph *sceneGraph;

    double x_mouse;

    double y_mouse;

    float rollDeg;
    float rollConstraint;
    float pitchDeg;
    float pitchConstraint;

    SGNode *cameraNode;

    SGNode *cameraMountNode;

    float cameraPitchDeg;
    float cameraPitchConstraint;

    SGNode *reflectorNode;

    float cameraVelocity;

    float vehicleVelocity;

    float cameraTurnRate;

    float vehicleTurnRate;

public:

    InputProfile currentInputProfile;

    Transformable *selectedTransformable;

    InputManager(GLFWwindow *inWindow, int width, int height, SceneGraph *inSceneGraph, SGNode *inCameraNode, SGNode *inCameraMountNode);

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void keyCallbackStatic(GLFWwindow *window, int key, int scancode, int action, int mods);

    void handleInput(Transformable *transformable, bool inFocus);

    void handleInput(SGNode *node, bool inFocus, double deltaTime);

    void handleKeyboardInput(Transformable *transformable);

    void handleMouseInput(Transformable *transformable, bool inFocus, bool calcMat);

    void addAnimator(Animator *inAnimator);

    void setInputProfile(InputProfile newInputProfile);

    void setReflector(SGNode *inReflector);

    void selfDestruct();

    bool bVehicleDestroyed;

    std::vector<glm::vec3> partDirections;

};