#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Renderable.h"

class Curve : public Renderable {

public:

    Curve() = default;

    Curve(std::vector<glm::vec3> inVertices);

    ~Curve();

private:

    std::vector<glm::vec3> controlVertices;

    std::vector<glm::vec3> vertices;

public:

    GLuint VBO;

    glm::vec3 color;

    void addVertex(glm::vec3 newVertex);

    int countVertices();

    void draw() override;

    glm::vec3 getVertex(int idx);

};