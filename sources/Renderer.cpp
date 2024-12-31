#include "../headers/Renderer.h"

Renderer::Renderer(Camera *inCamera, Light *inLight) {

    camera = inCamera;
    light = inLight;
}

void Renderer::renderObjects() {

    for (int i = 0; i < objects.size(); i++) {

        glUseProgram(objects[i]->getShader()->ID);

            glUniform3fv(uniformLocationLight, 1, glm::value_ptr(light->getPosition()));
            glUniform3fv(uniformLocationAI, 1, glm::value_ptr(light->ambientIntensity));
            glUniform3fv(uniformLocationSI, 1, glm::value_ptr(light->sourceIntensity));

            glUniform3fv(uniformLocationLight2, 1, glm::value_ptr(refLight->getPosition()));
            glUniform3fv(uniformLocationAI2, 1, glm::value_ptr(refLight->ambientIntensity));
            glUniform3fv(uniformLocationSI2, 1, glm::value_ptr(refLight->sourceIntensity));

            glUniform3fv(uniformDirectionLight2, 1, glm::value_ptr(refLight->getFront()));
            glUniform1f(uniformAngleLight2, glm::cos(glm::radians(refLight->fovHalfAngle)));

        glUseProgram(0);

        objects[i]->render(objects[i]->getModelMatrix(), camera->getViewMatrix(), camera->getPerspectiveMatrix(), refLight->DPT, refLight->getViewMatrix(), refLight->getPerspectiveMatrix());
    }
}

void Renderer::renderPaspObjects(double deltaTime) {

    for (int i = 0; i < paspObjects.size(); i++) {
        paspObjects[i]->render(paspObjects[i]->getModelMatrix(), camera->getViewMatrix(), camera->getPerspectiveMatrix(), deltaTime);
    }
}

void Renderer::renderShadowMap() {

    for (int i = 0; i < objects.size(); i++) {
        objects[i]->shadowRender(objects[i]->getModelMatrix(), refLight->getViewMatrix(), refLight->getPerspectiveMatrix());
    }
}

void Renderer::registerObject(Object *object) {

    uniformLocationLight = glGetUniformLocation(object->getShader()->ID, "lightPos");
    uniformLocationAI = glGetUniformLocation(object->getShader()->ID, "aI");
    uniformLocationSI = glGetUniformLocation(object->getShader()->ID, "sI");

    uniformLocationLight2 = glGetUniformLocation(object->getShader()->ID, "lightPos2");
    uniformLocationAI2 = glGetUniformLocation(object->getShader()->ID, "aI2");
    uniformLocationSI2 = glGetUniformLocation(object->getShader()->ID, "sI2");

    uniformDirectionLight2 = glGetUniformLocation(object->getShader()->ID, "lightDir2");
    uniformAngleLight2 = glGetUniformLocation(object->getShader()->ID, "lightAngle2");

    objects.push_back(object);
}

void Renderer::registerPaspObject(PaSpObject *paspObject) {

    paspObjects.push_back(paspObject);
}

int Renderer::countObjects() {

    return objects.size();
}

int Renderer::countPaspObjects() {

    return paspObjects.size();
}

void Renderer::setReflectorLight(ReflectorLight *newRefLight) {

    refLight = newRefLight;

    glGenFramebuffers(1, &FBO);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, refLight->DPT, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}