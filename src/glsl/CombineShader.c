uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform sampler2D Texture2;
uniform sampler2D Texture3;

uniform sampler2D Texture4;
uniform sampler2D Texture5;

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

    // grass
    vec4 tex2 = texture2D(Texture2, TexCoord0 );
    vec4 tex3 = texture2D(Texture3, TexCoord0 );

    // geom
    vec4 tex4 = texture2D(Texture4, TexCoord0 );
    vec4 tex5 = texture2D(Texture5, TexCoord0 );


    float bh1 = tex0.r+tex0.g*255.0;
    float bh2 = tex2.r+tex2.g*255.0;
    float bh3 = tex4.r+tex4.g*255.0;

    float isGrass = float(bh2+1.0>=bh1);
    float isGeom = float(bh3>=bh1);

    gl_FragData[0] = mix(tex0,tex4,isGeom);
    gl_FragData[1] = mix(tex1,tex5,isGeom);

    //gl_FragData[0] = mix( mix(tex0,tex2,isGrass), tex4, isGeom );
    //gl_FragData[1] = mix( mix(tex1,tex3,isGrass), tex5, isGeom );


}

/*
//float numSteps = bufferWidth/cameraZoom;
//float offset = 1.0/numSteps;
//vec2 newCoords = floor(TexCoord0*numSteps)/numSteps;
newCoords.x += offset;
tex3 += texture2D(Texture3, newCoords );
newCoords.y += offset;
tex3 += texture2D(Texture3, newCoords );
newCoords.x -= offset;
tex3 += texture2D(Texture3, newCoords );
tex3 = tex3/4.0;
*/
