#version 300 es

precision highp float;
uniform sampler2D iChannel0;
uniform vec2 iResolution;

in vec2 UV;
out vec4 fragColor;
void main()
{
    fragColor = texture(iChannel0, UV);
}