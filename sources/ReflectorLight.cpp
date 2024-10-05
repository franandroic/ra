#include "../headers/ReflectorLight.h"

ReflectorLight::ReflectorLight() {

    nearPlane = 0.1f;
    farPlane = 10.0f;
    fovHalfAngle = 30.0f;
    aspectRatio = 1.6f;
}

glm::mat4 ReflectorLight::getViewMatrix() {

    return MyGLM::lookAtMatrix(position, position + front, up);
}

glm::mat4 ReflectorLight::getPerspectiveMatrix() {

    float npHeight = 2 * nearPlane * glm::tan(glm::radians(fovHalfAngle));
    float npWidth = npHeight * aspectRatio;

    return MyGLM::frustum(-(npWidth / 2), npWidth / 2, -(npHeight / 2), npHeight / 2, nearPlane, farPlane);
}