#include "../headers/ParticleSpawner.h"

ParticleSpawner::ParticleSpawner(float inWidth, float inHeight, int inMaxNum, int inBatchSize, double inBatchSpawnFrequency, double inBatchDuration, float inMoveSpeed, int inMoveID, glm::vec3 inBaseColor) {

    width = inWidth;
    height = inHeight;
    maxNumOfParticles = inMaxNum;
    batchSize = inBatchSize;
    batchSpawnFrequency = inBatchSpawnFrequency;
    batchDuration = inBatchDuration;
    moveSpeed = inMoveSpeed;
    baseColor = inBaseColor;
    moveID = inMoveID;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
}

ParticleSpawner::~ParticleSpawner() {

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBO);
}

void ParticleSpawner::draw() {

    glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, lifeTimes.size() * sizeof(float), &(lifeTimes[0]), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *) 0);
        glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void ParticleSpawner::addParticle(Particle newParticle) {

    particles.push_back(newParticle);
    vertices.push_back(newParticle.getPosition());
    creationTimes.push_back(newParticle.creationTime);
    lifeTimes.push_back(0.0);
}

void ParticleSpawner::particleCleanup() {

    std::vector<Particle> tempParticles;
    std::vector<glm::vec3> tempVertices;
    std::vector<std::chrono::time_point<std::chrono::steady_clock>> tempCreationTimes;
    std::vector<float> tempLifeTimes;

    for (int i = 0; i < particles.size(); i++) {
        seconds_passed = std::chrono::steady_clock::now() - creationTimes[i];
        if (seconds_passed.count() < batchDuration) {
            tempParticles.push_back(particles[i]);
            tempVertices.push_back(particles[i].getPosition());
            tempCreationTimes.push_back(particles[i].creationTime);
            tempLifeTimes.push_back(seconds_passed.count());
        }
    }

    particles = tempParticles;
    vertices = tempVertices;
    creationTimes = tempCreationTimes;
    lifeTimes = tempLifeTimes;

    draw();
}

void ParticleSpawner::moveParticles(double deltaTime) {

    for (int i = 0; i < particles.size(); i++) {

        if (moveID == 0) moveDirection = glm::vec3(0.0, 1.0, 0.0);
        else if (moveID == 1) moveDirection = glm::normalize(glm::vec3(0.0, 1.0, 0.0) + (vertices[i] - spawnerLocation));
        else if (moveID == 2) moveDirection = glm::normalize(vertices[i] - spawnerLocation);

        particles[i].globalMove(moveDirection * moveSpeed * (float)deltaTime);
        //std::cout << (float)deltaTime << std::endl;
        vertices[i] = particles[i].getPosition();

        seconds_passed = std::chrono::steady_clock::now() - creationTimes[i];
        lifeTimes[i] = seconds_passed.count();
    }

    draw();
}

int ParticleSpawner::countVertices() {

    return vertices.size();
}

float ParticleSpawner::getWidth() {

    return width;
}

float ParticleSpawner::getHeight() {

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

double ParticleSpawner::getBatchSpawnFrequency() {

    return batchSpawnFrequency;
}

double ParticleSpawner::getBatchDuration() {

    return batchDuration;
}

glm::vec3 ParticleSpawner::getBaseColor() {

    return baseColor;
}

void ParticleSpawner::setSpawnerLocation(glm::vec3 newLocation) {

    spawnerLocation = newLocation;
}

void ParticleSpawner::setMoveDirection(glm::vec3 newDirection) {

    moveDirection = newDirection;
}