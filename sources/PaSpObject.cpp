#include "../headers/PaSpObject.h"

PaSpObject::PaSpObject(glm::vec3 pos, ParticleSpawner *inPs, Shader *inShader) : Transformable(false) {

    position = pos;
    particleSpawner = inPs;
    shader = inShader;

    uniformLocationModel = glGetUniformLocation(shader->ID, "modelMatrix");
    uniformLocationView = glGetUniformLocation(shader->ID, "viewMatrix");
    uniformLocationPerspective = glGetUniformLocation(shader->ID, "perspectiveMatrix");

    timeOfLastBatch = std::chrono::steady_clock::now();
    timeOfLastCleanup = std::chrono::steady_clock::now();
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

    seconds_passed = std::chrono::steady_clock::now() - timeOfLastBatch;
    if (seconds_passed.count() >= (particleSpawner->getBatchDuration() / 2)) {
        loadParticles();
        timeOfLastBatch = std::chrono::steady_clock::now();
    }
    
    seconds_passed = std::chrono::steady_clock::now() - timeOfLastCleanup;
    if (seconds_passed.count() >= particleSpawner->getBatchDuration()) {
        std::cout << "\t" << seconds_passed.count() << " seconds passed" << std::endl;
        particleSpawner->particleCleanup();
        timeOfLastCleanup = std::chrono::steady_clock::now();
    }
    
}

Shader *PaSpObject::getShader() {

    return shader;
}

void PaSpObject::loadParticles() {

    if (particleSpawner->countVertices() >= particleSpawner->getMaxNumOfParticles()) return;

    float tempX;
    float tempZ;
    Particle tempParticle;

    for (int i = 0; i < particleSpawner->getBatchSize(); i++) {
        tempX = -1 * (particleSpawner->getWidth() / 2) + (((float)rand() / RAND_MAX) * particleSpawner->getWidth());
        tempZ = -1 * (particleSpawner->getHeight() / 2) + (((float)rand() / RAND_MAX) * particleSpawner->getHeight());
        tempParticle.setPosition(glm::vec3(getPosition().x + tempX, getPosition().y, getPosition().z + tempZ));
        tempParticle.creationTime = std::chrono::steady_clock::now();
        particleSpawner->addParticle(tempParticle);
    }

    particleSpawner->draw();
}