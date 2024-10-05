#include "../headers/Animator.h"

Animator::Animator(Transformable *inTransformable, Curve *inCurve) {

    transformable = inTransformable;
    curve = inCurve;

    currAnimIdx = 0;
    nextAnimIdx = 1;

    currAnimPos = glm::vec3(0.0, 0.0, 0.0);
    nextAnimPos = glm::vec3(0.0, 0.0, 0.0);

    oldFront = transformable->getFront();
    oldRight = transformable->getRight();
    oldUp = transformable->getUp();

    newFront = glm::vec3(0.0, 0.0, 0.0);
    newRight = glm::vec3(0.0, 0.0, 0.0);
    newUp = glm::vec3(0.0, 0.0, 0.0);

}

bool Animator::animate() {

    if (currAnimIdx == curve->countVertices() - 1) {

        currAnimIdx = 0;
        nextAnimIdx = 0;

        return false;

    } else {

        currAnimPos = curve->getVertex(currAnimIdx);
        nextAnimPos = curve->getVertex(nextAnimIdx);
        transformable->globalMove(0.002f * glm::normalize(nextAnimPos - currAnimPos));

        newFront = glm::normalize(currAnimPos - nextAnimPos);
        newRight = glm::normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), newFront));
        newUp = glm::cross(newFront, newRight);
        transformable->setOrientation(newFront, newUp, newRight);

        if (glm::distance(transformable->getPosition(), nextAnimPos) <= 0.005f) {
            transformable->setPosition(nextAnimPos);
            currAnimIdx++;
            nextAnimIdx++;
        }

        return true;
    }
}