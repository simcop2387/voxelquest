#version 120
////////////////  GenerateVolume  ////////////////

uniform sampler3D Texture0; //volume texture nearest
uniform sampler3D Texture1; //voro texture linear
uniform sampler2D Texture2; //2d to 3d coords
varying vec2 TexCoord0;
uniform vec2 resolution;

uniform float unitsPerDim;
uniform float voroScale;


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



$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$


void main() {

	float voroMod = (1.0-(1.0/voroScale))/2.0;

	vec3 coords = texture2D(Texture2, TexCoord0.xy).rgb;
	vec3 newCoords = coords;//(coords * 255.0 + 0.5)/256.0;
	vec4 tex1 =  texture3D(Texture0, newCoords);


	int i = 0;
	int j = 0;
	int k = 0;

	float fi = 0.0;
	float fj = 0.0;
	float fk = 0.0;

	//float unitsPerDimScaled = unitsPerDim/voroScale;

	float rad = 1.0/(unitsPerDim);
	vec3 offVec = vec3(0.0,0.0,0.0);
	vec3 sampPoint = vec3(0.0,0.0,0.0);
	vec3 samp = vec3(0.0,0.0,0.0);
	vec3 bestSamp = vec3(0.0,0.0,0.0);
	float curDis = 0.0;
	float minDis1 = 999.0;
	float minDis2 = 999.0;

	const int minrad = -1;
	const int maxrad = 1;
	
	/*
	for (i = minrad; i <= maxrad; i++) {
		offVec.x = float(i);
		for (j = minrad; j <= maxrad; j++) {
			offVec.y = float(j);
			for (k = minrad; k <= maxrad; k++) {
				offVec.z = float(k);

				samp = texture3D(Texture0, floor( (newCoords + offVec*rad)*unitsPerDim )/unitsPerDim ).rgb;
				curDis = distance( samp , newCoords/voroScale + voroMod);

				if (curDis < minDis1) {
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
	}*/

	for (i = 0; i < 27; i++) {
		samp = texture3D(Texture0, floor( (newCoords + COLOR_MASKS[i]*rad)*unitsPerDim )/unitsPerDim ).rgb;

		if ((samp.r + samp.g + samp.b) == 0.0) {

		}
		else {
			curDis = distance( samp , newCoords/voroScale + voroMod);

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

	tex1.a = float(texture3D(Texture1, bestSamp).a > 0.5)*gradVal;
	
	tex1.rgb = bestSamp;
	

	gl_FragData[0] = tex1;

}

