#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "../headers/MyGLM.h"

class Transformable {

public:

    Transformable(bool isCamera);

protected:

    glm::vec3 position;
    
    glm::vec3 front;

    glm::vec3 up;

    glm::vec3 right;

    glm::vec3 scale;
    
    float pitchDeg;

public:

    glm::mat4 getModelMatrix();

    void rotate(glm::mat4 rot);

    void globalMove(glm::vec3 delta);

    void setOrientation(glm::vec3 newFront, glm::vec3 newUp, glm::vec3 newRight);

    void setPosition(glm::vec3 newPosition);

    void setScale(glm::vec3 newScale);

    glm::mat4 rotateFPS(float xOffset, float yOffset, float constrainPitch);

    glm::vec3 getFront();

    glm::vec3 getUp();

    glm::vec3 getRight();

    glm::vec3 getPosition();

    //void update(float deltaTime);

    //void registerAnimation(Lines krivulja);

};