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

uniform vec3 paramArr[128];


$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$


void main() {
	

	vec3 coords = texture2D(Texture2, TexCoord0.xy).rgb;
	vec3 newCoords = coords;
	vec3 worldPos = vec3(
		mix(worldMinBufInPixels.x, worldMaxBufInPixels.x, newCoords.x),
		mix(worldMinBufInPixels.y, worldMaxBufInPixels.y, newCoords.y),
		mix(worldMinBufInPixels.z, worldMaxBufInPixels.z, newCoords.z)
	);




	int i;
	int iParamsPerEntry = int(paramsPerEntry);
	int baseInd;

	vec3 boundsMinInPixels = vec3(0.0,0.0,0.0);
	vec3 boundsMaxInPixels = vec3(0.0,0.0,0.0);
	vec3 originInPixels = vec3(0.0,0.0,0.0);
	vec3 powerVals = vec3(0.0,0.0,0.0);
	vec3 coefficients = vec3(0.0,0.0,0.0);
	vec3 minMaxMat = vec3(0.0,0.0,0.0);

	vec3 dis = vec3(0.0,0.0,0.0);
	vec3 disNorm = vec3(0.0,0.0,0.0);
	vec3 absDisNorm = vec3(0.0,0.0,0.0);
	vec3 normLen = vec3(0.0,0.0,0.0);
	float tempLen;

	vec4 finalRes = vec4(0.0,0.0,0.0,0.0);



	for (i = 0; i < numEntries; i++) {

		baseInd = i*iParamsPerEntry;

		boundsMinInPixels = paramArr[baseInd+0];
		boundsMaxInPixels = paramArr[baseInd+1];
		originInPixels = paramArr[baseInd+2];
		powerVals = paramArr[baseInd+3];
		coefficients = paramArr[baseInd+4];
		minMaxMat = paramArr[baseInd+5];
		

		normLen = (boundsMaxInPixels - boundsMinInPixels)/2.0;
		dis = (worldPos.xyz - originInPixels.xyz);
		disNorm.x = dis.x/normLen.x;
		disNorm.y = dis.y/normLen.y;
		disNorm.z = dis.z/normLen.z;
		absDisNorm = abs(disNorm);

		if (
			(absDisNorm.x <= 1.0) &&
			(absDisNorm.y <= 1.0) &&
			(absDisNorm.z <= 1.0)
		) {
			tempLen = 
				coefficients.x*pow(absDisNorm.x,powerVals.x) +
				coefficients.y*pow(absDisNorm.y,powerVals.y) +
				coefficients.z*pow(absDisNorm.z,powerVals.z);

			if (tempLen > minMaxMat.x && tempLen < minMaxMat.y) {
				finalRes.a = minMaxMat.z/255.0;
			}
		}

	}

	if (finalRes.a != 0.0) {
		finalRes.r = 1.0;
	}

	gl_FragData[0] = finalRes;

	//vec4 tex1 =  texture3D(Texture1, newCoords);
	//gl_FragData[0] = float(tex1.a > threshVal/255.0)*(vec4(1.0,0.0,0.0,2.0/255.0));

}

