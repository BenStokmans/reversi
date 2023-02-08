#version 330 core
in vec2 fragCoord;
out vec4 fragColor;

uniform int boardSize;
uniform float lineWidth;
uniform vec4 lineColor;

const float PI = 3.1415926535897932384626433832795;
const vec2 CANVAS_SIZE = vec2(2);

vec2 getCellCoords(vec2 coord, vec2 grid);

void main()
{
    vec2 grid = vec2(boardSize);
    vec2 cellSize = CANVAS_SIZE / grid;

    vec2 cellCoord = getCellCoords(fragCoord, cellSize);
    vec2 cutoff = getCellCoords(vec2(1.0 - lineWidth), cellSize);

    vec2 alpha = step(cutoff, cellCoord);
    if (max(alpha.x, alpha.y) == 0.0) discard;

    fragColor = lineColor;
}

vec2 getCellCoords(vec2 coord, vec2 cellSize) {
    return cos(((2 * PI) / cellSize) * coord);
}