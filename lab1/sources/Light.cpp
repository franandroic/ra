#include "../headers/Light.h"

Light::Light() : Transformable(false) {

    ambientIntensity = glm::vec3(0.5, 0.5, 0.5);
    sourceIntensity = glm::vec3(1.0, 1.0, 1.0);

    DPwidth = 1024;
    DPheight = 1024;

    generateDepthMap();
}

Light::Light(glm::vec3 aI, glm::vec3 sI) : Transformable(false) {

    ambientIntensity = aI;
    sourceIntensity = sI;

    DPwidth = 1024;
    DPheight = 1024;

    generateDepthMap();
}

void Light::generateDepthMap() {

    glGenTextures(1, &DPT);
    glBindTexture(GL_TEXTURE_2D, DPT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DPwidth, DPheight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

}

glm::vec3 Light::getAmbientIntensity() {

    return ambientIntensity;
}

void Light::setAmbientIntensity(glm::vec3 inaI) {

    ambientIntensity = inaI;
}

glm::vec3 Light::getSourceIntensity() {

    return sourceIntensity;
}

void Light::setSourceIntensity(glm::vec3 insI) {

    sourceIntensity = insI;
}