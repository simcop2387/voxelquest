#version 330
//#extension GL_EXT_frag_depth : enable

uniform samplerBuffer Texture0; // limbTBO


//uniform int actorCount;
//uniform int MAX_PRIM_IDS;
//uniform float heightOfNearPlane;
//uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform vec3 lightPos;

uniform mat4 pmMatrix;

uniform vec4 limbArr[128];

const int MAX_PRIM_IDS = 24;
int primIdList[MAX_PRIM_IDS]; // this is related to MAX_PRIM_IDS



^INCLUDE:MATERIALS^

const int E_PRIMTEMP_VISMIN = 0;
const int E_PRIMTEMP_VISMAX = 1;
const int E_PRIMTEMP_BOUNDSMIN = 2;
const int E_PRIMTEMP_BOUNDSMAX = 3;
const int E_PRIMTEMP_CORNERDIS = 4; //E_PRIMTEMP_CORNERDISTHICKNESSPOWER
const int E_PRIMTEMP_MATPARAMS = 5;
const int E_PRIMTEMP_LENGTH = 6;

$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 data0;

out vec4 worldPos;
flat out vec4 vdata0;

flat out vec4 texelRes1;
flat out vec4 texelRes2;
//flat out vec4 pdVisMin;
//flat out vec4 pdVisMax;
flat out vec4 boxDim;
flat out vec2 boxPower;
flat out vec3 boxCenterPoint;






void main() {
	
	int cubeIndex = int(floor(data0.x+0.1));
	
	texelRes1 = limbArr[cubeIndex*2+0];
	texelRes2 = limbArr[cubeIndex*2+1];
	
	// int primDataInd = int(texelRes1.w);
	
	
	// vec4 header0 = texelFetch(Texture0, primDataInd); primDataInd++;
	// vec4 header1 = texelFetch(Texture0, primDataInd); primDataInd++;
	// vec4 header2 = texelFetch(Texture0, primDataInd); primDataInd++;
	
	// pdVisMin = header1;
	// pdVisMax = header2;
	
	// pdVisMin.xyz -= texelRes1.xyz;
	// pdVisMax.xyz -= texelRes1.xyz;
	
	// pdVisMin.xyz = vec3(-4.0);
	// pdVisMax.xyz = vec3(4.0);
	
	
	
	worldPos = vec4(
		mix(
			// (texelRes1.xyz + pdVisMin.xyz),
			// (texelRes1.xyz + pdVisMax.xyz),
			texelRes1.xyz,
			texelRes2.xyz,
			vposition.xyz	
		),
		1.0
	);
	vdata0 = data0;
	vec4 screenPos = pmMatrix*worldPos;
	screenPos.z /= 4.0;
	worldPos.w = vposition.w;
	gl_Position = screenPos;
	
	
	
	
}




$

in vec4 worldPos;
flat in vec4 vdata0;

flat in vec4 texelRes1;
flat in vec4 texelRes2;
//flat in vec4 pdVisMin;
//flat in vec4 pdVisMax;
flat in vec4 boxDim;
flat in vec2 boxPower;
flat in vec3 boxCenterPoint;

layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;
layout(location = 2) out vec4 FragColor2;

// qqqqq 


float globBestLimbDepth;
vec2 globTexDyn;
float globStepCount;
int globBestLimbInd;
bool globPrimaryRay;
float globBoneRad;
int primIdListLength;
float MAX_CAM_DIS;


const int TOT_DETAIL_STEPS = 8;
const float M_PI = 3.14159265359;
const int NUM_RAY_STEPS = 32;
const float SKY_ID = -5.0;
const float CAM_BOX_SIZE = 1.0;


float sdBox( vec3 p, vec3 b )
{
	vec3 d = abs(p) - b;
	return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

vec2 opU( vec2 d1, vec2 d2 )
{
		return mix(d2,d1,float(d1.x<d2.x));
}

float opS( float d1, float d2 )
{
		return max(-d2,d1);
}

float opI( float d1, float d2 )
{
		return max(d1,d2);
}


vec2 aabbIntersect(vec3 rayOrig, vec3 rayDir, vec3 minv, vec3 maxv) {
		float t0;
		float t1;
		
		vec3 invR = 1.0 / rayDir;
		vec3 tbot = invR * (minv-rayOrig);
		vec3 ttop = invR * (maxv-rayOrig);
		vec3 tmin = min(ttop, tbot);
		vec3 tmax = max(ttop, tbot);
		vec2 t = max(tmin.xx, tmin.yz);
		t0 = max(t.x, t.y);
		t = min(tmax.xx, tmax.yz);
		t1 = min(t.x, t.y);
		return vec2(t0,t1); // if (t0 <= t1) { did hit } else { did not hit }
}







vec4 pointSegDistance(vec3 testPoint, vec3 sp0, vec3 sp1)
{
		vec3 v = sp1 - sp0;
		vec3 w = testPoint - sp0;

		float d0 = distance(testPoint, sp0);
		float d1 = distance(testPoint, sp1);
		float d2 = distance(sp0, sp1);

		float c1 = dot(w, v);
		if ( c1 <= 0 ) {
				return vec4(sp0, d0);
		}

		float c2 = dot(v, v);
		if ( c2 <= c1 ) {
				return vec4(sp1,d1);
		}

		float b = c1 / c2;
		vec3 testPoint2 = sp0 + b * v; // testPoint2 is the nearest point on the line
		return vec4(
				testPoint2,
				distance(testPoint, testPoint2)
		);
}

// source http://geomalgorithms.com/a07-_distance.html#dist3D_Segment_to_Segment()
// dist3D_Segment_to_Segment(): get the 3D minimum distance between 2 segments
//    Input:  two 3D line segments S1 and S2
//    Return: the shortest distance between S1 and S2
float segSegDis(
	vec3 S1_P0,
	vec3 S1_P1,
	vec3 S2_P0,
	vec3 S2_P1
) {
	
		float SMALL_NUM = 0.000001;
	
    vec3   u = S1_P1 - S1_P0;
    vec3   v = S2_P1 - S2_P0;
    vec3   w = S1_P0 - S2_P0;
    float    a = dot(u,u);         // always >= 0
    float    b = dot(u,v);
    float    c = dot(v,v);         // always >= 0
    float    d = dot(u,w);
    float    e = dot(v,w);
    float    D = a*c - b*b;        // always >= 0
    float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
    float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
    if (D < SMALL_NUM) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else {                 // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    }
    else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d +  b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
    tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);

    // get the difference of the two closest points
    vec3 dP = w + (sc * u) - (tc * v);  //

    return length(dP);   // return the closest distance
}

void preLimb(vec3 ro, vec3 rd) {
	
	int i;
	int j;
	
	primIdListLength = 0;
	
	for (i = 0; i < MAX_PRIM_IDS; i++) {
			primIdList[i] = -1;
	}
	
	int primDataInd = 0;
	int endInd = 0;
	int firstInd = 0;
	
	vec4 header0;
	vec4 header1;
	vec4 header2;
	
	vec4 datVec;
	vec4 cenVec;
	vec4 tanVec;
	vec4 bitVec;
	vec4 norVec;
	vec4 ln0Vec;
	vec4 ln1Vec;
	
	vec3 seg0a = ro;
	vec3 seg0b = ro + rd*MAX_CAM_DIS;
	vec3 seg1a = vec3(0.0);
	vec3 seg1b = vec3(0.0);
	
	vec2 hitBox;
	
	float curDis;
	float maxDis;
	
	
	primDataInd = int(texelRes1.w);
	
	//for (i = 0; i < actorCount; i++) {
		header0 = texelFetch(Texture0, primDataInd); primDataInd++;
		header1 = texelFetch(Texture0, primDataInd); primDataInd++;
		header2 = texelFetch(Texture0, primDataInd); primDataInd++;
		
		
		endInd = int(header0.x);
		
		// if (endInd <= 0) {
		// 	break;
		// }
		
		
		//hitBox = aabbIntersect(ro,rd,header1.xyz,header2.xyz);
		
//		if (hitBox.x <= hitBox.y) {
			
			
			while (primDataInd < endInd) {
				
				firstInd = primDataInd;
				
				datVec = texelFetch(Texture0, primDataInd); primDataInd++;
				cenVec = texelFetch(Texture0, primDataInd); primDataInd++;
				tanVec = texelFetch(Texture0, primDataInd); primDataInd++;
				bitVec = texelFetch(Texture0, primDataInd); primDataInd++;
				norVec = texelFetch(Texture0, primDataInd); primDataInd++;
				ln0Vec = texelFetch(Texture0, primDataInd); primDataInd++;
				ln1Vec = texelFetch(Texture0, primDataInd); primDataInd++;
				
				seg1a = cenVec.xyz - tanVec.xyz*ln0Vec.x;
				seg1b = cenVec.xyz + tanVec.xyz*ln0Vec.x;
				
				maxDis = max(
					max(ln0Vec.z,ln0Vec.y),
					max(ln1Vec.z,ln1Vec.y)
				);
				
				//curDis = pointSegDistance(cenVec.xyz, seg0a, seg0b).w;
				curDis = segSegDis(seg0a,seg0b,seg1a,seg1b);
				
				
				
				if (curDis < maxDis) {
					
					primIdList[primIdListLength] = firstInd;
					primIdListLength++;
					
					if (primIdListLength == MAX_PRIM_IDS) {
						
							return;
					}
					
				}
				
			}
			
		// }
		// else {
			
		// 	primDataInd = endInd;
		// }
		
		// if (endInd <= 0) {
		// 	return;
		// }
		
		
		
	//}
	
	
}

float postLimb(vec3 pos) {
	
	vec4 datVec;
	vec4 cenVec;
	vec4 tanVec;
	vec4 bitVec;
	vec4 norVec;
	vec4 ln0Vec;
	vec4 ln1Vec;
	
	vec4 lnVec;
	
	vec3 seg1a = vec3(0.0);
	vec3 seg1b = vec3(0.0);
	
	int i;
	int primDataInd = 0;
	int firstInd;
	
	vec4 closestPoint = vec4(0.0);
	
	float lerpValP0toP1;
	float lerpValBit;
	float lerpValTan;
	float curRad;
	
	vec2 xyVal;
	
	vec2 minDis = vec2(MAX_CAM_DIS,-1);
	
	vec3 offVec;
	vec3 xyzAmount;
	
	for (i = 0; i < primIdListLength; i++) {
		primDataInd = primIdList[i];
		firstInd = primDataInd;
		
		datVec = texelFetch(Texture0, primDataInd); primDataInd++;
		cenVec = texelFetch(Texture0, primDataInd); primDataInd++;
		tanVec = texelFetch(Texture0, primDataInd); primDataInd++;
		bitVec = texelFetch(Texture0, primDataInd); primDataInd++;
		norVec = texelFetch(Texture0, primDataInd); primDataInd++;
		ln0Vec = texelFetch(Texture0, primDataInd); primDataInd++;
		ln1Vec = texelFetch(Texture0, primDataInd); primDataInd++;
		
		seg1a = cenVec.xyz - tanVec.xyz*ln0Vec.x;
		seg1b = cenVec.xyz + tanVec.xyz*ln0Vec.x;
		
		closestPoint = pointSegDistance(pos, seg1a, seg1b);
		
		offVec = normalize(pos.xyz - closestPoint.xyz);
		
		if (globPrimaryRay) {
			globBoneRad = min(globBoneRad,closestPoint.w);
		}
		
		xyzAmount = abs(vec3(
			dot(offVec, tanVec.xyz),
			dot(offVec, bitVec.xyz),
			dot(offVec, norVec.xyz)
		));
		
		lerpValP0toP1 = distance(closestPoint.xyz,seg1a)/(ln0Vec.x*2.0);
		lerpValBit = abs(dot(offVec,bitVec.xyz));
		lerpValTan = abs(dot(offVec,tanVec.xyz));
		
		xyVal.x = cos(lerpValBit*M_PI*0.5);
		xyVal.y = sin(lerpValBit*M_PI*0.5);
		
		lerpValP0toP1 = pow(lerpValP0toP1,cenVec.w);
		lerpValBit = pow(lerpValBit,tanVec.w);
		lerpValTan = pow(lerpValTan,bitVec.w);
		
		lerpValTan = mix(0.0,norVec.w,lerpValTan);
		
		
		
		lnVec = mix(ln0Vec,ln1Vec,lerpValP0toP1);
		
		curRad = mix(lnVec.y,lnVec.z,lerpValBit)*(1.0-lerpValTan);
		
		curRad *= clamp(1.0-xyzAmount.z,1.0-ln1Vec.w,1.0);
		
		// if (xyzAmount.z > 0.1) {
		// 	curRad = min(curRad,0.1);
		// }
		
		
		
		
		minDis = opU(
			minDis,
			vec2(
				closestPoint.w-curRad,
				float(firstInd)
			) 
		);
		
	}
	
	
	
	float DYN_PREC = 0.02;
	
	if (
		(minDis.x <= DYN_PREC*1.5)	&&
		(minDis.y >= 0.0)		
	) {
		if (globPrimaryRay) {
			globBestLimbInd = int(minDis.y);
		}
		
	}
	
	return minDis.x*0.5;
}



vec3 mapDyn(vec3 pos) {
		
		int i;
		
		vec2 res = vec2(MAX_CAM_DIS,SKY_ID);
		
		
		res = opU(res, vec2(
		    postLimb(pos),
		    TEX_EARTH
		));
				
		float oldX = res.x;
		
		res.x = opS(
			res.x,
			sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) ) //8.0 //CAM_BOX_SIZE
		);
		
		
		return vec3(res,oldX);
}

vec3 normDyn( vec3 pos )
{
		globPrimaryRay = false;
		vec3 eps = vec3( 0.0, 0.0, 0.0 );
		eps.x = 0.05;
		
		vec3 nor = vec3(
				mapDyn(pos+eps.xyy).x - mapDyn(pos-eps.xyy).x,
				mapDyn(pos+eps.yxy).x - mapDyn(pos-eps.yxy).x,
				mapDyn(pos+eps.yyx).x - mapDyn(pos-eps.yyx).x );
		globPrimaryRay = true;
		return normalize(nor);
}

vec4 castDyn(
		vec3 ro,
		vec3 rd,
		vec2 minMaxT,
		float fNumSteps
) {
		int p = 0;
		int numSteps;
		
		vec3 res = vec3(0.0);
		float t;
		float fp;
		
		vec3 pos;
		
		
		
		float DYN_PREC = 0.02;
		float DYN_PREC2 = 0.002;
		
		
		t = minMaxT.x;
		numSteps = int(fNumSteps);
		for( p = 0; p < numSteps; p++ ) {
				
				fp = float(p)/fNumSteps;
				
				pos = ro+rd*t;
				
				res = mapDyn( pos );
				if (
						(res.x < DYN_PREC )
						|| (t>minMaxT.y)
				) {
						break;
				}
				
				t += res.x;
		}
		
		globStepCount = float(p);
		
		if (res.x < DYN_PREC) {
				
				//t -= DYN_PREC*1.5;
				
				for( p = 0; p < TOT_DETAIL_STEPS; p++ ) {
						
						pos = ro+rd*t;
						
						res = mapDyn( pos );
						
						if (abs(res.x) < DYN_PREC2) {
								break;
						}
						
						t += res.x;
				}
				
				if (res.z < res.x) {
					globBestLimbDepth = res.z;
				}
				
				
				
				globTexDyn.x = res.y;
				globTexDyn.y = 0.0;
		}
		else {
				globTexDyn.xy = vec2(0.0);
				t = -1.0;
		}
		
		
		
		
		
		
		
		
		
		// if (
		//     (res.y != SKY_ID)
		//     && (res.x < DYN_PREC)
		// ) {
		//     globTexDyn.x = res.y;
				
		//     if (globTexDyn.x == TEX_EXPLOSION) {
		//         globTexDyn.y = pow(abs(dot(lookAtVec,normalize(globExplodeVec))),2.0);
		//     }
		//     else {
		//         globTexDyn.y = 0.0;
		//     }
		// }
		
		
		return vec4(normDyn(pos),t);
		
}





void main() {
	
	globStepCount = 0.0;
	globTexDyn = vec2(0.0);
	primIdListLength = 0;
	globBestLimbInd = -1;
	globPrimaryRay = true;
	globBoneRad = 99999.0;
	globBestLimbDepth = 99999.0;
	MAX_CAM_DIS = 999999.0;//clipDist.y;

	vec4 primRes = vec4(0.0,0.0,1.0,0.0);

	vec3 ro = cameraPos.xyz;
	vec3 rd = normalize(worldPos.xyz-cameraPos.xyz);

	vec3 minVisBox = texelRes1.xyz;//+pdVisMin.xyz;
	vec3 maxVisBox = texelRes2.xyz;//+pdVisMax.xyz;
	
	vec2 hitBox = aabbIntersect(
			ro,
			rd,
			minVisBox,
			maxVisBox
	);

	if (
		all(greaterThanEqual(ro,minVisBox)) &&
		all(lessThanEqual(ro,maxVisBox))
	) {
		// is inside box
		hitBox.x = 0.0;
	}


	float totSteps = 0.0;

	//TEX_EARTH
	vec2 curTex = vec2(0.0,0.0);
	
	float totStepVal = 0.0;
	
	primRes.w = distance(cameraPos.xyz,worldPos.xyz);
	
	// if (texelRes1.w == 0.0) {
	// 	discard;
	// }
	
	preLimb(ro,rd);
	primRes = castDyn(ro,rd,hitBox, NUM_RAY_STEPS);
	
	//curTex = globTexDyn;
	
	// if (curTex.x == TEX_NULL) {
	// 	primRes.w = 0.0;
	// }
	
	
	if (primRes.w < 0.0) {
		discard;
	}
	
	
	int primDataInd = 0;
	vec4 datVec;
	vec4 ln0Vec;
	vec4 limbRes = vec4(0.0);
			
	if (globBestLimbInd > -1) {
		
		primDataInd = globBestLimbInd;
		
		datVec = texelFetch(Texture0, primDataInd+0);
		ln0Vec = texelFetch(Texture0, primDataInd+5);
		
		limbRes.w = datVec.x;
		limbRes.z = datVec.y;
		
		curTex = vec2(
			datVec.z/255.0,
			clamp(datVec.w + (globStepCount/float(NUM_RAY_STEPS)*0.25),0.0,1.0)
		);
		
		if (
			(globBestLimbDepth < -0.01) &&
			(ln0Vec.w == 0.0)	
		) {
			
			curTex = vec2(
				TEX_MEAT,
				datVec.w
			);
			
			
			if (globBoneRad < 0.03) {
				curTex = vec2(
					TEX_BONE,
					datVec.w
				);
			}
			
		}
		
		
		
		
	}
	
	
	if (curTex.x == 0.0) {
		discard;
	}
	
	//primRes.w = ;
	
	vec3 newPos = ro+rd*primRes.w;

	

	// float myDepth = primRes.w/clipDist.y;
	// //gl_FragDepthEXT = myDepth;

	// // cameraSpacePosition.z += bias;
	// // vec4 clipPos = projMat * vec4(cameraSpacePosition, 1.0);
	// // float ndcDepth = clipPos.z / clipPos.w;
	// // gl_FragDepth = ((gl_DepthRange.diff * ndcDepth) +
	// //     gl_DepthRange.near + gl_DepthRange.far) / 2.0;
	
	// gl_FragDepth = myDepth;

	// FragColor0 = vec4(newPos,worldPos.w);
	// FragColor1 = vec4(primRes.xyz,curMat);
	
	//newPos.xyz = worldPos.xyz;
	
	//newPos.z += -0.1;
	
	
	float curMat = floor(curTex.x*256.0*255.0) + floor(curTex.y*255.0);
	
	vec4 clipPos = pmMatrix * vec4(newPos.xyz, 1.0);
	float ndcDepth = clipPos.z / clipPos.w;
	gl_FragDepth = ((gl_DepthRange.diff * ndcDepth) +
	    gl_DepthRange.near + gl_DepthRange.far) / 2.0;
	
	// float myDepth = hitBox.x/clipDist.y;
	// gl_FragDepth = myDepth;
	
	FragColor0 = vec4(newPos,distance(newPos.xyz,lightPos.xyz));
	FragColor1 = vec4(primRes.xyz,curMat);
	FragColor2 = vec4(texelRes2.x,0.0,limbRes.zw);


}









