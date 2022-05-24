#version 460 core

layout(std430, binding = 0) buffer Data {
    float data[];
};

uniform int size_x;
uniform int size_y;

in vec2 position;
in vec2 texCoord;

void main() {
    // Texture Coordinates to Array Index
    uint coord_x = int(floor(texCoord.x * size_x));
    uint coord_y = int(floor(texCoord.y * size_y));
    uint index = coord_y * size_x + coord_x;

    // Draw to screen
    gl_FragColor = vec4(data[index], data[index], data[index], 1.0);
}