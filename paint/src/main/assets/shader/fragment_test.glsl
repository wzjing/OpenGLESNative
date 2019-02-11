#version 300 es
precision highp float;
out vec4 fragmentColor;
uniform float iGlobalTime;

void main() {
    float r = sin(gl_FragCoord.x);
    float g = cos(gl_FragCoord.y);
    float b = 0.0f;
    fragmentColor = vec4(r, g, b, 1.0f);
}
