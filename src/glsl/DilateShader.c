#version 120

uniform sampler2D Texture0;
varying vec2 TexCoord0;
uniform float mapStep;
uniform float texPitch;
uniform float doDilate;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {

    int i;

    vec4 tex1 = texture2D(Texture0, TexCoord0.xy);

    vec4 tex1u = vec4(0.0);
    vec4 tex1d = vec4(0.0);
    vec4 tex1l = vec4(0.0);
    vec4 tex1r = vec4(0.0);

    vec4 tex1ul = vec4(0.0);
    vec4 tex1ur = vec4(0.0);
    vec4 tex1dl = vec4(0.0);
    vec4 tex1dr = vec4(0.0);

    float offsetAmount = 1.0/texPitch;
    float testHeight = 0.0;
    float heightMod = 0.0;
    float tot = 0.0;
    float isAboveSea = 0.0;
    float rand1 = 0.0;
    float rand2 = 0.0;


    tex1u = texture2D(Texture0, vec2(TexCoord0.x, TexCoord0.y + offsetAmount) );
    tex1d = texture2D(Texture0, vec2(TexCoord0.x, TexCoord0.y - offsetAmount) );
    tex1l = texture2D(Texture0, vec2(TexCoord0.x - offsetAmount, TexCoord0.y) );
    tex1r = texture2D(Texture0, vec2(TexCoord0.x + offsetAmount, TexCoord0.y) );


    tex1ul = texture2D(Texture0, vec2(TexCoord0.x - offsetAmount, TexCoord0.y + offsetAmount) );
    tex1ur = texture2D(Texture0, vec2(TexCoord0.x + offsetAmount, TexCoord0.y + offsetAmount) );
    tex1dl = texture2D(Texture0, vec2(TexCoord0.x - offsetAmount, TexCoord0.y - offsetAmount) );
    tex1dr = texture2D(Texture0, vec2(TexCoord0.x + offsetAmount, TexCoord0.y - offsetAmount) );
    

    float v1 = 0.0;
    float v2 = 0.0;
    float v3 = 0.0;

    if (doDilate == 1.0) {
        v1 = max( max(tex1u.b,tex1d.b), max(tex1l.b,tex1r.b) );
        v2 = max( max(tex1ul.b,tex1dl.b), max(tex1ur.b,tex1dr.b) );
        tex1.b = max( max(v1,v2), tex1.b);
        //tex1.b = max( v1, tex1.b);
    }
    else {

        v3 = tex1u.b + tex1d.b + tex1l.b + tex1r.b + tex1ur.b + tex1ul.b + tex1dr.b + tex1dl.b;

        // if (v1 >= 3.0) {
        //     tex1.b = 0.0;
        // }
        // else {
        //     tex1.b = 1.0;
        // }

        if ( (tex1.b > 0.0) && (v3 <= 4.0) ) {

        }
        else {
            
        }

        v1 = min( min(tex1u.b,tex1d.b), min(tex1l.b,tex1r.b) );
        v2 = min( min(tex1ul.b,tex1dl.b), min(tex1ur.b,tex1dr.b) );
        tex1.b = min( min(v1,v2), tex1.b);
        //tex1.b = min( v1, tex1.b);

        
    }
    


    gl_FragData[0] = tex1;
}


