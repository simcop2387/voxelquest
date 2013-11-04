#version 120

uniform sampler2D Texture0; // simplexFBO
uniform sampler2D Texture1; // imageHM0
uniform sampler2D Texture2; // imageHM1

uniform float mapSampScale;
uniform vec3 paramArrMap[16];

varying vec2 TexCoord0;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$

void main() {

    

    vec4 tex0 = texture2D( Texture0, (TexCoord0.xy + paramArrMap[2].xy) );
    vec4 tex1 = texture2D( Texture1, (TexCoord0.xy + paramArrMap[0].xy)*mapSampScale );
    vec4 tex2 = texture2D( Texture2, (TexCoord0.xy + paramArrMap[1].xy)*mapSampScale );


    float[3] sv;
    sv[0] = 0.0;
    sv[1] = 0.0;
    sv[2] = 0.0;

    float[6] vals;

    vals[0] = tex1.r;
    vals[1] = tex1.g;
    vals[2] = tex1.b;
    vals[3] = tex2.r;
    vals[4] = tex2.g;
    vals[5] = tex2.b;

    int i;
    int j;
    int curInd = -1;

    for (i = 0; i < 3; i++) {
        if (paramArrMap[2+i].x < 0.5) {
            curInd += 1;
        }
        else {
            curInd += 2;
        }

        sv[i] = vals[curInd];

    }


    float v0 = sv[0]*tex0.r*tex0.r;
    float v1 = sv[1]*tex0.g*tex0.g;
    float v2 = sv[2]*tex0.b*tex0.b;

    //float h = sqrt(v0 + v1 + v2);

    float h = pow(max(max(v0,v1),v2),0.4);

    /*
    if (h > 1.0) {
        h = 2.0-h;
    }
    */

    h = clamp(h, 0.0, 1.0);
    h = pow(h,1.5);

    gl_FragData[0] = vec4(h,0.0,0.0,0.0);

}


