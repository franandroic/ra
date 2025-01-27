#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

class Material {

public:

    Material();

    Material(glm::vec3 aK, glm::vec3 dK, glm::vec3 sK, float g);

private:

    Texture *diffuseTexture;

    Texture *aoTexture;

    Texture *glossTexture;

public:

    glm::vec3 ambientKoeficient;

    glm::vec3 diffuseKoeficient;

    glm::vec3 specularKoeficient;

    float gloss;

    void addDiffuseTexture(Texture *texture);

    void addGlossTexture(Texture *texture);

    Texture *getDiffuseTexture();

    Texture *getGlossTexture();

};