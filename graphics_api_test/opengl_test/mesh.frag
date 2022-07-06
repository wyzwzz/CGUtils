#version 460 core
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iUV;
layout(location = 0) out vec4 oFragColor;
uniform sampler2D AlbedoMap;
uniform vec3 LightRadiance;
uniform vec3 LightDir;
void main() {

    vec3 albedo = texture(AlbedoMap,iUV).rgb;
    oFragColor = vec4(albedo,1.0);
//    vec3 normal = normalize(iNormal);
//    vec3 ambient = 0.05 * Color;
//    vec3 diffuse = max(dot(normal,-LightDir),0) * albedo * LightRadiance;
//    vec3 color = ambient + diffuse;
//    color = pow(color,vec3(1.0/2.0));
//    oFragColor = vec4(color, 1.0);
}
