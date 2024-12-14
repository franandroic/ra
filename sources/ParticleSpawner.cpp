#include "../headers/ParticleSpawner.h"

ParticleSpawner::ParticleSpawner(int inWidth, int inHeight, int inMaxNum, int inBatchSize, double inBatchDuration, glm::vec3 inBaseColor) {

    width = inWidth;
    height = inHeight;
    maxNumOfParticles = inMaxNum;
    batchSize = inBatchSize;
    batchDuration = inBatchDuration;
    baseColor = inBaseColor;
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

glm::vec3 ParticleSpawner::getParticlePositionAt(int pos) {

    return particles[pos].getPosition();
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