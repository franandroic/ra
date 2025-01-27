#pragma once

#include <glm/glm.hpp>
#include "Light.h"

class ReflectorLight : public Light {

public:

    ReflectorLight();

    float nearPlane;

    float farPlane;

    float fovHalfAngle;

    float aspectRatio;

    glm::mat4 getViewMatrix();

    glm::mat4 getPerspectiveMatrix();
};