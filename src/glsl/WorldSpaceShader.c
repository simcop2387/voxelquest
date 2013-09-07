uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform sampler2D Texture2;
uniform sampler2D Texture3;

varying vec2 TexCoord0;
uniform vec2 mouseCoords;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec3 lightPosWS;
uniform vec2 lightPosSS;
uniform vec2 bufferDim;
uniform float cameraZoom;


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
    //vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    //vec4 tex3 = texture2D(Texture3, TexCoord0.xy);

    vec4 texFinal = tex0;

    texFinal.b = mix( tex0.b, tex2.b, float(tex2.b > 0.0) );

    float newZoom = min(cameraZoom,1.0);
    float baseHeight = unpack16(texFinal.rg);

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

    gl_FragData[0] = vec4( worldPosition.xyz, texFinal.b*255.0 );

}
