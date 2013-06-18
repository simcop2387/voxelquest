uniform float time;
uniform vec2 basepos;
uniform vec2 resolution;
varying vec2 tc;

$

void main()	{
	tc = uv.xy;
	//tc.y = 1.0-tc.y;


	vec3 pos2 = position.xyz;

	// insert movement here
	//pos2.x += abs(sin(time))/2.0;
	//pos2.y += abs(cos(time*2.0))/2.0;
	//

	pos2.xy += basepos;


	pos2.y *= -1.0;
	pos2.xy *= 2.0;
	pos2.x -= 1.0;


	gl_Position = vec4( pos2.xyz, 1.0 );

}

$

void main()	{
	gl_FragColor=vec4(tc,0.0,1.0);
}