
varying vec3 TexCoord0;

uniform float objectId;
uniform float curTime;
uniform vec3 cameraPos;
uniform vec3 offsetPos;
uniform float isWire;
uniform vec3 matVal;

varying vec3 finalVec;
varying vec4 worldPos;

varying vec4 screenPos;
uniform float clipDist;

$


void main() {

    TexCoord0 = gl_MultiTexCoord0.xyz;
    
    vec4 newPos = gl_Vertex;
    newPos.xyz += offsetPos;
    
    worldPos = newPos;
    screenPos = gl_ModelViewProjectionMatrix * newPos;
    
    gl_Position = screenPos;
    
}

$

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


    
    float rad = 0.98;

    if (isWire == 0.0) {

    }
    else {

        if (
            ( (abs(TexCoord0.x) < rad) && (abs(TexCoord0.y) < rad) ) ||
            ( (abs(TexCoord0.y) < rad) && (abs(TexCoord0.z) < rad) ) ||
            ( (abs(TexCoord0.x) < rad) && (abs(TexCoord0.z) < rad) )
        ) {
            discard;
        }
    }

    gl_FragData[0] = vec4((worldPos.xyz),1.0-screenPos.z/clipDist);//vec4(heightPacked.rg,matPacked.rg);//vec4(bhr,bhg,3.0/255.0,tex0.a);
    gl_FragData[1] = vec4(matVal.rgb/255.0,objectId);//vec4(resNorm.rgb, (TexCoord0.z+tex1.a)/2.0 ); //(TexCoord0.xyz+1.0)/2.0

}
