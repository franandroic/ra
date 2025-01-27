#include "../headers/Texture.h"

Texture::Texture(int width, int height) {

    texWidth = width;
    texHeight = height;
}

void Texture::generateTexture(unsigned char *data) {

    glGenTextures(1, &TEX);
    glBindTexture(GL_TEXTURE_2D, TEX);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

GLuint Texture::getTextureID() {

    return TEX;
}

void Texture::setTexWidth(int width) {

    texWidth = width;
}

void Texture::setTexHeight(int height) {

    texHeight = height;
}