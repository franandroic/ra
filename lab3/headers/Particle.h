#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <chrono>
#include "Transformable.h"

class Particle : public Transformable {

public:

    Particle();

    Particle(std::chrono::time_point<std::chrono::steady_clock> cTime);

    std::chrono::time_point<std::chrono::steady_clock> creationTime;

    glm::vec3 movementDirection;

};
