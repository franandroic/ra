#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Transformable.h"

class Particle : public Transformable {

public:

    Particle();

    Particle(float cTime, float lTime);

private:

    float creationTime;

    float lifeTime;

    glm::vec3 color;

    glm::vec3 movementDirection;

};
