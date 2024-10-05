#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/scene.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/gtx/vector_angle.hpp>
#include "Renderable.h"

class Mesh : public Renderable {

public:

    Mesh() = default;

    Mesh(aiMesh *aimesh);

private:

    void applyTransform(glm::mat4 mat);

    std::pair<glm::vec3, glm::vec3> getBoundingBox();

public:

    std::vector<glm::vec3> vertices;

    std::vector<glm::vec3> normals;

    std::vector<glm::vec2> uvCoords;

    std::vector<int> indices;

    GLuint VBO[3];

    GLuint EBO;

    void normalize();

    void draw() override;

};