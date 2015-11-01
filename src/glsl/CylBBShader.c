#version 120
#extension GL_EXT_frag_depth : enable

uniform sampler2D Texture0; // sprites
// uniform sampler2D Texture1; // geomBaseTargFBO
// uniform sampler2D Texture2; // geomBaseTargFBO


uniform float clipDist;
uniform vec2 bufferDim;
uniform vec4 rotZ;
uniform vec4 rotZ2;
uniform float curTime;

// varying vec4 multiTex0;
// varying vec4 multiTex1;
// varying vec4 multiTex2;
uniform vec3 cameraPos;
varying float camDis;


varying vec2 TexCoord0;
varying vec4 screenPos;


$

const float M_PI = 3.14159265359;

vec2 rotFunc(vec2 inp, float curRot) {
  mat2 m2x2;
  float rotInv = (M_PI*2.0-curRot)+M_PI/2.0;
  
  m2x2 = mat2(
      cos(rotInv),
      -sin(rotInv),
      sin(rotInv),
      cos(rotInv)
  );
  
  return m2x2*inp.xy;
  
}

void main() {

    
    
      
      //curTime;
      
    
    vec4 newPos = gl_Vertex;
    
    
    
    
    // newPos.xz = rotFunc(
    //   newPos.xz,
    //   // acos(
    //   //   1.0-abs(normalize(cameraPos-rotZ.xyz).z)
    //   // )
    //   curTime*4.0
    // );
    
    newPos.xyz -= rotZ2.xyz;
    newPos.xy = rotFunc(newPos.xy,M_PI*1.75);
    newPos.xyz += rotZ2.xyz;
    
    newPos.xyz -= rotZ.xyz;
    newPos.xy = rotFunc(newPos.xy,rotZ.w);
    newPos.xyz += rotZ.xyz;
    
    
    vec4 screenPos = gl_ModelViewProjectionMatrix * newPos;
    
    // // newPos.xyz = mix(
    // //   newPos.xyz,
    // //   rotZ.xyz,
    // //   (sin(curTime)+1.0)*0.5
    // // );

    // vec4 screenPos = gl_ModelViewProjectionMatrix * newPos; //
    
    
    camDis = distance(cameraPos.xyz,newPos.xyz);
    //float zbVal = 1.0-camDis/clipDist;
    //float zbVal = 1.0-screenPos.z/clipDist;
    
    
    
    //TexCoord0 = vec4(newPos.xyz,zbVal);
    // multiTex0 = gl_MultiTexCoord0; // object id
    // multiTex1 = gl_MultiTexCoord1; // x1,y1,x2,y2
    // multiTex2 = gl_MultiTexCoord2;
    
    //multiTex0.x += 0.01;
    
    
    
    
    // vec4 basePos = gl_Vertex;
    
    // basePos.xyz -= rotZ.xyz;
    
    
    // vec3 myAxis = normalize( vec3(cos(curRot),sin(curRot),0.0) );
    // mat4 transposeMat = transpose(gl_ModelViewProjectionMatrix);
    
    // vec3 pos = (
    //   basePos.y * transposeMat[0] +
    //   basePos.x * vec4(normalize(myAxis), 0.0)
    // ).xyz + rotZ.xyz;
    
    // vec4 newPos = vec4(pos, 1.0);


    // screenPos = gl_ModelViewProjectionMatrix *  newPos;
    
    
    
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = screenPos;
    
}

$



void main() {
    // vec4 geomSamp0 = texture2D(Texture1,gl_FragCoord.xy/bufferDim.xy);
    // vec4 geomSamp1 = texture2D(Texture2,gl_FragCoord.xy/bufferDim.xy);
    
    // vec4 final0 = geomSamp0;
    // vec4 final1 = geomSamp1;
    
    // vec2 newTC = multiTex2.xy;
    // newTC.x = mix(multiTex1.x,multiTex1.z,newTC.x);
    // newTC.y = mix(multiTex1.y,multiTex1.w,newTC.y);
  
  
    //vec3 finalCol = vec3(TexCoord0.xy*2.0 + 0.5,0.0);
    
    vec3 finalCol = texture2D(Texture0,TexCoord0).rgb;
    
    if ((finalCol.x + finalCol.y + finalCol.z) == 0.0) {
        discard;
    }
    
    // else {
    //     if (
    //         (int(geomSamp1.w) == int(multiTex0.x)) ||
    //         (multiTex0.y > 0.0)
    //     ) {
    //         final1.rgb = finalCol.rgb;
    //     }
    // }
    
    gl_FragDepthEXT = camDis/clipDist;
    
    gl_FragData[0] = vec4(screenPos);
    gl_FragData[1] = vec4(finalCol.rgb,0.0);
}
