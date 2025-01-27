#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include "Transformable.h"
#include "ParticleSpawner.h"
#include "Shader.h"

class PaSpObject : public Transformable {

public:

    PaSpObject(glm::vec3 pos, ParticleSpawner *inPs, Shader *inShader);

private:

    ParticleSpawner *particleSpawner;

    Shader *shader;

    GLint uniformLocationModel;
    GLint uniformLocationView;
    GLint uniformLocationPerspective;

    GLint uniformLocationBaseColor;

    GLint uniformLocationTimePassed;

    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::time_point<std::chrono::steady_clock> timeOfLastBatch;
    std::chrono::time_point<std::chrono::steady_clock> timeOfLastCleanup;

    std::chrono::duration<double> seconds_passed;

    void loadParticles();

public:

    void render(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix, double deltaTime);

    Shader *getShader();

    void moveLocation(glm::vec3 newLocation);
};