#version 330 core
layout (location = 1) in vec3 aPosition;
out vec2 fragCoord;

void main()
{
    fragCoord = aPosition.xy;
    gl_Position = vec4(aPosition, 1.0);
}