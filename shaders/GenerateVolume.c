
////////////////  GenerateVolume  ////////////////

uniform sampler3D Texture0; //volume texture
uniform sampler2D Texture1; //2d to 3d coords
varying vec2 TexCoord0;
uniform vec2 resolution;

$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$


vec4 fetchTex(vec2 pos) {
    vec4 coords = texture2D(Texture1,pos);
    return texture3D(Texture0, ((coords.xyz)*255.0+0.5)/256.0);
}

void main() {
    vec4 tex0 = fetchTex(TexCoord0.xy);
    gl_FragData[0] = tex0;
}

