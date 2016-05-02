#version 120

// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// result fbo
uniform sampler2D Texture2;

// result fbo blurred
uniform sampler2D Texture3;

// geom base fbo
uniform sampler2D Texture4;
uniform sampler2D Texture5;
uniform sampler2D Texture6;

// pal fbo
uniform sampler3D Texture7;

// noise fbo
uniform sampler2D Texture8;

// debug fbo
uniform sampler2D Texture9;
uniform sampler2D Texture10;

// numstepsFBO
uniform sampler2D Texture11;

uniform mat4 modelviewInverse;
uniform float FOV;
uniform vec3 lightVec;
uniform vec3 lightVecOrig;
uniform vec2 clipDist;

uniform vec3 patternTarg;
uniform float patternCells[25];
uniform bool placingPattern;


uniform bool testOn2;
uniform bool isFalling;
uniform bool isJumping;
uniform bool gridOn;
uniform int iNumSteps;
uniform float seaLevel;
uniform float volSizePrim;
uniform float curTime;
uniform float selLimbInd;
uniform float selObjInd;
uniform float actObjInd;
uniform float isUnderWater;
uniform float timeOfDay;
uniform vec2 bufferDim;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec3 lookAtVec;
uniform vec3 entPos;
uniform float thirdPerson;
//uniform vec4 fogPos;

vec3 dirVecs[6] = vec3[](
                                         vec3( 1.0,  0.0, 0.0 ), // right
                                         vec3( -1.0, 0.0, 0.0 ), // left
                                         vec3( 0.0, 1.0, 0.0 ), // up
                                         vec3( 0.0, -1.0, 0.0 ), // down
                                         vec3( 0.0, 0.0, 1.0 ), // above
                                         vec3( 0.0, 0.0, -1.0 ) // below
                                     );

^INCLUDE:MATERIALS^
//const float TEX_WATER = 32.0/255.0;

varying vec2 TexCoord0;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$




vec2 pack16(float num)
{

    int iz = int(num);
    int ig = (iz) / 256;
    int ir = iz - ig*256;//intMod(iz, 256);
    

    vec2 res;

    res.r = float(ir) / 255.0;
    res.g = float(ig) / 255.0;

    return res;

}

vec3 unpackColor(vec2 num, float lightVal)
{
    return texture3D( Texture7, vec3(lightVal, num.r, num.g + 0.5/255.0) ).rgb;
}




vec3 getRay() {
    float aspect = bufferDim.y/bufferDim.x;
    float NEAR = clipDist.x;
    float FAR = clipDist.y;
    
    vec2 newTC = TexCoord0.xy;
    //newTC.y += cameraPos.z*0.0001;
    
    float dx = tan(FOV*0.5f)*(newTC.x*2.0-1.0f)/aspect; //gl_FragCoord.x/(bufferDim.x*0.5)
    float dy = tan(FOV*0.5f)*((1.0f-newTC.y)*2.0-1.0f); //gl_FragCoord.y/(bufferDim.y*0.5)
    
    
    dx = -dx;
    
    vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
    vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);
    
    p1 = modelviewInverse*p1;
    p2 = modelviewInverse*p2;

    vec3 ro = p1.xyz;
    
    vec3 rd = normalize(p1.xyz-p2.xyz);
    
    return rd;
}
vec3 getFogColor()
{
    
    vec3 myRay = getRay();
    
    float zv = pow(1.0-(myRay.z+1.0)/2.0,2.0);
    
    return unpackColor(
        vec2(
            zv,
            TEX_SKY
        ),
        timeOfDay // (-lightVecOrig.z + 1.0)*0.5//timeOfDay
    )
    +
    
    (
        (
            pow(
                clamp(
                    dot(-lightVecOrig,myRay),0.0,1.0
                ),
                16.0    
            )*1.0
            *vec3(1.0,0.5,0.0) //+-lightVecOrig.z*0.25
        )
        +
        (
            pow(
                clamp(
                    dot(-lightVecOrig,myRay),0.0,1.0
                ),
                64.0    
            )*1.0
            *vec3(1.0,1.0,1.0) //+-lightVecOrig.z*0.25
        )    
    )*(pow(clamp((timeOfDay+0.4),0.0,1.0),8.0))
    
    
    +
    
    (
        (
            pow(
                clamp(
                    dot(-lightVecOrig*vec3(1.0,1.0,-1.0),myRay),0.0,1.0
                ),
                64.0    
            )*1.0
            *vec3(0.5,0.5,1.0) //+-lightVecOrig.z*0.25
        )
        +
        (
            pow(
                clamp(
                    dot(-lightVecOrig*vec3(1.0,1.0,-1.0),myRay),0.0,1.0
                ),
                256.0    
            )*1.0
            *vec3(1.0,1.0,1.0) //+-lightVecOrig.z*0.25
        )    
    )*(pow(clamp(1.0-(timeOfDay-0.4),0.0,1.0),8.0))
    ;
    
}


//##


const float M_PI = 3.14159265359;
const float timeScale = 0.0005;


const float WAVE_SPEED = 0.2;
const float WAVE_SCALE = 100.0;


float amplitude[8] = float[](
    1.0/256.0,
    2.0/256.0,
    4.0/256.0,
    8.0/256.0,
    
    16.0/256.0,
    32.0/256.0,
    64.0/256.0,
    128.0/256.0
        
);
float wavelength[8] = float[](
    WAVE_SCALE/128.0,
    WAVE_SCALE/64.0,
    WAVE_SCALE/32.0,
    WAVE_SCALE/16.0,
    
    WAVE_SCALE/8.0,
    WAVE_SCALE/4.0,
    WAVE_SCALE/2.0,
    WAVE_SCALE/1.0
    
);

^INCLUDE:WaveFuncs^


//##



// #########################


const float tau = 6.28318530717958647692;

// Gamma correction
#define GAMMA (2.2)

vec3 ToLinear( in vec3 col )
{
    // simulate a monitor, converting colour values into light values
    return pow( col, vec3(GAMMA) );
}

vec3 ToGamma( in vec3 col )
{
    // convert back into colour values, so the correct light will come out of the monitor
    return pow( col, vec3(1.0/GAMMA) );
}

vec4 Noise( in ivec2 x )
{
    
    vec2 coords = (vec2(x)+0.5)/bufferDim;
    
    
    return texture2D( Texture8, coords ); //, -100.0
}

// vec4 Rand( in int x )
// {
//     vec2 uv;
//     uv.x = (float(x)+0.5)/256.0;
//     uv.y = (floor(uv.x)+0.5)/256.0;
//     return texture2D( Texture8, uv, -100.0 );
// }

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}



vec4 particles( vec2 fragCoord, vec4 wp, float dir, float partSpeed, float partLength )
{
    vec3 ray = getRay();
    
    ray.z = ray.z*dir;
    
    bool offsetMod = false;
    
    if (ray.z < 0) {
        ray.z = abs(ray.z);
        offsetMod = true;
    }
    
    //ray.xy += fract(cameraPos.xy);
    //ray.z = abs(ray.z);
    // ray.xz = 2.0*(fragCoord.xy-bufferDim.xy*.5)/bufferDim.x;
    // ray.y = 1.0;

    float offset = 0.0;
    float speed2 = 0.0;
    float speed = 0.0;
    
    
    vec3 col = vec3(0.0);
    
    vec3 stp = ray/max(abs(ray.x),abs(ray.y));
    vec3 pos;
    int i;
    float z;
    float d;
    float w;
    
    vec3 c = vec3(0.0);
    int j;
    
    float offsetBase = 0.0;
    
    for (j = 0; j < 3; j++) {
        
        
        offset = curTime*partSpeed/1000.0 - cameraPos.z*0.001;// + floor(sin(pos.z) + sin(pos.x) + sin(pos.y))*0.3;
        if (offsetMod) {
            offset = 1.0-offset;
        }
        speed2 = partLength;//(cos(offset)+1.0)*2.0;
        speed = speed2+0.1;
        //offset += sin(offset)*.96;
        //offset *= 2.0;
        
        pos = 2.0*stp+0.5;
        
        offsetBase = offset;
        
        for (i = 0; i < 20; i++ )
        {
            
            offset = offsetBase + float(j)*0.3*floor((pos.x+pos.y)*16.0)/16.0;// + floor(sin(pos.x*20.0 + pos.y*20.0)*100.0);
            
            z = Noise(ivec2(pos.xy)).x; // + cameraPos.xy
            
            
            
            z = fract(z-offset);
            
            
            
            d = 50.0*z-pos.z;
            w = pow(
                max(
                    0.0,
                    1.0-16.0*length(fract(pos.xy)-.5)
                ),
                2.0
            );
            
            c = clamp(
                vec3(
                    1.0-abs(d+speed2*.5)/speed,
                    1.0-abs(d)/speed,
                    1.0-abs(d-speed2*.5)/speed
                ),
                
                vec3(0.0),
                vec3(1.0)
            );
            col += 1.5*(1.0-z)*c*w*clamp(bufferDim.x*0.02-length(pos),0.0,1.0);
            pos += stp;
            
            
        }
    }
    
    
    
    
    return vec4(ToGamma(col)*abs(ray.z),1.0);
}


// #########################

void main() {

    int i;
    vec4 oneVec = vec4(1.0);

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
    
    
    //vec4 tex4 = texture2D(Texture4, TexCoord0.xy);    
    //vec4 tex5 = texture2D(Texture5, TexCoord0.xy);
    vec4 tex4 = texture2D(Texture4, TexCoord0.xy);
    
    vec4 tex9 = texture2D(Texture9, TexCoord0.xy);
    vec4 tex10 = texture2D(Texture10, TexCoord0.xy);    
    vec4 matValsGeom = tex10;
    bool valIsGeom = dot(matValsGeom.rgb,oneVec.rgb) != 0.0;
    
    vec4 tex11 = texture2D(Texture11, TexCoord0.xy);
    
    
    vec4 worldPosition = tex0;
    // if (tex4.w > tex0.w) {
    //     worldPosition = tex4;
    // }
    
    // if (tex9.w > tex0.w) {
    //     worldPosition = tex9;
    // }
    
    
    
    
    vec4 matVals = vec4(0.0,0.0,pack16(tex1.w));

    
    /////////////
    
    float heightDif = 1.0-clamp((seaLevel - worldPosition.z)/512.0,0.0,1.0);
    
    
    
    // ??????????
    
    vec4 samp = vec4(0.0);
    float fi;
    
    float curRad;
    float minRotInc = 0.5;
    float maxRotInc = 0.01;
    float curRotInc = 0.0;
    float curRot = 0.0;
    vec2 offsetCoord;
    float curMin = 1.0;
    float curMax = 8.0;
    vec2 newTC = vec2(0.0);
    
    //float tot = distance(worldPosition.xyz,cameraPos)/clipDist.y//worldPosition.w;
    
    vec3 finalPos = cameraPos;
    
    // if (thirdPerson != 0.0) {
    //     finalPos = entPos;
    // }
    
    float camDis = distance(worldPosition.xyz,finalPos);
    
    float camDisNorm = clamp(camDis/clipDist.y,0.0,1.0);
    
    float waveScale = 0.001;//mix(0.01/camDis,0.05/camDis,camDis/16384.0);
    
    float waveVal = 
    clamp(
        
        waveHeight2(
            //(worldPosition.xy+worldPosition.zy)*mix(0.01,0.0005,camDisNorm)
            
            
            sin(worldPosition.xy*waveScale) *
            sin(worldPosition.yz*waveScale) *
            sin(worldPosition.zx*waveScale)
            
            + (worldPosition.xy + worldPosition.zz)*waveScale
            
        )*16.0 +
        
        waveHeight2(
            //(worldPosition.xy+worldPosition.zy)*mix(0.01,0.0005,camDisNorm)
            
            
            sin(worldPosition.yx*waveScale*1.3) *
            sin(worldPosition.zy*waveScale*2.2) *
            sin(worldPosition.xz*waveScale*3.1)
            
            + (worldPosition.xy + worldPosition.zz)*waveScale*2.2
            
        )*16.0
        
        +1.0,
        0.0,
        2.0
    )*0.5;
    
    
    
    float tot = 1.0-camDisNorm;
    
    
    //tot = tot*(0.8+0.2*mix(waveVal,1.0,isUnderWater));
    
    float totMax = 0.0;
    float weightVal;
    
    float fNumSteps = float(iNumSteps);
    
    // for (i = 0; i < iNumSteps; i++)
    // {

    //     fi = float(i) / fNumSteps;

    //     curRad = mix(curMin, curMax, fi);
    //     curRotInc = mix(minRotInc, maxRotInc, fi);

    //     offsetCoord.x = cos(curRot) * curRad;
    //     offsetCoord.y = sin(curRot) * curRad;

    //     newTC = TexCoord0.xy + (offsetCoord) / (bufferDim);

        
    //     samp = texture2D(Texture0, newTC );
        
    //     weightVal = 1.0-(curRad/curMax);
        
    //     tot += samp.w*weightVal;
    //     totMax += weightVal;

    //     curRot += curRotInc;
    // }
    // tot /= totMax;
    
    // ??????????
    
    
    
    
    
    float hfog = 
        pow(clamp(
            pow(1.0-tot,0.5) //+(1.0-timeOfDay)
            
            // + (cameraPos.z - worldPosition.z)/100.0
            
            ,
            0.0,
            1.0
        ),1.0);
        
    
    
    hfog -= clamp(
        (waveVal*0.2*(1.0-isUnderWater)*(1.0-camDisNorm)),
        0.0,
        1.0
    );
    // hfog += clamp(1.0-(worldPosition.z-seaLevel)/400.0,0.0,1.0)*camDisNorm*0.5;
    // hfog = pow(hfog,0.5);
    
    hfog += clamp(1.0-(worldPosition.z-seaLevel)/200.0,0.0,1.0)*pow(camDisNorm,0.5)*0.5*(1.0-camDisNorm);
    hfog = clamp(hfog,0.0,1.0);
    
    
    if (isUnderWater == 1.0) {
        //hfog = clamp(hfog*2.0,0.0,1.0);
    }
    
    if (dot(worldPosition.xyz,oneVec.xyz) == 0.0) {
        hfog = 1.0;
    }

    //vec3 lightMod = pow( (1.0-timeOfDay)*tex3.rgb, vec3(2.0) );

    vec3 fogColor = getFogColor();
    
    if (isUnderWater == 1.0) {
        fogColor = 
        mix(
            mix(
                vec3(0.0,0.25,0.6),
                vec3(0.0,0.1,0.4),
                hfog
            ),
            fogColor,
            0.1    
        );
        
        //fogColor.g += (1.0-heightDif)*0.2;
        
    }
    
    vec3 finalCol = mix(tex2.rgb,tex3.rgb,pow(hfog,2.0)); // increase hfog for more blur
    
    
    
    
    bool isOutline = false;
    bool isSelObj = false;
    bool isActObj = false;
    bool isSelLimb = false;
    
    if (tex4.z == selLimbInd) {
        isSelLimb = true;
    }
    if (tex4.w == selObjInd) {
        isSelObj = true;
    }
    if (tex4.w == actObjInd) {
        isActObj = true;
    }
    for (i = 0; i < 4; i++) {
        newTC = TexCoord0.xy + dirVecs[i].xy*1.0/bufferDim;
        samp = texture2D(Texture4, newTC );
        if (samp.w != tex4.w) {
            isOutline = true;
        }
        if (samp.w == selObjInd) {
            isSelObj = true;
        }
        if (samp.w == actObjInd) {
            isActObj = true;
        }
    }
    
    if (tex4.w == 0.0) {
        
    }
    else {
        isOutline = false;
    }
    
    // for (i = 0; i < 4; i++) {
    //     newTC = TexCoord0.xy + dirVecs[i].xy*1.0/bufferDim;
    //     samp = texture2D(Texture0, newTC );
    //     if (distance(samp.xyz,tex0.xyz) > 0.1 ) {
    //         isOutline = true;
    //     }
    // }
    
    
    // if (tex4.w < tex0.w) {
    //     isActObj = false;
    //     isSelObj = false;
    //     isOutline = false;
    // }
    
    vec3 finalColOrig = finalCol;
    
    float stripeVal = float(sin(
        (TexCoord0.x + TexCoord0.y)*(bufferDim.y/5.0) + curTime/50.0     
    ) > 0.0);
    
    if (selObjInd == 0) {
        isSelObj = false;
    }
    if (actObjInd == 0) {
        isActObj = false;
    }
    
    if (isSelLimb) {
        finalCol += vec3(1.0,0.0,1.0)*0.25;
    }
    
    if (isOutline) {
        
        
        
        
        if (isSelObj) { //&&isActObj
            if (isSelObj) {
                
                
                                
                finalCol += stripeVal;
                //vec3(1.0,0.0,0.0)*(0.5+0.5*abs(sin(curTime/200.0)));
            }
            if (isActObj) {
                
                if (isSelObj) {
                    finalCol *= vec3(0.0,1.0,0.0);
                }
                else {
                    finalCol += vec3(0.0,1.0,0.0);
                }
                
                if (isFalling) {
                    finalCol = vec3(1.0,0.0,0.0);
                }
                if (isJumping) {
                    finalCol = vec3(1.0,0.0,1.0);
                }
                
            }
            
        }
        else {
            finalCol *= vec3(0.25);
            // finalCol += vec3(1.0,1.0,1.0)*0.25;
        }
        
        
        // if (tex4.w < tex0.w) {
        //     finalCol = mix(finalCol,finalColOrig,0.9);
        // }
        
    }
    
    
    
    
    
    
    
    
    
    
    if (isUnderWater == 1.0) {
        finalCol = mix( finalCol, vec3(0.0,0.1,0.5), 0.5);
        
        
        waveScale = 0.125;
        finalCol += pow(
        
        
        (sin(waveHeight3(
            //(worldPosition.xy+worldPosition.zy)*mix(0.01,0.0005,camDisNorm)
            
            
            sin(worldPosition.xy*waveScale) *
            sin(worldPosition.yz*waveScale) *
            sin(worldPosition.zx*waveScale)
            
            + (worldPosition.xy + worldPosition.zz*0.5)*waveScale,
            
            8.0
            
        )*16.0)+1.0)*0.4
            
            
        ,8.0)*0.3*heightDif*float(matVals.a != TEX_WATER);
        
        
        
        
    }





    
    
    
    




    
    finalCol = mix(
        finalCol,
        fogColor, 
        pow(hfog,3.0) // *mix(1.0,0.75,isUnderWater)
    );
    
    // + lightMod*2.0*(hfog);
    
    float waterMod = clamp((1.0-worldPosition.w)*16.0,0.0,1.0);
    
    // if (isUnderWater == 1.0) {
    //     finalCol.rgb = 
        
            
    //         mix(
    //             finalCol.rgb,
    //             mix(
                    
    //                 mix(
    //                     finalCol.rgb,
    //                     dot(finalCol.rgb,oneVec.rgb)*vec3(0.15,0.3,1.0),
    //                     0.5
    //                 )*0.75,
    //                 mix(
    //                     finalCol.rgb,
    //                     dot(finalCol.rgb,oneVec.rgb)*vec3(0.1,0.2,0.5),
    //                     0.95
    //                 )*0.5,
                    
    //                 pow(clamp(waterMod*float(worldPosition.w != 0.0),0.0,1.0),0.5)       
    //             ),
    //            clamp(waterMod,0.0,1.0)*0.5+0.5
    //         );
    // }
    
    
    
    if (isUnderWater == 1.0) {
        // bubbles
        vec3 myRay = getRay();
        
        float newTime = (mod(curTime*0.1/1000.0,1.0) + 4.0);
        
        vec2 moveVec = -(vec2(TexCoord0.xy-0.5)*2.0)*newTime;
        vec2 moveVec2 = vec2(0.0,-1.0)*newTime;
        
        finalCol += 
        pow(
            mix(
                
                texture2D(Texture8, TexCoord0.xy + moveVec2  ).rgb,
                texture2D(Texture8, TexCoord0.xy - moveVec  ).rgb,
                abs(lookAtVec.z)
            )
            *0.85,
            vec3(10.0)
        )
        * 
        abs(sin(rand(TexCoord0.xy)*1000.0 + newTime))*3.0
        * float(matVals.a != TEX_WATER)
        * clamp(
            distance(cameraPos,worldPosition.xyz)/512.0,
            0.0,
            1.0
        )
        *heightDif
        ;
    }
    
    
    
    
    
    
    float cellSize = 1.0;// *cellsPerBlock;
    vec3 grid0 = 
        //floor(worldPosition.xyz/cellSize);
        abs(mod(worldPosition.xyz, cellSize) - cellSize / 2.0) * 2.0;
    
    float unitBuf = (cellSize - cellSize/8.0);
    float unitBuf2 = (cellSize - cellSize/32.0);
    
    float gridVal0 = float(
        (grid0.x >= unitBuf) ||
        (grid0.y >= unitBuf)
    );
    
    float gridVal1 = float(
        (grid0.x >= unitBuf2) ||
        (grid0.y >= unitBuf2)
    );
    
    
    
    //gridVal0 = max(gridVal0 - abs(lookAtVec),vec3(0.0));
    
    float disMod = clamp(1.0-distance(worldPosition.xyz, entPos.xyz)/16.0,0.0,1.0);
    
    //gridVal0 *= disMod;
    //gridVal1 *= disMod;
    
    
    if ( (!gridOn) || (dot(abs(entPos.xyz),oneVec.xyz) == 0.0) || isOutline || (tex4.w != 0.0) ) {
        gridVal0 = (0.0);
        gridVal1 = (0.0);
    }
    
    
    
    finalCol.rgb += vec3(0.0,(gridVal1*0.5+gridVal0*0.5)*disMod,0.0);
    

    if (valIsGeom&&(!isOutline)) {
        
        if (worldPosition.w < tex9.w) {
            finalCol = mix(finalCol,tex10.rgb,0.5);
        }
    }
    
    int maxDiam = 5;
    int maxRad = maxDiam/2;
    ivec2 patXY = ivec2(patternTarg.xy);
    ivec2 worldXY = ivec2(worldPosition.xy);
    ivec2 offXY = patXY-worldXY;
    ivec2 absXY = offXY;
    
    if (absXY.x < 0) {absXY.x *= -1;}
    if (absXY.y < 0) {absXY.y *= -1;}
    
    vec3 lastCol;
    
    offXY += ivec2(maxRad,maxRad);
    
    if (placingPattern) {
        
        if (
            (absXY.x <= maxRad) &&
            (absXY.y <= maxRad)    
        ) {
           
            //
            
            if (isOutline || (tex4.w != 0.0)) {
                
            }
            else {
                if (patternCells[offXY.x + offXY.y*maxDiam] > 0.0) {
                    
                    lastCol = finalCol.rgb;
                    
                    if (gridVal0 < 0.5) {
                        finalCol.rgb *= 0.5;
                    }
                    
                    
                    finalCol.r += pow(0.5*(1.0-gridVal0)*(sin(curTime/200.0 + max(
                        grid0.x, grid0.y    
                    )*8.0)+1.0),4.0);
                    
                    //finalCol.rgb = mix(finalCol.rgb,lastCol,0.5);
                    
                }
            }
            
            
        }
        
    }
    
    
    //finalCol = vec3(tex7.a);
    
    //finalCol = vec3(stripeVal);

    vec4 particleRes;
    
    // if (isUnderWater == 1.0) {
    //     particleRes = particles(TexCoord0.xy*bufferDim.xy, worldPosition, -1.0, 0.05, 0.05)*0.1;
    //     finalCol.rgb += particleRes.rgb;
    // }
    
    
    // if (tex4.w > 0.0) {
    //     finalCol = vec3(1.0,0.0,0.0);
    // }
    
    //finalCol = vec3(tex4.w/1000.0);


    if (testOn2) {
        finalCol = tex11.rgb;
    }

    gl_FragData[0] = vec4(finalCol,1.0);

}
