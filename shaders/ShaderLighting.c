
  ////////////////  ShaderLighting  ////////////////  

uniform sampler2D Texture0;
uniform sampler2D Texture1;
varying vec2 TexCoord0;
uniform vec2 mouseCoords;
uniform vec2 resolution;
//uniform vec4 bgColor;

const int iNumRaySteps = 64;
const float fNumRaySteps = 64.0;
const float rayDis = 128.0;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;// = ;//TexCoord0 = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
}

$

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    ////////////////////////
    //gl_FragData[0] = tex0;
    ////////////////////////

    
    float theta = mouseCoords.x/100.0;
    float phi = mouseCoords.y/100.0;

    float tot = tex0.r + tex0.g + tex0.b + tex0.a;

    if (tot == 0.0) {
            discard;
    }

    vec3 lightVec = vec3(sin(phi)*cos(theta), sin(phi)*sin(theta), cos(phi));
    lightVec = normalize(lightVec);

    vec3 targ;

    float baseHeight = tex0.r + tex0.g*255.0;

    targ.x = (lightVec.x-lightVec.y);
    targ.y = -(lightVec.x/2.0) + -(lightVec.y/2.0) + lightVec.z;
    targ.z = baseHeight + lightVec.z*rayDis/255.0;

    targ.x = targ.x*rayDis/resolution.x + TexCoord0.x;
    targ.y = targ.y*rayDis/resolution.y + TexCoord0.y;


    vec3 orig = vec3(TexCoord0.x, TexCoord0.y, baseHeight);

    int i;
    float fi;
    vec3 curCoord;
    vec4 curSamp;



    float testHeight;
    float resComp = 0.0;

    float curShadow;
    float tval;
    float aoval = tex1.a;



    //+rand(targ.xy*100.0)/200.0

    for (i = 0; i < iNumRaySteps; i++) {
            fi = float(i)/fNumRaySteps;
            curCoord = mix(orig, targ.xyz, fi);

            //curCoord = floor(curCoord*resolution.x)/resolution.x;

            curSamp = texture2D(Texture0, curCoord.xy);

            testHeight = curSamp.r + curSamp.g*255.0;
            tval = curCoord.z - testHeight;
            resComp += float(tval>0.11)*2.0/fNumRaySteps;
            //resComp += float(curCoord.z > (testHeight+0.01) )/fNumRaySteps;
            

    }
    resComp = clamp(pow(resComp,0.5),0.0,1.0);

    resComp = resComp*float(resComp > 0.01); //resComp*

    //resComp = clamp(resComp,0.0,1.0);

    //resComp = float(resComp > 0.9);

    if (tex1.a == 1.0) {
        //resComp = 1.0;
    }

    vec3 myVec;
    myVec.x = (tex1.r-0.5)*2.0;
    myVec.y = (tex1.g-0.5)*2.0;
    myVec.z = (tex1.b-0.5)*2.0;

    float lightval = dot(myVec,lightVec);
    lightval = clamp(lightval, 0.0, 1.0);
    lightval = lightval*resComp;
    float lightRes = mix(aoval*0.3,lightval,lightval);
    lightRes = pow(lightRes,0.8);
    lightRes = clamp(lightRes,0.0,1.0);

    float hfog = clamp(baseHeight*0.3,0.0,1.0);
    hfog = pow(hfog, 2.0);

    lightRes *= hfog;


    //float curHeight = tex0.g;
    //gl_FragData[0] = vec4(tex0.r,tex0.g,0.0,1.0);

    //lightRes = floor(lightRes*16.0)/16.0;

    gl_FragData[0] = vec4(lightRes*0.8,lightRes*0.7,lightRes*0.6,hfog);
    

    //gl_FragData[0] = vec4(lightRes,lightRes,lightRes,1.0);
    //gl_FragData[1] = gl_FragData[0];


}