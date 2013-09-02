uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform sampler2D Texture2;
uniform sampler2D Texture3;

uniform float cameraZoom;

varying vec2 TexCoord0;

$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$

void main() {

    // pages
    vec4 tex0 = texture2D(Texture0, TexCoord0 );
    vec4 tex1 = texture2D(Texture1, TexCoord0 );


    float numSteps = 1024.0/cameraZoom;

    float offset = 1.0/numSteps;

    vec2 newCoords = floor(TexCoord0*numSteps)/numSteps;

    // grass
    vec4 tex2 = texture2D(Texture2, newCoords );
    vec4 tex3 = texture2D(Texture3, newCoords );


    /*
    newCoords.x += offset;
    tex3 += texture2D(Texture3, newCoords );
    newCoords.y += offset;
    tex3 += texture2D(Texture3, newCoords );
    newCoords.x -= offset;
    tex3 += texture2D(Texture3, newCoords );
    tex3 = tex3/4.0;
    */

    float bh1 = tex0.r+tex0.g*255.0;
    float bh2 = tex2.r+tex2.g*255.0;

    float isGrass = float(bh2+1.0>=bh1);

    gl_FragData[0] = mix(tex0,tex2,isGrass);
    gl_FragData[1] = mix(tex1,tex3,isGrass);
}
