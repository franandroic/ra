#include "../headers/Material.h"

Material::Material() {

    ambientKoeficient = glm::vec3(0.0, 0.0, 0.0);
    diffuseKoeficient = glm::vec3(0.0, 0.0, 0.0);
    specularKoeficient = glm::vec3(0.0, 0.0, 0.0);
    gloss = 0.0;
}

Material::Material(glm::vec3 aK, glm::vec3 dK, glm::vec3 sK, float g) {

    ambientKoeficient = aK;
    diffuseKoeficient = dK;
    specularKoeficient = sK;
    gloss = g;
}

void Material::addDiffuseTexture(Texture *texture) {

    diffuseTexture = texture;
}

void Material::addGlossTexture(Texture *texture) {

    glossTexture = texture;
}

Texture *Material::getDiffuseTexture() {

    return diffuseTexture;
}

Texture *Material::getGlossTexture() {

    return glossTexture;
}