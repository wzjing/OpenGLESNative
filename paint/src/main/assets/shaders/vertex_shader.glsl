uniform mat4 uMatrix;
attribute vec4 vertexCoord;
void main()
{
    gl_Position = vertexCoord;
}