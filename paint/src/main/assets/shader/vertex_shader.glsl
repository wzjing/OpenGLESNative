#version 300 es

in vec3 vertexPosition;
//out vec3 position;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vertexPosition, 1);
//    position = vertexPosition;
}