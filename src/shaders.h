#ifndef REVERSI_RESOURCES_H
#define REVERSI_RESOURCES_H

const char* circleFrag = "#version 330 core\nprecision highp float;\nin vec2 fragCoord;\nout vec4 fragColor;\n\nuniform vec2 centre;\nuniform float radius;\n\nuniform bool white;\n\nvoid main() {\n    float dist = distance(fragCoord, centre);\n    if (dist >= radius) {\n        discard;\n    }\n\n    if (white) {\n        fragColor = vec4(1.0);\n        return;\n    }\n    fragColor = vec4(0, 0, 0, 1.0);\n}";
const char* circleVert = "#version 330 core\nlayout (location = 0) in vec3 aPosition;\nout vec2 fragCoord;\n\nuniform vec2 centre;\n\nvoid main()\n{\n    fragCoord = aPosition.xy + centre;\n    gl_Position = vec4(aPosition + vec3(centre, 0), 1.0);\n}";
const char* gridFrag = "#version 330 core\nin vec2 fragCoord;\nout vec4 fragColor;\n\nuniform int boardSize;\nuniform float lineWidth;\nuniform vec4 lineColor;\n\nconst float PI = 3.1415926535897932384626433832795;\nconst vec2 CANVAS_SIZE = vec2(2);\n\nvec2 getCellCoords(vec2 coord, vec2 grid);\n\nvoid main()\n{\n    vec2 grid = vec2(boardSize);\n    vec2 cellSize = CANVAS_SIZE / grid;\n\n    vec2 cellCoord = getCellCoords(fragCoord, cellSize);\n    vec2 cutoff = getCellCoords(vec2(1.0 - lineWidth), cellSize);\n\n    vec2 alpha = step(cutoff, cellCoord);\n    if (max(alpha.x, alpha.y) == 0.0) discard;\n\n    fragColor = lineColor;\n}\n\nvec2 getCellCoords(vec2 coord, vec2 cellSize) {\n    return cos(((2 * PI) / cellSize) * coord);\n}";
const char* gridVert = "#version 330 core\nlayout (location = 0) in vec3 aPosition;\nout vec2 fragCoord;\n\nvoid main()\n{\n    fragCoord = aPosition.xy;\n    gl_Position = vec4(aPosition, 1.0);\n}";
const char* squareFrag = "#version 330 core\nprecision highp float;\n\nin vec2 fragCoord;\nout vec4 fragColor;\n\nuniform vec4 color;\n\nvoid main() {\n    fragColor = color;\n}";
const char* squareVert = "#version 330 core\nlayout (location = 0) in vec3 aPosition;\nout vec2 fragCoord;\n\nuniform vec2 centre;\n\nvoid main()\n{\n    fragCoord = aPosition.xy + centre;\n    gl_Position = vec4(aPosition + vec3(centre, 0), 1.0);\n}";

#endif //REVERSI_RESOURCES_H
