#include "../headers/PaSpObject.h"

PaSpObject::PaSpObject(ParticleSpawner *inPs, Shader *inShader) : Transformable(false) {

    particleSpawner = inPs;
    shader = inShader;

    uniformLocationModel = glGetUniformLocation(shader->ID, "modelMatrix");
    uniformLocationView = glGetUniformLocation(shader->ID, "viewMatrix");
    uniformLocationPerspective = glGetUniformLocation(shader->ID, "perspectiveMatrix");
}

void PaSpObject::render(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix) {

    glUseProgram(shader->ID);

        glUniformMatrix4fv(uniformLocationModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(uniformLocationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(uniformLocationPerspective, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));
        
        glBindVertexArray(particleSpawner->VAO);
        glDrawArrays(GL_POINTS, 0, particleSpawner->countVertices());
        glBindVertexArray(0);
    
    glUseProgram(0);
}

Shader *PaSpObject::getShader() {

    return shader;
}

void PaSpObject::loadParticles(int count, glm::vec3 centerPos) {

    float tempX;
    float tempZ;
    float tempMin;
    float tempMax;
    Particle tempParticle;

    for (int i = 0; i < count; i++) {
        tempX = -1 * (particleSpawner->getWidth() / 2) + (float)rand() * particleSpawner->getWidth();
        tempZ = -1 * (particleSpawner->getHeight() / 2) + (float)rand() * particleSpawner->getHeight();
        tempParticle.setPosition(glm::vec3(tempX, position.y, tempZ));
        particleSpawner->addParticle(tempParticle);
    }

    particleSpawner->draw();
}