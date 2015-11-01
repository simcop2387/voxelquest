#version 120


uniform bool testOn;
uniform float curTime;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform vec3 lookAtVec;
uniform mat4 modelviewInverse;
uniform float FOV;
uniform vec2 clipDist;
uniform vec3 lightVec;

varying vec2 TexCoord0;

varying vec4 rayStart;
varying vec4 rayEnd;

uniform sampler2D Texture0;



const float MAX_CAM_DIS = 20.0;
const float MAX_DF_STEPS = 10.0;
const float MAX_STEPS = MAX_DF_STEPS*3.0;

const float MAX_BOX_DIS = 0.03;
const int BOX_TEST_STEPS = 6;

const float CELL_SIZE = 1.0;
const vec3 skyCol = vec3(0.55, 0.7, 0.8);
const vec3 crand0 = vec3(12.989, 78.233, 98.422);
const vec3 crand1 = vec3(93.989, 67.345, 54.256);
const vec3 crand2 = vec3(43.332, 93.532, 43.734);

const vec3 voroOffsets[8] = vec3[](
    vec3(  -1.0,  -1.0,  -1.0  ),
    vec3(  -1.0,  -1.0,  1.0  ),
    vec3(  -1.0,  1.0,  -1.0  ),
    vec3(  -1.0,  1.0,  1.0  ),
    vec3(  1.0,  -1.0,  -1.0  ),
    vec3(  1.0,  -1.0,  1.0  ),
    vec3(  1.0,  1.0,  -1.0  ),
    vec3(  1.0,  1.0,  1.0  )
    
);

vec3 rand(vec3 co) {
    
    vec3 myres = vec3(
        dot(co.xyz, crand0),
        dot(co.zyx, crand1),
        dot(co.yxz, crand2)
    );
    
    return fract(sin(myres) * 43758.8563);
}
float randf3(vec3 co) {
    return fract(sin(dot(co.xyz , crand0)) * 43758.8563);
}

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
    
    
}

$



//vec3 globP1;
//vec3 globP2;
vec3 globCenterPoint;
vec4 globBoxDim;
float globCurSteps;
float globTotSteps;
float globTotLS;
vec2 globBoxPower;



vec4 getBoxDim(float seed) {
    return vec4(
        vec3(
            1.0 + cos( seed*1.1 ),
            1.0 + sin( seed*3.2 ),
            1.0 + sin( seed*2.3 )
        )*CELL_SIZE/20.0,
        CELL_SIZE*( 1.5 + cos( seed*0.7 ) )/10.0
    );
}

vec2 getBoxPower(float seed) {
  return vec2(
    mix(4.0,0.75,(1.0+sin(seed*1.3))*0.5),
    mix(4.0,0.75,(1.0+sin(seed*2.5))*0.5)
  );
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


float sdPlane( vec3 p ) {
    return p.z;
}

float sdTer( vec3 p )
{
    
    vec4 tex3 = texture2D(Texture0, p.xy/400.0 + 0.3);
    vec4 tex0 = texture2D(Texture0, p.xy/70.0 + 0.2);
    vec4 tex1 = texture2D(Texture0, p.xy/4.0 + 0.1);
    vec4 tex2 = texture2D(Texture0, p.xy*4.0);
    
    return (p.z-(
        tex3*16.0 + tex0.x*4.0 + tex1*0.2 + tex2*0.01   
    ))*0.25;
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



float length8( vec2 p )
{
    p = p*p; p = p*p; p = p*p;
    return pow( p.x + p.y, 1.0/8.0 );
}




float sdCone( vec3 p, vec3 c )
{
    vec2 q = vec2( length(p.xz), p.y );
    float d1 = -p.y-c.z;
    float d2 = max( dot(q,c.xy), p.y);
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.0);
}


vec4 getUVW(
    vec3 pos,
    vec3 centerOffset, 
    vec3 innerBoxSize,
    float cornerRad    
) {
    
    vec3 newOffset = max(abs(centerOffset)-innerBoxSize,0.0);
    vec2 newNorm1 = normalize(newOffset.xy);
    vec2 newNorm2 = normalize(vec2(length(newOffset.xy), newOffset.z));
    vec3 uvwCoords = pos.xyz;
    
    //vec3 innerBoxSizeFloored = floor(innerBoxSize*8.0)/8.0;
    
    vec3 centerOffsetNorm = abs(centerOffset/innerBoxSize);
    
    // x side
    if (newNorm1.x == 0.0) {      
        uvwCoords.x = pos.x;
        uvwCoords.y = pos.z;
        
        //uvwCoords.x *= innerBoxSizeFloored.x;
    }
    
    // y side
    if (newNorm1.y == 0.0) {
        uvwCoords.x = pos.y;
        uvwCoords.y = pos.z;
        
        //uvwCoords.x *= innerBoxSizeFloored.y;
    }
    
    float curPhi = atan(newNorm1.y, newNorm1.x);
    float curThe = atan(newNorm2.y, newNorm2.x);
    
    // side corner
    if (newNorm1.x*newNorm1.y != 0.0) {
        uvwCoords.x = curPhi*cornerRad;
        uvwCoords.y = pos.z;
    }
    
    // top corner
    if (newNorm2.x*newNorm2.y != 0.0) {
        //uvwCoords.x = curPhi*cornerRad;
        uvwCoords.y = curThe*cornerRad;
    }
    
    // top
    if (newNorm2.x == 0.0) {
        if (centerOffsetNorm.x > centerOffsetNorm.y) {
            uvwCoords.x = pos.y;
            uvwCoords.y = pos.x;
            
            if (centerOffset.x > 0.0) {
                uvwCoords.y *= -1.0;
            }
        }
        else {
            uvwCoords.x = pos.x;
            uvwCoords.y = pos.y;
            
            if (centerOffset.y > 0.0) {
                uvwCoords.y *= -1.0;
            }
        }
        
        // if (centerOffset.x < 0.0) {
        //     uvwCoords.x *= -1.0;
        // }
        
        
        
        
    }
    
    uvwCoords.z = length(centerOffset);
    
    return vec4(uvwCoords,newNorm2.x);
}

float udRoundBox2( vec3 centerPoint, vec3 vectorFromCenter, vec4 boxDim ) //, float pVert, float pHorz
{
    
    // float rayDis = pointSegDistance(centerPoint,globP1,globP2);
    
    // if (rayDis > length(boxDim+r)*2.0) {
    //     return 1.0;
    // }
    
    return (length(max(abs(vectorFromCenter)-boxDim.xyz,0.0))-boxDim.w);
}

float udRoundBox( vec3 centerPoint, vec3 vectorFromCenter, vec4 boxDim, vec2 boxPower )
{
    vec3 newP = abs(max(abs(vectorFromCenter)-boxDim.xyz,0.0));
    
    //float pVert, float pHorz
    //vec2 pv1 = vec2(pHorz);
    //vec2 pv2 = vec2(pVert);
    
    newP.xy = pow(newP.xy, boxPower.xx );
    newP.x = pow( newP.x + newP.y, 1.0/boxPower.x );
    
    newP.xz = pow(newP.xz, boxPower.yy );
    newP.x = pow( newP.x + newP.z, 1.0/boxPower.y );
    
    
    return (newP.x-boxDim.w)*0.5;

}

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

float sdTriPrism( vec3 p, vec2 h )
{
    vec3 q = abs(p);
    float d1 = q.z-h.y;
    float d2 = max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5;
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.0);
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

float opS( float d1, float d2 )
{
    return max(-d2,d1);
}

float opD( float d1, float d2 )
{
    return d1+d2;
}

vec4 opU( vec4 d1, vec4 d2 )
{
    return (d1.x<d2.x) ? d1 : d2;
}

vec3 opRep( vec3 p, vec3 c )
{
    return mod(p,c)-0.5*c;
}

vec3 opRepAlt( vec3 p, vec3 c )
{
    float yv = mod(p.y,c.y)-0.5*c.y;
    yv = p.y-yv;
    
    vec3 res = vec3(yv*0.5,0.0,0.0);
    
    return mod(p+res,c)-0.5*c;
}

vec3 opTwist( vec3 p )
{
    float  c = cos(10.0*p.y+10.0);
    float  s = sin(10.0*p.y+10.0);
    mat2   m = mat2(c,-s,s,c);
    return vec3(m*p.xz,p.y);
}

float opBlend( float a, float b )
{
    float k = 8.0;
    a = pow( a, k ); b = pow( b, k );
    return pow( (a*b)/(a+b), 1.0/k );
}


vec2 cell3D(vec3 P) {
    // source: http://github.com/BrianSharpe/Wombat/blob/master/Cellular3D.glsl

    //  establish our grid cell and unit position
    vec3 Pi = floor(P);
    vec3 Pf = P - Pi;

    // clamp the domain
    Pi.xyz = Pi.xyz - floor(Pi.xyz * ( 1.0 / 69.0 )) * 69.0;
    vec3 Pi_inc1 = step( Pi, vec3( 69.0 - 1.5 ) ) * ( Pi + 1.0 );

    // calculate the hash ( over -1.0->1.0 range )
    vec4 Pt = vec4( Pi.xy, Pi_inc1.xy ) + vec2( 50.0, 161.0 ).xyxy;
    Pt *= Pt;
    Pt = Pt.xzxz * Pt.yyww;
    const vec3 SOMELARGEFLOATS = vec3( 635.298681, 682.357502, 668.926525 );
    const vec3 ZINC = vec3( 48.500388, 65.294118, 63.934599 );
    
    const float maxOff = 1.5;
    const float maxOff2 = maxOff*2.0;
    
    vec3 lowz_mod = vec3( 1.0 / ( SOMELARGEFLOATS + Pi.zzz * ZINC ) );
    vec3 highz_mod = vec3( 1.0 / ( SOMELARGEFLOATS + Pi_inc1.zzz * ZINC ) );
    vec4 hash_x0 = fract( Pt * lowz_mod.xxxx ) * maxOff2 - maxOff;
    vec4 hash_x1 = fract( Pt * highz_mod.xxxx ) * maxOff2 - maxOff;
    vec4 hash_y0 = fract( Pt * lowz_mod.yyyy ) * maxOff2 - maxOff;
    vec4 hash_y1 = fract( Pt * highz_mod.yyyy ) * maxOff2 - maxOff;
    vec4 hash_z0 = fract( Pt * lowz_mod.zzzz ) * maxOff2 - maxOff;
    vec4 hash_z1 = fract( Pt * highz_mod.zzzz ) * maxOff2 - maxOff;

    //  generate the 8 point positions
    const float JITTER_WINDOW = 0.166666666;  // 0.166666666 will guarentee no artifacts.
    hash_x0 = ( ( hash_x0 * hash_x0 * hash_x0 ) - sign( hash_x0 ) ) * JITTER_WINDOW + vec4( 0.0, 1.0, 0.0, 1.0 );
    hash_y0 = ( ( hash_y0 * hash_y0 * hash_y0 ) - sign( hash_y0 ) ) * JITTER_WINDOW + vec4( 0.0, 0.0, 1.0, 1.0 );
    hash_x1 = ( ( hash_x1 * hash_x1 * hash_x1 ) - sign( hash_x1 ) ) * JITTER_WINDOW + vec4( 0.0, 1.0, 0.0, 1.0 );
    hash_y1 = ( ( hash_y1 * hash_y1 * hash_y1 ) - sign( hash_y1 ) ) * JITTER_WINDOW + vec4( 0.0, 0.0, 1.0, 1.0 );
    hash_z0 = ( ( hash_z0 * hash_z0 * hash_z0 ) - sign( hash_z0 ) ) * JITTER_WINDOW + vec4( 0.0, 0.0, 0.0, 0.0 );
    hash_z1 = ( ( hash_z1 * hash_z1 * hash_z1 ) - sign( hash_z1 ) ) * JITTER_WINDOW + vec4( 1.0, 1.0, 1.0, 1.0 );

    //  return the closest squared distance
    vec4 dx1 = Pf.xxxx - hash_x0;
    vec4 dy1 = Pf.yyyy - hash_y0;
    vec4 dz1 = Pf.zzzz - hash_z0;
    vec4 dx2 = Pf.xxxx - hash_x1;
    vec4 dy2 = Pf.yyyy - hash_y1;
    vec4 dz2 = Pf.zzzz - hash_z1;
    vec4 d1 = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
    vec4 d2 = dx2 * dx2 + dy2 * dy2 + dz2 * dz2;


    d1 = min(d1, d2);
    d1.xy = min(d1.xy, d1.wz);
    
    float finalRes = min(d1.x, d1.y); // return a value scaled to 0.0->1.0
    float gradVal = clamp((1.0 - (finalRes * 2.0 / (d1.x + d1.y))), 0.0, 1.0);

    return vec2(finalRes,gradVal) * ( 9.0 / 12.0 );

}

void getCobble(vec3 pos) {


    vec4 myResult = vec4(0.0);
    vec3 curPos;
    vec3 bestPos = vec3(0.0);
    vec3 nextBestPos = vec3(0.0);
    float curVoroDis;
    float notValid = 99999.0;
    float minDis1 = notValid;
    float minDis2 = notValid;
    float gradVal;
    vec3 randNum = vec3(0.0);
    vec3 oneVec = vec3(1.0);
    float bestRand;

    int i;
    
    
    //vec2 roughPos = (pos.xy + pos.yz) / cellsPerWorld;
    //float roughVoro = 0.5;//abs(texture2D(Texture2, 256.0 * roughPos ).r);

    float tempr = 0.0;

    float vsize = 4.0;

    vec3 newWP = floor(pos.xyz/vsize)*vsize + vsize/2.0;
    
    vec3 newWP2 = pos.xyz;// + clamp((roughVoro-0.5)*8.0,-1.0,1.0)*0.5;

    float x;
    float y;
    float z;

    for (i = 0; i < 8; i++) {
        
        
        curPos = newWP + voroOffsets[i]*vsize/2.0;
        tempr = rand(curPos);
        curPos += (tempr-0.5)*vsize;
        
        curVoroDis = distance(curPos.xyz, newWP2)*(1.0+tempr*0.5);

        if (curVoroDis < minDis1) {
            nextBestPos = bestPos;
            bestPos = curPos.xyz;

            minDis2 = minDis1;
            minDis1 = curVoroDis;
        }
        else {
            if (curVoroDis < minDis2) {
                nextBestPos = curPos.xyz;
                minDis2 = curVoroDis;
            }
        }
    }

    // bestRand = randf3(bestPos);
    // gradVal = (clamp(1.0 - (minDis1 * 2.0 / (minDis1 + minDis2)), 1.0 / 255.0, 1.0));
    // voroId = floor(bestRand * 120.0) + 1.0;
    // voroGrad = gradVal;
    // voroPos = bestPos;

}

// vec4 getTerrain(vec3 worldPos) {

//     vec4 matResult = vec4(0.0);

//     float newMult = 1.0/128.0;

//     float finalMat = 0.0;
//     float finalNormUID = 0.0;
//     float finalMod = 0.0;

//     float rockSize = distance(worldMinBufInPixels.xyz,worldMaxBufInPixels.xyz)/2.0;

//     float baseRand = randf(worldPos.xy);

//     float grassMod = float(worldPos.z >= (seaLevel) );

//     float resMod = 0.25;

//     float randv = baseRand;
//     vec3 newCellsPerBlock = blockMaxBufInPixels.xyz - blockMinBufInPixels.xyz;
//     vec3 voroCoords = (voroPos - blockMinBufInPixels.xyz) / newCellsPerBlock;
//     vec4 texVoro = texture3D(Texture4, voroCoords);
//     float randVoro = randf(voroCoords.xy);
//     vec3 wCoords = (worldPos - blockMinBufInPixels.xyz) / newCellsPerBlock;
//     vec4 texW = texture3D(Texture4, wCoords);
    
    
//     vec2 roughPos = (worldPos.xy + worldPos.yz) * 0.25 / cellsPerWorld;
    
//     vec2 roughVal = vec2(
//                                         abs(texture2D(Texture2, 512.0 * roughPos+0.01*randVoro*voroGrad ).r),
//                                         abs(texture2D(Texture2, 256.0 * roughPos+0.015*randVoro*voroGrad ).r)
//                                     );
    
    
//     float fIsSnow = 0.0;
//     float snowRand = roughVal.x;
    
//     vec3 wCoords2 = (worldPos +
//                                      vec3(
//                                          0.25,
//                                          0.25,
//                                          -mix(randv,snowRand,fIsSnow)
//                                      ) - blockMinBufInPixels.xyz) / newCellsPerBlock;
//     vec4 texW2 = texture3D(Texture4, wCoords2);
//     vec4 texW3 = texture3D(Texture4, 
//         (worldPos + vec3(0.5,0.5,-0.5) - blockMinBufInPixels.xyz) / newCellsPerBlock    
//     );
    
    
//     roughVal = -abs(roughVal - 0.5) * 2.0;
//     float hardVal = clamp((smoothVal-0.5)*4.0, 0.0, 1.0);

//     float tempRand = randf3(voroPos);
//     float jutDis = clamp(
//             ( distance(worldPos.xyz,vec3(voroPos.xy,voroPos.z-rockSize)) ) / (2.0 * rockSize),
//             0.0,
//             1.0
//         );
    
//     float voroJut = mix(
//         1.0 - jutDis,
//         0.0,
//         roughVal.y  
//     );

//     roughVal = 
//             mix(
//             roughVal * clamp( (1.0 - voroGrad), 0.0, 1.0),
//             roughVal,
//             randVoro * 0.25 + 0.75
//         );
    
    
//     roughVal *= resMod;
//     voroJut *= resMod;
    
//     float multVal = 0.3
//     float multVal2 = 0.6
    

//     float newTWA = pow(texW.a, multVal)*multVal2;
//     float newTWA2 = pow(texW2.a, multVal)*multVal2;
//     float newTWA3 = pow(texW3.a, multVal)*multVal2;
//     float newMedian = 0.5
//     float compScale = 0.4

//     float threshVal = 
//         newMedian +
//         (tempRand * 0.05 + 0.6 * voroJut)*compScale;
    
    
//     float threshVal2 = 
//         newMedian +
//         (
//             (tempRand * 0.01) -
//             (roughVal.x*0.8 - ((1.0 - voroGrad) * 0.25 + roughVal.y) * 0.1)
//         )*compScale;
    
//     float threshVal3 = newMedian +
//         (
//         (voroJut*0.5) - 
//         (roughVal.x * (0.4 + roughVal.y*0.2)) -
//         smoothVal * 0.1
//         )*compScale;
    
//     float threshVal4 = newMedian +
//         (-roughVal.x*0.4)*compScale;
        
    
    
    
    
    
//     // if (grassMod == 1.0) {
        
//     //     if (fIsSnow == 1.0) {
//     //         if (
//     //             (newTWA2*@snowVal@*4.0 - threshVal2) > mix(-0.5,0.05,newMult)*compScale     // GRASS 
//     //         ) {
//     //             finalNormUID = 2.0;
//     //             finalMat = TEX_SNOW;
//     //             finalMod = 1.0;
//     //         }
//     //     }
//     //     else {
//     //         if (
//     //             (newTWA2 - threshVal2) > mix(-0.5,0.05,newMult)*compScale       // GRASS 
//     //         ) {
//     //             finalNormUID = 2.0;
//     //             finalMat = TEX_GRASS;
//     //             finalMod = clamp((newTWA2 - threshVal2) * 10.0, 0.0, 1.0) * 0.25 + 0.5;
//     //         }
//     //     }
        
//     // }
    
//     if (
//         (newTWA3 - threshVal4) > (mix(
//             mix(-0.5,0.12,newMult)
//             ,0.0,hardVal)*compScale ) ||
//         (newTWA - threshVal4) > mix(-0.5,0.12,newMult)*compScale
        
//                 // SAND
//     ) {
//         finalNormUID = 2.0;
//         finalMat = TEX_EARTH;
//         finalMod = 0.0;
//     }

    

//     // ROCKS
//     if (
//         ( texVoro.a > 0.0 ) &&
//         (
//             ( (newTWA - threshVal3) > mix(-0.2,0.0,newMult) )
            
//             &&
            
//             ( voroGrad > (roughVal.y*0.1 + mix(0.2*newMult,-0.2,randVoro)*abs(jutDis-0.5)*2.0 ) ) //
            
//         )
        
        
//     ) {
//         finalNormUID = voroId;
//         finalMat = TEX_EARTH;
//         finalMod = voroGrad;
//     }


//     matResult.x = finalNormUID;
//     matResult.y = finalMat;
//     matResult.z = 0.0;
//     matResult.w = finalMod;

//     return matResult;


// }









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
    

    float numScales = 2.0;
    float finalDis = 0.0;

    vec2 iUV = ivec2(uvwBase.xy * numScales);
    vec2 dis = uvwBase.xy * numScales - floor(uvwBase.xy * numScales);

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




//----------------------------------------------------------------------


vec4 getSE(vec3 pos, bool getColor) {
    
    int i;
    
    vec3 centerPos = globCenterPoint;
    vec3 repPos = pos;
    
    vec2 cellVal = vec2(1.0);
    
    vec3 centerOffset;
    vec3 waterColor = vec3(0.0,0.5,1.0);
    vec3 terColor = vec3(0.0);
    vec3 torColor = vec3(0.0);
    vec3 houseColor = vec3(0.0);
    float disVal = 0.0;
    vec4 uvw = vec4(0.0);
    
    
    float terMod = clamp((pos.z-8.0)/1.0,0.0,1.0);
    
    if (getColor) {
        
        terColor = mix(vec3(0.3,0.4,0.3),vec3(0.5,0.5,0.6),terMod);
        
        //float f = mod( floor(10.0*pos.y + sin(pos.x*2.0)) + floor(10.0*pos.x + sin(pos.y*2.0)), 2.0);
        // planeColor = abs(mod(pos.zxy*0.03,vec3(1.0))-0.5) + 0.1*f;
        // planeColor.rgb = vec3( (planeColor.g+planeColor.b)*0.5 );
        
        // torColor = vec3(0.5,0.0,0.0);
    }
    vec4 res = vec4(MAX_CAM_DIS,skyCol);
    
    if (
        (distance(cameraPos, pos) >= (MAX_CAM_DIS-1.0))
    ) {
        return res;
    }
    
    // res = opU(
    //     res,
    //     vec4(
    //         opD(
    //             sdTer(pos),
    //             - (1.0-cell3D(pos*8.0))*(1.0-terMod)/10.0
    //         ),
    //         terColor
    //     )
    // );
    
    // res = opU(
    //     res,
    //     vec4(
    //         opD(
    //             sdPlane(pos-6.5),
    //             0.0//sin(curTime+pos.x*50.0)*sin(curTime+pos.y*40.0)*0.02
    //         ),
    //         waterColor
    //     )
    // );
    
    
    centerOffset = repPos-centerPos;
    uvw = getUVW(repPos, centerOffset, globBoxDim.xyz, globBoxDim.w);
    uvw.x += curTime*0.25;
    
    if (uvw.w >= 0.999) {
        cellVal = cell3D(pos*16.0);
        
        disVal = (
            (1.0-cellVal.y)
            //+ texture2D(Texture0, (uvw.xy)*1.0)*1.0
        )*0.02;
        // - (1.0-cell3D(pos*8.0))/20.0;
        //getRidge2(uvw.xy*4.0)*0.05;
    }
    else {
        disVal = getShingle(uvw.xy*16.0)*0.02;
    }
    
    if (getColor) {  
        if (uvw.w >= 0.999) {
            houseColor = vec3(0.6,0.5,0.4);
        }
        else {
            houseColor = mix(
                (cos(centerPos*2.33)+1.0)*0.3,
                (sin(centerPos*3.44)+1.0)*0.1,
                pow(disVal*50.0,2.0)
            );
            //vec3(0.2,0.5,0.4) + vec3(5.0,0.0,20.0)*disVal;
            
        }
    }
    
    res = opU(
        res,
        vec4(
            //opS
            (
                opD(
                    udRoundBox(
                      centerPos,
                      centerOffset,
                      globBoxDim,
                      globBoxPower
                    ),
                    disVal
                )
                //,sdCylinder(pos-(centerPos),vec2(0.1,3.0))
                //,sdBox(pos-cameraPos, vec3(0.75) )
            ),
            houseColor*mix(0.5,1.0,cellVal.y)
        )
        
    );
    
    
    
    return res;
}


float map( vec3 pos ) {
    return getSE(pos, false).x;
}

vec4 mapWithCol( vec3 pos ) {
    return getSE(pos, true);
}

vec3 roundVal(vec3 p) {
    return floor(p+0.5);
}


struct Ray {
    vec3 Origin;
    vec3 Dir;
};


vec3 aabbIntersect(vec3 rayOrig, vec3 rayDir, vec3 minv, vec3 maxv) {
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
    return vec3(t0,t1,float(t0 <= t1));
}

// vec2 aabbIntersect2(vec2 rayOrig, vec2 rayDirInv, vec2 minv, vec2 maxv) {//box b, ray r) {
//     float tx1 = (minv.x - rayOrig.x)*rayDirInv.x;
//     float tx2 = (maxv.x - rayOrig.x)*rayDirInv.x;
 
//     float tmin = min(tx1, tx2);
//     float tmax = max(tx1, tx2);
 
//     float ty1 = (minv.y - rayOrig.y)*rayDirInv.y;
//     float ty2 = (maxv.y - rayOrig.y)*rayDirInv.y;
 
//     tmin = max(tmin, min(ty1, ty2));
//     tmax = min(tmax, max(ty1, ty2));
 
//     return vec2(tmin, tmax);
// }





float rayCast( vec3 ro, vec3 rd, vec2 minMaxT, float fNumSteps ) {
    int p;
    int p2;
    
    
    float precis = 0.002;
    
    int numSteps = int(fNumSteps);
    
    //globP1 = ro + rd*200.0;
    //globP2 = ro - rd*200.0;
    
    float res = 0.0;
    
    float t = minMaxT.x;
    
    for( p = 0; p < numSteps; p++ ) {
        
        res = map( ro+rd*t );
        if (
            (res<precis) ||
            (t>minMaxT.y)
        ) {
            break;
        }
        t += res;
    }
    
    float stepVal = 0.0005;
    t = t - stepVal*fNumSteps*0.5;
    
    for (p2 = 0; p2 < numSteps; p2++) {
        
        res = map( ro+rd*(t+stepVal) );
        
        t += stepVal;
        
        if (res < -0.00001) {
            break;
        }
    }
    
    globCurSteps += float(p+p2);
    
    return t;
}

float pointBoxDis(vec3 rectMin, vec3 rectMax, vec3 p) {
    
    vec3 res0 = vec3(0.0);
    vec3 res1 = vec3(rectMin-p);
    vec3 res2 = vec3(p-rectMax);
    vec3 res3 = max(max(res0,res1),res2);
    return length(res3);
}

float lineStep(vec3 ro, vec3 rd, float maxCells, float accuracy) {
    globCurSteps = 0.0;
    
    vec3 oneVec = vec3(1.0);
    float seed = 0.0;
    
    vec3 p1 = (ro);    
    vec3 p2 = (ro + rd*MAX_CAM_DIS/CELL_SIZE);
    vec3 d = p2-p1;
    vec3 dabs = abs(d);
    int i;
    int n = int(
        min(
            max(max(dabs.x,dabs.y),dabs.z),
            maxCells
        )
    );
    
    vec3 s = d/float(n);
    vec3 centerPoint = vec3(0.0);
    vec3 hitBox = vec3(0.0);

    // todo: wont show at origin
    vec3 lastPoint = vec3(0.0);

    vec3 p = p1;
    
    vec3 hitPoint1;
    vec3 hitPoint2;
    
    float tval = 0.0;
    float seBoxDis = 0.0;
    
    vec2 diag = normalize(vec2(1.0,1.0));
    
    vec3 curPoint;
    
    
    int j;
    float tt;
    
    vec3 testSeg;
    
    vec2 minMaxT = vec2(0.0);
    
    for (i = 0; i < n; i++) {
        
        centerPoint = roundVal(p)*CELL_SIZE;
        
        if (all(equal(centerPoint,lastPoint)) ) {
            
        }
        else {
            seed = curTime+dot(centerPoint,oneVec);
            globBoxPower = getBoxPower(seed);
            globBoxDim = getBoxDim(seed);
            hitBox = aabbIntersect(p1,rd,centerPoint-globBoxDim.xyz-globBoxDim.w, centerPoint+globBoxDim.xyz+globBoxDim.w);


            if (hitBox.z > 0.0) {
                globCenterPoint = centerPoint;
                
                
                tt = hitBox.x;
                for (j = 0; j < BOX_TEST_STEPS; j++) {
                    curPoint = (ro+rd*tt);
                    seBoxDis = udRoundBox( centerPoint, curPoint-centerPoint, globBoxDim, globBoxPower );
                    tt += seBoxDis;
                }
                minMaxT.x = tt;
                minMaxT.y = hitBox.y;
                
                
                // hitPoint1 = ro + hitBox.x*rd;
                // hitPoint2 = ro + hitBox.y*rd;
                
                
                
                
                // hitPoint1 = max((abs(hitPoint1 - centerPoint)),0.0);
                // hitPoint2 = max((abs(hitPoint2 - centerPoint)),0.0);
                
                //(length(max(abs(vectorFromCenter)-boxDim.xyz,0.0))-boxDim.w);
                
                // hitPoint1.xyz = ((max(abs(hitPoint1.xyz-centerPoint.xyz)-globBoxDim.xyz,0.0)))/globBoxDim.w;
                // hitPoint2.xyz = ((max(abs(hitPoint2.xyz-centerPoint.xyz)-globBoxDim.xyz,0.0)))/globBoxDim.w;
                
                // hitPoint1.x = length(hitPoint1.xy);
                // hitPoint1.y = length(hitPoint1.xz);
                
                // hitPoint2.x = length(hitPoint2.xy);
                // hitPoint2.y = length(hitPoint2.xz);
                
                // seBoxDis = float(
                //     //(hitPoint1.x >= 0.9999) &&
                //     //(hitPoint2.x >= 0.9999)
                //     // (hitPoint1.y >= 0.9999) &&
                //     // (hitPoint2.y >= 0.9999)
                    
                //     (hitBox.y-hitBox.x) > (globBoxDim.w/sqrt(2.0))
                    
                // );// *float(hitBox.y-hitBox.x < globBoxDim.w/sqrt(2.0) ); 
                
                
                // (
                //     udRoundBox2( centerPoint, hitPoint1-centerPoint, globBoxDim )/globBoxDim.w
                //     //+ udRoundBox2( centerPoint, hitPoint2-centerPoint, globBoxDim )/globBoxDim.w    
                // );
                //*float(hitBox.y-hitBox.x < globBoxDim.w*sqrt(2.0) ); 
                
                // (
                //     distance(hitPoint1,min(hitPoint1,globBoxDim.xyz) ) +
                //     distance(hitPoint2,min(hitPoint2,globBoxDim.xyz) )
                // );//max(length(hitPoint1),length(hitPoint2));
                //length(hitPoint1.xyz);
                //float(hitBox.y-hitBox.x < globBoxDim.w);
                
                // if (testOn) {
                //     globTotSteps = seBoxDis;
                //     break;
                // }
                
                
                if (
                    (seBoxDis <= MAX_BOX_DIS)
                    && (tt > 0.01) && 
                    
                    ((hitBox.y-hitBox.x) > (globBoxDim.w/sqrt(2.0)))
                    
                ) {
                  
                  tval = rayCast(
                      p1,
                      rd,
                      minMaxT.xy,
                      mix(
                          MAX_DF_STEPS,
                          1.0,
                          clamp(distance(cameraPos,p1)/(MAX_CAM_DIS*0.5),0.0,1.0)
                      )*accuracy
                  );
                     
                  if (
                      (tval < hitBox.y)
                      && (tval > 0.01)
                  ) {
                      globTotLS += float(i);
                      return tval;
                  }
                  
                  if (globCurSteps >= MAX_STEPS) {
                    break;
                  }
                   
                }
                
                
                
                
                
                
            }
        }
        
        p += s*0.5;
        
        lastPoint = centerPoint;
    }
    
    globTotLS += float(i);
    return MAX_CAM_DIS;
}

// float hardShadow(vec3 ro, vec3 rd) {
    
//     float t = lstep(ro+rd*0.1, rd, , 0.0);    
//     return clamp(float(t < (MAX_CAM_DIS*0.1))+t/0.5,0.0,1.0);
    
// }

float softShadow( vec3 ro, vec3 rd, float mint, float maxt, float numSteps )
{
    float res = 1.0;
    float t = mint;
    float h = 0.0;
    
    float i;
    
    float stepMod = 1.0/(numSteps/16.0);
    
    for( i = 0.0; i < numSteps; i += 1.0 )
    {
        h = map( ro + rd*t );
        res = min( res, 8.0*stepMod*h/t );
        t += clamp( h, 0.02*stepMod, 0.10*stepMod );
        if( h<0.001 || t>maxt ) break;
    }
    return clamp( res, 0.0, 1.0 );

}


vec3 getNormal( vec3 pos )
{
    vec3 eps = vec3( 0.003, 0.0, 0.0 );
    vec3 nor = vec3(
        map(pos+eps.xyy) - map(pos-eps.xyy),
        map(pos+eps.yxy) - map(pos-eps.yxy),
        map(pos+eps.yyx) - map(pos-eps.yyx) );
    return normalize(nor);
}

float calcAO( vec3 pos, vec3 nor )
{
    float occ = 0.0;
    float sca = 1.0;
    for( int i=0; i<5; i++ )
    {
        float hr = 0.01 + 0.12*float(i)/4.0;
        vec3 aopos =  nor * hr + pos;
        float dd = map( aopos );
        occ += -(dd-hr)*sca;
        sca *= 0.95;
    }
    return clamp( 1.0 - 3.0*occ, 0.0, 1.0 );    
}




vec3 render( vec3 roBase, vec3 rdBase )
{
    vec3 brdf = vec3(0.0);
    
    float occ;
    float amb;
    float dif;
    float bac;
    float fre;
    float spe;
    float t;
    float shadVal;
    float fogVal;
    vec3 ro = roBase;
    vec3 rd = rdBase;
    vec3 refVal;
    vec3 pos;
    vec3 col = vec3(0.0);
    vec3 curCol = vec3(0.0);
    vec3 nor;
    vec3 ref;
    
    float disMod = 1.0;
    
    int i;
    
    for (i = 0; i < 1; i++) {
        t = lineStep(ro,rd,(MAX_CAM_DIS/CELL_SIZE)*disMod,1.0);
        globTotSteps += globCurSteps;
        
        pos = ro + t*rd;
        curCol = mapWithCol(pos).gba;
        
        //fogVal: 1.0 near camera, 0.0 far away
        
        fogVal = clamp(1.0 - distance(roBase,pos)*2.0/MAX_CAM_DIS,0.0,1.0);
        //1.0 - clamp(pow(t*2.0/MAX_CAM_DIS,2.0),0.0,1.0);
        nor = getNormal( pos );
        ref = normalize(reflect( rd, nor ));
        
        
        if( t < MAX_CAM_DIS ) {
            
            
            occ = calcAO( pos, nor );
            amb = clamp( 0.5+0.5*nor.z, 0.0, 1.0 );
            dif = clamp( dot( nor, lightVec ), 0.0, 1.0 )*fogVal;
            bac = clamp( dot( nor, normalize(vec3(-lightVec.x,-lightVec.y,0.0))), 0.0, 1.0 )*fogVal;
            fre = pow( clamp(1.0+dot(nor,rd),0.0,1.0), 2.0 );
            spe = pow(clamp( dot( ref, lightVec ), 0.0, 1.0 ),16.0)*fogVal;
            
            shadVal = softShadow( pos, lightVec, 0.02, 1.0, fogVal*16.0 );
            
            
            brdf += dif;
            brdf += 2.0*spe*dif;
            brdf += amb*occ;
            
            brdf += bac*occ;
            brdf += fre*occ;
            curCol = curCol*brdf;
            
            curCol = mix(curCol*0.4,curCol,shadVal);
            curCol = mix(skyCol,curCol,fogVal);
            
            ro = pos;
            rd = ref;
            
        }
        
        if (i == 0) {
            col += curCol;
        }
        else {
            col += curCol*0.1;
        }
        
        
        
        if( t >= MAX_CAM_DIS ) {
            break;
        }
        
        disMod = fogVal;
    }
    
    

    return vec3( clamp(col,0.0,1.0) );
}

mat3 setCamera( vec3 ro, vec3 ta, float cr )
{
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}




void main() {
    
    globCurSteps = 0.0;
    globTotSteps = 0.0;
    globTotLS = 0.0;
    //globBoxDis = 0.0;
    globBoxDim = vec4(0.0);
    globCenterPoint = vec3(0.0);
    globBoxPower = vec2(0.0);

    

    float aspect = bufferDim.y/bufferDim.x;
    float NEAR = clipDist.x;
    float FAR = clipDist.y;

    float dx = tan(FOV*0.5f)*(gl_FragCoord.x/(bufferDim.x*0.5)-1.0f)/aspect;
    float dy = tan(FOV*0.5f)*(1.0f-gl_FragCoord.y/(bufferDim.y*0.5));
    
    dx = -dx;
    
    vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
    vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);
    
    p1 = modelviewInverse*p1;
    p2 = modelviewInverse*p2;

    vec3 ro = p1.xyz;
    vec3 rd = normalize(p1.xyz-p2.xyz);

    vec3 col = vec3(0.0);
    
    
    float dis = 0.0;
    
    
    col = render( ro, rd );
    
    if (testOn) {
        col = vec3(
            globTotSteps/(MAX_STEPS+MAX_DF_STEPS),
            0.0,
            globTotLS/MAX_CAM_DIS
        );
    }

    gl_FragData[0] = vec4( col, 1.0 );

}
