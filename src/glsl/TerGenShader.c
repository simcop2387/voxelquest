#version 330

uniform float curTime;
uniform float voroSize;
uniform float mapPitch;
uniform float heightMapMaxInCells;
uniform float seaLevel;
uniform vec4 mapFreqs;
uniform vec4 mapAmps;

uniform vec3 bufferDim;
uniform vec3 volMinReadyInPixels;
uniform vec3 volMaxReadyInPixels;
uniform float cellsPerWorld;

uniform bool getVoro;
uniform bool getBlockHolders;

uniform sampler3D Texture0; // E_PL_TERRAIN
//uniform sampler3D Texture1; // E_PL_PRIMIDS

// prim ids
uniform samplerBuffer Texture1; 

// hm fbo
uniform sampler2D Texture2; 

//E_VW_VORO
uniform sampler3D Texture13;

//E_VW_WORLD
uniform sampler3D Texture14;

$


layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vtexcoord;
out vec4 TexCoord0;
void main() {
   TexCoord0 = vtexcoord;
   gl_Position = vposition;
};

$

^INCLUDE:MATERIALS^


const float M_PI = 3.14159265359;
const vec3 crand0 = vec3(12.989, 78.233, 98.422);
const vec3 crand1 = vec3(93.989, 67.345, 54.256);
const vec3 crand2 = vec3(43.332, 93.532, 43.734);

const float LAND_ID = -3.0;
const float WATER_ID = -4.0;
const float SKY_ID = -5.0;
const float EXPLODE_ID = -6.0;



const int NUM_VORO = 20; //13
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
  
    ,vec3(  0.0,  0.0, -1.0  ),
    vec3(  1.0,  0.0, -1.0  ),
    vec3(  0.5,  -1.0, -1.0  ),
    vec3(  -0.5,  -1.0, -1.0  ),
    vec3(  -1.0,  0.0, -1.0  ),
    vec3(  -0.5,  1.0, -1.0  ),
    vec3(  0.5,  1.0, -1.0  )   
  
  
    // vec3(-1.0,-1.0,-1.0),
    // vec3(-1.0,-1.0, 0.0),
    // vec3(-1.0,-1.0, 1.0),
    
    // vec3(-1.0, 0.0,-1.0),
    // vec3(-1.0, 0.0, 0.0),
    // vec3(-1.0, 0.0, 1.0),
    
    // vec3(-1.0, 1.0,-1.0),
    // vec3(-1.0, 1.0, 0.0),
    // vec3(-1.0, 1.0, 1.0),
    
    
    
    // vec3( 0.0,-1.0,-1.0),
    // vec3( 0.0,-1.0, 0.0),
    // vec3( 0.0,-1.0, 1.0),
    
    // vec3( 0.0, 0.0,-1.0),
    // // vec3( 0.0, 0.0, 0.0),
    // vec3( 0.0, 0.0, 1.0),
    
    // vec3( 0.0, 1.0,-1.0),
    // vec3( 0.0, 1.0, 0.0),
    // vec3( 0.0, 1.0, 1.0),
    
    
    
    // vec3( 1.0,-1.0,-1.0),
    // vec3( 1.0,-1.0, 0.0),
    // vec3( 1.0,-1.0, 1.0),
    
    // vec3( 1.0, 0.0,-1.0),
    // vec3( 1.0, 0.0, 0.0),
    // vec3( 1.0, 0.0, 1.0),
    
    // vec3( 1.0, 1.0,-1.0),
    // vec3( 1.0, 1.0, 0.0),
    // vec3( 1.0, 1.0, 1.0)
    
    
);


vec3 randPN(vec3 coBase) {
    
    vec3 co = abs(coBase-8.0);
    
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






float globTest;
float globTexTap;


// float MAX_CAM_DIS;
// float MAX_CAM_VOL_DIS;

// vec2 globTexEarth;
// vec2 globTexPrim;
// vec2 globTexWater;
// float globWoodDir;
// vec3 globWoodCoords;

// int globNumPrims;
// float globWaterMod;
// float globCurSteps;
// float globTotSteps;
// float globIntersect;
// float globTest;
// vec3 globTest3;

// //vec4 testObject;



// vec4 texelRes1;
// vec4 texelRes2;
// vec4 pdVisMin;
// vec4 pdVisMax;
// vec4 pdBoundsMin;
// vec4 pdBoundsMax;
// vec4 pdCornerDis;
// vec4 pdMatParmas;
// vec4 boxDim;
// vec2 boxPower;
// vec3 boxCenterPoint;


in vec4 TexCoord0;
layout(location = 0) out vec4 FragColor0;


const float INVALID = 99999.0;

// >>>>>>>>>>> COMMON <<<<<<<<<<<<<

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

float opI( float d1, float d2 )
{
    return max(d1,d2);
}
float opD( float d1, float d2 )
{
    return d1+d2;
}
vec2 opU( vec2 d1, vec2 d2 )
{
    return mix(d2,d1,float(d1.x<d2.x));
}
vec3 opRep( vec3 p, vec3 c )
{
    return mod(p,c)-0.5*c;
}

float sdSphere( vec3 p, float s )
{
    return length(p)-s;
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

// >>>>>>>>>>> END COMMON <<<<<<<<<<<<<


^INCLUDE:SampleFuncs^

////////////////





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
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
  { 
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
    vec4 centerPos = vec4(ipos.xyz,0.0);
    
    
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
    centerPos = p0;
    centerPos = mix(centerPos, p1, float(p1.w < centerPos.w));
    centerPos = mix(centerPos, p2, float(p2.w < centerPos.w));
    
    
    
    vec3 MAX_MOD = vec3(0.25);// *(sin(curTime/2.0));
    
    vec4 bestPoint = centerPos;
    bestPoint.xyz += randPN(bestPoint.xyz)*MAX_MOD;
    bestPoint.w = distance(pos,bestPoint.xyz);
    
    vec4 nextBestPoint = centerPos;
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




//----------------------------------------------------------------------





^INCLUDE:TerHeightFunc^



float mapWaterMacro( vec3 pos ) {
    
    float wh = 0.0;
    
    return getSeaLevel(pos,wh,1.0);
}

float getNoise(vec3 pos) {
  return (snoise(pos/16.0)*4.0 + snoise(pos/32.0)*8.0 + snoise(pos/64.0)*32.0)/48.0;
}

float opS( float d1, float d2 )
{
    return max(-d2,d1);
}

// float mapLandMacro( vec3 pos ) {
    
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
    
//     vec2 newTC = pos.xy/cellsPerWorld;
    
//     float heightRes = getTerHeight(Texture2,newTC,pos.z).x;//mix(getTerHeight(pos),getTerHeight(mapPos),lerpVal);
    
//     float heightMod = clamp(-heightRes/64.0,0.0,1.0);
    
//     float noiseVal = getNoise(pos);
//     float baseRes = heightRes + noiseVal*256.0*heightMod;// *(1.0-lerpVal);
    
//     float heightMod2 = clamp((-baseRes)/4.0,0.0,1.0);
    
    
    
//     //float baseRes2 = getTerHeight(offsetPos) + getNoise(offsetPos)*128.0;
//     //float randVal = 1.0-clamp((baseRes2-baseRes)*100.0,0.0,1.0);
    
    
//     float tempVal = (gradVal.x)*64.0*float(baseRes < 2.0)*randVal;
    
//     float baseRes3 = (heightRes+8.0) - (gradVal2.x)*32.0*float((pos.z-cellSize2.z*0.0) < cellVal2.z);
//     float tempVal2 = (gradVal.x)*32.0*float(baseRes3 < 2.0)*randVal;
//     float heightMod3 = clamp((-baseRes3)/8.0,0.0,1.0);
    
//     return 
//     //max(
//       //(baseRes+6.0) - (gradVal.x)*32.0*float(baseRes < 2.0)*randVal*(heightMod2)
//       //,
    
//       mix(
//       baseRes3 - tempVal2*heightMod3 - tempVal*heightMod3
//       ,(baseRes+8.0) - tempVal*heightMod2
//       ,clamp( (sin(snoise(pos/43.0)*4.0)+1.0)*0.5,0.0,1.0)
//       )
      
//       //+ getNoise(pos)*256.0*heightMod - (gradVal.x)*32.0*float(baseRes < 2.0)*randVal*(heightMod2)
      
//       // + clamp(1.0 - ((pos.z-cellSize2.z*0.25) - cellVal2.z)/cellSize2.z,0.0,1.0)*32.0
      
//       //,(baseRes+6.0) - *32.0
      
//       //,(heightRes+12.0) - gradVal2.x*64.0*float(heightRes < 0.0)*(1.0-heightMod)
//     //)
//     ;
// }


float seamlessNoise(vec3 pos, float scale) {
  

  float fx = pos.x*scale;
  float fy = pos.y*scale;
  float fz = pos.z*scale;
  float ts = bufferDim.z*scale;


  float testVal = 
    (
    snoise(vec3(fx,   fy,   fz)) *        (ts-fx) *   (ts-fy) * (ts-fz) +
    snoise(vec3(fx-ts,  fy,   fz)) *      (fx) *      (ts-fy) * (ts-fz) +
    snoise(vec3(fx,   fy-ts,  fz)) *      (ts-fx) *   (fy) *    (ts-fz) +
    snoise(vec3(fx-ts,  fy-ts,  fz)) *    (fx) *      (fy) *    (ts-fz) +
    snoise(vec3(fx,   fy,   fz-ts)) *     (ts-fx) *   (ts-fy) * (fz) +
    snoise(vec3(fx-ts,  fy,   fz-ts)) *   (fx) *      (ts-fy) * (fz) +
    snoise(vec3(fx,   fy-ts,  fz-ts)) *   (ts-fx) *   (fy) *    (fz) +
    snoise(vec3(fx-ts,  fy-ts,  fz-ts)) * (fx) *      (fy) *    (fz)
    )/(ts*ts*ts);

    if (isnan(testVal)) {
      testVal = 1.0;
    }
    if (isinf(testVal)) {
      testVal = 1.0;
    }

  return testVal;
}

vec4 genVoro( vec3 pos ) {
    
    
    vec3 cellSize = vec3(16.0);
    
    vec3 gradVal = vec3(0.0);
    vec3 norVal = vec3(0.0);
    vec4 cellVal = cell2D(
        pos,
        cellSize,
        0.1,
        0.3,
        gradVal,
        norVal
    );
    
    if (isnan(gradVal.x)) {
      gradVal.x = 1.0;
    }
    if (isinf(gradVal.x)) {
      gradVal.x = 1.0;
    }
    
    float nv = (seamlessNoise(pos,1.0/128.0)+1.0)*0.5;
    //float nv2 = (seamlessNoise(abs(cellVal.xyz-vec3(64.0)),1.0/64.0)+1.0)*0.5;
    float cellZ = (pos.z-cellVal.z)/(cellSize.z);
    
    return vec4(
      gradVal.x,
      nv,
      (cellZ+0.5),
      //((float(nv2>0.5)-0.5)*2.0*gradVal.x + 1.0)*0.5,
      0.0//nv2
    );
}







//----------------------------------------------------------------------


^INCLUDE:MapLand^




void main() {
    
    globTest = 0.0;
    globTexTap = 0.0;
    
    ivec2 iCoords = ivec2(TexCoord0.xy*bufferDim.xy);//-ivec2(1);
    
    int totIC = iCoords.x + iCoords.y*int(bufferDim.x);
    
    float fVolPitch = bufferDim.z;
    int iVolPitch = int(fVolPitch);
    
    
    int z = totIC/(iVolPitch*iVolPitch);
    totIC -= z*(iVolPitch*iVolPitch);
    
    int y = totIC/iVolPitch;
    totIC -= y*iVolPitch;
    
    int x = totIC;
    
    
    //vec3 pos = vec3(x,y,z) + volMinReadyInPixels + 0.5;
    
    vec3 pos = mix(volMinReadyInPixels, volMaxReadyInPixels, (vec3(x,y,z)+0.5)/fVolPitch );
    
    vec4 finalRes = vec4(0.0);
    
    //float watVal = mapWaterMacro( pos );
    float terVal = 0.0;
    
    
    if (getVoro) {
      finalRes = genVoro(pos);
    }
    else {
      
      terVal = getTerVal(pos, 0.0, getBlockHolders).x;
      
      if (getBlockHolders) {
        
        finalRes = vec4(
            terVal,
            0.0, //terVal
            0.0,
            0.0
            // float(watVal<0.25),
            // watVal
        );
      }
      else {
        
        finalRes = vec4(
            float(terVal < -1.0),
            0.0, //terVal
            0.0,
            0.0
            // float(watVal<0.25),
            // watVal
        );
      }
      
      
      // if (terVal < threshVal) {
      //   if (
      //     (
      //       (mapLandMacro( pos + vec3(1.0,0.0,0.0) ) >= threshVal) &&
      //       (mapLandMacro( pos + vec3(-1.0,0.0,0.0) ) >= threshVal)
      //     ) || 
      //     (
      //       (mapLandMacro( pos + vec3(0.0,1.0,0.0) ) >= threshVal) &&
      //       (mapLandMacro( pos + vec3(0.0,-1.0,0.0) ) >= threshVal)
      //     ) ||
      //     (
      //       (mapLandMacro( pos + vec3(0.0,0.0,1.0) ) >= threshVal) &&
      //       (mapLandMacro( pos + vec3(0.0,0.0,-1.0) ) >= threshVal)
      //     )
      //   ) {
      //     terVal = threshVal*2.0;
      //   }
      // }
      
      
    }
    
    
    
    FragColor0 = finalRes;
}










