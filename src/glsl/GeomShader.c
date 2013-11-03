//uniform sampler3D Texture0; //volume texture
//uniform sampler2D Texture0;
//uniform sampler2D Texture1;

//uniform sampler2D Texture0;
//uniform sampler3D Texture1;
//varying vec4 TexCoord0;
//varying vec3 TexCoord1;

//uniform float curHeight;
//uniform vec3 worldMin;
//uniform vec3 worldMax;

varying vec3 TexCoord0;

uniform float curTime;
uniform float cameraZoom;
uniform vec3 cameraPos;
uniform vec2 bufferDim;

uniform float isWire;

uniform float matVal;

varying vec3 finalVec;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xyz;
    //TexCoord1 = gl_MultiTexCoord1.xyz;

    //vec2 newTC = ((TexCoord0.xy)*newZoom+1.0)/2.0;



    vec3 myVert = gl_Vertex.xyz - cameraPos.xyz;
    vec3 transVert;

    transVert.x = (myVert.x-myVert.y);
    transVert.y = (-(myVert.x/2.0) + -(myVert.y/2.0) + myVert.z);
    transVert.z = myVert.z;

    
    float newZoom = min(cameraZoom,1.0);
    
    finalVec.x = (transVert.x)*newZoom/(bufferDim.x);
    finalVec.y = (transVert.y)*newZoom/(bufferDim.y);
    finalVec.z = gl_Vertex.z;//(transVert.z);


    //float hv = mix(0.5,0.0,float(matVal == 4.0));
    //*0.5 + hv

    gl_Position = vec4(finalVec.xy, clamp( (1.0-finalVec.z/(256.0*255.0)) ,0.0,1.0),gl_Vertex.w);

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

    vec2 res = pack16(finalVec.z);


    
    float rad = 0.99;

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
    
    
    //float bhr = mod(finalVec.z/2.0,256.0);
    //float bhg = floor((finalVec.z)/256.0);

    gl_FragData[0] = vec4(res.rg,matVal/255.0,1.0);//vec4(bhr,bhg,3.0/255.0,tex0.a);
    gl_FragData[1] = vec4((TexCoord0.xyz+1.0)/2.0,1.0);//vec4(resNorm.rgb, (TexCoord0.z+tex1.a)/2.0 );

}
