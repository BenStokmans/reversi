#version 330 core
precision highp float;
in vec2 fragCoord;
out vec4 fragColor;

uniform vec2 centre;

void main()
{
    float R = 1.0;
    float R2 = 0.125;
    float dist = distance(fragCoord, centre);
    if (dist >= R2) {
        discard;
    }

    fragColor = vec4(1.0);
}