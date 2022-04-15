#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec3 a_Color;

out vec2 position;
out vec3 color;

uniform mat4 P;

void main() {
    gl_Position = P * vec4(a_Position, 0.0, 1.0);
    position = a_Position;
    color = a_Color;
}