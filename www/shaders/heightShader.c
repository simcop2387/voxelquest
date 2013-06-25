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


	vec4 samp;
	vec2 tc;

	int i;
	int j;
	float fi;
	float fj;
	float dis;

	int loopMax = 10;
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

			dis = clamp(sqrt(fi*fi+fj*fj)/fLoopMax,0.0,1.0);

			samp = texture2D( u_Texture0, v_TexCoords + tc );

			// if the sample is within the letter
			if (baseval.r == samp.r) {
				//avgVal += baseval.g;

				mixVal = abs(baseval.g - samp.g);
				minVal = mix( minVal, min(dis,minVal), mixVal );
				

			}
			else {
				minVal = min(dis,minVal);
			}

			//totDis += 1.0;

		}
	}


	/*
	loopMax = 7;//int(minVal*4.0+4.0);
	fLoopMax = float(loopMax);



	for (i = -loopMax; i <= loopMax; i++) {
		for (j = -loopMax; j <= loopMax; j++) {

			fi = float(i);
			fj = float(j);
			tc.x = fi/512.0;
			tc.y = fj/512.0;

			dis = sqrt(fi*fi+fj*fj)/fLoopMax;

			samp = texture2D( u_Texture0, v_TexCoords + tc );

			// if the sample is within the letter
			if (baseval.r == samp.r) {

				mixVal = abs(baseval.g - samp.g);

				if (baseval.g != samp.g) {
					totVec.x += tc.x*mixVal;
					totVec.y += tc.y*mixVal;
				}
				

			}
			else {

				totVec.x += tc.x;
				totVec.y += tc.y;

			}

		}
	}
	*/


	totVec = (normalize(totVec) + 1.0)/2.0;
	baseval.b = minVal;
	gl_FragColor = baseval;
}