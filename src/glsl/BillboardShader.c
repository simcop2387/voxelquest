#version 120
#extension GL_EXT_frag_depth : enable

uniform sampler2D Texture0; // sprites
uniform sampler2D Texture1; // geomBaseTargFBO
uniform sampler2D Texture2; // geomBaseTargFBO


uniform float cellsPerHolder;
uniform float heightOfNearPlane;
uniform float clipDist;
uniform vec2 bufferDim;


varying vec4 TexCoord0;
varying vec4 multiTex0;
varying vec4 multiTex1;
uniform vec3 cameraPos;
uniform vec3 offsetPos;
varying float camDis;



$


void main() {

    vec4 newVert = gl_Vertex;
    newVert.xyz += offsetPos;
    

    vec4 screenPos = gl_ModelViewProjectionMatrix * newVert;
    
    
    camDis = distance(cameraPos.xyz,newVert.xyz);
    float zbVal = 1.0-camDis/clipDist;
    //float zbVal = 1.0-screenPos.z/clipDist;
    
    TexCoord0 = vec4(newVert.xyz,zbVal);
    multiTex0 = gl_MultiTexCoord0; // object id
    multiTex1 = gl_MultiTexCoord1; // x1,y1,x2,y2
    
    multiTex0.x += 0.01;
    
    
    
    gl_PointSize = (heightOfNearPlane / screenPos.w)*0.5;
    
    
    
    gl_Position = screenPos;
    
}

$



void main() {
    vec4 geomSamp0 = texture2D(Texture1,gl_FragCoord.xy/bufferDim.xy);
    vec4 geomSamp1 = texture2D(Texture2,gl_FragCoord.xy/bufferDim.xy);
    
    vec4 final0 = geomSamp0;
    vec4 final1 = geomSamp1;
    
    //if (DO_POINTS) {
        // vec2 mv = 2.0*(gl_PointCoord.xy - 0.5);
        // mv = mv*mv;
        // float c = 1.0 - (mv.x + mv.y);
        
        // if (
        //     //(c < 0.0) ||
        //     (facingCam < -0.5)
        // ) {
        //     discard;
        // }
    //}
    
    
    
    float myDepth = camDis/clipDist;// - 0.01;
    
    gl_FragDepthEXT = myDepth;
    
    vec2 newTC = gl_PointCoord.xy;
    //newTC.y = 1.0 - newTC.y;
    
    newTC.x = mix(multiTex1.x,multiTex1.z,newTC.x);
    newTC.y = mix(multiTex1.y,multiTex1.w,newTC.y);
    
    vec3 finalCol = texture2D(Texture0,newTC).rgb;
    
    
    if ((finalCol.x + finalCol.y + finalCol.z) == 0.0) {
        discard;
    }
    else {
        if (
            (int(geomSamp1.w) == int(multiTex0.x)) ||
            (multiTex0.y > 0.0)
        ) {
            final1.rgb = finalCol.rgb;
        }
    }
    
    gl_FragData[0] = vec4(final0.xyz,max(final0.w,TexCoord0.w));//max(final0,myDepth);//TexCoord0;
    gl_FragData[1] = final1;//vec4(finalCol,multiTex0.x);
}
