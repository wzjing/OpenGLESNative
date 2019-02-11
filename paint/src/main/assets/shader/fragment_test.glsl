#version 300 es

precision highp float;

//in vec3 position;
out vec4 fragmentColor;

uniform float iGlobalTime;
uniform vec2 iResolution;

void main() {
    float speed = iGlobalTime*2.0;
    vec2 uv = (gl_FragCoord.xy +1.0f)*4.0;
    vec2 uv0=uv;
    float i0=1.0;
    float i1=1.0;
    float i2=1.0;
    float i4=0.0;
    for(int s=0;s<7;s++)
    {
        vec2 r;
        r=vec2(cos(uv.y*i0-i4+speed/i1),sin(uv.x*i0-i4+speed/i1))/i2;
        r+=vec2(-r.y,r.x)*0.3;
        uv.xy+=r;

        i0*=1.93;
        i1*=1.15;
        i2*=1.7;
        i4+=0.05+0.1*speed*i1;
    }
    float r=sin(uv.x-speed)*0.5+0.5;
    float b=sin(uv.y+speed)*0.5+0.5;
    float g=sin((uv.x+uv.y+sin(speed*0.5))*0.5)*0.5+0.5;
    fragmentColor = vec4(r,g,b,1.0);
}
