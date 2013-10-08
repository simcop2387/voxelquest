#version 120

uniform sampler3D Texture0; //volume texture nearest
uniform sampler3D Texture1; //voro texture linear
uniform sampler2D Texture2; //2d to 3d coords

uniform vec2 resolution;
uniform float bufferedPageSizeInUnits;
uniform float threshVal;
uniform float bufferMult;
uniform float numEntries;
uniform float paramsPerEntry;

uniform vec3 worldMinVisInPixels;
uniform vec3 worldMaxVisInPixels;
uniform vec3 worldMinBufInPixels;
uniform vec3 worldMaxBufInPixels;

varying vec2 TexCoord0;

uniform float paramArr[256];


$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$


void main() {

	float xx = paramArr[0];

	

	vec3 coords = texture2D(Texture2, TexCoord0.xy).rgb;
	vec3 newCoords = coords;
	vec3 worldPos = vec3(
		mix(worldMinBufInPixels.x, worldMaxBufInPixels.x, newCoords.x),
		mix(worldMinBufInPixels.y, worldMaxBufInPixels.y, newCoords.y),
		mix(worldMinBufInPixels.z, worldMaxBufInPixels.z, newCoords.z)
	);




	int i;

	vec3 

	for (i = 0; i < numEntries; i++) {

	}



	vec3 origin = vec3(2048.0,2048.0,512.0);


	float maxRad = 512.0;

	vec3 dis = (worldPos.xyz - origin.xyz);
	vec3 disNorm = dis/maxRad;
	vec3 absDisNorm = abs(disNorm);
	float disTest = 0.0;
	float tempLen = 0.0;
	float tempLen2 = 0.0;

	float powFactor = 2.0;

	if (
		(absDisNorm.x <= 1.0) &&
		(absDisNorm.y <= 1.0) &&
		(absDisNorm.z <= 1.0)
	) {
		tempLen = pow(absDisNorm.x,powFactor) + pow(absDisNorm.y,powFactor) + pow(absDisNorm.z,powFactor);
		tempLen2 = pow(absDisNorm.x,powFactor) + pow(absDisNorm.z,powFactor);

		if (tempLen > 0.5 && tempLen < 1.0 && tempLen2 > 0.25) {
			disTest = 1.0;
		}
	}

	//float dis = distance(worldPos.xy, origin.xy);
	//float zdis = abs(dis-128.0);

	// float disTest = float(
	// 	(dis > 256.0) &&
	// 	(dis < 512.0) &&
	// 	(worldPos.z < 512.0) &&
	// 	(zdis < 512.0-worldPos.z)

	// );

	gl_FragData[0] = disTest * ( vec4(1.0,0.0,0.0,2.0/255.0) );

	//vec4 tex1 =  texture3D(Texture1, newCoords);
	//gl_FragData[0] = float(tex1.a > threshVal/255.0)*(vec4(1.0,0.0,0.0,2.0/255.0));

}

