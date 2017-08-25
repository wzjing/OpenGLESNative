precision mediump float;
uniform sampler2D tex;
uniform float time;
varying vec2 fragCoord;
void main()
{
    gl_FragColor = texture2D(tex, fragCoord);
    //gl_FragColor = vec4(pow(sin(fragCoord.x), 2.0), pow(sin(fragCoord.y), 2.0), pow(sin(time), 2.0), 1.0);
}