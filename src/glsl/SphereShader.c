#version 120
#extension GL_EXT_frag_depth : enable

uniform float cellsPerHolder;
uniform float heightOfNearPlane;
uniform float clipDist;
uniform vec2 bufferDim;
uniform vec3 lookAtVec;
uniform float curTime;

//uniform sampler2D Texture0;

varying vec4 TexCoord0;
varying vec4 TexCoord1;
uniform vec3 cameraPos;
varying float camDis;

const float blendDepth = 0.01;
const vec3 crand0 = vec3(12.989, 78.233, 98.422);

$


void main() {

    vec4 newVert = gl_Vertex;
    vec4 screenPos = gl_ModelViewProjectionMatrix * newVert;
    
    camDis = distance(cameraPos.xyz,newVert.xyz);
    float zbVal = 1.0-camDis/clipDist;
    //float zbVal = 1.0-screenPos.z/clipDist;
    
    TexCoord0 = vec4(newVert.xyz,zbVal);
    TexCoord1 = gl_MultiTexCoord0;
    
    
    gl_PointSize = (heightOfNearPlane / screenPos.w)*gl_MultiTexCoord0.x/16.0;
    
    //TexCoord1.w += 0.01;
    //TexCoord1.xyz = vec3(0.0,0.0,1.0);
    
    gl_Position = screenPos;
    
}

$

float randf(vec2 co) {
    return fract(sin(dot(co.xy, crand0.xy)) * 43758.5453);
}

void main() {
    
    //vec4 tex0 = texture2D(Texture0, gl_PointCoord.xy*0.125);
    
    
    
    vec2 mv = 2.0*(gl_PointCoord.xy - 0.5);
    //mv = floor(mv*24.0)/24.0;
    mv = mv*mv;
    
    
    
    float c = (1.0 - (mv.x + mv.y));
    
    float rv = randf(gl_PointCoord.xy);//floor(gl_PointCoord.xy*48.0)/48.0);
    
    if (c < 0.0) {
        discard;
    }

    
    
    
    vec3 upVec = vec3(0.0,0.0,1.0);
    vec3 newVec = normalize(mix(upVec,-lookAtVec,c));
    
    vec4 newTC0 = TexCoord0;
    newTC0.xyz += newVec*TexCoord1.x;
    
    vec4 newTC1 = vec4(
        newVec,
        //tex0.a + 25.0*256
        floor(((c+sin(c*2.0 + rv*1.0 + curTime/100.0))+1.0)*255.0/3.0*c) + 25.0*256.0
    ); 
    
    
    
    gl_FragDepthEXT = camDis/clipDist + length(mv)*0.002;
    
    
    gl_FragData[0] = newTC0;
    gl_FragData[1] = newTC1;
}

