uniform mat4 uMatrix;
attribute vec4 vertexCoord;
attribute vec4 textureCoord;
varying vec2 gl_fragCoord;

void main()
{
    gl_Position = uMatrix * vertexCoord;
    gl_fragCoord = textureCoord;
}