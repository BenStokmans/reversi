#version 330 core
precision highp float;
in vec2 fragCoord;
out vec4 fragColor;

uniform vec2 centre;
uniform float radius;

uniform bool white;

void main() {
    float dist = distance(fragCoord, centre);
    if (dist >= radius) {
        discard;
    }

    if (white) {
        fragColor = vec4(1.0);
        return;
    }
    fragColor = vec4(0, 0, 0, 1.0);
}