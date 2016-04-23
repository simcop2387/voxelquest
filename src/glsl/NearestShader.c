#version 330

uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform int totRad;


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



void main() {

    vec4 oneVec = vec4(1.0);

    vec2 TexCoord0 = gl_FragCoord.xy/(bufferDim.xy);
    vec4 tex0 = texture(Texture0,TexCoord0.xy);
    vec4 tex1 = texture(Texture1,TexCoord0.xy);    
    
    
    int i;
    int j;
    vec2 tc;
    
    float camDis = 0.0;
    
    float bestDis = 99999.0;
    float testDis = 0.0;
    
    vec4 absSamp = vec4(0.0);
    vec4 samp = vec4(0.0);
    vec4 bestSamp0 = tex0;
    vec4 bestSamp1 = tex1;

    vec2 curCoord = vec2(0.0);
    vec2 offVal = vec2(0.0);
    
    float baseDis = distance(cameraPos,tex0.xyz);
    
    if (dot(tex0.xyz,oneVec.xyz) == 0.0) {
        baseDis = 99999.0;   
    }
    
    
    for (j = -totRad; j <= totRad; j++ ) {
        for (i = -totRad; i <= totRad; i++ ) {
            tc.x = float(i);
            tc.y = float(j);

            offVal = tc/bufferDim;
            curCoord = TexCoord0 + offVal;
            samp = texture2D(Texture0,curCoord);
            absSamp = abs(samp);

            if (dot(absSamp.xyz,oneVec.xyz) > 0.0) {
                
                
                testDis = distance(cameraPos.xyz,samp.xyz);;
                if (testDis < bestDis) {
                    bestDis = testDis;
                    bestSamp0 = samp;
                    bestSamp1 = texture2D(Texture1,curCoord);
                }
                
                
            }           

        }
    }
    
    
    if (abs(bestDis-baseDis) > 2.0) {
        
    }
    else {
        bestSamp0 = tex0;
        bestSamp1 = tex1;
    }
      


    FragColor0 = bestSamp0;
    FragColor1 = bestSamp1;

}









