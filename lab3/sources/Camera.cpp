#include "../headers/Camera.h"

Camera::Camera() : Transformable(true) {

    nearPlane = 1;
    farPlane = 10;
    fovHalfAngle = 45;
    aspectRatio = 1.6;
}

Camera::Camera(float nPlane, float fPlane, float angle, float aspect) : Transformable(true) {
    
    nearPlane = nPlane;
    farPlane = fPlane;
    fovHalfAngle = angle;
    aspectRatio = aspect;
}

glm::mat4 Camera::getViewMatrix() {

    return MyGLM::lookAtMatrix(position, position - front, up);
}

glm::mat4 Camera::getPerspectiveMatrix() {

    float npHeight = 2 * nearPlane * glm::tan(glm::radians(fovHalfAngle));
    float npWidth = npHeight * aspectRatio;

    return MyGLM::frustum(-(npWidth / 2), npWidth / 2, -(npHeight / 2), npHeight / 2, nearPlane, farPlane);
}

void Camera::setZoom(float newZoom) {

    
}

glm::vec3 Camera::getEyePos() {

    return position;
}