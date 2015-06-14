#version 330



uniform bool testOn;

uniform float UNIT_MAX;
uniform float FOV;
uniform float curTime;
uniform float pixelsPerCell;
uniform float volSizePrim;
uniform float waterLerp;

uniform vec2 bufferDim;
uniform vec2 clipDist;
uniform vec3 lightVec;
uniform vec3 cameraPos;
uniform vec3 lookAtVec;

uniform mat4 proj;
uniform mat4 modelview;
uniform mat4 modelviewInverse;


//varying vec2 TexCoord0;




// varying vec4 rayStart;
// varying vec4 rayEnd;

//uniform sampler2D Texture0;

uniform sampler3D Texture0; // E_PL_TERRAIN
//uniform sampler3D Texture1; // E_PL_PRIMIDS

uniform samplerBuffer Texture1;

//texture(Texture0, newCoords);


//const int NUM_SAMPLES = 4;
const int MAX_BOUNCES = 1;

float MAX_CAM_DIS;

//x: basic pass, y: detail pass
const float MAX_DETAIL_DIS = 40.0;
const float MIN_STEPS = 1.0;
const float MAX_STEPS = 30.0;

//const float MAX_DF_STEPS = 30.0;


const float MAX_DET_STEPS = MAX_STEPS*3.0;

const float MAX_BOX_DIS = 0.02;



const float M_PI = 3.14159265359;
const vec3 skyCol = vec3(0.55, 0.7, 0.8);
const vec3 crand0 = vec3(12.989, 78.233, 98.422);
const vec3 crand1 = vec3(93.989, 67.345, 54.256);
const vec3 crand2 = vec3(43.332, 93.532, 43.734);





//##
const float timeScale = 0.4;
const int numWaves = 8;
float amplitude[8] = float[]( 1.0/16.0, 1.0/32.0, 1.0/2.0, 1.0/4.0, 1.0/8.0, 1.0/64.0, 1.0/128.0, 1.0/256.0 );
float wavelength[8] = float[]( 48.0/1.0, 48.0/5.0, 48.0/9.0, 48.0/19.0, 48.0/29.0, 48.0/41.0, 48.0/53.0, 48.0/68.0 );
float speed[8] = float[]( 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);
vec2 direction[8] = vec2[](
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



layout(location = 0) in vec4 vposition;
layout(location = 1) in vec2 vtexcoord;
out vec2 TexCoord0;
void main() {
   TexCoord0 = vtexcoord;
   gl_Position = vposition;
};

// void main() {

//     //TexCoord0 = gl_MultiTexCoord0.xy;
//     gl_Position = gl_Vertex;
    
    
// }

$



//vec3 globP1;
//vec3 globP2;
//float globWaterAbove;
//float globFirstWaterHit;
//float globWaterVal;
//vec3 globCenterPointWater;
//float globTotBoxSteps;
//float globTotLS;
//bool globFailed;

//bool globFinalHit;
int globNumPrims;
int globBaseInd;
float globCurSteps;
float globTotSteps;
float globIntersect;

// vec3 globCenterPoint;
// vec4 globBoxDim;
// vec2 globBoxPower;



in vec2 TexCoord0;
layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;
layout(location = 2) out vec4 FragColor2;
layout(location = 3) out vec4 FragColor3;


vec3 getBoxCenterPoint(vec3 offsetPoint, int primTempInd) {
    int offset = primTempInd*E_PRIMTEMP_LENGTH;
    
    return (
        
        (offsetPoint + primTemp[offset + E_PRIMTEMP_BOUNDSMAX].xyz) +
        (offsetPoint + primTemp[offset + E_PRIMTEMP_BOUNDSMIN].xyz)
    )*0.5;// + sin(curTime*3.0 + float(primTempInd))*2.0;
}

vec4 getBoxDim(int primTempInd) {//, float seed) {
    
    int offset = primTempInd*E_PRIMTEMP_LENGTH;
    
    return vec4(
        (primTemp[offset + E_PRIMTEMP_BOUNDSMAX].xyz-primTemp[offset + E_PRIMTEMP_BOUNDSMIN].xyz)*0.5,
        primTemp[offset + E_PRIMTEMP_CORNERDIS].x
    );
    
    // return vec4(
    //     1.0,
    //     1.0,
    //     1.0,
    //     1.0
    // )/5.0;
    
    // return vec4(
    //     vec3(
    //         2.0 + cos( seed*1.1 ),
    //         2.0 + sin( seed*3.2 ),
    //         2.0 + sin( seed*2.3 )
    //     )/25.0,
    //     ( 1.5 + cos( seed*0.7 ) )/10.0
    // );
}

vec2 getBoxPower(int primTempInd) {//, float seed) {
  
  int offset = primTempInd*E_PRIMTEMP_LENGTH;
  
  return primTemp[offset + E_PRIMTEMP_CORNERDIS].zw;// + sin(curTime*3.0 + float(primTempInd)) + 1.0;
  // return vec2(
  //   mix(4.0,1.0,(1.0+sin(seed*1.3))*0.5),
  //   mix(4.0,1.0,(1.0+sin(seed*2.5))*0.5)
  // );

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

float pointSegDisF(vec3 testPoint, vec3 sp0, vec3 sp1)
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
    return distance(testPoint, testPoint2);
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




float sdCone( vec3 p, vec3 c )
{
    vec2 q = vec2( length(p.xz), p.y );
    float d1 = -p.y-c.z;
    float d2 = max( dot(q,c.xy), p.y);
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.0);
}



vec4 getUVW(
    vec3 worldPos,
    vec3 centerPos, 
    vec4 boxDim,
    vec4 uvwScale
) {
    
    globIntersect = 999.0;
    
    vec3 minCorner = centerPos-boxDim.xyz;
    vec3 pos = (worldPos-minCorner)*uvwScale.xyz/(boxDim.xyz*2.0); //
    
    vec3 centerOffset = worldPos-centerPos;
    vec3 innerBoxSize = boxDim.xyz;
    float cornerRad = boxDim.w;
    
    //vec3 totBoxSize = innerBoxSize+cornerRad;
    //vec3 pos = pPos*(floor(totBoxSize*16.0)/16.0)/totBoxSize;
    
    vec3 uvwCoords = pos.xyz;
    
    
    
    
    vec3 newOffset = max(abs(centerOffset)-innerBoxSize,0.0);
    vec2 newNorm1 = normalize(newOffset.xy);
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
    
    // side corner
    if (newNorm1.x*newNorm1.y != 0.0) {
        
        uvwCoords.x = curPhi*uvwScale.w*2.0/M_PI;
        uvwCoords.y = pos.z;
    }
    
    // top corner
    if (newNorm2.x*newNorm2.y != 0.0) {
        //uvwCoords.x = curPhi*cornerRad;
        uvwCoords.y = curThe*uvwScale.w*2.0/M_PI;
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
        
        
        //uvwCoords.x += length((worldPos.xy-centerPos.xy)*4.0/boxDim.xy);
        
        //uvwCoords.xy = normalize(worldPos.xy-centerPos.xy)*2.0;
        
    }
    
    
    
    uvwCoords.z = length(centerOffset);
    
    return vec4(uvwCoords,newNorm2.x);
}

float udRoundBox2( vec3 centerPoint, vec3 vectorFromCenter, vec4 boxDim ) //, float pVert, float pHorz
{
    
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
    
    
    return ((newP.x-boxDim.w))*0.5;

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



float getTer3D(vec3 pos) {
    
    vec4 samp = texture(Texture0, pos/volSizePrim );
    return samp.a;
}



float getWater3D(vec3 pos) {
    
    vec4 samp = texture(Texture0, (pos)/volSizePrim );
    
    float waterVal = mix(samp.b,samp.g,waterLerp);
    
    return mix(
        1.0,
        -2.0,
        mix(waterVal,0.0,0.2) //samp.g //+sin(curTime)*0.1
    );
}


float getOld3D(vec3 pos) {
    
    vec4 samp = texture(Texture0, pos/volSizePrim );
    vec4 samp2 = texture(Texture0, pos*2.0/volSizePrim );
    vec4 samp3 = texture(Texture0, pos*4.0/volSizePrim );
    vec4 samp4 = texture(Texture0, pos*8.0/volSizePrim );
    // vec4 samp5 = texture(Texture0, pos*16.0/volSizePrim );
    // vec4 samp6 = texture(Texture0, pos*32.0/volSizePrim );
    
    float smax = (samp.g*8.0 + samp2.g*4.0 + samp3.g*2.0 + samp4.g*1.0)/15.0;
    
    //smax = pow(smax,2.0-2.0*smax); 
        //(samp.a*8.0 + samp2.a*4.0 + samp3.a*2.0 + samp4.a*1.0 + samp5.a*0.5 + samp6.a*0.25)/15.75;//max(max(samp.r,samp.g),max(samp.b,samp.a));
    
    //smax = floor(smax*64.0 + 0.99)/64.0;
    
    return mix(-1.0,1.0,smax);//2.0*smax*float(smax < 0.5))*0.5;
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



vec4 getSEWater(vec3 pos, bool getColor) {
    
    int i;
    
    
    vec2 cellVal = vec2(1.0);
    
    vec3 waterColor = vec3(0.0,0.5,1.0);

    vec4 res = vec4(MAX_CAM_DIS,skyCol);
    
    if (
        (distance(cameraPos, pos) >= (MAX_CAM_DIS-1.0))
    ) {
        return res;
    }
    
    
    res = opU(
        res,
        vec4(
            
            
            getWater3D(pos + vec3(
                0.0,
                0.0,
                (waveHeight(pos.xy*2.0) + 1.0) * 0.3    
            )),
            
            waterColor
        )
        
    );
    
    return res;
}



vec4 getSE(vec3 pos, bool getColor) {
    
    int m;
    int primDataInd;
    int primTempInd;
    vec4 texelRes = vec4(0.0);
    vec4 globBoxDim = vec4(0.0);
    vec2 globBoxPower = vec2(0.0);
    vec3 boxCenterPoint = vec3(0.0);
    vec4 res = vec4(MAX_CAM_DIS,skyCol);
        
    if (
        (distance(cameraPos, pos) >= (MAX_CAM_DIS-1.0)) ||
        (globBaseInd < 0)
    ) {
        return res;
    }
    
    // vec2 cellVal = cell3D(pos);
    // float disVal = ( (1.0-cellVal.y) )*0.25;
    
    
    for (m = 0; m < PRIMS_PER_MACRO; m++) {
        
        primDataInd = globBaseInd + m;
        
        texelRes = texelFetch(Texture1, primDataInd);
        
        primTempInd = int(texelRes.w);
        if (primTempInd == 0) {
            break;
        }
        
        globBoxPower = getBoxPower(primTempInd);//,seed);
        globBoxDim = getBoxDim(primTempInd);//,seed);
        boxCenterPoint = getBoxCenterPoint(texelRes.xyz,primTempInd);
        
        res = opU(
            res,
            vec4(
                
                //opD(
                  udRoundBox(
                    boxCenterPoint,
                    pos-boxCenterPoint,
                    globBoxDim,
                    globBoxPower
                  ),
                //disVal),
                1.0,
                0.0,
                0.0
            )
            
        );
        
        
    }
    
    // res.x = opS(
    //   res.x,
    //   sdBox(pos-cameraPos, vec3(20.0) )
    // );
    
    
    return res;
    
    
}


float mapWater( vec3 pos ) {
    return getSEWater(pos, false).x;
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


float rayCast( vec3 ro, vec3 rd, vec2 minMaxT, float fNumSteps ) {
    int p = 0;
    int p2 = 0;
    
    
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

float rayCastWater( vec3 ro, vec3 rd, vec2 minMaxT, float fNumSteps ) {
    int p = 0;
    int p2 = 0;
    
    
    float precis = 0.2;
    
    int numSteps = int(fNumSteps);
    
    //globP1 = ro + rd*200.0;
    //globP2 = ro - rd*200.0;
    
    float res = 0.0;
    
    float t = minMaxT.x;
    
    for( p = 0; p < numSteps; p++ ) {
        
        res = mapWater( ro+rd*t );
        if (
            (res<precis) ||
            (t>minMaxT.y)
        ) {
            break;
        }
        t += res;
    }
    
    // float stepVal = 0.0005;
    // t = t - stepVal*fNumSteps*0.5;
    
    // for (p2 = 0; p2 < numSteps; p2++) {
        
    //     res = mapWater( ro+rd*(t+stepVal) );
        
    //     t += stepVal;
        
    //     if (res < -0.00001) {
    //         break;
    //     }
    // }
    
    //globCurSteps += float(p+p2);
    
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


float powDis( vec2 vectorFromCenter, vec2 boxDim, vec2 boxPower )
{
    vec2 newP = abs(max(abs(vectorFromCenter)-boxDim.xy,0.0));
    
    newP.xy = pow(newP.xy, boxPower.xx );
    newP.x = pow( newP.x + newP.y, 1.0/boxPower.x );
    
    //newP.xz = pow(newP.xz, boxPower.yy );
    //newP.x = pow( newP.x + newP.z, 1.0/boxPower.y );
    
    
    return newP.x;

}


float lineStep(vec3 ro, vec3 rd, float maxDis, float accuracy) {
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
    int n = int(
        max(max(dabs.x,dabs.y),dabs.z)
    )*2;
    
    vec3 s = d/float(n);
    
    vec4 globBoxDim = vec4(0.0);
    vec2 globBoxPower = vec2(0.0);
    
    vec2 hitBox = vec2(0.0);
    vec2 hitBoxFinal = vec2(0.0);
    vec2 cellBox = vec2(0.0);
    
    vec2 curHitBox;
    vec2 hitBoxXYZ[3];

    vec3 p = p1;
    
    
    float tval = 0.0;
    float seBoxDis1 = 0.0;
    float seBoxDis2 = 0.0;
    
    vec2 radMod = vec2(0.0);
    
    float j = 0.0;
    
    float camDis;
    float curLOD;
    float minLOD = 0.0;
    float maxLOD = 1.0;
    float btSteps = 0.0;
    
    vec3 minPlanes;
    vec3 maxPlanes;
    
    vec3 minPlanesMacro;
    vec3 maxPlanesMacro;
    
    vec3 boxCenterPoint = vec3(0.0);
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
    bool didHitFinal = false;
    
    vec4 texelRes = vec4(0.0);
    
    vec3 minBox = vec3(0.0);
    vec3 maxBox = vec3(0.0);
    
    vec3 firstHitPoint1 = vec3(0.0);
    vec3 closestHitPoint1 = vec3(0.0);
    
    vec4 closestPointOnRay1 = vec4(0.0);
    vec4 closestPointOnRay2 = vec4(0.0);
    
    vec3 firstHitPoint2 = vec3(0.0);
    vec3 closestHitPoint2 = vec3(0.0);
    
    vec3 newRD = rd*minPrec;
    
    int volSizePrimDiv = int(volSizePrim/PRIM_DIV);
    int primDataInd = 0;
    int primTempInd = 0;
    
    for (i = 0; i < n; i++) {
        
        
        testPoint = p + newRD;
        
        
        minPlanes = floor(testPoint);
        maxPlanes = minPlanes + oneVec;
        centerPoint = (minPlanes+maxPlanes)*0.5;
        
        minPlanesMacro = floor(minPlanes/PRIM_DIV)*PRIM_DIV;
        maxPlanesMacro = minPlanesMacro + PRIM_DIV;
        centerPointMacro = (minPlanesMacro+maxPlanesMacro)*0.5;
        //centerPointMacroUnit = ;
        iCenterPointMacroUnit = ivec3(centerPointMacro/PRIM_DIV);
        
        didHitFinal = false;
        hitBoxFinal.x = 99999.0;
        hitBoxFinal.y = -99999.0;
        
        //cellBox = aabbIntersect(ro,rd,minPlanes,maxPlanes);
        cellBox = aabbIntersect(ro,rd,minPlanesMacro,maxPlanesMacro);
        
        globBaseInd = -1;
        
        if (
            (iCenterPointMacroUnit.x < 0) ||
            (iCenterPointMacroUnit.x >= volSizePrimDiv) ||
            (iCenterPointMacroUnit.y < 0) ||
            (iCenterPointMacroUnit.y >= volSizePrimDiv) ||
            (iCenterPointMacroUnit.z < 0) ||
            (iCenterPointMacroUnit.z >= volSizePrimDiv)
        ) {
            // out of bounds
        }
        else {
            
            globBaseInd = (
                iCenterPointMacroUnit.x +
                iCenterPointMacroUnit.y*volSizePrimDiv +
                iCenterPointMacroUnit.z*volSizePrimDiv*volSizePrimDiv
            )*PRIMS_PER_MACRO;
            
            
            
            for (m = 0; m < PRIMS_PER_MACRO; m++) {
                
                primDataInd = globBaseInd + m;
                
                texelRes = texelFetch(Texture1, primDataInd).xyzw;
                
                primTempInd = int(texelRes.w);
                if (primTempInd == 0) {
                    globNumPrims = m;
                    break;
                }
                //globFinalHit = true;
                
                globBoxPower = getBoxPower(primTempInd);//,seed);
                globBoxDim = getBoxDim(primTempInd);//,seed);
                boxCenterPoint = getBoxCenterPoint(texelRes.xyz,primTempInd);
                
                
                radMod.y = globBoxDim.w;
                
                minBox = boxCenterPoint-globBoxDim.xyz;
                maxBox = boxCenterPoint+globBoxDim.xyz;
                
                hitBox = aabbIntersect(p1,rd,minBox-globBoxDim.w, maxBox+globBoxDim.w);
                //sampVal = texture(Texture0, centerPoint/volSizePrim );
                
                if (
                    (hitBox.x <= hitBox.y)
                    // && (sampVal.r > thresh)
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
                    
                    onlyHitCorner = !(
                        (hitBoxXYZ[0].x <= hitBoxXYZ[0].y) ||
                        (hitBoxXYZ[1].x <= hitBoxXYZ[1].y) ||
                        (hitBoxXYZ[2].x <= hitBoxXYZ[2].y)    
                    );
                    
                    didHit = false;
                    
                    if (onlyHitCorner) {
                        
                        firstHitPoint1 = (hitBox.x)*rd+ro;
                        firstHitPoint2 = (hitBox.y)*rd+ro;
                        
                        closestHitPoint1 = clamp(
                            firstHitPoint1,
                            minBox,
                            maxBox
                        );
                        
                        closestPointOnRay1 = pointSegDistance(closestHitPoint1,p1,p2);
                        seBoxDis1 = udRoundBox( boxCenterPoint, closestPointOnRay1.xyz-boxCenterPoint, globBoxDim, globBoxPower );
                        didHit = seBoxDis1 <= 0.0;
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
                                seBoxDis1 = udRoundBox( boxCenterPoint, closestPointOnRay1.xyz-boxCenterPoint, globBoxDim, globBoxPower );
                                
                                closestPointOnRay2 = pointSegDistance(closestHitPoint2,p1,p2);
                                seBoxDis2 = udRoundBox( boxCenterPoint, closestPointOnRay2.xyz-boxCenterPoint, globBoxDim, globBoxPower );
                                
                                didHit = didHit || (
                                    
                                    (seBoxDis1 <= 0.0) ||
                                    (seBoxDis2 <= 0.0)
                                );
                                
                                if (didHit) {
                                    break;
                                }
                                
                            }
                        }
                    }
                    
                    didHitFinal = didHitFinal || didHit;
                    
                    if (didHit) {
                        hitBoxFinal.x = min(hitBoxFinal.x,hitBox.x);
                        hitBoxFinal.y = max(hitBoxFinal.y,hitBox.y);
                    }
                    
                    
                    
                }
                
                
                
                //////////
                
                
                
                
            }
        }
        
        
        if (didHitFinal) {
            
          
          
          camDis = distance(cameraPos,p);
          
          // 1.0 = close, 0.0 = far away
          curLOD = mix(
              maxLOD,
              minLOD,
              clamp(camDis/MAX_DETAIL_DIS,minLOD,maxLOD)
          )*accuracy;
          
          btSteps = mix(MIN_STEPS,MAX_STEPS,curLOD);
          
          tval = rayCast(
              p1,
              rd,
              hitBoxFinal.xy,
              btSteps
          );
             
          if (
              (tval < hitBoxFinal.y)
              && (tval > 0.01)
          ) {
              //globTotLS += float(i);
              return tval;
          }
          
          if (globCurSteps >= MAX_DET_STEPS) {
            break;
          }
           
        }
        
        
        
        
        
        
        p = ro + rd*cellBox.y;
        
        if (distance(p,ro) > maxDis) {
            break;
        }
    }
    
    //globFailed = true;
    
    //globTotLS += float(i);
    return MAX_CAM_DIS;
}


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


vec3 getNormal( vec3 pos, float dis )
{
    vec3 eps = vec3( 0.0, 0.0, 0.0 );
    eps.x = mix(0.001,0.1,dis);
    
    vec3 nor = vec3(
        map(pos+eps.xyy) - map(pos-eps.xyy),
        map(pos+eps.yxy) - map(pos-eps.yxy),
        map(pos+eps.yyx) - map(pos-eps.yyx) );
    return normalize(nor);
}

vec3 getNormalWater( vec3 pos, float dis )
{
    vec3 eps = vec3( 0.0, 0.0, 0.0 );
    eps.x = mix(0.1,0.1,dis);
    
    vec3 nor = vec3(
        mapWater(pos+eps.xyy) - mapWater(pos-eps.xyy),
        mapWater(pos+eps.yxy) - mapWater(pos-eps.yxy),
        mapWater(pos+eps.yyx) - mapWater(pos-eps.yyx) );
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




// vec3 render( vec3 roBase, vec3 rdBase )
// {
//     vec3 brdf = vec3(0.0);
    
//     float occ;
//     float amb;
//     float dif;
//     float bac;
//     float fre;
//     float spe;
//     float t;
//     float shadVal;
//     float fogVal;
//     vec3 ro = roBase;
//     vec3 rd = rdBase;
//     vec3 refVal;
//     vec3 pos;
//     vec3 col = vec3(0.0);
//     vec3 curCol = vec3(0.0);
//     vec3 nor;
//     vec3 ref;
//     //vec3 tempCol;
    
//     //vec3 baseCol;
    
//     float disMod = 1.0;
    
//     int i;
//     int j;
    
//     //vec3 rdm[4];
    
//     for (i = 0; i < MAX_BOUNCES; i++) {
        
//         // rdm[0] = vec3(rd.x,rd.y,rd.z);
//         // rdm[1] = vec3(rd.x,rd.y,rd.z);
//         // rdm[2] = vec3(rd.x,rd.y,rd.z);
//         // rdm[3] = vec3(rd.x,rd.y,rd.z);
//         //tempCol = vec3(0.0);
        
//         t = lineStep(ro,rd,MAX_CAM_DIS*disMod,1.0);
//         globTotSteps += globCurSteps;
        
        
        
//         //for (j = 0; j < NUM_SAMPLES; j ++) {
            
//             pos = ro + t*rd;
//             curCol = mapWithCol(pos).gba;
            
//             //fogVal: 1.0 near camera, 0.0 far away
            
//             fogVal = clamp(1.0 - distance(roBase,pos)*2.0/MAX_CAM_DIS,0.0,1.0);
            
//             //return vec3(1.0-fogVal);
            
//             //1.0 - clamp(pow(t*2.0/MAX_CAM_DIS,2.0),0.0,1.0);
//             nor = getNormal( pos, t/MAX_CAM_DIS );
//             ref = normalize(reflect( rd, nor ));
            
            
//             if( t < MAX_CAM_DIS ) {
                
                
//                 occ = calcAO( pos, nor );
//                 amb = clamp( 0.5+0.5*nor.z, 0.0, 1.0 );
//                 dif = clamp( dot( nor, lightVec ), 0.0, 1.0 )*fogVal;
//                 bac = clamp( dot( nor, normalize(vec3(-lightVec.x,-lightVec.y,0.0))), 0.0, 1.0 )*fogVal;
//                 fre = pow( clamp(1.0+dot(nor,rd),0.0,1.0), 2.0 );
//                 spe = pow(clamp( dot( ref, lightVec ), 0.0, 1.0 ),16.0)*fogVal;
                
//                 shadVal = softShadow( pos, lightVec, 0.02, 1.0, fogVal*16.0 );
                
//                 brdf = vec3(0.0);
//                 brdf += dif;
//                 brdf += 4.0*spe*dif;
//                 brdf += amb*occ;
                
//                 brdf += bac*occ;
//                 brdf += fre*occ;
//                 curCol = curCol*brdf;
                
//                 //curCol += brdf*0.1;
                
//                 curCol = mix(curCol*0.4,curCol,shadVal);
//                 curCol = mix(skyCol,curCol,fogVal);
                
                
                
//             }
            
//             //tempCol += curCol;
            
//         //}
        
//         //curCol = tempCol/float(NUM_SAMPLES);
        
        
        
//         if (i == 0) {
//             col += curCol;
//         }
//         else {
//             col += curCol*0.1;
//         }
        
//         //col = baseCol;
        
//         if( t >= MAX_CAM_DIS ) {
//             break;
//         }
//         else {
//             ro = pos;
//             rd = ref;
//         }
        
//         disMod = fogVal;
//     }
    
//     //col = pow(col,vec3(0.85));
//     col = mix(vec3(0.5),col,1.2);
    
//     // return vec3(
        
//     //     mix(
//     //         1.0,
//     //         0.0,
//     //         clamp(distance(cameraPos,pos)/(MAX_DETAIL_DIS),0.0,1.0)
//     //     )
        
//     // );
    

//     return vec3( clamp(col,0.0,1.0) );
// }

mat3 setCamera( vec3 ro, vec3 ta, float cr )
{
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}



float depthToZ(float depth, float n, float f) { // depth is in world units
    float A = f;//modelview[2].z;
    float B = n;//modelview[3].z;
    return 0.5*(-A*depth + B) / depth + 0.5;
}

void main() {
    
    //globFailed = false;
    //globWaterAbove = 0.0;
    //globFirstWaterHit = -1.0;
    //globWaterVal = 0.0;
    //globTotBoxSteps = 0.0;
    //globTotLS = 0.0;
    //globBoxDis = 0.0;
    //globCenterPointWater = vec3(0.0);
    //globSampWater = vec4(0.0);
    
    
    //globFinalHit = false;
    //globCenterPoint = vec3(0.0);
    globCurSteps = 0.0;
    globTotSteps = 0.0;
    globIntersect = 0.0;
    globBaseInd = -1;
    globNumPrims = 0;
    

    float aspect = bufferDim.y/bufferDim.x;
    float NEAR = clipDist.x;
    float FAR = clipDist.y;

    MAX_CAM_DIS = FAR;

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
    
    
    // col = render( ro, rd );
    // if (testOn) {
    //     col = vec3(
    //         globTotSteps/(MAX_DET_STEPS+MAX_STEPS.y),
    //         0.0,
    //         globTotBoxSteps/40.0
    //         //globTotLS/MAX_CAM_DIS
    //     );
    //     if (globFailed) {
    //         col = vec3(0.0,1.0,0.0);
    //     }
    // }
    // col = mix(
    //     col,
    //     vec3(0.0,0.5,1.0),
    //     clamp(globWaterVal/2.0,0.0,1.0)*0.75
    // );
    //gl_FragData[0] = vec4( col, 1.0 );
    
    
    float tBase[2];
    float t;
    tBase[0] = lineStep(ro,rd,MAX_CAM_DIS,1.0);
    tBase[1] = 
        -1;
        // rayCastWater(
        //     ro,
        //     rd,
        //     vec2(0.0,tBase[0]),
        //     100.0
        // );
    
    float baseMat[2];
    baseMat[0] = TEX_EARTH;
    baseMat[1] = TEX_WATER;
    
    float curMat = 0.0;
    float curMod = 0.0;
    float camDis;
    float zbVal;
    vec3 nor = vec3(0.0);
    vec3 pos = vec3(0.0);
    int i;
    
    vec4 fragRes0 = vec4(0.0);
    vec4 fragRes1 = vec4(0.0);
    
    // 0 = red, 3 = green, 6 = magenta
    int offset = 2;
    vec3 testVec = vec3(1.0,0.0,0.0);//texelFetch(Texture1, offset).rgb;
    
    // if (globFinalHit) {
    //   testVec = vec3(0.0,1.0,0.0);
    // }
    
    for (i = 0; i < 2; i++) {
        
        t = tBase[i];
        pos = ro + t*rd;
        
        if (
            (t > 0.0) &&
            (t < MAX_CAM_DIS)    
        ) {
            curMat = baseMat[i]*256.0*255.0 + curMod*255.0;
        }
        else {
            curMat = 0.0;
        }
        
        if (baseMat[i] == TEX_WATER) {
            nor = getNormalWater( pos, t/MAX_CAM_DIS );
            
        }
        else {
            nor = getNormal( pos, t/MAX_CAM_DIS );
        }
        
        camDis = distance(cameraPos.xyz,pos.xyz);
        zbVal = 1.0-camDis/clipDist.y;
        
        if (testOn) {
            fragRes0 =
            //gl_FragData[0+i*2] = 
            
            
                
            
                vec4(
                    
                    
                    vec3(
                        globCurSteps/40.0,//(MAX_DET_STEPS+MAX_STEPS.y),
                        0.0,
                        (globNumPrims/float(PRIMS_PER_MACRO)) //globTotBoxSteps/40.0
                    )
                    
                    //(nor.xyz+1.0)*0.5
                    
                    
                    //testVec
                    
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










