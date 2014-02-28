#version 120

// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// pal fbo
uniform sampler2D Texture2;

uniform float cameraZoom;
uniform vec2 bufferDim;
uniform vec2 mouseCoords;
uniform vec2 resolution;

uniform vec3 cameraPos;

uniform float numLights;
uniform vec4 lightArr[4*6];

const float TEX_GRASS = 12.0/255.0;
const float TEX_GRASS2 = 13.0/255.0;
const int VECS_PER_LIGHT = 4;

varying vec2 TexCoord0;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;// = ;//TexCoord0 = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
}

$



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



bool isGeom(vec2 num) {
    int test = int(unpack16(num));
    return (test >= 32768);
}


vec3 unpackColorGeom(vec2 num) {
    int test = int(unpack16(num));
    ivec3 intRGB;
    vec3 res = vec3(0.0);

    test -= 32768;

    intRGB.b = test/1024;
    test -= intRGB.b*1024;

    intRGB.g = test/32;
    test -= intRGB.g*32;

    intRGB.r = test;

    res = vec3(intRGB)/31.0;

    return res;
}

vec3 unpackColor(vec2 num, float lightVal) {
    return texture2D( Texture2, vec2(lightVal, (num.g*255.0+num.r*2.0 + 0.5)/255.0 ) ).rgb; //vec3(lightVal);//
}


vec3 getFogColor(float lv) {
    float newlv = clamp( 1.0-pow( (lv-0.5)*2.0, 2.0 ), 0.0, 1.0); 
    vec3 fogColor1 = vec3(0.05, 0.1, 0.3);
    vec3 fogColor2 = vec3(0.7, 0.8, 1.0);

    float newlv2 = clamp( 1.0-pow( (lv-0.5)*4.0 - 1.0, 2.0 ), 0.0, 1.0); 

    return mix(fogColor1,fogColor2, newlv ) + pow(newlv2,4.0)/4.0;
}


void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    const int iNumSteps = 64;
    const float fNumSteps = float(iNumSteps);

    float newZoom = min(cameraZoom,1.0);    
    float tot = float(tex0.r + tex0.g + tex0.b + tex0.a > 0.0);    
    float baseHeight = unpack16(tex0.rg);

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

    
    vec4 lightPosWS = vec4(0.0);
    vec2 lightPosSS = vec2(0.0);
    vec3 lightVec = vec3(0.0);
    vec3 resColor = vec3(0.0);
    vec3 resColorTemp = vec3(0.0);


    float resComp = 1.0;
    float aoval = tex1.a;
    float notBlank = 1.0;//float(aoval != 0.0);


    int i;
    int j;
    int k;
    int baseInd;
    float fj;
    float fi;
    float flerp = 0.0;
    float curHeight = 0.0;
    float totHits = 0.0;
    float wasHit = 0.0;
    float lastHit = 0.0;

    vec4 oneVec = vec4(1.0);

    

    vec3 wStartPos = worldPosition;
    vec2 sStartPos = TexCoord0.xy;
    vec3 wEndPos = vec3(0.0);
    vec2 sEndPos = vec2(0.0);

    //vec3 globLightCol = vec3(0.0);

    vec3 wCurPos;
    vec2 sCurPos;
    vec4 samp;
    vec3 resColGS = vec3(0.0);
    
    float ssao = 1.0;
    float totRays;
    vec2 offsetCoord = vec2(0.0);
    
    vec4 testTex = vec4(0.0);
    vec3 bucket = vec3(0.0);//tex1.xyz;
    vec3 myVec = vec3(0.0);

    for (i = -1; i <= 1; i += 1) {
        fi = float(i);
        for (j = -1; j <= 1; j+= 1) {
            fj = float(j);

            testTex = texture2D(Texture1, TexCoord0.xy + vec2(fi,fj)/(bufferDim/newZoom) );

            if (testTex.x == 1.0) {
                bucket.x += 1.0;
            }
            if (testTex.y == 1.0) {
                bucket.y += 1.0;
            }
            if (testTex.z == 1.0) {
                bucket.z += 1.0;
            }

        }
    }

  
    if (aoval == 0.0) {

        if (bucket.x > bucket.y) {
            if (bucket.x > bucket.z) {
                myVec.x = 1.0;
            }
            else {
                myVec.z = 1.0;
            }
        }
        else {
            if (bucket.y > bucket.z) {
                myVec.y = 1.0;
            }
            else {
                myVec.z = 1.0;
            }
        }

    }
    else {
        myVec = normalize( (tex1.rgb-0.5)*2.0 );
    }

    // outlines

    vec2 dirModXY[4];
    dirModXY[0] = vec2(1.0,0.0);
    dirModXY[1] = vec2(-1.0,1.0);
    dirModXY[2] = vec2(0.0,1.0);
    dirModXY[3] = vec2(0.0,-1.0);
    float testHeight = 0.0;
    float bestHeight = 0.0;
    float maxDif = 512.0;

    float resCompTot = 0.0;
    float frontLightTot = 0.0;

    for (i = 0; i < 4; i++) {

        testTex = texture2D(Texture0, TexCoord0.xy + dirModXY[i].xy/bufferDim );
        
        testHeight = abs( baseHeight-unpack16(testTex.rg) );
        if (testHeight > bestHeight) {
            bestHeight = testHeight;
        }
        

    }
    float outDif = clamp(bestHeight/maxDif,0.0,1.0)*0.05;


    const int aoRad = 64;
    const float faoRad = float(aoRad);
    const int aoRadStep = 1;
    float pi = 3.14159;
    
    float minRad = 2.0;
    float maxRad[2];
    maxRad[0] = 64.0;
    maxRad[1] = 512.0;
    float curRad = 0.0;
    
    //float hitCount;

    float minRotInc = pi;
    float maxRotInc = 0.01;
    float curRotInc = 0.0;
    
    float curRot = 0.0;
    vec2 newTC = vec2(2.0);

    

    
    
    float frontLight = 0.0;
    float backLight = 0.0;
    float topLight = 0.0;
    float bottomLight = 0.0;
    float lightVal = 0.0;
    float newAO = 0.0;
    float lightRes = 0.0;
    float lightFlooding = 0.0;
    
    float lightDis = 0.0;
    float totLightDis = 0.0;

    float lightColorization = 0.0;
    float totColorization = 0.0;

    float lightIntensity = 0.0;
    float totLightIntensity = 0.0;
    
    vec3 totLightColor = vec3(0.0);
    vec3 curLightColor = vec3(0.0);

    
    if (tot == 0.0) {
        resColor = getFogColor(TexCoord0.y);
    }
    else {
        


        for (j = 0; j < 2; j++) {
            totHits = 0.0;
            totRays = 0.0;
            for (i = 0; i < aoRad; i++) {

                    fi = float(i)/faoRad;

                    curRad = mix(minRad,maxRad[j],fi);
                    curRotInc = mix(minRotInc,maxRotInc,fi);

                    offsetCoord.x = cos(curRot)*curRad;
                    offsetCoord.y = sin(curRot)*curRad;
                    
                    newTC = TexCoord0.xy + offsetCoord/(bufferDim/newZoom);
                    samp = texture2D(Texture0, newTC );
                    curHeight = unpack16(samp.rg);



                    totHits += (clamp((curHeight - baseHeight)/(faoRad),-1.0,1.0)+1.0)/2.0; // - length(offsetCoord)
                    totRays += 1.0;

                    curRot += curRotInc;
            }
            ssao *= clamp((1.0 - totHits/totRays),0.0,1.0);
        }
        newAO = clamp(ssao, 0.0,1.0);

        if ( (tex0.a >= TEX_GRASS) && (tex0.a <= TEX_GRASS2) ) {
            newAO *= aoval;
        }




        // LIGHT LOOP START
        for (k = 0; k < numLights; k++) {
            baseInd = k*VECS_PER_LIGHT;
            lightPosWS = lightArr[baseInd+0];


            if (distance(worldPosition,lightPosWS.xyz) <= lightPosWS.w) {
                lightPosSS = lightArr[baseInd+1].xy;
                curLightColor = lightArr[baseInd+2].xyz;



                lightDis = 1.0-clamp(distance(worldPosition,lightPosWS.xyz)/lightPosWS.w,0.0,1.0);
                lightVec = normalize(lightPosWS.xyz-worldPosition);
                wEndPos = lightPosWS.xyz;
                sEndPos = lightPosSS;

                
                lightIntensity = lightArr[baseInd+2].w;
                lightColorization = lightArr[baseInd+3].r;
                lightFlooding = lightArr[baseInd+3].g;

                



                // shadows

                
                totHits = 0.0;
                totRays = 0.0;
                //hitCount = 0.0;
                for (i = 0; i < iNumSteps; i++) {
                    fi = float(i);
                    flerp = fi/fNumSteps;

                    wCurPos = mix(wStartPos,wEndPos,flerp);
                    sCurPos = mix(sStartPos,sEndPos,flerp);

                    samp = texture2D(Texture0, sCurPos);

                    curHeight = unpack16(samp.rg);

                    if (samp.b*samp.a < 1.0) {
                        wasHit = float( curHeight > wCurPos.z+2.0 );// *clamp(flerp+0.1,0.0,1.0);
                        totHits += wasHit;
                        lastHit = mix(lastHit, flerp, wasHit);
                        //hitCount += 1.0;
                    }

                }

                resComp = mix(1.0,0.0, totHits*4.0/fNumSteps );
                resComp = clamp(resComp,0.0,1.0);


                
                frontLight = clamp(dot(myVec,lightVec),0.0,1.0);
                backLight = clamp(dot(myVec,-lightVec),0.0,1.0);
                topLight = clamp(dot(myVec,vec3(0.0,0.0,1.0)),0.0,1.0);
                bottomLight = clamp(dot(myVec,vec3(0.0,0.0,-1.0)),0.0,1.0);


                totLightDis += lightDis;
                totColorization += lightColorization*lightDis*clamp(resComp+lightFlooding,0.0,1.0);
                totLightIntensity += lightIntensity*lightDis*resComp;

                totLightColor += frontLight*resComp*curLightColor*lightDis;
                totLightColor += vec3(0.0,0.5,1.0)*bottomLight*0.2*lightDis;
                totLightColor += vec3(1.0,0.5,0.0)*topLight*0.025*(1.0-newAO)*lightDis;
                totLightColor += vec3(0.0,0.5,1.0)*backLight*0.05*lightDis;

                resCompTot += resComp*lightDis;
                frontLightTot += frontLight*lightDis;

                

            }
        }
        // LIGHT LOOP END



        resCompTot = clamp(resCompTot,0.0,1.0);
        frontLightTot = clamp(frontLightTot,0.0,1.0);
        

        if (isGeom(tex0.ba)) {
            resColorTemp = unpackColorGeom(tex0.ba);
        }
        else {

            lightVal = clamp(dot(oneVec.xyz,totLightColor.xyz)/3.0,0.0,1.0);
            //lightRes = mix(newAO*0.4,lightVal, mix(0.4,0.7,lightVal));
            lightRes = mix(newAO*0.5, lightVal, lightVal) + ((1.0-newAO)-0.5)/2.0;
            lightRes = clamp(lightRes,0.0,1.0);

            resColorTemp = unpackColor(tex0.ba,lightRes);
            resColorTemp = mix(resColorTemp,unpackColor(tex0.ba,newAO),0.1);
            resColorTemp = mix(resColorTemp*vec3(0.4,0.4,1.0)*newAO,resColorTemp, min(frontLightTot+0.5,1.0)*resCompTot );



            // resColorTemp = mix(
            //     resColorTemp,
            //     globLightCol*vec3( clamp(dot(resColorTemp,oneVec.xyz)/3.0,0.0,1.0) ),
            //     globLightColIntensity
            // );
            resColorTemp = mix(
                resColorTemp,
                totLightColor*(lightRes + clamp(totLightDis,0.0,1.0) ),//vec3( clamp(dot(resColorTemp,oneVec.xyz)/3.0,0.0,1.0) ) + totLightColor*totLightIntensity,
                clamp(totColorization,0.0,1.0)
            );


            resColGS = vec3( clamp(dot(resColorTemp,oneVec.xyz)/3.0,0.0,1.0) );
            resColorTemp = mix(
                resColGS,
                resColorTemp,
                clamp(1.0-distance(resColGS,resColorTemp),0.0,1.0)*1.5//*mix(0.5,1.0,totLightDis)
            );


            // resColorTemp = mix(
            //     resColGS,
            //     resColorTemp,
            //     clamp(1.0-distance(resColGS,resColorTemp),0.0,1.0)*2.0//*mix(2.0,4.0,totLightDis)
            // );

            //resColorTemp += mix( (resColorTemp),vec3(0.5),0.5)/5.0;

            //resColorTemp = vec3(lightVal);
        }


        
        resColor = resColorTemp;

        //resColor = vec3(resCompTot);
    }

    resColor = clamp(resColor-outDif,0.0,1.0);


    gl_FragData[0].rgb = resColor;
    gl_FragData[0].a = tot;

}
