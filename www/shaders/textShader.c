uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform float u_Time;

uniform vec2 u_Resolution;
uniform float u_Zoom;
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
	gl_Position = vec4( -1.0 + (position.x*u_Zoom/u_Resolution.x)*2.0, 1.0 - (position.y*u_Zoom/u_Resolution.y)*2.0, position.z, 1.0 );

}

$

void main()	{

	vec4 val = texture2D( u_Texture0, v_TexCoords );
	vec4 val2 = texture2D( u_Texture1, v_TexCoords );

	vec3 normVec = normalize( (val2.rgb-0.5)*2.0 );
	vec3 lightVec = normalize(vec3(0.24,0.37,1.0));
	float lVal = dot(normVec,-lightVec);

	vec4 colText = vec4(lVal,lVal,lVal,val.g);
	vec4 colBG = vec4(1.0,0.0,0.0,0.5);

	gl_FragColor = mix(colText,colBG,v_Data0.x);
	//gl_FragColor=vec4(v_TexCoords,0.0,1.0);
}