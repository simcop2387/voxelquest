#version 120

uniform sampler3D Texture0; //volume texture nearest
uniform sampler3D Texture1; //voro texture linear
uniform sampler2D Texture2; //2d to 3d coords

uniform vec2 resolution;
uniform float bufferedPageSizeInUnits;
uniform vec3 worldMin;
uniform vec3 worldMax;
uniform float threshVal;

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

	vec4 tex1Clamped = texture3D(Texture0, newCoords+0.5/bufferedPageSizeInUnits);
	//vec4 tex2Clamped = texture3D(Texture1, newCoords - 1.2/bufferedPageSizeInUnits);

	//vec4 tex2Clamped = texture3D(Texture1, floor(newCoords*bufferedPageSizeInUnits + 0.5)/bufferedPageSizeInUnits );


	vec4 res = vec4(0.0,0.0,0.0,0.0);

	int i = 0;
	int j = 0;
	int k = 0;

	float fi = 0.0;
	float fj = 0.0;
	float fk = 0.0;


	float rad = 1.0/(bufferedPageSizeInUnits);
	vec4 samp =     vec4(0.0,0.0,0.0,0.0);
	vec4 bestSamp = vec4(0.0,0.0,0.0,0.0);
	float curDis = 0.0;


	float notValid = 999.0;

	float minDis1 = notValid;
	float minDis2 = notValid;

	const int minrad = -1;
	const int maxrad = 1;

	vec3 newCoords2 = newCoords;
	


	newCoords2.x *= tex2.r;
	newCoords2.y *= tex2.g;
	newCoords2.z *= tex2.b;
	




	float terrainAbove = 0.0;


	// to create voro scale, just use more dense array for voro data

	for (i = 0; i < 27; i++) {



		samp = texture3D(Texture0, newCoords + COLOR_MASKS[i]*rad );
		terrainAbove += float( (COLOR_MASKS[i].z > 0.0)&&(samp.a > threshVal/255.0) );

		if ((samp.r + samp.g + samp.b) == 0.0) {

		}
		else {
			

			curDis = distance( samp.rgb, newCoords2 );


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

	terrainAbove = float(terrainAbove > 3.0);//clamp(terrainAbove,0.0,1.0);

	float gradVal = clamp(1.0 - (minDis1*2.0/(minDis1+minDis2)),1.0/255.0,1.0);
	
	//(bestSamp.r + bestSamp.g*bufferedPageSizeInUnits + bestSamp.b*bufferedPageSizeInUnits*bufferedPageSizeInUnits);

	
	float rockIsOnTer = float( texture3D(Texture1, bestSamp.rgb).a > threshVal/255.0);
	float rockIsInside = rockIsOnTer*float(gradVal > 0.05);
	float isTerrain = float(tex2.a > threshVal/255.0);

	//vec3 normSamp;
	

	if ( (minDis1 == notValid) || (minDis2 == notValid) ) {

	}
	else {
		

		if (terrainAbove == 0.0 && tex1Clamped.a < 1.0) {
			res.a = isTerrain;
			res.a += float( (rockIsInside+isTerrain) > 0.0);
			res.rgb = mix(vec3(0.0,0.0,0.0),bestSamp.rgb,res.a);
			res.a = res.a/255.0;
		}
		else {
			res.a = rockIsOnTer/255.0; //*mix(1.0, 2.0, terrainAbove )
			res.rgb = bestSamp.rgb;
		}


	}

	gl_FragData[0] = res;

}

