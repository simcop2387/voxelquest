uniform sampler2D u_Texture0;
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
				//avgVal += baseval.g;

				minVal = mix( minVal, min(dis,minVal), abs(baseval.g - samp.g) );

				/*
				if (baseval.g == samp.g) {

				}
				else {
					minVal = min(dis,minVal);
				}
				*/

			}
			else {
				minVal = min(dis,minVal);
			}

			//totDis += 1.0;

		}
	}

	baseval.b = minVal;// + avgVal/(totDis*4.0);
	baseval.a = 1.0;

	gl_FragColor = baseval;
	//gl_FragColor=vec4(v_TexCoords,0.0,1.0);
}