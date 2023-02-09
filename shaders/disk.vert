#version 330 core
layout (location = 0) in vec3 aPosition;
out vec2 fragCoord;

uniform vec2 centre;

void main()
{
    fragCoord = aPosition.xy + centre;
    gl_Position = vec4(aPosition + vec3(centre, 0), 1.0);
}