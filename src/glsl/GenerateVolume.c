#version 120
////////////////  GenerateVolume  ////////////////

uniform sampler3D Texture0; //volume texture linear
uniform sampler3D Texture1; //voro texture nearest
uniform sampler2D Texture2; //2d to 3d coords
varying vec2 TexCoord0;
uniform vec2 resolution;

const float unitsPerDim = 8.0;
const float voroPerDim = 8.0;

/*
vec3 COLOR_MASKS[27] = vec3[](
	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),

	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),

	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),

	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),

	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),

	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),

	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),

	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),

	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 ),
	vec3( 0.0, 0.0, 0.0 )

);
*/


$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$


void main() {

	vec3 coords = texture2D(Texture2, TexCoord0.xy).rgb;

	vec3 newCoords = (coords * 255.0 + 0.5)/256.0;

	//vec4 tex1 =  texture3D(Texture0, floor(newCoords*unitsPerDim)/unitsPerDim);
	vec4 tex1 =  texture3D(Texture0, newCoords);



	int i = 0;
	int j = 0;
	int k = 0;

	float fi = 0.0;
	float fj = 0.0;
	float fk = 0.0;

	float rad = 1.0/voroPerDim;
	vec3 offVec = vec3(0.0,0.0,0.0);
	vec3 sampPoint = vec3(0.0,0.0,0.0);
	vec3 samp = vec3(0.0,0.0,0.0);

	float curDis = 0.0;

	float minDis1 = 999.0;
	float minDis2 = 999.0;

	for (i = -1; i <= 1; i++) {
		offVec.x = float(i);
		for (j = -1; j <= 1; j++) {
			offVec.y = float(j);
			for (k = -1; k <= 1; k++) {
				offVec.z = float(k);

				sampPoint = newCoords+offVec*rad;

				samp = texture3D(Texture1,sampPoint).rgb;

				samp = (samp-0.5)*0.25*rad + sampPoint;

				curDis = distance(samp,newCoords);

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
	}

	tex1.b = 1.0 - (minDis1*2.0/(minDis1+minDis2));
	gl_FragData[0] = tex1;


	/*
	vec4 ress = texture3D(Texture1, newCoords);
	ress.a = tex1.a;
    gl_FragData[0] = ress;
    */
}

