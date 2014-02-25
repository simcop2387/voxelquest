#version 120

// old combine
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// water
uniform sampler2D Texture2;
uniform sampler2D Texture3;

// water height
uniform sampler2D Texture4;

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


$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;// = ;//TexCoord0 = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
}

$



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

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
    vec4 tex3Orig = tex3;

    vec4 tex4 = texture2D(Texture4, TexCoord0.xy);

    float newZoom = min(cameraZoom,1.0);    
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


    float seaLev = seaLevel*heightmapMax;

    int i;
    float fi;
    const int numSteps = 64;
    const float fNumSteps = float(numSteps);
    float flerp = 0.0;

    const float maxWaveHeight = 256.0;

    //float tallestHeight = seaLev;
    //float lowestHeight = seaLev-maxWaveHeight;
    float curHeight = 0.0;

    
    float curWaterHeight = 0.0;
    float waveDif = 0.0;

    vec2 newTC = vec2(0.0);
    vec4 samp = vec4(0.0);

    bool hitWater = false;
    bool hitLand = false;


    vec2 newTC2 = vec2(0.0);
    vec4 samp2 = vec4(0.0);
    vec4 samp3 = vec4(0.0);
    bool isTopOfWater = false;


    int i2;
    float fi2 = 0.0;
    const int numSteps2 = 64;
    const float fNumSteps2 = float(numSteps2);
    float flerp2 = 0.0;


    bool hitAir = false;
    bool hitWaterSide = false;

    bool hitSideWater = (tex3Orig.a == 0.0)&&(dot(tex3Orig.rgb,oneVec.rgb) != 0.0)&&(baseHeightWater>=baseHeight);



    if (tex2.a == TEX_WATER) {
        for (i = 0; i < numSteps; i++) {
            fi = float(i);
            flerp = fi/fNumSteps;
            newTC = TexCoord0.xy + vec2(0.0,fi)/(bufferDim/newZoom);
            samp = texture2D(Texture4, newTC);

            waveDif = samp.a;//pow( (samp.a-0.2,0.0,1.0),10.0);

            curHeight = baseHeightWater-flerp*maxWaveHeight;//mix(tallestHeight, lowestHeight, flerp);
            curWaterHeight = seaLev-waveDif*maxWaveHeight;


            if ( (baseHeight > curHeight) ) { //&&(!hitSideWater)
                hitLand = true;
                break;
            }
            if ( curWaterHeight > curHeight ) {
                hitWater = true;
                break;
            }

        }

        for (i2 = 0; i2 < numSteps2; i2++) {
            fi2 = float(i2);
            flerp2 = fi2/fNumSteps2;
            newTC2 = TexCoord0.xy + vec2(0.0,fi2)/(bufferDim/newZoom);
            samp2 = texture2D(Texture2, newTC2);
            samp3 = texture2D(Texture3, newTC2);

            // if ((samp3.a == 0.0) && (tex3.a == 1.0)) {
            //     hitWaterSide = true;
            //     break;
            // }

            if (dot(oneVec,samp2) == 0.0) {
                hitAir = true;
                break;
            }

            

        }



        if ( (flerp2 < samp.a) && hitWater ) { //(tot == 0.0) &&   && (!hitWaterSide)
            isTopOfWater = true;
        }
        else {

        }

        if (hitLand || (!hitWater) || isTopOfWater ) {
            gl_FragData[0] = tex0;
            gl_FragData[1] = tex1;
        }
        else {

            tex2.rg = pack16(curHeight);
            tex3.rgb = samp.rgb;

            if (tex3.a == 0.0) {
                if (i >= 2) {
                    tex3.a = 1.0;
                }
            }

            gl_FragData[0] = tex2;
            gl_FragData[1] = tex3;

            if (tex3.a == 0.0) {
                gl_FragData[1].rgb = tex3Orig.rgb;
            }

            if (tex3Orig.b < 0.4) {
                gl_FragData[1].rgba = vec4(0.0);
            }

        }
    }
    else {
        if (baseHeight > baseHeightWater) {
            gl_FragData[0] = tex0;
            gl_FragData[1] = tex1;
        }
        else {
            gl_FragData[0] = tex2;
            gl_FragData[1] = tex3;
        }
    }






    

    
}


