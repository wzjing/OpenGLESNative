#version 300 es

in vec3 vertexPosition;
in vec2 vertexUV;
out vec2 UV;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vertexPosition, 1);
    UV = (vertexPosition.xy+1.0)/2.0;
}