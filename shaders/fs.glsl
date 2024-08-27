#version 330 core
out vec4 fragColor;

uniform bool trigger;
void main(){
    fragColor = trigger ? vec4(0.0, 1.0, 0.0, 1.0) : vec4(1.0, 0.0, 0.0, 1.0);
}