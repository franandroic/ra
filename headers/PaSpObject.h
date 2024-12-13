#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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

public:

    void render(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix);

    Shader *getShader();
    
    void loadParticles(int count, glm::vec3 centerPos);
};