#include "../headers/ParticleSpawner.h"

ParticleSpawner::ParticleSpawner(int inWidth, int inHeight, int inMaxNum, int inBatchSize, double inBatchDuration, float inMoveSpeed, glm::vec3 inBaseColor) {

    width = inWidth;
    height = inHeight;
    maxNumOfParticles = inMaxNum;
    batchSize = inBatchSize;
    batchDuration = inBatchDuration;
    moveSpeed = inMoveSpeed;
    baseColor = inBaseColor;
}

void ParticleSpawner::draw() {

    glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void ParticleSpawner::addParticle(Particle newParticle) {

    particles.push_back(newParticle);
    vertices.push_back(newParticle.getPosition());
    creationTimes.push_back(newParticle.creationTime);
}

void ParticleSpawner::particleCleanup() {

    std::vector<Particle> tempParticles;
    std::vector<glm::vec3> tempVertices;
    std::vector<std::chrono::time_point<std::chrono::steady_clock>> tempCreationTimes;

    for (int i = 0; i < particles.size(); i++) {
        seconds_passed = std::chrono::steady_clock::now() - creationTimes[i];
        if (seconds_passed.count() < batchDuration) {
            tempParticles.push_back(particles[i]);
            tempVertices.push_back(particles[i].getPosition());
            tempCreationTimes.push_back(particles[i].creationTime);
        }
    }

    particles = tempParticles;
    vertices = tempVertices;
    creationTimes = tempCreationTimes;

    draw();
}

void ParticleSpawner::moveParticles() {

    for (int i = 0; i < particles.size(); i++) {
        particles[i].globalMove(glm::vec3(0.0, 1.0, 0.0) * moveSpeed);
        vertices[i] = particles[i].getPosition();
    }

    draw();
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

int ParticleSpawner::getMaxNumOfParticles() {

    return maxNumOfParticles;
}

Particle ParticleSpawner::getParticleAt(int pos) {

    return particles[pos];
}

glm::vec3 ParticleSpawner::getVertexAt(int pos) {

    return vertices[pos];
}

int ParticleSpawner::getBatchSize() {

    return batchSize;
}

double ParticleSpawner::getBatchDuration() {

    return batchDuration;
}

glm::vec3 ParticleSpawner::getBaseColor() {

    return baseColor;
}