#version 120


varying vec2 TexCoord0;

uniform float curTime;
uniform float cameraZoom;
uniform vec3 cameraPos;
uniform vec2 bufferDim;


$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;// = ;//TexCoord0 = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
}

$

vec3 randv(vec2 co) {
    return vec3(
        fract(sin(dot(co.xy ,vec2(12.989, 78.233))) * 43758.8563),
        fract(sin(dot(co.yx ,vec2(93.989, 67.345))) * 24634.6345),
        fract(sin(dot(co.yx ,vec2(43.332, 93.532))) * 56445.2345)
    );
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    float newZoom = min(cameraZoom,1.0);

    // vec2 tcMod = (vec2(TexCoord0.x,1.0-TexCoord0.y)*2.0-1.0 );
    // tcMod.x *= bufferDim.x/(newZoom);
    // tcMod.y *= bufferDim.y/(newZoom);
    // tcMod.y -= cameraPos.z;

    // vec4 oneVec = vec4(1.0);

    // vec3 worldPosition = vec3(0.0,0.0,0.0);
    // worldPosition.x = tcMod.y + tcMod.x/2.0 + (baseHeight);
    // worldPosition.y = tcMod.y - tcMod.x/2.0 + (baseHeight);
    // worldPosition.x += cameraPos.x;
    // worldPosition.y += cameraPos.y;
    // worldPosition.z = baseHeight;

    // vec3 worldPositionWater = vec3(0.0,0.0,0.0);
    // worldPositionWater.x = tcMod.y + tcMod.x/2.0 + (baseHeightWater);
    // worldPositionWater.y = tcMod.y - tcMod.x/2.0 + (baseHeightWater);
    // worldPositionWater.x += cameraPos.x;
    // worldPositionWater.y += cameraPos.y;
    // worldPositionWater.z = baseHeightWater;


    gl_FragData[0] = vec4(randv(TexCoord0.xy),1.0);


    
}