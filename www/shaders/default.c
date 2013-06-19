uniform sampler2D u_Texture0;
uniform float u_Time;
uniform vec2 u_Resolution;
uniform vec4 u_SourceRect;
uniform vec4 u_DestRect;


varying vec2 v_TexCoords;
varying vec2 v_Position;


$

void main()	{

	vec2 a_TexCoords = uv.xy;

	v_TexCoords.x = mix(u_SourceRect.x, u_SourceRect.z, a_TexCoords.x);
	v_TexCoords.y = mix(u_SourceRect.y, u_SourceRect.w, a_TexCoords.y);

	vec4 newDest;

	newDest.x = -1.0 +  u_DestRect.x*2.0;
	newDest.y =  1.0 -  u_DestRect.y*2.0;
	newDest.z = -1.0 + (u_DestRect.x + u_DestRect.z)*2.0;
	newDest.w =  1.0 - (u_DestRect.y + u_DestRect.w)*2.0;


	v_Position.x = mix(newDest.x,newDest.z,a_TexCoords.x);
	v_Position.y = mix(newDest.y,newDest.w,a_TexCoords.y);	


	gl_Position = vec4( v_Position.xy, position.z, 1.0 );

}

$

void main()	{

	float val = texture2D( u_Texture0, v_TexCoords ).a;
	gl_FragColor = vec4(1.0,1.0,1.0,val);
	//gl_FragColor=vec4(v_TexCoords,0.0,1.0);
}