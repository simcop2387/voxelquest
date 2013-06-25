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

$



$

void main()	{

	vec2 a_TexCoords = uv.xy;
	v_TexCoords = uv.xy;
	gl_Position = vec4( position.xyz, 1.0 );

}

$

void main()	{

	vec4 baseval = texture2D( u_Texture0, v_TexCoords );
	vec4 baseval2 = texture2D( u_Texture1, v_TexCoords );

	vec4 samp0;
	vec4 samp1;
	vec2 tc;

	int i;
	int j;
	float fi;
	float fj;
	float dis;

	int loopMax = 5;
	float fLoopMax = float(loopMax);

	float divVal = 2.0*fLoopMax*fLoopMax;
	float totDis = 0.0;
	float totDiv = 0.0;

	float avgVal = 0.0;

	float minVal = 1.0;

	float oldMinVal = 0.0;
	float mixVal;

	vec2 minVec = vec2(0.0,0.0);
	vec2 totVec = vec2(0.0,0.0);



	for (i = -loopMax; i <= loopMax; i++) {
		for (j = -loopMax; j <= loopMax; j++) {

			fi = float(i);
			fj = float(j);
			tc.x = fi/u_Resolution.x;
			tc.y = fj/u_Resolution.y;

			dis = 1.0+(1.0-clamp(sqrt(fi*fi+fj*fj)/fLoopMax,0.0,1.0));

			samp0 = texture2D( u_Texture0, v_TexCoords + tc );
			//samp1 = texture2D( u_Texture1, v_TexCoords + tc );

			// if the sample is within the letter
			if (baseval.r == samp0.r) {

				mixVal = abs(baseval.g - samp0.g);

				if (baseval.g != samp0.g) {
					totVec.x += tc.x*dis*mixVal;
					totVec.y += tc.y*dis*mixVal;
				}
				

			}
			else {

				totVec.x += tc.x*dis;
				totVec.y += tc.y*dis;

			}

		}
	}



	vec3 xx = vec3(normalize(totVec),baseval2.b );

	vec3 totVec2 = (normalize(xx) + 1.0)/2.0;


	gl_FragColor=vec4(totVec2.rg, baseval2.b,1.0);
}