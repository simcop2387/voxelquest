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

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()	{

	vec4 baseval = texture2D( u_Texture0, v_TexCoords );



	vec3 offsetNorm;
	offsetNorm.rg = (baseval.rg-0.5)*2.0;
	offsetNorm.b = abs(sqrt( abs(1.0-(offsetNorm.r*offsetNorm.r+offsetNorm.g*offsetNorm.g) )));

	offsetNorm *= 2.0;

	vec4 samp;
	vec3 tc;

	int i;
	int j;
	int k;
	float fl;
	float fi;
	float fj;
	float fk;
	float dis;

	float fit;
	float fjt;

	const int jMax = 128;
	const int iMax = 227;
	const float fjMax = float(jMax);
	const float fiMax = float(iMax);

	const int loopMax = 32;//10;
	const int stepAmount = 1;
	const float fLoopMax = float(loopMax);

	float pi = 3.14159;

	float totHits = 0.0;
	float totRays = 0.0;

	float hitPower;

	float phi;
	float theta;

	float offsetLen = fLoopMax/4.0;

	//for (i = -loopMax; i <= loopMax; i += stepAmount) {
		//for (j = -loopMax; j <= loopMax; j += stepAmount) {
			//for (k = -loopMax; k <= loopMax; k += stepAmount) {

		for (j = 2; j < jMax; j *= 2) {

			fjt = float(j);

			hitPower = (fjMax-fjt)/fjMax;

			for (i = 0; i < iMax; i++) {

				fit = float(i)*pi/fiMax;

				//fi = float(i);
				//fj = float(j);
				//fk = float(k);

				//fi = rand(vec2(v_TexCoords.x*23.0+fl*7.0, v_TexCoords.y*23.0+fl*22.0));
				//fj = rand(vec2(v_TexCoords.x*52.0+fl*32.0, v_TexCoords.y*16.0+fl*29.0));
				//fk = rand(vec2(v_TexCoords.x*21.0+fl*13.0, v_TexCoords.y*12.0+fl*3.0));

				//fi = (fi-0.5)*fLoopMax;
				//fj = (fj-0.5)*fLoopMax;
				//fk *= fLoopMax;


				theta = 2.0*pi - fit/2.0;
				phi = 20.0*fit;

				fi = fjt*cos(phi)*sin(theta);
				fj = fjt*sin(phi)*sin(theta);
				fk = fjt*cos(theta);

				tc.x = (fi + offsetNorm.x)/u_Resolution.x;
				tc.y = ( (fj - offsetNorm.y) + (fk + offsetNorm.z)  )/u_Resolution.y;
				tc.z = baseval.b + (fk + offsetNorm.z)/255.0;

				dis = clamp(sqrt(fi*fi+fj*fj+fk*fk)/fjt,0.0,1.0);

				samp = texture2D( u_Texture0, v_TexCoords + tc.xy );

				if (samp.b < tc.z) {
					totHits += hitPower;
				}

				totRays += hitPower;
			}
		}
			

			//}

		//}
	//}

	float resVal = (totHits/totRays);

	resVal = clamp(resVal,0.0,1.0);
	resVal = pow(resVal,6.0);

	



	gl_FragColor = vec4(baseval.rgb,resVal);
	//gl_FragColor = vec4(resVal,resVal,resVal,1.0);
	
}