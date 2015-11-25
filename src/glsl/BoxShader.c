#version 120
#extension GL_EXT_frag_depth : enable

uniform mat4 objmat;
uniform mat4 modelview;
uniform mat4 proj;

uniform mat3 normalRot;

//uniform vec4 objQuat;

uniform vec3 lightVec;


uniform float bodyUID;
uniform float limbUID;
uniform float curTime;
uniform vec3 cameraPos;
uniform vec3 lookAtVec;
uniform float isWire;
uniform vec3 matVal;

//varying vec3 TexCoord0;

//varying vec3 curColor;
varying vec3 curNormal;
varying vec3 finalVec;
varying vec4 worldPos;

varying vec4 screenPos;
varying float camDis;
uniform float clipDist;

^INCLUDE:MATERIALS^


$

const float M_PI = 3.14159265359;

// //source: https://www.opengl.org/discussion_boards/showthread.php/166386-Quaternions-and-hardware-skinning
// vec3 rotateVec( vec4 quat, vec3 vec ) {
//     return vec + 2.0 * cross( cross( vec, quat.xyz ) + quat.w * vec, quat.xyz );
// }

void main() {

    //TexCoord0 = gl_MultiTexCoord0.xyz;
    
    vec4 newPos = gl_Vertex;
    
    newPos = objmat * newPos;
    
    // newPos.xyz += cameraPos + lookAtVec*100.0;
    
    // /curColor = gl_Color.rgb;
    //vec4 tempNorm =  vec4(gl_Normal.xyz,1.0);
    //curNormal = gl_Normal.xyz;
    
    
    
    mat3 normalMatrix = mat3(objmat);
    //normalMatrix = inverse(normalMatrix);
    //normalMatrix = transpose(normalMatrix);
    //curNormal = rotateVec(objQuat,gl_Normal.xyz); //normalize(normalMatrix*gl_Normal.xyz);
    
    curNormal = normalize(normalRot*gl_Normal.xyz);
    
    
    
    //newPos.xyz = mix(newPos.xyz,cameraPos,(sin(curTime)+1.0)*0.5);
    
    //newPos.xyz += cameraPos;
    
    
    worldPos = newPos;
    
    mat4 myMat = proj*modelview; // *objmat;// gl_ModelViewProjectionMatrix;
    
    screenPos = myMat * newPos; //
    camDis = distance(cameraPos.xyz,newPos.xyz);
    
    gl_Position = screenPos;
    
}

$

void main() {

    
    
    float rad = 0.98;

    // if (isWire == 0.0) {

    // }
    // else {

    //     if (
    //         ( (abs(TexCoord0.x) < rad) && (abs(TexCoord0.y) < rad) ) ||
    //         ( (abs(TexCoord0.y) < rad) && (abs(TexCoord0.z) < rad) ) ||
    //         ( (abs(TexCoord0.x) < rad) && (abs(TexCoord0.z) < rad) )
    //     ) {
    //         discard;
    //     }
    // }
    
    gl_FragDepthEXT = camDis/clipDist;
    //vec3 newMat = matVal.rgb/255.0;
    
    
    
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

    //
    vec2 curTex = vec2(TEX_EARTH,0.5);
    float curMat = floor(curTex.x*256.0*255.0) + floor(curTex.y*255.0);

    gl_FragData[0] = vec4((worldPos.xyz),zbVal);//vec4(heightPacked.rg,matPacked.rg);//vec4(bhr,bhg,3.0/255.0,tex0.a);
    gl_FragData[1] = vec4(
        -curNormal, //todo fix this
        curMat//bodyUID//curMat
        //vec3(clamp(dot(curNormal,lightVec),0.0,1.0)) + matVal.rgb/255.0
        //,objectId
    );//vec4(resNorm.rgb, (TexCoord0.z+tex1.a)/2.0 ); //(TexCoord0.xyz+1.0)/2.0
    gl_FragData[2] = vec4(
        0.0,
        0.0,
        limbUID,
        bodyUID
    );
    
}
