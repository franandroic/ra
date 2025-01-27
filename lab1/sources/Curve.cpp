#include "../headers/Curve.h"

Curve::Curve(std::vector<glm::vec3> inVertices) {

    controlVertices = inVertices;
    color = glm::vec3(1.0, 1.0, 1.0);

    glGenVertexArrays(1, &(VAO));
    glGenBuffers(1, &(VBO));
}

Curve::~Curve() {

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Curve::addVertex(glm::vec3 newVertex) {

    vertices.push_back(newVertex);
}

int Curve::countVertices() {

    return vertices.size();
}

void Curve::draw() {

    glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

glm::vec3 Curve::getVertex(int idx) {

    return vertices[idx];
}