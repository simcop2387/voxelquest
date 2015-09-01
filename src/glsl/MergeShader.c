#version 120

uniform sampler2D Texture0; // resultFBO
uniform sampler2D Texture1; // lastFBO

uniform vec2 bufferDim;

varying vec2 TexCoord0;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$

void main() {

  vec4 tex0 = texture2D(Texture0, TexCoord0);
  vec4 tex1 = texture2D(Texture1, TexCoord0);
  
  //if (distance(tex0.xyz,tex1.xyz) < 0.05) {
    gl_FragColor = mix(tex0,tex1,0.9);
  // }
  // else {
  //   gl_FragColor = mix(tex0,tex1,0.5);
  // }
  
  

}
