// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// geom fbo
uniform sampler2D Texture2;
uniform sampler2D Texture3;

varying vec2 TexCoord0;
uniform vec2 mouseCoords;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec3 lightPosWS;
uniform vec2 lightPosSS;
uniform vec4 activeObjectPos;
uniform vec4 lastUnitPos;
uniform vec4 lastPagePos;
uniform vec2 bufferDim;
uniform float cameraZoom;
uniform float diskOn;
uniform float curTime;

uniform vec4 fogPos;

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




int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}

vec2 pack16(float num) {

    int iz = int(num);
    int ir = intMod(iz,256);
    int ig = (iz)/256;

    vec2 res;

    res.r = float(ir)/255.0;
    res.g = float(ig)/255.0;

    return res;

}

float unpack16(vec2 num) {
    return num.r*255.0 + num.g*65280.0;
}

float distanceAspect(vec2 p1, vec2 p2, float aspect) {
    p1.x = p1.x * aspect;
    p2.x = p2.x * aspect;

    return distance(p1,p2);
}


void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex3 = texture2D(Texture3, TexCoord0.xy);

    

    float newZoom = min(cameraZoom,1.0);
    float newZoom2 = max(cameraZoom,1.0);

    float newZoom3 = mix(newZoom,cameraZoom, float(cameraZoom <= 1.0) );
    

    float tot = tex0.r + tex0.g + tex0.b + tex0.a;
    //if (tot == 0.0) {
            //discard;
    //}
    
    float baseHeight = unpack16(tex0.rg);//tex0.r*255.0 + tex0.g*255.0*256.0;
    

    float baseHeight2 = unpack16(tex2.rg);
    float geomMod = float(tex2.b > 0.0)*mix(0.0,1.0,float( baseHeight2 > baseHeight ));



    vec3 worldPosition = vec3(0.0,0.0,0.0);
    vec2 tcMod = (vec2(TexCoord0.x,1.0-TexCoord0.y)*2.0-1.0 );
    tcMod.x *= bufferDim.x/(newZoom);
    tcMod.y *= bufferDim.y/(newZoom);


    tcMod.y -= cameraPos.z;
    worldPosition.x = tcMod.y + tcMod.x/2.0 + (baseHeight);
    worldPosition.y = tcMod.y - tcMod.x/2.0 + (baseHeight);
    worldPosition.z = baseHeight;
    worldPosition.x += cameraPos.x;
    worldPosition.y += cameraPos.y;


    //worldPosition.x = 0.0;
    //worldPosition.y = 0.0;
    //worldPosition.z = 0.0;


    
    // float isLastUnit =

    // float(worldPosition.x >= lastUnitPos.x-4.0) *
    // float(worldPosition.y >= lastUnitPos.y-4.0) *
    // float(worldPosition.z >= lastUnitPos.z-4.0) *
    // float(worldPosition.x <= lastUnitPos.x+lastUnitPos.w+4.0) *
    // float(worldPosition.y <= lastUnitPos.y+lastUnitPos.w+4.0) *
    // float(worldPosition.z <= lastUnitPos.z+lastUnitPos.w+4.0) * 0.2;
    

    // float isLastPage =

    // float(worldPosition.x >= lastPagePos.x-4.0) *
    // float(worldPosition.y >= lastPagePos.y-4.0) *
    // float(worldPosition.z >= lastPagePos.z-4.0) *
    // float(worldPosition.x <= lastPagePos.x+lastPagePos.w+4.0) *
    // float(worldPosition.y <= lastPagePos.y+lastPagePos.w+4.0) *
    // float(worldPosition.z <= lastPagePos.z+lastPagePos.w+4.0) * 0.1;


    


    vec3 lightVec = normalize(lightPosWS-worldPosition);

    vec3 targ;
    vec3 orig = vec3(TexCoord0.x, TexCoord0.y, baseHeight);

    float resComp = 1.0;
    float aoval = tex1.a;



    int i;
    float fi;

    const int iNumSteps = 64;
    const float fNumSteps = float(iNumSteps);
    float flerp = 0.0;

    vec3 wStartPos = worldPosition;
    vec3 wEndPos = lightPosWS;

    vec2 sStartPos = TexCoord0.xy;
    vec2 sEndPos = lightPosSS;

    vec3 wCurPos;
    vec2 sCurPos;
    vec4 samp;

    float curHeight;

    float aspectRatio = bufferDim.x/bufferDim.y;



    //float mval = float( distanceAspect(sStartPos,sEndPos,aspectRatio)/(newZoom3) < 0.1)*0.2;

    float wpDis = distance(activeObjectPos.xy,worldPosition.xy);

    vec2 wpNorm = normalize(activeObjectPos.xy-worldPosition.xy);

    float theta = atan(wpNorm.y,wpNorm.x);

    float pval = float(wpDis < 100.0 && wpDis > 75.0) *
        float(abs(sin(theta*8.0 + curTime/100.0)) > 0.75) * 
        (1.0-clamp(abs(activeObjectPos.z-worldPosition.z)/1024.0,0.1,1.0)) * diskOn * 0.5; //

    float totHits = 0.0;
    float wasHit = 0.0;
    float lastHit = 0.0;
    
    for (i = 0; i < iNumSteps; i++) {
        fi = float(i);
        flerp = fi/fNumSteps;

        wCurPos = mix(wStartPos,wEndPos,flerp);
        sCurPos = mix(sStartPos,sEndPos,flerp);

        samp = texture2D(Texture0, sCurPos);

        curHeight = unpack16(samp.rg);

        wasHit = float (curHeight > wCurPos.z );
        totHits += wasHit;
        lastHit = mix(lastHit, flerp, wasHit);
    }

    resComp = mix(1.0,0.0, totHits*2.0/fNumSteps );
    resComp = clamp(resComp,0.0,1.0);
    



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
    float lightRes = mix(aoval*0.1,lightval,lightval);
    lightRes = pow(lightRes,0.8);
    lightRes = clamp(lightRes,0.0,1.0);



    vec2 newCam = cameraPos.xy - worldPosition.xy;
    
    
    newCam.x = min(newCam.x,0.0);
    newCam.y = min(newCam.y,0.0);
    
    

    vec3 newFog = (fogPos.xyz-worldPosition.xyz)/256.0;
    newFog.xy /= 4.0;


    vec3 fogXYZ = 1.0-clamp( newFog, 0.0, 1.0);

    float fogLen = 1.0-clamp(1.0-(fogXYZ.x*fogXYZ.y),0.0,1.0);

    float hfog = min(clamp(sqrt(fogLen),0.0,1.0),fogXYZ.z);//(clamp( distance(newFP.xyz,worldPosition)*float(baseHeight > 0.0)/6000.0, 0.0, 1.0) * fogV);

    //hfog: 0.0 is foggy, 1.0 is clear

    hfog *= float(baseHeight > 0.0);

    hfog = pow(hfog, 2.0);
    lightRes *= hfog;

    vec3 fogColor = vec3(0.6, 0.6, 0.7);
    vec3 resCol0 = vec3(0.0,0.0,0.0);
    vec3 resCol1 = vec3(1.0,1.0,1.0);


    // stone
    if (tex0.b == 1.0/255.0) {
        resCol0 = vec3(35.0/255.0, 32.0/255.0, 27.0/255.0)*0.3;
        resCol1 = vec3(232.0/255.0, 225.0/255.0, 206.0/255.0);

    }

    // dirt
    if (tex0.b == 2.0/255.0) {
        resCol0 = vec3(22.0/255.0, 9.0/255.0, 17.0/255.0);
        resCol1 = vec3(110.0/255.0, 80.0/255.0, 70.0/255.0);
    }

    // grass
    if (tex0.b == 3.0/255.0) {

        lightRes = clamp( pow((lightRes+aoval)/2.0,2.0),0.0,1.0);

        resCol0 = vec3(10.0/255.0, 25.0/255.0, 0.0/255.0)*0.3;
        resCol1 = vec3(145.0/255.0, 192.0/255.0, 62.0/255.0);
    }

    /*
    // light
    if (tex0.b == 4.0/255.0 || tex0.b == 5.0/255.0) {

        resCol0 = vec3(255.0/255.0, 255.0/255.0, 255.0/255.0);
        resCol1 = vec3(255.0/255.0, 255.0/255.0, 255.0/255.0);

    }
    */


    if (aoval == 0.0) {
        //resCol0 = vec3(0.0/255.0, 0.0/255.0, 0.0/255.0);
        //resCol1 = vec3(0.0/255.0, 0.0/255.0, 0.0/255.0);
        //resCol0 = mod(worldPosition,256.0)/255.0;
        //resCol1 = resCol0;

        
        
        

        resCol0 = vec3 (0.0,0.0,0.0);
        resCol1 = vec3 (0.0,0.0,0.0);

    }


    vec3 resColor = mix(resCol0,resCol1,lightRes);
    
    //vec3 resColor = mod(worldPosition,256.0)/255.0;
    //vec3 resColor = vec3(lightRes);//mix(resCol0,resCol1,lightRes);//mod(worldPosition,256.0)/255.0;//vec3(lightRes);//mix(resCol0,resCol1,lightRes);
    //vec3 resColor = tex1.rgb;


    //vec4(lightRes*0.8,lightRes*0.7,lightRes*0.6, lightRes)
    gl_FragData[0] = vec4(mix( fogColor, resColor, hfog ),1.0)+geomMod*0.5+pval;//+geomMod;//+isLastUnit+isLastPage;//+isLastUnit+isLastPage; //vec4(lightRes,lightRes,lightRes,1.0);//
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




