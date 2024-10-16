#include "../headers/Pathmaker.h"

Pathmaker::Pathmaker(Shader *inShader, Camera *inCamera) {

    shader = inShader;
    camera = inCamera;
    bReadyToAnimate = false;
}

void Pathmaker::setControlPoint(glm::vec3 point) {

    controlVertices.push_back(point);
}

void Pathmaker::makeControlPolygon() {

    if (controlVertices.size() < 2) return;

    Curve curve(controlVertices);

    for (int i = 0; i < controlVertices.size(); i++) {
        curve.addVertex(controlVertices[i]);
    }

    curves.push_back(curve);
}

void Pathmaker::makeApproximationCurve() {

    if (controlVertices.size() < 3) return;

    Curve curve(controlVertices);

    float t;
    float b;
    glm::vec3 p;

    float n = controlVertices.size() - 1;
    float nFact = factorial(n);

    for (int i = 0; i <= 100; i++) {

        t = (1.0 / 100.0) * i;
        p = glm::vec3(0.0, 0.0, 0.0);

        for (int j = 0; j <= n; j++) {
            b = (nFact / (factorial(j) * factorial(n - j))) * glm::pow(t, j) * glm::pow(1 - t, n - j);
            p += controlVertices[j] * b;
        }

        curve.addVertex(p);
    }

    curve.color = glm::vec3(1.0, 0.0, 1.0);

    curves.push_back(curve);
}

void Pathmaker::makeInterpolationCurve() {

    if (controlVertices.size() < 4) return;

    int n = controlVertices.size();

    std::vector<glm::vec3> lastFour;
    lastFour.push_back(controlVertices[n - 4]);
    lastFour.push_back(controlVertices[n - 3]);
    lastFour.push_back(controlVertices[n - 2]);
    lastFour.push_back(controlVertices[n - 1]);

    Curve curve(lastFour);

    float t;
    glm::vec3 p;

    glm::mat4 A(18.0f, 0.0f, 0.0f, 0.0f,
                -33.0f, 54.0f, -27.0f, 6.0f,
                21.0f, -81.0f, 81.0f, -21.0f,
                -6.0f, 27.0f, -54.0f, 33.0f);
    
    A = ((float) (1.0 / 18.0)) * A;

    glm::mat4 P(glm::vec4(lastFour[0], 0.0f),
                glm::vec4(lastFour[1], 0.0f),
                glm::vec4(lastFour[2], 0.0f),
                glm::vec4(lastFour[3], 0.0f));

    glm::mat4 aComp = P * A;

    glm::vec3 a0(aComp[0].x, aComp[0].y, aComp[0].z);
    glm::vec3 a1(aComp[1].x, aComp[1].y, aComp[1].z);
    glm::vec3 a2(aComp[2].x, aComp[2].y, aComp[2].z);
    glm::vec3 a3(aComp[3].x, aComp[3].y, aComp[3].z);

    for (int i = 0; i <= 100; i++) {

        t = (1.0 / 100.0) * i;
        p = a0 +
            a1 * ((float) (3 * t - 3 * glm::pow(t, 2) + glm::pow(t, 3))) +
            a2 * ((float) (3 * glm::pow(t, 2) - 2 * glm::pow(t, 3))) +
            a3 * ((float) glm::pow(t, 3));
        
        curve.addVertex(p);
    }

    curve.color = glm::vec3(0.2, 0.2, 1.0);

    curves.push_back(curve);
}

void Pathmaker::makeBSplineCurve() {

    if (controlVertices.size() < 4) return;

    Curve curve(controlVertices);

    int n = controlVertices.size();

    glm::mat4 B(-1.0f, 3.0f, -3.0f, 1.0f,
                3.0f, -6.0f, 0.0f, 4.0f,
                -3.0f, 3.0f, 3.0f, 1.0f,
                1.0f, 0.0f, 0.0f, 0.0f);

    B = (1.0f / 6.0f) * B;

    std::vector<glm::vec3> lastFour;
    glm::mat4 R;
    glm::mat4 BR;
    float t;
    glm::vec4 T;
    glm::vec4 p;

    for (int idx = 0; idx < n - 3; idx++) {

        lastFour.clear();

        lastFour.push_back(controlVertices[idx]);
        lastFour.push_back(controlVertices[idx + 1]);
        lastFour.push_back(controlVertices[idx + 2]);
        lastFour.push_back(controlVertices[idx + 3]);

        R = glm::mat4(glm::vec4(lastFour[0], 0.0f),
                      glm::vec4(lastFour[1], 0.0f),
                      glm::vec4(lastFour[2], 0.0f),
                      glm::vec4(lastFour[3], 0.0f));

        BR = B * glm::transpose(R);

        for (int i = 0; i < 20; i++) {

            t = (1.0f / 20.0f) * i;
            
            T = glm::vec4(glm::pow(t, 3), glm::pow(t, 2), t, 1);
            p = T * BR;
            curve.addVertex(glm::vec3(p));

            T = glm::vec4(3 * glm::pow(t, 2), 2 * t, 1, 0);
            p = T * BR;
            tangentVectors.push_back(glm::normalize(glm::vec3(p)));

            T = glm::vec4(6 * t, 2, 0, 0);
            p = T * BR;
            secDerVectors.push_back(glm::normalize(glm::vec3(p)));
        }
    }

    curve.color = glm::vec3(0.8, 0.1, 0.1);

    curves.push_back(curve);
}

void Pathmaker::makeTangents() {

    if (curves.size() < 4) return;

    Curve tangents;

    for (int i = 0; i < tangentVectors.size(); i += 10) {
        tangents.addVertex(curves[3].getVertex(i));
        tangents.addVertex(curves[3].getVertex(i) + 0.5f * tangentVectors[i]);
    }

    tangents.color = glm::vec3(0.9, 0.9, 0.0);

    curves.push_back(tangents);
}

void Pathmaker::remakeCurves() {

    curves.clear();
    bReadyToAnimate = false;

    makeControlPolygon();
    makeApproximationCurve();
    makeInterpolationCurve();
    makeBSplineCurve();
    makeTangents();

    for (int i = 0; i < curves.size(); i++) {
        curves[i].draw();
    }

    bReadyToAnimate = true;
}

void Pathmaker::renderCurves(int firstCurveIdx, int lastCurveIdx) {

    if (lastCurveIdx > curves.size() - 1) return;

    for (int i = firstCurveIdx; i <= lastCurveIdx; i++) {

        GLint uniformLocationView = glGetUniformLocation(shader->ID, "viewMatrix");
        GLint uniformLocationPerspective = glGetUniformLocation(shader->ID, "perspectiveMatrix");
        GLint uniformLocationColor = glGetUniformLocation(shader->ID, "aColor");

        glUseProgram(shader->ID);
        glUniformMatrix4fv(uniformLocationView, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
        glUniformMatrix4fv(uniformLocationPerspective, 1, GL_FALSE, glm::value_ptr(camera->getPerspectiveMatrix()));
        glUniform3f(uniformLocationColor, curves[i].color.x, curves[i].color.y, curves[i].color.z);
        glBindVertexArray(curves[i].VAO);
        if (i == 4) glDrawArrays(GL_LINES, 0, curves[i].countVertices());
        else glDrawArrays(GL_LINE_STRIP, 0, curves[i].countVertices());
        glBindVertexArray(0);
        glUseProgram(0);

    }
}

float Pathmaker::factorial(float n) {

    float value = 1;

    for (float i = 1; i <= n; i++) {
        value *= i;
    }

    return value;
}

Curve *Pathmaker::getAnimationCurve() {

    return &curves[3];
}

std::vector<glm::vec3> *Pathmaker::getAnimationTangents() {

    return &tangentVectors;
}

std::vector<glm::vec3> *Pathmaker::getAnimationSecDer() {

    return &secDerVectors;
}