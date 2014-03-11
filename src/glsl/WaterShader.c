#version 120

// old combine
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// water combine
uniform sampler2D Texture2;
uniform sampler2D Texture3;

// rendered
uniform sampler2D Texture4;

// rendered blurred
uniform sampler2D Texture5;

// noise fbo
uniform sampler2D Texture6;

// wave fbo
uniform sampler2D Texture7;


varying vec2 TexCoord0;


uniform float seaLevel;
uniform float heightmapMax;
uniform float curTime;
uniform float cameraZoom;
uniform vec3 cameraPos;
uniform vec2 bufferDim;


const float TEX_WATER = 34.0/255.0;
const float TEX_GLASS = 35.0/255.0;
const float pi = 3.14159;

const int maxEntries = 5;
const int maxEntriesM1 = maxEntries-1;

float distances[maxEntries] = float[](0.0,32.0,128.0,256.0,512.0);

// vec3 colVecs[4] = vec3[](
//     vec3( 1.0, 1.0, 1.0 ),
//     vec3( 1.0, 0.0, 1.0 ),
//     vec3( 0.0, 1.0, 1.0 ),
//     vec3( 0.0, 1.0, 1.0 )
// );

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;// = ;//TexCoord0 = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
}

$

vec3 randv(vec2 co) {
    return vec3(
        fract(sin(dot(co.xy ,vec2(12.989, 78.233))) * 43758.8563),
        fract(sin(dot(co.yx ,vec2(93.989, 67.345))) * 24634.6345),
        fract(sin(dot(co.yx ,vec2(43.332, 93.532))) * 56445.2345)
    );
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}
float unpack16(vec2 num) {
    return num.r*255.0 + num.g*65280.0;
}
vec2 pack16(float num) {

    int iz = int(num);
    int ir = intMod(iz,256);
    int ig = (iz)/256;

    vec2 res;

    res.r = float(ir)/255.0;
    res.g = float(ig)/255.0;

    return res;

}


void main() {
    float newZoom = min(cameraZoom,1.0);

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex3 = texture2D(Texture3, TexCoord0.xy);

    vec4 tex4 = texture2D(Texture4, TexCoord0.xy);
    vec4 tex5 = texture2D(Texture5, TexCoord0.xy);

    float tot = float(tex0.r + tex0.g + tex0.b + tex0.a > 0.0);    
    float baseHeight = unpack16(tex0.rg);
    float baseHeightWater = unpack16(tex2.rg);

    vec2 tcMod = (vec2(TexCoord0.x,1.0-TexCoord0.y)*2.0-1.0 );
    tcMod.x *= bufferDim.x/(newZoom);
    tcMod.y *= bufferDim.y/(newZoom);
    tcMod.y -= cameraPos.z;

    vec4 oneVec = vec4(1.0);

    vec3 worldPosition = vec3(0.0,0.0,0.0);
    worldPosition.x = tcMod.y + tcMod.x/2.0 + (baseHeight);
    worldPosition.y = tcMod.y - tcMod.x/2.0 + (baseHeight);
    worldPosition.x += cameraPos.x;
    worldPosition.y += cameraPos.y;
    worldPosition.z = baseHeight;

    vec3 worldPositionWater = vec3(0.0,0.0,0.0);
    worldPositionWater.x = tcMod.y + tcMod.x/2.0 + (baseHeightWater);
    worldPositionWater.y = tcMod.y - tcMod.x/2.0 + (baseHeightWater);
    worldPositionWater.x += cameraPos.x;
    worldPositionWater.y += cameraPos.y;
    worldPositionWater.z = baseHeightWater;


    float heightDif = 0.0;

    vec3 waterNorm = -normalize((tex3.rgb-0.5)*2.0);

    vec2 newTC = vec2(0.0);
    vec2 resTC = vec2(0.0);


    vec4 tex0Ref = vec4(0.0);
    vec4 tex1Ref = vec4(0.0);

    vec4 tex2Ref = vec4(0.0);
    vec4 tex3Ref = vec4(0.0);

    vec4 tex4Ref = vec4(0.0);
    vec4 tex5Ref = vec4(0.0);

    vec4 tex7Ref = vec4(0.0);
    vec4 tex7Ref2 = vec4(0.0);
    vec4 tex7Ref3 = vec4(0.0);

    float baseHeightRef = 0.0;

    int i;

    float curLerp = 0.0;
    float lval;

    float lerpVal;
    float lerpValNorm = 0.0;
    float curSeaLev = seaLevel*heightmapMax;

    float totRef;


    vec3 finalCol = vec3(0.0);

    vec3 colVecs[maxEntries];

    colVecs[0] = vec3(0.0);
    colVecs[1] = vec3(0.0);
    colVecs[2] = vec3(0.0);
    colVecs[3] = vec3(0.0);
    colVecs[4] = vec3(0.0);

    if (tex2.a == TEX_WATER) {


        newTC.xy = TexCoord0.xy + (waterNorm.xy-waterNorm.z)*32.0/(bufferDim/newZoom);


        


        tex0Ref = texture2D(Texture0, newTC.xy);
        
        //tex1Ref = texture2D(Texture1, newTC.xy);
        //tex2Ref = texture2D(Texture2, newTC.xy);
        //tex3Ref = texture2D(Texture3, newTC.xy);
        //tex4Ref = texture2D(Texture4, newTC.xy);

        tex5Ref = texture2D(Texture5, newTC.xy);


        baseHeightRef = unpack16(tex0Ref.rg);

        totRef = float(dot(tex0Ref, oneVec) > 0.0);

        if (totRef == 0.0) {
            baseHeightRef = baseHeightWater-128.0;
        }

        heightDif = clamp((baseHeightWater - baseHeightRef), 0.0, distances[maxEntriesM1]);

        lval = dot(oneVec.rgb, tex5Ref.rgb)/3.0;


        tex7Ref = texture2D(Texture7, newTC.xy);
        tex7Ref2 = texture2D(Texture7, newTC.xy + tex7Ref.xy - (baseHeightWater - baseHeightRef)/bufferDim.xy );
        tex7Ref3 = texture2D(Texture7, newTC.xy + 1.0 - (tex7Ref.xy - (baseHeightWater - baseHeightRef)/bufferDim.xy) );


        // bigger == deeper under water

        colVecs[0] = vec3(0.9,0.9,1.0);
        colVecs[1] = vec3(0.3,1.0,1.0);
        colVecs[2] = vec3(0.2,0.5,1.0);
        colVecs[3] = vec3(0.1,0.3,1.0);
        colVecs[4] = vec3(0.1,0.0,0.5);

        if (tex3.a == 0.0 ) {
            heightDif = max(heightDif,distances[1]+1.0);
        }

        for (i = 0; i < maxEntriesM1; i++) {
            if (
                (heightDif >= distances[i]) &&
                (heightDif <= distances[i+1]) 
            ) {

                

                lerpVal = clamp( (heightDif - distances[i])/(distances[i+1]-distances[i]), 0.0, 1.0);
                lerpValNorm = heightDif/distances[maxEntriesM1];

                finalCol = mix(
                    mix(colVecs[i], colVecs[i+1], lerpVal)*lval,
                    mix(colVecs[i], colVecs[i+1], lerpVal)*lval*vec3(0.4,0.5,0.6) + tex4.rgb,
                    clamp(dot(oneVec.xyz,-waterNorm.xyz),0.0,0.3)
                ) + tex4.rgb*vec3(0.1,0.2,0.4);

                if (i == 0) {
                    finalCol += (rand(TexCoord0.xy*curTime/10000.0)*0.15+0.25)*(1.0-lerpVal);
                }

                if (tex3.a == 0.0 ) {

                    if ( abs(waterNorm.z) > 0.5) {

                    }
                    else {
                        // finalCol += pow(
                        //     clamp(
                        //         dot(causNorm,-waterNorm),
                        //         0.0,
                        //         1.0
                        //     )+0.4,
                        //     4.0
                        // )*0.1;

                        finalCol += sin(
                            (worldPositionWater.x + worldPositionWater.y + worldPositionWater.z/2.0)/(100.0) + curTime/500.0
                        )*0.2 * 
                        pow(1.0-clamp((curSeaLev-worldPositionWater.z)/2000.0,0.0,1.0),5.0 );

                    }
                    resTC = TexCoord0.xy;
                }
                else {
                    resTC = newTC.xy;
                    
                }

                finalCol += 
                pow(
                    texture2D(Texture6, resTC*vec2(1.0,1.0)-vec2(0.0,(curTime/20000.0))  ).rgb*0.85,
                    vec3(10.0)
                ) * 
                abs(sin(rand(TexCoord0.xy)*1000.0 + curTime/200.0))*(1.0-lerpValNorm);

            }
        }

        finalCol += pow(
        (
            //pow(abs(sin(tex7Ref.a*10.0)),4.0) +
            pow( ( (tex7Ref2.a ) ), 2.0 ) +
            pow( ( (tex7Ref3.a ) ), 2.0 ) 
        ) *
        clamp( 1.0 - ((baseHeightWater - baseHeightRef)/512.0),0.0,1.0 )*
        totRef

        ,2.0)*0.2;

        gl_FragData[0] = vec4(finalCol,1.0);
    }
    else {

        if (tex2.a == TEX_GLASS) {
            gl_FragData[0] = tex4 + tex5;
        }
        else {
            gl_FragData[0] = tex4;
        }

        
    }


    
}