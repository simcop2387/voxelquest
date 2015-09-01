#version 330


uniform bool placingGeom;
uniform bool testOn;
uniform bool depthInvalidMove;
uniform bool depthInvalidRotate;
uniform bool doSphereMap;
uniform bool USE_SPHERE_MAP;

uniform float invalidCount;
uniform float UNIT_MAX;
uniform float FOV;
uniform float curTime;
uniform float volSizePrim;
uniform float waterLerp;

uniform vec3 entPos;
uniform float thirdPerson;
uniform float CAM_BOX_SIZE;
uniform float isUnderWater;
uniform float stepMod;
uniform float SPHEREMAP_SCALE_FACTOR;

uniform vec2 bufferDim;
uniform vec2 clipDist;
uniform vec3 lightVec;
uniform vec3 cameraPos;
uniform vec3 lookAtVec;
uniform vec3 volMinReadyInPixels;
uniform vec3 volMaxReadyInPixels;
uniform vec3 worldSizeInCells;

uniform vec4 paramFetch1;
uniform vec4 paramFetch2;

uniform mat4 proj;
uniform mat4 modelview;
uniform mat4 modelviewInverse;

uniform int numSplashes;
uniform vec4 splashArr[8];

uniform int numExplodes;
uniform vec4 explodeArr[8];

uniform vec4 paramArrGeom[20];


uniform sampler3D Texture0; // E_PL_TERRAIN
//uniform sampler3D Texture1; // E_PL_PRIMIDS

uniform samplerBuffer Texture1; // prim ids

uniform sampler2D Texture2; // hm fbo

// depth targ fbo
uniform sampler2D Texture3;
uniform sampler2D Texture4;

// depth targ fbo sphereMap
uniform sampler2D Texture5;
uniform sampler2D Texture6;

// // sphereTargFBO
// uniform sampler2D Texture7;
// uniform sampler2D Texture8;
// uniform sampler2D Texture9;
// uniform sampler2D Texture10;
// uniform sampler2D Texture11;
// uniform sampler2D Texture12;

$



layout(location = 0) in vec4 vposition;
layout(location = 1) in vec2 vtexcoord;
out vec2 TexCoord0;
void main() {
   TexCoord0 = vtexcoord;
   gl_Position = vposition;
};

$

// qqqq

//x: basic pass, y: detail pass
//const float MAX_DETAIL_DIS = 90.0;
const float MIN_STEPS = 1.0;
const float MAX_STEPS = 100.0;

const float WAVE_HEIGHT = 0.6;

//const int MAX_LAND_PASSES = 2;
//const int MAX_LAND_DET_STEPS = 20;

const float EARTH_STEPS = 400.0;
const float WATER_STEPS = EARTH_STEPS;

const int MAX_PRIM_IDS = 8;
const int MAX_PRIMTEST = 8;

const float SOLID_PREC = 0.002;
const float SOLID_NORMAL_PREC = 0.025;



const float S2O2 = 0.70710678118;
const float MAX_DIAG_RAD = 40.0;
const float MAX_DIAG_RAD2 = MAX_DIAG_RAD*S2O2*S2O2;

int primIdList[MAX_PRIM_IDS];
int primIdListLength;

float primAlreadyTested[MAX_PRIMTEST];

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
const float EXPLODE_ID = -6.0;

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



//##
const float timeScale = 0.4;
const int numWaves = 8;
float amplitude[8] = float[]( 1.0/16.0, 1.0/32.0, 1.0/2.0, 1.0/4.0, 1.0/8.0, 1.0/64.0, 1.0/128.0, 1.0/256.0 );
float wavelength[8] = float[]( 48.0/1.0, 48.0/5.0, 48.0/9.0, 48.0/19.0, 48.0/29.0, 48.0/41.0, 48.0/53.0, 48.0/68.0 );
float speed[8] = float[]( 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);
const vec2 direction[8] = vec2[](
    vec2(cos(-0.7),sin(-0.7)),
    vec2(cos(0.4),sin(0.4)),
    vec2(cos(0.1),sin(0.1)),
    vec2(cos(-0.75),sin(-0.75)),
    vec2(cos(-0.2),sin(-0.2)),
    vec2(cos(0.3),sin(0.3)),
    vec2(cos(-0.1),sin(-0.1)),
    vec2(cos(-0.25),sin(-0.25))
);
//##


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

const vec3 dirVec[6] = vec3[](
    
    vec3(  1.0,  0.0, 0.0  ),
    vec3(  -1.0,  0.0, 0.0  ),
    vec3(  0.0,  1.0, 0.0  ),
    vec3(  0.0,  -1.0, 0.0  ),
    vec3(  0.0,  0.0, 1.0  ),
    vec3(  0.0,  0.0, -1.0  )
    
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

vec2 globTexEarth;
vec2 globTexPrim;
vec2 globTexWater;
float globWoodDir;
vec3 globWoodCoords;

int globNumPrims;
float globWaterMod;
float globCurSteps;
float globLandSteps;
float globTotSteps;
float globIntersect;
float globTest;
vec3 globTest3;

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


in vec2 TexCoord0;
layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;
layout(location = 2) out vec4 FragColor2;
layout(location = 3) out vec4 FragColor3;
layout(location = 4) out vec4 FragColor4;
layout(location = 5) out vec4 FragColor5;



void getPrimVals(int _ptInd) {
    
    int ptInd = _ptInd;
    
    int primReadOffset = ptInd*E_PRIMTEMP_LENGTH;
    
    
    if (ptInd == -1) {
        
        //globTest = 1.0;
        
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


float pointSegDistance2D(vec2 testPt, vec2 pt1, vec2 pt2)
{
  vec2 lineDir = pt2 - pt1;
  vec2 perpDir = vec2(lineDir.y, -lineDir.x);
  vec2 dirToPt1 = pt1 - testPt;
  return abs(dot(normalize(perpDir), dirToPt1));
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



float wave(int i, float x, float y) {
    float frequency = 2.0*M_PI/(wavelength[i]);
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    return amplitude[i] * sin(theta * frequency + curTime*timeScale * phase);
}

float waveHeight(vec2 param) {

    float x = param.x;
    float y = param.y;

    float height = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        height += wave(i, x, y);
    }
    return height;
}

float dWavedx(int i, float x, float y) {
    float frequency = 2.0*M_PI/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].x * frequency;
    float res = A * cos(theta * frequency + curTime*timeScale * phase);
    return res*2.0;
}

float dWavedy(int i, float x, float y) {
    float frequency = 2.0*M_PI/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].y * frequency;
    float res =  A * cos(theta * frequency + curTime*timeScale * phase);
    return res*2.0;
}

vec3 waveNormal(vec2 param) {
    float x = param.x;
    float y = param.y;
    float dx = 0.0;
    float dy = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    vec3 n = vec3(-dx, -dy, 1.0);
    return normalize(n);
}



float sdPlane( vec3 p ) {
    return p.z;
}

float sdTer( vec3 p )
{
    
    // vec4 tex3 = texture2D(Texture0, p.xy/400.0 + 0.3);
    // vec4 tex0 = texture2D(Texture0, p.xy/70.0 + 0.2);
    // vec4 tex1 = texture2D(Texture0, p.xy/4.0 + 0.1);
    // vec4 tex2 = texture2D(Texture0, p.xy*4.0);
    
    // return (p.z-(
    //     tex3*16.0 + tex0.x*4.0 + tex1*0.2 + tex2*0.01   
    // ))*0.25;
    
    return 1.0;
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

vec2 sdBoxM( float m, vec3 p, vec3 b ) {
    vec3 d = abs(p) - b;
    return vec2(
        min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0)),
        m
    );
}

float sdBoxDisp( vec3 p, vec3 pb, float dv )
{
  vec3 b = pb;
  b.z -= dv;    
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}



float length8( vec2 p )
{
    p = p*p; p = p*p; p = p*p;
    return pow( p.x + p.y, 1.0/8.0 );
}


float sdCone( in vec3 p, in vec3 c )
{
    vec2 q = vec2( length(p.xz), p.y );
    return max( max( dot(q,c.xy), p.y), -p.y-c.z ); 
}

// float sdCone( vec3 p, vec3 c )
// {
//     vec2 q = vec2( length(p.xz), p.y );
//     float d1 = -p.y-c.z;
//     float d2 = max( dot(q,c.xy), p.y);
//     return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.0);
// }


// float sdCone( vec3 pos, vec3 base, float len, float rad ) {
//     float dis = distance(pos.xy,base.xy);
//     float lv = smoothstep(base.z, base.z + len, pos.z);
//     return dis-mix(rad,0.0,lv);
// }


vec4 getUVW(
    vec3 worldPos,
    vec3 centerPos, 
    vec4 box_dim,
    vec4 uvwScale,
    bool mirrored
) {
    
    globIntersect = 999.0;
    
    vec3 minCorner = centerPos-(box_dim.xyz+box_dim.w);
    vec3 pos = (worldPos-minCorner)*uvwScale.xyz;// /(box_dim.xyz+box_dim.w); //
    
    vec3 centerOffset = worldPos-centerPos;
    vec3 innerBoxSize = box_dim.xyz;
    float cornerRad = box_dim.w;
    
    //vec3 totBoxSize = innerBoxSize+cornerRad;
    //vec3 pos = pPos*(floor(totBoxSize*16.0)/16.0)/totBoxSize;
    
    vec3 uvwCoords = pos.xyz;
    
    
    
    
    vec3 newOffset = max(abs(centerOffset)-innerBoxSize,0.0);
    vec2 newNorm1 = normalize(newOffset.xy)*sign(centerOffset.xy);
    vec2 newNorm2 = normalize(vec2(length(newOffset.xy), newOffset.z));
    
    
    //vec3 innerBoxSizeFloored = floor(innerBoxSize*8.0)/8.0;
    
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
    
    // if (mirrored) {
    //     if (centerOffset.y < 0.0) {
    //         curPhi *= -1.0;
    //     }
    //     if (centerOffset.z < 0.0) {
    //         curPhi *= -1.0;
    //     }
    // }
    
    float angMod = 
        (uvwScale.w*2.0/M_PI) *
        (max(floor(sqrt(box_dim.w*box_dim.w*2.0)),1.0));
    
    //globTest3.xy = vec2(curPhi,curThe)/M_PI;
    
    // side corner
    if (newNorm1.x*newNorm1.y != 0.0) {
        uvwCoords.x = curPhi*angMod;
        uvwCoords.y = pos.z;
    }
    
    // top corner
    if (newNorm2.x*newNorm2.y != 0.0) {
        //uvwCoords.x = curPhi*cornerRad;
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
    
    
    uvwCoords.z = 0.0;//length(max(abs(centerOffset)-box_dim.xyz,0.0));
    
    //uvwCoords.z = length(centerOffset);
    
    return vec4(uvwCoords,newNorm2.x);
}


vec2 opRotate( vec2 p, float theta )
{
    mat2 m = mat2(
        cos(theta),
        -sin(theta),
        sin(theta),
        cos(theta)
    );
    
    return m*p;
}

vec3 get2UVW(
    vec3 basePos,
    vec3 bCenter,
    vec4 box_dim
) {
    
    vec3 offPos = (basePos - bCenter);
    vec3 innerBoxSize = box_dim.xyz;
    vec3 closestInnerPoint = clamp(offPos,-innerBoxSize,innerBoxSize);    
    vec2 normCenter = normalize(offPos.xy);
    vec3 normCenter2 = abs(normalize(offPos.xyz));
    vec3 norm = 
        abs(normalize(offPos.xyz));
        //(normalize(offPos-closestInnerPoint));
    
    vec3 upVec = vec3(0.0,0.0,1.0);
    
    if (abs(dot(upVec,norm)) >= 0.999) {
        upVec = vec3(1.0,0.0,0.0);
    }
    
    // vec3 tang = vec3(0.0);
    // vec3 btng = vec3(0.0);
    // btng = normalize(cross(norm,upVec));
    // tang = normalize(cross(norm,btng));
    
    // mat3 tbn = mat3(
    //   tang.x, btng.x, norm.x, 
    //   tang.y, btng.y, norm.y,
    //   tang.z, btng.z, norm.z
    // );
    
    // return tbn*abs(basePos)/mix(
    //     vec3(20.0,10.0,10.0),
    //     vec3(20.0,20.0,20.0),
    //     vec3(pow(normCenter2.z,2.0) )
    // );// + bCenter;
    
    
    
    
    vec3 finalPos = (
        // norm.xyz*norm.xzy +
        // norm.xyz*norm.yxz +
        // norm.xyz*norm.yzx +
        // norm.xyz*norm.zxy +
        // norm.xyz*norm.zyx +
        
        // norm.xzy*norm.yxz +
        // norm.xzy*norm.yzx +
        // norm.xzy*norm.zxy +
        // norm.xzy*norm.zyx +
        
        // norm.yxz*norm.yzx +
        // norm.yxz*norm.zxy +
        // norm.yxz*norm.zyx +
        
        // norm.yzx*norm.zxy +
        // norm.yzx*norm.zyx +
        
        // norm.zxy*norm.zyx
        
        norm.xyz + norm.yzx + norm.zxy
    );
    
    
    
    
    float zval = clamp(abs(offPos.z)/(box_dim.z+box_dim.w),0.0,1.0);
    zval = abs(zval-0.5)*2.0;
    
    zval = mix(0.25,1.0,zval);
    
    //finalPos.x = zval*basePos.z;
    
    //finalPos.xy *= zval;
    
    finalPos = offPos*0.4*finalPos + bCenter;
    
    return finalPos;
    
    //return vec3(theta*16.0/M_PI,phi*16.0/M_PI,offPos.z);
    
    // vec3 resPos = offPos;
    
    // if (normCenter2.x > normCenter2.y) {
    //     if (normCenter2.x > normCenter2.z) {
    //         resPos.xyz = offPos.yzx;
    //         resPos.y += 0.75;
    //     }
    // }
    // else {
    //     if (normCenter2.y > normCenter2.z) {
    //         resPos.xyz = offPos.xzy;
    //     }
    // }
    
    // return resPos;// + bCenter;
    
    
    // float theta = acos(normCenter2.z);
    // //theta += M_PI/2.0;
    // float phi = atan(normCenter.y,normCenter.x);
    // vec3 newPos = offPos;
    // //newPos.yz = opRotate(newPos.yz,theta);
    // vec3 newPos2 = newPos;
    // newPos2.xy = opRotate(newPos2.xy,phi);
    // newPos2.yz = opRotate(newPos2.yz,theta);
    // return mix(newPos2,newPos,abs(normCenter2.z)) + bCenter;
    
    
    //vec3 norm = abs(normalize(offPos));
    
    //offPos = abs(sin(offPos));
    
    //1: xyz
    //2: xzy
    //3: yxz
    //4: yzx
    //5: zxy
    //6: zyx
    
    
    
    //return vec3(norm.xy + norm.yx + norm.zx, 1.0 )*4.0;
    
    // return (
    //     norm.xyz*norm.xzy +
    //     norm.xyz*norm.yxz +
    //     norm.xyz*norm.yzx +
    //     norm.xyz*norm.zxy +
    //     norm.xyz*norm.zyx +
        
    //     norm.xzy*norm.yxz +
    //     norm.xzy*norm.yzx +
    //     norm.xzy*norm.zxy +
    //     norm.xzy*norm.zyx +
        
    //     norm.yxz*norm.yzx +
    //     norm.yxz*norm.zxy +
    //     norm.yxz*norm.zyx +
        
    //     norm.yzx*norm.zxy +
    //     norm.yzx*norm.zyx +
        
    //     norm.zxy*norm.zyx
        
        
        
    // ) * 
    
    // (
    
    //     1.0-(
    //         abs(dot(norm.xyz,vec3(1.0,0.0,0.0))) +
    //         abs(dot(norm.xyz,vec3(0.0,1.0,0.0))) +
    //         abs(dot(norm.xyz,vec3(0.0,0.0,1.0)))
    //     )/3.0
        
    // ) * offPos/1.0;// + basePos;
    
    
    // // if (all(equal(closestInnerPoint,offPos))) {
    // //     norm = normalize(offPos);
    // // }
    
    
    // float theta = acos(norm.z);
    // //theta += M_PI/2.0;
    // float phi = atan(norm.y,norm.x);
    // vec3 newPos = offPos;
    // //newPos.yz = opRotate(newPos.yz,theta);
    // newPos.xy = opRotate(newPos.xy,phi);
    // return newPos + bCenter;
    
    
    
    // // if (abs(dot(upVec,norm)) >= 0.999) {
    // //     upVec = vec3(1.0,0.0,0.0);
    // // }
    
    // // if (abs(dot(upVec,norm)) >= 0.999) {
        
    // //     upVec = vec3(1.0,0.0,0.0);
        
    // //     //norm = normalize(offPos);
    // //     //tang = vec3(1.0,0.0,0.0);
    // //     //btng = vec3(0.0,1.0,0.0);
    // // }
    // // // else {
    // // //     if  {
    // // //         tang = vec3(-1.0,0.0,0.0);
    // // //         btng = vec3(0.0,-1.0,0.0);
    // // //     }
    // // //     else {
            
    // // //     }
    // // // }
    
    
    // // tang = x;
    // // btng = y;
    // // norm = z;
    
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

// vec4 udRoundBoxLayered(
//     vec3 vectorFromCenter,
//     vec4 box_dim,
//     vec2 box_power,
//     vec4 outer,
//     vec4 inner
// ) {
//     vec3 absVecFromCenter = abs(vectorFromCenter);
    
//     vec3 newP = abs(max( absVecFromCenter-(box_dim.xyz), vec3(0.0) ));
    
//     newP.xy = pow(newP.xy, box_power.xx );
//     newP.x = pow( newP.x + newP.y, 1.0/box_power.x );
    
//     newP.xz = pow(newP.xz, box_power.yy );
//     newP.x = pow( newP.x + newP.z, 1.0/box_power.y );
    
//     return max(
//         (newP.xxxx-outer),
//         ( (outer-inner)-newP.xxxx )    
//     );
// }


//vec3 newP2 = abs(max( (box_dim.xyz + box_dim.w)-absVecFromCenter,0.0));
//float len1 = length(newP);
// vec3 bdTot = box_dim.w+box_dim.xyz;
// find closest edge

// vec3 disVals = vec3(
//     distance(absVecFromCenter,vec3(absVecFromCenter.x, bdTot.y, bdTot.z)),
//     distance(absVecFromCenter,vec3(bdTot.x, absVecFromCenter.y, bdTot.z)),
//     distance(absVecFromCenter,vec3(bdTot.x, bdTot.y, absVecFromCenter.z))
// );

// float len2 = distance(absVecFromCenter,bdTot);

// // float len2 = min(min(disVals.x,disVals.y),disVals.z);

// // len2 = max(len2,distance(absVecFromCenter,bdTot));

// //len2 = min(len2,distance(absVecFromCenter,bdTot));

// // len2 = mix(
// //     len2,
// //     max(len2,distance(absVecFromCenter,bdTot)),
// //     float(all(greaterThan(absVecFromCenter,box_dim.xyz)))
// // );

// float mixVal = 0.5;//abs(sin(curTime));

// float finalDis1 = mix(
//     len1 - box_dim.w,
//     (box_dim.w - len2),
//     mixVal
// );
// float finalDis2 = mix(
//     (box_dim.w-wallThickness)-len1,
//     len2 - (box_dim.w+wallThickness),//len2-(box_dim.w-wallThickness),//(box_dim.w - len2),
//     mixVal
// );

// return vec3(
//     finalDis1,//finalDis-box_dim.w,
//     finalDis2,//finalDis2,//(box_dim.w-wallThickness)-finalDis,
//     -1000.0//finalDis-(box_dim.w-wallThickness)
// );










float sdTorus( vec3 p, vec2 t )
{
  return length( vec2(length(p.yz)-t.x,p.x) )-t.y;
}

float sdHexPrism( vec3 p, vec2 h )
{
    vec3 q = abs(p);
#if 0
    return max(q.z-h.y,max((q.x*0.866025+q.y*0.5),q.y)-h.x);
#else
    float d1 = q.z-h.y;
    float d2 = max((q.x*0.866025+q.y*0.5),q.y)-h.x;
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.);
#endif
}

float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
    vec3 pa = p-a;
    vec3 ba = b-a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - r;
}



float sdTriPrism2( vec3 p, vec2 h )
{
    vec3 q = abs(p);
    
    float d1 = q.z-h.y;
    float d2 = max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5;
    
    return max(d1,d2);
}


float sdCylinder( vec3 p, vec2 h )
{
  vec2 d = abs(vec2(length(p.xz),p.y)) - h;
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}



float length2( vec2 p )
{
    return sqrt( p.x*p.x + p.y*p.y );
}

float length6( vec2 p )
{
    p = p*p*p; p = p*p;
    return pow( p.x + p.y, 1.0/6.0 );
}



float sdTorus82( vec3 p, vec2 t )
{
  vec2 q = vec2(length2(p.xz)-t.x,p.y);
  return length8(q)-t.y;
}

float sdTorus88( vec3 p, vec2 t )
{
  vec2 q = vec2(length8(p.xz)-t.x,p.y);
  return length8(q)-t.y;
}

float sdCylinder6( vec3 p, vec2 h )
{
  return max( length6(p.xz)-h.x, abs(p.y)-h.y );
}

//----------------------------------------------------------------------

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
    //(d1.x<d2.x) ? d1 : d2;
}

vec3 opUTie( vec3 d1, vec3 d2 )
{
    // if (d1.x == d2.x) {
        
    //     globTest = 1.0;
        
    //     return mix(d2,d1,float(d1.z<d2.z));
    // }
    // else {
    //     return mix(d2,d1,float(d1.x<d2.x));
    // }
    
    
    
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

vec3 opTwist( vec3 p )
{
    float amount = 10.0;
    
    float  c = cos(amount*p.z+amount + curTime);
    float  s = sin(amount*p.z+amount + curTime);
    mat2   m = mat2(c,-s,s,c);
    return vec3(m*p.xy,p.z);
}

// float opBlend( float a, float b )
// {
//     float k = 8.0;
//     a = pow( a, k );
//     b = pow( b, k );
//     return pow( (a*b)/(a+b), 1.0/k );
// }

// vec4 opBlend5( vec4 ap, vec4 bp )
// {
//     float k = 2.0;
//     float a = pow( ap.x, k );
//     float b = pow( bp.x, k );
//     return
//         vec4( 
//             pow( (a*b)/(a+b), 1.0/k )
//             ,0.0,0.0,0.0
//         );
// }

// vec4 opBlend4( vec4 ap, vec4 bp )
// {
//     float a = ap.x;
//     float b = bp.x;
//     return
//         vec4( 
//             mix(a,b,clamp(a/(a+b),0.0,1.0))
//             ,0.0,0.0,0.0
//         );
// }





// vec4 cell3D(vec3 P) {
//     // source: http://github.com/BrianSharpe/Wombat/blob/master/Cellular3D.glsl

//     //  establish our grid cell and unit position
//     vec3 Pi = floor(P);
//     vec3 Pf = P - Pi;

//     // clamp the domain
//     Pi.xyz = Pi.xyz - floor(Pi.xyz * ( 1.0 / 69.0 )) * 69.0;
//     vec3 Pi_inc1 = step( Pi, vec3( 69.0 - 1.5 ) ) * ( Pi + 1.0 );

//     // calculate the hash ( over -1.0->1.0 range )
//     vec4 Pt = vec4( Pi.xy, Pi_inc1.xy ) + vec2( 50.0, 161.0 ).xyxy;
//     Pt *= Pt;
//     Pt = Pt.xzxz * Pt.yyww;
//     const vec3 SOMELARGEFLOATS = vec3( 635.298681, 682.357502, 668.926525 );
//     const vec3 ZINC = vec3( 48.500388, 65.294118, 63.934599 );
    
//     const float maxOff = 1.0;
//     const float maxOff2 = maxOff*2.0;
    
    
//     vec3 lowz_mod = vec3( 1.0 / ( SOMELARGEFLOATS + Pi.zzz * ZINC ) );
//     vec3 highz_mod = vec3( 1.0 / ( SOMELARGEFLOATS + Pi_inc1.zzz * ZINC ) );
    
//     lowz_mod = vec3(0.0);
//     highz_mod = vec3(0.0);
    
//     int iy = int(P.y);
    
    
//     // vec2 evenOdd = vec2(0.0);
    
//     // if ( mod(iy,2) == 0 ) {
//     //     evenOdd.x = 1.0;
//     // }
//     // else {
//     //     evenOdd.y = 1.0;
//     // }
    
    
    
//     vec4 hash_x0 = fract( Pt * lowz_mod.xxxx ) * maxOff2 - maxOff;
//     vec4 hash_x1 = fract( Pt * highz_mod.xxxx ) * maxOff2 - maxOff;
//     vec4 hash_y0 = fract( Pt * lowz_mod.yyyy ) * maxOff2 - maxOff;
//     vec4 hash_y1 = fract( Pt * highz_mod.yyyy ) * maxOff2 - maxOff;
//     vec4 hash_z0 = fract( Pt * lowz_mod.zzzz ) * maxOff2 - maxOff;
//     vec4 hash_z1 = fract( Pt * highz_mod.zzzz ) * maxOff2 - maxOff;

//     //  generate the 8 point positions
//     const float JITTER_WINDOW = 0.166666666;  // 0.166666666 will guarentee no artifacts.
//     hash_x0 = ( ( hash_x0 * hash_x0 * hash_x0 ) - sign( hash_x0 ) ) * JITTER_WINDOW + vec4( 0.0, 1.0, 0.0, 1.0 );
//     hash_y0 = ( ( hash_y0 * hash_y0 * hash_y0 ) - sign( hash_y0 ) ) * JITTER_WINDOW + vec4( 0.0, 0.0, 1.0, 1.0 );
//     hash_x1 = ( ( hash_x1 * hash_x1 * hash_x1 ) - sign( hash_x1 ) ) * JITTER_WINDOW + vec4( 0.0, 1.0, 0.0, 1.0 );
//     hash_y1 = ( ( hash_y1 * hash_y1 * hash_y1 ) - sign( hash_y1 ) ) * JITTER_WINDOW + vec4( 0.0, 0.0, 1.0, 1.0 );
//     hash_z0 = ( ( hash_z0 * hash_z0 * hash_z0 ) - sign( hash_z0 ) ) * JITTER_WINDOW + vec4( 0.0, 0.0, 0.0, 0.0 );
//     hash_z1 = ( ( hash_z1 * hash_z1 * hash_z1 ) - sign( hash_z1 ) ) * JITTER_WINDOW + vec4( 1.0, 1.0, 1.0, 1.0 );

//     //  return the closest squared distance
//     vec4 dx1 = Pf.xxxx - hash_x0;
//     vec4 dy1 = Pf.yyyy - hash_y0;
//     vec4 dz1 = Pf.zzzz - hash_z0;
//     vec4 dx2 = Pf.xxxx - hash_x1;
//     vec4 dy2 = Pf.yyyy - hash_y1;
//     vec4 dz2 = Pf.zzzz - hash_z1;
//     vec4 d1 = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
//     vec4 d2 = dx2 * dx2 + dy2 * dy2 + dz2 * dz2;


//     vec4 d3 = min(d1, d2);
//     d3.xy = min(d3.xy, d3.wz);
    
//     float finalRes = min(d3.x, d3.y); // return a value scaled to 0.0->1.0
//     float gradVal = clamp((1.0 - (finalRes * 2.0 / (d3.x + d3.y))), 0.0, 1.0);

    
//     vec3 q0 = vec3(dx1.x,dy1.x,dz1.x);
//     vec3 q1 = vec3(dx1.y,dy1.y,dz1.y);
//     vec3 q2 = vec3(dx1.z,dy1.z,dz1.z);
//     vec3 q3 = vec3(dx1.w,dy1.w,dz1.w);
//     vec3 q4 = vec3(dx2.x,dy2.x,dz2.x);
//     vec3 q5 = vec3(dx2.y,dy2.y,dz2.y);
//     vec3 q6 = vec3(dx2.z,dy2.z,dz2.z);
//     vec3 q7 = vec3(dx2.w,dy2.w,dz2.w);
    
//     //float r0 = float(finalRes == d1.x);
//     float r1 = float(finalRes == d1.y);
//     float r2 = float(finalRes == d1.z);
//     float r3 = float(finalRes == d1.w);
//     float r4 = float(finalRes == d2.x);
//     float r5 = float(finalRes == d2.y);
//     float r6 = float(finalRes == d2.z);
//     float r7 = float(finalRes == d2.w);
    
//     vec3 finalPoint = q0;
//     finalPoint = mix(finalPoint,q1,r1);
//     finalPoint = mix(finalPoint,q2,r2);
//     finalPoint = mix(finalPoint,q3,r3);
//     finalPoint = mix(finalPoint,q4,r4);
//     finalPoint = mix(finalPoint,q5,r5);
//     finalPoint = mix(finalPoint,q6,r6);
//     finalPoint = mix(finalPoint,q7,r7);


//     return vec4(finalPoint,gradVal);//vec2(finalRes,gradVal) * ( 9.0 / 12.0 );

// }

vec3 getTexPos(vec3 inPos) {
    return (
        inPos-volMinReadyInPixels
    )/volSizePrim; // 
}

// float getLand3D(float sampVal) {
    
//     return mix(
//         0.5,
//         -0.5,
//         sqrt(sampVal)//sampVal// //samp.g //+sin(curTime)*0.1
//     );
// }

float getLand3D(vec3 pos, float sampVal) {
    
    
    float maxDis = mix(0.5,1.0,distance(cameraPos,pos)/MAX_CAM_DIS);
    
    return mix(
        maxDis,
        -maxDis,
        sqrt(sampVal) //samp.g //+sin(curTime)*0.1
    );
}

float getWater3D(vec3 pos) {
    
    vec4 samp = texture(Texture0, getTexPos(pos) );
    float hasWater = float(samp.g > 0.0);
    
    float waterVal = mix(
        samp.b*hasWater, // last value
        samp.g, // new value
        waterLerp
    );
    
    float maxDis = mix(0.5,1.0,distance(cameraPos,pos)/MAX_CAM_DIS);
    
    return isUnderWater*mix(
        maxDis,
        -maxDis,
        sqrt(waterVal) //samp.g //+sin(curTime)*0.1
    ) + globWaterMod;
    
    // return mix(
    //         0.5,
    //         -0.5,
    //         sqrt(waterVal) //samp.g //+sin(curTime)*0.1
    //     );
    
    // return mix(
    //     1.0,
    //     -2.0,
    //     mix(waterVal,0.0,0.2) //samp.g //+sin(curTime)*0.1
    // );
}

float getEmpty3D(vec3 pos) {
    
    vec4 samp = texture(Texture0, getTexPos(pos) );
    float maxDis = mix(0.5,1.0,distance(cameraPos,pos)/MAX_CAM_DIS);
    
    return mix(
        maxDis,
        -maxDis,
        sqrt(samp.a)
    );
}



float getRidge(vec2 uvwBase) {
    vec2 newUVW = mod(uvwBase.xy, 2.0)-1.0;
    return length(newUVW.xy);
}

float getRidge2(vec2 uvwBase) {
    
    vec2 newUVW1 = mod(uvwBase.xy, 2.0)-1.0;
    vec2 newUVW2 = mod(uvwBase.xy+1.0, 2.0)-1.0;
    
    return max(length(newUVW1.xy),length(newUVW2.xy));
    

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





vec4 cell2D(
    in vec3 basePos,
    in float cellSize,
    in float dv,
    in float roundness,
    inout float gradVal,
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
    
    float MAX_MOD = 0.25;// *(sin(curTime/2.0));
    
    vec4 bestPoint = centerPos;
    bestPoint.xyz += randPN(bestPoint.xyz)*MAX_MOD;
    
    vec4 nextBestPoint = centerPos;
    
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
    float disVal = 
        //distance(closestPoint.xyz,bestPoint.xyz) -
        
        //distance(bestPoint.xyz,basePos)-cellSize*0.5;
        
        
        mix(
            dv-distance(closestPoint.xyz,(bestPoint.xyz+nextBestPoint.xyz)*0.5),
            distance(bestPoint.xyz,basePos)-cellSize*0.5,
            roundness
        );
    
    gradVal = clamp((1.0 - (bestPoint.w * 2.0 / (bestPoint.w + nextBestPoint.w))), 0.0, 1.0);
    
    norVal = normalize(bestPoint.xyz-nextBestPoint.xyz);
    
    //gradVal = pow(gradVal,0.25);
        
    return vec4(bestPoint.xyz*cellSize,disVal);
    
}



vec2 getTerrain(
    vec2 pRes,
    vec3 terNorm,
    vec3 pos
) {
    
    int i;
    float fi;
    
    vec2 res = pRes;
    
    float primRes = res.x;
    
    vec2 grassSize = vec2(0.5,2.0)*0.25;
    
    vec3 absTerNorm = abs(terNorm);
    
    vec4 cellVal = vec4(0.0);
    float cellSize = 0.5;
    float jutDis = 0.0;
    float gradVal;
    
    
    float texVal;
    float earthX = 0.0;
    float lerpVal;
    float grassRes = 0.0;
    
    vec3 norVal = vec3(0.0);
    
    vec3 repPos = vec3(0.0);
    // vec3 texPos = pos.xyz*mix(
    //     0.0125,
    //     0.0325,
    //     abs(sin((pos.x*3.1 - pos.y*2.2 + pos.z*1.5)/500.0))    
    // );
    
    
    // vec3 offPos = (pos - bestBoxCenterPoint);
    // vec3 innerBoxSize = bestBoxDim.xyz;
    // vec3 closestInnerPoint = clamp(offPos,-innerBoxSize,innerBoxSize);
    // vec3 myNorm = (normalize(offPos-closestInnerPoint));
    
    float oldX = res.x;
    float cellX;
    
    float isGrass;
    bool isInAir = false;
    //????
    
    //float powVal;
    
    float upTex;
    
    float seedRand = abs(sin((pos.x*6.2 - pos.y*2.6)/20.0))*0.33 + 
                    abs(sin((pos.y*3.4 + pos.z*1.1)/30.0))*0.33 +
                    abs(sin((pos.z*4.4 + pos.x*2.1)/30.0))*0.33;
    
    float camDetail = clamp(distance(pos,cameraPos)*2.0/MAX_CAM_DIS,0.0,1.0);
    
    float texSpacing = 100.0*@myTexSpacing@;
    
    //if (oldX <= max(SOLID_PREC,SOLID_NORMAL_PREC)) {
        
        oldX = res.x;
        
        upTex = texture2D(Texture2, pos.xy/texSpacing).r;
        texVal = 
            texture2D(Texture2, pos.yz/texSpacing).r*absTerNorm.x +
            texture2D(Texture2, pos.zx/texSpacing).r*absTerNorm.y +
            upTex*absTerNorm.z;
        
        texVal *= 3.0*@myTexTest@;
                
        
        //globTest3 = pos; //abs(normalize(pos-bestBoxCenterPoint));//abs(normalize(pos-bestaBoxCenterPoint));//
        
        cellVal = cell2D(
            pos.xyz,
            cellSize*2.0,
            0.1,
            mix(
                0.1,
                0.5,
                seedRand    
            ),
            gradVal,
            norVal
        );
        
        isGrass = (
            (1.0-abs(cos((upTex)*3.0)))*2.0 + 
            (1.0-gradVal*(1.0-seedRand))
        )*smoothstep(0.7,1.0,terNorm.z)*seedRand*upTex*2.0
        /3.0;
        
        //isGrass = 0.0;
        
        //res.x += (1.0-gradVal)*0.1;
        
        
        //sin(curTime)*
        jutDis = clamp(
            distance(pos.z, cellVal.z - cellSize )/cellSize*(pow(gradVal,0.5)),
            0.0,
            1.0
        );
        
        //jutDis = mix(jutDis,0.0,pow(jutDis,8.0));
        
        jutDis = 1.0-jutDis;//pow(jutDis,1.0);// *sin(texVal*M_PI);
        
        
        
        
        
        
        //cellX = opI(res.x,cellVal.w);
        
        
        
        // res.x = opD(
        //     res.x,
            
        //     //pow(jutDis,2.0)*0.25+ 
        //     // + (1.0-gradVal)*0.1
        //     pow(texVal,2.0)*0.125
            
        // );
        
        
        // res.x = mix(res.x,cellX,abs(sin(texVal*4.0)));
        
        oldX = res.x;
        
        res.x = opD(
            res.x,
            (
                jutDis +
                pow(texVal,2.0)
            )*mix(0.2,0.4,camDetail)
        );
        
        res.x = mix(res.x,opD(pRes.x,texVal*0.5),pow(seedRand,3.0));
        
        
        
        
        
        
        res.x = opI(res.x,cellVal.w);
        res.x = mix(
            res.x,
            oldX + texVal*(0.15+seedRand*0.35),
            pow(seedRand,2.0)
        );
        
        
        earthX = res.x;
        
        isInAir = ((res.x) > 0.02);
        
        
        globTexEarth.x = TEX_EARTH;
        
        
        
        
        
        // if (isGrass > 0.5) {
            
        //     for (i = 0; i < 2; i++) {
        //         fi = float(i);
        //         repPos = opRep(
        //             pos
        //             + fi*grassSize.x*0.5
        //             + sin(curTime*2.0+pos.x+pos.y+pos.z + fi*5.3) //  + upTex*10.0
        //             * 0.125 * (1.0-clamp(0.7-primRes,0.0,1.0)) //* grassNorm
        //             ,vec3(grassSize.xx*1.0,1.0)
        //         );
                
        //         //repPos.z += ( earthX ) + sin(curTime);//mix(pos.z,pos.z+earthX,0.5);
                
        //         //repPos.z *= 0.5;
                
        //         lerpVal = pow(abs(sin(pos.x+pos.y+pos.z)),6.0);
                
        //         grassRes = sdSphere(
        //                 vec3(repPos.xy,earthX)*vec3(
        //                     mix(0.5,1.0,lerpVal),
        //                     mix(0.5,1.0,1.0-lerpVal),
        //                     0.5*grassSize.x + (1.0-isGrass)*0.5
        //                 )
        //                 +vec3(
        //                     0.0,
        //                     0.0,
        //                     (1.0-isGrass)*0.05//(1.0-isGrass) + float(isGrass < 0.5)*1000.0
        //                 )
        //                 , 
        //                 // *vec3(1.0,1.0,0.2*grassSize.x),
        //                 // + vec3(0.0,0.0,(earthX+0.25)*2.0))*vec3(1.0,1.0,0.2*grassSize.x
        //                 (grassSize.x)*0.0625 // *(1.0-pow(upTex+0.2,4.0)) //grassNorm*
        //             );
                
        //         res.x = min(
        //             res.x,
                    
                    
        //             grassRes
                    
        //             // sdSphere(
        //             //     repPos, 
        //             //     mix(
        //             //         grassSize.x*0.25,
        //             //         0.0,
        //             //         1.3 - clamp( abs(primRes*5.0*(1.0-powVal)),0.0,1.0)    
        //             //     ) *
        //             //     pow(clamp(distance(primRes,-0.4)*4.0,0.0,1.0),4.0)
        //             // )
        //             //sdBox( repPos, vec3(grassSize.x*0.125,grassSize.x*0.125, 0.0325) )
        //         );
        //     }
            
            
            
        //     // if (res.x >= earthX + texVal*0.1) {
        //     //     res.x = earthX + texVal*0.1;
        //     //     globTexEarth.x = TEX_EARTH;
        //     // }
        //     // else {
                
        //         if (isInAir&&(grassRes < earthX)) { //
        //             globTexEarth.x = 
        //                 TEX_GRASS;
        //                 //TEX_STONE;
                        
        //                 globTexEarth.y = abs(sin(seedRand*4.0+earthX*2.0));
        //                 //globTexEarth.y = clamp(abs(primRes)*4.0,0.0,1.0);
        //         }
                
                
        //     //}
            
            
            
        // }
        
        
        
        
        if (globTexEarth.x == TEX_EARTH) {
            globTexEarth.y = mod(
                abs(
                    sin(pos.z/16.0)*
                    sin(pos.x/16.0)*
                    sin(pos.y/16.0)
                ),
                1.0
            );
        }
        
        //globTexEarth.x = TEX_STONE;
        //globTexEarth.y = isGrass;//absTerNorm.z;
        
        
        // res.x = opD(
        //     res.x,
        //     texture2D(Texture2, (pos.xy+pos.yz)*0.5 ).r*0.1
        // );
        
        
        
        // if (res.x < -0.05) {
        //     res.x = MAX_CAM_DIS;
        // }
        
        
        
        //cellVal.xyz += pos;
        
        //res.x += cellVal.w;//pow(1.0-cellVal.w, 0.5)*0.2;
    //}
    
    
    return res;
}




//----------------------------------------------------------------------





float mapWater( vec3 pos ) {
    
    vec2 res = vec2(MAX_CAM_DIS,SKY_ID);
    res = opU(
        res,
        vec2(
            
            
            getWater3D(pos + vec3(
                0.0,
                0.0,
                (waveHeight(pos.xy*1.0) + 1.0) * 0.5 * WAVE_HEIGHT    
            )),
            
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
    
    
    res.x = opS(
      res.x,
      sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) )
    );
    
    return res.x;

}

float mapLand( vec3 pos ) {
    
    int i;
    
    vec2 ov = vec2(0.0,0.5);
    
    vec2 res = vec2(MAX_CAM_DIS,SKY_ID);
    
    float baseRes = texture(Texture0, getTexPos(pos) ).r;
    
    vec3 terNorm = vec3(0.0);
    
    res = opU(
        res,
        vec2(
            getLand3D(pos, baseRes),
            LAND_ID
        )
    );
    
    // float splashTot = 0.0;
    // float splashDis = 0.0;
    // float maxDis = 10.0;
    // float waveMod = clamp(abs(res.x-1.0),0.0,1.0);
    // for (i = 0; i < numSplashes; i++) {
    //     splashDis = distance(splashArr[i].xyz+vec3(0.0,0.0,1.0),pos.xyz);
    //     splashTot += (
            
    //         (sin(splashDis*2.0+splashArr[i].w*0.5-curTime*4.0)+1.0)*0.4
            
    //     ) * 
    //     clamp(1.0-splashDis/maxDis, 0.0, 1.0);
    // }
    // res.x = opD(res.x,splashTot*waveMod*1.5);
    
    
    float splashTot = 0.0;
    float splashDis = 0.0;
    float maxDis = 10.0;
    float waveMod = clamp(abs(res.x-1.0),0.0,1.0);
    for (i = 0; i < numExplodes; i++) {
        splashDis = distance(explodeArr[i].xyz+vec3(0.0,0.0,1.0),pos.xyz);
        splashTot += (
            
            (sin(splashDis*2.0+explodeArr[i].w*0.5-curTime*4.0)+1.0)*explodeArr[i].w*0.5
            
        ) * 
        clamp(1.0-splashDis/maxDis, 0.0, 1.0);
    }
    res.x = opD(res.x,splashTot*waveMod*1.5);
    
    
    if (res.x <= max(SOLID_PREC,SOLID_NORMAL_PREC)*8.0) {
        
        terNorm = normalize(
            vec3(baseRes) - 
            vec3(
                texture(Texture0, getTexPos(pos+ov.yxx) ).r,
                texture(Texture0, getTexPos(pos+ov.xyx) ).r,
                texture(Texture0, getTexPos(pos+ov.xxy) ).r
            )
        );
        
        res = getTerrain(res, terNorm, pos);
    }
    
    
    float multVal = 1.5;
    
    vec2 expRes = vec2(0.0,EXPLODE_ID);
    vec3 expVec = vec3(0.0);
    float sphereRes;
    for (i = 0; i < numExplodes; i++) {
        expRes.x = sdSphere(
            pos-explodeArr[i].xyz,
            explodeArr[i].w + pow(sin(pos.x*multVal)*sin(pos.y*multVal)*sin(pos.z*multVal),2.0)
        );
        expVec += normalize(pos-explodeArr[i].xyz);
        res = opU(res,expRes);
    }
    
    if (res.y == EXPLODE_ID) {
        globTexEarth.x = TEX_EXPLOSION;
        globTexEarth.y = pow(abs(dot(lookAtVec,normalize(expVec))),2.0);
    }
    
    
    
    res.x = opS(
      res.x,
      sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) )
    );
    
    return res.x;

}



float getBrick( vec3 uvwCoords) {
    
    float mv1 = float(mod(uvwCoords.y,2.0) < 1.0);
    float mv2 = float(mod(uvwCoords.z,2.0) < 1.0);
    
    vec3 res = mod(uvwCoords.xyz + vec3(0.5*(mv1+mv2),0.0,0.0), vec3(1.0));
    
    res = abs((res-0.5))*2.0;
    
    return max(max(res.x,res.y),res.z);
}

float getWoodGrain(float normalUID, vec3 worldPosInPixels, float woodRad, float boardDir, float stretchAmount) {
    float woodDiam = woodRad * 2.0;

    vec3 newPos = vec3(0.0);//worldPosInPixels.xyz;

    if (boardDir == 0.0) {
        newPos = worldPosInPixels.xyz;
    }
    if (boardDir == 1.0) {
        newPos = worldPosInPixels.xzy;
    }
    if (boardDir == 2.0) {
        newPos = worldPosInPixels.yxz;
    }
    if (boardDir == 3.0) {
        newPos = worldPosInPixels.yzx;
    }
    if (boardDir == 4.0) {
        newPos = worldPosInPixels.zxy;
    }
    if (boardDir == 5.0) {
        newPos = worldPosInPixels.zyx;
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
    
    float subResult1 = MAX_CAM_DIS;
    float subResult2 = MAX_CAM_DIS;
    
    vec3 res1 = vec3(MAX_CAM_DIS,SKY_ID,MAX_CAM_DIS);
    vec3 res2 = res1;
    
    float uvwDepth = MAX_CAM_DIS;
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
    if (subResult1 == MAX_CAM_DIS) {
        
    }
    else {
        res1.x = opS(res1.x,opD(subResult1,-0.01) ); 
    }
    
    if (subResult2 == MAX_CAM_DIS) {
        
    }
    else {
        res2.x = opS(res2.x,opD(subResult2,-0.01));
    }
    
    //res.x = opD(res.x,0.1);
    
    // globTexPrim.x = TEX_PLASTER; 
    // return res1.x;
    
    // if (res.x == MAX_CAM_DIS) {
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
        
        // if (subResult == MAX_CAM_DIS) {
            
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
            dis2 = texture2D(Texture2, uvwCoords.xy/16.0).r*0.3 + float(pdMatParmas.x > 0.0);
            
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
        
        // cellRes = cell2D(
        //     uvwCoords.xyz,
        //     1.0,
        //     0.1,
        //     0.0,
        //     gradVal,
        //     norVal
        // );
        
        // globTexPrim.y = gradVal;
        
    }
    
    
    
    
    
    
    ///////////////
    
    
    res.x = opS(res.x, getEmpty3D(pos));
        
    
    res.x = opS(
      res.x,
      sdBox(pos-cameraPos, vec3(CAM_BOX_SIZE) ) //CAM_BOX_SIZE
    );
    
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


vec3 roundVal(vec3 p) {
    return floor(p+0.5);
}


struct Ray {
    vec3 Origin;
    vec3 Dir;
};


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



vec2 rayCast( vec3 ro, vec3 rd, vec2 minMaxT, float fNumSteps ) {
    int p = 0;
    int numSteps = int(fNumSteps);
    
    float res = 0.0;
    float t = minMaxT.x;
    
    vec3 pos;
    //float sphereRad = 20.0;
    
    for( p = 0; p < numSteps; p++ ) {
        
        pos = ro+rd*t;
        
        res = mapSolid( pos );
                
        //res = opS(res, sdSphere(pos - rd*(sphereRad), sphereRad ));
        
        if (
            (res < (SOLID_PREC)) ||
            (t>minMaxT.y)
        ) {
            break;
        }
        t += res;
    }
    
    //globCurSteps += float(p);
    
    return vec2(t,res);
}

float rayCastWater( vec3 ro, vec3 rd, vec2 minMaxT, float fNumSteps ) {
    int p = 0;
    int numSteps = int(fNumSteps);
    
    float precis = 0.02;
    float res = 0.0;
    float t = minMaxT.x;
    
    float fp;
    
    for( p = 0; p < numSteps; p++ ) {
        fp = float(p);
        
        res = mapWater( ro+rd*t );
        if (
            (abs(res) < (mix(0.02,0.2,fp/fNumSteps))) ||
            //(abs(res)<precis) ||
            (t>minMaxT.y)
        ) {
            break;
        }
        t += res*0.5 - pow(fp/fNumSteps, 10.0)*0.01;
        //t += res*0.5;
    }
    
    globCurSteps += float(p);
    
    // if (
    //     (p == numSteps) &&
    //     (res > 5.0)
    //     //(distance(ro+rd*t,cameraPos) > (MAX_CAM_DIS*0.9))
    // ) {
    //     t = MAX_CAM_DIS;
    // }
    
    
    
    return t;
}

float rayCastLand( vec3 ro, vec3 rd, vec2 minMaxT, float fNumSteps ) {
    int p = 0;
    int q = 0;
    int numSteps = int(fNumSteps);
    
    float precis = 0.02;
    float res = 0.0;
    float t = minMaxT.x;
    float fp;
    
    for( p = 0; p < numSteps; p++ ) {
        
        fp = float(p);
        
        res = mapLand( ro+rd*t );
        if (
            (abs(res) < (mix(0.02,0.2,fp/fNumSteps))) || // todo: fix this
            (t>minMaxT.y)
        ) {
            break;
        }
        
        t += res*0.5 - pow(fp/fNumSteps, 10.0)*0.01; // todo: fix this also
    }
    
    
    
    // t -= 0.01;
    // for( q = 0; q < 20; q++ ) {
        
    //     res = mapLand( ro+rd*t );
    //     if (
    //         (abs(res) < 0.002) ||
    //         (t>minMaxT.y)
    //     ) {
    //         break;
    //     }
        
    //     t += res*0.5;
    // }
    
    
    globCurSteps += float(p+q);
    
    globLandSteps = float(p);
    
    return t;
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

int intMod(int lhs, int rhs)
{
    return lhs - ( (lhs / rhs) * rhs );
}

float lineStep(
    vec3 ro,
    vec3 rd,
    float maxDis,
    float accuracy
    //,int _startIndex
) {
    
    
    //int startIndex = _startIndex;
    
    globCurSteps = 0.0;
    
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
    
    
    vec2 tval = vec2(0.0);
    vec3 seBoxDis1 = vec3(0.0);
    vec3 seBoxDis2 = vec3(0.0);
    
    vec2 radMod = vec2(0.0);
    
    float j = 0.0;
    
    float camDis;
    float curLOD;
    float minLOD = 0.0;
    float maxLOD = 1.0;
    float btSteps = 0.0;
    
    
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
    
    for (i = 0; i < MAX_PRIM_IDS; i++) {
        primIdList[i] = -1;
        primAlreadyTested[i] = -1.0f;
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
                            
                            //globTest = 1.0;
                            
                            //globTest += 1.0/float(MAX_PRIM_IDS);
                            
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
    
    
    
    if (didHitFinal) {
        
      // if (primIdList[0] == -1) {
      //   tempInt = primIdList[primIdListLength-1];
      //   primIdList[primIdListLength-1] = -1;
      //   primIdList[0] = tempInt;
      // }
      
      camDis = distance(cameraPos,ro+rd*hitBoxFinal.x);
      
      // 1.0 = close, 0.0 = far away
      curLOD = mix(
          maxLOD,
          minLOD,
          clamp(camDis/(clipDist.y*0.5),minLOD,maxLOD) //MAX_DETAIL_DIS
      )*accuracy;
      
      btSteps = mix(MIN_STEPS,MAX_STEPS,curLOD); //*stepMod
      
      tval = rayCast(
          p1,
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
    
    //globTest = 1.0;
    
    
    return MAX_CAM_DIS;
}



vec3 getNormalSolid( vec3 pos )
{
    vec3 eps = vec3( 0.0, 0.0, 0.0 );
    eps.x = SOLID_NORMAL_PREC;
    
    vec3 nor = vec3(
        mapSolid(pos+eps.xyy) - mapSolid(pos-eps.xyy),
        mapSolid(pos+eps.yxy) - mapSolid(pos-eps.yxy),
        mapSolid(pos+eps.yyx) - mapSolid(pos-eps.yyx) );
    return normalize(nor);
}

vec3 getNormalWater( vec3 pos )
{
    vec3 eps = vec3( 0.0, 0.0, 0.0 );
    eps.x = 0.1;
    
    vec3 nor = vec3(
        mapWater(pos+eps.xyy) - mapWater(pos-eps.xyy),
        mapWater(pos+eps.yxy) - mapWater(pos-eps.yxy),
        mapWater(pos+eps.yyx) - mapWater(pos-eps.yyx) );
    return normalize(nor);
}

vec3 getNormalLand( vec3 pos )
{
    vec3 eps = vec3( 0.0, 0.0, 0.0 );
    eps.x = 0.05;
    
    vec3 nor = vec3(
        mapLand(pos+eps.xyy) - mapLand(pos-eps.xyy),
        mapLand(pos+eps.yxy) - mapLand(pos-eps.yxy),
        mapLand(pos+eps.yyx) - mapLand(pos-eps.yyx) );
    return normalize(nor);
}

// float calcAO( vec3 pos, vec3 nor )
// {
//     float occ = 0.0;
//     float sca = 1.0;
//     for( int i=0; i<5; i++ )
//     {
//         float hr = 0.01 + 0.12*float(i)/4.0;
//         vec3 aopos =  nor * hr + pos;
//         float dd = mapSolid( aopos );
//         occ += -(dd-hr)*sca;
//         sca *= 0.95;
//     }
//     return clamp( 1.0 - 3.0*occ, 0.0, 1.0 );    
// }


mat3 setCamera( vec3 ro, vec3 ta, float cr )
{
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}



// float depthToZ(float depth, float n, float f) { // depth is in world units
//     float A = f;//modelview[2].z;
//     float B = n;//modelview[3].z;
//     return 0.5*(-A*depth + B) / depth + 0.5;
// }

void main() {
    
    globLandSteps = 0.0;
    globCurSteps = 0.0;
    globTotSteps = 0.0;
    globIntersect = 0.0;
    globTest = 0.0;
    globTest3 = vec3(0.0);
    globNumPrims = 0;
    primIdListLength = 0;
    
    globWoodDir = 0.0;
    globWoodCoords = vec3(0.0);
    globTexEarth = vec2(TEX_EARTH,0.0);
    globTexPrim = vec2(0.0);
    globTexWater = vec2(TEX_WATER,0.0);
    
    globWaterMod = float(isUnderWater < 0.0)*(1.0-pow(abs(lookAtVec.z),4.0))*0.6;
    
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
    

    float aspect = bufferDim.y/bufferDim.x;
    float NEAR = clipDist.x;
    float FAR = clipDist.y;



    MAX_CAM_DIS = FAR;

    float dx = tan(FOV*0.5f)*(gl_FragCoord.x/(bufferDim.x*0.5)-1.0f)/aspect;
    float dy = tan(FOV*0.5f)*(1.0f-gl_FragCoord.y/(bufferDim.y*0.5));
    
    
    
    //vec3 randV = (randPN(vec3(vec2(dx,dy)*sin(curTime*0.1),1.0))-0.5)*2.0;
    // dx += randV.x*0.002;
    // dy += randV.y*0.002;
    
    
    
    dx = -dx;
    
    vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
    vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);
    
    p1 = modelviewInverse*p1;
    p2 = modelviewInverse*p2;


    vec3 ro = p1.xyz;
    
    
    
    float phi = TexCoord0.x*M_PI*2.0;// - atan(lookAtVec.y,lookAtVec.x) - M_PI;
    float theta = (TexCoord0.y)*M_PI;// - acos(lookAtVec.z);
    
    vec3 rd = vec3(0.0);
    
    
    if (doSphereMap) {
        rd = normalize(vec3(
            cos(phi)*sin(theta),
            sin(phi)*sin(theta),
            cos(theta)
        )
        //+randV*0.01
        );
        
    }
    else {
        rd = normalize(p1.xyz-p2.xyz);
        
        
        
        // FragColor0 = texture(Texture7,newTex.xy);
        // FragColor1 = texture(Texture8,newTex.xy);
        // FragColor2 = texture(Texture9,newTex.xy);
        // FragColor3 = texture(Texture10,newTex.xy);
        // FragColor4 = texture(Texture11,newTex.xy);
        // FragColor5 = texture(Texture12,newTex.xy);
        
        
    }
    
    vec2 newTex = vec2(
        ((atan(-rd.y,-rd.x)/(M_PI))+1.0)/2.0,
        acos(rd.z)/(M_PI)
    );
    
    
    
    

    
    vec3 origMod = ro;// - rd*1000.0;
    vec2 volBounds = aabbIntersect(origMod, rd, volMinReadyInPixels, volMaxReadyInPixels);
    //volBounds.xy += 1000.0;
    
    
    int i;
    int j;
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
    
    vec4 rayCache = vec4(0.0);
    
    float tBase[2];
    float t;
    float landVal = 0.0;
    float solidVal = 0.0;
    float waterVal = 0.0;
    float transVal = 0.0;
    
    float fdiMove = float(depthInvalidMove);
    
    float fi;
    float fj;
    
    vec4 dtexLast = vec4(999999.0);
    vec4 dtexSphere = vec4(999999.0);
    
    vec2 tcl;
    vec2 tcs;
    
    
    
    float cacheMiss = 0.0;
    
    //float subPrec = 0.125;
    
    vec3 solidNorm = vec3(0.0);
    vec3 transNorm = vec3(0.0);
    
    float MIN_CAM_DIS = 0.0;
    
    vec4 v1 = vec4(0.0);
    vec4 v2 = vec4(0.0);
    vec4 v3 = vec4(0.0);
    
    
    
    
    
    if (doSphereMap) {
        tcs = TexCoord0.xy;
        tcl = newTex.xy;
    }
    else {
        tcl = TexCoord0.xy;
        tcs = newTex.xy;
    }
    
    
    
    const int T_RAD = 1;
    
    
    for (j = -T_RAD; j <= T_RAD; j++) {
        fj = float(j);
        for (i = -T_RAD; i <= T_RAD; i++) {
            fi = float(i);
            
            dtexLast = min(
                dtexLast,
                texture(Texture3, tcl.xy + vec2(fi,fj)*
                    1.0*invalidCount / 
                    bufferDim.xy ).xyzw   
            );
            
            if (USE_SPHERE_MAP) {
                dtexSphere = min(
                    dtexSphere,
                    texture(Texture5, tcs.xy + vec2(fi,fj) *
                        4.0*(invalidCount/SPHEREMAP_SCALE_FACTOR) / 
                        bufferDim.xy ).xyzw   
                );
            }
            
        }
    }
    
    
    
    
    
    if (volBounds.x <= volBounds.y) {
        
        //rayStart = ro + rd*volBounds.x;
        
        if (
            all(greaterThanEqual(ro,volMinReadyInPixels)) &&
            all(lessThanEqual(ro,volMaxReadyInPixels))
        ) {
            
        }
        else {
            MIN_CAM_DIS = volBounds.x;
        }
        MAX_CAM_DIS = volBounds.y;
        
        rayCache = vec4(MIN_CAM_DIS);
        
        if (thirdPerson == 0.0) {
            MAX_CAM_DIS = min(MAX_CAM_DIS,FAR);
        }
        
        if (USE_SPHERE_MAP) {
            if (doSphereMap) {
                // rayCache = max(
                //     rayCache,
                //     vec4(
                //         dtexSphere.x - (fdiMove*0.5),
                //         dtexSphere.y - (fdiMove*0.5),
                //         dtexSphere.z - (fdiMove*0.5),
                //         0.0
                //     )
                // );
            }
            else {
                
                // v1 = vec4(
                //     dtexSphere.x - (subPrec + invalidCount*2.0),
                //     dtexSphere.y - (subPrec + invalidCount*2.0),
                //     dtexSphere.z - (subPrec + invalidCount*2.0),
                //     0.0
                // );
                // v2 = vec4(
                //     dtexLast.x - subPrec,
                //     dtexLast.y - subPrec,
                //     dtexLast.z - subPrec,
                //     0.0
                // );
                
                // if (depthInvalidRotate||depthInvalidMove) {
                //     v3 = v1;
                // }
                // else {
                //     v3 = mix(min(v1,v2),max(v1,v2),invalidCount);
                // }
                
                //v3 = v1;
                //v3 = mix(min(v1,v2),v2,invalidCount);
                //v3 = min(v1,v2);
                
                
                
                if (depthInvalidMove) {
                    
                }
                else {
                    rayCache = max(
                        rayCache,
                        vec4(
                            dtexSphere.xyz-vec3(
                                0.001,
                                0.001,
                                WAVE_HEIGHT
                                
                            ),
                            0.0
                        )
                    );
                }
                
                
                
            }
        }
        else {
            rayCache = max(
                rayCache,
                vec4(
                    dtexLast.x - (1.0+invalidCount*2.0),
                    dtexLast.y - (1.0+invalidCount*2.0),
                    dtexLast.z - (1.0+invalidCount*2.0),
                    0.0
                )
            );
        }
        
        
        
        
        
        landVal = MAX_CAM_DIS;
        solidVal = MAX_CAM_DIS;
        waterVal = MAX_CAM_DIS;
        transVal = MAX_CAM_DIS;
        
        // raycache -- x:landVal, y:solidVal, z:waterVal, w:transVal
        
        landVal = rayCastLand(ro,rd,vec2(rayCache.x,MAX_CAM_DIS),EARTH_STEPS*stepMod);
        
        //solidVal = lineStep(ro,rd,min(landVal,MAX_CAM_DIS),1.0); //,-1
        solidNorm = getNormalSolid( ro + solidVal*rd );
        curTexSolid = globTexPrim;
        
        if (placingGeom) {
            primIdList[0] = -1;
            primIdListLength = 1;
            transVal = rayCast(ro, rd, vec2(rayCache.w,solidVal), 50.0 ).x;
            transNorm = getNormalSolid( ro + transVal*rd );
            curTexTrans = globTexPrim;
        }
        
        
        waterVal = rayCastWater(ro,rd,vec2(rayCache.z,min(landVal,solidVal)),WATER_STEPS*stepMod);
        
        
        //solidVal = min(solidVal,transVal);
        
        
        tBase[0] = min(landVal,solidVal);
        tBase[1] = min(waterVal,transVal);
        
        
        
        
        for (i = 0; i < 2; i++) {
            
            t = tBase[i];
            pos = ro + t*rd;
            
            
            
            if (i == 0) {
                
                if (landVal < solidVal) {
                    curTex = globTexEarth;
                    nor = getNormalLand( pos );
                }
                else {
                    curTex = curTexSolid;
                    nor = solidNorm;
                }
                
                if (dot(nor,rd) > 0.0) {
                    nor = -nor;
                }
            }
            else {
                
                if (transVal < waterVal) {
                    curTex = curTexTrans;
                    nor = transNorm;
                    
                }
                else {
                    curTex = globTexWater;
                    nor = getNormalWater( pos );
                }
                
            }
            
            if (
                (t > 0.0) &&
                (t < MAX_CAM_DIS) &&
                //(globTest < 1.0) &&
                //(landVal != MAX_CAM_DIS) && 
                ((curTex.x + curTex.y) > 0.0)
            ) {
                
                if (curTex.x == TEX_WOOD) {
                    curTex.y = getWoodGrain(
                        0.0,//float normalUID,
                        globWoodCoords,//vec3 worldPosInPixels,
                        1.0,//float woodRad,
                        globWoodDir, //float boardDir,
                        4.0 //float stretchAmount    
                    );
                }
                
                
                
                curMat = curTex.x*256.0*255.0 + curTex.y*255.0;
                
                camDis = distance(cameraPos.xyz,pos.xyz);
                zbVal = 1.0-camDis/clipDist.y;
                
            }
            else {
                t = MAX_CAM_DIS;
                nor = vec3(0.0);
                curMat = 0.0;
                pos = vec3(0.0);
                zbVal = 0.0;
            }
            
            cacheMiss = clamp(distance(rayCache.xyz,vec3(landVal,solidVal,waterVal))/50.0,0.0,1.0);
            
            
            if (testOn) {
                fragRes0 =
                
                    vec4(
                        
                        
                        vec3(
                            
                            //
                            //solidVal/MAX_CAM_DIS, //, //40.0,//,
                            //t/MAX_CAM_DIS,// 
                            
                            
                            globCurSteps/(EARTH_STEPS*stepMod),
                            //max(MIN_CAM_DIS,rayCache.x)/MAX_CAM_DIS,
                            
                            //globTest3.x,
                            0.0,//
                            0.0
                            //dtexSphere.w
                            //0.0
                            //dtexSphere.w
                            //cacheMiss
                            //landVal/MAX_CAM_DIS //0.0
                            //distance(dtexSphere.xyz,dtexLast.xyz)/100.0//float(depthInvalid)
                            //globTest//float(primIdListLength)*2.0/float(MAX_PRIM_IDS)//,
                            //
                        )
                        
                        //nor
                        
                        // mod(globTest3,vec3(1.0)).xy,
                        // 0.0
                        
                        // vec3(
                        //     globTest,
                        //     0.0,
                        //     0.0
                        // )
                        
                        //mod(pos,1.0)
                        //(mod(pos,PRIM_DIV)/PRIM_DIV)
                        
                        
                        //(mod(pos,PRIM_DIV)/PRIM_DIV)*(globCurSteps/(MAX_STEPS*2))
                        
                        //(nor.xyz+1.0)*0.5
                        
                        
                        // mix(
                        //     vec3(globSampWater.g),
                        //     vec3(1.0,0.0,0.0),
                        //     float(globSampWater.g == 1.0/UNIT_MAX)
                        // )
                    
                    
                    , 1.0);  
            }
            else {
                fragRes0 =
                //gl_FragData[0+i*2] =
                    vec4(pos.xyz, zbVal);  
            }
            
            fragRes1 = 
            //gl_FragData[1+i*2] =
                vec4(nor.xyz, curMat);
            
            
            if (i == 0) {
                FragColor0 = fragRes0;
                FragColor1 = fragRes1;
            }
            else {
                FragColor2 = fragRes0;
                FragColor3 = fragRes1;
            }
            
        }
        
        
    }
    else {
        
        FragColor0 = vec4(0.0);
        FragColor1 = vec4(0.0);
        FragColor2 = vec4(0.0);
        FragColor3 = vec4(0.0);
        
        
        if (testOn) {
            FragColor0 = vec4(0.0,1.0,0.0,0.0);
        }
    }
    
    // rayCache
    FragColor4 = vec4(landVal,solidVal,waterVal,globCurSteps/(EARTH_STEPS*stepMod));
    FragColor5 = vec4(0.0);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

}










