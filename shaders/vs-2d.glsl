#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat3 M;
uniform mat4 P;
void main(){
    vec3 p = M * vec3(aPos, 1.0);
    gl_Position = P * vec4(p.xy, 0.0, 1.0);
}