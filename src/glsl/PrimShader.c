
uniform bool placingGeom;
uniform bool testOn;
uniform bool skipPrim;
uniform bool depthInvalidMove;
uniform bool depthInvalidRotate;
uniform bool doSphereMap;
//uniform bool readPoly;

uniform int actorCount;
uniform int MAX_PRIM_IDS;
uniform int MAX_PRIMTEST;

uniform float mapPitch;
uniform float heightMapMaxInCells;
uniform float seaLevel;
uniform vec4 mapFreqs;
uniform vec4 mapAmps;

uniform float invalidCount;
uniform float UNIT_MAX;
uniform float FOV;
uniform float curTime;
uniform float volSizePrim;
uniform float waterLerp;

uniform vec3 entPos;
uniform float thirdPerson;
//uniform float CAM_BOX_SIZE;
uniform float isUnderWater;
uniform float SPHEREMAP_SCALE_FACTOR;

uniform vec2 bufferDim;
uniform vec2 clipDist;
uniform vec3 lightVec;
uniform vec3 cameraPos;
uniform vec3 lookAtVec;

uniform float voroSize;

uniform vec3 waterMin;
uniform vec3 waterMax;

uniform vec3 volMinReadyInPixels;
uniform vec3 volMaxReadyInPixels;


uniform float cellsPerWorld;

uniform vec4 paramFetch1;
uniform vec4 paramFetch2;

uniform mat4 proj;
uniform mat4 modelview;
uniform mat4 modelviewInverse;

uniform int numSplashes;
uniform vec4 splashArr[8];

uniform int numExplodes;
uniform vec4 explodeArr[8];

uniform vec4 paramArrGeom[24];


uniform sampler3D Texture0; // E_PL_TERRAIN
//uniform sampler3D Texture1; // E_PL_PRIMIDS

uniform samplerBuffer Texture1; // prim ids

uniform sampler2D Texture2; // hm fbo

// now: rasterFBO               ///////////// depth targ fbo
uniform sampler2D Texture3;
uniform sampler2D Texture4;

// depth targ fbo sphereMap
// now is geomTargFBO
uniform sampler2D Texture5;
uniform sampler2D Texture6;

// terTargFBO or primTargFBO
uniform sampler2D Texture7;
uniform sampler2D Texture8;
uniform sampler2D Texture9;
uniform sampler2D Texture10;
uniform sampler2D Texture11;
uniform sampler2D Texture12;

//E_VW_VORO
uniform sampler3D Texture13;

//E_VW_WORLD
uniform sampler3D Texture14;

// imageVoro // noisefbolinear
uniform sampler2D Texture15;

// // polyFBO
// uniform sampler2D Texture14;




$


layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vtexcoord;
//out vec4 TexCoord0;
out vec4 worldPos;



void main() {
		
		
		//TexCoord0 = vtexcoord;
		
		worldPos = vtexcoord;
		gl_Position = vposition;
		
		#ifdef DOPOLY
				gl_Position = proj*modelview*vec4(vposition.xyz,1.0);
				gl_Position.z = smoothstep(clipDist.x,clipDist.y*2.0,gl_Position.z);
		#endif
		
		
		
}




$





























































const float CAM_BOX_SIZE = 2.0;

// qqqq
const int TOT_STEPS = 128;
const int TOT_DETAIL_STEPS = 8;
const int TOT_STEPS_POLY = 16;

const float MAX_SHAD_DIS_PRIM = 64.0;
const int HARD_STEPS = 8;
const int HARD_STEPS_PRIM = 8;
const int SOFT_STEPS = 32;


//x: basic pass, y: detail pass
const float MIN_STEPS = 20.0;
const float MAX_STEPS = 80.0;



const float MAX_CLIP = 16384.0;



const float S2O2 = 0.70710678118;
const float MAX_DIAG_RAD = 40.0;
const float MAX_DIAG_RAD2 = MAX_DIAG_RAD*S2O2*S2O2;

int primIdList[24]; // this is related to MAX_PRIM_IDS
float primAlreadyTested[8]; // this is related to MAX_PRIMTEST
int primIdListLength;

const float INVALID = 99999.0;

const mat2 ROT45 = mat2(
			S2O2,
			-S2O2,
			S2O2,
			S2O2
);

const float M_PI = 3.14159265359;
const vec3 crand0 = vec3(12.989, 78.233, 98.422);
const vec3 crand1 = vec3(93.989, 67.345, 54.256);
const vec3 crand2 = vec3(43.332, 93.532, 43.734);

const float LAND_ID = -3.0;
const float WATER_ID = -4.0;
const float SKY_ID = -5.0;
// const float EXPLODE_ID = -6.0;
// const float TREE_ID = -7.0;
const float TEST_ID1 = - 9.0;
const float TEST_ID2 = - 10.0;


// 432
// 501
// 678

const vec3 diagVals[9] = vec3[](
		vec3(  0.0,  0.0,  0.0  ),
		vec3(  MAX_DIAG_RAD,  0.0,  0.0  ),
		vec3(  MAX_DIAG_RAD2,  MAX_DIAG_RAD2,  0.0  ),
		
		vec3(  0.0,  MAX_DIAG_RAD,  0.0  ),
		vec3(  -MAX_DIAG_RAD2,  MAX_DIAG_RAD2,  0.0  ),
		vec3(  -MAX_DIAG_RAD,  0.0,  0.0  ),
		
		vec3(  -MAX_DIAG_RAD2,  -MAX_DIAG_RAD2,  0.0  ),
		vec3(  0.0,  -MAX_DIAG_RAD,  0.0  ),
		vec3(  MAX_DIAG_RAD2,  -MAX_DIAG_RAD2,  0.0  )
		
);






const int E_PRIMTEMP_VISMIN = 0;
const int E_PRIMTEMP_VISMAX = 1;
const int E_PRIMTEMP_BOUNDSMIN = 2;
const int E_PRIMTEMP_BOUNDSMAX = 3;
const int E_PRIMTEMP_CORNERDIS = 4; //E_PRIMTEMP_CORNERDISTHICKNESSPOWER
const int E_PRIMTEMP_MATPARAMS = 5;
const int E_PRIMTEMP_LENGTH = 6;


^INCLUDE:PRIMTEMPLATES^

^INCLUDE:MATERIALS^

const int NUM_VORO = 13; // 6; //
const vec3 voroOffsets[NUM_VORO] = vec3[](
			 
		vec3(  1.0,  0.0, 0.0  ),
		vec3(  0.5,  -1.0, 0.0  ),
		vec3(  -0.5,  -1.0, 0.0  ),
		vec3(  -1.0,  0.0, 0.0  ),
		vec3(  -0.5,  1.0, 0.0  ),
		vec3(  0.5,  1.0, 0.0  )
		
		,vec3(  0.0,  0.0, 1.0  ),
		vec3(  1.0,  0.0, 1.0  ),
		vec3(  0.5,  -1.0, 1.0  ),
		vec3(  -0.5,  -1.0, 1.0  ),
		vec3(  -1.0,  0.0, 1.0  ),
		vec3(  -0.5,  1.0, 1.0  ),
		vec3(  0.5,  1.0, 1.0  )
		
		
);


vec3 randPN(vec3 co) {
		
		vec3 myres = vec3(
				dot(co.xyz, crand0),
				dot(co.zyx, crand1),
				dot(co.yxz, crand2)
		);
		
		return fract(sin(myres) * 43758.8563)*2.0-1.0;
}
float randf3(vec3 co) {
		return fract(sin(dot(co.xyz , crand0)) * 43758.8563);
}


// ?????









float MAX_CAM_DIS;
float MAX_CAM_VOL_DIS;


float globGrassPos;
vec3 globExplodeVec;
vec2 globTexDyn;
vec2 globTexEarth;
vec2 globTexPrim;
vec2 globTexWater;
float globWoodDir;
vec3 globWoodCoords;

float globBestLimbDepth;
float globBoneRad;
bool globPrimaryRay;
int globBestLimbInd;
int globNumPrims;
float globWaterMod;
float globLastScenePos;
vec3 globLastSceneNorm;
float globCurSteps;
float globTotSteps;
float globIntersect;
float globTest;
float globTexTap;

//vec4 testObject;



vec4 texelRes1;
vec4 texelRes2;
vec4 pdVisMin;
vec4 pdVisMax;
vec4 pdBoundsMin;
vec4 pdBoundsMax;
vec4 pdCornerDis;
vec4 pdMatParmas;
vec4 boxDim;
vec2 boxPower;
vec3 boxCenterPoint;

in vec4 worldPos;
//in vec4 TexCoord0;

layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;
layout(location = 2) out vec4 FragColor2;
layout(location = 3) out vec4 FragColor3;
layout(location = 4) out vec4 FragColor4;
layout(location = 5) out vec4 FragColor5;
layout(location = 6) out vec4 FragColor6;
layout(location = 7) out vec4 FragColor7;




// >>>>>>>>>>> COMMON <<<<<<<<<<<<<

/*
// from shadertoy

#ifdef USE_PROCEDURAL
*/
float hash( float n ) { return fract(sin(n)*753.5453123); }
float hnoise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
	
    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
}
/*
#else
float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
	f = f*f*(3.0-2.0*f);
	
	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
	vec2 rg = texture2D( iChannel0, (uv+0.5)/256.0, -100.0 ).yx;
	return mix( rg.x, rg.y, f.z );
}
#endif
*/

float smin( float a, float b, float k )
{
	float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
	return mix( b, a, h ) - k*h*(1.0-h);
}

vec2 smin( vec2 a, vec2 b, float k )
{
	float h = clamp( 0.5 + 0.5*(b.x-a.x)/k, 0.0, 1.0 );
	return vec2( mix( b.x, a.x, h ) - k*h*(1.0-h), mix( b.y, a.y, h ) );
}

float smax( float a, float b, float k )
{
	float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
	return mix( a, b, h ) + k*h*(1.0-h);
}

struct Ray {
		vec3 Origin;
		vec3 Dir;
};


int intMod(int lhs, int rhs)
{
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


float sdSphere( vec3 p, float s )
{
		return length(p)-s;
}

float sdBox( vec3 p, vec3 b )
{
	vec3 d = abs(p) - b;
	return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}


float opI( float d1, float d2 )
{
		return max(d1,d2);
}
float opI3( float d1, float d2, float d3 )
{
		return max(max(d1,d2),d3);
}
float opI4( float d1, float d2, float d3, float d4 )
{
		return max(max(d1,d2),max(d3,d4));
}

float opS( float d1, float d2 )
{
		return max(-d2,d1);
}

float opSI( float d1, float d2, float d3 )
{
		return max(max(-d2,d1),d3);
}

float opD( float d1, float d2 )
{
		return d1+d2;
}

vec2 opU( vec2 d1, vec2 d2 )
{
		return mix(d2,d1,float(d1.x<d2.x));
}

vec3 opUTie( vec3 d1, vec3 d2 )
{
		return mix(
				mix(d2,d1,float(d1.x<d2.x)),
				mix(d2,d1,float(d1.z<d2.z)),
				float(d1.x == d2.x)
		);
		
		
}

vec2 opU3( vec2 d1, vec2 d2, vec2 d3 )
{
		vec2 res = mix(d2, d1, float(d1.x<d2.x));
		return mix(d3, res, float(res.x<d3.x));
}

float opUF( float d1, float d2 )
{
		return min(d1,d2);//mix(d2,d1,float(d1<d2));
		//(d1.x<d2.x) ? d1 : d2;
}
float opUF3( float d1, float d2, float d3 )
{
		return min(min(d1,d2),d3);//mix(d2,d1,float(d1<d2));
		//(d1.x<d2.x) ? d1 : d2;
}


vec3 opRep( vec3 p, vec3 c )
{
		return mod(p,c)-0.5*c;
}


^INCLUDE:SampleFuncs^

// vec4 getTexLin(vec3 pos) {
// 	return texture(Texture0, (pos)/volSizePrim );
// }

vec2 getEmpty3D(vec3 pos, float strength) {
		
		vec4 samp = getTexCubic(Texture0, pos-volMinReadyInPixels, volSizePrim);
		float maxDis = mix(0.5,1.0,distance(cameraPos,pos)/MAX_CAM_VOL_DIS);
		
		return vec2(
			mix(
					maxDis*strength,
					-maxDis,
					sqrt(samp.a)
			),
			samp.a
		);
}

float remBoxCommon(vec3 pos, float _res) {
	float res = _res;
	int i;
	float splashTot = 0.0;
	float splashDis = 0.0;
	float maxDis = 10.0;
	float waveMod = clamp(abs(res-1.0),0.0,1.0);
	for (i = 0; i < numExplodes; i++) {
			splashDis = distance(explodeArr[i].xyz+vec3(0.0,0.0,1.0),pos.xyz);
			splashTot += (
					
					(sin(splashDis*2.0+explodeArr[i].w*0.5-curTime*4.0)+1.0)*explodeArr[i].w*0.5
					
			) * 
			clamp(1.0-splashDis/maxDis, 0.0, 1.0);
	}
	res = opD(res,splashTot*waveMod*1.5);
	
	
	res = opS(
		res,
		sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) ) //8.0 //CAM_BOX_SIZE
	);
	
	return res;
}

float remBox(vec3 pos, float resBase, float strength) {
	float res = resBase;
	
	// res = opS(
	// 	res,
	// 	sdSphere(pos-(cameraPos+lookAtVec*100.0),200.0)
	// );
	
	vec2 emptyVal = vec2(0.0);
	
	if (
		//hasDestruction &&
		all(greaterThan(pos,volMinReadyInPixels)) &&
		all(lessThan(pos,volMaxReadyInPixels))
	) {
		
		emptyVal = getEmpty3D(pos, strength);
	
		if (emptyVal.y > 0.0) {
			res = opI(
				res,
				-(emptyVal.x)*8.0
			);
		}
		
	}
	
	res = remBoxCommon(pos,res);
	
	return res;
	
}



float remBoxPrim(vec3 pos, float resBase, float strength) {
	float res = resBase;
	
	
	vec2 emptyVal = vec2(0.0);
	
	if (
		all(greaterThan(pos,volMinReadyInPixels)) &&
		all(lessThan(pos,volMaxReadyInPixels))
	) {
		
		emptyVal = getEmpty3D(pos, strength);
	
		res = opS(
			res,
			emptyVal.x*0.5
		);
		
		if (emptyVal.y > 0.0) {
			res = opD(
				res,
				(1.0-getTexLin(Texture13, pos*(9.0), voroSize).r)*emptyVal.y*2.0*strength
			);
		}
		
	}
	
	res = remBoxCommon(pos,res);
	
	return res;
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


vec2 psDistanceV2(vec3 testPoint, vec3 sp0, vec3 sp1)
{
		vec3 v = sp1 - sp0;
		vec3 w = testPoint - sp0;

		float d0 = distance(testPoint, sp0);
		float d1 = distance(testPoint, sp1);
		float d2 = distance(sp0, sp1);

		float c1 = dot(w, v);
		if ( c1 <= 0 ) {
				return vec2(d0, 0.0);
		}

		float c2 = dot(v, v);
		if ( c2 <= c1 ) {
				return vec2(d1, 1.0);
		}

		float b = c1 / c2;
		vec3 testPoint2 = sp0 + b * v; // testPoint2 is the nearest point on the line
		return vec2(distance(testPoint, testPoint2), distance(testPoint2, sp0) / d2 );
}

float lineSphereDis(
	vec3 p1, // line seg 1
	vec3 p2, // line seg 2
	vec3 p3, // sphere center 
	float r // sphere rad	
) {
	// from stack overflow
	
	vec3 d = p2 - p1;

	float a = dot(d, d);
	float b = 2.0 * dot(d, p1 - p3);
	float c = dot(p3, p3) + dot(p1, p1) - 2.0 * dot(p3, p1) - r*r;
	
	return c;
}




float getWoodGrain(float normalUID, vec3 wpInPixels, float woodRad, float boardDir, float stretchAmount) {
		float woodDiam = woodRad * 2.0;

		vec3 newPos = vec3(0.0);//wpInPixels.xyz;

		if (boardDir == 0.0) {
				newPos = wpInPixels.xyz;
		}
		if (boardDir == 1.0) {
				newPos = wpInPixels.xzy;
		}
		if (boardDir == 2.0) {
				newPos = wpInPixels.yxz;
		}
		if (boardDir == 3.0) {
				newPos = wpInPixels.yzx;
		}
		if (boardDir == 4.0) {
				newPos = wpInPixels.zxy;
		}
		if (boardDir == 5.0) {
				newPos = wpInPixels.zyx;
		}

		vec2 woodCenter = floor( (newPos.xy + woodRad) / woodDiam) * woodDiam;
		vec2 woodVec = normalize(newPos.xy - woodCenter);
		float woodLen = newPos.z;


		woodCenter.y *= stretchAmount;
		newPos.y *= stretchAmount;


		float woodAngle = atan(woodVec.y, woodVec.x);
		float finalMod = (sin(

																						( distance(newPos.xy + mod(normalUID, 4.0), woodCenter) + (woodRad / 2.0) ) *
																						( (8.0 + sin(woodAngle * 24.0 ) * 0.0625 + sin(woodAngle * 12.0 ) * 0.125 + sin(woodLen / 16.0) * 0.5 + sin(woodLen / 4.0) * 0.25  ) / (woodRad / sqrt(2.0)) )

																				));
		if (finalMod < 0.0) {
				finalMod = (1.0 - finalMod) / 2.0;
		}

		return finalMod;
}

// vec3 opTwist( vec3 p )
// {
//     float amount = 10.0;
		
//     float  c = cos(amount*p.z+amount + curTime);
//     float  s = sin(amount*p.z+amount + curTime);
//     mat2   m = mat2(c,-s,s,c);
//     return vec3(m*p.xy,p.z);
// }

// vec2 opRotate( vec2 p, float theta )
// {
//     mat2 m = mat2(
//         cos(theta),
//         -sin(theta),
//         sin(theta),
//         cos(theta)
//     );
		
//     return m*p;
// }


// >>>>>>>>>>> END COMMON <<<<<<<<<<<<<

#ifdef DOPRIM

void getPrimVals(int _ptInd) {
		
		int ptInd = _ptInd;
		
		int primReadOffset = ptInd*E_PRIMTEMP_LENGTH;
		
		
		if (ptInd == -1) {
				
				
				pdVisMin = paramArrGeom[E_PRIMTEMP_VISMIN];
				pdVisMax = paramArrGeom[E_PRIMTEMP_VISMAX];
				pdBoundsMin = paramArrGeom[E_PRIMTEMP_BOUNDSMIN];
				pdBoundsMax = paramArrGeom[E_PRIMTEMP_BOUNDSMAX];
				pdCornerDis = paramArrGeom[E_PRIMTEMP_CORNERDIS];
				pdMatParmas = paramArrGeom[E_PRIMTEMP_MATPARAMS];
				
				
		}
		else {
				pdVisMin = primTemp[primReadOffset + E_PRIMTEMP_VISMIN];
				pdVisMax = primTemp[primReadOffset + E_PRIMTEMP_VISMAX];
				pdBoundsMin = primTemp[primReadOffset + E_PRIMTEMP_BOUNDSMIN];
				pdBoundsMax = primTemp[primReadOffset + E_PRIMTEMP_BOUNDSMAX];
				pdCornerDis = primTemp[primReadOffset + E_PRIMTEMP_CORNERDIS];
				pdMatParmas = primTemp[primReadOffset + E_PRIMTEMP_MATPARAMS];
		}
		
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





vec2 sdBoxM( float m, vec3 p, vec3 b ) {
		vec3 d = abs(p) - b;
		return vec2(
				min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0)),
				m
		);
}



vec4 getUVW(
		vec3 myWP,
		vec3 centerPos, 
		vec4 box_dim,
		vec4 uvwScale,
		bool mirrored
) {
		
		globIntersect = 999.0;
		
		vec3 minCorner = centerPos-(box_dim.xyz+box_dim.w);
		vec3 pos = (myWP-minCorner)*uvwScale.xyz;
		
		vec3 centerOffset = myWP-centerPos;
		vec3 innerBoxSize = box_dim.xyz;
		float cornerRad = box_dim.w;
		
		vec3 uvwCoords = pos.xyz;
		
		
		
		
		vec3 newOffset = max(abs(centerOffset)-innerBoxSize,0.0);
		vec2 newNorm1 = normalize(newOffset.xy)*sign(centerOffset.xy);
		vec2 newNorm2 = normalize(vec2(length(newOffset.xy), newOffset.z));
		
		vec3 centerOffsetNorm = abs(centerOffset/innerBoxSize);
		
		// y side
		if (newNorm1.x == 0.0) {
				uvwCoords.x = pos.x;
				uvwCoords.y = pos.z;
				
				//uvwCoords.x *= innerBoxSizeFloored.x;
		}
		
		// x side
		if (newNorm1.y == 0.0) {
				uvwCoords.x = pos.y;
				uvwCoords.y = pos.z;
				
				//uvwCoords.x *= innerBoxSizeFloored.y;
		}
		
		float curPhi = atan(newNorm1.y, newNorm1.x);
		float curThe = atan(newNorm2.y, newNorm2.x);
		
		float angMod = 
				(uvwScale.w*2.0/M_PI) *
				(max(floor(sqrt(box_dim.w*box_dim.w*2.0)),1.0));
		
		
		// side corner
		if (newNorm1.x*newNorm1.y != 0.0) {
				uvwCoords.x = curPhi*angMod;
				uvwCoords.y = pos.z;
		}
		
		// top corner
		if (newNorm2.x*newNorm2.y != 0.0) {
				uvwCoords.y = curThe*angMod;
		}
		
		// top
		if (newNorm2.x == 0.0) {
				if (centerOffsetNorm.x > centerOffsetNorm.y) {
						uvwCoords.x = pos.y;
						uvwCoords.y = pos.x;
						
						if (centerOffset.x > 0.0) {
								uvwCoords.y *= -1.0;
								
								uvwCoords.y += 0.5;
						}
				}
				else {
						uvwCoords.x = pos.x;
						uvwCoords.y = pos.y;
						
						if (centerOffset.y > 0.0) {
								uvwCoords.y *= -1.0;
								
								uvwCoords.y += 0.5;
								
						}
				}
				
				globIntersect = abs(centerOffsetNorm.x - centerOffsetNorm.y);
				
		}
		
		
		uvwCoords.z = 0.0;
		
		return vec4(uvwCoords,newNorm2.x);
}


float getBrick( vec3 uvwCoords) {
		
		float mv1 = float(mod(uvwCoords.y,2.0) < 1.0);
		float mv2 = float(mod(uvwCoords.z,2.0) < 1.0);
		
		vec3 res = mod(uvwCoords.xyz + vec3(0.5*(mv1+mv2),0.0,0.0), vec3(1.0));
		
		res = abs((res-0.5))*2.0;
		
		return max(max(res.x,res.y),res.z);
}


float getShingle(vec2 uvwBase) {
		

		float finalDis = 0.0;

		vec2 iUV = ivec2(uvwBase.xy);
		vec2 dis = uvwBase.xy - floor(uvwBase.xy);

		dis.y = 1.0 - dis.y;

		float modv = mod(iUV.x + iUV.y, 2.0);

		if ( modv < 1.0 ) {

		}
		else {
				dis.x = 1.0 - dis.x;
		}
		
		float disLength = length(dis);

		if (disLength < 1.0) {
				finalDis = 1.0 - (dis.y * 0.5 + 0.5);
				
				finalDis = mix(finalDis,disLength,pow(disLength,8.0));
		}
		else {
				finalDis = 1.0 - dis.y * 0.5;
		}
		
		// finalDis = mix(
		//     1.0 - (dis.y * 0.5 + 0.5),
		//     clamp(length(dis),0.0,1.0)
		// );

		return finalDis;
		

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

float udMinBox( vec3 vectorFromCenter, vec4 box_dim )
{
		vec3 absVecFromCenter = abs(vectorFromCenter);
		
		vec3 newBoxDim = box_dim.xyz;
		newBoxDim.z = 0.0;
		if (newBoxDim.x > newBoxDim.y) {
				newBoxDim.y = 0.0;
		}
		else {
				newBoxDim.x = 0.0;
		}
		
		return length(max(absVecFromCenter-newBoxDim, vec3(0.0) ));
		
}



float mapSolid( vec3 pos ) {
		
		int i;
		int m;
		int n;
		int primDataInd;
		int primTempInd;
		//int diagInd = 0;
		
		float dis1;
		float dis2;
		
		float gradVal = 0.0;
		vec3 norVal = vec3(0.0);
		vec4 cellRes = vec4(0.0);
		
		float testDis = 0.0;
		
		vec4 uvwCoords = vec4(0.0);
		vec4 uvwCoordsMir = vec4(0.0);
		vec4 uvwCoordsMirScaled = vec4(0.0);
		
		//vec4 boxRes = vec4(0.0);
		
		
		
		vec4 boxRes1 = vec4(0.0);
		vec4 boxRes2 = vec4(0.0);
		
		float subResult1 = MAX_CAM_VOL_DIS;
		float subResult2 = MAX_CAM_VOL_DIS;
		
		vec3 res1 = vec3(MAX_CAM_VOL_DIS,SKY_ID,MAX_CAM_VOL_DIS);
		vec3 res2 = res1;
		
		float uvwDepth = MAX_CAM_VOL_DIS;
		float minBoxRes = 0.0;
		
		//vec3 diagRes = vec3(0.0);
		
		vec3 visCenterPoint = vec3(0.0);
		vec3 visDim = vec3(0.0);
		
		
		
		//bool notInside = true;
		//vec3 bestBoxCenterPoint = vec3(0.0);
		//vec4 bestBoxDim = vec4(0.0);
		
		vec2 tempDis;
		vec2 tempDis1;
		vec2 tempDis2;
		vec2 tempDis3;
		float shingleVal;
		float visBoxDis = 0.0;
		
		
		
		
		
		for (m = 0; m < primIdListLength; m++) {
				
				primDataInd = primIdList[m];
				
				if (primDataInd == -1) {
						texelRes1 = paramFetch1;
						texelRes2 = paramFetch2;
				}
				else {
						texelRes1 = texelFetch(Texture1, primDataInd);
						texelRes2 = texelFetch(Texture1, primDataInd+1);
				}
				
				//diagInd = int(texelRes2.x);
				primTempInd = int(texelRes1.w);
				
				if (primTempInd == 0) {
						break;
				}
				
				
				getPrimVals(primTempInd);
				
				visCenterPoint = vec3(
						(texelRes1.xyz + pdVisMin.xyz) +
						(texelRes1.xyz + pdVisMax.xyz)
				)*0.5;
				
				visDim = (pdVisMax.xyz - pdVisMin.xyz)*0.5;
				
				visBoxDis = sdBox(pos-visCenterPoint, visDim);
				
				
				boxRes1.xyz = udRoundBox(
					pos-boxCenterPoint,
					boxDim,
					boxPower,
					pdCornerDis.y
				);
				boxRes2.xyz = udRoundBox(
					pos-boxCenterPoint,
					boxDim,
					boxPower,
					pdCornerDis.y*2.0
				);
				
				minBoxRes = udMinBox(pos-boxCenterPoint, boxDim);
				
				// diagRes = udRoundBox(
				//   pos-(texelRes1.xyz+diagVals[diagInd]),
				//   vec4(0.0,0.0,MAX_DIAG_RAD,MAX_DIAG_RAD),
				//   vec2(1.0),
				//   MAX_DIAG_RAD
				// );
				
				// testDis = opI(//opI3(
				//   max(boxRes.x,boxRes.y) * 0.5,
				//   //diagRes.x * 0.5,
				//   visBoxDis
				// );
				
				
				
				// if (testDis < res.x) {
				//     //bestBoxCenterPoint = boxCenterPoint;
				//     //bestBoxDim = boxDim;
				//     notInside = (boxRes.x > boxRes.y);
				// }
				
				subResult1 = min(
						subResult1,
						opI(
								boxRes1.z * 0.5,
								visBoxDis
						)
				);
				
				// uvwDepth = min(
				//     uvwDepth,
				//     max(boxRes1.x,boxRes1.y)
				// );
				
				res1 = opUTie(
						res1,
						vec3(
								opI(
									max(boxRes1.x,boxRes1.y) * 0.5,
									visBoxDis
								),
								float(primDataInd),
								minBoxRes
						)
				);
				
				
				
				subResult2 = min(
						subResult2,
						opI(
								boxRes2.z * 0.5,
								visBoxDis
						)
				);
				
				res2 = opUTie(
						res2,
						vec3(
								opI(
									max(boxRes2.x,boxRes2.y) * 0.5,
									visBoxDis
								),
								float(primDataInd),
								minBoxRes
						)
				);
				
		}
		
		
		// make subtracted region slightly bigger with opD to counter bugs
		if (subResult1 == MAX_CAM_VOL_DIS) {
				
		}
		else {
				res1.x = opS(res1.x,opD(subResult1,-0.01) ); 
		}
		
		if (subResult2 == MAX_CAM_VOL_DIS) {
				
		}
		else {
				res2.x = opS(res2.x,opD(subResult2,-0.01));
		}
		
		//res.x = opD(res.x,0.1);
		
		// globTexPrim.x = TEX_PLASTER; 
		// return res1.x;
		
		// if (res.x == MAX_CAM_VOL_DIS) {
		//     return res.x;
		// }
		
		
		//////////////
		
		
		
		vec2 res = res2.xy;
		vec4 boxRes = boxRes2;
		
		
		primDataInd = int(res.y);
		
		if (primDataInd == -1) {
				texelRes1 = paramFetch1;
				texelRes2 = paramFetch2;
		}
		else {
				texelRes1 = texelFetch(Texture1, primDataInd);
				texelRes2 = texelFetch(Texture1, primDataInd+1);
		}
		
		//diagInd = int(texelRes2.x);
		primTempInd = int(texelRes1.w);
		
		float origDis = res1.x;
		float myDis = 0.0;
		
		
		
		
		if (primTempInd == 0) {
				
		}
		else {
				getPrimVals(primTempInd);
				
				visCenterPoint = vec3(
						(texelRes1.xyz + pdVisMin.xyz) +
						(texelRes1.xyz + pdVisMax.xyz)
				)*0.5;
				
				visDim = (pdVisMax.xyz - pdVisMin.xyz)*0.5;
				visBoxDis = sdBox(pos-visCenterPoint, visDim);
				
				// origDis = opI(
				//     origDis,
				//     visBoxDis  
				// );
				
				uvwCoords = getUVW(
					pos,
					boxCenterPoint, 
					boxDim,
					vec4(1.0),
					false
				);
				uvwCoords.z = 
				
						udRoundBox(
											pos-boxCenterPoint,
											boxDim,
											boxPower,
											pdCornerDis.y
										).x;
						//uvwDepth;
				
				uvwCoordsMir = uvwCoords;
				
				uvwCoordsMirScaled = uvwCoords*0.5;
				
				//float outer = box_dim.w;
				//float inner = wallThickness.x;
				
				// boxRes = udRoundBoxLayered(
				//   pos-boxCenterPoint,
				//   boxDim,
				//   boxPower,
				//   vec4(boxDim.w,boxDim.w-0.4,boxDim.w-0.8,boxDim.w-0.4),
				//   vec4(0.4,0.2,0.4,0.6)*2.0  
				// )*0.5;
				
				boxRes.x = res.x;
				boxRes.y = opD(res.x,0.4);
				boxRes.z = opD(res.x,0.6);
				boxRes.w = opD(res.x,0.2);
				
				boxRes.x = opSI(boxRes.x,boxRes.y,res1.x);
				boxRes.y = opSI(boxRes.y,boxRes.z,res1.x);
				boxRes.z = opI(boxRes.z,res1.x);
				boxRes.w = opI(boxRes.w,res1.x);
				
				// tempDis1 = vec2(boxRes.x,TEX_SHINGLE);
				// tempDis2 = vec2(boxRes.y,TEX_EARTH);
				// tempDis3 = vec2(boxRes.z,TEX_PLASTER);
				// res = 
				//     opU3(tempDis1,tempDis2,tempDis3);
				//     //vec2(res1.x,TEX_PLASTER);
				
				// if (subResult == MAX_CAM_VOL_DIS) {
						
				// }
				// else {
				//     //opS: max(-d2,d1);
				//     boxRes.xyzw = max(boxRes.xyzw, vec4(-subResult) );
				// }
				
				
				
				tempDis = opU3(
						sdBoxM(
								2.0,
								opRep(uvwCoordsMirScaled.xyz,vec3(2.0)),
								vec3(0.125,0.84,10.0)
						),
						sdBoxM(
								5.0,
								opRep(uvwCoordsMirScaled.xyz+vec3(1.0,1.0,0.0),vec3(4.0,2.0,2.0)),
								vec3(1.97,0.125,10.0)
						),
						sdBoxM(
								2.0,
								opRep(
										uvwCoordsMirScaled.xyz + vec3(1.0,uvwCoordsMirScaled.x,0.0),
										vec3(2.0)),
								vec3(0.8,0.15,10.0)
						)
				);
				globWoodDir = tempDis.y;
				globWoodCoords = uvwCoordsMirScaled.xyz;
				tempDis.y = TEX_WOOD;
				
				if (pdMatParmas.x < 2.0) {
						//////////
						// brick
						//////////        
						
						// tempDis1 = vec2(boxRes.x,TEX_WOOD);
						// tempDis2 = vec2(boxRes.y,TEX_WOOD);
						// tempDis3 = vec2(boxRes.z,TEX_WOOD);
						
						if (pdMatParmas.x < 1.0) {
								myDis = boxRes.w;
						}
						else {
								myDis = origDis;
						}
						
						tempDis.x = opI(tempDis.x,origDis);
						
						cellRes.x = getBrick(uvwCoords.xyz*vec3(0.5,1.0,0.5));
						
						dis1 = (max(cellRes.x,0.8)-0.8);
						dis2 = texture(Texture2, uvwCoords.xy/16.0).r*0.3 + float(pdMatParmas.x > 0.0);
						
						//min(dis1,dis2));
						
						tempDis1.x = opD(myDis,dis1);
						tempDis2.x = opD(origDis,dis2);
						tempDis2.x = opS(tempDis2.x,tempDis1.x);
						tempDis2.y = TEX_PLASTER;
						
						if (
								(cellRes.x > 0.95)
								// || ( min(dis1,dis2) < dis1 )
						) {
								tempDis1.y = TEX_PLASTER;
						}
						else {
								tempDis1.y = TEX_BRICK;
						}
						
						
						res = opU(tempDis1,tempDis2);
						
						if (pdMatParmas.x < 1.0) {
								res.x = opS(res.x,tempDis.x);
								res = opU(res,tempDis);
						}
						
						
						
				}
				else {
						tempDis1 = vec2(boxRes.x,TEX_SHINGLE);
						tempDis2 = vec2(boxRes.y,TEX_WOOD);
						tempDis3 = vec2(boxRes.z,TEX_WOOD);
						
						shingleVal = getShingle(abs(uvwCoords.xy*2.0) );
						
						tempDis1.x = opD(tempDis1.x,shingleVal*0.2 + (0.25-clamp(pos.z - (visCenterPoint.z-visDim.z),0.0,0.25))*shingleVal );
						
						tempDis2.x = opI(
								tempDis2.x,
								sdBox(
										opRep(
												uvwCoords.xyz + vec3(
												float(mod(uvwCoords.y,1.0) < 0.5)*2.0    
												,0.0,0.0),
												vec3(4.0,0.5,2.0)
										),
										vec3(1.95,0.23,10.0)
								)
						);
						
						
						
						
						tempDis3.x = opI(tempDis3.x,tempDis.x);
						
						
						
						res = opU3(tempDis1,tempDis2,tempDis3);
				}
				
				
				
				
				
				
				// res.x = opI(
				//     res.x,
				//     visBoxDis  
				// );
				
				
				
				//res = tempDis;
				
				
				globTexPrim.x = res.y;
				
				globWoodCoords = uvwCoords.xyz*0.5;
				
				if (globTexPrim.x == TEX_SHINGLE) {
						globTexPrim.y = shingleVal*0.3+0.3;
				}
				
				//res = tempDis;
				
				//getWoodGrain
				
				
				//cellRes.x;
				
				//brick(uvwCoords.xyz, v, bool isVert)
				
				
				//res.x = opD(res.x,getShingle(uvwCoords.xy)*0.2);
				
				
				
				// globTexPrim.y = gradVal;
				
		}
		
		
		
		
		
		
		///////////////
		
		
		
		
		res.x = remBoxPrim(pos, res.x, 0.5);
		
		//
		
		
		
		
		
		
		
		
		// if (primDataInd == -1) {
		//     res.x = opS(
		//         res.x,
		//         opI(
		//           boxRes.z*0.5,
		//           sdBox(pos-visCenterPoint, visDim)
		//         )    
		//     );
		// }
		
		
		
		
		
		
		return res.x;
		
}

vec2 castSolid( vec3 ro, vec3 rd, vec2 minMaxT, float fNumSteps ) {
		int p = 0;
		int numSteps = int(fNumSteps);
		
		float res = 0.0;
		float t = minMaxT.x;
		
		
		float SOLID_PREC = 0.002;
		float SOLID_PREC2 = 0.002;
		
		vec3 pos;
		
		for( p = 0; p < numSteps; p++ ) {
				
				pos = ro+rd*t;
				
				res = mapSolid( pos );
				
				if (
						(res < (SOLID_PREC)) ||
						(t>minMaxT.y)
				) {
						break;
				}
				t += res;
		}
		
		
		globCurSteps += float(p);
		
		
		if (res < SOLID_PREC) {
				
				t -= SOLID_PREC*1.5;
				for( p = 0; p < TOT_DETAIL_STEPS; p++ ) {
						
						pos = ro+rd*t;
						
						res = mapSolid( pos );
						
						if (res < SOLID_PREC2) {
								break;
						}
						
						t += res*0.5;
				}
				
				
				globCurSteps += float(p);
				
				//p += TOT_DETAIL_STEPS;
		}
		else {
				globTexPrim.xy = vec2(0.0);
		}
		
		
		// if (res < SOLID_PREC) {
				
		// }
		// else {
		//     globTexPrim.xy = vec2(0.0);
		// }
		
		
		return vec2(t,res);
}


float pointBoxDis(vec3 rectMin, vec3 rectMax, vec3 p) {
		
		vec3 res0 = vec3(0.0);
		vec3 res1 = vec3(rectMin-p);
		vec3 res2 = vec3(p-rectMax);
		vec3 res3 = max(max(res0,res1),res2);
		return length(res3);
}

float pointRectDist(vec2 point, vec2 rectCenter, vec2 rectDimHalf) {
		vec2 dist = max(abs(point - rectCenter) - rectDimHalf, 0.0);
		return length(dist);
}


bool testHitDiag(vec3 ro, vec3 rd, vec3 centerPoint) {
		
		vec3 p1 = ro - centerPoint;
		vec3 p2 = ro + rd*MAX_CAM_DIS - centerPoint;
						
		p1.xy = ROT45*p1.xy;
		p2.xy = ROT45*p2.xy;
		
		p1 += centerPoint;
		p2 += centerPoint;
		
		vec2 res = aabbIntersect(
				p1,
				normalize(p2-p1),
				centerPoint-MAX_DIAG_RAD*S2O2,
				centerPoint+MAX_DIAG_RAD*S2O2
		);
		
		return (res.x <= res.y);
}



vec3 lineStep(
		vec3 ro,
		vec3 rd,
		float maxDis
		//,int _startIndex
) {
		
		
		//int startIndex = _startIndex;
		
		vec3 oneVec = vec3(1.0);
		//float seed = 0.0;
		
		vec3 p1 = (ro);
		vec3 p2 = (ro + rd*maxDis);
		vec3 d = p2-p1;
		vec3 dabs = abs(d);
		int i;
		int k;
		int m;
		int q;
		int n = int(
				max(max(dabs.x,dabs.y),dabs.z)
		)*2;
		
		vec3 s = d/float(n);
		
		
		
		
		vec3 visCenterPoint = vec3(0.0);
		vec3 visDim = vec3(0.0);
		
		vec2 hitBox = vec2(0.0);
		vec2 fullHitBox = vec2(0.0);
		vec2 hitBoxFinal = vec2(0.0);
		vec2 cellBox = vec2(0.0);
		
		vec2 curHitBox;
		vec2 innerBox;
		vec2 hitBoxXYZ[3];
		

		vec3 p = p1;
		
		
		
		vec3 seBoxDis1 = vec3(0.0);
		vec3 seBoxDis2 = vec3(0.0);
		
		vec2 radMod = vec2(0.0);
		
		float j = 0.0;
		
		
		
		
		float uniqueId = 0.0;
		
		vec3 minPlanes;
		vec3 maxPlanes;
		
		vec3 minPlanesMacro;
		vec3 maxPlanesMacro;
		
		
		vec3 centerPoint = vec3(0.0);
		vec3 centerPointMacro = vec3(0.0);
		//vec3 centerPointMacroUnit = vec3(0.0);
		ivec3 iCenterPointMacroUnit = ivec3(0);
		
		vec3 testPoint;
		
		vec4 sampVal;
		
		float thresh = 0.0;
		float minPrec = 0.001;
		
		bool onlyHitCorner = false;
		bool didHit = false;
		bool didHitDiag = false;
		bool didHitFinal = false;
		bool isInside = false;
		
		int tempInt = 0;
		int diagInd = 0;
		
		
		vec2 newBoxPower = vec2(2.0);
		
		vec3 minBox = vec3(0.0);
		vec3 maxBox = vec3(0.0);
		vec3 minVisBox = vec3(0.0);
		vec3 maxVisBox = vec3(0.0);
		
		vec3 firstHitPoint1 = vec3(0.0);
		vec3 closestHitPoint1 = vec3(0.0);
		
		vec4 closestPointOnRay1 = vec4(0.0);
		vec4 closestPointOnRay2 = vec4(0.0);
		
		vec3 firstHitPoint2 = vec3(0.0);
		vec3 closestHitPoint2 = vec3(0.0);
		
		vec3 newRD = rd*minPrec;
		
		float cellBoxOffset = 0.001;
		
		int volSizePrimDiv = int(volSizePrim/PRIM_DIV);
		ivec3 vpMin = ivec3(0);
		ivec3 vpMax = ivec3(volSizePrimDiv);
		//ivec3 cpmOffset = ivec3(volMinReadyInPixels/PRIM_DIV);
		
		//cpmOffset -= volSizePrimDiv/2;
		
		int primDataInd = 0;
		int primTempInd = 0;
		
		int curInd = 0;
		int primAlreadyTestedInd = 0;
		
		primIdListLength = 0;
		
		for (i = 0; i < MAX_PRIM_IDS; i++) {
				primIdList[i] = -1;
				primAlreadyTested[i] = -1.0;
		}
		
		
		
		bool alreadyExists = false;
		
		didHitFinal = false;
		hitBoxFinal.x = INVALID;
		hitBoxFinal.y = -INVALID;
		
		int myBaseInd = -1;
		
		for (i = 0; i < n; i++) {
				
				
				testPoint = p + newRD;
				
				
				minPlanes = floor(testPoint);
				maxPlanes = minPlanes + oneVec;
				centerPoint = (minPlanes+maxPlanes)*0.5;
				
				minPlanesMacro = floor(minPlanes/PRIM_DIV)*PRIM_DIV;
				maxPlanesMacro = minPlanesMacro + PRIM_DIV;
				centerPointMacro = (minPlanesMacro+maxPlanesMacro)*0.5;
				//centerPointMacroUnit = ;
				iCenterPointMacroUnit = ivec3((centerPointMacro-volMinReadyInPixels)/PRIM_DIV);
				
				//iCenterPointMacroUnit -= cpmOffset;
				
				//iCenterPointMacroUnit.z += 4;
				
				//cellBox = aabbIntersect(ro,rd,minPlanes,maxPlanes);
				cellBox = aabbIntersect(ro,rd,minPlanesMacro-cellBoxOffset,maxPlanesMacro+cellBoxOffset);
				
				
				if (
						
						//true
						
						//all(greaterThan(p,volMinReadyInPixels)) &&
						//all(lessThan(p,volMaxReadyInPixels))
						
						all(greaterThanEqual(iCenterPointMacroUnit,vpMin)) &&
						all(lessThan(iCenterPointMacroUnit,vpMax))
						
						// (iCenterPointMacroUnit.x >= 0) &&
						// (iCenterPointMacroUnit.x < volSizePrimDiv) &&
						// (iCenterPointMacroUnit.y >= 0) &&
						// (iCenterPointMacroUnit.y < volSizePrimDiv) &&
						// (iCenterPointMacroUnit.z >= 0) &&
						// (iCenterPointMacroUnit.z < volSizePrimDiv)
				) {
						
						//iCenterPointMacroUnit += cpmOffset;
						
						myBaseInd = (
								iCenterPointMacroUnit.x +
								iCenterPointMacroUnit.y*volSizePrimDiv +
								iCenterPointMacroUnit.z*volSizePrimDiv*volSizePrimDiv
						)*PRIMS_PER_MACRO*VECS_PER_PRIM;
						
						
						for (m = 0; m < PRIMS_PER_MACRO; m++) { //startIndex
								
								alreadyExists = false;
								
								//%%%%%%%%
								// if (m == -1) {
								//     startIndex = 0;
								//     primDataInd = -1;
								//     texelRes1 = paramFetch1;
								//     texelRes2 = paramFetch2;
								// }
								// else {
										primDataInd = myBaseInd + m*VECS_PER_PRIM;
										texelRes1 = texelFetch(Texture1, primDataInd).xyzw;
										texelRes2 = texelFetch(Texture1, primDataInd+1).xyzw;
								//}
								//%%%%%%%%
								
								
								
								for (q = 0; q < MAX_PRIMTEST; q++) {
										
										curInd = intMod(q + primAlreadyTestedInd, MAX_PRIMTEST);
										
										if (primAlreadyTested[q] == texelRes2.y) {
												alreadyExists = true;
												break;
										}
								}
								
								
								
								if (alreadyExists) {
										
								}
								else {
										
										curInd = intMod(primAlreadyTestedInd, MAX_PRIMTEST);
										primAlreadyTested[curInd] = texelRes2.y;
										primAlreadyTestedInd++;
										
										diagInd = int(texelRes2.x);
										
										primTempInd = int(texelRes1.w);
										if (primTempInd == 0) {
												globNumPrims = m;
												break;
										}
										
										getPrimVals(primTempInd);
										
										minVisBox = texelRes1.xyz+pdVisMin.xyz;
										maxVisBox = texelRes1.xyz+pdVisMax.xyz;
										
										hitBox = aabbIntersect(
												p1,
												rd,
												minVisBox,
												maxVisBox
										);
										
										
										
										radMod.y = boxDim.w;
										minBox = boxCenterPoint-boxDim.xyz;
										maxBox = boxCenterPoint+boxDim.xyz;
										
										
										didHitDiag = 
										//true;
										testHitDiag(
												ro,
												rd,
												texelRes1.xyz+diagVals[diagInd]
										);
										
										
										if (
												(hitBox.x <= hitBox.y)
												&& didHitDiag
										) {
												
												
												
												
												
												hitBoxXYZ[0] = aabbIntersect(
														p1,
														rd,
														minBox-radMod.xyy,
														maxBox+radMod.xyy
												);
												
												hitBoxXYZ[1] = aabbIntersect(
														p1,
														rd,
														minBox-radMod.yxy,
														maxBox+radMod.yxy
												);
												
												hitBoxXYZ[2] = aabbIntersect(
														p1,
														rd,
														minBox-radMod.yyx,
														maxBox+radMod.yyx
												);
												
												for (k = 0; k < 3; k++) {
														hitBoxXYZ[k].x = max(hitBox.x,hitBoxXYZ[k].x);
														hitBoxXYZ[k].y = min(hitBox.y,hitBoxXYZ[k].y);
												}
												
												
												onlyHitCorner = !(
														(hitBoxXYZ[0].x <= hitBoxXYZ[0].y) ||
														(hitBoxXYZ[1].x <= hitBoxXYZ[1].y) ||
														(hitBoxXYZ[2].x <= hitBoxXYZ[2].y)    
												) || ((boxDim.x+boxDim.y+boxDim.z) == 0.0);
												
												
												
												
												
												didHit = false;
												
												
												newBoxPower = max(boxPower,vec2(1.5));
												
												
												if (onlyHitCorner) {
														
														//
														
														//
														
														//fullHitBox = aabbIntersect(p1,rd,minBox-boxDim.w, maxBox+boxDim.w);
														
														firstHitPoint1 = (hitBox.x)*rd+ro;
														firstHitPoint2 = (hitBox.y)*rd+ro;
														
														closestHitPoint1 = clamp(
																firstHitPoint1,
																minBox,
																maxBox
														);
														
														closestPointOnRay1 = pointSegDistance(closestHitPoint1,p1,p2);
														seBoxDis1 = udRoundBox(
																closestPointOnRay1.xyz-boxCenterPoint,
																boxDim,
																newBoxPower,
																pdCornerDis.y
														);
														didHit = seBoxDis1.x <= 0.0;
												}
												else {
														
														for (k = 0; k < 3; k++) {
																
																curHitBox = hitBoxXYZ[k];
																
																if (curHitBox.x <= curHitBox.y) {
																		firstHitPoint1 = (curHitBox.x)*rd+ro;
																		firstHitPoint2 = (curHitBox.y)*rd+ro;
																		
																		closestHitPoint1 = clamp(
																				firstHitPoint1,
																				minBox,
																				maxBox
																		);
																		closestHitPoint2 = clamp(
																				firstHitPoint2,
																				minBox,
																				maxBox
																		);
																		
																		
																		closestPointOnRay1 = pointSegDistance(closestHitPoint1,p1,p2);
																		seBoxDis1 = udRoundBox(
																				closestPointOnRay1.xyz-boxCenterPoint,
																				boxDim,
																				newBoxPower,
																				pdCornerDis.y
																		);
																		
																		closestPointOnRay2 = pointSegDistance(closestHitPoint2,p1,p2);
																		seBoxDis2 = udRoundBox(
																				closestPointOnRay2.xyz-boxCenterPoint,
																				boxDim,
																				newBoxPower,
																				pdCornerDis.y
																		);
																		
																		
																		didHit = didHit || (
																				(seBoxDis1.x <= 0.0) ||
																				(seBoxDis2.x <= 0.0)
																		);
																		
																		if (didHit) {
																				break;
																		}
																		
																}
														}
												}
												
												
												
												
												if (didHit) {
														
														
														seBoxDis2 = udRoundBox(
																(cameraPos)-boxCenterPoint,
																boxDim,
																newBoxPower,
																pdCornerDis.y
														);
														isInside = (seBoxDis2.y > seBoxDis2.x);
														
														if (isInside) {                                
																hitBox.x = 0.0;
														}
														
														hitBoxFinal.x = min(hitBoxFinal.x,hitBox.x);
														hitBoxFinal.y = max(hitBoxFinal.y,hitBox.y);
														
														primIdList[primIdListLength] = primDataInd;
														primIdListLength++;
												}
												
												
												didHitFinal = didHitFinal || didHit;
												
												
												
										}
								}
								
								if (primIdListLength == MAX_PRIM_IDS) {
										break;
								}
								
								
						}
						
						if (primIdListLength == MAX_PRIM_IDS) {
								break;
						}
				}
				
				p = ro + rd*cellBox.y;
				
				if (distance(p,ro) > maxDis) {
						break;
				}
		}
		
	
	return vec3(hitBoxFinal.xy, float(didHitFinal));
		
}

float postLineStep(
	float accuracy,
	vec3 ro,
	vec3 rd,
	vec3 hitBoxFinal
) {
	
	bool didHitFinal = (hitBoxFinal.z > 0.0);
	
	float camDis;
	float curLOD;
	float btSteps = 0.0;
	vec2 tval = vec2(0.0);
	
	if (didHitFinal) {
			
		// if (primIdList[0] == -1) {
		//   tempInt = primIdList[primIdListLength-1];
		//   primIdList[primIdListLength-1] = -1;
		//   primIdList[0] = tempInt;
		// }
		
		camDis = distance(cameraPos,ro+rd*hitBoxFinal.x);
		
		// 1.0 = close, 0.0 = far away
		curLOD = mix(
				1.0,
				0.0,
				clamp(camDis/(clipDist.y*0.5),0.0,1.0) //MAX_DETAIL_DIS
		)*accuracy;
		
		btSteps = mix(MIN_STEPS,MAX_STEPS,curLOD);
		
		tval = castSolid(
				ro,
				rd,
				hitBoxFinal.xy,
				btSteps
		);
		
		if (
				(tval.x < hitBoxFinal.y)
				&& (tval.x > 0.01)
				//&& (tval.y < 0.0)
		) {
					
				return tval.x;
		}
	}
	
	return MAX_CAM_VOL_DIS;
	
}


vec3 normSolid( vec3 pos )
{
		globPrimaryRay = false;
	
		vec3 eps = vec3( 0.0, 0.0, 0.0 );
		eps.x = 0.025;
		
		vec3 nor = vec3(
				mapSolid(pos+eps.xyy) - mapSolid(pos-eps.xyy),
				mapSolid(pos+eps.yxy) - mapSolid(pos-eps.yxy),
				mapSolid(pos+eps.yyx) - mapSolid(pos-eps.yyx) );
		
		globPrimaryRay = true;
		return normalize(nor);
}

/*
for( int i=0; i<numSteps; i++ )
{
		h = 
		min(
			mapLand( ro + rd*t ).x*0.5
			,mapDyn(ro + rd*t).x*2.0
		)
		;
		res = min( res, 2.0*h/t );
		t += clamp( h, 0.0, 0.5 );
		if( h<0.001 || t>tmax ) break;
}
*/

float softShadowPrim( vec3 ro, vec3 rd, float tmin, float tmax, int numSteps )
{
		globPrimaryRay = false;
		float res = 1.0;
		float t = tmin;
		float h;
		for( int i=0; i<numSteps; i++ )
		{
				h = mapSolid( ro + rd*t )*4.0;
				res = min( res, 2.0*h/t );
				t += clamp( h, 0.0, 0.5 );
				if( h<0.001 || t>tmax ) break;
		}
		globPrimaryRay = true;
		return clamp( res, 0.0, 1.0 );

}

float hardShadowPrim(
		vec3 ro,
		vec3 rd,
		vec2 minMaxT,
		float fNumSteps
) {
		globPrimaryRay = false;
		int p = 0;
		int numSteps;
		
		float res = (0.0);
		float t;
		float fp;
		
		vec3 pos;
		
		float SHAD_PREC = 0.1;
		
		t = minMaxT.x;
		numSteps = int(fNumSteps);
		for( p = 0; p < numSteps; p++ ) {
				
				fp = float(p)/fNumSteps;
				
				pos = ro+rd*t;
				
				res = mapSolid( pos );
				
				if (res < SHAD_PREC ) {
						break;
				}
				
				t += res;
		}
		
		globPrimaryRay = true;
		
		return t;
		
}

// float hardShadowPrim( vec3 ro, vec3 rd, float tmin, float tmax, int numSteps )
// {
// 		float res = 0.0;
// 		float t = tmin;
		
		
// 		float fNumSteps = float(numSteps);
// 		float fi;
// 		float h;
		
		
// 		for( int i=0; i<numSteps; i++ ) {
				
// 				fi = float(i)/fNumSteps;
				
// 				h = mapSolid( ro + rd*t );
				
// 				res += -clamp(h-1.0,-1.0,0.01);
// 				t = mix(tmin,tmax,fi);
								
// 		}
		
// 		return 1.0-clamp( res, 0.0, 1.0 );
		
// }


#endif
// end of DOPRIM

#ifdef DOTER


////////////////


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

float getTree(vec3 pos, vec3 tp0, vec3 tp1, vec3 tp2, vec2 thickVals) {
		// p0: start point
		// p1: end point
		// p2: control point
		//      p2
		// p0        p1
		
		
		vec2 dres = psDistanceV2(pos, tp0, tp1);
		float t =  dres.y;
		vec3 bezTanP0 = mix(tp0, tp2, t);
		vec3  bezTanP1 = mix(tp2, tp1, t);
		vec2 resArr = psDistanceV2(pos, bezTanP0, bezTanP1);

		float curThickness = mix(thickVals.x, thickVals.y, t);

		return resArr.x - curThickness;
}


void preLimb(vec3 ro, vec3 rd) {
	
	int i;
	int j;
	
	primIdListLength = 0;
	
	for (i = 0; i < MAX_PRIM_IDS; i++) {
			primIdList[i] = -1;
			primAlreadyTested[i] = -1.0;
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
	
	
	
	
	for (i = 0; i < actorCount; i++) {
		header0 = texelFetch(Texture1, primDataInd); primDataInd++;
		header1 = texelFetch(Texture1, primDataInd); primDataInd++;
		header2 = texelFetch(Texture1, primDataInd); primDataInd++;
		
		
		endInd = int(header0.x);
		
		// if (endInd <= 0) {
		// 	break;
		// }
		
		hitBox = aabbIntersect(ro,rd,header1.xyz,header2.xyz);
		
		if (hitBox.x <= hitBox.y) {
			
			
			while (primDataInd < endInd) {
				
				firstInd = primDataInd;
				
				datVec = texelFetch(Texture1, primDataInd); primDataInd++;
				cenVec = texelFetch(Texture1, primDataInd); primDataInd++;
				tanVec = texelFetch(Texture1, primDataInd); primDataInd++;
				bitVec = texelFetch(Texture1, primDataInd); primDataInd++;
				norVec = texelFetch(Texture1, primDataInd); primDataInd++;
				ln0Vec = texelFetch(Texture1, primDataInd); primDataInd++;
				ln1Vec = texelFetch(Texture1, primDataInd); primDataInd++;
				
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
			
		}
		else {
			
			
			if (hitBox.x <= hitBox.y) {
				
			}
			
			primDataInd = endInd;
		}
		
		// if (endInd <= 0) {
		// 	return;
		// }
		
		
		
	}
	
	
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
		
		datVec = texelFetch(Texture1, primDataInd); primDataInd++;
		cenVec = texelFetch(Texture1, primDataInd); primDataInd++;
		tanVec = texelFetch(Texture1, primDataInd); primDataInd++;
		bitVec = texelFetch(Texture1, primDataInd); primDataInd++;
		norVec = texelFetch(Texture1, primDataInd); primDataInd++;
		ln0Vec = texelFetch(Texture1, primDataInd); primDataInd++;
		ln1Vec = texelFetch(Texture1, primDataInd); primDataInd++;
		
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




//#############################


//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
		 return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
	return vec4(1.79284291400159 - 0.85373472095314 * r);
}

float snoise(vec3 v) { 
	const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
	const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

	// First corner
	vec3 i  = floor(v + dot(v, C.yyy) );
	vec3 x0 =   v - i + dot(i, C.xxx) ;

	// Other corners
	vec3 g = step(x0.yzx, x0.xyz);
	vec3 l = 1.0 - g;
	vec3 i1 = min( g.xyz, l.zxy );
	vec3 i2 = max( g.xyz, l.zxy );

	//   x0 = x0 - 0.0 + 0.0 * C.xxx;
	//   x1 = x0 - i1  + 1.0 * C.xxx;
	//   x2 = x0 - i2  + 2.0 * C.xxx;
	//   x3 = x0 - 1.0 + 3.0 * C.xxx;
	vec3 x1 = x0 - i1 + C.xxx;
	vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
	vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

	// Permutations
	i = mod289(i); 
	vec4 p = permute( permute( permute( 
						 i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
					 + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
					 + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

	// Gradients: 7x7 points over a square, mapped onto an octahedron.
	// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
	float n_ = 0.142857142857; // 1.0/7.0
	vec3  ns = n_ * D.wyz - D.xzx;

	vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

	vec4 x_ = floor(j * ns.z);
	vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

	vec4 x = x_ *ns.x + ns.yyyy;
	vec4 y = y_ *ns.x + ns.yyyy;
	vec4 h = 1.0 - abs(x) - abs(y);

	vec4 b0 = vec4( x.xy, y.xy );
	vec4 b1 = vec4( x.zw, y.zw );

	//vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
	//vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
	vec4 s0 = floor(b0)*2.0 + 1.0;
	vec4 s1 = floor(b1)*2.0 + 1.0;
	vec4 sh = -step(h, vec4(0.0));

	vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
	vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

	vec3 p0 = vec3(a0.xy,h.x);
	vec3 p1 = vec3(a0.zw,h.y);
	vec3 p2 = vec3(a1.xy,h.z);
	vec3 p3 = vec3(a1.zw,h.w);

	//Normalise gradients
	vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;

	// Mix final noise value
	vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
	m = m * m;
	return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
																dot(p2,x2), dot(p3,x3) ) );
}


//#############################

const float timeScale = 0.4;

const float WAVE_HEIGHT = 64.0;
const float WAVE_SPEED = 1.5;
const float WAVE_SCALE = 60.0;


float amplitude[8] = float[](
	0.25/256.0,
	0.5/256.0,
	1.0/256.0,
	4.0/256.0,
	
	16.0/256.0,
	32.0/256.0,
	64.0/256.0,
	128.0/256.0
		
);
float wavelength[8] = float[](
	WAVE_SCALE/48.0,
	WAVE_SCALE/40.0,
	WAVE_SCALE/32.0,
	WAVE_SCALE/16.0,
	
	WAVE_SCALE/8.0,
	WAVE_SCALE/4.0,
	WAVE_SCALE/2.0,
	WAVE_SCALE/1.0
	
);

^INCLUDE:WaveFuncs^



////////////////






float getLand3D(vec3 pos, float sampVal) {
		
		
		float maxDis = mix(0.5,1.0,distance(cameraPos,pos)/MAX_CAM_VOL_DIS);
		
		return mix(
				maxDis,
				-maxDis,
				(sampVal) //samp.g //+sin(curTime)*0.1
		);
}



float getWater3D(vec3 pos) {
		
		vec4 samp = getTexCubic(Texture0,pos-volMinReadyInPixels,volSizePrim);
		//float hasWater = 1.0*float(samp.g > 0.0);
		
		float waterVal = mix(
				samp.b, // last value
				samp.g, // new value
				waterLerp
		);
		
		float maxDis = mix(0.5,1.0,distance(cameraPos,pos)/MAX_CAM_VOL_DIS);
		
		return isUnderWater*mix(
				maxDis,
				-maxDis,
				(waterVal)
		);
}






vec4 cell2D(
		in vec3 basePos,
		in vec3 cellSize,
		in float dv,
		in float roundness,
		inout vec3 gradVal,
		inout vec3 norVal
) {
		
		vec3 pos = basePos/cellSize;
		
		vec3 ipos = floor(pos);
		int i;
		
		int iy = int(ipos.y);
		float evenOdd = float(mod(iy,2) == 0);
		
		vec3 o = vec3(0.0,0.5,1.0);
		
		vec4 p0, p1, p2;
		
		p0.xyz = mix(ipos + o.yxx, ipos + o.xxx, evenOdd);
		p1.xyz = mix(ipos + o.xzx, ipos + o.zxx, evenOdd);
		p2.xyz = mix(ipos + o.zzx, ipos + o.yzx, evenOdd);
		
		p0.w = distance(pos,p0.xyz);
		p1.w = distance(pos,p1.xyz);
		p2.w = distance(pos,p2.xyz);
		
		vec4 centerPos = p0;
		
		centerPos = mix(centerPos, p1, float(p1.w < centerPos.w));
		centerPos = mix(centerPos, p2, float(p2.w < centerPos.w));
		
		vec3 MAX_MOD = vec3(0.25);// *(sin(curTime/2.0));
		
		vec4 bestPoint = centerPos;
		vec4 nextBestPoint = centerPos;
		
		bestPoint.xyz += randPN(bestPoint.xyz)*MAX_MOD;
		
		
		
		bestPoint.w = distance(pos,bestPoint.xyz);
		nextBestPoint.w = INVALID;
		
		vec4 testPoint;
		
		for (i = 0; i < NUM_VORO; i++) {
				testPoint.xyz = centerPos.xyz + voroOffsets[i];
				testPoint.xyz += randPN(testPoint.xyz)*MAX_MOD;
				
				testPoint.w = distance(pos,testPoint.xyz);
				
				if (testPoint.w < bestPoint.w) {
						nextBestPoint = bestPoint;
						bestPoint = testPoint;
				}
				else {
						nextBestPoint = mix(nextBestPoint,testPoint,float(testPoint.w<nextBestPoint.w));
				}
		}
		
		
		
		vec4 closestPoint = pointSegDistance(pos,bestPoint.xyz,nextBestPoint.xyz);
		
		float planeDistance = distance(closestPoint.xyz,(bestPoint.xyz+nextBestPoint.xyz)*0.5);
		float disVal = 
				//distance(closestPoint.xyz,bestPoint.xyz) -
				
				//distance(bestPoint.xyz,basePos)-cellSize*0.5;
				
				
				mix(
						dv-planeDistance,
						distance(bestPoint.xyz,basePos)-cellSize.x*0.5,
						roundness
				);
		
		gradVal.x = clamp((1.0 - (bestPoint.w * 2.0 / (bestPoint.w + nextBestPoint.w))), 0.0, 1.0);
		gradVal.y = clamp((      (bestPoint.w * 2.0 / (bestPoint.w + nextBestPoint.w))), 0.0, 1.0);; //bestPoint.w-
		gradVal.z = planeDistance;
		
		norVal = normalize(bestPoint.xyz-nextBestPoint.xyz);
		
		return vec4(bestPoint.xyz*cellSize,disVal);
		
}


// vec2 getGrass(
//     vec2 pRes,
//     vec3 terNorm,
//     vec3 pos    
// ) {
		
//     int i;
		
//     vec2 res = pRes;
		
//     float earthX = res.x;
//     float grassRes = 0.0;
//     float grassSize = 1.0;
		
		
		
//     float isGrass = 
//     (
//         (1.0-abs(cos((upTex)*3.0)))*2.0 + 
//         (1.0-gradVal.x*(1.0-seedRand))
//     )*smoothstep(0.7,1.0,terNorm.z)*seedRand*upTex*2.0
//     /3.0;
		
//     if (isGrass > 0.25) {
				
//         for (i = 0; i < 2; i++) {
//             fi = float(i);
//             repPos = opRep(
//                 pos
//                 + fi*grassSize*0.5
//                 + sin(curTime*2.0+pos.x+pos.y+pos.z + fi*5.3) //  + upTex*10.0
//                 * 0.125 * (1.0-clamp(0.7-primRes,0.0,1.0)) //* grassNorm
//                 ,vec3(grassSize, grassSize, 2.0)
//             );
						
//             //repPos.z += ( earthX ) + sin(curTime);//mix(pos.z,pos.z+earthX,0.5);
						
//             //repPos.z *= 0.5;
						
//             lerpVal = pow(abs(sin(pos.x+pos.y+pos.z)),6.0);
						
//             grassRes = sdSphere(
//                     vec3(repPos.xy,earthX)
//                         // *vec3(
//                         // mix(0.5,1.0,lerpVal),
//                         // mix(0.5,1.0,1.0-lerpVal)
//                     //     ,
//                     //     0.5*grassSize + (1.0-isGrass)*0.5
//                     // )
//                     -vec3(
//                         0.0,
//                         0.0,
//                         (1.0-isGrass)*0.25//(1.0-isGrass) + float(isGrass < 0.5)*1000.0
//                     )
//                     , 
//                     // *vec3(1.0,1.0,0.2*grassSize),
//                     // + vec3(0.0,0.0,(earthX+0.25)*2.0))*vec3(1.0,1.0,0.2*grassSize
//                     (grassSize)*0.125 // *(1.0-pow(upTex+0.2,4.0)) //grassNorm*
//                 );
						
//             res.x = min(
//                 res.x,
								
								
//                 grassRes
								
//                 // sdSphere(
//                 //     repPos, 
//                 //     mix(
//                 //         grassSize*0.25,
//                 //         0.0,
//                 //         1.3 - clamp( abs(primRes*5.0*(1.0-powVal)),0.0,1.0)    
//                 //     ) *
//                 //     pow(clamp(distance(primRes,-0.4)*4.0,0.0,1.0),4.0)
//                 // )
//                 //sdBox( repPos, vec3(grassSize*0.125,grassSize*0.125, 0.0325) )
//             );
//         }
				
				
				
//         // if (res.x >= earthX + texVal*0.1) {
//         //     res.x = earthX + texVal*0.1;
//         //     globTexEarth.x = TEX_EARTH;
//         // }
//         // else {
						
//             if ((grassRes < earthX)) { //
//                 globTexEarth.x = 
//                     TEX_GRASS;
//                     //TEX_STONE;
										
//                     globTexEarth.y = abs(sin(seedRand*4.0+earthX*2.0));
//                     //globTexEarth.y = clamp(abs(primRes)*4.0,0.0,1.0);
//             }
						
						
//         //}
				
				
				
//     }
		
		
//     return res;
// }






//----------------------------------------------------------------------





^INCLUDE:TerHeightFunc^



float mapWater2( vec3 pos ) {
		
		vec2 res = vec2(MAX_CAM_DIS,SKY_ID);
		
		vec3 wh = vec3(
				0.0,
				0.0,
				(waveHeight(pos.xy*1.0,1.0) + 1.0) * 0.5 * WAVE_HEIGHT
		);
		
		
		// float watDis = getSeaLevel(pos,wh.z,isUnderWater);
		// float camDis = distance(cameraPos,pos)/MAX_CAM_VOL_DIS;
		// float volLerp = pow(clamp(camDis,0.0,1.0),4.0);
		// // if (camDis < 1.0) {
		// //     watDis = mix(
		// //         getWater3D(pos + wh),
		// //         watDis,
		// //         volLerp
		// //     );
		// // }
		
		float watDis = getWater3D(pos + wh);
		
		
		
		res = opU(
				res,
				vec2(
						watDis,
						WATER_ID
				)
				
		);
		
		
		int i;
		float splashTot = 0.0;
		float splashDis = 0.0;
		float maxDis = 10.0;
		float waveMod = clamp(abs(res.x-1.0),0.0,1.0);
		for (i = 0; i < numSplashes; i++) {
				splashDis = distance(splashArr[i].xyz+vec3(0.0,0.0,1.0),pos.xyz);
				splashTot += (
						
						(sin(splashDis*2.0+splashArr[i].w*0.5-curTime*4.0)+1.0)*0.4
						
				) * 
				clamp(1.0-splashDis/maxDis, 0.0, 1.0);
		}
		res.x = opD(res.x,splashTot*waveMod);
		
		if ((isUnderWater < 0.0)) {
			res.x = opD(res.x,0.2);
		}
		
		
		res.x = opI(
			res.x,
			sdBox(
				pos-((volMinReadyInPixels + volMaxReadyInPixels)*0.5),
				vec3((volMaxReadyInPixels - volMinReadyInPixels)*0.5)
			)
		);
		
		// res.x = opS(
		// 	res.x,
		// 	sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) )
		// );
		
		return res.x;

}


float mapWater( vec3 pos ) {
		
		vec2 res = vec2(MAX_CAM_DIS,SKY_ID);
		
		float camDis = clamp(distance(cameraPos,pos)*32.0/MAX_CLIP,0.0,1.0);
		
		float myWH = waveHeight(
						pos.xy*0.25,
						camDis
						// * mix(1.0,0.0001,camDis)
						// * 0.1
					);
		
		vec3 wh = vec3(
				0.0,
				0.0,
				(
					myWH + 1.0
				) * 0.5 * WAVE_HEIGHT // * (1.0-camDis)
		);
		
		
		// float watDis = getSeaLevel(pos,wh.z,isUnderWater);
		// float camDis = distance(cameraPos,pos)/MAX_CAM_VOL_DIS;
		// float volLerp = pow(clamp(camDis,0.0,1.0),4.0);
		// // if (camDis < 1.0) {
		// //     watDis = mix(
		// //         getWater3D(pos + wh),
		// //         watDis,
		// //         volLerp
		// //     );
		// // }
		
		
		float watDis = getSeaLevel(pos,wh.z,isUnderWater);
		//float watDis = getWater3D(pos + wh);
		
		
		
		res = opU(
				res,
				vec2(
						watDis,
						WATER_ID
				)
				
		);
		
		
		int i;
		float splashTot = 0.0;
		float splashDis = 0.0;
		float maxDis = 10.0;
		float waveMod = clamp(abs(res.x-1.0),0.0,1.0);
		for (i = 0; i < numSplashes; i++) {
				splashDis = distance(splashArr[i].xyz+vec3(0.0,0.0,1.0),pos.xyz);
				splashTot += (
						
						(sin(splashDis*2.0+splashArr[i].w*0.5-curTime*4.0)+1.0)*0.4
						
				) * 
				clamp(1.0-splashDis/maxDis, 0.0, 1.0);
		}
		res.x = opD(res.x,splashTot*waveMod);
		
		if ((isUnderWater < 0.0)) {
			res.x = opD(res.x,0.2);
		}
		
		// if (
		// 		all(greaterThanEqual(pos.xyz,volMinReadyInPixels)) &&
		// 		all(lessThanEqual(pos.xyz,volMaxReadyInPixels))
		// ) {
		// 		res.x = min(res.x, mapWater2(pos));
		// }
		
		res.x = opS(
			res.x,
			sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) )
		);
		
		return res.x;

}












// ################


// /*--------------------------------------------------------------------------------------
// License CC0 - http://creativecommons.org/publicdomain/zero/1.0/
// To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
// ----------------------------------------------------------------------------------------
// ^This means do anything you want with this code. Because we are programmers, not lawyers.

// -Otavio Good
// */

// float smax(float a, float b, float k)
// {
//     float h = clamp( 0.5+0.5*((-b)+a)/k, 0.0, 1.0 );
//     return -(mix( -b, -a, h ) - k*h*(1.0-h));
// }

// // IQ's style of super fast texture noise
// float noiseTex(in vec3 x)
// {
//     vec3 fl = floor(x);
//     vec3 fr = fract(x);
//     fr = fr * fr * (3.0 - 2.0 * fr);
//     vec2 uv = (fl.xy + vec2(37.0, 17.0) * fl.z) + fr.xy;
//     vec2 rg = texture2D(Texture14, (uv + 0.5) * 0.00390625, -100.0 ).xy;
//     return mix(rg.y, rg.x, fr.z);
// }


// float getNoiseVal(vec3 p) {
//     float n = noiseTex(p*2.0+curTime*0.6);
//     n += noiseTex(p*4.0+curTime*0.7)*0.5;
//     n += noiseTex(p*8.0)*0.25;
//     n += noiseTex(p*16.0)*0.125;
//     n += noiseTex(p*32.0)*0.0625;
//     //n += noiseTex(p*64.0)*0.0625*0.5;
//     //n += noiseTex(p*128.0)*0.0625*0.25;
		
//     return n;
// }

// // Calculate the distance field that defines the object.
// float mapClouds(vec3 pos)
// {
//     // first distort the y with some noise so it doesn't look repetitive.
//     //p.xyz = RotateY(p, length(p.xz) + curTime);
//     //p.y += 0.1;
//     //p.xyz = RotateZ(p, length(p.z) + curTime);
		
//     vec3 p = pos;
		
//     p.z += noiseTex(p*0.5)*0.25;
//     // multiple frequencies of noise, with time added for animation
		
//     float n = getNoiseVal(p*vec3(1.0,1.0,16.0));
		
//     // subtract off distance for cloud thickness
//     float dist = n*0.25 - (0.275);// - abs(p.y*0.02)/* - curTime*0.01*/);
//     //dist = smax(dist, -(length(p-cameraPos) - 20.0), 0.1);    // nice near fade
//     // smooth blend subtract repeated layers
//     dist = smax(dist, -(abs(fract(p.z*4.0)-0.5) - 0.15), 0.4);
		
//     return dist;
		
//     //vec2 distMat = vec2(dist, 0.0);
//     // sun in the distance
//     //distMat = matMin(distMat, vec2(length(p-camLookat - vec3(0.0, 0.5, -1.0)) - 0.6, 6.0));
//     //return distMat;
// }

// ################


// #define DOCLOUDS

// #ifdef DOCLOUDS

// vec4 cloudNoise(vec3 pos) {
	
// 	return getTexLin(Texture13, pos*1.0, voroSize);
	
// }

// float mapClouds(vec3 pos) {
		
// 		float newTime = curTime * 20.0;
		
// 		float distOrig = (cameraPos.z+2048.0-distance(cameraPos,pos)/10.0)-pos.z; //getSeaLevel(pos,0.0,isUnderWater)-2048.0f; //
		
// 		float dist = distOrig - 100.0;//(cameraPos.z+20.0)-pos.z;
		
// 		distOrig = clamp(distOrig/400.0,0.0,1.0);
		
// 		vec3 newPos = pos;
// 		newPos.z -= cameraPos.z;
// 		newPos.z *= 2.0;
		
		
		
// 		dist = opI(dist,cloudNoise(pos/128.0 + newTime*0.125).g*60.0+distOrig*100.0);
		
		
// 		dist -= cloudNoise(newPos/64.0 + newTime*0.25).g*80.0;
// 		dist -= cloudNoise(newPos/32.0 + newTime*0.5).g*40.0;
// 		dist -= cloudNoise(newPos/16.0 + newTime*1.0).g*20.0;
		
// 		return dist*0.5;
// }
// #endif

^INCLUDE:MapLand^


vec3 mapDyn(vec3 pos) {
		
		int i;
		float multVal = 1.5;
		
		vec2 res = vec2(MAX_CAM_DIS,SKY_ID);
		vec2 expRes = vec2(MAX_CAM_DIS*2.0,TEX_EXPLOSION);
		vec3 expVec = vec3(0.0);
		
		for (i = 0; i < numExplodes; i++) {
				expRes.x = min(
						expRes.x,
						sdSphere(
								pos-explodeArr[i].xyz,
								explodeArr[i].w + pow(sin(pos.x*multVal)*sin(pos.y*multVal)*sin(pos.z*multVal),2.0)
						)
				);
				expVec += normalize(pos-explodeArr[i].xyz);
				
		}
		
		
		// vec3 sunPos = cameraPos + vec3(30.0);
		// expRes.x = min(
		//     expRes,
		//     sdSphere(
		//         pos-sunPos,
		//         10.0
		//     )
		// );
		// expVec += normalize(pos-sunPos);
		
		res = opU(res,expRes);
		
		
		
		globExplodeVec = expVec;
		
		
		
		// vec3 focalCenter = cameraPos+lookAtVec*30.0;
		
		// float sphereRad = 4.0;
		// vec3 sphereCenter = vec3(0.0);//cameraPos+lookAtVec*sphereRad*3.0;
		// float ct = curTime*4.0;
		
		// vec3 newPos = 
		//     //opRep(normalize(pos-focalCenter)*20.0,vec3(10.0));
		//     opRep(pos,vec3(20.0));
		
		// //sphereCenter += offset;
		
		// vec3 tp0 = sphereCenter + vec3(sin(ct*0.23),sin(ct*0.3),sin(ct*0.11))*2.0;
		// vec3 tp1 = sphereCenter + vec3(sin(ct*0.34),sin(ct*0.13),sin(ct*0.21))*2.0;
		// vec3 tp2 = sphereCenter + vec3(sin(ct*0.17),sin(ct*0.2),sin(ct*0.36))*2.0;
		

		
		
		
		// res = opU(res, vec2(
		//     //getTree(newPos, tp0, tp1, tp2, vec2(2.0,1.0)),
				
		//     sdSphere(newPos-sphereCenter,sphereRad),
				
		//     TEX_EARTH
		// ));
		
		// float camDis = distance(cameraPos,pos)/MAX_CLIP;
		// vec3 myTV = getTerVal(pos, camDis, false);
		// if (myTV.x < 2.0) {
			
		// 	vec3 cenPoint2 = vec3(0.5);
		// 	vec3 repPos2 = opRep(pos,cenPoint2);
		// 	res = opU(
		// 		res,
		// 		vec2(
		// 			sdSphere(vec3(repPos2.xy,0.0),0.25*clamp(0.25-myTV.x,0.0,1.0)),
		// 			TEX_GRASS	
		// 		)
		// 	);
			
		// }
		
		
		res = opU(res, vec2(
		    postLimb(pos),
		    TEX_EARTH
		));
				
		float oldX = res.x;
		
		res.x = opS(
			res.x,
			sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) ) //8.0 //CAM_BOX_SIZE
		);
		
		
		
		// res.x = opS(
		// 	res.x,
		// 	sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) ) //8.0 //CAM_BOX_SIZE
		// );
		
		
		// #ifdef DOCLOUDS
		// res = opU(res,vec2(mapClouds(pos), TEX_SKY));
		
		// float randVal = mod(
		// 						abs(
		// 								sin(pos.z/32.0)*
		// 								sin(pos.x/32.0)*
		// 								sin(pos.y/32.0)
		// 						),
		// 						1.0
		// 				);
		// float myVal;
		// float pScale = 0.01;
		// if (res.y == TEX_SKY) {
		// 		myVal = clamp(
		// 				pow( (1.0-getTexCubic(Texture13, (pos*1.0 + randVal*16.0)*pScale, voroSize).r), 4.0 )
						
		// 				,0.0,1.0);

		// 		res.x = opD(
		// 				res.x,
		// 				myVal*10.0
		// 		);
				
		// 		// myVal = clamp(
		// 		// 		pow( (1.0-getTexCubic(Texture13, (pos*4.0 + randVal*16.0)*pScale, voroSize).r), 4.0 )
		// 		// 		*0.4
		// 		// 		,0.0,1.0);

		// 		// res.x = opD(
		// 		// 		res.x,
		// 		// 		myVal*20.0
		// 		// );
				
		// }
		// #endif
		
		
		return vec3(res,oldX);
}


float newNoise(vec3 pos) {
	return (getTexCubic(Texture13, pos, voroSize).g-0.5)*2.0;
}

// vec2 mapLand( vec3 pos ) {
    
//     float myRes = 0.0;
    
//     // landDis += ;
    
//     vec3 cellSize = vec3(8.0);
    
//     vec3 gradVal2 = vec3(0.0);
//     vec3 gradVal = vec3(0.0);
//     vec3 norVal = vec3(0.0);
//     vec4 cellVal = cell2D(
//         pos,
//         cellSize,
//         0.1,
//         0.3,
//         gradVal,
//         norVal
//     );
    
//     float randVal = 1.0 -
//       clamp(
//         abs(
//           pow(
//             (
//               sin(pos.z/32.0)*
//               sin(pos.x/32.0)*
//               sin(pos.y/32.0)  
//             )*2.0 + 0.2,
//             4.0
//           )  
//         ),  
//         0.0,
//         1.0
//       );
    
    
//     vec3 cellSize2 = vec3(64.0,64.0,32.0);
//     vec4 cellVal2 = cell2D(
//         vec3(pos.xy+pos.z*0.5,pos.z),//+pos.z*0.5,pos.z),
//         cellSize2,
//         0.1,
//         0.3,
//         gradVal2,
//         norVal
//     );
    
//     // gradval.x is biggest near center
//     // gradval.y is biggest near edge
    
//     //float lerpVal = pow(gradVal2.y,4.0);
//     // vec3 mapPos = vec3(
//     //   cellVal2.xy,
//     //   pos.z
//     // );//pos;//mix(cellVal2.xyz,pos,pow(clamp(1.0-gradVal2.x,0.0,1.0),4.0));//pos;
    
    
//     vec3 offsetPos = pos + vec3(0.0,0.0,0.25);
    
//     float heightRes = getTerHeight(Texture2,pos);//mix(getTerHeight(pos),getTerHeight(mapPos),lerpVal);
    
//     float heightMod = clamp(-heightRes/64.0,0.0,1.0);
    
//     float noiseVal = (newNoise(pos*64.0/16.0)*4.0 + newNoise(pos*64.0/32.0)*8.0 + newNoise(pos*64.0/64.0)*32.0)/48.0;
//     float baseRes = heightRes + noiseVal*256.0*heightMod;// *(1.0-lerpVal);
    
//     float heightMod2 = clamp((-baseRes)/4.0,0.0,1.0);
    
//     float tempVal = (gradVal.x)*64.0*float(baseRes < 2.0)*randVal;
    
//     float baseRes3 = (heightRes+8.0) - (gradVal2.x)*32.0*float((pos.z-cellSize2.z*0.0) < cellVal2.z);
//     float tempVal2 = (gradVal.x)*32.0*float(baseRes3 < 2.0)*randVal;
//     float heightMod3 = clamp((-baseRes3)/8.0,0.0,1.0);
    
//     globTexEarth.x = TEX_EARTH;
    
//     myRes = 
// 	    mix(
// 	    baseRes3 - tempVal2*heightMod3 - tempVal*heightMod3
// 	    ,(baseRes+8.0) - tempVal*heightMod2
// 	    ,clamp( (sin(newNoise(pos*64.0/43.0)*4.0)+1.0)*0.5,0.0,1.0)
// 	    )
//     ;
    
//     return vec2(myRes/32.0,TEX_EARTH);
// }


// vec3 opRep( vec3 p, vec3 c )
// {
// 		return mod(p,c)-0.5*c;
// }

float getGV(vec3 pos, float _res, float camDis) {
	float res = _res;
	// float grassMod = 4.0;
	
	// vec3 newPos = pos;//opRep(pos*grassMod, vec3(1.0));
	// newPos.z = 0.0;
	
	// // vec3 newPos2 = floor(newPos.xyz*(grassMod))/(grassMod);
	// // newPos2.z = 0.0;
	
	// //float myVal2 = randf3( newPos2 );
	
	// vec3 newPos2 = floor(newPos.xyz*(grassMod))/(grassMod);
	// newPos2.z = 0.0;
	
	// float myVal2 = randf3( newPos2 );
	
	
	
	// float lenMod = clamp(
	// 	((newPos-newPos2)*grassMod - 0.5)*2.0,
	// 	0.0,
	// 	1.0
	// );
	
	float lenMod = 0.0;
	camDis = clamp(1.0-camDis*256.0,0.0,1.0);
	
	if (camDis > 0.0) {
		lenMod = 0.0;//texture(Texture15, pos.xy*(1.0/8.0)).r*camDis;
	}
	
	
	
	
	res = opD(res,lenMod);
	
	return res;
}

vec2 mapLand(vec3 pos) {
	
	vec2 res = vec2(MAX_CAM_DIS, TEX_EARTH);
	
	float camDis = distance(cameraPos,pos)/MAX_CLIP;
	vec3 myTV = getTerVal(pos, camDis, false);
	
	res.x = myTV.x;
	
	// float baseRes = getTexLin(Texture0, pos-volMinReadyInPixels, volSizePrim).r;
	// float lv1 = mix(1.0,-1.0,baseRes);
	// res.x = opS(res.x, -lv1);
	
	
	//res.x = getGV(pos, res.x, camDis);
	
	res.x = remBox(pos,res.x, 4.0);
	
	
	
	
	return res;
	
}



vec4 mapLandMicro(vec3 pos) { //, vec3 terNorm
	
	vec2 res = vec2(MAX_CAM_DIS, TEX_EARTH);
	
	float camDis = distance(cameraPos,pos)/MAX_CLIP;
	vec3 myTV = getTerVal(pos, camDis, false);
	
	res.x = myTV.x;
	
	float camDis3 = clamp(1.0-camDis*128.0,0.0,1.0);
	float texVal = 0.0;
	float texSpacing = 10.0;//0.01*cellsPerWorld;
	//vec3 absTerNorm = abs(terNorm);
	
	vec2 mp = vec2(1.0/mapPitch);
	vec2 mp2 = vec2(mapPitch);
	
	// if (camDis3 > 0.0) {
		
	// 	//bilin(in sampler2D t, in vec2 uvIn, in vec2 textureSize, in vec2 texelSize)
		
	// 	texVal = 
	// 			bilin(Texture2, pos.yz/texSpacing, mp2, mp).r*absTerNorm.x*absTerNorm.x +
	// 			bilin(Texture2, pos.zx/texSpacing, mp2, mp).r*absTerNorm.y*absTerNorm.y +
	// 			bilin(Texture2, pos.xy/texSpacing, mp2, mp).r*absTerNorm.z*absTerNorm.z;
		
	// 	// texVal = 
	// 	// 		texture(Texture2, pos.yz/texSpacing).r*absTerNorm.x*absTerNorm.x +
	// 	// 		texture(Texture2, pos.zx/texSpacing).r*absTerNorm.y*absTerNorm.y +
	// 	// 		texture(Texture2, pos.xy/texSpacing).r*absTerNorm.z*absTerNorm.z;
		
	// 	texVal = (sin(texVal*M_PI*2.0)+1.0)*0.5*camDis3;
		
		
	// 	globTexTap += 3.0;
		
	// 	//float texVal = bilin(Texture2, (pos.xy+pos.zy+pos.xz)/32.0, vec2(mapPitch), vec2(1.0/mapPitch)).r*camDis3;
	// 	res.x = opD(
	// 		res.x,
	// 		texVal*0.0625
	// 	);
	// 	myTV.y -= texVal*0.05;
	// }
	
	
	
	//res.x = getGV(pos, res.x, camDis);
	
	float oldRes = res.x;
	
	res.x = remBox(pos,res.x, 4.0);
	
	
	
	return vec4(res.x, oldRes, myTV.zy);
	
}

// vec2 mapLand2(vec3 pos) {
		
// 		int i;
		
// 		vec2 ov = vec2(0.0,2.0);
// 		vec2 res = vec2(MAX_CAM_DIS,SKY_ID);
// 		float baseRes = getTexCubic(Texture0,pos-volMinReadyInPixels, volSizePrim).r;
// 		float landDis = getLand3D(pos, baseRes);
// 		vec3 terNorm = normalize(
// 				vec3(baseRes) - 
// 				vec3(
					
// 						getTexLin(Texture0, (pos+ov.yxx)-volMinReadyInPixels, volSizePrim).r,
// 						getTexLin(Texture0, (pos+ov.xyx)-volMinReadyInPixels, volSizePrim).r,
// 						getTexLin(Texture0, (pos+ov.xxy)-volMinReadyInPixels, volSizePrim).r
					
// 				)
// 		);
		
// 		res = opU(
// 				res,
// 				vec2(
// 						landDis,
// 						LAND_ID
// 				)
// 		);
		
		
		
// 		///////////
		
		
		
		
		
		
		
		
		
// 		// float splashTot = 0.0;
// 		// float splashDis = 0.0;
// 		// float maxDis = 10.0;
// 		// float waveMod = clamp(abs(res.x-1.0),0.0,1.0);
// 		// for (i = 0; i < numSplashes; i++) {
// 		//     splashDis = distance(splashArr[i].xyz+vec3(0.0,0.0,1.0),pos.xyz);
// 		//     splashTot += (
						
// 		//         (sin(splashDis*2.0+splashArr[i].w*0.5-curTime*4.0)+1.0)*0.4
						
// 		//     ) * 
// 		//     clamp(1.0-splashDis/maxDis, 0.0, 1.0);
// 		// }
// 		// res.x = opD(res.x,splashTot*waveMod*1.5);
		
		
// 		float splashTot = 0.0;
// 		float splashDis = 0.0;
// 		float maxDis = 10.0;
// 		float waveMod = clamp(abs(res.x-1.0),0.0,1.0);
// 		for (i = 0; i < numExplodes; i++) {
// 				splashDis = distance(explodeArr[i].xyz+vec3(0.0,0.0,1.0),pos.xyz);
// 				splashTot += (
						
// 						(sin(splashDis*2.0+explodeArr[i].w*0.5-curTime*4.0)+1.0)*explodeArr[i].w*0.5
						
// 				) * 
// 				clamp(1.0-splashDis/maxDis, 0.0, 1.0);
// 		}
// 		res.x = opD(res.x,splashTot*waveMod*1.5);
		
		
// 		if (res.x <= 0.1) {
// 				//globTexEarth.x = TEX_EARTH;
// 				res = getTerrain(res, terNorm, pos, 0.0);
				
				
				
// 		}
		
// 		res.x = opI(
// 			res.x,
// 			sdBox(
// 				pos-((volMinReadyInPixels + volMaxReadyInPixels)*0.5),
// 				vec3((volMaxReadyInPixels - volMinReadyInPixels)*0.5)
// 			)
// 		);
		
// 		res.x = opS(
// 			res.x,
// 			sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) ) //8.0 //CAM_BOX_SIZE
// 		);
		
		
// 		// vec4 nearestPoint = vec4(0.0);
		
// 		// if (thirdPerson == 1.0) {
// 		//     nearestPoint = pointSegDistance(pos, cameraPos, e );
				
				
				
// 		//     res.x = opS(
// 		//         res.x,
// 		//         sdSphere(
// 		//             pos-nearestPoint.xyz,
// 		//             mix(
// 		//                 5.0,
// 		//                 20.0,
// 		//                 clamp((pos.z-nearestPoint.z)/10.0,0.0,1.0)
// 		//             )
// 		//             *
// 		//             clamp(
// 		//                 distance(nearestPoint.xyz,entPos)/10.0,
// 		//                 0.25,
// 		//                 1.0
// 		//             )
// 		//         )
// 		//     );
						
						
// 		//     // res.x = opS(
// 		//     //     res.x,
// 		//     //     sdSphere(
// 		//     //         pos-cameraPos,
// 		//     //         mix(
// 		//     //             2.0,
// 		//     //             distance(cameraPos,entPos),
// 		//     //             clamp(distance(pos,entPos)/10.0,0.0,1.0)
// 		//     //         )
								
// 		//     //     )
// 		//     // );
				
// 		// }
		
		
// 		return res;
// }




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
		
		float stepCount;
		
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
		
		globCurSteps += float(p);
		
		stepCount = float(p);
		
		
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
				
				globCurSteps += float(p);
				
				stepCount += float(p);
				
				globTexDyn.x = res.y;
				
				if (globTexDyn.x == TEX_EXPLOSION) {
						globTexDyn.y = pow(abs(dot(lookAtVec,normalize(globExplodeVec))),2.0);
				}
				else {
						
						if (globTexDyn.x == TEX_SNOW) {
								globTexDyn.y = clamp(stepCount/100.0,0.0,1.0);
						}
						else {
								globTexDyn.y = 0.0;
						}
						
						
				}
				
				//p += TOT_DETAIL_STEPS;
		}
		else {
				globTexDyn.xy = vec2(0.0);
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


vec3 normWater( vec3 pos )
{
		globPrimaryRay = false;
		vec3 eps = vec3( 0.0, 0.0, 0.0 );
		eps.x = 0.1;
		
		vec3 nor = vec3(
				mapWater(pos+eps.xyy) - mapWater(pos-eps.xyy),
				mapWater(pos+eps.yxy) - mapWater(pos-eps.yxy),
				mapWater(pos+eps.yyx) - mapWater(pos-eps.yyx) );
		globPrimaryRay = true;
		return normalize(nor);
}


vec4 castWater(
		vec3 ro,
		vec3 rd,
		vec4 minT,
		float maxT,
		int numSteps
) {
		int p = 0;
		float fNumSteps = float(numSteps);
		
		float res = 0.0;
		float t;
		float fp;
		
		vec3 pos;
		
		float WAT_PREC = 0.3;
		float WAT_PREC2 = 0.002;
		
		float camDis = 0.0;
		
		t = minT.x;
		for( p = 0; p < numSteps; p++ ) {
				
				fp = float(p)/fNumSteps;
				
				pos = ro+rd*t;
				
				camDis = clamp(distance(cameraPos,pos)/MAX_CLIP,0.0,1.0);
				WAT_PREC = mix(-16.0,64.0, max(camDis,fp) ); //max(camDis,fp)
				WAT_PREC = max(WAT_PREC,0.2);
				
				res = mapWater( pos );
				if (
						(res < WAT_PREC ) ||
						(t>maxT)
				) {
						break;
				}
				
				t += res;
		}
		globCurSteps += float(p);
		
		
		
		
		
		
		if (res < WAT_PREC) {
				
				t -= WAT_PREC*1.25;
				for( p = 0; p < TOT_DETAIL_STEPS; p++ ) {
						
						pos = ro+rd*t;
						
						res = mapWater( pos );
						
						if (res < WAT_PREC2) {
								break;
						}
						
						t += res;
				}
				globCurSteps += float(p);
				
				globTexWater.x = TEX_WATER;
				//p += TOT_DETAIL_STEPS;
		}
		else {
				globTexWater.xy = vec2(0.0);
		}
		
		
		
		
		
		
		
		
		return vec4(normWater(pos),t);
		
}



vec3 normLandMicro( vec3 pos, vec3 terNorm, float camDis )
{
		globPrimaryRay = false;
		vec3 eps = vec3( 0.0, 0.0, 0.0 );
		eps.x = mix(0.2,20.0,camDis);
		
		vec3 nor = vec3(
				mapLandMicro(pos+eps.xyy).x - mapLandMicro(pos-eps.xyy).x,
				mapLandMicro(pos+eps.yxy).x - mapLandMicro(pos-eps.yxy).x,
				mapLandMicro(pos+eps.yyx).x - mapLandMicro(pos-eps.yyx).x );
		globPrimaryRay = true;
		return normalize(nor);
}

vec3 normLand( vec3 pos )
{
		globPrimaryRay = false;
		vec3 eps = vec3( 0.0, 0.0, 0.0 );
		eps.x = 0.1;
		
		vec3 nor = vec3(
				mapLand(pos+eps.xyy).x - mapLand(pos-eps.xyy).x,
				mapLand(pos+eps.yxy).x - mapLand(pos-eps.yxy).x,
				mapLand(pos+eps.yyx).x - mapLand(pos-eps.yyx).x );
		globPrimaryRay = true;
		return normalize(nor);
}

// #ifdef DOGRASS

// 		float mapGrass( vec3 pos ) {
			
// 			vec3 cenPoint2 = vec3(0.5);
// 			vec3 repPos2 = opRep(pos,cenPoint2);
// 			return sdSphere(repPos2,0.1);	
			
// 		}

// 		vec3 normGrass( vec3 pos )
// 		{
// 				vec3 eps = vec3( 0.0, 0.0, 0.0 );
// 				eps.x = 0.01;
				
// 				vec3 nor = vec3(
// 						mapGrass(pos+eps.xyy) - mapGrass(pos-eps.xyy),
// 						mapGrass(pos+eps.yxy) - mapGrass(pos-eps.yxy),
// 						mapGrass(pos+eps.yyx) - mapGrass(pos-eps.yyx) );
// 				return normalize(nor);
// 		}

// 		vec4 castGrass(
// 				vec3 ro,
// 				vec3 rd,
// 				float minT,
// 				float maxT,
// 				int numSteps
// 		) {
// 			//float fNumSteps = float(numSteps);
			
// 			float t = minT;
// 			int p = 0;
// 			vec3 pos;
// 			float res;
			
// 			float GRASS_PREC = 0.01;
			
// 			for( p = 0; p < numSteps; p++ ) {
					
// 					pos = ro+rd*t;
// 					res = mapGrass( pos );
					
// 					if (res < GRASS_PREC) {
// 							break;
// 					}
					
// 					t += res;
// 			}
			
// 			return vec4(normGrass(pos),t);
			
// 		}

// #endif

vec4 castLand(
		vec3 ro,
		vec3 rd,
		vec4 minT,
		float maxT,
		int numSteps
) {
		int r = 0;
		int p = 0;
		int q = 0;
		float fNumSteps = float(numSteps);
		
		vec4 res2 = vec4(0.0);
		vec2 res = vec2(0.0);
		float t;
		float fp;
		float camDis;
		vec4 finalRes = vec4(0.0,0.0,0.0,MAX_CAM_DIS);
		
		vec3 pos;
		
		vec3 landNorm = vec3(0.0);

		float TER_PREC = 0.3;
		float TER_PREC2 = 0.002;
		
		bool notFoundGrass = true;
		
		float tvals[4];
		tvals[0] = minT.x;
		tvals[1] = minT.y;
		tvals[2] = minT.z;
		tvals[3] = minT.w;
		
		
		
		
		// for (r = 0; r < 4; r++) {
				
				
		//     if (tvals[r] == 0.0) {
						
		//         // if (r > 0) {
		//         //     res.x = TER_PREC - 0.001;
		//         // }
						
		//         break;
		//     }
				
				t = tvals[r];
				
				for( p = 0; p < numSteps; p++ ) {
						
						fp = float(p)/fNumSteps;
						
						pos = ro+rd*t;
						
						camDis = clamp(distance(cameraPos,pos)/MAX_CLIP,0.0,1.0);
						TER_PREC = mix(-64.0,64.0, max(camDis,fp) ); //
						TER_PREC = max(TER_PREC,mix(0.2,1.0, max(camDis,fp) ));
						
						res = mapLand( pos );
						
						// if (notFoundGrass&&(res.x < 2.0)) {
						// 	notFoundGrass = false;
						// 	globGrassPos = t;
						// }
						
						if (
								(abs(res.x) < TER_PREC)
								|| (t >= maxT)
						) {
								break;
						}
						
						// res.x *= mix(0.25,0.125,max(fp,
						// 	1.0-clamp(
						// 		res.x/50.0,
						// 		0.0,
						// 		1.0	
						// 	)
						// ) );
						
						t += res.x*0.25;
				}
				globCurSteps += float(p);
				
		//     if (
		//         (res.x < TER_PREC)
		//         || (t >= maxT)
		//     ) {
		//         break;
		//     }
				
				
		// }
		
		// if (res.x <= 0.0) {
		// 	globTest = 1.0;
			
		// 	t -= 4.0;
			
		// }
		
		
		
		int newDetailSteps = 8;//int( mix(float(TOT_DETAIL_STEPS),2.0,camDis) );
		
		TER_PREC2 = 0.002;//max(TER_PREC*0.001,0.002);
		
		if ((abs(res.x) < TER_PREC)&&(t < maxT)) {
				
				//t -= 4.0;
				
				//t -= 1.1;
				
				//t -= TER_PREC;
			
				for( p = 0; p < newDetailSteps; p++ ) {
						
						pos = ro+rd*t;
						
						res2 = mapLandMicro( pos );
						
						if (abs(res2.x) < TER_PREC2) {
							break;
						}
						
						t += res2.x*0.125;
				}
				globCurSteps += float(p);
				
				// if (res2.x < 0.0) {
				// 	t = TER_PREC2;
				// }
				//pos = ro+rd*t;
				res2 = mapLandMicro( pos );
				
				
				
				globTexEarth.x = res2.w;
				globTexEarth.y = clamp((sin(pos.z/512.0)+1.0)*0.5,0.0,1.0);
				
				float isInTer = float( res2.y < (-0.25) );//res2.y; //
				
				
				
				float snowSource = res2.z;
				float camDis = clamp(distance(cameraPos,pos)*4.0/MAX_CLIP,0.0,1.0);
				landNorm = normLandMicro(pos, vec3(0.0,0.0,1.0), camDis);
				
				float grassMod = 32.0;//mix(2.0,0.000001,camDis);
				float myVal2 = randf3( floor(pos.xyz*(grassMod))/(grassMod) )*clamp(1.0-camDis*4.0,0.0,1.0);
				
				float hv = clamp(1.0-(heightMapMaxInCells-pos.z)/heightMapMaxInCells, 0.0,1.0)*0.3
					+ abs(sin(pos.x/512.0)*sin(pos.y/512.0)*sin(pos.z/512.0))*0.01;
				
				float snowVal = (
					hv
					+ snowSource*0.4
					- camDis*0.02
					//- clamp(-landNorm.z,0.0,1.0)*0.05
					- 0.15 // - camDis*0.1
				); // + landNorm.z*0.05
				
				snowVal += landNorm.z*0.05*float((hv) > 0.1);
				
				float isGrass = snowVal - myVal2*0.01 - clamp(hv-0.12,0.0,0.1)*2.0;
				
				// isGrass -= mod(
				// 						abs(
				// 								sin(pos.z/256.0)*
				// 								sin(pos.x/512.0)*
				// 								sin(pos.y/160.0)
				// 						),
				// 						1.0
				// 				)*0.75*hv;
				
				
				if (globTexEarth.x == TEX_BARK) {
					if (res2.y > -0.4) {
						
					}
					else {
						globTexEarth.x = TEX_TREEWOOD;
						globTexEarth.y = 
							abs(
									sin(pos.z/256.0)*
									sin(pos.x/512.0)*
									sin(pos.y/160.0)
							);
							//(sin(res2.y*4.0)+1.0)*0.5;
					}
				}
				
				
				
				if (isGrass*(1.0-isInTer) > 0.001) {
					if ((pos.z-100.0) > ((seaLevel+myVal2*0.01)*heightMapMaxInCells)) {
						globTexEarth.x = TEX_GRASS;
						globTexEarth.y = clamp(myVal2,0.0,1.0);
					}
				}
				
				if ((pos.z-20.0) < ((seaLevel+myVal2*0.005)*heightMapMaxInCells)) {
					if (landNorm.z > 0.5) {
						globTexEarth.x = TEX_SAND;
					}
				}
				
				if ((snowVal*(1.0-isInTer) > 0.04)&&(globTexEarth.x != TEX_BARK)) {
					globTexEarth.x = TEX_SNOW;
					globTexEarth.y = clamp(snowVal*4.0,0.0,1.0);
				}
				
				
				
				
				
				
				
				//globTexEarth.x = TEX_EARTH;
				
				//p += newDetailSteps;
		}
		else {
				globTexEarth.xy = vec2(0.0);
		}
		
		
		
		
		
		return vec4(normLandMicro(pos,landNorm,camDis),t);
}



float softShadow( vec3 ro, vec3 rd, float tmin, float tmax, int numSteps )
{
		globPrimaryRay = false;
		float res = 1.0;
		float t = tmin;
		float h;
		
		for( int i=0; i<numSteps; i++ )
		{
				h = 
				min(
					mapLand( ro + rd*t ).x*0.5
					,mapDyn(ro + rd*t).x*2.0
				)
				;
				res = min( res, 2.0*h/t );
				t += clamp( h, 0.0, 0.5 );
				if( h<0.001 || t>tmax ) break;
		}
		globPrimaryRay = true;
		return clamp( res, 0.0, 1.0 );

}

// float hardShadowDyn(
// 		vec3 ro,
// 		vec3 rd,
// 		vec2 minMaxT,
// 		float fNumSteps
// ) {
// 		int p = 0;
// 		int numSteps;
		
// 		float res = (0.0);
// 		float t;
// 		float fp;
		
// 		vec3 pos;
		
// 		float SHAD_PREC = 0.1;
		
// 		t = minMaxT.x;
// 		numSteps = int(fNumSteps);
// 		for( p = 0; p < numSteps; p++ ) {
				
// 				fp = float(p)/fNumSteps;
				
// 				pos = ro+rd*t;
				
// 				res = mapDyn( pos ).x;
				
// 				if (res < SHAD_PREC ) {
// 						break;
// 				}
				
// 				t += res;
// 		}
		
// 		return t;
		
// }

float hardShadow( vec3 ro, vec3 rd, float tmin, float tmax, int numSteps )
{
		float res = 0.0;
		float t = tmin;
		
		
		float fNumSteps = float(numSteps);
		float fi;
		float h;
		
		for( int i=0; i<numSteps; i++ ) {
				
				fi = float(i)/fNumSteps;
				
				h = 
				//min(
					mapLand( ro + rd*t ).x
				// 	,mapDyn(ro + rd*t).x
				// )
				;
				
				res += -clamp(h*0.1,-1.0,0.0);
				
				//res = min( res, 8.0*h/t );
				
				t = mix(tmin,tmax,fi);
				
		}
		return 1.0-clamp( res, 0.0, 1.0 );

}











#endif
// end of DOTER




//----------------------------------------------------------------------






//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// const float uvScale = 0.1;
// const float colorUvScale = 0.1;
// const float furDepth = 0.2;
// const int furLayers = 64;
// const float rayStep = furDepth*2.0 / float(furLayers);
// const float furThreshold = 0.4;
// const float shininess = 50.0;

// bool intersectSphere(vec3 ro, vec3 rd, float r, out float t)
// {
//     float b = dot(-ro, rd);
//     float det = b*b - dot(ro, ro) + r*r;
//     if (det < 0.0) return false;
//     det = sqrt(det);
//     t = b - det;
//     return t > 0.0;
// }

// vec3 rotateX(vec3 p, float a)
// {
//     float sa = sin(a);
//     float ca = cos(a);
//     return vec3(p.x, ca*p.y - sa*p.z, sa*p.y + ca*p.z);
// }

// vec3 rotateY(vec3 p, float a)
// {
//     float sa = sin(a);
//     float ca = cos(a);
//     return vec3(ca*p.x + sa*p.z, p.y, -sa*p.x + ca*p.z);
// }

// vec2 cartesianToSpherical(vec3 p)
// {       
//     float r = length(p);

//     float t = (r - (1.0 - furDepth)) / furDepth;    
//     p = rotateX(p.zyx, -cos(curTime*1.5)*t*t*0.4).zyx;  // curl

//     p /= r; 
//     vec2 uv = vec2(atan(p.y, p.x), acos(p.z));

//     //uv.x += cos(curTime*1.5)*t*t*0.4; // curl
//     //uv.y += sin(curTime*1.7)*t*t*0.2;
//     uv.y -= t*t*0.1;    // curl down
//     return uv;
// }

// // returns fur density at given position
// float furDensity(vec3 pos, out vec2 uv, vec3 sc)
// {
//     uv = pos.xy;//cartesianToSpherical(pos.xyz-sc); 
//     vec4 tex = texture2D(Texture14, uv*uvScale);

//     // thin out hair
//     float density = smoothstep(furThreshold, 1.0, tex.x);
		
//     float r = length(pos-sc);
//     float t = (r - (1.0 - furDepth)) / furDepth;
		
//     // fade out along length
//     float len = tex.y;
//     density *= smoothstep(len, len-0.2, t);

//     return density; 
// }

// // calculate normal from density
// vec3 furNormal(vec3 pos, float density, vec3 sc)
// {
//     float eps = 0.01;
//     vec3 n;
//     vec2 uv;
//     n.x = furDensity( vec3(pos.x+eps, pos.y, pos.z), uv, sc ) - density;
//     n.y = furDensity( vec3(pos.x, pos.y+eps, pos.z), uv, sc ) - density;
//     n.z = furDensity( vec3(pos.x, pos.y, pos.z+eps), uv, sc ) - density;
//     return normalize(n);
// }

// vec3 furShade(vec3 pos, vec2 uv, vec3 ro, float density, vec3 sc)
// {
//     // lighting
//     const vec3 L = vec3(0.0,0.0,1.0);//lightVec;
//     vec3 V = normalize(ro - pos);
//     vec3 H = normalize(V + lightVec);

//     vec3 N = -furNormal(pos, density, sc);
		
//     globLastSceneNorm = N;
		
//     //float diff = max(0.0, dot(N, lightVec));
//     float diff = max(0.0, dot(N, lightVec)*0.5+0.5);
//     float spec = pow(max(0.0, dot(N, H)), shininess);
		
//     // base color
//     vec3 color = texture2D(Texture14, uv*0.05).xyz;

//     // darken with depth
//     float r = length(pos);
//     float t = (r - (1.0 - furDepth)) / furDepth;
//     t = clamp(t, 0.0, 1.0);
//     float i = t*0.5+0.5;
				
//     return color*diff*i + vec3(spec*i);
// }       

// vec4 scene(vec3 ro, vec3 rd, vec3 sc)
// {
//     const float r = 1.0;
//     float t;                  
//     bool hit = intersectSphere(ro - sc, rd, r, t);
		
//     float tot = 0.0;
//     vec4 sampleCol;
//     vec2 uv;
//     vec4 c = vec4(0.0);
//     if (hit) {
				
				
//         vec3 pos = ro + rd*t;

//         // ray-march into volume
//         for(int i=0; i<furLayers; i++) {
						
//             sampleCol.a = furDensity(pos, uv, sc);
						
//             tot += sampleCol.a;
//             if (sampleCol.a > 0.0) {
//                 sampleCol.rgb = furShade(pos, uv, ro, sampleCol.a, sc);

//                 // pre-multiply alpha
//                 sampleCol.rgb *= sampleCol.a;
//                 c = c + sampleCol*(1.0 - c.a);
//                 if (c.a > 0.95) break;
//             }
						
//             pos += rd*rayStep;
//         }
				
//         globLastScenePos = t;
				
//     }
		
//     return sampleCol;
// }


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%






void main() {
		
		vec4 oneVec = vec4(1.0);
				
		float NEAR = clipDist.x;
		float FAR = clipDist.y;
		
		int i;
		int j;
		
		int primDataInd = 0;
		vec4 datVec;
		vec4 ln0Vec;
		// vec4 cenVec;
		// vec4 tanVec;
		// vec4 bitVec;
		float maxT = 20.0;
		float hardShadowDynRes = 0.0;
		
		vec3 rd = vec3(0.0);
		
		float MIN_CAM_DIS = 0.0;
		MAX_CAM_DIS = FAR;
		
		float MIN_CAM_VOL_DIS = MIN_CAM_DIS;
		MAX_CAM_VOL_DIS = MAX_CAM_DIS;
		
		vec2 volBounds = vec2(0.0);
		
		globTexTap = 0.0;
		
		
		// #ifdef DOPOLY
				
				
		// 		volBounds = aabbIntersect(worldPos.xyz, rd, volMinReadyInPixels, volMaxReadyInPixels);
		// 		if (volBounds.x <= volBounds.y) {
						
		// 				if (
		// 						all(greaterThanEqual(worldPos.xyz,volMinReadyInPixels)) &&
		// 						all(lessThanEqual(worldPos.xyz,volMaxReadyInPixels))
		// 				) {
								
		// 				}
		// 				else {
		// 						MIN_CAM_VOL_DIS = volBounds.x;
		// 				}
		// 				MAX_CAM_VOL_DIS = volBounds.y;
						
		// 		}
				
				
		// 		rd = normalize(worldPos.xyz-cameraPos.xyz);
		// 		float testRes = 99999.0;
		// 		float tempOffset = 0.0;
		// 		vec3 tempPos = vec3(0.0);
				
		// 		for (i = 0; i < TOT_STEPS_POLY; i++) {
		// 				tempPos = worldPos.xyz + rd*tempOffset;
		// 				testRes = mapLand(tempPos).x;
		// 				tempOffset += testRes;
		// 		}
				
		// 		if (testRes > 0.2) {
		// 				//discard;
		// 		}
				
		// 		FragColor0 = vec4(
		// 				distance(tempPos,cameraPos.xyz),
		// 				worldPos.xyz
		// 		);

		// 		return;
		// #endif
		
		
		
		globLastSceneNorm = vec3(0.0);
		globLastScenePos = 0.0;
		vec4 sceneRes = vec4(0.0);
		
		globCurSteps = 0.0;
		globTotSteps = 0.0;
		globIntersect = 0.0;
		globTest = 0.0;
		globNumPrims = 0;
		globBestLimbInd = -1;
		globPrimaryRay = true;
		globBoneRad = 99999.0;
		globBestLimbDepth = 99999.0;
		primIdListLength = 0;
		
		globWoodDir = 0.0;
		globWoodCoords = vec3(0.0);
		globTexEarth = vec2(0.0);
		globTexDyn = vec2(0.0);
		globExplodeVec = vec3(0.0);
		globGrassPos = -999.0;
		globTexPrim = vec2(0.0);
		globTexWater = vec2(0.0);
		
		vec2 earthMatRes = vec2(0.0);
		vec2 waterMatRes = vec2(0.0);
		
		globWaterMod = float(isUnderWater < 0.0)*1.0;// *(1.0-pow(abs(lookAtVec.z),4.0))*0.6;
		
		texelRes1 = vec4(0.0);
		texelRes2 = vec4(0.0);

		pdVisMin = vec4(0.0);
		pdVisMax = vec4(0.0);
		pdBoundsMin = vec4(0.0);
		pdBoundsMax = vec4(0.0);
		pdCornerDis = vec4(0.0);
		pdMatParmas = vec4(0.0);

		boxDim = vec4(0.0);
		boxPower = vec2(0.0);
		boxCenterPoint = vec3(0.0);
		
		vec2 baseCoords = gl_FragCoord.xy/(bufferDim.xy);

		float aspect = bufferDim.y/bufferDim.x;
		float dx = tan(FOV*0.5)*(baseCoords.x*2.0-1.0)/aspect; //gl_FragCoord.x/(bufferDim.x*0.5)
		float dy = tan(FOV*0.5)*((1.0-baseCoords.y)*2.0-1.0); //gl_FragCoord.y/(bufferDim.y*0.5)
		
		// if (doSphereMap) {
		// 	dx *= 400.0;
		// 	dy *= 400.0;
		// }
		
		dx = -dx;
		vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
		vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);
		p1 = modelviewInverse*p1;
		p2 = modelviewInverse*p2;
		vec3 ro = p1.xyz;
		
		
		
		float phi = baseCoords.x*M_PI*2.0;// - atan(lookAtVec.y,lookAtVec.x) - M_PI;
		float theta = (baseCoords.y)*M_PI;// - acos(lookAtVec.z);
		
		
		vec3 lookAtPoint;
		
		vec4 terSamp4;
		
			if (skipPrim) {
				terSamp4 = vec4(MAX_CAM_DIS);
			}
			else {
				terSamp4 = texture(Texture11,baseCoords.xy);
			}
			
		
		if (doSphereMap) {
				
				// ro = cameraPos.xyz;
				// ro.xy += (baseCoords.xy-0.5)*(cameraPos.z-2000.0);

				// rd = vec3(0.0,0.0,-1.0);
				
				//ro = cameraPos.xyz;
			
				// lookAtPoint = cameraPos.xyz;
			
				// ro = normalize(
				// 	vec3(
				// 		cos(phi)*sin(theta),
				// 		sin(phi)*sin(theta),
				// 		cos(theta)
				// 	)
				// )*500.0 + lookAtPoint;
			
				// ro += -rd*100.0;
				
				// rd = normalize(lookAtPoint-ro);
			
			
			
			ro = cameraPos.xyz;
			rd = normalize(
				vec3(
					cos(phi)*sin(theta),
					sin(phi)*sin(theta),
					cos(theta)
				)
			);
				
				
		}
		else {
				rd = normalize(p1.xyz-p2.xyz);
				
		}
		
		vec2 newTex = vec2(
				((atan(-rd.y,-rd.x)/(M_PI))+1.0)/2.0,
				acos(rd.z)/(M_PI)
		);
		
		
		
		vec3 newWaterMin = waterMin+volMinReadyInPixels;
		vec3 newWaterMax = waterMax+volMinReadyInPixels;

		volBounds = aabbIntersect(ro, rd, volMinReadyInPixels, volMaxReadyInPixels);
		vec2 volBoundsWater = aabbIntersect(ro-rd, rd, newWaterMin, newWaterMax);
		
		
		
		bool hitWater = false;
		float waterStart = MAX_CAM_VOL_DIS;
		
		if (all(lessThan(newWaterMin,newWaterMax))) {
				if (
						all(greaterThan(ro,newWaterMin)) &&
						all(lessThan(ro,newWaterMax))
				) {
						// inside water
						
						hitWater = true;
						waterStart = MIN_CAM_VOL_DIS;//volBoundsWater.y-1.0;
				}
				else {
						
						
						hitWater = (volBoundsWater.x < volBoundsWater.y);
						
						// todo: fix this dirty trick
						if (rd.z > 0.1) {
								hitWater = false;
						}
						
						if (hitWater) {
								waterStart = volBoundsWater.x-1.0;
						}
				}
		}
		
		
		bool hitWaterMacro = false;
		float waterStartMacro = MAX_CAM_DIS;
		float sl = seaLevel*heightMapMaxInCells;
		float disNeeded = 0.0;
		
		if (ro.z < sl) {
			hitWaterMacro = true;
			waterStartMacro = 0.0;
		}
		else {
			if (rd.z < 0.0) {
				disNeeded = abs((sl - ro.z)/rd.z);
				if (disNeeded < MAX_CAM_DIS) {
					hitWaterMacro = true;
					waterStartMacro = disNeeded;
				}
			}
		}
		
		bool hitLandMacro = false;
		float landStartMacro = MAX_CAM_DIS;
		float landEndMacro = MAX_CAM_DIS;
		sl = heightMapMaxInCells;
		disNeeded = abs((sl - ro.z)/rd.z);
		
		if (ro.z < sl) {
			hitLandMacro = true;
			landStartMacro = 0.0;
			
			if (rd.z > 0.0) {
				landEndMacro = disNeeded;
			}
			else {
				landEndMacro = abs(ro.z/rd.z);
			}
			
		}
		else {
			if (rd.z < 0.0) {
				
				if (disNeeded < MAX_CAM_DIS) {
					hitLandMacro = true;
					landStartMacro = disNeeded;
				}
			}
		}
		
		landEndMacro = min(landEndMacro,FAR);
		
		
		
		float camDis;
		float zbVal;
		float curMat = 0.0;
		vec2 curTex = vec2(0.0);
		
		vec2 curTexSolid = vec2(0.0);
		vec2 curTexTrans = vec2(0.0);
		
		vec3 nor = vec3(0.0);
		vec3 pos = vec3(0.0);

		vec4 fragRes0 = vec4(0.0);
		vec4 fragRes1 = vec4(0.0);
		
		vec4 cacheLand = vec4(0.0);
		vec4 cacheWater = vec4(0.0);
		
		float tBase[2];
		float t;
		float landVal = 0.0;
		float dynVal = 0.0;
		float solidVal = 0.0;
		float waterVal = 0.0;
		float transVal = 0.0;
		
		float fi;
		float fj;
		
		//vec4 dtexLast = vec4(999999.0);
		vec4 dtexSphere = vec4(999999.0);
		
		vec2 tcl;
		vec2 tcs;
		
		bool setMax = false;
		bool setMaxLand = false;
		bool setMaxWater = false;
		
		
		
		//float subPrec = 0.125;
		
		vec3 solidNorm = vec3(0.0);
		vec3 transNorm = vec3(0.0);
		
		
		
		
		
		
		vec4 v1 = vec4(0.0);
		vec4 v2 = vec4(0.0);
		vec4 v3 = vec4(0.0);
		
		
		vec4 wpTex = vec4(0.0);
		
		if (doSphereMap) {
				tcs = baseCoords.xy;
				tcl = newTex.xy;
		}
		else {
				tcl = baseCoords.xy;
				tcs = newTex.xy;
		}
		
		
		
		int T_RAD = 1;
		
		float minPlaneDis = distance(ro,cameraPos);
		
		float newInv = clamp(invalidCount,0.125,0.25);
		
		
		// #ifdef USESPHEREMAP
		
		// for (j = -T_RAD; j <= T_RAD; j++) {
		// 		fj = float(j);
		// 		for (i = -T_RAD; i <= T_RAD; i++) {
		// 				fi = float(i);
						
		// 				// dtexLast = min(
		// 				//     dtexLast,
		// 				//     texture(Texture3, tcl.xy + vec2(fi,fj)*
		// 				//         1.0*newInv / 
		// 				//         bufferDim.xy ).xyzw   
		// 				// );
						
						
		// 				dtexSphere = min(
		// 						dtexSphere,
		// 						texture(Texture5, tcs.xy + vec2(fi,fj) *
		// 								4.0*(newInv/SPHEREMAP_SCALE_FACTOR) / 
		// 								bufferDim.xy ).xyzw   
		// 				);
		// 		}
		// }
		
		// #endif
		
		
		
		
		
		
		
		if (volBounds.x <= volBounds.y) {
				
				if (
						all(greaterThanEqual(ro,volMinReadyInPixels)) &&
						all(lessThanEqual(ro,volMaxReadyInPixels))
				) {
						
				}
				else {
						MIN_CAM_VOL_DIS = volBounds.x;
				}
				MAX_CAM_VOL_DIS = volBounds.y;
				
		}
		
		
		
		
		
		
		
		
		
		vec4 maxRes = vec4(0.0);
		
		
		//cacheLand = texture(Texture3,baseCoords.xy).xxxx;// - vec4(1.0);
		cacheWater = vec4(MIN_CAM_VOL_DIS);
		
		vec4 finalMaxDis = vec4( max(MAX_CAM_DIS,MAX_CAM_VOL_DIS) );
		
		// #ifdef USESPHEREMAP
		// 		if (doSphereMap) {
						
		// 		}
		// 		else {
						
		// 				if (depthInvalidMove) {
								
		// 				}
		// 				else {
		// 						// cache = max(
		// 						//     cache,
		// 						//     dtexSphere.xxyy - vec4(0.5,0.5,4.0,4.0)
		// 						// );
		// 				}
		// 		}
		// #endif
						
		

		//if (readPoly) {}
				
		
		
		
		
		
		dynVal = MAX_CAM_DIS;
		landVal = MAX_CAM_VOL_DIS;//MAX_CAM_DIS;
		solidVal = MAX_CAM_VOL_DIS;
		waterVal = MAX_CAM_VOL_DIS;//MAX_CAM_DIS;
		transVal = MAX_CAM_VOL_DIS;
		
		
		vec4 dynRes = vec4(0.0,0.0,0.0,MAX_CAM_DIS);
		vec4 rclRes = dynRes;
		vec4 grassRes = dynRes;
		vec4 rcwRes = dynRes;
		
		//vec3 tempNor = vec3(0.0);
		
		float tArr[2];
		vec2 curTexArr[2];
		vec3 norArr[2];
		vec3 posArr[2];
		
		norArr[0] = vec3(0.0);
		norArr[1] = vec3(0.0);
		
		
		float shadowRes = 1.0;
		
		float tempf = 0.0;
		
		vec3 tempRes = vec3(0.0);
		
		//float explodeMod = 0;
		// #ifdef DOCLOUDS
		// 		explodeMod = 1;
		// #endif
		//+explodeMod
		
		
				
				
		vec4 limbRes = vec4(0.0);
		
		
		#ifdef DOTER
				
				
				vec3 landPos = vec3(0.0);
				vec4 objTex1 = texture(Texture5,baseCoords.xy);
				vec4 objTex2 = texture(Texture6,baseCoords.xy);
				
				//if (volBounds.x < volBounds.y) {}
				
				
				
				
				
				// if (doSphereMap) {

				// }
				// else {
						
						
						
						
						// T_RAD = 4;
						
						// for (j = -T_RAD; j <= T_RAD; j++) {
						//     fj = float(j);
						//     for (i = -T_RAD; i <= T_RAD; i++) {
						//         fi = float(i);
										
						//         maxRes = max(
						//             maxRes,
						//             texture(Texture14, baseCoords.xy + vec2(fi,fj)/bufferDim.xy ).xyzw   
						//         );
						//     }
						// }
						
						
						
						
						
						// setMax = false;//(volBounds.x > volBounds.y);
						// wpTex = texture(Texture14,baseCoords.xy);
						// //wpTex -= (0.75+minPlaneDis)*vec4(notEqual(wpTex,vec4(0.0)));
						// cacheLand = max(
						// 		cacheLand,
						// 		wpTex
						// );
						// if (wpTex.x == 0.0) {
						// 		setMax = true;
						// }
						// // cacheLand = imx(cacheLand,finalMaxDis,vec4(
						// // 		greaterThan(
						// // 				wpTex,
						// // 				vec4(volBounds.y)
						// // 		)
						// // ));
						// // if (getTexCubic(Texture0,ro+rd*MIN_CAM_VOL_DIS-volMinReadyInPixels,volSizePrim).r > 0.0) {
						// // 	 setMax = false;//cacheLand.x = MIN_CAM_VOL_DIS;
						// // 	 cacheLand.x = MIN_CAM_VOL_DIS;
						// // }
						// if (setMax) {
						// 		cacheLand = finalMaxDis;
						// }
						// setMaxLand = setMax;
						
						
						
						
						
						
						setMax = false;//(volBounds.x > volBounds.y);
						
						
						if (hitWater) {
								
								cacheWater = max(
										cacheWater,
										vec4(waterStart)
								);
								
						//if (all(lessThan(newWaterMin,newWaterMax))) {
								// if (volBoundsWater.x <= volBoundsWater.y) {
										
										
								// }
								// else {
								//     setMax = true;
								// }
						}
						else {
								setMax = true;
						}
						
						if (setMax) {
								cacheWater = finalMaxDis;
						}
						
						setMaxWater = setMax;
//				}
				
				
				
				
				// if (cache.x < MAX_CAM_VOL_DIS) {
						
						
				// }
				
				
				
				///////////////////
				// todo: get rid of this for volumes
				
				// MIN_CAM_DIS = 0.0;
				// MAX_CAM_DIS = FAR;
				// MIN_CAM_VOL_DIS = MIN_CAM_DIS;
				// MAX_CAM_VOL_DIS = MAX_CAM_DIS;
				
				///////////////////
				
				
				
				rclRes = castLand(
						ro,
						rd,
						max(cacheLand,vec4(landStartMacro)),//vec4(0.0),//vec4(0.0),//
						min(FAR,landEndMacro),//MAX_CAM_VOL_DIS,
						TOT_STEPS
				);
				
// #ifdef DOGRASS
				
// 				if (globGrassPos >= 0.0) {
// 					grassRes = castGrass(
// 						ro,
// 						rd,
// 						globGrassPos,
// 						globGrassPos+10.0,
// 						32
// 					);
// 				}
				
// 				if (grassRes.w < rclRes.w) {
// 					rclRes = grassRes;
// 					globTexEarth = vec2(TEX_GRASS,0.0);
// 				}
				
// #endif
				
				
				
				
				preLimb(ro, rd);
				
				earthMatRes = globTexEarth;
				
				//landPos = ro+rd*;
				
				
				landVal = rclRes.w;
				
				
				//if (cacheWater.x < min(MAX_CAM_VOL_DIS,landVal) ) {
						
				//}
				
				waterStart = min(waterStart,waterStartMacro);
				
				if (
					(hitWaterMacro||hitWater) &&
					(waterStart < landVal)
				) {
					rcwRes = castWater(
							ro,
							rd,
							vec4(waterStart),//cacheWater,
							min(landVal,FAR),//min(landVal,MAX_CAM_VOL_DIS),
							TOT_STEPS/2
					);
					
					rcwRes.w += 0.1;
					
					if ((isUnderWater < 0.0) ) {
						
						
						//tempf = mapWater(ro + rd*landVal);
						
						
						if ( 
							((vec3(rd*landVal).z - vec3(rd*rcwRes.w).z) < 2.0)
							
							//||( tempf < 0.4 )
						) {
							rcwRes = vec4(0.0,0.0,0.0,MAX_CAM_DIS);
						}
						
						// && (abs(landVal-rcwRes.w) < 4.0)
						
						
					}
					
					waterVal = rcwRes.w;
				}
				
				
				
				waterMatRes = globTexWater;
				
				
				
				
				
				
				//shadowRes = 0.0;
				
				// todo: put this back in
				//if (numExplodes > 0) {
						dynRes = castDyn(
								ro,
								rd,
								vec2( MIN_CAM_DIS, min(landVal,MAX_CAM_DIS) ),
								32.0
						);
						
						dynVal = dynRes.w;
						
						if (
								(dynVal < landVal)
								&& (globTexDyn.x > 0.0)
						) {
								landVal = dynVal;
								rclRes = dynRes;
								earthMatRes = globTexDyn;
								
								//if (earthMatRes.x == TEX_EXPLOSION) {
								//		shadowRes = 1.0;
								//}
								
								if (globBestLimbInd > -1) {
									
									primDataInd = globBestLimbInd;
									
									datVec = texelFetch(Texture1, primDataInd+0);
									ln0Vec = texelFetch(Texture1, primDataInd+5);
									
									limbRes.w = datVec.x;
									limbRes.z = datVec.y;
									
									earthMatRes = vec2(
										datVec.z/255.0,
										datVec.w
									);
									
									if (
										(globBestLimbDepth < -0.01) &&
										(ln0Vec.w == 0.0)	
									) {
										
										earthMatRes = vec2(
											TEX_MEAT,
											datVec.w
										);
										
										
										if (globBoneRad < 0.05) {
											earthMatRes = vec2(
												TEX_BONE,
												datVec.w
											);
										}
										
									}
									
									
									
									
								}
								
						}
				//}
				
				
				float shadDis = min(landVal,min(terSamp4.x,terSamp4.z) );
				
				
				
				if (
					(dot(oneVec,objTex1) > 0.0) &&
					(distance(objTex1.xyz,ro) < rclRes.w)	
				) {
					shadDis = distance(objTex1.xyz,ro);
					//rclRes = vec4(objTex2.xyz, distance(objTex1.xyz,ro));
					//earthMatRes = vec2(TEX_EARTH,0.5);//pack16(objTex2.w);
				}
				
				preLimb(ro+rd*(shadDis-0.1), -lightVec);
				
				
				// // distance from light
				// hardShadowDynRes = hardShadowDyn(
				// 	ro+rd*shadDis-lightVec*maxT,
				// 	lightVec,
				// 	vec2(0.0, maxT),
				// 	HARD_STEPS
				// );
				
				
				shadowRes = 
				//1.0;
				clamp((
						//(clamp((hardShadowDynRes+1.0-maxT)/1.0,0.0,1.0))fe
						// * hardShadow( ro+rd*(shadDis), -lightVec, 0.02, 32.0, HARD_STEPS*4 ) 
						(hardShadow( ro+rd*(shadDis-0.1), -lightVec, 20.0, 512.0, HARD_STEPS )*0.75+0.25)
						*
						pow(softShadow( ro+rd*(shadDis-0.02), -lightVec, 0.02, 20.0, SOFT_STEPS ),2.0)
				),0.0,1.0);
				
				
				
				
				
				// if (
				//     (earthMatRes.x == TEX_EARTH) &&
				//     (rclRes.z > 0.5)    
				// ) {
				//     sceneRes = scene(ro,rd, ro+rd*landVal);
				
				//     if (sceneRes.a + rclRes.z > 1.25) {
				//         earthMatRes.x = TEX_GRASS;
				//         earthMatRes.y = clamp((sceneRes.r+sceneRes.g+sceneRes.b)/3.0,0.0,1.0);
								
				//         landVal = globLastScenePos;
				//         rclRes.w = globLastScenePos;
				//         rclRes.xyz = globLastSceneNorm;
								
				//     }
				
						
				// }
				
				//shadowRes = //(sceneRes.r+sceneRes.g+sceneRes.b)/3.0;
				
				
				
				
				
				///////////////////////
				
				
				
				norArr[0] = rclRes.xyz;
				norArr[1] = rcwRes.xyz;
				
				
				
				tArr[0] = landVal;
				posArr[0] = ro + tArr[0]*rd;
				curTexArr[0] = earthMatRes;
				
				tArr[1] = waterVal;
				posArr[1] = ro + tArr[1]*rd;
				curTexArr[1] = waterMatRes;
				
				
				
		#endif
		
		#ifdef DOPRIM
				
				
				vec4 terSamp0 = texture(Texture7,baseCoords.xy);
				vec4 terSamp1 = texture(Texture8,baseCoords.xy);
				vec4 terSamp2 = texture(Texture9,baseCoords.xy);
				vec4 terSamp3 = texture(Texture10,baseCoords.xy);
				// terSamp4
				vec4 terSamp5 = texture(Texture12,baseCoords.xy);
				
				shadowRes = terSamp4.w;
				
				landVal = terSamp4.x;
				waterVal = terSamp4.y;
				
				if (volBounds.x <= volBounds.y) {
						
						if (MAX_PRIM_IDS > 0) {
								solidVal = postLineStep(
									1.0,
									ro,
									rd,
									lineStep(ro,rd,min(landVal,MAX_CAM_VOL_DIS))
								);
								solidNorm = normSolid( ro + solidVal*rd );
								curTexSolid = globTexPrim;
						}
						
						
						if (placingGeom) {
								primIdList[0] = -1;
								primIdListLength = 1;
								transVal = castSolid(ro, rd, vec2(
										MIN_CAM_VOL_DIS,
										min(landVal,MAX_CAM_VOL_DIS)    
								), MAX_STEPS ).x;
								transNorm = normSolid( ro + transVal*rd );
								curTexTrans = globTexPrim;
						}
				}
				
				
				if (solidVal >= MAX_CAM_VOL_DIS) {
						solidVal = MAX_CAM_DIS;
						
				}
				if (transVal >= MAX_CAM_VOL_DIS) {
						transVal = MAX_CAM_DIS;
				}
				
				
				
				
				tArr[0] = min(landVal,solidVal);
				posArr[0] = ro + tArr[0]*rd;
				if ((solidVal+0.05) < landVal) {
						curTexArr[0] = curTexSolid;
						norArr[0] = solidNorm;
						//shadowRes = 1.0;
				}
				else {
						curTexArr[0] = terSamp5.xy;
						norArr[0] = terSamp1.xyz;
				}
				
				
				// = min(waterVal,transVal);
				
				if ((transVal+0.1) < waterVal) {
						tArr[1] = transVal;
						curTexArr[1] = curTexTrans;
						norArr[1] = transNorm;
				}
				else {
						tArr[1] = waterVal;
						curTexArr[1] = terSamp5.zw;
						norArr[1] = terSamp3.xyz;
				}
				
				posArr[1] = ro + tArr[1]*rd;
				
				pos = posArr[0];
				
				if (MAX_PRIM_IDS > 0) {
					tempRes = lineStep(
						pos - lightVec*MAX_SHAD_DIS_PRIM,
						lightVec,
						MAX_SHAD_DIS_PRIM
					);
					
					// distance from light
					
					
					// hardShadowDynRes = hardShadowPrim(
					// 	pos-lightVec*maxT,
					// 	lightVec,
					// 	vec2(0.0, maxT),
					// 	HARD_STEPS_PRIM
					// );
					
					
					shadowRes *= clamp((
							// hardShadowPrim(
							// 	pos-lightVec*MAX_SHAD_DIS_PRIM, 
							// 	lightVec,
							// 	0.02,
							// 	MAX_SHAD_DIS_PRIM,
							// 	HARD_STEPS*4
							// )
						
							//(clamp((hardShadowDynRes+1.0-maxT)/0.2,0.0,1.0))
							//* 
							//pow(softShadow( ro+rd*(shadDis-0.02), -lightVec, 0.02, 20.0, SOFT_STEPS ),2.0)
							pow(softShadowPrim( pos-rd*0.1, -lightVec, 0.1, 20.0, SOFT_STEPS ),2.0)
					),0.0,1.0);
				}
				
				
				
				
		#endif
		
		
		for (i = 0; i < 2; i++) {
				
				t = tArr[i];
				pos = posArr[i];
				nor = norArr[i];
				curTex = curTexArr[i];
				
				
				//nor = normalize(floor(nor*4.0)/4.0);
				
				if (
						//(t > 0.0)
						//&& (t < MAX_CAM_DIS)
						(
								// (
								// 		all(greaterThanEqual(pos+rd*0.2,volMinReadyInPixels)) &&
								// 		all(lessThanEqual(pos+rd*0.2,volMaxReadyInPixels))
								// )
								
								(t < (FAR-0.1))
								
								//true
								
						)
						
						&& ((curTex.x + curTex.y) > 0.0)
						
				) {
						
						if (curTex.x == TEX_WOOD) {
								curTex.y = getWoodGrain(
										0.0,
										globWoodCoords,
										1.0,
										globWoodDir,
										4.0
								);
						}
						
						curMat = floor(curTex.x*256.0*255.0) + floor(curTex.y*255.0);
						camDis = distance(cameraPos.xyz,pos.xyz);
						zbVal = 1.0-camDis/clipDist.y;
						
				}
				else {
						
						
						t = (FAR-0.1);
						nor = vec3(0.0);
						curMat = 0.0;
						pos = ro+rd*t;//vec3(0.0);
						camDis = distance(cameraPos.xyz,pos.xyz);
						zbVal = 1.0-camDis/clipDist.y;
				}
				
				fragRes0 = vec4(pos.xyz, zbVal);
				fragRes1 = vec4(nor.xyz, curMat);
				
				// if (i == 0) {
				//     tempNor = nor;
				// }
				
				//shadowRes *= clamp(globCurSteps/float(TOT_STEPS*2.0),0.0,1.0);
				
				// if (testOn) {
						
						
						
				// 		#ifdef DOTER
					
				// 				//shadowRes = texture(Texture15, baseCoords.xy).r;
				// 				shadowRes = globCurSteps/float(TOT_STEPS*2.0);
					
				// 				// skip geom pass and send current data to debug
				// 				fragRes0 = vec4(
										
				// 						//texture(Texture14,baseCoords.xy).xxx,
										
				// 						//tempNor.xyz,
										
				// 						//float(setMaxLand),0.0,0.0,
										
										

				// 						//baseCoords.xy, 0.0,
										
				// 						//cacheLand.xyz,//floor(mod(worldPos.xyz,1.0)*16.0)/16.0,//0.0,//float(setMaxLand),
										
				// 						//float(volBounds.x <= volBounds.y), 0.0, 0.0,
										
				// 						//(sin(t/100.0)+1.0)*0.5,0.0,0.0,
										
				// 						shadowRes,
				// 						//globTest,
				// 						0.0,
				// 						0.0,
				// 						//globTexTap/300.0, //
				// 						//float(texture(Texture14,baseCoords.xy).x != 0.0)*0.25,
										
				// 						// (getTexCubic(Texture13, vec3(
				// 						// 	baseCoords.x+(sin(curTime)+1.0)/2.0,
				// 						// 	0.0,//baseCoords.y+(sin(curTime)+1.0)/2.0,
				// 						// 	baseCoords.y+(sin(curTime)+1.0)/2.0
				// 						// )*256.0, voroSize).b), 0.0, 0.0,
										
				// 						//mod(texture(Texture14,baseCoords.xy).x*rd+cameraPos + 1.0,32.0)/32.0,//*float(zbVal > 0.0),
										
										
				// 						1.0
				// 				);
								
								
				// 		#endif
				// 		#ifdef DOPRIM
				// 				fragRes0 = vec4(
				// 						shadowRes + terSamp4.w,
				// 						0.0,
				// 						0.15,
				// 						1.0
				// 				);
				// 		#endif
						
				// }
				
				
				if (i == 0) {
						FragColor0 = fragRes0;
						FragColor1 = fragRes1;
				}
				else {
						FragColor2 = fragRes0;
						FragColor3 = fragRes1;
				}
		}
		
		
		

		
		
		// cache 
		FragColor4 = vec4(
			landVal,
			waterVal,
			solidVal,//hardShadowDynRes,
			shadowRes
		); //terSamp4
		FragColor5 = vec4(curTexArr[0].xy,curTexArr[1].xy); //terSamp5
		
		FragColor6 = limbRes;
		FragColor7 = vec4(globCurSteps/float(TOT_STEPS*2.0),0.0,0.0,0.0);
		
		
		
		
		
		
		
		
		
		
		
		
		

}










