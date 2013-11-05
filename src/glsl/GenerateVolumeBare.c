#version 120

uniform sampler3D Texture0; //volume texture nearest
uniform sampler3D Texture1; //voro texture linear
uniform sampler2D Texture2; //terrain heightmap

uniform float slicesPerPitch;
uniform float heightmapMax;
uniform float bufferedPageSizeInUnits;
uniform float threshVal;
uniform float bufferMult;
uniform float numEntries;
uniform float paramsPerEntry;
uniform vec2 resolution;
uniform vec3 worldSizeInPixels;
uniform vec3 worldMinVisInPixels;
uniform vec3 worldMaxVisInPixels;
uniform vec3 worldMinBufInPixels;
uniform vec3 worldMaxBufInPixels;
uniform vec3 paramArr[256];

uniform vec4 mapFreqs;
uniform vec4 mapAmps;


varying vec2 TexCoord0;




$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$

int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}

/*
mat4 rotationMatrix(vec3 axis, float angle)
{
    vec3 axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}
*/

void main() {
	
	vec4 finalRes = vec4(0.0);

	int iSlicesPerPitch = int(slicesPerPitch);
	int side = iSlicesPerPitch*iSlicesPerPitch;
	float fSideM1 = float(side-1);
	int volGenFBOSize = side*iSlicesPerPitch;
	int volGenFBOSizeM1 = volGenFBOSize-1;
	int i = int( float(volGenFBOSizeM1)*TexCoord0.x );
	int j = int( float(volGenFBOSizeM1)*TexCoord0.y );
	vec3 newCoords = vec3(0.0);
	newCoords.x = float(intMod(i,side))/fSideM1;
	newCoords.y = float(intMod(j,side))/fSideM1;
	newCoords.z = float(i/side + (j/side)*slicesPerPitch)/fSideM1;



	vec3 worldPosInPixels = vec3(
		mix(worldMinBufInPixels.x, worldMaxBufInPixels.x, newCoords.x),
		mix(worldMinBufInPixels.y, worldMaxBufInPixels.y, newCoords.y),
		mix(worldMinBufInPixels.z, worldMaxBufInPixels.z, newCoords.z)
	);


	
	int iParamsPerEntry = int(paramsPerEntry);
	int baseInd;

	float squareMult = 1.0;
	float tempLen = 0.0;
	float curDis = 0.0;
	float curRadThe = 0.0;
	float curRadPhi = 0.0;
	float airVal = 0.0;

	vec3 boundsMinInPixels = vec3(0.0);
	vec3 boundsMaxInPixels = vec3(0.0);
	vec3 originInPixels = vec3(0.0);
	vec3 powerVals = vec3(0.0);
	vec3 coefficients = vec3(0.0);
	vec3 squareVals = vec3(0.0);
	vec3 minMaxMat = vec3(0.0);

	vec2 curDisXY = vec2(0.0);
	vec2 curDisYZ = vec2(0.0);
	vec2 curDisXZ = vec2(0.0);
	
	vec3 dis = vec3(0.0);
	vec3 disNorm = vec3(0.0);
	vec3 absDisNorm = vec3(0.0);
	vec3 absDisNormalized = vec3(0.0);
	vec3 normLen = vec3(0.0);
	vec3 absOriginInPixels = vec3(0.0);
	vec3 absWorldPosInPixels = vec3(0.0);
	vec3 eqRes;
	vec3 cornerDis = vec3(0.5,0.5,0.5);
	vec3 newCornerDis = vec3(0.0);
	vec3 orig = vec3(0.0);

	bvec3 bLessThanOrig;
	vec3 lessThanOrig;

	for (i = 0; i < numEntries; i++) {

		baseInd = i*iParamsPerEntry;

		boundsMinInPixels = paramArr[baseInd+0];
		boundsMaxInPixels = paramArr[baseInd+1];
		

		if (
			
			all( lessThanEqual(worldPosInPixels.xyz,boundsMaxInPixels.xyz) ) &&
			all( greaterThanEqual(worldPosInPixels.xyz,boundsMinInPixels.xyz) )
		) {


			//originInPixels = paramArr[baseInd+2];
			powerVals = paramArr[baseInd+3];
			coefficients = paramArr[baseInd+4];
			squareVals = paramArr[baseInd+5];
			minMaxMat = paramArr[baseInd+6];

			orig = (boundsMaxInPixels - boundsMinInPixels)/2.0;
			absWorldPosInPixels = ((worldPosInPixels)-(orig));

			newCornerDis = cornerDis*orig;

			originInPixels = clamp(worldPosInPixels, boundsMinInPixels + newCornerDis, boundsMaxInPixels - newCornerDis );
			absOriginInPixels = ((originInPixels) - (orig));

			dis = (absWorldPosInPixels - absOriginInPixels);
			absDisNorm = abs( dis.xyz/newCornerDis.xyz );

			bLessThanOrig = lessThanEqual(absWorldPosInPixels,originInPixels);
			lessThanOrig = vec3(bLessThanOrig);
			dis *= lessThanOrig;

			

			// stair-stepping, ribbing, airval


			curDisXY.x = max(absDisNorm.x,absDisNorm.y);
			curDisYZ.x = max(absDisNorm.y,absDisNorm.z);
			curDisXZ.x = max(absDisNorm.x,absDisNorm.z);

			//curRadPhi = atan(absDisNormalized.y, absDisNormalized.x);
			//curRadThe = atan(absDisNormalized.z,length(absDisNormalized.xy));
			// + step(abs(sin(curRadPhi*8.0) ),0.5)*0.1 + step(abs(sin(curRadThe*4.0) ),0.5)*0.1

			if (squareVals.x*squareVals.y*squareVals.z == 1.0) {
				absDisNorm.xyz = vec3( max(max(absDisNorm.x,absDisNorm.y),absDisNorm.z) );
			}
			else {
				absDisNorm.xy = mix(absDisNorm.xy, curDisXY.xx, squareVals.x*squareVals.y );
				absDisNorm.yz = mix(absDisNorm.yz, curDisYZ.xx, squareVals.y*squareVals.z );
				absDisNorm.xz = mix(absDisNorm.xz, curDisXZ.xx, squareVals.z*squareVals.x );
			}


			absDisNorm.z = floor(absDisNorm.z*8.0)/8.0;

			
			eqRes.x = coefficients.x*pow(absDisNorm.x,powerVals.x);
			eqRes.y = coefficients.y*pow(absDisNorm.y,powerVals.y);
			eqRes.z = coefficients.z*pow(absDisNorm.z,powerVals.z);


			tempLen = (eqRes.x + eqRes.y + eqRes.z);

			if (airVal == 1.0) {

			}
			else {
				if ( ( tempLen >= minMaxMat.x ) && (tempLen <= minMaxMat.y ) ) {
					finalRes.a = minMaxMat.z/255.0;
				}
			}
			

			if ( tempLen < minMaxMat.x ) { // || abs(sin(worldPosInPixels.x/100.0)) > 0.5
				airVal = 1.0;
				finalRes.a = 0.0/255.0;
			}

			
			

			

		}

	}


	if (finalRes.a != 0.0) {
		finalRes.r = 1.0;
	}
	
	


	vec4 texHM0 =  texture2D(Texture2, mapFreqs.x * worldPosInPixels.xy / (worldSizeInPixels.x) );
	vec4 texHM1 =  texture2D(Texture2, mapFreqs.y * worldPosInPixels.xy / (worldSizeInPixels.x) );
	vec4 texHM2 =  texture2D(Texture2, mapFreqs.z * worldPosInPixels.xy / (worldSizeInPixels.x) );
	vec4 texHM3 =  texture2D(Texture2, mapFreqs.w * worldPosInPixels.xy / (worldSizeInPixels.x) );

	//float testHeight = texHM0.r*256.0;//(texHM0.r*0.8 + texHM1.r*0.15 + texHM2.r*0.04 + texHM3.r*0.01)*512.0;

	float testHeight = (texHM0.r*mapAmps.x + texHM1.r*mapAmps.y + texHM2.r*mapAmps.z + texHM3*mapAmps.w);// *0.6 + texHM1.r*0.2 + texHM2.r*0.07 + texHM3.r*0.03);
	//testHeight = pow(testHeight,2.0);
	testHeight *= heightmapMax;

	if (testHeight > worldPosInPixels.z ) {
		finalRes = vec4(1.0,0.0,0.0,2.0/255.0);
	}

	gl_FragData[0] = finalRes;


	//vec4 tex1 =  texture3D(Texture1, newCoords);
	//gl_FragData[0] = float(tex1.a > threshVal/255.0)*(vec4(1.0,0.0,0.0,2.0/255.0));

}