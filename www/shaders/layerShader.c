uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform float u_Time;
uniform float u_Zoom;
uniform vec2 u_Resolution;

varying vec2 v_TexCoords;
varying vec3 v_Position;

$



$

void main()	{

	vec2 a_TexCoords = uv.xy;
	v_TexCoords = uv.xy;
	v_Position = position.xyz;
	gl_Position = vec4( position.xyz, 1.0 );

}

$

void main()	{

	vec4 baseval = texture2D( u_Texture0, v_TexCoords );
	if (baseval.b <= v_Position.z) { //+(sin(u_Time/1000.0 + v_Position.x*10.0))/20.0
		discard;
	}

	/*
	vec2 nmVec = baseval.rg;
	if (baseval.b != v_Position.z) {
		vec2 nmVec = (baseval.rg-0.5)*2.0;
		nmVec = normalize(nmVec);
		nmVec = (nmVec+1.0)/2.0;
	}
	*/



	gl_FragColor = vec4(baseval.xy,v_Position.z,1.0);//vec4(cp,1.0);
	
}