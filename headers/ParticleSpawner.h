#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Renderable.h"
#include "Shader.h"
#include "Particle.h"

class ParticleSpawner : public Renderable {

public:

    ParticleSpawner(int inWidth, int inHeight, int inMaxNum);

private:

    int width;

    int height;

    int maxNumOfParticles;

    std::vector<Particle> particles;

    std::vector<glm::vec3> vertices;

public:

    GLuint VBO;

    void draw() override;

    void addParticle(Particle newParticle);

    int countVertices();

    int getWidth();

    int getHeight();

};