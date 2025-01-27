#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Transformable.h"

class SGNode {

public:

    SGNode();

    SGNode(Transformable *inItem, std::string inName, bool inIsPaSp);

    Transformable *item;

    std::string name;

    bool isPaSp;

    bool doRotate;

    std::vector<SGNode *> children;

    glm::mat4 nodeModelMatrix;

    void moveNode(std::string nodeName, glm::vec3 delta, bool foundNode);

    void rotateNode(std::string nodeName, glm::vec3 axis, float degrees, bool foundNode, glm::vec3 parentPosition);

    void scaleNode(std::string nodeName, glm::vec3 factor, bool foundNode, glm::vec3 parentPosition);

    void detachNode(std::string nodeName, bool foundNode, std::vector<SGNode *> &detachedNodes);

private:

    glm::vec3 tempVec;

    glm::mat4 tempMat;

};