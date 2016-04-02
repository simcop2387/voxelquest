#version 330

//uniform usamplerBuffer Texture0;
uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform float voxelsPerCell;
uniform int stepNum;
uniform int cellsPerHolder;
uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform vec2 hvMult;
uniform int totRad;
uniform int growSteps;
uniform mat4 modelviewInverse;

// uniform mat4 pmMatrix;



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


void getRay(in vec2 newTC, inout vec3 ro, inout vec3 rd) {
 float aspect = bufferDim.y/bufferDim.x;
 float NEAR = clipDist.x;
 float FAR = clipDist.y;

 float dx = tan(FOV*0.5f)*(newTC.x*2.0-1.0f)/aspect; //gl_FragCoord.x/(bufferDim.x*0.5)
 float dy = tan(FOV*0.5f)*((1.0f-newTC.y)*2.0-1.0f); //gl_FragCoord.y/(bufferDim.y*0.5)

 dx = -dx;

 vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
 vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);

 p1 = modelviewInverse*p1;
 p2 = modelviewInverse*p2;

 ro = p1.xyz;
 rd = normalize(p1.xyz-p2.xyz);
}

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

void main() {

    vec4 oneVec = vec4(1.0);

    vec2 TexCoord0 = gl_FragCoord.xy/(bufferDim.xy);
    vec4 tex0 = texture(Texture0,TexCoord0.xy);
    vec4 tex1 = texture(Texture1,TexCoord0.xy);

    vec3 ro = vec3(0.0);
    vec3 rd = vec3(0.0);
    
    getRay(TexCoord0,ro,rd);
    
    
    
    int i;
    int j;
    vec2 tc;
    
    float voxelWidth = 0.5/voxelsPerCell;
    float camDis = 0.0;
    
    
    // int radh = int(hvRad.x);
    // int radv = int(hvRad.y);
    
    
    float bestDis = 99999.0;
    float testDis = 0.0;
    
    vec4 absSamp = vec4(0.0);
    vec4 samp = vec4(0.0);
    vec4 bestSamp0 = tex0;
    vec4 bestSamp1 = tex1;

    vec2 curCoord = vec2(0.0);
    vec2 offVal = vec2(0.0);
    
    vec2 boxVal;

    // /float bestBox = distance(cameraPos.xyz,tex0.xyz);

    // for (j = -radv; j <= radv; j++) {
    //     tc.y = float(j);
        
    // }
    
    for (i = -totRad; i <= totRad; i++ ) {
        tc.x = float(i);
        tc.y = tc.x;
        tc.xy *= hvMult;

        offVal = tc/bufferDim;
        curCoord = TexCoord0 + offVal;
        samp = texture2D(Texture0,curCoord);

        absSamp = abs(samp);

        if (dot(absSamp.xyz,oneVec.xyz) > 0.0) {
            
            
            boxVal = aabbIntersect(ro,rd,samp.xyz-voxelWidth,samp.xyz+voxelWidth);
            
            if (
                (boxVal.x <= boxVal.y)
                // || (camDis > 50.0)
            ) {
                camDis = distance(cameraPos.xyz,samp.xyz);
                testDis = boxVal.x+camDis;
                if (testDis < bestDis) {
                    bestDis = testDis;
                    bestSamp0 = samp;
                    bestSamp1 = texture2D(Texture1,curCoord);
                    //bestBox = boxVal.x;
                }
            }
            
            
        }           

    }  



    // vec3 finalCol = 
    //     bestSamp.xyz
    // ;    
    // if (stepNum == (growSteps-1)) {
    //     //finalCol = ro+rd*bestBox;
    //     finalCol = mod(finalCol+0.01,1.0);
    // }
    
    
    //bestSamp0.xyz = ro+rd*bestBox;

    FragColor0 = bestSamp0;
    FragColor1 = bestSamp1;

}









