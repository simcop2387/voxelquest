uniform sampler2D Texture0; // main fbo
uniform sampler2D Texture1;

uniform sampler2D Texture2; // geom fbo
uniform sampler2D Texture3;

uniform sampler2D Texture4; // water fbo
uniform sampler2D Texture5;

varying vec2 TexCoord0;
uniform vec2 mouseCoords;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec2 bufferDim;
uniform float cameraZoom;

uniform vec3 process;


$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;// = ;//TexCoord0 = gl_MultiTexCoord0;
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

    //vec4 texFinal = tex0;
    //texFinal.b = mix( tex0.b, tex2.b, float(tex2.a > 0.0) );

    float isObject = float(tex2.a > 0.0);

    float newZoom = min(cameraZoom,1.0);
    float baseHeight1 = unpack16(tex0.rg);
    float baseHeight2 = unpack16(tex2.rg);
    float baseHeight3 = unpack16(tex4.rg);
    float baseHeight = max(max(baseHeight1*process.x, baseHeight2*process.y), baseHeight3*process.z);
    //max(baseHeight1,baseHeight3);

    vec3 worldPosition = vec3(0.0,0.0,0.0);
    vec2 tcMod = (vec2(TexCoord0.x,1.0-TexCoord0.y)*2.0-1.0 );

    tcMod.x *= bufferDim.x/(newZoom);
    tcMod.y *= bufferDim.y/(newZoom);

    tcMod.y -= cameraPos.z;
    worldPosition.x = tcMod.y + tcMod.x/2.0 + (baseHeight);
    worldPosition.y = tcMod.y - tcMod.x/2.0 + (baseHeight);
    worldPosition.z = baseHeight;
    worldPosition.x += cameraPos.x;
    worldPosition.y += cameraPos.y;

    gl_FragData[0] = vec4( worldPosition.xyz, isObject ); //texFinal.b*255.0

}
