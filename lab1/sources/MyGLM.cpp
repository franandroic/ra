#include "../headers/MyGLM.h"

namespace MyGLM {

    glm::mat4 translate3D(glm::vec3 translateVector) {

        return glm::mat4(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         translateVector.x, translateVector.y, translateVector.z, 1);

    }

    glm::mat4 scale3D(glm::vec3 scaleVector) {

        return glm::mat4(scaleVector.x, 0, 0, 0,
                         0, scaleVector.y, 0, 0,
                         0, 0, scaleVector.z, 0,
                         0, 0, 0, 1);

    }

    glm::mat4 rotate3D(char axis, float angle) {

        if (axis == 'x')
            return glm::mat4(1, 0, 0, 0,
                             0, glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0,
                             0, -glm::sin(glm::radians(angle)), glm::cos(glm::radians(angle)), 0,
                             0, 0, 0, 1);
        else if (axis == 'y')
            return glm::mat4(glm::cos(glm::radians(angle)), 0, -glm::sin(glm::radians(angle)), 0,
                             0, 1, 0, 0,
                             glm::sin(glm::radians(angle)), 0, glm::cos(glm::radians(angle)), 0,
                             0, 0, 0, 1);
        else if (axis == 'z')
            return glm::mat4(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0, 0,
                             -glm::sin(glm::radians(angle)), glm::cos(glm::radians(angle)), 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1);
        else return glm::mat4(1);

    }

    glm::mat4 lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 viewUp) {

        glm::vec3 vecN = eye - center;
        glm::vec3 vecV = viewUp;
        glm::vec3 vecU = glm::cross(vecN, vecV);

        vecN /= glm::length(vecN);
        vecV /= glm::length(vecV);
        vecU /= glm::length(vecU);

        glm::mat4 matchOrigins = glm::mat4(1, 0, 0, 0,
                                           0, 1, 0, 0,
                                           0, 0, 1, 0,
                                           -eye.x, -eye.y, -eye.z, 1);
        
        glm::mat4 switchSystems = glm::mat4(vecU.x, vecV.x, vecN.x, 0,
                                            vecU.y, vecV.y, vecN.y, 0,
                                            vecU.z, vecV.z, vecN.z, 0,
                                            0, 0, 0, 1);
        
        return switchSystems * matchOrigins;

    }

    glm::mat4 frustum(float l, float r, float b, float t, float n, float f) {

        return glm::mat4((2*n)/(r-l), 0, 0, 0,
                         0, (2*n)/(t-b), 0, 0,
                         (r+l)/(r-l), (t+b)/(t-b), -((f+n)/(f-n)), -1,
                         0, 0, (-2*f*n)/(f-n), 1);
    }
}