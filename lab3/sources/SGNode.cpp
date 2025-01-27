#include "../headers/SGNode.h"

SGNode::SGNode() {

    item = nullptr;
    name = "leaf";
    isPaSp = false;
    children.clear();
    nodeModelMatrix = glm::mat4(1);
    doRotate = true;
}

SGNode::SGNode(Transformable *inItem, std::string inName, bool inIsPaSp) {

    item = inItem;
    name = inName;
    isPaSp = inIsPaSp;
    children.clear();
    nodeModelMatrix = glm::mat4(1);
    doRotate = true;
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

void SGNode::rotateNode(std::string nodeName, glm::vec3 axis, float degrees, bool foundNode, glm::vec3 parentPosition) {

    if (!doRotate) return;

    if (foundNode == true) {
        
        tempVec = item->getPosition() - parentPosition;
        tempMat = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);

        item->setPosition(parentPosition);
        item->rotate(tempMat);
        item->setPosition(parentPosition + glm::vec3(tempMat * glm::vec4(tempVec, 1.0f)));
        
        for (int i = 0; i < children.size(); i++) {
            children[i]->rotateNode(nodeName, axis, degrees, true, parentPosition);
        }

    } else if (name == nodeName) {

        item->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis));

        for (int i = 0; i < children.size(); i++) {
            children[i]->rotateNode(nodeName, axis, degrees, true, item->getPosition());
        }

    } else {
        
        for (int i = 0; i < children.size(); i++) {
            children[i]->rotateNode(nodeName, axis, degrees, false, parentPosition);
        }

    }
}

void SGNode::scaleNode(std::string nodeName, glm::vec3 factor, bool foundNode, glm::vec3 parentPosition) {

    if (foundNode == true) {
        
        tempVec = item->getPosition() - parentPosition;
        
        item->setPosition(parentPosition);
        item->setScale(factor);
        item->setPosition(parentPosition + (tempVec * factor));
        
        for (int i = 0; i < children.size(); i++) {
            children[i]->scaleNode(nodeName, factor, true, parentPosition);
        }

    } else if (name == nodeName) {

        item->setScale(factor);

        for (int i = 0; i < children.size(); i++) {
            children[i]->scaleNode(nodeName, factor, true, item->getPosition());
        }

    } else {

        for (int i = 0; i < children.size(); i++) {
            children[i]->scaleNode(nodeName, factor, false, parentPosition);
        }

    }
}

void SGNode::detachNode(std::string nodeName, bool foundNode, std::vector<SGNode *> &detachedNodes) {

    if (foundNode == true || name == nodeName) {

        for (int i = 0; i < children.size(); i++) {
            children[i]->detachNode(nodeName, true, detachedNodes);
            detachedNodes.push_back(children[i]);
        }

        children.clear();

    } else {

        for (int i = 0; i < children.size(); i++) {
            children[i]->detachNode(nodeName, false, detachedNodes);
        }
    }
}