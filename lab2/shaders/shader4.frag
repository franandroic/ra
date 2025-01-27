#version 330 core

out vec4 FragColor;

in vec3 color;
in float lifeTime;

uniform float timePassed;

void main()
{
    float t_val_loc = 0.5 + sin(2 * lifeTime) * 0.5;
    float t_val_glob = 0.5 + sin(timePassed) * 0.5;

    //FragColor = vec4(mix(mix(vec3(0.0, 0.0, 0.0), color, t_val_glob), vec3(1.0, 1.0, 1.0), t_val_loc), 1.0);

    if (lifeTime < 8) {
        FragColor = vec4(mix(vec3(0.8, 0.8, 0.0), color, t_val_loc), 1.0);
    } else {
        FragColor = vec4(mix(vec3(0.5, 0.5, 0.5), vec3(0.0, 0.0, 0.0), t_val_glob), 1.0);
    }
    
}