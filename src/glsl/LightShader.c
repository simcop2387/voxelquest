#version 330

uniform sampler2D Texture0;

uniform float voxelsPerCell;
uniform int cellsPerHolder;
uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform vec3 lightVec;

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



void main() {

    vec4 oneVec = vec4(1.0);

    vec2 TexCoord0 = gl_FragCoord.xy/(bufferDim.xy);
    vec4 tex = texture(Texture0,TexCoord0.xy);

    vec3 finalCol = mod(tex.xyz+0.01,1.0);

    FragColor0 = vec4(
        finalCol.xyz,
        1.0
    );

}









