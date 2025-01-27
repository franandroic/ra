#pragma once

#include <glad/glad.h>

class Texture {

public:

    Texture() = default;

    Texture(int width, int height);

private:

    GLuint TEX;

    int texWidth;

    int texHeight;

public:

    void generateTexture(unsigned char *data);

    GLuint getTextureID();

    void setTexWidth(int width);

    void setTexHeight(int height);

};