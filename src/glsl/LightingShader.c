uniform sampler2D Texture0;
uniform sampler2D Texture1;
varying vec2 TexCoord0;
uniform vec2 mouseCoords;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec3 lightPosWS;
uniform vec3 lightPosSS;
uniform float bufferWidth;
uniform float cameraZoom;
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

    
    

    float tot = tex0.r + tex0.g + tex0.b + tex0.a;
    //if (tot == 0.0) {
            //discard;
    //}
    
    float baseHeight = tex0.r*255.0 + tex0.g*255.0*256.0;

    //float lDis = clamp( (1.0-distance(lightPosSS.xy,TexCoord0.xy)*40.0),0.0,1.0)*float(lightPosSS.z > baseHeight);


    vec3 worldPosition = vec3(0.0,0.0,0.0);

    vec2 tcMod = TexCoord0.xy*bufferWidth/cameraZoom;
    tcMod.y += baseHeight;

    worldPosition.x = tcMod.x + tcMod.y;
    worldPosition.y = tcMod.x - tcMod.y;
    worldPosition.z = baseHeight;


    //map.x = screen.x / TILE_WIDTH + screen.y / TILE_HEIGHT;
    //map.y = screen.y / TILE_HEIGHT - screen.x / TILE_WIDTH;


    vec3 lightVec = normalize(worldPosition-lightPosWS);

    vec3 targ;
    vec3 orig = vec3(TexCoord0.x, TexCoord0.y, baseHeight);

    float resComp = 1.0;
    float aoval = tex1.a;





    if (tex1.a == 0.0) {
        resComp = 1.0;
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

    float hfog = clamp( (baseHeight-cameraPos.z)/1024.0,0.0,1.0);
    hfog = pow(hfog, 2.0);

    lightRes *= hfog;


    //float curHeight = tex0.g;
    //gl_FragData[0] = vec4(tex0.r,tex0.g,0.0,1.0);

    //lightRes = floor(lightRes*16.0)/16.0;


    //if (baseHeight == 0.0) {
        //discard;
    //}

    vec3 fogColor = vec3(0.6, 0.6, 0.7);


    vec3 resCol0 = vec3(0.0,0.0,0.0);
    vec3 resCol1 = vec3(1.0,1.0,1.0);


    //35:32:27
    //232:225:206


    //26:9:17
    //75:40:44

    if (tex0.b == 2.0/255.0) {
        resCol0 = vec3(26.0/255.0, 9.0/255.0, 17.0/255.0);
        resCol1 = vec3(75.0/255.0, 40.0/255.0, 44.0/255.0);
    }
    if (tex0.b == 1.0/255.0) {
        resCol0 = vec3(35.0/255.0, 32.0/255.0, 27.0/255.0);
        resCol1 = vec3(232.0/255.0, 225.0/255.0, 206.0/255.0);

    }
    if (tex0.b == 3.0/255.0) {

        lightRes = clamp(lightRes*aoval,0.0,1.0);

        resCol0 = vec3(10.0/255.0, 25.0/255.0, 0.0/255.0);
        resCol1 = vec3(145.0/255.0, 192.0/255.0, 62.0/255.0);
    }

    if (tex0.b == 4.0/255.0) {

        resCol0 = vec3(255.0/255.0, 255.0/255.0, 255.0/255.0);
        resCol1 = vec3(255.0/255.0, 255.0/255.0, 255.0/255.0);

    }


    //vec4(lightRes*0.8,lightRes*0.7,lightRes*0.6, lightRes)
    gl_FragData[0] = vec4(mix( fogColor, mix(resCol0,resCol1,lightRes), hfog ),1.0); //vec4(lightRes,lightRes,lightRes,1.0);//
    //gl_FragData[0] = vec4(aoval,aoval,aoval,1.0);

    //gl_FragData[0] = vec4(lightRes,lightRes,lightRes,1.0);
    //gl_FragData[1] = gl_FragData[0];


}






// void main() {
//     vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
//     vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

//     gl_FragData[0] = tex0;
// }










/*
    targ.x = (lightVec.x-lightVec.y);
    targ.y = -(lightVec.x/2.0) + -(lightVec.y/2.0) + lightVec.z;
    targ.z = baseHeight + lightVec.z*rayDis/255.0;

    targ.x = targ.x*rayDis/resolution.x + TexCoord0.x;
    targ.y = targ.y*rayDis/resolution.y + TexCoord0.y;

    

    int i;
    float fi;
    vec3 curCoord;
    vec4 curSamp;



    float testHeight;
    

    float tval;
    


    for (i = 0; i < iNumRaySteps; i++) {
            fi = float(i)/fNumRaySteps;
            curCoord = mix(orig, targ.xyz, fi);

            //curCoord = floor(curCoord*resolution.x)/resolution.x;

            curSamp = texture2D(Texture0, curCoord.xy);

            testHeight = curSamp.r + curSamp.g*255.0;
            tval = curCoord.z - testHeight;
            resComp += float(tval>0.22)/fNumRaySteps;
            //resComp += float(curCoord.z > (testHeight+0.01) )/fNumRaySteps;
            

    }
    */




