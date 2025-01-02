#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SGNode.h"

class SceneGraph {

public:

    SceneGraph();

    SGNode root;

    void moveSubtree(std::string nodeName, glm::vec3 delta);

    void rotateSubtree(std::string nodeName, glm::vec3 axis, float degrees);

    void scaleSubtree(std::string nodeName, glm::vec3 factor);

};