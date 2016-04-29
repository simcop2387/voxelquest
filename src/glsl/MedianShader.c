#version 120

uniform sampler2D Texture0;

// solidTargFBO
uniform sampler2D Texture1;
uniform sampler2D Texture2;

uniform vec3 cameraPos;
uniform vec2 bufferDim;

varying vec2 TexCoord0;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$

#define s2(a, b)                temp = a; a = min(a, b); b = max(temp, b);
#define mn3(a, b, c)            s2(a, b); s2(a, c);
#define mx3(a, b, c)            s2(b, c); s2(a, c);

#define mnmx3(a, b, c)          mx3(a, b, c); s2(a, b);
#define mnmx4(a, b, c, d)       s2(a, b); s2(c, d); s2(a, c); s2(b, d);
#define mnmx5(a, b, c, d, e)    s2(a, b); s2(c, d); mn3(a, c, e); mx3(b, d, e);
#define mnmx6(a, b, c, d, e, f) s2(a, d); s2(b, e); s2(c, f); mn3(a, b, c); mx3(d, e, f);

void main() {

    vec2 sampSize = 1.0/bufferDim;
    

    vec3 v[9];

    for(int dX = -1; dX <= 1; ++dX) {
        for(int dY = -1; dY <= 1; ++dY) {       
          vec2 offset = vec2(float(dX), float(dY));

          v[(dX + 1) * 3 + (dY + 1)] = texture2D(Texture0, TexCoord0 + offset * sampSize).rgb;
        }
    }

  vec3 temp;

  
  mnmx6(v[0], v[1], v[2], v[3], v[4], v[5]);
  mnmx5(v[1], v[2], v[3], v[4], v[6]);
  mnmx4(v[2], v[3], v[4], v[7]);
  mnmx3(v[3], v[4], v[8]);
  
  vec4 tex0 = texture2D(Texture0, TexCoord0);
  vec4 tex1 = texture2D(Texture1, TexCoord0);
  
  
  float dval = pow(tex1.w,5.0);
  
  // float mixVal = clamp(
  //   mix(1.25,0.2,dval),
  //   0.0,
  //   1.0
  // );
  
  float mixVal = clamp(distance(cameraPos,tex1.xyz)/100.0,0.0,1.0);
  
  //mixVal = mix(mixVal,0.25,0.75);
  
  //gl_FragColor = vec4(vec3(mixVal),1.0);
  
  vec3 finalRes;
  
  // if (distance(tex0.rgb,v[4].rgb) < 0.5) {
  //   finalRes = tex0.rgb;
  // }
  // else {
    finalRes = mix(
      v[4].rgb,
      tex0.rgb,
      1.0-float(distance(tex0.rgb,v[4].rgb) > 0.25)//mixVal
    );
  //}
  
  gl_FragColor = vec4(finalRes,1.0);
  
  //gl_FragColor = vec4(mixVal,mixVal,mixVal,1.0);
  
  //gl_FragColor = texture2D(Texture0, TexCoord0);

}
