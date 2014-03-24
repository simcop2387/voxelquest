#version 120

// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// geom fbo
uniform sampler2D Texture2;
uniform sampler2D Texture3;

// pal fbo
uniform sampler2D Texture4;

// radiosity
uniform sampler2D Texture5;


uniform float holderSizeInPixels;
uniform float visPageSizeInPixels;
uniform float unitSizeInPixels;
uniform float directPass;
uniform float fogOn;
uniform float waterOn;

uniform float gridOn;
uniform float cameraZoom;
uniform float diskOn;
uniform float curTime;
uniform vec2 bufferDim;
uniform vec2 mouseCoords;
uniform vec2 resolution;

uniform vec2 aoPosSS;
uniform vec3 cameraPos;

uniform vec4 activeObjectPos;
uniform vec4 lastUnitPos;
uniform vec4 lastPagePos;
uniform vec4 fogPos;

const int maxLights = 3;

uniform vec3 lightPosWS;
uniform vec2 lightPosSS;

uniform vec3 lightPosWSArr[maxLights];
uniform vec2 lightPosSSArr[maxLights];


const float TEX_GRASS = 12.0;



varying vec2 TexCoord0;

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

vec3 unpackColor(vec2 num, float lightVal) {
    int test = int(unpack16(num));
    ivec3 intRGB;
    vec3 res = vec3(0.0);

    if (test >= 32768) {
        test -= 32768;

        intRGB.b = test/1024;
        test -= intRGB.b*1024;

        intRGB.g = test/32;
        test -= intRGB.g*32;

        intRGB.r = test;

        res = vec3(intRGB)/31.0;

    }
    else {
        res = texture2D( Texture4, vec2(lightVal, (num.g*255.0+num.r*2.0 + 0.5)/255.0 ) ).rgb;
    }


    return res;
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

    vec4 tex5 = texture2D(Texture5, TexCoord0.xy);

    

    float newZoom = min(cameraZoom,1.0);
    float newZoom2 = max(cameraZoom,1.0);

    float newZoom3 = mix(newZoom,cameraZoom, float(cameraZoom <= 1.0) );
    

    
    float tot = float(tex0.r + tex0.g + tex0.b + tex0.a + tex2.r + tex2.g + tex2.b + tex2.a > 0.0);
    /*if (tot == 0.0) {
        discard;
    }
    */
    
    float baseHeight = unpack16(tex0.rg);//tex0.r*255.0 + tex0.g*255.0*256.0;
    

    float baseHeight2 = unpack16(tex2.rg);
    float geomMod = float(tex2.a > 0.0)*mix(1.0,0.2, clamp( abs(baseHeight2 - baseHeight)/10.0, 0.0, 1.0 )*0.75 + float(baseHeight2 < baseHeight)*0.25  );

    float geomInFront = float(baseHeight2 > baseHeight);


    vec3 worldPosition = vec3(0.0,0.0,0.0);
    vec2 tcMod = (vec2(TexCoord0.x,1.0-TexCoord0.y)*2.0-1.0 );
    tcMod.x *= bufferDim.x/(newZoom);
    tcMod.y *= bufferDim.y/(newZoom);


    tcMod.y -= cameraPos.z;
    worldPosition.x = tcMod.y + tcMod.x/2.0 + (baseHeight);// + visPageSizeInPixels/2.0;
    worldPosition.y = tcMod.y - tcMod.x/2.0 + (baseHeight);// + visPageSizeInPixels/2.0;
    worldPosition.z = baseHeight;
    worldPosition.x += cameraPos.x;
    worldPosition.y += cameraPos.y;


    


    vec3 lightVec = normalize(lightPosWS-worldPosition);

    vec3 targ;
    vec3 orig = vec3(TexCoord0.x, TexCoord0.y, baseHeight);

    float resComp = 1.0;
    float aoval = tex1.a;
    float notBlank = float(aoval != 0.0);


    int i;
    int j;
    float fj;
    float fi;
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



    float mval = diskOn*float( distanceAspect(sStartPos,aoPosSS,aspectRatio)/(newZoom3) < 0.1)*0.2;

    float wpDis = distance(activeObjectPos.xy,worldPosition.xy);

    vec2 wpNorm = normalize(activeObjectPos.xy-worldPosition.xy);

    float theta = atan(wpNorm.y,wpNorm.x);

    float pval = float(wpDis < 100.0 && wpDis > 75.0) *
        float(abs(sin(theta*8.0 + curTime/100.0)) > 0.75) * 
        (1.0-clamp(abs(activeObjectPos.z-worldPosition.z)/1024.0,0.1,1.0)) *
        diskOn * float(activeObjectPos.z > worldPosition.z) * 0.5;

    float totHits = 0.0;
    float wasHit = 0.0;
    float lastHit = 0.0;
    


    const int iNumSteps = 128;
    const int aoRad = 16;
    const float fNumSteps = float(iNumSteps);
    const float faoRad = float(aoRad);
    const int aoRadStep = 1;

    float ssao = 0.0;
    float totRays;
    vec2 offsetCoord = vec2(0.0);
    

    

    // vec2 tcm;
    // tcm = TexCoord0.xy;
    // tcm.x *= bufferDim.x/(newZoom);
    // tcm.y *= bufferDim.y/(newZoom);

    vec4 testTex = vec4(0.0);
    vec3 bucket = vec3(0.0);//tex1.xyz;

    vec2 dirModXY[4];

    dirModXY[0] = vec2(1.0,0.0);
    dirModXY[1] = vec2(-1.0,1.0);
    dirModXY[2] = vec2(0.0,1.0);
    dirModXY[3] = vec2(0.0,-1.0);

    float testHeight = 0.0;
    float bestHeight = 0.0;
    float maxDif = 512.0;

    for (i = 0; i < 4; i++) {

        testTex = texture2D(Texture0, TexCoord0.xy + dirModXY[i].xy/bufferDim );
        
        // if (testTex.a != tex0.a) {
        //     bestHeight = maxDif;
        //     break;
        // }
        // else {
            testHeight = abs( baseHeight-unpack16(testTex.rg) );
            if (testHeight > bestHeight) {
                bestHeight = testHeight;
            }
        //}
        

    }

    // if (bestHeight == 0.0) {
    //     for (i = 0; i < 4; i++) {

    //         testTex = texture2D(Texture0, TexCoord0.xy + dirModXY[i].xy/bufferDim );

    //         testHeight = (unpack16(testTex.rg)-(baseHeight) );
    //         if (testHeight > bestHeight) {
    //             bestHeight = testHeight;
    //         }
    //     }
    // }

    float outDif = clamp(bestHeight/maxDif,0.0,1.0)*float(tex0.a != TEX_GRASS/255.0)*0.25;



    vec3 myVec = vec3(0.0);//(tex1.rgb-0.5)*2.0;
    //vec3 normTot = vec3(0.0);
    //vec3 normTot2 = (tex1.rgb-0.5)*2.0;
    //vec3 oneVec = vec3(1.0);
    //float bestNorm = 0.0;

    for (i = -1; i <= 1; i += 1) {
        fi = float(i);
        for (j = -1; j <= 1; j+= 1) {
            fj = float(j);

            testTex = texture2D(Texture1, TexCoord0.xy + vec2(fi,fj)/bufferDim );
            
            // normTot = (testTex.rgb-0.5)*2.0;

            // if ( dot((normalize(normTot)),oneVec) > bestNorm) {
            //     bestNorm = dot((normalize(normTot)),oneVec);
            //     normTot2 = normTot;
            // }

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
        resComp = 1.0;

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

        //myVec = normTot2;
        myVec = normalize( (tex1.rgb-0.5)*2.0 );

        // if (
        //     abs(
        //         dot(abs(normalize(normTot)),oneVec) - dot(abs(normalize(normTot2)),oneVec)
        //     ) > 0.025 
        // ) {
        //     myVec = 0.0;
        // }
        // else {
        //     myVec = normalize(normTot2);
        // }
    }


    for (i = 0; i < iNumSteps; i++) {
        fi = float(i);
        flerp = fi/fNumSteps;

        wCurPos = mix(wStartPos,wEndPos,flerp);
        sCurPos = mix(sStartPos,sEndPos,flerp);

        samp = texture2D(Texture0, sCurPos);

        curHeight = unpack16(samp.rg);

        wasHit = float( curHeight > wCurPos.z+2.0 );
        totHits += wasHit;
        lastHit = mix(lastHit, flerp, wasHit);
    }

    resComp = mix(1.0,0.0, totHits*4.0/fNumSteps );
    resComp = clamp(resComp,0.0,1.0);




    totHits = 0.0;
    totRays = 0.0;
    for (j = -aoRad; j <= aoRad; j += aoRadStep) {
        fj = float(j);
        for (i = -aoRad; i <= aoRad; i += aoRadStep) {
            fi = float(i);
            offsetCoord = vec2(fi,fj);
            samp = texture2D(Texture0, TexCoord0.xy + offsetCoord/(bufferDim/newZoom) );
            curHeight = unpack16(samp.rg);

            totHits += (clamp((curHeight - length(offsetCoord) - baseHeight)/(faoRad),-1.0,1.0)+1.0)/2.0;
            totRays += 1.0;
        }
    }

    ssao = 1.0 - totHits/totRays;




    //lightRes = mix(-0.2,1.4,lightRes)*newAO;
    // lightRes = mix(lightRes/4.0,lightRes,clamp( (resComp+newAO)*lightRes/newAO*0.8 ,0.0,1.0) );
    // lightRes += lightRes/2.0;
    // lightRes = mix(lightRes,0.5,0.5);
    // lightRes = (lightRes-0.25)*2.0;
    // lightRes += 0.3;

    // + pow(aoval,3.0)*0.1
    //

    //resColor = vec3( (resColor.r+resColor.g+resColor.b)/3.0 ) + newAO;
    //lightVal *= 1.0-clamp(distance(cameraPos,lightPosWS)/1024.0,0.0,1.0);
    //mix(ssao,aoval,0.2);//(aoval-0.5)*2.0;
    //mix(resCol0,resCol1,lightRes);



    vec3 fogColor = vec3(0.6, 0.6, 0.7);






    /////////////

    vec3 newFog = vec3(0.0);
    vec3 fogXYZ = vec3(0.0);
    float fogLen = 0.0;
    float hfog = 1.0;
    if ( (fogOn == 1.0) && (waterOn == 0.0) ) {
        // if ( worldPositionOcean.z < worldPosition.z) {
        //     newFog = (fogPos.xyz-worldPosition.xyz);
        // }
        // else {
        //     newFog = (fogPos.xyz-worldPositionOcean.xyz);
        // }

        // newFog += waveh*300.0;

        newFog = (fogPos.xyz-worldPosition.xyz);

        newFog /= 1024.0;
        newFog.xy /= 2.0;
        fogXYZ = 1.0-clamp( newFog, 0.0, 1.0);
        fogLen = 1.0-clamp(1.0-(fogXYZ.x*fogXYZ.y),0.0,1.0);
        hfog = min(clamp(sqrt(fogLen),0.0,1.0),fogXYZ.z);
        hfog *= float(baseHeight > 0.0);
        //hfog = clamp(hfog+clamp(1.0-waveh,0.0,1.0)/2.0,0.0,1.0);
        hfog = pow(hfog, 2.0);
    }

   

    /////////////






    float frontLight = clamp(dot(myVec,lightVec),0.0,1.0);
    float backLight = clamp(dot(myVec,-lightVec),0.0,1.0);
    float topLight = clamp(dot(myVec,vec3(0.0,0.0,1.0)),0.0,1.0);
    float bottomLight = clamp(dot(myVec,vec3(0.0,0.0,-1.0)),0.0,1.0);
    float lightVal = frontLight*resComp;
    float newAO = clamp( ((ssao)), 0.0,1.0);//pow( (ssao + aoval)*0.6 ,3.0);
    float lightRes = mix(newAO*0.5, lightVal, lightVal);
    lightRes = clamp(lightRes,0.0,1.0);

    
    
    //lightRes = mix(lightRes,tex5.r,0.5);

    vec3 resColor = unpackColor(tex0.ba,lightRes);
    vec3 resCol2 = unpackColor(tex2.ba,lightRes);

    resColor = mix(resColor,unpackColor(tex0.ba,newAO),0.1);
    resColor = mix(resColor*vec3(0.4,0.4,1.0)*0.75,resColor, min(frontLight+0.5,1.0)*resComp*hfog );




    resColor += vec3(0.0,0.5,1.0)*bottomLight*0.2*notBlank;
    resColor += vec3(1.0,0.5,0.0)*topLight*0.025*(1.0-newAO)*notBlank;
    resColor += vec3(0.0,0.5,1.0)*backLight*0.05*notBlank;

    //resColor = vec3(newAO);


    resColor = clamp(resColor-outDif,0.0,1.0);
    resColor = mix(fogColor,resColor,hfog);
    

    //resColor = vec3(newAO);


    vec3 grid0 = abs(mod(worldPosition, unitSizeInPixels) - unitSizeInPixels/2.0)*2.0;
    vec3 grid1 = abs(mod(worldPosition,visPageSizeInPixels) - visPageSizeInPixels/2.0)*2.0;
    vec3 grid2 = abs(mod(worldPosition, holderSizeInPixels) - holderSizeInPixels/2.0)*2.0;
    

    
    float gridVal0 = float( (grid0.x >= (unitSizeInPixels-2.0/newZoom)) || (grid0.y >= (unitSizeInPixels-2.0/newZoom)) ) * 0.1 * notBlank;
    float gridVal1 = float( (grid1.x >= (visPageSizeInPixels-3.0/newZoom)) || (grid1.y >= (visPageSizeInPixels-3.0/newZoom)) ) * 0.2 * notBlank;
    float gridVal2 = float( (grid2.x >= (holderSizeInPixels-4.0/newZoom)) || (grid2.y >= (holderSizeInPixels-4.0/newZoom)) ) * 0.5 * notBlank;


    

    vec3 finalCol = resColor + pval+mval+(gridVal0+gridVal1+gridVal2)*gridOn;
    //finalCol = mod(worldPosition,256.0)/255.0;

    if ( (geomMod*geomInFront == 0.0) ) {
        finalCol = finalCol*0.9 + resCol2*geomMod*0.5;
    }
    else {
       finalCol = finalCol*0.25 + resCol2*geomMod*3.0;
    }

    if (tot == 0.0) {
        finalCol = fogColor;
    }












    
    vec3 zeroVec = vec3(0.0);



    if (directPass == 1.0) {
        gl_FragData[0] = tex0;
    }
    else {
        gl_FragData[0] = vec4(finalCol,tot);//tot //mix(finalCol/8.0,finalCol+lightVal*0.1,resComp)
    }

    //gl_FragData[0] = vec4(notBlank,notBlank,notBlank,1.0);

}





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

// vec3 resCol0 = vec3(0.0,0.0,0.0);
// vec3 resCol1 = vec3(1.0,1.0,1.0);


// // stone
// if (tex0.b == 1.0/255.0) {
//     resCol0 = vec3(35.0/255.0, 32.0/255.0, 27.0/255.0)*0.3;
//     resCol1 = vec3(232.0/255.0, 225.0/255.0, 206.0/255.0);
// }

// // dirt
// if (tex0.b == 2.0/255.0) {
//     resCol0 = vec3(22.0/255.0, 9.0/255.0, 17.0/255.0);
//     resCol1 = vec3(110.0/255.0, 80.0/255.0, 70.0/255.0);
// }

// // grass
// if (tex0.b == 3.0/255.0) {

//     lightRes = clamp( pow((lightRes+aoval)/2.0,2.0),0.0,1.0);
//     resCol0 = vec3(10.0/255.0, 25.0/255.0, 0.0/255.0)*0.3;
//     resCol1 = vec3(145.0/255.0, 192.0/255.0, 62.0/255.0);
// }



// if (tex2.b == 4.0/255.0) {
//     resCol2 = vec3(1.0,1.0,1.0);
// }
// if (tex2.b == 5.0/255.0) {
//     resCol2 = vec3(0.0,0.0,1.0);
// }
// if (tex2.b == 6.0/255.0) {
//     resCol2 = vec3(1.0,0.0,0.0);
// }

// if (tex2.b == 8.0/255.0) {
//     resCol2 = vec3(1.0,0.0,1.0);
//     geomMod = pow(geomMod,3.0);

// }

// if (tex2.b == 9.0/255.0) {
//     resCol2 = vec3(0.0,0.0,1.0);
//     geomMod = float(geomMod>0.5);
// }


/*
// light
if (tex0.b == 4.0/255.0 || tex0.b == 5.0/255.0) {

    resCol0 = vec3(255.0/255.0, 255.0/255.0, 255.0/255.0);
    resCol1 = vec3(255.0/255.0, 255.0/255.0, 255.0/255.0);

}
*/


// if (aoval == 0.0) {
//     //resCol0 = vec3(0.0/255.0, 0.0/255.0, 0.0/255.0);
//     //resCol1 = vec3(0.0/255.0, 0.0/255.0, 0.0/255.0);
//     //resCol0 = mod(worldPosition,256.0)/255.0;
//     //resCol1 = resCol0;


//     //resCol0 = vec3 (0.0,0.0,0.0);
//     //resCol1 = vec3 (0.0,0.0,0.0);

// }



// if (tex0.b == 7.0/255.0) {
//     resColor = texture2D( Texture4, vec2(clamp(worldPosition.z/heightmapMax,0.0,1.0), (5.0 + 0.5)/255.0 ) ).rgb + lightRes-0.2;

// }


//resColor = vec3(lightRes,lightRes,lightRes);
//resColor = texture2D( Texture4, vec2(lightRes, (0.0 + 0.5)/255.0 ) ).rgb;
//resColor = mod(worldPosition,256.0)/255.0;
//vec3 resColor = vec3(lightRes);//mix(resCol0,resCol1,lightRes);//mod(worldPosition,256.0)/255.0;//vec3(lightRes);//mix(resCol0,resCol1,lightRes);
//vec3 resColor = tex1.rgb;
//vec4(lightRes*0.8,lightRes*0.7,lightRes*0.6, lightRes)
//+geomMod;//+isLastUnit+isLastPage;//+isLastUnit+isLastPage; //vec4(lightRes,lightRes,lightRes,1.0);//
//gl_FragData[0] = vec4(aoval,aoval,aoval,1.0);

//gl_FragData[0] = vec4(lightRes,lightRes,lightRes,1.0);
//gl_FragData[1] = gl_FragData[0];

