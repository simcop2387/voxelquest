#version 330

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;

$


layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vtexcoord;
out vec4 TexCoord0;


void main() {
    TexCoord0 = vtexcoord;
    gl_Position = vposition;
}


$

in vec4 worldPos;
in vec4 TexCoord0;
layout(location = 0) out vec4 FragColor0;

void main() {
    
    vec4 samp0 = texture(Texture0,TexCoord0.xy);
    vec4 samp1 = texture(Texture1,TexCoord0.xy);
    vec4 samp2 = vec4(0.0);//texture(Texture2,TexCoord0.xy);
    vec4 samp3 = vec4(0.0);//texture(Texture3,TexCoord0.xy);
    
    vec4 finalRes = max(
        max(
            samp0,
            samp1
        ),
        max(
            samp2,
            samp3
        )
    );
    
    FragColor0 = finalRes;
    
    // float readVals[4];
    // float writeVals[4];
    
    // readVals[0] = finalRes.x;
    // readVals[1] = finalRes.y;
    // readVals[2] = finalRes.z;
    // readVals[3] = finalRes.w;
    
    // writeVals[0] = 0.0;
    // writeVals[1] = 0.0;
    // writeVals[2] = 0.0;
    // writeVals[3] = 0.0;
    
    
    // int i;
    // int writePos = 0;
    
    // for (i = 0; i < 4; i++) {
    //     if (readVals[i] != 0.0) {
    //         writeVals[writePos] = readVals[i];
    //         writePos++;
    //     }
    // }
    
    // // if (finalRes.x == 0.0) {
    // //     finalRes.x = finalRes.y;
    // //     finalRes.y = finalRes.z;
    // //     finalRes.z = finalRes.w;
    // //     finalRes.w = 0.0;
    // // }
    // // if (finalRes.y == 0.0) {
    // //     finalRes.y = finalRes.z;
    // //     finalRes.z = finalRes.w;
    // //     finalRes.w = 0.0;
    // // }
    // // if (finalRes.z == 0.0) {
    // //     finalRes.z = finalRes.w;
    // //     finalRes.w = 0.0;
    // // }
    
    // FragColor0 = vec4(
    //     writeVals[0],
    //     writeVals[1],
    //     writeVals[2],
    //     writeVals[3]
    // );
}
