#version 120

uniform bool skippedPrim;

//solidBaseTargFBO
uniform sampler2D Texture0;
uniform sampler2D Texture1;

//geomTargFBO
uniform sampler2D Texture2;
uniform sampler2D Texture3;

varying vec2 TexCoord0;

^INCLUDE:MATERIALS^

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$

void main() {
    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);
    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
    
    // vec2 curTex = vec2(TEX_EARTH,0.5);
    // float curMat = floor(curTex.x*256.0*255.0) + floor(curTex.y*255.0);
    
    //tex3.w = curMat;
    
    if (skippedPrim) {
        
    }
    else {
        if (tex2.w > tex0.w) {
            tex0 = tex2;
            tex1 = tex3;
        }
    }
    
    
    
    gl_FragData[0] = tex0;
    gl_FragData[1] = tex1;
}


