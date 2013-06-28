uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform float u_Time;
uniform float u_Zoom;
uniform vec2 u_Resolution;
uniform vec2 u_MouseCoords;

varying vec2 v_TexCoords;
varying vec2 v_Position;
varying vec2 v_MouseCoords;




$



$

void main()	{

	vec2 a_TexCoords = uv.xy;
	v_TexCoords = uv.xy;
	

	v_Position = position.xy;
	v_MouseCoords = u_MouseCoords;

	/*
	v_Position.x = position.x * m_Zoom / u_Resolution.x;
	v_Position.y = position.y * m_Zoom / u_Resolution.y;
	v_MouseCoords.x = u_MouseCoords.x / u_Resolution.x;
	v_MouseCoords.y = u_MouseCoords.y / u_Resolution.y;
	*/
	
	gl_Position = vec4( position.xyz, 1.0 );

}

$

void main()	{

	vec4 baseval = texture2D( u_Texture0, v_TexCoords );


	vec3 finalNorm;
	finalNorm.rg = (baseval.rg-0.5)*2.0;
	finalNorm.b = abs(sqrt( abs(1.0-(finalNorm.r*finalNorm.r+finalNorm.g*finalNorm.g) )));
	finalNorm = normalize(finalNorm);
	
	vec3 lightVec;
	float disVal = 1.0-clamp(distance(v_MouseCoords, v_Position.xy)*0.5,0.0,1.0);
	lightVec.xy = v_MouseCoords - v_Position.xy;
	//lightVec.xy = normalize(lightVec.xy);
	lightVec.z = 0.2;
	lightVec = normalize(lightVec);
	lightVec.y = -lightVec.y;


	//for (i = 0; i <)


	float lVal = dot(finalNorm,lightVec)*disVal;
	lVal = mix(baseval.a*0.2,lVal,lVal);
	lVal *= float(baseval.b > 0.0);



	gl_FragColor = vec4(lVal,lVal,lVal,1.0);
	

	
}