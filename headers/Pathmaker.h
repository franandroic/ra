#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Curve.h"
#include "Shader.h"
#include "Camera.h"

class Pathmaker {

public:

    Pathmaker(Shader *inShader, Camera *inCamera);

private:

    std::vector<glm::vec3> controlVertices;

    std::vector<Curve> curves;

    Shader *shader;

    Camera *camera;

    void makeControlPolygon();

    void makeApproximationCurve();

    void makeInterpolationCurve();

    float factorial(float n);

public:

    void setControlPoint(glm::vec3 point);

    void remakeCurves();

    void renderCurves();

    bool bReadyToAnimate;

    Curve *getAnimationCurve();
};