#version 330 core
layout (location = 0) in vec4 aPos;

uniform vec4 scaling;
uniform vec4 translation;
uniform mat4 left_isoclinic;
uniform mat4 right_isoclinic;
void main(){
    gl_Position = translation + left_isoclinic * aPos * right_isoclinic;
}