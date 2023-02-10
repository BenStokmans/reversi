#version 330 core
precision highp float;
in vec2 fragCoord;
out vec4 fragColor;

uniform vec2 centre;
uniform float radius;

uniform vec4 color;

void main() {
    float dist = distance(fragCoord, centre);
    if (dist >= radius) {
        discard;
    }
    float delta = fwidth(dist);
    float alpha = smoothstep(radius-delta, radius, dist);
    fragColor = vec4(color.x, color.y, color.z, clamp(color.w-alpha, 0, 1));
}