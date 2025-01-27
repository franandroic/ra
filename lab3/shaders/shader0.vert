#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
//layout (location = 2) in int aEdge;

out vec3 normal;
out vec3 vPos;
//out int vEdge;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

void main()
{
    vec4 modelPos = modelMatrix * vec4(aPos, 1.0);

    normal = normalize(mat3(transpose(inverse(modelMatrix))) * aNorm);
    vPos = vec3(modelPos);

    //vEdge = aEdge;

    gl_Position = perspectiveMatrix * viewMatrix * modelPos;
}