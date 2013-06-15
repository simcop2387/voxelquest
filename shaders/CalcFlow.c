
////////////////  CalcFlow  ////////////////



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

    vec2 baseCoord = TexCoord0;//(TexCoord0*(resolution.x-1.0) + 0.5)/resolution.x;
    vec3 off = offb/resolution.x;


    vec4 orig11 = texture2D(Texture0, baseCoord);
    vec4 orig01 = texture2D(Texture0, baseCoord+off.xy);
    vec4 orig21 = texture2D(Texture0, baseCoord+off.zy);
    vec4 orig10 = texture2D(Texture0, baseCoord+off.yx);
    vec4 orig12 = texture2D(Texture0, baseCoord+off.yz);

    float mult = 100.0;

    float s11 = (orig11.a)*mult;
    float s01 = (orig01.a)*mult;
    float s21 = (orig21.a)*mult;
    float s10 = (orig10.a)*mult;
    float s12 = (orig12.a)*mult;
    vec3 va = normalize(vec3(size.xy,s21-s11));
    vec3 vb = normalize(vec3(size.yx,s12-s10));

    vec3 bump = normalize(cross(va,vb));

    vec3 bumpRGB = (bump+1.0)/2.0;


    float theta = mouseCoords.x/100.0;
    float phi = mouseCoords.y/100.0;
    vec3 lightVec = vec3(sin(phi)*cos(theta), sin(phi)*sin(theta), cos(phi));
    lightVec = normalize(lightVec);
    float lightVal = dot(lightVec,bump);

    lightVal = pow(lightVal,3.0);

    lightVal *= orig11.a;

    vec2 mcUp = mouseCoordsUp/resolution.x;

    mcUp.y = 1.0- mcUp.y;

    mcUp = clamp(mcUp,0.0,1.0);




    vec4 res0 = orig11;

    //lightVal = res0.a;

    vec4 res1 = vec4(lightVal, lightVal, lightVal, 1.0);

    float rad = 64.0;
    float irad = 1.0/rad;

    float dis = clamp((irad-clamp(distance(mcUp,gl_FragCoord.xy/resolution.x),0.0,irad))*rad,0.0,1.0);
    res0.b += mDown/1000.0;//mDown*dis/255.0;//mDown*dis/2550.0; // *rand(TexCoord0+time)
    
    res0.b = clamp(res0.b,0.0,0.1);
    res0.a = clamp(res0.a,0.0,1.0);

    res0.g = res0.b + res0.a;

    float dval = clamp(res0.b*200.0,0.0,1.0);

    //vec4 grassVal = vec4(0.0,0.6*lightVal,0.0,1.0);
    //res1 = mix(res1, grassVal, clamp(1.0-res0.a,0.0,1.0) );//;
    //vec4 waterVal = vec4(0.0,0.5,1.0,1.0)*lightVal;
    //vec4 waterVal2 = vec4(0.0,0.0,0.5,1.0)*lightVal;
    //vec4 waterVal3 = mix(waterVal,waterVal2,dval);

    //res1 = mix(res1,waterVal3, float(dval>0.0));

    vec4 blueCol = vec4(0.0,0.0,1.0,1.0);

    res1 = mix(res1,blueCol,dval*0.75);

    float newres = orig11.a;//float(rand(TexCoord0) > 0.999);//*

    res1 = vec4(newres,newres,newres,1.0);

    //res1.r = res0.r;

    gl_FragData[0] = res0;
    gl_FragData[1] = res1;//res1;//texture2D(Texture1, TexCoord0);//;//vec4(dis);
}

$


////////////////  Erode  ////////////////



uniform sampler2D Texture0;
uniform sampler2D Texture1;
varying vec2 TexCoord0;
uniform vec2 resolution;
uniform vec2 mouseCoords;
uniform float mDown;
uniform float time;

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

    int i;
    float fi;

    vec2 baseCoord;
    vec3 off;

    vec4 tex1_11;

    vec4 tex0_00;
    vec4 tex0_01;
    vec4 tex0_02;
    vec4 tex0_10;
    vec4 tex0_11;
    vec4 tex0_12;
    vec4 tex0_20;
    vec4 tex0_21;
    vec4 tex0_22;

    float flowRate = 1.0/8.0;
    float flowIn = 0.0;
    float flowOut = 0.0;
    float compVal;

    

    float tota = 0.0;
    float totb = 0.0;

    float flowRate2;


    for (i = 2; i < 16; i*=2) {
        
        fi = float(i);//pow(2.0,float(i));

        baseCoord = TexCoord0;//(TexCoord0*(resolution.x-1.0) + 0.5)/resolution.x;
        off = offb*fi/resolution.x;// + sin(time/1000.0)/512.0;//sin(time/1000.0)/2000.0

        tex0_00 = texture2D(Texture0, baseCoord+off.xx);
        tex0_01 = texture2D(Texture0, baseCoord+off.xy);
        tex0_02 = texture2D(Texture0, baseCoord+off.xz);
        tex0_10 = texture2D(Texture0, baseCoord+off.yx);
        tex0_11 = texture2D(Texture0, baseCoord);
        tex0_12 = texture2D(Texture0, baseCoord+off.yz);
        tex0_20 = texture2D(Texture0, baseCoord+off.zx);
        tex0_21 = texture2D(Texture0, baseCoord+off.zy);
        tex0_22 = texture2D(Texture0, baseCoord+off.zz);
        
        flowIn = 0.0;
        flowOut = 0.0;
        compVal = tex0_11.g;// + rand(TexCoord0+time)/5.0;

        flowIn += clamp( (tex0_00.g-compVal), 0.0, 1.0)*tex0_00.b*flowRate;
        flowIn += clamp( (tex0_01.g-compVal), 0.0, 1.0)*tex0_01.b*flowRate;
        flowIn += clamp( (tex0_02.g-compVal), 0.0, 1.0)*tex0_02.b*flowRate;
        flowIn += clamp( (tex0_10.g-compVal), 0.0, 1.0)*tex0_10.b*flowRate;
        flowIn += clamp( (tex0_12.g-compVal), 0.0, 1.0)*tex0_12.b*flowRate;
        flowIn += clamp( (tex0_20.g-compVal), 0.0, 1.0)*tex0_20.b*flowRate;
        flowIn += clamp( (tex0_21.g-compVal), 0.0, 1.0)*tex0_21.b*flowRate;
        flowIn += clamp( (tex0_22.g-compVal), 0.0, 1.0)*tex0_22.b*flowRate;
        
        flowRate2 = (0.1-tex0_11.b)*float(tex0_11.b > 0.0);

        flowOut += clamp( (compVal-tex0_00.g), -1.0, 0.0)*flowRate2*flowRate;
        flowOut += clamp( (compVal-tex0_01.g), -1.0, 0.0)*flowRate2*flowRate;
        flowOut += clamp( (compVal-tex0_02.g), -1.0, 0.0)*flowRate2*flowRate;
        flowOut += clamp( (compVal-tex0_10.g), -1.0, 0.0)*flowRate2*flowRate;
        flowOut += clamp( (compVal-tex0_12.g), -1.0, 0.0)*flowRate2*flowRate;
        flowOut += clamp( (compVal-tex0_20.g), -1.0, 0.0)*flowRate2*flowRate;
        flowOut += clamp( (compVal-tex0_21.g), -1.0, 0.0)*flowRate2*flowRate;
        flowOut += clamp( (compVal-tex0_22.g), -1.0, 0.0)*flowRate2*flowRate;

        

        totb += (flowIn*2.0);  //-flowOut
        tota += (flowOut)/20.0;
        //tota += (flowIn)/10.0;
        //tota += sin(time/1000.0)*(flowIn-flowOut)*2.0; //
        //tota -= (flowIn-flowOut)/50.0;
    }

    

    vec4 res0 = texture2D(Texture0, TexCoord0);
    vec4 res1 = texture2D(Texture1, TexCoord0);

    res0.r = flowRate2*20.0;

    res0.a += tota;
    res0.b += totb-((1.0-mDown)*res0.b/4.0);

    gl_FragData[0] = res0;//clamp(res0,0.0,1.0);
    gl_FragData[1] = res1;//clamp(res1,0.0,1.0);



    /*
    res0.b += (flowIn)- (1.0/2048.0);//-flowOut);// - clamp(-1000.0*flowOut,0.0,1.0)/1000.0 - (1.0/2048.0);
    //res0.a -= clamp(-1000.0*flowOut,0.0,1.0)/10000.0;
    res0.r = clamp(-1000.0*flowOut,0.0,1.0);
*/


    //res0.a -= (flowOut);

    /*
    res0.b += (flowIn+flowOut)-(1.0/255.0);
    res0.a -= (flowOut);
    */

    //res1 = vec4(-50.0*flowOut);
    //res1.r = res0.a;

    /*
    tex1_00.rgb = (tex1_00.rgb*2.0)-1.0;
    tex1_01.rgb = (tex1_01.rgb*2.0)-1.0;
    tex1_02.rgb = (tex1_02.rgb*2.0)-1.0;
    tex1_10.rgb = (tex1_10.rgb*2.0)-1.0;
    tex1_11.rgb = (tex1_11.rgb*2.0)-1.0;
    tex1_12.rgb = (tex1_12.rgb*2.0)-1.0;
    tex1_20.rgb = (tex1_20.rgb*2.0)-1.0;
    tex1_21.rgb = (tex1_21.rgb*2.0)-1.0;
    tex1_22.rgb = (tex1_22.rgb*2.0)-1.0;
    */

    /*
    float mult = 50.0;

    float s11 = (orig11.a+orig11.b)*mult;
    float s01 = (orig01.a+orig01.b)*mult;
    float s21 = (orig21.a+orig21.b)*mult;
    float s10 = (orig10.a+orig10.b)*mult;
    float s12 = (orig12.a+orig12.b)*mult;
    vec3 va = normalize(vec3(size.xy,s21-s11));
    vec3 vb = normalize(vec3(size.yx,s12-s10));

    vec3 bump = normalize(cross(va,vb));

    vec3 bumpRGB = (bump+1.0)/2.0;


    float theta = mouseCoords.x/100.0;
    float phi = mouseCoords.y/100.0;
    vec3 lightVec = vec3(sin(phi)*cos(theta), sin(phi)*sin(theta), cos(phi));
    lightVec = normalize(lightVec);
    float lightVal = dot(lightVec,bump);

    lightVal = pow(lightVal,3.0);


    
    flowIn += tex1_01.x; //-1, 0
    flowIn -= tex1_21.x; // 1, 0
    flowIn += tex1_10.y; // 0, -1
    flowIn -= tex1_12.y; // 0, 1
    
    flowOut -= tex1_01.x; //-1, 0
    flowOut += tex1_21.x; // 1, 0
    flowOut -= tex1_10.y; // 0, -1
    flowOut += tex1_12.y; // 0, 1

    //res0.b += flowIn/100.0;
    //res0.a -= flowOut/1000.0;
    //res0.b -= 1.0/255.0;
    */

}