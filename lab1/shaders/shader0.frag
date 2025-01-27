#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 vPos;
//in int vEdge;

uniform vec3 eyePos;
uniform vec3 lightPos;
uniform vec3 lightPos2;

uniform vec3 aI;
uniform vec3 aI2;
uniform vec3 sI;
uniform vec3 sI2;

uniform vec3 aK;
uniform vec3 dK;
uniform vec3 sK;
uniform float n;

uniform vec3 darkest;
uniform vec3 darker;
uniform vec3 medium;
uniform vec3 lighter;
uniform vec3 lightest;

vec3 cutoff(vec3 v1, vec3 v2)
{
    vec3 returnColor;

    if (v1.x + v2.x <= 1.0) returnColor.x = v1.x + v2.x;
    else returnColor.x = 1.0;

    if (v1.y + v2.y <= 1.0) returnColor.y = v1.y + v2.y;
    else returnColor.y = 1.0;

    if (v1.z + v2.z <= 1.0) returnColor.z = v1.z + v2.z;
    else returnColor.z = 1.0;

    return returnColor;
}

void main()
{
    vec3 l = normalize(lightPos - vPos);
    vec3 r = reflect(-l, normal);
    vec3 v = normalize(eyePos - vPos);

    vec3 l2 = normalize(lightPos2 - vPos);
    vec3 r2 = reflect(-l2, normal);
    vec3 v2 = normalize(eyePos - vPos);

    vec3 color1 = aI * aK + sI * (dK * max(dot(l, normal), 0.0) + sK * pow(max(dot(r, v), 0.0), n));
    vec3 color2 = aI2 * aK + sI2 * (dK * max(dot(l2, normal), 0.0) + sK * pow(max(dot(r2, v2), 0.0), n));

    /*
    if (vEdge == 1) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    */

    vec3 color = color1 + color2;

    if (color.x + color.y + color.z < 0.2) FragColor = vec4(cutoff(darkest, color), 1.0);
    else if (color.x + color.y + color.z < 0.4) FragColor = vec4(cutoff(darker, color), 1.0);
    else if (color.x + color.y + color.z < 0.6) FragColor = vec4(cutoff(medium, color), 1.0);
    else if (color.x + color.y + color.z < 0.8) FragColor = vec4(cutoff(lighter, color), 1.0);
    else FragColor = vec4(cutoff(lightest, color), 1.0);
} 