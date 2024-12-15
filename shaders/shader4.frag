#version 330 core

out vec4 FragColor;

in vec3 color;

uniform float timePassed;

void main()
{
    float t_val = 0.5 + sin(timePassed) * 0.5;

    FragColor = vec4(mix(mix(vec3(0.0, 0.0, 0.0), color, t_val), vec3(1.0, 1.0, 1.0), t_val), 1.0);
}