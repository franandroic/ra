#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Transformable.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

class Object : public Transformable {

private:

    Mesh *mesh;

    Shader *shader;

    Material *material;

    GLint uniformLocationModel;
    GLint uniformLocationView;
    GLint uniformLocationPerspective;

    GLint uniformLocationAK;
    GLint uniformLocationDK;
    GLint uniformLocationSK;
    GLint uniformLocationN;

    GLint uniformTextureDiffuse;
    GLint uniformTextureGloss;

    Shader *shadowShader;

    GLint uniformShadowModel;
    GLint uniformShadowView;
    GLint uniformShadowPerspective;

    GLint uniformLightView;
    GLint uniformLightPerspective;

public:

    Object(Mesh *inMesh, Shader *inShader, Material *inMaterial, Shader *inShadowShader);

    void render(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix, GLuint shadowTexID, glm::mat4 viewLightMatrix, glm::mat4 perspectiveLightMatrix);

    void shadowRender(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix);

    Shader *getShader();

};