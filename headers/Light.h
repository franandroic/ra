#pragma once

#include <glm/glm.hpp>
#include "Transformable.h"

class Light : public Transformable {

public:

    Light();

    Light(glm::vec3 aI, glm::vec3 sI);

private:

    void generateDepthMap();

public:

    glm::vec3 ambientIntensity;

    glm::vec3 sourceIntensity;

    GLuint DPT;

    int DPwidth;

    int DPheight;

};