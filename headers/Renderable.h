#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Renderable {

public:

    Renderable() = default;

    GLuint VAO;

    virtual void draw() = 0;

};