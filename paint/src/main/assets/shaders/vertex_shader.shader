uniform mat4 uMatrix;
attribute vec4 vertexCoord;
attribute vec2 textureCoord;
varying vec2 fragCoord;
void main()
{
    fragCoord = textureCoord;
    gl_Position = vertexCoord;
}