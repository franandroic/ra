#pragma once

#include <glm/glm.hpp>
#include "Transformable.h"
#include "Curve.h"

class Animator {

public:

    Animator(Transformable *inTransformable, Curve *inCurve, std::vector<glm::vec3> *inTangents, std::vector<glm::vec3> *inSecDer);

private:

    Transformable *transformable;

    Curve *curve;

    std::vector<glm::vec3> *tangents;

    std::vector<glm::vec3> *secDer;

    int currAnimIdx;

    int nextAnimIdx;

    glm::vec3 currAnimPos;

    glm::vec3 nextAnimPos;

    glm::vec3 oldFront;

    glm::vec3 oldRight;

    glm::vec3 oldUp;

    glm::vec3 newFront;

    glm::vec3 newRight;

    glm::vec3 newUp;

    bool bSetToAnimate;

    int frameCounter;

public:

    void moveToStartingPosition();

    bool animate();

};