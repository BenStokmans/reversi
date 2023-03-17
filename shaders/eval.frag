#version 330 core
precision highp float;

in vec2 fragCoord;
out vec4 fragColor;

uniform float opacity;
uniform float divider;

void main() {
    vec3 col = vec3(1.0);
    if (fragCoord.y < divider) {
        col = vec3(0);
    }
    fragColor = vec4(col, opacity);
}