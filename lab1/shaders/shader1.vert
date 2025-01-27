#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

out vec3 normal;
out vec3 vPos;
out vec2 vTex;

out vec4 shadowPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

uniform mat4 lightViewMatrix;
uniform mat4 lightPerspectiveMatrix;

void main()
{
    vec4 modelPos = modelMatrix * vec4(aPos, 1.0);

    normal = normalize(mat3(transpose(inverse(modelMatrix))) * aNorm);
    vPos = vec3(modelPos);
    vTex = aTex;
    
    shadowPos = lightPerspectiveMatrix * lightViewMatrix * modelMatrix * vec4(aPos, 1.0);
    gl_Position = perspectiveMatrix * viewMatrix * modelPos;
}