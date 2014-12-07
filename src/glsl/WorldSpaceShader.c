uniform sampler2D Texture0; // main fbo
uniform sampler2D Texture1;

uniform sampler2D Texture2; // water fbo
uniform sampler2D Texture3;

uniform sampler2D Texture4; // geom fbo
uniform sampler2D Texture5;

varying vec2 TexCoord0;


uniform vec2 resolution;
uniform vec2 mouseCoords;
uniform vec3 cameraPos;
uniform vec2 bufferDim;
uniform int iMax;



$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}

vec2 pack16(float num) {

    int iz = int(num);
    int ir = intMod(iz,256);
    int ig = (iz)/256;

    vec2 res;

    res.r = float(ir)/255.0;
    res.g = float(ig)/255.0;

    return res;

}

float unpack16(vec2 num) {
    return num.r*255.0 + num.g*65280.0;
}

void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex4 = texture2D(Texture4, TexCoord0.xy);
    vec4 tex5 = texture2D(Texture5, TexCoord0.xy);

    float objectId = unpack16(tex5.rg);

    vec3 worldPosition[4];
    
    worldPosition[0] = tex0.xyz;
    worldPosition[1] = tex2.xyz;
    worldPosition[2] = tex4.xyz;
    worldPosition[3] = tex0.xyz;
    
    int i;

    for (i = 0; i < iMax; i++) {

        gl_FragData[i] = vec4( worldPosition[i].xyz, objectId );
    }
    
}
