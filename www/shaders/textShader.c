uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform float u_Time;

uniform vec2 u_Resolution;
uniform float u_Zoom;
uniform vec2 u_MouseCoords;
//uniform vec4 u_SourceRect;
//uniform vec4 u_DestRect;


varying vec2 v_TexCoords;
varying vec2 v_Position;
varying vec4 v_Data0;
varying vec2 v_MouseCoords;

$

attribute vec4 a_Data0;


$

void main()	{

	vec2 a_TexCoords = uv.xy;
	v_TexCoords = uv.xy;
	v_Data0 = a_Data0;

	float zoomi = 1.0/u_Zoom;

	v_Position.x = position.x*u_Zoom/u_Resolution.x;
	v_Position.y = position.y*u_Zoom/u_Resolution.y;

	v_MouseCoords.x = u_MouseCoords.x / u_Resolution.x;
	v_MouseCoords.y = u_MouseCoords.y / u_Resolution.y;

	gl_Position = vec4( -1.0 + (position.x*u_Zoom/u_Resolution.x)*2.0, 1.0 - (position.y*u_Zoom/u_Resolution.y)*2.0, position.z, 1.0 );

}

$

void main()	{

	
	vec4 heightRT = texture2D( u_Texture0, v_TexCoords );
	vec4 normRT = texture2D( u_Texture1, v_TexCoords );
	vec3 baseNorm = (normRT.rgb-0.5)*2.0;
	baseNorm.z = 0.1;
	baseNorm = normalize(baseNorm);

	vec3 faceNorm = vec3(0.0,0.0,1.0);
	float curDis = heightRT.b;
	float maxDis = 128.0/255.0;
	float startZ = 0.0;
	float endZ = 1.0;
	float lerpPow = 2.0;
	float lerpAmount = max(maxDis-curDis,0.0)/maxDis;
	

	if (curDis <= 16.0/255.0) {
		lerpAmount = 1.0;
	}
	vec3 finalNorm = mix(faceNorm, baseNorm, clamp(pow(lerpAmount,lerpPow),0.0,1.0) );

	finalNorm = normalize(finalNorm);


	float alphaVal = heightRT.g;

	
	if (alphaVal != 0.0) {
		alphaVal = 1.0;
	}

	gl_FragColor = vec4((finalNorm.xy+1.0)/2.0, (40.0)/255.0, alphaVal ); //+sin(v_Position.x*v_Position.y*100.0)*10.0
	
	
}