#include "../headers/Transformable.h"

Transformable::Transformable(bool isCamera) {

    position = glm::vec3(0, 0, 0);
    if (isCamera) front = glm::vec3(0, 0, -1);
    else front = glm::vec3(0, 0, 1);
    up = glm::vec3(0, 1, 0);
    right = glm::vec3(1, 0, 0);
    scale = glm::vec3(1, 1, 1);
}

glm::mat4 Transformable::getModelMatrix() {

    glm::mat4 rotation_matrix = glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(front, 0), glm::vec4(0, 0, 0, 1));
    glm::mat4 translation_matrix = MyGLM::translate3D(position);
    glm::mat4 scaling_matrix = MyGLM::scale3D(scale);

    return translation_matrix * rotation_matrix * scaling_matrix * glm::mat4(1);
}

void Transformable::rotate(glm::mat4 rot) {

    glm::vec4 tempVector;
    float length;

    tempVector = glm::vec4(front.x, front.y, front.z, 1);
    tempVector = rot * tempVector;
    tempVector /= tempVector.w;
    front = glm::vec3(tempVector.x, tempVector.y, tempVector.z);
    front /= glm::length(front);
    
    tempVector = glm::vec4(up.x, up.y, up.z, 1);
    tempVector = rot * tempVector;
    tempVector /= tempVector.w;
    up = glm::vec3(tempVector.x, tempVector.y, tempVector.z);
    up /= glm::length(up);

    tempVector = glm::vec4(right.x, right.y, right.z, 1);
    tempVector = rot * tempVector;
    tempVector /= tempVector.w;
    right = glm::vec3(tempVector.x, tempVector.y, tempVector.z);
    right /= glm::length(right);
}

void Transformable::globalMove(glm::vec3 delta) {

    position += delta;
}

void Transformable::setOrientation(glm::vec3 newFront, glm::vec3 newUp, glm::vec3 newRight) {
    
    front = newFront;
    up = newUp;
    right = newRight;
}

void Transformable::setPosition(glm::vec3 newPosition) {

    position = newPosition;
}

void Transformable::setScale(glm::vec3 newScale) {

    scale = newScale;
}

glm::mat4 Transformable::rotateFPS(float xOffset, float yOffset, bool constrainPitch) {

    float pitchDeg = 0.0f;
    glm::mat4 rotationMatrix(1);

    if (xOffset < 0) {
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-1.0f), glm::vec3(0, 1, 0));
	} else if (xOffset > 0) {
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(1.0f), glm::vec3(0, 1, 0));
	}

	if (yOffset > 0 && pitchDeg > -constrainPitch) {
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-1.0f), right);
		pitchDeg--;
	} else if (yOffset < 0 && pitchDeg < constrainPitch) {
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(1.0f), right);
		pitchDeg++;
	}

    return rotationMatrix;
}

glm::vec3 Transformable::getFront() {

    return front;
}

glm::vec3 Transformable::getUp() {

    return up;
}

glm::vec3 Transformable::getRight() {

    return right;
}

glm::vec3 Transformable::getPosition() {
    
    return position;
}

//void Transformable::update(float deltaTime);

//void Transformable::registerAnimation(Lines krivulja);