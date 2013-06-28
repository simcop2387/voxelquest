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



	vec3 offsetNorm;
	offsetNorm.rg = (baseval.rg-0.5)*2.0;
	offsetNorm.b = abs(sqrt( abs(1.0-(offsetNorm.r*offsetNorm.r+offsetNorm.g*offsetNorm.g) )));


	vec4 samp;
	vec3 tc;

	int i;
	int j;
	int k;
	float fi;
	float fj;
	float fk;
	float dis;

	int loopMax = 8;
	int stepAmount = 2;
	float fLoopMax = float(loopMax);

	float totHits = 0.0;
	float totRays = 0.0;

	float offsetLen = fLoopMax/4.0;

	for (i = -loopMax; i <= loopMax; i += stepAmount) {
		for (j = -loopMax; j <= loopMax; j += stepAmount) {
			for (k = -loopMax; k <= loopMax; k += stepAmount) {

				fi = float(i);
				fj = float(j);
				fk = float(k);

				tc.x = (fi + offsetNorm.x)/u_Resolution.x;
				tc.y = (fj + offsetNorm.y  + (fk + offsetNorm.z)  )/u_Resolution.y;
				tc.z = baseval.b + (fk + offsetNorm.z)/255.0;

				dis = clamp(sqrt(fi*fi+fj*fj+fk*fk)/fLoopMax,0.0,1.0);

				samp = texture2D( u_Texture0, v_TexCoords + tc.xy*dis*dis );

				if (samp.b < tc.z) {
					totHits += 1.0;
				}

				totRays += 1.0;

			}

		}
	}

	float resVal = (totHits/totRays);


	gl_FragColor = vec4(baseval.rgb,resVal);
	//gl_FragColor = vec4(resVal,resVal,resVal,1.0);
	
}