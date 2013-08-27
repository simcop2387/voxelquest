
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