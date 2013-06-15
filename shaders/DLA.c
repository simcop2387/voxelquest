
////////////////  DLA  ////////////////



uniform sampler2D Texture0;
uniform sampler2D Texture1;
varying vec2 TexCoord0;
uniform vec2 resolution;
uniform vec2 mouseCoords;
uniform vec2 mouseCoordsUp;
uniform float time;
uniform float mDown;

const vec2 size = vec2(2.0,0.0);
const vec3 offb = vec3(-1.0,0.0,1.0);

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
    vec3 off = offb/resolution.x;

    int i;
    int j;

    float fi;
    float fj;

    

    



    vec4 tex0_00 = texture2D(Texture0, TexCoord0+off.xx);
    vec4 tex0_01 = texture2D(Texture0, TexCoord0+off.xy);
    vec4 tex0_02 = texture2D(Texture0, TexCoord0+off.xz);
    vec4 tex0_10 = texture2D(Texture0, TexCoord0+off.yx);
    vec4 tex0_11 = texture2D(Texture0, TexCoord0);
    vec4 tex0_12 = texture2D(Texture0, TexCoord0+off.yz);
    vec4 tex0_20 = texture2D(Texture0, TexCoord0+off.zx);
    vec4 tex0_21 = texture2D(Texture0, TexCoord0+off.zy);
    vec4 tex0_22 = texture2D(Texture0, TexCoord0+off.zz);

    

    

    float sum = tex0_00.a + tex0_01.a + tex0_02.a + tex0_10.a + tex0_12.a + tex0_20.a + tex0_21.a + tex0_22.a;
    float test = float(sum > 0.001)*float(sum <= 1.0);
    //float proc = test - tex0_11.a;

    float colVal = clamp(1.0 - (time/800.0), 0.0, 1.0);
    float newtval = abs(sin(time/100.0));

    int rad = 1 + int(newtval)*8;
    float frad = float(rad);
    float multi = 5.0 + newtval*8.0;

    float tot = 0.0;
    float tot2 = 0.0;
    float weight;
    //discard;

    float maxDis = sqrt(frad*frad*2.0);

    vec4 texVal;

    float totWeight = 0.0;
    float totBlur = 0.0;

    vec2 offset;


    //if (proc > 0.5) {
        for (i = -rad; i <= rad; i++) {
            for (j = -rad; j <= rad; j++) {
                fi = float(i);
                fj = float(j);

                weight = (maxDis-sqrt(fi*fi+fj*fj))/maxDis;

                offset = vec2(fi*fi*fi*multi/resolution.x,fj*fj*fj*multi/resolution.x);

                texVal = texture2D(Texture0, TexCoord0+offset);



                totBlur += (texVal.r+texVal.a)*weight;
                totWeight += weight;

                tot += texVal.a;// *float(tex0_11.r-texVal.r > 0.0);
                tot2 += 1.0;
            }
        }
    //}

    float bval = totBlur/totWeight;

    float totFill = ((tot2-tot)/tot2);

    float rval = abs(rand(TexCoord0+sin(time/200.0)));
    float res =  (float ( totFill*totFill*test*rval > 0.8 ) + tex0_11.a);//*float(tot < 8.0); //+ tex0_11.a

    float v1;
    float v2;

    if (tex0_11.b != 0.0) {
        v1 = tex0_11.b;//min(v1,tex0_11.b);
    }
    else {
        if (tex0_11.b == 1.0) {
            v1 = 1.0;
        }
        else {
            v1 = res*colVal;
        }
    }

    float maxWeight = 0.0;


    rad = 8;
    frad = float(rad);
    
    for (i = -rad; i <= rad; i++) {
        for (j = -rad; j <= rad; j++) {
            fi = float(i);
            fj = float(j);



            offset = vec2(fi/resolution.x,fj/resolution.x);

            texVal = texture2D(Texture0, TexCoord0+offset);

            weight = min((1.0-sqrt(fi*fi + fj*fj)/frad)*2.0,1.0)*texVal.b;
            maxWeight = max(maxWeight,weight);
        }
    }


    vec4 res0 = vec4(maxWeight, maxWeight, v1, res);//tex0_11.r,res,res,res);

    gl_FragData[0] = res0;//tex0_11.aaaa;
    gl_FragData[1] = res0;//tex0_11.aaaa;
}



/*
    if ((res != 0.0) && (tex0_11.a == 0.0)) {
        res0 = vec4(tex0_11.r,res*timeval,res*timeval,res);
    }
    else {
        res0 = tex0_11;
    }
    */
    /*
    float subval = bval/100.0;

    if (res-subval > 0.02) {
        res -= subval;
    }
    else {

    }
    */

    //float br = (bval-0.2)*timeval + tex0_11.a;



