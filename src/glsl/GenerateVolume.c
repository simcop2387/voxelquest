#version 120
////////////////  GenerateVolume  ////////////////

uniform sampler3D Texture0; //volume texture nearest
uniform sampler3D Texture1; //voro texture linear
uniform sampler2D Texture2; //2d to 3d coords

uniform vec2 resolution;
uniform float unitsPerDim;
uniform vec3 worldMin;
uniform vec3 worldMax;

varying vec2 TexCoord0;


vec3 COLOR_MASKS[27] = vec3[](
	vec3(  -1.0,  -1.0,  -1.0  ),
	vec3(  -1.0,  -1.0,  0.0  ),
	vec3(  -1.0,  -1.0,  1.0  ),

	vec3(  -1.0,  0.0,  -1.0  ),
	vec3(  -1.0,  0.0,  0.0  ),
	vec3(  -1.0,  0.0,  1.0  ),

	vec3(  -1.0,  1.0,  -1.0  ),
	vec3(  -1.0,  1.0,  0.0  ),
	vec3(  -1.0,  1.0,  1.0  ),


	
	vec3(  0.0,  -1.0,  -1.0  ),
	vec3(  0.0,  -1.0,  0.0  ),
	vec3(  0.0,  -1.0,  1.0  ),

	vec3(  0.0,  0.0,  -1.0  ),
	vec3(  0.0,  0.0,  0.0  ),
	vec3(  0.0,  0.0,  1.0  ),

	vec3(  0.0,  1.0,  -1.0  ),
	vec3(  0.0,  1.0,  0.0  ),
	vec3(  0.0,  1.0,  1.0  ),



	vec3(  1.0,  -1.0,  -1.0  ),
	vec3(  1.0,  -1.0,  0.0  ),
	vec3(  1.0,  -1.0,  1.0  ),

	vec3(  1.0,  0.0,  -1.0  ),
	vec3(  1.0,  0.0,  0.0  ),
	vec3(  1.0,  0.0,  1.0  ),

	vec3(  1.0,  1.0,  -1.0  ),
	vec3(  1.0,  1.0,  0.0  ),
	vec3(  1.0,  1.0,  1.0  )

);



// vec3 COLOR_MASKS[8] = vec3[](
// 	vec3(  -1.0,  -1.0,  -1.0  ),
// 	vec3(  -1.0,  -1.0,  1.0  ),
// 	vec3(  -1.0,  1.0,  -1.0  ),
// 	vec3(  -1.0,  1.0,  1.0  ),
// 	vec3(  1.0,  -1.0,  -1.0  ),
// 	vec3(  1.0,  -1.0,  1.0  ),
// 	vec3(  1.0,  1.0,  -1.0  ),
// 	vec3(  1.0,  1.0,  1.0  )

// );



$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$


void main() {

	vec3 coords = texture2D(Texture2, TexCoord0.xy).rgb;
	vec3 newCoords = coords;//(coords * 255.0 + 0.5)/256.0;
	vec4 tex1 =  texture3D(Texture0, newCoords);
	vec4 tex2 =  texture3D(Texture1, newCoords);

	int i = 0;
	int j = 0;
	int k = 0;

	float fi = 0.0;
	float fj = 0.0;
	float fk = 0.0;


	float rad = 1.0/(unitsPerDim);
	vec3 offVec = vec3(0.0,0.0,0.0);
	vec3 sampPoint = vec3(0.0,0.0,0.0);
	vec3 samp = vec3(0.0,0.0,0.0);
	vec3 bestSamp = vec3(0.0,0.0,0.0);
	float curDis = 0.0;


	float notValid = 999.0;

	float minDis1 = notValid;
	float minDis2 = notValid;

	const int minrad = -1;
	const int maxrad = 1;

	vec3 newCoords2 = newCoords;
	//newCoords2.x *= 0.5;
	//newCoords2.z *= 0.5;
	

	// to create voro scale, just use more dense array for voro data

	for (i = 0; i < 27; i++) {



		samp = texture3D(Texture0, newCoords + COLOR_MASKS[i]*rad ).rgb;

		if ((samp.r + samp.g + samp.b) == 0.0) {

		}
		else {



			curDis = distance( samp, newCoords2 );


			if (curDis < minDis1) {
				bestSamp = samp;

				minDis2 = minDis1;
				minDis1 = curDis;
			}
			else {
				if (curDis < minDis2) {
					minDis2 = curDis;
				}
			}
		}

	}

	float gradVal = clamp(1.0 - (minDis1*2.0/(minDis1+minDis2)),1.0/255.0,1.0);
	
	//(bestSamp.r + bestSamp.g*unitsPerDim + bestSamp.b*unitsPerDim*unitsPerDim);

	
	float rockIsOnTer = float( texture3D(Texture1, bestSamp).a > 126.0/255.0);
	float rockIsInside = rockIsOnTer*float(gradVal > 0.05);
	float isTerrain = float(tex2.a > 126.0/255.0);

	
	vec4 res = vec4(0.0,0.0,0.0,0.0);
	vec4 resA = vec4(0.0,0.0,0.0,0.0);
	vec4 resB = vec4(0.0,0.0,0.0,0.0);

	if ( (minDis1 == notValid) || (minDis2 == notValid) ) {

	}
	else {
		
		//res.a = float( pow(tex2.a,6.0) > 0.01);
		//res.rgb = vec3(0.0,0.0,1.0);

		resA.a = isTerrain;
		resA.a += float( (rockIsInside+isTerrain) > 0.0);
		resA.a = resA.a/255.0;
		resA.rgb = mix(vec3(0.0,0.0,0.0),bestSamp,rockIsInside);

		resB.rgb = bestSamp;
		resB.a = rockIsOnTer*2.0/255.0;

		res = resA;//mix(resA,resB, float( mod(tex2.a*1024.0,69.0)/69.0 > 0.5));
	}


	gl_FragData[0] = res;

}

