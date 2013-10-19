#version 120

uniform sampler2D Texture0;
uniform sampler2D Texture1;
varying vec2 TexCoord0;

uniform float curTime;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$



vec3 COLOR_MASKS[16] = vec3[](
    vec3(  1.0,  0.0,  0.0  ),
    vec3(  1.0,  1.0,  0.0  ),
    vec3(  0.0,  1.0,  0.0  ),
    vec3(  0.0,  1.0,  1.0  ),
    vec3(  0.0,  0.0,  1.0  ),
    vec3(  1.0,  0.0,  1.0  ),

    vec3(  0.7,  0.0,  0.0  ),
    vec3(  0.7,  0.7,  0.0  ),
    vec3(  0.0,  0.7,  0.0  ),
    vec3(  0.0,  0.7,  0.7  ),
    vec3(  0.0,  0.0,  0.7  ),
    vec3(  0.7,  0.0,  0.7  ),
   
    vec3(  0.3,  0.0,  0.0  ),
    vec3(  0.3,  0.3,  0.0  ),
    vec3(  0.0,  0.3,  0.0  ),
    vec3(  0.0,  0.3,  0.3  )

);

int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}

void main() {

    float newTime = curTime/20000.0;

    vec4 tex1 = texture2D( Texture1, TexCoord0.xy);//
    vec4 tex0 = texture2D( Texture0, vec2(tex1.r, (5.0 + 0.5)/255.0 ) );

    



    float offsetAmount = 2.0/1024.0;

    vec4 tex1u = texture2D(Texture1, vec2(TexCoord0.x, TexCoord0.y + offsetAmount) );
    vec4 tex1d = texture2D(Texture1, vec2(TexCoord0.x, TexCoord0.y - offsetAmount) );
    vec4 tex1l = texture2D(Texture1, vec2(TexCoord0.x - offsetAmount, TexCoord0.y) );
    vec4 tex1r = texture2D(Texture1, vec2(TexCoord0.x + offsetAmount, TexCoord0.y) );

    /*
    vec4 tex1ul = texture2D(Texture1, vec2(TexCoord0.x - offsetAmount, TexCoord0.y + offsetAmount) );
    vec4 tex1ur = texture2D(Texture1, vec2(TexCoord0.x + offsetAmount, TexCoord0.y + offsetAmount) );
    vec4 tex1dl = texture2D(Texture1, vec2(TexCoord0.x - offsetAmount, TexCoord0.y - offsetAmount) );
    vec4 tex1dr = texture2D(Texture1, vec2(TexCoord0.x + offsetAmount, TexCoord0.y - offsetAmount) );
    */

    float mod = 1.0;
    if (
        tex1u.g != tex1.g ||
        tex1d.g != tex1.g ||
        tex1l.g != tex1.g ||
        tex1r.g != tex1.g /*||

        tex1ul.g < tex1.g ||
        tex1ur.g < tex1.g ||
        tex1dl.g < tex1.g ||
        tex1dr.g < tex1.g*/

    ) {
        mod = 0.2;
    }



    int colInd = intMod(int(tex1.g*255.0),16);
    vec4 colMod = vec4(0.2,0.2,0.2,0.2);//vec4(COLOR_MASKS[colInd],1.0);
    if (tex1.g == 0) {
        colMod = vec4(0.0,0.0,0.0,0.0);
    }


    gl_FragData[0] = tex0*mod;//1.0-tex1.bbbb;//tex0*mod;//tex0;//1.0-tex1.bbbb;////1.0-tex1.bbbb;//(tex0)*mod; // + colMod

}


