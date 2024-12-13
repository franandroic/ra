#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Object.h"
#include "Camera.h"
#include "Curve.h"
#include "Light.h"
#include "ReflectorLight.h"
#include "PaSpObject.h"

class Renderer {

public:

    Renderer(Camera *inCamera, Light *inLight);

private:

    GLint uniformLocationLight;
    GLint uniformLocationAI;
    GLint uniformLocationSI;

    GLint uniformLocationLight2;
    GLint uniformLocationAI2;
    GLint uniformLocationSI2;

    GLint uniformDirectionLight2;
    GLint uniformAngleLight2;

public:

    std::vector<Object *> objects;

    std::vector<PaSpObject *> paspObjects;

    Camera *camera;

    Light *light;

    ReflectorLight *refLight;

    GLuint FBO;

    void renderObjects();

    void renderPaspObjects();

    void renderShadowMap();

    void registerObject(Object *object);

    void registerPaspObject(PaSpObject *paspObject);

    int countObjects();

    int countPaspObjects();

    void setReflectorLight(ReflectorLight *newRefLight);

};