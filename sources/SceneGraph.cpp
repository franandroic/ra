#include "../headers/SceneGraph.h"

SceneGraph::SceneGraph() {

    root = SGNode(nullptr, "root", false);
}

void SceneGraph::moveSubtree(std::string nodeName, glm::vec3 delta) {

    root.moveNode(nodeName, delta, false);
}

void SceneGraph::rotateSubtree(std::string nodeName, glm::vec3 axis, float degrees) {

    root.rotateNode(nodeName, axis, degrees, false, glm::vec3(0.0f, 0.0f, 0.0f));
}

void SceneGraph::scaleSubtree(std::string nodeName, glm::vec3 factor) {

    root.scaleNode(nodeName, factor, false, glm::vec3(0.0f, 0.0f, 0.0f));
}