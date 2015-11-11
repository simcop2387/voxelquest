#version 120

// old combine
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// water
uniform sampler2D Texture2;
uniform sampler2D Texture3;

// geom fbo
uniform sampler2D Texture4;
uniform sampler2D Texture5;

// water height
//uniform sampler2D Texture6;


varying vec2 TexCoord0;

// uniform float clipDist;
// uniform float curTime;
// uniform vec3 cameraPos;
// uniform vec2 bufferDim;


^INCLUDE:MATERIALS^

const float pi = 3.14159;


$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;// = ;//TexCoord0 = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
}

$







void main() {
    vec4 oneVec = vec4(1.0);

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);
    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
    vec4 tex4 = texture2D(Texture4, TexCoord0.xy);
    vec4 tex5 = texture2D(Texture5, TexCoord0.xy);
    
    
    vec2 curTex = vec2(TEX_EARTH,0.0);
    float curMat = floor(curTex.x*256.0*255.0) + floor(curTex.y*255.0);
    
    tex5.w = curMat;
    
    
    
    // vec4 matValsGeom = tex5;
    // bool valIsGeom = (dot(matValsGeom.rgb,oneVec.rgb) != 0.0);
    
    // tex0.w = max(tex0.w,tex4.w);


    
    // vec4 tex3Orig = tex3;

    // vec4 matValsSolid = vec4(0.0,0.0,pack16(tex1.w));
    // vec4 matValsWater = vec4(0.0,0.0,pack16(tex3.w));
    
    
    
    
    // float maxWaveHeight = 0.01;//@maxWaveH@*4.0/clipDist;

    
    // float baseHeightSolid = tex0.w;
    // float baseHeightWater = tex2.w;
    // //vec3 worldPosition = tex0.xyz;
    // //vec3 worldPositionWater = tex2.xyz;
    
    

    // int i;
    // float fi;
    // const int numSteps = 64;
    // const float fNumSteps = float(numSteps);
    // float flerp = 0.0;

    

    // float curHeight = 0.0;

    
    // float curWaterHeight = 0.0;
    // float waveDif = 0.0;

    // vec2 newTC = vec2(0.0);
    // vec4 samp = vec4(0.0);

    // bool hitWater = false;
    // bool hitLand = false;


    // vec2 newTC2 = vec2(0.0);
    // vec4 samp2 = vec4(0.0);
    // vec4 samp3 = vec4(0.0);
    // bool isTopOfWater = false;


    // int i2;
    // float fi2 = 0.0;
    // const int numSteps2 = 64;
    // const float fNumSteps2 = float(numSteps2);
    // float flerp2 = 0.0;
    
    // float flerp3 = 0.0;


    // bool hitAir = false;

    // float endRes = 0.0;

    // float yOff = 0.0;
    

    // if (tex2.w > tex0.w) {
    //     if (
    //         (matValsWater.a == TEX_GLASS) ||
    //         (matValsWater.a == TEX_WATER)    
    //     ) {
    //         endRes = 1.0;
    //     }
    //     else {
    //         if (
    //             (matValsWater.a == TEX_WATER)
    //         ) {
    //             for (i = 0; i < numSteps; i++) {
    //                 fi = float(i);
    //                 flerp = fi/fNumSteps;
    //                 newTC = TexCoord0.xy + vec2(yOff,fi+yOff)/(bufferDim);
    //                 samp = texture2D(Texture6, newTC);

    //                 waveDif = samp.a;

    //                 curHeight = flerp*maxWaveHeight;
    //                 curWaterHeight = waveDif*maxWaveHeight;


    //                 if ( (baseHeightSolid > (baseHeightWater-curWaterHeight) ) ) {
    //                     hitLand = true;
    //                     break;
    //                 }
    //                 if ( curWaterHeight > curHeight ) {
    //                     hitWater = true;
    //                     break;
    //                 }

    //             }
                

    //             // for (i2 = 0; i2 < numSteps2; i2++) {
    //             //     fi2 = float(i2);
    //             //     flerp2 = fi2/fNumSteps2;
    //             //     newTC2 = TexCoord0.xy + vec2(0.0,fi2*tex2.w)/(bufferDim);
    //             //     //samp2 = texture2D(Texture2, newTC2);
    //             //     samp3 = texture2D(Texture3, newTC2);

                    

    //             //     if (samp3.w == 0.0) {
    //             //         hitAir = true;
    //             //         break;
    //             //     }
    
    //             // }
                
    //             // for (i2 = 0; i2 < numSteps2; i2++) {
    //             //     fi2 = float(i2);
    //             //     flerp3 = fi2/fNumSteps2;
    //             //     newTC2 = TexCoord0.xy - vec2(0.0,fi2*tex2.w)/(bufferDim);
    //             //     //samp2 = texture2D(Texture2, newTC2);
    //             //     samp3 = texture2D(Texture3, newTC2);

                    

    //             //     if (samp3.w == 0.0) {
    //             //         hitAir = true;
    //             //         break;
    //             //     }

                    

    //             // }
                
    //             // flerp2 = min(flerp2,flerp3);
                


    //             // if ( (flerp2 < waveDif*tex2.w) && hitWater&&hitAir ) {
    //             //     isTopOfWater = true;
    //             // }
    //             // else {

    //             // }

    //             if (hitLand||isTopOfWater) {// || (!hitWater) || isTopOfWater ) {
    //                 endRes = 0.0;
    //             }
    //             else {

    //                 //tex2.rg = pack16(curHeight);
    //                 //tex2.z = curHeight*clipDist;
                    
    //                 tex2.w = baseHeightWater-curWaterHeight;
    //                 tex2.z = tex2.z-waveDif*maxWaveHeight*512.0;
                    
                    
    //                 tex3.xyz = normalize(mix(
    //                     tex1.xyz,
    //                     normalize((samp.xyz-0.5)*2.0),
                        
                        
    //                     clamp((tex2.w-tex0.w)*1000.0,0.0,1.0)
    //                 ));

    //                 // if (tex3.a == 0.0) {
    //                 //     if (i >= 2) {
    //                 //         tex3.a = 1.0;
    //                 //     }
    //                 // }

    //                 endRes = 1.0;

    //                 //outOfWater = false;

    //                 // if (tex3.a == 0.0) {
    //                 //     gl_FragData[1].rgb = tex3Orig.rgb;
    //                 // }

    //                 // if (tex3Orig.b < 0.4) {
    //                 //     gl_FragData[1].rgba = vec4(0.0);
    //                 // }

    //             }
    //         }
    //     }
    // }
    // else {
    //     endRes = 0.0;
    // }

    

    // // if (isGeometry) {
    // //     if (outOfWater) {

    // //     }
    // //     else {
    // //         //tex0.b = 0.0/255.0;
    // //         //tex0.a = 140.0/255.0;
            
    // //     }

    // //     endRes = 0.0;
        
        
    // // }

    // // if (
    // //     (matValsWater.a == TEX_WATER) || 
    // //     (matValsWater.a == TEX_GLASS)
    // // ) {
    // //     endRes = 1.0;
    // // }


    vec4 res0 = vec4(0.0);
    vec4 res1 = vec4(0.0);
    
    if (tex0.w >= tex2.w) {
        res0 = tex0;
        res1 = tex1;
    }
    else {
        res0 = tex2;
        res1 = tex3;
    }
    
    // if (res0.w >= tex4.w) {
        
    // }
    // else {
    //     res0 = tex4;
    //     res1 = tex5;
    // }
    
    gl_FragData[0] = res0;
    gl_FragData[1] = res1;

    
    

    
}


