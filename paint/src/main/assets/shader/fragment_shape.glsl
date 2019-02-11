precision highp float;
uniform sampler2D iChannel0;
uniform float iGlobalTime;
uniform vec2 iResolution;
void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    uv.x = 0.1*uv.y*sin(10.0*iGlobalTime) + uv.x;
    uv.y = 0.1*uv.x*sin(10.0*iGlobalTime) + uv.y;

    vec4 color = texture2D(iChannel0, vec2(uv.x, 1.0-uv.y));
    gl_FragColor = color;
}