#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

void main()
{
    color = vec3(1.0, 1.0, 1.0);
    gl_Position = perspectiveMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    gl_PointSize = 10.0;
}
