precision highp float;
uniform sampler2D tex;
uniform float time;
uniform vec2 iResolution;
varying vec2 fragCoord;

void main()
{
    float speed = time*2.0;
    vec2 uv = (gl_FragCoord.xy / iResolution.xy-0.5)*8.0;
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
    gl_FragColor = vec4(r,g,b,1.0);
}