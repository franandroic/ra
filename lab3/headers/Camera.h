#pragma once

#include <glm/glm.hpp>
#include "Transformable.h"

class Camera : public Transformable {

public:

    Camera();

    Camera(float nPlane, float fPlane, float angle, float aspect);

private:

    float nearPlane;

    float farPlane;

    float fovHalfAngle;

    float aspectRatio;

public:

    glm::mat4 getViewMatrix();

    glm::mat4 getPerspectiveMatrix();

    void setZoom(float newZoom);

    glm::vec3 getEyePos();

};