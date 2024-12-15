#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <chrono>
#include "Renderable.h"
#include "Shader.h"
#include "Particle.h"

class ParticleSpawner : public Renderable {

public:

    ParticleSpawner(int inWidth, int inHeight, int inMaxNum, int inBatchSize, double inBatchDuration, float inMoveSpeed, glm::vec3 inBaseColor);

private:

    int width;

    int height;

    int maxNumOfParticles;

    int batchSize;

    double batchDuration;

    float moveSpeed;

    glm::vec3 baseColor;

    std::vector<Particle> particles;

    std::vector<glm::vec3> vertices;

    std::vector<std::chrono::time_point<std::chrono::steady_clock>> creationTimes;

    std::chrono::duration<double> seconds_passed;

public:

    GLuint VBO[2];

    void draw() override;

    void addParticle(Particle newParticle);

    void particleCleanup();

    void moveParticles();

    int countVertices();

    int getWidth();

    int getHeight();

    int getMaxNumOfParticles();

    Particle getParticleAt(int pos);

    glm::vec3 getVertexAt(int pos);

    int getBatchSize();

    double getBatchDuration();

    glm::vec3 getBaseColor();

};