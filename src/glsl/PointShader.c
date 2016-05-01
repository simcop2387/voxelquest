#version 330

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;

uniform float voxelsPerCell;
uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform vec2 hvMult;
uniform int totRad;
uniform bool lastPass;
uniform mat4 modelviewInverse;



$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 data0;

out vec4 worldPos;

void main() {
    
    worldPos = vec4(vposition.xyz,1.0);
    gl_Position = worldPos;
    
}

$

in vec4 worldPos;

layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;
layout(location = 2) out vec4 FragColor2;

^INCLUDE:MATERIALS^

^INCLUDE:RayFuncs^



void main() {

    vec4 oneVec = vec4(1.0);

    vec2 TexCoord0 = gl_FragCoord.xy/(bufferDim.xy);
    vec3 ro = vec3(0.0);
    vec3 rd = vec3(0.0);
    getRay(TexCoord0,ro,rd);
    
    vec4 tex0 = texture(Texture0,TexCoord0.xy);
    vec4 tex1 = texture(Texture1,TexCoord0.xy);
    vec4 tex2 = texture(Texture2,TexCoord0.xy);
    
    
    
    
    
    
    int i;
    int j;
    vec2 tc;
    
    float voxelWidth = 0.5/voxelsPerCell;
    float resWidth = 0.0;
    float camDis = 0.0;
    
    
    // vec3 voxelMod1 = vec3(voxelWidth);
    // vec3 voxelMod2 = vec3(voxelWidth);
    // vec3 vm = vec3(voxelMod1);
    //voxelMod2.z *= 8.0;
    
    
    // int radh = int(hvRad.x);
    // int radv = int(hvRad.y);
    
    
    float INVALID = 99999.0;
    
    float mipLevel;
    float bestBox = INVALID;
    float bestDis = INVALID;
    float testDis = 0.0;
    
    vec4 absSamp = vec4(0.0);
    vec4 samp = vec4(0.0);
    vec4 bestSamp0 = tex0;
    vec4 bestSamp1 = tex1;
    vec4 bestSamp2 = tex2;

    vec2 curCoord = vec2(0.0);
    vec2 offVal = vec2(0.0);
    
    vec2 boxVal;

    
    
    for (i = -totRad; i <= totRad; i++ ) {
        tc.x = float(i);
        tc.y = tc.x;
        tc.xy *= hvMult;

        offVal = tc/bufferDim;
        curCoord = TexCoord0 + offVal;
        samp = texture2D(Texture0,curCoord);

        absSamp = abs(samp);

        if (dot(absSamp.xyz,oneVec.xyz) > 0.0) {
            
            // if (distance(samp.w,TEX_GRASS*65280.0) < 4.0 ) {
            //     vm = voxelMod2;
            // }
            // else {
            //     vm = voxelMod1;
            // }
            
            mipLevel = 
            resWidth = pow(2.0,samp.w)*voxelWidth;
            
            boxVal = aabbIntersect(ro,rd,samp.xyz-resWidth,samp.xyz+resWidth);
            
            if (boxVal.x <= boxVal.y) {
                camDis = distance(cameraPos.xyz,samp.xyz);
                testDis = boxVal.x+camDis;
                if (testDis < bestDis) {
                    bestBox = boxVal.x;
                    bestDis = testDis;
                    bestSamp0 = samp;
                    bestSamp1 = texture2D(Texture1,curCoord);
                    bestSamp2 = texture2D(Texture2,curCoord);
                }
            }
            
            
        }           

    }
    
    
    if (lastPass&&(bestBox != INVALID)) {
        bestSamp0.xyz = ro + rd*bestBox;
    }
    

    FragColor0 = bestSamp0;
    FragColor1 = bestSamp1;
    FragColor2 = bestSamp2;

}









