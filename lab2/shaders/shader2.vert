#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 color;

uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

uniform vec3 aColor;

void main()
{
    color = aColor;
    gl_Position = perspectiveMatrix * viewMatrix * vec4(aPos, 1.0);
}