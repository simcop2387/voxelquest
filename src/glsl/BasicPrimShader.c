#version 330
//#extension GL_EXT_frag_depth : enable

uniform samplerBuffer Texture0; // primTBO


//uniform int actorCount;
//uniform int MAX_PRIM_IDS;
//uniform int MAX_PRIMTEST;
//uniform float heightOfNearPlane;
//uniform float FOV;
uniform vec3 lightPos;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;

uniform mat4 pmMatrix;

// uniform vec4 paramFetch1;
// uniform vec4 paramFetch2;

//uniform vec4 paramArrGeom[24];

uniform vec4 primArr[128];
// uniform float primArrLength;


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
flat out vec4 pdVisMin;
flat out vec4 pdVisMax;
flat out vec4 pdBoundsMin;
flat out vec4 pdBoundsMax;
flat out vec4 pdCornerDis;
flat out vec4 pdMatParmas;
flat out vec4 boxDim;
flat out vec2 boxPower;
flat out vec3 boxCenterPoint;





void getPrimVals(int _ptInd) { //
		
		// texelRes1 = paramFetch1;
		// texelRes2 = paramFetch2;
		
		int ptInd = _ptInd;
		int primReadOffset = ptInd*E_PRIMTEMP_LENGTH;
		
		
		
//		if (ptInd == -1) {
				
				
				// pdVisMin = paramArrGeom[E_PRIMTEMP_VISMIN];
				// pdVisMax = paramArrGeom[E_PRIMTEMP_VISMAX];
				// pdBoundsMin = paramArrGeom[E_PRIMTEMP_BOUNDSMIN];
				// pdBoundsMax = paramArrGeom[E_PRIMTEMP_BOUNDSMAX];
				// pdCornerDis = paramArrGeom[E_PRIMTEMP_CORNERDIS];
				// pdMatParmas = paramArrGeom[E_PRIMTEMP_MATPARAMS];
				
				
		// }
		// else {
				pdVisMin = texelFetch(Texture0, primReadOffset + E_PRIMTEMP_VISMIN);
				pdVisMax = texelFetch(Texture0, primReadOffset + E_PRIMTEMP_VISMAX);
				pdBoundsMin = texelFetch(Texture0, primReadOffset + E_PRIMTEMP_BOUNDSMIN);
				pdBoundsMax = texelFetch(Texture0, primReadOffset + E_PRIMTEMP_BOUNDSMAX);
				pdCornerDis = texelFetch(Texture0, primReadOffset + E_PRIMTEMP_CORNERDIS);
				pdMatParmas = texelFetch(Texture0, primReadOffset + E_PRIMTEMP_MATPARAMS);
		// }
		
		boxPower = pdCornerDis.zw;
		boxDim = vec4(
				(
						(pdBoundsMax.xyz-pdCornerDis.x) -
						(pdBoundsMin.xyz+pdCornerDis.x)
				)*0.5,
				pdCornerDis.x
		);
		boxCenterPoint = vec3(
				(texelRes1.xyz + pdBoundsMin.xyz) +
				(texelRes1.xyz + pdBoundsMax.xyz)
		)*0.5;
}


void main() {
	
	int cubeIndex = int(floor(data0.x+0.1));
	
	
	texelRes1 = primArr[cubeIndex*2+0];
	texelRes2 = primArr[cubeIndex*2+1];
	
	int templateId = int(texelRes1.w);
	
	getPrimVals(templateId);
	//pdVisMin.xyz = vec3(-4.0);
	//pdVisMax.xyz = vec3(4.0);
	
	
	
	worldPos = vec4(
		mix(
			(texelRes1.xyz + pdVisMin.xyz),
			(texelRes1.xyz + pdVisMax.xyz),
			vposition.xyz	
		),
		1.0
	);
	vdata0 = data0;
	//vdata0.w = floor(vdata0.w+1.0); // *65281.0 + 0.1)/65280.0;
	vec4 screenPos = pmMatrix*worldPos;
	screenPos.z /= 2.0;
	worldPos.w = vposition.w;
	gl_Position = screenPos;
	
	
	
	
}




$

in vec4 worldPos;
flat in vec4 vdata0;

flat in vec4 texelRes1;
flat in vec4 texelRes2;
flat in vec4 pdVisMin;
flat in vec4 pdVisMax;
flat in vec4 pdBoundsMin;
flat in vec4 pdBoundsMax;
flat in vec4 pdCornerDis;
flat in vec4 pdMatParmas;
flat in vec4 boxDim;
flat in vec2 boxPower;
flat in vec3 boxCenterPoint;

layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;
layout(location = 2) out vec4 FragColor2;

// qqqqq 

bool globPrimaryRay;
float MAX_CAM_DIS;


const int TOT_DETAIL_STEPS = 8;
const float M_PI = 3.14159265359;
const int NUM_RAY_STEPS = 128;
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

vec3 udRoundBox( vec3 vectorFromCenter, vec4 box_dim, vec2 box_power, float wallThickness )
{
		vec3 absVecFromCenter = abs(vectorFromCenter);
		
		vec3 newP = abs(max( absVecFromCenter-(box_dim.xyz), vec3(0.0) ));
		
		newP.xy = pow(newP.xy, box_power.xx );
		newP.x = pow( newP.x + newP.y, 1.0/box_power.x );
		
		newP.xz = pow(newP.xz, box_power.yy );
		newP.x = pow( newP.x + newP.z, 1.0/box_power.y );
		
		return vec3(
				(newP.x-box_dim.w),
				( (box_dim.w-wallThickness)-newP.x ),
				(newP.x-(box_dim.w-wallThickness))
		);
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



float mapPrim(vec3 pos) {
	
	
	vec3 visCenterPoint = vec3(
			(texelRes1.xyz + pdVisMin.xyz) +
			(texelRes1.xyz + pdVisMax.xyz)
	)*0.5;
	
	vec3 visDim = (pdVisMax.xyz - pdVisMin.xyz)*0.5;
	
	float visBoxDis = sdBox(pos-visCenterPoint, visDim);
	
	vec3 boxRes1;
	
	boxRes1.xyz = udRoundBox(
		pos-boxCenterPoint,
		boxDim,
		boxPower,
		pdCornerDis.y
	);
	
	float finalRes = opI(
		max(boxRes1.x,boxRes1.y) * 0.5,
		visBoxDis
	);
	
	finalRes = opS(
		finalRes,
		sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) ) //8.0 //
	);
	
	return finalRes;
	
}

vec3 normPrim( vec3 pos )
{
		vec3 eps = vec3( 0.0, 0.0, 0.0 );
		eps.x = 0.05;
		
		vec3 nor = vec3(
				mapPrim(pos+eps.xyy) - mapPrim(pos-eps.xyy),
				mapPrim(pos+eps.yxy) - mapPrim(pos-eps.yxy),
				mapPrim(pos+eps.yyx) - mapPrim(pos-eps.yyx) );
		return normalize(nor);
}

vec4 castPrim(
		vec3 ro,
		vec3 rd,
		float minT,
		float maxT,
		int numSteps,
		inout float totSteps
) {
		int p = 0;
		float fNumSteps = float(numSteps);
		
		float res = 0.0;
		float t;
		float fp;
		
		vec3 pos;
		
		float MIN_PREC = 0.01;
		
		t = minT;
		for( p = 0; p < numSteps; p++ ) {
				
				fp = float(p)/fNumSteps;
				
				pos = ro+rd*t;
				
				res = mapPrim( pos );
				if (
						(abs(res) < MIN_PREC ) ||
						(t>maxT)
				) {
						break;
				}
				
				t += res*0.5;
		}
		
		totSteps = float(p);
		
		if (res < MIN_PREC ) {
			return vec4(normPrim(pos),t);
		}
		else {
			return vec4(-1.0f);
		}
		
}



void main() {
	
	globPrimaryRay = true;
	MAX_CAM_DIS = 999999.0;//clipDist.y;

	vec4 primRes = vec4(0.0,0.0,1.0,0.0);

	vec3 ro = cameraPos.xyz;
	vec3 rd = normalize(worldPos.xyz-cameraPos.xyz);

	vec3 minVisBox = texelRes1.xyz+pdVisMin.xyz;
	vec3 maxVisBox = texelRes1.xyz+pdVisMax.xyz;
	
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

	// primRes.w = hitBox.x;

	float totSteps = 0.0;


	vec2 curTex = vec2(TEX_EARTH,0.0);
	
	float totStepVal = 0.0;
	
	
	primRes = castPrim(ro,rd,hitBox.x,hitBox.y,NUM_RAY_STEPS, totSteps);
	totStepVal = totSteps/float(NUM_RAY_STEPS);
	curTex = vec2(TEX_EARTH,totStepVal);
	
	
	if (primRes.w < 0.0) {
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
	FragColor2 = vec4(texelRes2.x,0.0,0.0,0.0);

}









