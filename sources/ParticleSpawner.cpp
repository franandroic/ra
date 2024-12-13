#include "../headers/ParticleSpawner.h"

ParticleSpawner::ParticleSpawner(int inWidth, int inHeight, int inMaxNum) {

    width = inWidth;
    height = inHeight;
    maxNumOfParticles = inMaxNum;
}

void ParticleSpawner::draw() {

    glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void ParticleSpawner::addParticle(Particle newParticle) {

    particles.push_back(newParticle);
    vertices.push_back(newParticle.getPosition());
}

int ParticleSpawner::countVertices() {

    return vertices.size();
}

int ParticleSpawner::getWidth() {

    return width;
}

int ParticleSpawner::getHeight() {

    return height;
}