#include "../headers/Animator.h"

Animator::Animator(Transformable *inTransformable, Curve *inCurve, std::vector<glm::vec3> *inTangents, std::vector<glm::vec3> *inSecDer) {

    transformable = inTransformable;
    curve = inCurve;
    tangents = inTangents;
    secDer = inSecDer;

    nextAnimIdx = 1;

    newFront = glm::vec3(0.0, 0.0, 0.0);
    newRight = glm::vec3(0.0, 0.0, 0.0);
    newUp = glm::vec3(0.0, 0.0, 0.0);

    bSetToAnimate = false;

    frameCounter = 0;
}

void Animator::moveToStartingPosition() {

    transformable->setPosition(curve->getVertex(0));

    newFront = (*tangents)[0];
    newUp = glm::cross((*tangents)[0], (*secDer)[0]);
    newRight = glm::cross(newFront, newUp);
    transformable->setOrientation(newFront, newUp, newRight);

    bSetToAnimate = true;
}

bool Animator::animate() {

    if (!bSetToAnimate) return false;

    if (nextAnimIdx == curve->countVertices()) {

        nextAnimIdx = 1;
        return false;

    } else {

        if (frameCounter >= 30) {

            transformable->setPosition(curve->getVertex(nextAnimIdx));

            newFront = (*tangents)[nextAnimIdx];
            newUp = glm::cross((*tangents)[nextAnimIdx], (*secDer)[nextAnimIdx]);
            newRight = glm::cross(newFront, newUp);
            transformable->setOrientation(newFront, newUp, newRight);

            nextAnimIdx++;
            frameCounter = 0;

        } else {

            frameCounter++;
        
        }

        return true;
    }
}