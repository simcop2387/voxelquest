#version 120

// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// result fbo
uniform sampler2D Texture2;

// result fbo blurred
uniform sampler2D Texture3;

// geom fbo
uniform sampler2D Texture4;
uniform sampler2D Texture5;

// pal fbo
uniform sampler3D Texture6;

// wave fbo
//uniform sampler2D Texture7;

uniform int iNumSteps;
uniform float volSizePrim;
uniform float curTime;
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
uniform vec2 clipDist;
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
    return texture3D( Texture6, vec3(lightVal, num.r, num.g + 0.5/255.0) ).rgb;
}

vec3 getFogColor(vec2 lv)
{
    
    return unpackColor(
        vec2(
            ((1.0 - lv.y - distance(lv,vec2(0.5))*0.1) - lookAtVec.z ),
            TEX_SKY
        ),
        timeOfDay
    );
    
}


//##
const float M_PI = 3.14159265359;
const float timeScale = 0.0005;
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

//##


void main() {

    int i;

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
    
    
    vec4 tex4 = texture2D(Texture4, TexCoord0.xy);
    
    //vec4 tex7 = texture2D(Texture7, TexCoord0.xy);
    

    
    vec4 oneVec = vec4(1.0);
    
    vec4 tex5 = texture2D(Texture5, TexCoord0.xy);    
    vec4 matValsGeom = tex5;
    bool valIsGeom = dot(matValsGeom.rgb,oneVec.rgb) != 0.0;
    
    
    
    
    vec4 worldPosition = tex0;
    if (tex4.w > tex0.w) {
        worldPosition = tex4;
    }
    
    
    
    
    vec4 matVals = vec4(0.0,0.0,pack16(tex1.w));

    
    /////////////
    
    
    
    float waveVal = (waveHeight(
        sin(worldPosition.xy/2.0) *
        sin(worldPosition.yz/2.0) *
        sin(worldPosition.zx/2.0)
        
        + (worldPosition.xy + worldPosition.zz)*0.25
        
    )+1.0)*0.5;
    
    
    
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
    
    if (thirdPerson != 0.0) {
        finalPos = entPos;
    }
    
    float tot = 1.0-clamp(distance(worldPosition.xyz,finalPos)/(clipDist.y),0.0,1.0);
    
    
    tot = tot*(0.8+0.2*mix(waveVal,1.0,isUnderWater));
    
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
            (1.0-tot)*mix(1.0,1.0,isUnderWater),
            0.0,
            1.0
        ),1.0);
        

    //vec3 lightMod = pow( (1.0-timeOfDay)*tex3.rgb, vec3(2.0) );

    vec3 fogColor = getFogColor(TexCoord0.xy);
    
    if (isUnderWater == 1.0) {
        fogColor = mix(
            vec3(0.0,0.25,1.0),
            vec3(0.0,0.1,0.5),
            hfog
        );
    }
    
    vec3 finalCol = mix(tex2.rgb,tex3.rgb,pow(hfog,2.0)); // increase hfog for more blur
    
    
    
    
    bool isOutline = false;
    bool isSelObj = false;
    bool isActObj = false;
    
    
    
    if (tex5.w == selObjInd) {
        isSelObj = true;
    }
    if (tex5.w == actObjInd) {
        isActObj = true;
    }
    for (i = 0; i < 4; i++) {
        newTC = TexCoord0.xy + dirVecs[i].xy*1.0/bufferDim;
        samp = texture2D(Texture5, newTC );
        if (samp.w != tex5.w) {
            isOutline = true;
        }
        if (samp.w == selObjInd) {
            isSelObj = true;
        }
        if (samp.w == actObjInd) {
            isActObj = true;
        }
    }
    
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
    if (isOutline) {
        
        if (isSelObj||isActObj) {
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
                
                
            }
            
        }
        else {
            finalCol += vec3(1.0,1.0,1.0)*0.25;
        }
        
        // if (tex4.w < tex0.w) {
        //     finalCol = mix(finalCol,finalColOrig,0.9);
        // }
        
    }
    
    
    
    
    
    
    
    
    
    
    if (isUnderWater == 1.0) {
        finalCol = mix( finalCol, vec3(0.0,0.1,0.5), 0.5);
        finalCol += pow( waveVal, 8.0)*0.25;
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
    
    
    
    
    
    
    
    

    if (valIsGeom&&(!isOutline)) {
        //finalCol = tex2.rgb;
    }
    
    //finalCol = vec3(tex7.a);
    
    //finalCol = vec3(stripeVal);

    gl_FragData[0] = vec4(finalCol,1.0);

}
