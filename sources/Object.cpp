#include "../headers/Object.h"

Object::Object(Mesh *inMesh, Shader *inShader, Material *inMaterial, Shader *inShadowShader) : Transformable(false) {

    mesh = inMesh;
    shader = inShader;
    material = inMaterial;
    shadowShader = inShadowShader;

    uniformLocationModel = glGetUniformLocation(shader->ID, "modelMatrix");
    uniformLocationView = glGetUniformLocation(shader->ID, "viewMatrix");
    uniformLocationPerspective = glGetUniformLocation(shader->ID, "perspectiveMatrix");

    uniformLocationAK = glGetUniformLocation(shader->ID, "aK");
    uniformLocationDK = glGetUniformLocation(shader->ID, "dK");
    uniformLocationSK = glGetUniformLocation(shader->ID, "sK");
    uniformLocationN = glGetUniformLocation(shader->ID, "n");

    uniformShadowModel = glGetUniformLocation(shadowShader->ID, "modelMatrix");
    uniformShadowView = glGetUniformLocation(shadowShader->ID, "viewMatrix");
    uniformShadowPerspective = glGetUniformLocation(shadowShader->ID, "perspectiveMatrix");

    uniformLightView = glGetUniformLocation(shader->ID, "lightViewMatrix");
    uniformLightPerspective = glGetUniformLocation(shader->ID, "lightPerspectiveMatrix");
}

void Object::render(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix, GLuint shadowTexID, glm::mat4 viewLightMatrix, glm::mat4 perspectiveLightMatrix) {

    glUseProgram(shader->ID);

        glUniformMatrix4fv(uniformLocationModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(uniformLocationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(uniformLocationPerspective, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));

        glUniform3fv(uniformLocationAK, 1, glm::value_ptr(material->ambientKoeficient));
        glUniform3fv(uniformLocationDK, 1, glm::value_ptr(material->diffuseKoeficient));
        glUniform3fv(uniformLocationSK, 1, glm::value_ptr(material->specularKoeficient));
        glUniform1f(uniformLocationN, material->gloss);

        glUniformMatrix4fv(uniformLightView, 1, GL_FALSE, glm::value_ptr(viewLightMatrix));
        glUniformMatrix4fv(uniformLightPerspective, 1, GL_FALSE, glm::value_ptr(perspectiveLightMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->getDiffuseTexture()->getTextureID());
        glUniform1i(glGetUniformLocation(shader->ID, "diffuseMap"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadowTexID);
        glUniform1i(glGetUniformLocation(shader->ID, "shadowMap"), 1);
        
        glBindVertexArray(mesh->VAO);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, (GLvoid *) 0);
        glBindVertexArray(0);

    glUseProgram(0);

}

void Object::shadowRender(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix) {

    glUseProgram(shadowShader->ID);

        glUniformMatrix4fv(uniformShadowModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(uniformShadowView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(uniformShadowPerspective, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));

        glBindVertexArray(mesh->VAO);
            glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, (GLvoid *) 0);
        glBindVertexArray(0);

    glUseProgram(0);
}

Shader *Object::getShader() {

    return shader;
}