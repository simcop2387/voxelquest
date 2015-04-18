#version 120

uniform float curTime;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform vec3 lookAtVec;
uniform mat4 modelviewInverse;
uniform float FOV;
uniform vec2 clipDist;

varying vec2 TexCoord0;

varying vec4 rayStart;
varying vec4 rayEnd;

uniform sampler2D Texture0;

const float MAX_CAM_DIS = 10.0;
const vec3 skyCol = vec3(0.55, 0.7, 0.8);
const float CELL_SIZE = 1.0;
const float MAX_CELLS = 999.0;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
    
    
}

$

vec3 globP1;
vec3 globP2;
vec3 globCenterPoint;
vec4 globBoxDim;


float pointSegDistance(vec3 testPoint, vec3 sp0, vec3 sp1)
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
    return distance(testPoint, testPoint2);
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

float udRoundBox2( vec3 centerPoint, vec3 vectorFromCenter, vec3 boxDim, float r ) //, float pVert, float pHorz
{
    
    // float rayDis = pointSegDistance(centerPoint,globP1,globP2);
    
    // if (rayDis > length(boxDim+r)*2.0) {
    //     return 1.0;
    // }
    
    return (length(max(abs(vectorFromCenter)-boxDim,0.0))-r);
}

float udRoundBox( vec3 centerPoint, vec3 vectorFromCenter, vec3 boxDim, float r, float pVert, float pHorz )
{
    
    
    // float rayDis = pointSegDistance(centerPoint,globP1,globP2);
    
    // if (rayDis > length(boxDim+r)) {
    //     return 1.0;
    // }
        
    vec3 newP = abs(max(abs(vectorFromCenter)-boxDim,0.0));
    
    vec2 pv1 = vec2(pHorz);
    vec2 pv2 = vec2(pVert);
    
    newP.xy = pow(newP.xy, pv1 );
    newP.x = pow( newP.x + newP.y, 1.0/pv1.x );
    
    newP.xz = pow(newP.xz, pv2 );
    newP.x = pow( newP.x + newP.z, 1.0/pv2.x );
    
    
    return (newP.x-r)*0.5;

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


float cell3D(vec3 P) {
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
    vec3 lowz_mod = vec3( 1.0 / ( SOMELARGEFLOATS + Pi.zzz * ZINC ) );
    vec3 highz_mod = vec3( 1.0 / ( SOMELARGEFLOATS + Pi_inc1.zzz * ZINC ) );
    vec4 hash_x0 = fract( Pt * lowz_mod.xxxx ) * 2.0 - 1.0;
    vec4 hash_x1 = fract( Pt * highz_mod.xxxx ) * 2.0 - 1.0;
    vec4 hash_y0 = fract( Pt * lowz_mod.yyyy ) * 2.0 - 1.0;
    vec4 hash_y1 = fract( Pt * highz_mod.yyyy ) * 2.0 - 1.0;
    vec4 hash_z0 = fract( Pt * lowz_mod.zzzz ) * 2.0 - 1.0;
    vec4 hash_z1 = fract( Pt * highz_mod.zzzz ) * 2.0 - 1.0;

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


    float divAmount = 1.0;

    d1 = min(d1, d2);
    d1.xy = min(d1.xy, d1.wz);
    float finalRes = min(d1.x, d1.y) * ( 9.0 / 12.0 ); // return a value scaled to 0.0->1.0

    // float isStone = float(finalRes < divAmount);

    // return vec4(
    //  3.0,// finalNormUID = matResult.x;
    //  mix(TEX_NULL,TEX_DEBUG,isStone),// finalMat = matResult.y;
    //  isStone,// finalInside = matResult.z;
    //  finalRes/divAmount// finalMod = matResult.w;
    // );

    return finalRes;

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


vec4 getSE(vec3 pos, bool getColor) {
    
    float newTime = curTime;
    int i;
    
    vec3 centerPos = globCenterPoint;
    vec3 repPos = pos;
    
    vec3 centerOffset;
    vec3 innerBoxSize = globBoxDim.xyz;
    float cornerRad = globBoxDim.w;
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
    
    res = opU(
        res,
        vec4(
            opD(
                sdTer(pos),
                - (1.0-cell3D(pos*8.0))*(1.0-terMod)/10.0
            ),
            terColor
        )
    );
    
    res = opU(
        res,
        vec4(
            opD(
                sdPlane(pos-6.5),
                0.0//sin(curTime+pos.x*50.0)*sin(curTime+pos.y*40.0)*0.02
            ),
            waterColor
        )
    );
    
    
    //for (i = 0; i < 1; i++) {
        //centerPos.x += 2.0;
        
        centerOffset = repPos-centerPos;
        uvw = getUVW(repPos, centerOffset, innerBoxSize, cornerRad);
        uvw.x += newTime*0.25;
        
        if (uvw.w >= 0.999) {
            disVal = 0.0;
            // - (1.0-cell3D(pos*8.0))/20.0;
            //getRidge2(uvw.xy*4.0)*0.05;
        }
        else {
            disVal = getShingle(uvw.xy*16.0);
        }
        
        if (getColor) {  
            if (uvw.w >= 0.999) {
                houseColor = vec3(0.5,0.4,0.3);
            }
            else {
                houseColor = mix(
                    (sin(centerPos)+1.0)*0.5,
                    (cos(centerPos*1.5)+1.0),
                    disVal
                )*(1.0-disVal);
                //vec3(0.2,0.5,0.4) + vec3(5.0,0.0,20.0)*disVal;
                
            }
        }
        
        
        
        res = opU(
            res,
            vec4(
                opS(
                    opD(
                        udRoundBox(  centerPos, centerOffset, innerBoxSize, cornerRad, mix(4.0,0.75,abs(sin(newTime/1.5))), mix(0.75,4.0,abs(cos(newTime))) ),
                        disVal*0.02
                    ),
                    sdBox(pos-cameraPos, vec3(0.5) )
                ),
                houseColor
            )
            
        );
        
        
    //}
    
    
    
    return res;
}


float map( vec3 pos ) {
    return getSE(pos, false).x;
}

vec4 mapWithCol( vec3 pos ) {
    return getSE(pos, true);
}


// sdBox(
//     opRepAlt( 
//         // vec3(
//         //     atan(pos.x,pos.y)/6.2831,
//         //     pos.z,
//         //     0.02 // +0.5*length(pos-vec3(-2.0,0.2, 0.0))
//         // ),
        
//         // vec3(
//         //     atan(pos.x,pos.y)/6.2831,
//         //     pos.z,
//         //     distance(pos,vec3( 1.0, 1.0,0.25))
//         // ),
        
//         // pos.xyz,
//         uvw.xyz,
        
//         vec3(0.25,0.25,0.0)
//     ),
//     vec3(0.05,0.05,0.05)
// )*0.1
// - (1.0-cell3D(pos*16.0))/50.0



// return vec2 (distance, id);
// float map( vec3 pos )
// {
    
    // float newTime = curTime; //1.0;//
    
    // vec2 res = opU(
    //     vec2( sdPlane(pos), 1.0 ),
    //     vec2( sdSphere(pos-vec3( 0.0, 0.0, 0.25), 0.25 ), 46.9 )
    // );
    // res = opU( res, vec2( sdBox(       pos-vec3( 1.0, 0.0,0.25), vec3(0.25) ), 3.0 ) );
    // //res = opU( res, vec2( udRoundBox(  pos-vec3( 1.0, 1.0,0.25), vec3(0.1, 0.1 + sin(newTime)*0.1, 0.2), 0.25, 2.0, 2.0 ), 41.0 ) );
    // res = opU( res, vec2( sdTorus(     pos-vec3( 0.0, 1.0,0.25), vec2(0.20,0.05) ), 25.0 ) );
    // res = opU( res, vec2( sdCapsule(   pos,vec3(-1.3,-0.1,0.20), vec3(-1.0,0.20,0.2), 0.1  + sin(newTime)*0.05  ), 31.9 ) );
    // res = opU( res, vec2( sdTriPrism(  pos-vec3(-1.0,-1.0,0.25), vec2(0.25,0.05) ),43.5 ) );
    // res = opU( res, vec2( sdCylinder(  pos-vec3( 1.0,-1.0,0.30), vec2(0.1,0.2) ), 8.0 ) );
    // res = opU( res, vec2( sdCone(      pos-vec3( 0.0,-1.0,0.50), vec3(0.8,0.6,0.3) ), 55.0 ) );
    // res = opU( res, vec2( sdTorus82(   pos-vec3( 0.0, 2.0,0.25), vec2(0.20,0.05) ),50.0 ) );
    // res = opU( res, vec2( sdTorus88(   pos-vec3(-1.0, 2.0,0.25), vec2(0.20,0.05) ),43.0 ) );
    // res = opU( res, vec2( sdCylinder6( pos-vec3( 1.0, 2.0,0.30), vec2(0.1,0.2) ), 12.0 ) );
    // res = opU( res, vec2( sdHexPrism(  pos-vec3(-1.0, 1.0,0.20), vec2(0.25,0.05) ),17.0 ) );

    // res = opU( res, vec2( opS(
    //                  udRoundBox(  pos-vec3(-2.0,1.0,0.2), vec3(0.15),0.05, 2.0, 2.0),
    //                  sdSphere(    pos-vec3(-2.0, 1.0, 0.2 + sin(newTime)*0.25), 0.25)), 13.0 ) );
    // res = opU(
    //     res, 
    //     vec2( 
    //         opI(
    //             sdTorus82(  pos-vec3(-2.0,0.2, 0.0), vec2(0.20,0.1)),
    //             sdBox(
    //                 opRep( 
    //                     vec3(
    //                         atan(pos.x+2.0,pos.z)/6.2831 + newTime/10.0,
    //                         pos.y,
    //                         0.02+0.5*length(pos-vec3(-2.0,0.2, 0.0))
    //                     ),
    //                     vec3(0.05,1.0,0.05)
    //                 ),
    //                 vec3(0.02*@tempv1@,0.5,0.02*@tempv1@)
    //             )
    //         ),
    //         51.0
    //     )
    // );
    // res = opU( res, vec2( 0.7*sdSphere(    pos-vec3(-2.0,-1.0,0.25), 0.2 ) + 
    //                                    0.03*sin(50.0*pos.x+newTime)*sin(50.0*pos.y+newTime)*sin(50.0*pos.z+newTime), 
    //                                    65.0 ) );
    // res = opU( res, vec2( 0.5*sdTorus( opTwist(pos-vec3(-2.0,2.0,0.25)),vec2(0.20,0.05)), 46.7 ) );


    
    // vec3 centerOffset = pos-vec3( 1.0, 1.0,0.25);
    // vec3 innerBoxSize = vec3(1.0 + cos(newTime), 1.0 + sin(newTime), 1.0)*0.5;
    // float cornerRad = 0.5;
    
    // vec4 uvw = getUVW(pos, centerOffset, innerBoxSize, cornerRad);
    
    // uvw.x += newTime*0.5;
    
    // float disVal = 0.0;
    
    
    
    // if (uvw.w >= 0.999) {
    //     disVal = 0.0;
    // }
    // else {
    //     disVal = getShingle(uvw.xy*4.0)*0.05;
    // }
    
    
    // res = opU( res, vec2(
        
    //     41.0     
    // ) );


    //return res;
//}


vec3 roundVal(vec3 p) {
    return floor(p+0.5);
}


struct Ray {
    vec3 Origin;
    vec3 Dir;
};


vec3 aabbIntersect(vec3 rayOrig, vec3 rayDir, vec3 minv, vec3 maxv)
{
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

vec4 getBoxDim(vec3 centerPoint) {
    vec3 oneVec = vec3(1.0);
    float seed = curTime+dot(centerPoint,oneVec);
    return vec4(
        vec3(
            1.0 + cos( seed ),
            1.0 + sin( seed ),
            1.0
        )*CELL_SIZE/10.0,
        CELL_SIZE/10.0
    );
    
    //return vec4(0.0);

}


float castRay( vec3 ro, vec3 rd, vec2 minMaxT, int numSteps ) {
    int p;
    
    float precis = 0.002;
    
    globP1 = ro + rd*200.0;
    globP2 = ro - rd*200.0;
    
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
    t = t - stepVal*20.0;
    
    for (p = 0; p < 40; p++) {
        
        res = map( ro+rd*(t+stepVal) );
        
        t += stepVal;
        
        if (res < -0.00001) {
            break;
        }
    }
    
    
    return t;
}

float pointBoxDis(vec3 rectMin, vec3 rectMax, vec3 p) {
    
    vec3 res0 = vec3(0.0);
    vec3 res1 = vec3(rectMin-p);
    vec3 res2 = vec3(p-rectMax);
    vec3 res3 = max(max(res0,res1),res2);
    return length(res3);
}

float lineStep(vec3 ro, vec3 rd, float maxCells) {
    
    globP1 = ro + rd*200.0;
    globP2 = ro - rd*200.0;
    
    
    
    vec3 p1 = (ro);
    
    // if (maxCells == MAX_CELLS) {
    //     // we are shooting ray from camera
    //     p += rd*CELL_SIZE;
    // }
    
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
    
    
    //bool hitPoint = false;
    
    vec4 boxDim = vec4(0.0);
    //float cornerRad = CELL_SIZE/10.0;
    
    vec3 hitBox = vec3(0.0);

    // todo: wont show at origin
    vec3 lastPoint = vec3(0.0);

    vec3 p = p1;
    
    float tv1 = castRay(ro,rd,vec2(0.0,MAX_CAM_DIS), 50);
    float tv2 = MAX_CAM_DIS;
    
    vec3 hit1 = vec3(0.0);
    vec3 hit2 = vec3(0.0);
    
    float testRad = 0.0;
    
    bool didHit = false;
    
    float tval = 0.0;
    
    for (i = 0; i < 0; i++) { //n
        
        didHit = false;
        
        centerPoint = roundVal(p)*CELL_SIZE;
        
        if (all(equal(centerPoint,lastPoint)) ) {
            
        }
        else {
            globBoxDim = getBoxDim(centerPoint);
            
            
            
            //hitPoint = boxCheck(p1, rd, centerPoint, innerBoxRad, cornerRad);
            
            hitBox = aabbIntersect(p1,rd,centerPoint-globBoxDim.xyz-globBoxDim.w, centerPoint+globBoxDim.xyz+globBoxDim.w);
            
            // if (hitBox.z == 0.0) {
            //     hitBox = aabbIntersect(p1,rd,vec3(-100.0), vec3(100.0, 100.0, 0.0));
            // }
            
            
            
            if (hitBox.z > 0.0) {
                didHit = true;
                
                // hit1 = (p1 + hitBox.x*rd);
                // hit2 = (p1 + hitBox.y*rd);
                
                // didHit = pointBoxDis(centerPoint-globBoxDim.xyz, centerPoint+globBoxDim.xyz, hit1) < globBoxDim.w;
                // didHit = didHit||(pointBoxDis(centerPoint-globBoxDim.xyz, centerPoint+globBoxDim.xyz, hit2) < globBoxDim.w);
                
                
                //hit1 = p1 + hitBox.x*rd;
                //didHit = length(max(abs(p-centerPoint)-globBoxDim.xyz,0.0))<globBoxDim.w;  
            }
            
            if (didHit) {
                // hit1 = abs((p1 + hitBox.x*rd) - centerPoint);
                // hit2 = abs((p1 + hitBox.y*rd) - centerPoint);
                
                // testRad = max(
                //     dot(hit1,innerBoxRad),
                //     dot(hit2,innerBoxRad)
                // )-innerBoxRad;
                
                // if (testRad < cornerRad) {
                //     didHit = true;
                //     break;
                // }
                
                
                
                globCenterPoint = centerPoint;
                
                tval = castRay(p1,rd,hitBox.xy, 30);
                
                if ( tval < hitBox.y ) {
                    break;
                }
                
            }
            else {
                
            }
            
        }
        
        p += s*0.5;
        
        lastPoint = centerPoint;
    }
    
    if (didHit) {
        if (tval < hitBox.y) {
            tv2 = tval;
        }
        else {
            tv2 = MAX_CAM_DIS;
        }
    }
    else {
        tv2 = MAX_CAM_DIS;
    }
    
    
    
    return min(tv1,tv2);
}

float hardShadow(vec3 ro, vec3 rd, float mint, float tmax) {
    
    float t = lineStep(ro,rd, 2.0);    
    vec3 pos = ro + t*rd;
    vec3 col = mapWithCol(pos).gba;
    
    return float(t >= tmax);
    
    // if (all(equal(col,skyCol)) || (distance(pos,cameraPos) >= tmax) ) {
    //     return 1.0;
    // }
    // else {
    //     return 0.0;
    // }
    
    
}

vec3 getReflection(vec3 ro, vec3 rd) {
    vec3 startPos = ro;
    
    float res = lineStep(ro+rd*0.01, rd, 4.0);
    
    vec3 endPos = ro + res*rd;
    
    return mapWithCol(endPos).gba*clamp(1.0-distance(startPos,endPos)/1.0,0.0,1.0);
}

float softShadow( vec3 ro, vec3 rd, float mint, float maxt )
{
    float res = 1.0;
    float t = mint;
    float h = 0.0;
    for( int i=0; i<16; i++ )
    {
        h = map( ro + rd*t );
        res = min( res, 8.0*h/t );
        t += clamp( h, 0.02, 0.10 );
        if( h<0.001 || t>maxt ) break;
    }
    return clamp( res, 0.0, 1.0 );

}


vec3 calcNormal( vec3 pos )
{
    vec3 eps = vec3( 0.001, 0.0, 0.0 );
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




vec3 render( vec3 ro, vec3 rd )
{
    
    
    float t = lineStep(ro,rd,MAX_CELLS);
    vec3 pos = ro + t*rd;
    vec3 col = mapWithCol(pos).gba;
    
    float shadVal;
    
    float fogVal;
    
    if( t < MAX_CAM_DIS ) {
        
        
        
        
        vec3 nor = calcNormal( pos );
        vec3 ref = reflect( rd, nor );
        
        // material        
        // col = 0.45 + 0.3*sin( vec3(0.05,0.08,0.10)*(m-1.0) );
        // col.r = abs(col.r-1.0);

        
        // if( m<1.5 )
        // {
            
        //     float f = mod( floor(5.0*pos.y + sin(pos.x*2.0)) + floor(5.0*pos.x + sin(pos.y*2.0)), 2.0);
        //     col = abs(mod(pos.zxy*0.03,vec3(1.0))-0.5) + 0.1*f;
        //     col.rgb = vec3( (col.g+col.b)*0.5 );
            
        //     col.rgb = clamp(col.rgb,0.0,1.0);
        // }

        // lighitng
        
        fogVal = 1.0-clamp(t/MAX_CAM_DIS,0.0,1.0);
                
        float occ = calcAO( pos, nor );
        vec3  lig = normalize( vec3(-0.6, 0.7, 1.0) )*fogVal;
        float amb = clamp( 0.5+0.5*nor.z, 0.0, 1.0 );
        float dif = clamp( dot( nor, lig ), 0.0, 1.0 );
        float bac = clamp( dot( nor, normalize(vec3(-lig.x,-lig.y,0.0))), 0.0, 1.0 )*clamp( 1.0-pos.z,0.0,1.0);
        float dom = smoothstep( -0.5, 0.5, ref.y );
        float fre = pow( clamp(1.0+dot(nor,rd),0.0,1.0), 2.0 );
        float spe = pow(clamp( dot( ref, lig ), 0.0, 1.0 ),16.0);
        
        shadVal = softShadow( pos, lig, 0.02, 1.0 );
        
        dif *= shadVal;
        dom *= getReflection( pos, ref)*fogVal;
        
        

        vec3 brdf = vec3(0.0);
        brdf += 1.20*dif*vec3(1.00,0.90,0.60);
        brdf += 1.20*spe*vec3(1.00,0.90,0.60)*dif;
        brdf += 0.30*amb*vec3(0.50,0.70,1.00)*occ;
        if (length(dom) > -1.0) {
            brdf += 2.0*dom*vec3(0.50,0.70,1.00)*occ;
        }
        brdf += 0.30*bac*vec3(0.25,0.25,0.25)*occ;
        brdf += 0.40*fre*vec3(1.00,1.00,1.00)*occ;
        brdf += 0.02;
        
        //brdf = pow(brdf,vec3(0.5));
        
        //col = pow( col, vec3(0.4545) );
        col = col*brdf;
        //col += dom*vec3(0.50,0.70,1.00)*occ*0.3;

        //col = mix( col, vec3(0.8,0.9,1.0), 1.0-exp( -0.0005*t*t ) );

        //col = (nor+1.0)/2.0;
        //col = mod(pos,0.5)*2.0;
        
        
        
        //col = pow( col, vec3(0.75) );
        
        col = mix(skyCol,col,fogVal );
        
        
        
        //col = vec3(shadVal);
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
    
    // vec2 q = TexCoord0.xy;
    // vec2 p = -1.0+2.0*q;
    // p.x *= bufferDim.x/bufferDim.y;
    // vec2 mo = vec2(0.0);//iMouse.xy/bufferDim.xy;
    // float time = 15.0 + curTime;

    
    globBoxDim = vec4(0.0);
    globCenterPoint = vec3(0.0);
    globP1 = vec3(0.0);
    globP2 = vec3(0.0);

    

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

    vec3 col = render( ro, rd );

    

    gl_FragData[0] = vec4( col, 1.0 );

}
