#version 120

uniform sampler3D Texture0; //volume texture nearest
uniform sampler3D Texture1; //volume texture linear
uniform sampler2D Texture2; //terrain heightmap
//uniform sampler3D Texture3; //voro nearest

uniform float slicesPerPitch;
uniform float heightmapMax;
uniform float bufferedPageSizeInUnits;
uniform float threshVal;
uniform float bufferMult;
uniform float numEntries;
uniform float paramsPerEntry;
uniform float seaLevel;
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

vec3 voroOffsets[27] = vec3[](
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

int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}

float intModF(float v1, int rhs) {
    int lhs = int(v1);
    return lhs - ( (lhs/rhs)*rhs );
}

vec3 intModFV(vec3 v1, ivec3 rhs) {
    ivec3 lhs = ivec3(v1);
    return vec3(lhs - ( (lhs/rhs)*rhs ));
}


vec3 rand(vec3 co) {
    return vec3(
    	fract(sin(dot(co.xyz ,vec3(12.989, 78.233, 98.422))) * 43758.8563),
    	fract(sin(dot(co.zyx ,vec3(93.989, 67.345, 54.256))) * 24634.6345),
    	fract(sin(dot(co.yxz ,vec3(43.332, 93.532, 43.734))) * 56445.2345)
    );
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

vec3 vecRot( vec4 quat, vec3 vec )
{
	return vec + 2.0 * cross( cross( vec, quat.xyz ) + quat.w * vec, quat.xyz );
}

vec3 quatRot(vec3 v1, vec3 v2) {
	vec4 q;
	vec3 a = cross(v1, v2);
	q.xyz = a;
	q.w = sqrt( pow(length(v1), 2.0) * pow(length(v2), 2.0) ) + dot(v1, v2);
	q = normalize(q);

	//q.w = 6.28318530717958 - q.w;

	return vecRot(q,v2);
}

bool brick(vec3 uvw) {
	vec3 color;
	vec3 position, useBrick;
	
	vec3 bricksize = vec3(256.0,128.0,256.0);
	vec3 brickPct = vec3(0.8,0.8,0.8);

	position = uvw.xyz / bricksize.xyz;

	if (fract(position.y * 0.5) > 0.5){
		position.x += 0.5;
        //position.y += 0.5;
	}

	// if (fract(position.x * 0.5) > 0.5){
	// 	position.y += 0.5;
	// 	position.z += 0.5;
	// }

	// if (fract(position.z * 0.5) > 0.5){
	// 	position.x += 0.5;
	// 	position.y += 0.5;
	// }
    
	position = fract(position - 0.1);
	useBrick = step(position, brickPct.xyz);

	return (useBrick.x * useBrick.y * useBrick.z) > 0.0;
}

bool checker(vec3 uvw) {
	float checkSize = 64.0;
	float checkSizeO2 = checkSize/2.0;
	float fmodResult = mod(
		float(fract(uvw.x/checkSize)>0.5) +
		float(fract(uvw.y/checkSize)>0.5),
		//+ float(fract(uvw.z/checkSize)>0.5),
		2.0
	);
	return (fmodResult < 1.0);
}


void main() {
	
	vec4 finalRes = vec4(0.0);

	int iSlicesPerPitch = int(slicesPerPitch);
	int side = iSlicesPerPitch*iSlicesPerPitch;
	float fSideM1 = float(side);
	int volGenFBOSize = side*iSlicesPerPitch;
	int volGenFBOSizeM1 = volGenFBOSize;
	int i = int( float(volGenFBOSizeM1)*TexCoord0.x );
	int j = int( float(volGenFBOSizeM1)*TexCoord0.y );
	int k;
	vec3 newCoords = vec3(0.0);
	newCoords.x = float(intMod(i,side))/fSideM1;
	newCoords.y = float(intMod(j,side))/fSideM1;
	newCoords.z = float(i/side + (j/side)*slicesPerPitch)/fSideM1;


	vec3 upVec = vec3(0.0,0.0,1.0);

	vec3 worldPosInPixels = vec3(
		mix(worldMinBufInPixels.x, worldMaxBufInPixels.x, newCoords.x),
		mix(worldMinBufInPixels.y, worldMaxBufInPixels.y, newCoords.y),
		mix(worldMinBufInPixels.z, worldMaxBufInPixels.z, newCoords.z)
	);

	ivec3 iWorldPosInPixels = ivec3(worldPosInPixels);

	
	int iParamsPerEntry = int(paramsPerEntry);
	int baseInd;

	float squareMult = 1.0;
	float tempLen = 0.0;
	float curDis = 0.0;
	float curRad = 0.0;
	float curThe = 0.0;
	float curPhi = 0.0;
	float airVal = 0.0;

	vec3 boundsMinInPixels = vec3(0.0);
	vec3 boundsMaxInPixels = vec3(0.0);
	vec3 cornerDisInPixels = vec3(0.0);
	vec3 powerVals = vec3(0.0);
	vec3 coefficients = vec3(0.0);
	vec3 squareVals = vec3(0.0);
	vec3 minMaxMat = vec3(0.0);

	vec3 originInPixels = vec3(0.0);

	vec2 curDisXY = vec2(0.0);
	vec2 curDisYZ = vec2(0.0);
	vec2 curDisXZ = vec2(0.0);
	
	vec3 orig = vec3(0.0);

	vec3 dis = vec3(0.0);
	vec3 disNorm = vec3(0.0);
	vec3 absDisNorm = vec3(0.0);
	vec3 normLen = vec3(0.0);
	vec3 absOriginInPixels = vec3(0.0);
	vec3 absWorldPosInPixels = vec3(0.0);
	vec3 eqRes;
	vec3 cornerDis = vec3(0.0);
	vec3 newCornerDis = vec3(0.0);
	vec3 radInPixels = vec3(0.0);

	vec2 cutRad = vec2(0.0);
	vec2 maxCut = vec2(0.0);
	vec4 cutCenter = vec4(0.0);
	vec4 cutVec = vec4(0.0);
	vec4 cutVecAbs = vec4(0.0);

	bool doCont = false;
	float curMat = 0.0;
	float PI = 3.14159;
	float PIO2 = PI/2.0;
	float PIO4 = PI/4.0;

	bvec4 cutBools = bvec4(false);

	bvec3 bLessThanOrig;
	vec3 lessThanOrig;
	vec3 iLessThanOrig;

	vec3 zeroVec = vec3(0.0);

	vec3 newUVW = vec3(0.0);

	vec3 planeDis = vec3(0.0);

	vec3 vecN = vec3(0.0);
	vec3 vecT = vec3(0.0);
	vec3 vecB = vec3(0.0);


	
	//vec2 isRidge = vec2(0.0);
	


	for (i = 0; i < numEntries; i++) {

		baseInd = i*iParamsPerEntry;

		boundsMinInPixels = paramArr[baseInd+0];
		boundsMaxInPixels = paramArr[baseInd+1];
		

		if (
			
			all( lessThanEqual(worldPosInPixels.xyz,boundsMaxInPixels.xyz) ) &&
			all( greaterThanEqual(worldPosInPixels.xyz,boundsMinInPixels.xyz) )
		) {


			cornerDisInPixels = paramArr[baseInd+2];
			powerVals = paramArr[baseInd+3];
			coefficients = paramArr[baseInd+4];
			squareVals = paramArr[baseInd+5];
			minMaxMat = paramArr[baseInd+6];

			radInPixels = (boundsMaxInPixels - boundsMinInPixels)/2.0;
			orig = (boundsMaxInPixels + boundsMinInPixels)/2.0;
			absWorldPosInPixels = ((worldPosInPixels)-(radInPixels));



			doCont = true;
			// doCont = false;

			// maxCut = radInPixels.xy;//abs(boundsMaxInPixels.xy-boundsMaxInPixels.xy);
			// cutRad.x = min(maxCut.x, maxCut.y);
			// cutRad.y = cutRad.x;
			// cutCenter.xy = boundsMinInPixels.xy + cutRad.x;
			// cutCenter.zw = boundsMaxInPixels.xy - cutRad.x;
			// cutVec.xy = (worldPosInPixels.xy-cutCenter.xy);
			// cutVec.zw = (worldPosInPixels.xy-cutCenter.zw);
			// cutVecAbs = abs(cutVec);

			// if (all( lessThanEqual( cutVecAbs.xy, cutRad ) ) ) {
			// 	cutBools.x = cutVec.x + cutVec.y >= 0;
			// 	cutBools.y = cutVec.x + cutVec.y < 0;
			// 	cutBools.z = cutVec.x - cutVec.y >= 0;
			// 	cutBools.w = cutVec.x - cutVec.y < 0;

			// 	if (maxCut.x > maxCut.y) {
			// 		doCont = all(cutBools.xz);
			// 	}
			// 	else {
			// 		doCont = all(cutBools.xw);
			// 	}
			// }
			// else {

			// 	if (all( lessThanEqual( cutVecAbs.zw, cutRad ) ) ) {

			// 		cutBools.x = cutVec.z + cutVec.w >= 0;
			// 		cutBools.y = cutVec.z + cutVec.w < 0;
			// 		cutBools.z = cutVec.z - cutVec.w >= 0;
			// 		cutBools.w = cutVec.z - cutVec.w < 0;

			// 		if (maxCut.x > maxCut.y) {
			// 			doCont = all(cutBools.yw);
			// 		}
			// 		else {
			// 			doCont = all(cutBools.yz);
			// 		}
			// 	}
			// 	else {
			// 		doCont = true;
			// 	}
			// }


			
			



			if (doCont) {
				

				newCornerDis = cornerDisInPixels;// *radInPixels;

				originInPixels = clamp(worldPosInPixels, boundsMinInPixels + newCornerDis, boundsMaxInPixels - newCornerDis );
				absOriginInPixels = ((originInPixels) - (radInPixels));

				dis = (absWorldPosInPixels - absOriginInPixels);
				disNorm = dis.xyz/newCornerDis.xyz;
				absDisNorm = abs( disNorm );

				bLessThanOrig = lessThanEqual( abs(worldPosInPixels-orig), abs(originInPixels-orig) );
				lessThanOrig = vec3(bLessThanOrig);
				iLessThanOrig = 1.0-lessThanOrig;
				//dis *= lessThanOrig;

				

				// stair-stepping, ribbing, airval


				curDisXY.x = max(absDisNorm.x,absDisNorm.y);
				curDisYZ.x = max(absDisNorm.y,absDisNorm.z);
				curDisXZ.x = max(absDisNorm.x,absDisNorm.z);

				if (squareVals.x*squareVals.y*squareVals.z == 1.0) {
					absDisNorm.xyz = vec3( max(max(absDisNorm.x,absDisNorm.y),absDisNorm.z) );
				}
				else {
					absDisNorm.xy = mix(absDisNorm.xy, curDisXY.xx, squareVals.x*squareVals.y );
					absDisNorm.yz = mix(absDisNorm.yz, curDisYZ.xx, squareVals.y*squareVals.z );
					absDisNorm.xz = mix(absDisNorm.xz, curDisXZ.xx, squareVals.z*squareVals.x );
				}


				// stair step
				// absDisNorm.z = floor(absDisNorm.z*8.0)/8.0;

				
				eqRes.x = coefficients.x*pow(absDisNorm.x,powerVals.x);
				eqRes.y = coefficients.y*pow(absDisNorm.y,powerVals.y);
				eqRes.z = coefficients.z*pow(absDisNorm.z,powerVals.z);

				curRad = length(disNorm);
				curPhi = atan(disNorm.y, disNorm.x);
				curThe = acos(disNorm.z/curRad);




				// vecN = absDisNorm;

				// if (vecN.x > vecN.y) {
				// 	if (vecN.x > vecN.z) {
				// 		vecN.yz = zeroVec.xy;
				// 	}
				// 	else {
				// 		vecN.xy = zeroVec.xy;
				// 	}
				// }
				// else {
				// 	if (vecN.y > vecN.z) {
				// 		vecN.xz = zeroVec.xy;
				// 	}
				// 	else {
				// 		vecN.xy = zeroVec.xy;
				// 	}
				// }
				// vecN = normalize(vecN);
				// newUVW = (vecN.x * worldPosInPixels.zyx) + (vecN.y * worldPosInPixels.xzy) + (vecN.z * worldPosInPixels.xyz);
				// newUVW.z = curRad*32.0;

				//isRidge.x = float(intMod(int(newUVW.x),256) > 64);
				//isRidge.y = float(intMod(int(newUVW.y),256) > 64);
				
				newUVW = vec3(curPhi/PIO2,curThe/PIO2,curRad);


				// magic value

				// if (newUVW.y < 1.0/3.0) {
				// 	newUVW.x /= 4.0;
				// }


				newUVW.x *= (newCornerDis.x+newCornerDis.y);
				newUVW.y *= (newCornerDis.z) + (newCornerDis.x+newCornerDis.y)/2.0;
				newUVW.z *= length(newCornerDis);


				//tempMat = 2.0/255.0;

				vecN = worldPosInPixels - boundsMinInPixels;

				if ( all(bLessThanOrig.xy) ) {
					newUVW.xy = vecN.xy;
				}
				else {
					if ( all(bLessThanOrig.yz) ) {
						newUVW.xy = vecN.yz;
					}
					else {
						if ( all(bLessThanOrig.xz) ) {
							newUVW.xy = vecN.xz;
						}
						else {

							if (bLessThanOrig.x) {
								newUVW.x = vecN.x;
							}
							if (bLessThanOrig.y) {
								newUVW.x = vecN.y;
							}
							if (bLessThanOrig.z) {
								newUVW.y = vecN.z;
							}

						}
					}
				}


				// finalRes.rgb = intModFV(newUVW, ivec3(256) );
				// if (finalRes.r < 0.0) {
				// 	finalRes.r = 256.0-finalRes.r;
				// }
				// if (finalRes.g < 0.0) {
				// 	finalRes.g = 256.0-finalRes.g;
				// }
				// finalRes.rg /= 255.0;


				if ( brick(newUVW) ) {
					curMat = 2.0/255.0;
				}
				else {

					if (curRad < 0.9 ) {
						curMat = 1.0/255.0;
					}
					else {
						curMat = 0.0/255.0;
					}

					
				}



				// if (sin(curPhi*curThe*32.0) > 0.0) {
				// 	curMat = 3.0/255.0;
				// }


				// planeDis = vec3(length(absDisNorm.yz),length(absDisNorm.xz),length(absDisNorm.xy));
				// absDisNorm.xy = 1.0-absDisNorm.xy;
				// absDisNorm *= absDisNorm;
				// tempLen = planeDis.x*absDisNorm.x + planeDis.y*absDisNorm.y + planeDis.z*absDisNorm.z;
				
				tempLen = (eqRes.x + eqRes.y + eqRes.z);




				if (airVal == 1.0) {

				}
				else {
					if ( ( tempLen >= minMaxMat.x ) && (tempLen <= minMaxMat.y) ) {// - isRidge.x/8.0 - isRidge.y/8.0 ) ) {
						finalRes.a = curMat;

						

					}
				}
				

				if ( tempLen < minMaxMat.x ) { // || abs(sin(worldPosInPixels.x/100.0)) > 0.5
					airVal = 1.0;
					finalRes.a = 0.0/255.0;
				}
				
				

			}
			else {
				
			}
			

		}

	}


	if (finalRes.a != 0.0) {
		//finalRes.r = 1.0;
	}

	// ivec2 wpp = ivec2(worldPosInPixels.xy)/64;

	// wpp *= 64;

	
	vec2 mapCoord = (worldPosInPixels.xy / worldSizeInPixels.x);

	vec4 texHM0 =  texture2D(Texture2, mapFreqs.x * mapCoord );
	vec4 texHM1 =  texture2D(Texture2, mapFreqs.y * mapCoord );
	vec4 texHM2 =  texture2D(Texture2, mapFreqs.z * mapCoord );
	vec4 texHM3 =  texture2D(Texture2, mapFreqs.w * mapCoord );


	float testHeight0 = (texHM0.r*mapAmps.x + texHM1.r*mapAmps.y + texHM2.r*mapAmps.z + texHM3*mapAmps.w);
	float testHeight1 = (seaLevel+20.0/255.0);
	float testHeight = mix(testHeight0,testHeight1, float(texHM0.a>0.0) );


	testHeight *= heightmapMax;

	if (testHeight > worldPosInPixels.z ) {
		finalRes = vec4(0.0,0.0,0.0,2.0/255.0);
	}


	vec4 tex2 =  texture3D(Texture1, newCoords);
	if (tex2.a > 0.5) {
		
		finalRes = vec4(0.0,0.0,0.0,1.0/255.0);
	}
	else {
		if (tex2.b > 0.5) {
			finalRes = vec4(0.0,0.0,0.0,0.0/255.0);
		}
	}




	// vec3 curPos;
	// vec3 bestPos;
	// float curVoroDis;
	// float notValid = 999.0;
	// float minDis1 = notValid;
	// float minDis2 = notValid;
	// float gradVal;
	// float voroSize = 64.0;

	// if (finalRes.a != 0.0) {
	// 	for (i = 0; i < 27; i++) {
	// 		curPos = voroOffsets[i] + floor(worldPosInPixels/voroSize);
	// 		curPos += rand(curPos)*0.5;
	// 		curPos *= voroSize;

	// 		curVoroDis = distance(curPos, worldPosInPixels);

	// 		if (curVoroDis < minDis1) {
	// 			bestPos = curPos;

	// 			minDis2 = minDis1;
	// 			minDis1 = curVoroDis;
	// 		}
	// 		else {
	// 			if (curVoroDis < minDis2) {
	// 				minDis2 = curVoroDis;
	// 			}
	// 		}

	// 	}


	// 	gradVal = clamp(1.0 - (minDis1*2.0/(minDis1+minDis2)),1.0/255.0,1.0);

	// 	if (gradVal > 0.05) {
	// 		finalRes.a = 1.0/255.0;
	// 	}
	// 	else {
	// 		finalRes.a = 2.0/255.0;
	// 	}

	// }

	


	gl_FragData[0] = finalRes;


	//vec4 tex1 =  texture3D(Texture1, newCoords);
	//gl_FragData[0] = float(tex1.a > threshVal/255.0)*(vec4(1.0,0.0,0.0,2.0/255.0));

}