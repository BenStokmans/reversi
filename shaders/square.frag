#version 330 core
precision highp float;

in vec2 fragCoord;
out vec4 fragColor;

uniform vec4 color;

void main() {
    fragColor = color;
}