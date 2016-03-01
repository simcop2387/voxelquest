#version 120
#extension GL_EXT_frag_depth : enable

uniform mat4 modelview;
uniform mat4 proj;
uniform mat4 objmat;

uniform float objectId;
uniform vec3 cameraPos;
uniform float isWire;
uniform vec3 matVal;
uniform vec3 lookAtVec;
uniform vec4 rotationZ;

varying vec3 TexCoord0;

varying vec4 worldPos;

varying vec4 screenPos;
varying float camDis;
uniform float clipDist;

$

const float M_PI = 3.14159265359;

void main() {

    TexCoord0 = gl_MultiTexCoord0.xyz;
    
    vec4 newPos = gl_Vertex;
    
    newPos = objmat * newPos;
    
    
    mat2 m2x2;
    
    float rotInv = (M_PI*2.0-rotationZ.w)+M_PI/2.0;
    
    if (rotationZ.w != 0.0) {
        newPos.xyz -= rotationZ.xyz;
        
        m2x2 = mat2(
            cos(rotInv),
            -sin(rotInv),
            sin(rotInv),
            cos(rotInv)
        );
        
        newPos.xy = m2x2*newPos.xy;
        
        newPos.xyz += rotationZ.xyz;
    }
    
    worldPos = newPos;
    
    mat4 myMat = proj*modelview;// gl_ModelViewProjectionMatrix;
    
    screenPos = myMat * newPos;
    camDis = distance(cameraPos.xyz,newPos.xyz);
    
    gl_Position = screenPos;
    
}

$

void main() {

    
    
    float rad = 0.95;

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
    
    gl_FragDepthEXT = camDis/clipDist;
    vec3 newMat = matVal.rgb/255.0;
    
    
    
    // vec2 curTC = vec2(0.0);
    // vec3 newTC = (TexCoord0.xyz+1.0)*0.5;
    // if (abs(newTC.z-0.5) == 0.5) {
    //     curTC = newTC.xy;
    // }
    // else {
    //     if (abs(newTC.y-0.5) == 0.5) {
    //         curTC = newTC.xz;
    //     }
    //     else {
    //         curTC = newTC.yz;
    //     }
    // }
    // vec4 newTex = texture2D(Texture0,curTC.xy);
    // newMat.rgb = newMat.rgb*0.5 + newTex.rgb;
    
    
    
    //camDis = distance(cameraPos.xyz,newVert.xyz);
    float zbVal = 1.0-camDis/clipDist;
    //float zbVal = 1.0-screenPos.z/clipDist;

    gl_FragData[0] = vec4((worldPos.xyz),zbVal);//vec4(heightPacked.rg,matPacked.rg);//vec4(bhr,bhg,3.0/255.0,tex0.a);
    gl_FragData[1] = vec4(newMat.rgb,objectId);//vec4(resNorm.rgb, (TexCoord0.z+tex1.a)/2.0 ); //(TexCoord0.xyz+1.0)/2.0

}
