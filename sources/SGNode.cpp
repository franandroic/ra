#include "../headers/SGNode.h"

SGNode::SGNode() {

    item = nullptr;
    name = "leaf";
    isPaSp = false;
    children.clear();
    nodeModelMatrix = glm::mat4(1);
}

SGNode::SGNode(Transformable *inItem, std::string inName, bool inIsPaSp) {

    item = inItem;
    name = inName;
    isPaSp = inIsPaSp;
    children.clear();
    nodeModelMatrix = glm::mat4(1);
}

void SGNode::moveNode(std::string nodeName, glm::vec3 delta, bool foundNode) {

    if (foundNode == true || name == nodeName) {
        
        if (isPaSp) item->globalMove(delta * 0.5f);
        else item->globalMove(delta);
        
        for (int i = 0; i < children.size(); i++) {
            children[i]->moveNode(nodeName, delta, true);
        }

    } else {
        for (int i = 0; i < children.size(); i++) {
            children[i]->moveNode(nodeName, delta, false);
        }
    }

}

void SGNode::rotateNode(std::string nodeName, glm::vec3 axis, float degrees, bool foundNode) {

    if (foundNode == true || name == nodeName) {
        
        if (!isPaSp) item->rotate(glm::rotate(glm::mat4(1), glm::radians(degrees), axis));
        
        for (int i = 0; i < children.size(); i++) {
            children[i]->rotateNode(nodeName, axis, degrees, true);
        }

    } else {
        for (int i = 0; i < children.size(); i++) {
            children[i]->rotateNode(nodeName, axis, degrees, false);
        }
    }
}