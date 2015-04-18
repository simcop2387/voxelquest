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
uniform sampler2D Texture7;

uniform float curTime;
uniform float selObjInd;
uniform float actObjInd;
uniform float seaLevel;
uniform float timeOfDay;
uniform vec2 bufferDim;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec3 lookAtVec;
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

void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
    
    
    vec4 tex4 = texture2D(Texture4, TexCoord0.xy);
    
    vec4 tex7 = texture2D(Texture7, TexCoord0.xy);
    

    
    vec4 oneVec = vec4(1.0);
    
    vec4 tex5 = texture2D(Texture5, TexCoord0.xy);    
    vec4 matValsGeom = tex5;
    bool valIsGeom = dot(matValsGeom.rgb,oneVec.rgb) != 0.0;
    
    
    
    
    // vec3 worldPosition = tex7.xyz;
    // float baseHeight = tex7.z;
    
    // if (tex6.z > tex7.z) {
    //     worldPosition = tex6.xyz;
    //     baseHeight = tex6.z;
    // }
    
    vec4 worldPosition = tex0;
    
    worldPosition.w = max(tex0.w,tex4.w);
    
    float baseHeight = worldPosition.w;
    
    

    /////////////

    //vec3 newFog = vec3(0.0);
    //vec3 fogXYZ = vec3(0.0);
    //float fogLen = 0.0;
    //float hfog = 1.0;
    
    
    vec4 matVals = vec4(0.0,0.0,pack16(tex1.w));

    

    // newFog = (fogPos.xyz-worldPosition.xyz);
    // newFog /= 4096.0;
    // newFog.xy /= 2.0;
    // fogXYZ = 1.0-clamp( newFog, 0.0, 1.0);
    // fogLen = 1.0-clamp(1.0-(fogXYZ.x*fogXYZ.y),0.0,1.0);
    // hfog = min(clamp(sqrt(fogLen),0.0,1.0),fogXYZ.z);
    // hfog *= float(baseHeight > 0.0);
    // //hfog = clamp(hfog+clamp(1.0-waveh,0.0,1.0)/2.0,0.0,1.0);
    // hfog = pow( hfog , 2.0);
    // hfog = 1.0-clamp(hfog,0.0,1.0);


    /////////////
    
    
    float isUW = 0.0;
    
    // if (
    //     (
    //     (matVals.a == TEX_WATER) ||
    //     (tex0.w == 0.0)
    //     )
    // ) {
        
    // }
    // else {
    //     isUW = float(
    //         //(worldPosition.z < seaLevel) ||
    //         (cameraPos.z < seaLevel)
    //     );
    // }
    
    // isUW = float(
    //     //(worldPosition.z < seaLevel) ||
    //     (cameraPos.z < seaLevel)
    // );
    
    if (cameraPos.z > seaLevel) {
        isUW = 
            clamp((seaLevel-worldPosition.z)/512.0,0.0,1.0) *
                float(
                    (matVals.a != TEX_WATER) &&
                    (tex0.w != 0.0)
                );
              
    }
    else {
        isUW = 1.0;
    }
    
    
    float hfog = 
        pow(clamp(
            mix(
                0.0,
                2.0,
                (1.0-worldPosition.w) + mix(0.0,0.25,isUW)
            ) - tex7.a*0.1,
            0.0,
            1.0
        ),0.75);
        

    vec3 lightMod = pow( (1.0-timeOfDay)*tex3.rgb, vec3(2.0) );

    vec3 fogColor = getFogColor(TexCoord0.xy);

    vec3 finalCol = mix(tex2.rgb,tex3.rgb,hfog); // increase hfog for more blur


    finalCol = mix(
        finalCol,
        fogColor, 
        hfog*mix(1.0,0.75,isUW)
    ) + lightMod*2.0*(hfog);
    
    if (cameraPos.z < seaLevel) {
        finalCol.rgb = 
        
            
            mix(
                finalCol.rgb,
                mix(
                    
                    mix(
                        finalCol.rgb,
                        dot(finalCol.rgb,oneVec.rgb)*vec3(0.15,0.3,1.0),
                        0.5
                    )*0.75,
                    mix(
                        finalCol.rgb,
                        dot(finalCol.rgb,oneVec.rgb)*vec3(0.1,0.2,0.5),
                        0.95
                    )*0.5,
                    
                    pow(clamp((seaLevel-worldPosition.z)/(2048.0)*float(worldPosition.w != 0.0),0.0,1.0),0.5)       
                ),
               clamp((seaLevel-worldPosition.z)/(512.0),0.0,1.0)*0.5+0.5
            );
    }
    
    

    
    int i;
    bool isOutline = false;
    bool isSelObj = false;
    bool isActObj = false;
    
    vec2 newTC = vec2(0.0);
    vec4 samp = vec4(0.0);
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
                
                
                                
                finalCol.rgb += stripeVal;
                //vec3(1.0,0.0,0.0)*(0.5+0.5*abs(sin(curTime/200.0)));
            }
            if (isActObj) {
                
                if (isSelObj) {
                    finalCol.rgb *= vec3(0.0,1.0,0.0);
                }
                else {
                    finalCol.rgb += vec3(0.0,1.0,0.0);
                }
                
                
            }
            
        }
        else {
            finalCol.rgb += vec3(1.0,1.0,1.0)*0.25;
        }
        
        
    }
    
    

    if (valIsGeom&&(!isOutline)) {
        finalCol = tex2.rgb;
    }
    
    //finalCol = vec3(stripeVal);

    gl_FragData[0] = vec4(finalCol,1.0);

}
