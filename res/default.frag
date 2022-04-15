#version 460 core

in vec2 position;
in vec3 color;

void main() {
    gl_FragColor = vec4(color, 1.0);
}