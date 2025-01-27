#include "../headers/Particle.h"

Particle::Particle() : Transformable(false) {

    creationTime = std::chrono::steady_clock::now();
}

Particle::Particle(std::chrono::time_point<std::chrono::steady_clock> cTime) : Transformable(false) {

    creationTime = cTime;
}