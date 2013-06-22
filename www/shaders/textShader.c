uniform sampler2D u_Texture0;
uniform float u_Time;

uniform vec2 u_Resolution;
//uniform vec4 u_SourceRect;
//uniform vec4 u_DestRect;


varying vec2 v_TexCoords;
varying vec4 v_Data0;

$

attribute vec4 a_Data0;


$

void main()	{

	vec2 a_TexCoords = uv.xy;
	v_TexCoords = uv.xy;
	v_Data0 = a_Data0;
	gl_Position = vec4( -1.0 + (position.x/u_Resolution.x)*2.0, 1.0 - (position.y/u_Resolution.y)*2.0, position.z, 1.0 );

}

$

void main()	{

	float val = texture2D( u_Texture0, v_TexCoords ).a;

	vec4 colText = vec4(1.0,1.0,1.0,val);
	vec4 colBG = vec4(1.0,0.0,0.0,0.5);

	gl_FragColor = mix(colText,colBG,v_Data0.x);
	//gl_FragColor=vec4(v_TexCoords,0.0,1.0);
}