precision highp float;
uniform sampler2D tex;
uniform float time;
varying vec2 fragCoord;
uniform vec2 iResolution;

float beat = 0.;
float mb(vec2 p1, vec2 p0) { return (0.04+beat)/(pow(p1.x-p0.x,2.)+pow(p1.y-p0.y,2.)); }

void main()
{
	float ct = time;
	if ((ct > 8.0 && ct < 33.5)
	|| (ct > 38.0 && ct < 88.5)
	|| (ct > 93.0 && ct < 194.5))
		beat = pow(sin(ct*3.1416*3.78+1.9)*0.5+0.5,15.0)*0.05;

	vec2 mbr,mbg,mbb;
	vec2 p = (2.0*gl_FragCoord.xy-iResolution.xy)/iResolution.y;
	vec2 o = vec2(pow(p.x,2.),pow(p.y,2.));
	vec3 col = vec3(pow(2.*abs(o.x+o.y)+abs(o.x-o.y),5.));
	col = max(col,1.);
	float t=time+beat*2.;
	
	float t2=t*2.0,t3=t*3.0,s2=sin(t2),s3=sin(t3),s4=sin(t*4.0),c2=cos(t2),c3=cos(t3); // Let me extend this line a little more with an useless comment :-)
	
	mbr = mbg = mbb = vec2(0.);
	mbr += vec2(0.10*s4+0.40*c3,0.40*s2 +0.20*c3);
	mbg += vec2(0.15*s3+0.30*c2,0.10*-s4+0.30*c3);
	mbb += vec2(0.10*s3+0.50*c3,0.10*-s4+0.50*c2);
	
	col.r *= length(mbr.xy-p.xy);
	col.g *= length(mbg.xy-p.xy);
	col.b *= length(mbb.xy-p.xy);
	col   *= pow(mb(mbr,p)+mb(mbg,p)+mb(mbb,p),1.75);
	
	gl_FragColor = vec4(col,1.);
}