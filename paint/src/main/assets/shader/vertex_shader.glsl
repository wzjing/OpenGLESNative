uniform mat4 projectionMatrix;
attribute vec4 vertexCoord;
void main()
{
    gl_Position = vertexCoord;
}