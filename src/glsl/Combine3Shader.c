#version 330

// rasterFBO
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;

// rasterLowFBO
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform sampler2D Texture5;

uniform vec2 bufferDim;
uniform vec3 cameraPos;


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



void main() {

    vec4 oneVec = vec4(1.0);

    vec2 TexCoord0 = gl_FragCoord.xy/(bufferDim.xy);
    
    vec4 tex0 = texture(Texture0,TexCoord0.xy);
    vec4 tex1 = texture(Texture1,TexCoord0.xy);
    vec4 tex2 = texture(Texture2,TexCoord0.xy);
    
    vec4 tex3 = texture(Texture3,TexCoord0.xy);
    vec4 tex4 = texture(Texture4,TexCoord0.xy);
    vec4 tex5 = texture(Texture5,TexCoord0.xy);
    
    vec4 bestSamp0 = tex0;
    vec4 bestSamp1 = tex1;
    vec4 bestSamp2 = tex2;
    
    

    if (dot(tex3.xyz,oneVec.xyz) == 0.0) {
        
    }
    else {
        if (
            distance(tex3.xyz,cameraPos.xyz) <
            distance(bestSamp0.xyz,cameraPos.xyz) 
        ) {
            bestSamp0 = tex3;
            bestSamp1 = tex4;
            bestSamp2 = tex5;
        }
    }



    FragColor0 = bestSamp0;
    FragColor1 = bestSamp1;
    FragColor2 = bestSamp2;
}









