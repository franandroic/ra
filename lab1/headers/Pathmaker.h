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

    std::vector<glm::vec3> tangentVectors;

    std::vector<glm::vec3> secDerVectors;

    Shader *shader;

    Camera *camera;

    void makeControlPolygon();

    void makeApproximationCurve();

    void makeInterpolationCurve();

    void makeBSplineCurve();
    
    void makeTangents();

    float factorial(float n);

public:

    void setControlPoint(glm::vec3 point);

    void remakeCurves();

    void renderCurves(int firstCurveIdx, int lastCurveIdx);

    bool bReadyToAnimate;

    Curve *getAnimationCurve();

    std::vector<glm::vec3> *getAnimationTangents();

    std::vector<glm::vec3> *getAnimationSecDer();
};