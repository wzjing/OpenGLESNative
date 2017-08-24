precision mediump float;
uniform sampler2D tex;
attribute int time;
varying vec2 gl_fragCoord;

void main()
{
    gl_FragColor = texture2D(tex, gl_fragCoord);
}