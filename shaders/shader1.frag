#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 vPos;
in vec2 vTex;

in vec4 shadowPos;

uniform vec3 eyePos;
uniform vec3 lightPos;
uniform vec3 lightPos2;

uniform vec3 lightDir2;
uniform float lightAngle2;

uniform vec3 aI;
uniform vec3 sI;
uniform vec3 aI2;
uniform vec3 sI2;

uniform vec3 aK;
uniform vec3 dK;
uniform vec3 sK;
uniform float n;

uniform float roughness;

uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;

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

float isInShadow(vec4 sPos)
{
    vec3 projectedSPos = (sPos.xyz / sPos.w) * 0.5 + 0.5;

    if (projectedSPos.z > (texture(shadowMap, projectedSPos.xy).r) + 0.1) return 1.0;

    return 0.0;
}

void main()
{
    /*
    vec3 l = normalize(lightPos - vPos);
    vec3 r = reflect(-l, normal);
    vec3 v = normalize(eyePos - vPos);
    */

    vec3 l2 = normalize(lightPos2 - vPos);
    vec3 r2 = reflect(-l2, normal);
    vec3 v2 = normalize(eyePos - vPos);

    //https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl#comment18361606_4275343
    //l = l + 0.5 * roughness * fract(sin(dot(l.xy, vec2(12.9898, 78.233))) * 43758.5453);

    float curAngle = dot(l2, normalize(-lightDir2));

    //vec3 color = aI * aK + sI * (texture(diffuseMap, vTex).rgb * max(dot(l, normal), 0.0) + sK * pow(max(dot(r, v), 0.0), n)); //texture(glossMap, vTex).r * 255
    
    vec3 color2 = aI2 * texture(diffuseMap, vTex).rgb;

    if (curAngle > lightAngle2) {
        color2 += (1.0 - isInShadow(shadowPos)) * (sI2 * (texture(diffuseMap, vTex).rgb * max(dot(l2, normal), 0.0) + sK * pow(max(dot(r2, v2), 0.0), n))); //texture(glossMap, vTex).r * 255
    }

    //FragColor = vec4(texture(diffuseMap, vTex).rgb, 1.0);
    FragColor = vec4(color2, 1.0);
}