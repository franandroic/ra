#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aLifeTime;

out vec3 color;
out float lifeTime;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

uniform vec3 baseColor;

void main()
{
    color = baseColor;
    lifeTime = aLifeTime;
    gl_Position = perspectiveMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    gl_PointSize = 10.0;
}
