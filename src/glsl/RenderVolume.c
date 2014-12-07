#version 120

uniform sampler2D Texture0; // volume layer 0
uniform sampler2D Texture1; // volume layer 1 (unused now)
//uniform sampler2D Texture2; // front face
//uniform sampler2D Texture3; // back face

uniform vec3 worldMinVisInPixels;
uniform vec3 worldMaxVisInPixels;
uniform float blendAmount;
uniform float bufferMult;
uniform float visPageSizeInPixels;
uniform float volumePitch;
uniform float volGenSuperMod;
uniform float holderSizeInPagesLog;

int iVGSM = int(volGenSuperMod)/2;

varying vec2 TexCoord0;

vec3 dirVecs[6] = vec3[](
	vec3( 1.0,  0.0, 0.0 ), // right
	vec3( -1.0, 0.0, 0.0 ), // left
	vec3( 0.0, 1.0, 0.0 ), // up
	vec3( 0.0, -1.0, 0.0 ), // down
	vec3( 0.0, 0.0, 1.0 ), // above
	vec3( 0.0, 0.0, -1.0 ) // below
);

$

void main() {
	TexCoord0 = gl_MultiTexCoord0.xy;
	gl_Position = gl_Vertex;
}

$

vec4 sampleAtPoint(vec3 point) {
	vec2 texc;
	vec3 newPoint = point / bufferMult + (1.0 - 1.0 / bufferMult) / 2.0;

	newPoint.y = clamp(newPoint.y, 0.5 / volumePitch, (volumePitch - 0.5) / volumePitch);

	texc.x = newPoint.x;
	texc.y = newPoint.y / volumePitch + floor(newPoint.z * volumePitch) / volumePitch;

	return texture2D(Texture0, texc.xy);
}


vec4 getAO(vec3 tp, vec4 curSamp) { //, vec3 wp
	int i;
	int j;
	float fi = 0.0;
	float fj = 0.0;
	int a, b, c;



	vec3 offVal;
	vec3 offValAbs;

	float len;


	float aoVal = 0.0;
	float totalAO = 0.0;

	float notZero;
	float fLerp = 0.0;


	vec4 res;

	int maxRad;
	float tsize = visPageSizeInPixels;
	float pixMod = 1.0 / (tsize);
	vec3 testPoint = tp + pixMod;

	float fMaxRad;
	float curPower;

	float rval;
	float rval2;

	vec3 norm = vec3(0.0, 0.0, 0.0);
	vec3 norm2 = vec3(0.0, 0.0, 0.0);

	int rad;
	int radStep;
	float theta = 0.0;
	float phi = 0.0;
	float frad = float(radStep);

	float totSteps = 0.0;
	float totHits = 0.0;

	float allHits = 0.0;

	float ovLen = 0.0;
	float rvMix = 0.0;

	float twoPI = 6.2831853;
	float PI = 3.14159;

	float isCurLayer = 0.0;

	float isAir = 0.0;

	// rad = 1;
	// radStep = 1;
	rad = 16;
	radStep = 4;

	// for (i = 0; i < 2; i++) {

		for (c = -rad; c <= rad; c += radStep) {
			for (b = -rad; b <= rad; b += radStep) {
				for (a = -rad; a <= rad; a += radStep) {

					offVal.x = float(a);
					offVal.y = float(b);
					offVal.z = float(c);
					
					res = sampleAtPoint(offVal / tsize + testPoint);


					isCurLayer = float(res.r == curSamp.r);
					isAir = 1.0-isCurLayer;//float(isCurLayer == 0.0); // //res.a*


					allHits += isAir;

					if (curSamp.g == 0.0) {
						rval = isAir;
					}
					else {
						rval = float( (res.g != curSamp.g) || (isAir == 1.0) );
					}

					rvMix = mix(rval, isAir, blendAmount);
					norm += rvMix * (offVal);

					totHits += rval;

				}
			}
		}

	// 	if (totHits == 0.0) {
	// 		break;
	// 	}
	// 	else {
	// 		rad = 12;
	// 		radStep = 3;
	// 	}


	// }

	// if (totHits == 0.0) {
	// 	norm = vec3(0.0, 0.0, 0.0); //norm2/totSteps;

	// 	if (tp.x >= tp.y) {

	// 		if (tp.x >= tp.z) {
	// 			norm.x = 1.0;
	// 		}
	// 		else {
	// 			norm.z = 1.0;
	// 		}
	// 	}
	// 	else {
	// 		if (tp.y >= tp.z) {
	// 			norm.y = 1.0;
	// 		}
	// 		else {
	// 			norm.z = 1.0;
	// 		}
	// 	}

	// 	aoVal = 0.0;
	// }
	// else {
	// 	aoVal = float(allHits > 0.0);
	// }



	norm = normalize(norm);
	norm = (norm + 1.0) / 2.0;





	return vec4(norm, aoVal);
}


int intMod(int lhs, int rhs) {
	return lhs - ( (lhs / rhs) * rhs );
}

vec2 pack16(float num) {

	int iz = int(num);
	int ir = intMod(iz, 256);
	int ig = (iz) / 256;

	vec2 res;

	res.r = float(ir) / 255.0;
	res.g = float(ig) / 255.0;

	return res;

}

float unpack16(vec2 num) {
	return num.r * 255.0 + num.g * 65280.0;
}

vec3 get3DCoords(vec2 tc, float vPitch) {
	int ivPitch = int(vPitch);
	vec3 newCoords = vec3(0.0);
	int j = int( vPitch * vPitch * tc.y );
	newCoords.x = tc.x;
	newCoords.z = float(j / ivPitch) / vPitch;
	newCoords.y = (tc.y - newCoords.z) * vPitch;
	
	return newCoords;
}

void main() {

	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;

	float fLerp = 0.0;
	
	
	vec3 oneVec = vec3(1.0);

	vec4 blackCol = vec4(0.0);
	vec4 samp = vec4(0.0);
	vec4 samp2 = vec4(0.0);

	
	
	
	float newPitch = volumePitch/volGenSuperMod;
	
	vec3 basePos = get3DCoords(TexCoord0.xy,newPitch);
	
	basePos.xy += vec2(0.5/volumePitch);
	basePos.z += 1.0/volumePitch;
	
	//basePos = basePos*((newPitch+2.0/newPitch)/newPitch) - (1.0/newPitch);
	
	vec3 curPos = vec3(0.0);
	vec3 offsetPos = vec3(0.0);
	vec3 bestPos = vec3(0.0);
	
	float fi = 0.0;
	float fj = 0.0;
	float fk = 0.0;
	
	
	bool foundTrans = false;
	bool foundSolid = false;
	bool foundAir = false;
	
	float solidVal = 0.0/255.0;
	float transVal = 1.0/255.0;
	float airVal = 2.0/255.0;
	
	vec3 bestSolid = vec3(0.0);
	vec3 bestTrans = vec3(0.0);
	
	bool foundDif = false;
	

	// fk = float(-iVGSM);
	// fj = fk;
	// fi = fk;
	// offsetPos = vec3(fi,fj,fk);
	// curPos = basePos + offsetPos/volumePitch;
	// vec4 sampOrig = sampleAtPoint( curPos );
	
	// bool foundBest = false;
	
	// if (sampOrig.r != 2.0/255.0) {
	// 	bestPos = curPos;
	// 	foundBest = true;
	// }
	
	// int curLayer = 0;
	
	// for (m = 0; m < 3; m++) {
		
	// }

	for (k = -iVGSM; k <= iVGSM; k++) {
		fk = float(k);
		for (j = -iVGSM; j <= iVGSM; j++) {
			fj = float(j);
			for (i = -iVGSM; i <= iVGSM; i++) {
				fi = float(i);
				offsetPos = vec3(fi,fj,fk);
				curPos = basePos + offsetPos/volumePitch;
				samp = sampleAtPoint( curPos );
				
				
				if (samp.r == solidVal) {
					bestSolid = curPos;
					foundSolid = true;
				}
				if (samp.r == airVal) {
					foundAir = true;
				}
				if (samp.r == transVal) {
					bestTrans = curPos;
					foundTrans = true;
				}
				
				// if (samp.r != sampOrig.r) {
				// 	foundDif = true;
				// 	break;
				// }
				
				// if (samp.a > 0.0) {
				// 	foundSolid = true;
				// 	bestPos = curPos;
				// }
				// else {
				// 	foundAir = true;
				// }
				
				// if (foundSolid&&foundAir) {
				// 	break;
				// }
				
			}
		}
	}
	
	
	vec4 heightMat = blackCol;
	heightMat.r = 2.0/255.0;
	vec4 normAO = blackCol;
	
	foundDif = 
		(foundSolid&&(foundAir||foundTrans)) ||
		(foundTrans&&(foundAir||foundSolid));
	
	
	if (foundSolid) {
		bestPos = bestSolid;
	}
	else {
		if (foundTrans) {
			bestPos = bestTrans;
		}	
	}
	
	if (foundDif) {
		samp = sampleAtPoint( bestPos );
		heightMat = vec4(samp.r,float(foundDif), samp.ba);
		normAO = getAO(bestPos, samp);
	}
	
	

	gl_FragData[0] = heightMat;
	gl_FragData[1] = normAO;
	


}
