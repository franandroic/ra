#include "../headers/Particle.h"

Particle::Particle() : Transformable(false) {

    creationTime = 0;
    lifeTime = 0;
}

Particle::Particle(float cTime, float lTime) : Transformable(false) {

    creationTime = cTime;
    lifeTime = lTime;
}