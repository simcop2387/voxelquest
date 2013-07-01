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

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()	{

	
	vec4 baseval = texture2D( u_Texture0, v_TexCoords );

	gl_FragColor = vec4(baseval.aaa,1.0);

	/*
	vec3 finalNorm;
	finalNorm.rg = (baseval.rg-0.5)*2.0;
	finalNorm.b = abs(sqrt( abs(1.0-(finalNorm.r*finalNorm.r+finalNorm.g*finalNorm.g) )));

	finalNorm = normalize(finalNorm);
	
	vec3 lightVec;
	float disVal = min(1.0-clamp(distance(v_MouseCoords, v_Position.xy)*1.0,0.0,1.0)+0.1,1.0);
	lightVec.xy = v_MouseCoords - v_Position.xy;
	//lightVec.xy = normalize(lightVec.xy);
	lightVec.z = 40.0/255.0;
	lightVec = normalize(lightVec);
	lightVec.y = -lightVec.y;


	finalNorm.y *= -1.0;

	///////


	vec4 samp;
	vec4 samp2;
	vec3 tc;

	int i;
	int j;
	int k;
	float fi;
	float fj;
	float fk;
	float dis;

	int loopMax = 128;
	int stepAmount = 8;
	float fLoopMax = float(loopMax);

	
	float totHits = 0.0;
	float totRays = 0.0;
	float bias = 2.0/255.0;

	vec3 offsetNorm;

	float finalVal = 0.0;


	for (i = 0; i <= loopMax; i += stepAmount) {

			fi = float(i);

			offsetNorm = finalNorm*fi;

			tc.x = (offsetNorm.x)/u_Resolution.x;
			tc.y = (offsetNorm.y + offsetNorm.z  )/u_Resolution.y;
			//tc.z = baseval.b + offsetNorm.z/255.0;

			samp = texture2D( u_Texture0, v_TexCoords + tc.xy );


			if (samp.b > baseval.b+bias) {
				finalVal = samp.a*(1.0-fi/fLoopMax);

				i = loopMax*2;
			}




	}


	float resVal = finalVal+baseval.a;
	

	///////





	gl_FragColor = vec4(resVal,resVal,resVal,1.0);//mix(vec4(finalVal,finalVal,finalVal,1.0),vec4(baseval.a,baseval.a,baseval.a,1.0),0.5);
	*/

	
}