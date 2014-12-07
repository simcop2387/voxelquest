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

uniform float seaLevel;
uniform float timeOfDay;
uniform vec2 bufferDim;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec4 fogPos;


const float TEX_WATER = 32.0/255.0;

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

vec3 getFogColor(float lv)
{
    float lvBase = (lv-0.5)*0.65 + 0.5;
    
    float newlv = clamp( 1.0 - pow( (lvBase - 0.5) * 2.0, 2.0 ), 0.0, 1.0);
    float newlv2 = clamp( 1.0 - pow( (lvBase - 0.5) * 4.0 - 1.0, 2.0 ), 0.0, 1.0);

    vec3 fogColor1 = vec3(0.0);
    vec3 fogColor2 = vec3(0.0);

    // 0: moon is high, 1: sun is high

    float timeLerp = 0.0;

    if (timeOfDay < 0.5)
    {
        timeLerp = timeOfDay * 2.0;

        fogColor1 = mix(
                                    vec3(0.0, 0.0, 0.05),
                                    vec3(0.3, 0.1, 0.05),
                                    timeLerp
                                );
        fogColor2 = mix(
                                    vec3(0.025, 0.0, 0.1),
                                    vec3(1.0, 0.8, 0.7),
                                    timeLerp
                                );
    }
    else
    {
        timeLerp = (timeOfDay - 0.5) * 2.0;

        fogColor1 = mix(
                                    vec3(0.3, 0.1, 0.05),
                                    vec3(0.05, 0.1, 0.3),
                                    timeLerp
                                );
        fogColor2 = mix(
                                    vec3(1.0, 0.8, 0.7),
                                    vec3(0.7, 0.8, 1.0),
                                    timeLerp
                                );
    }

    return mix(fogColor1, fogColor2, newlv ) + pow(newlv2, 4.0) * timeLerp / 4.0;
}

void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
    
    // vec4 tex4 = texture2D(Texture4, TexCoord0.xy);
    // vec4 tex5 = texture2D(Texture5, TexCoord0.xy);
    // vec4 tex6 = texture2D(Texture6, TexCoord0.xy);
    // vec4 tex7 = texture2D(Texture7, TexCoord0.xy);

    
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
            ),
            0.0,
            1.0
        ),2.0);
        

    vec3 lightMod = pow( (1.0-timeOfDay)*tex3.rgb, vec3(2.0) );

    vec3 fogColor = getFogColor(TexCoord0.y);

    vec3 finalCol = mix(tex2.rgb,tex3.rgb,hfog);


    

    finalCol = mix(
        finalCol,
        fogColor, 
        hfog*mix(1.0,0.75,isUW)
    ) + lightMod*2.0*(hfog);
    
    if (isUW == 1.0) {
        finalCol.rgb = 
        
            
            
            
            mix(
                
                mix(
                    finalCol.rgb,
                    dot(finalCol.rgb,oneVec.rgb)*vec3(0.15,0.3,1.0),
                    0.5
                )*0.75,
                mix(
                    finalCol.rgb,
                    dot(finalCol.rgb,oneVec.rgb)*vec3(0.0,0.1,0.3),
                    0.95
                )*0.5,
                
                clamp((seaLevel-worldPosition.z)/(2048.0)*float(worldPosition.w != 0.0),0.0,1.0)       
            );
            
            
            
        

    }
    

    if (valIsGeom) {
        finalCol = tex2.rgb;
    }

    gl_FragData[0] = vec4(finalCol,1.0);

}
