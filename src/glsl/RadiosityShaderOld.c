// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// result fbo
uniform sampler2D Texture2;

uniform float passNum;
uniform float heightmapMax;
uniform float cameraZoom;
uniform vec2 bufferDim;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec3 lightPosWS;

varying vec2 TexCoord0;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
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


void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);
    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec3 oneVec = vec3(1.0);
    
    float newZoom = min(cameraZoom,1.0);
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




    vec3 lightVec = normalize(lightPosWS-worldPosition);



    float totHits = 0.0;
    float totRays = 0.0;
    int i;
    int j;
    float fi;
    float fj;
    const int aoRad = 256;
    float faoRad = float(aoRad);
    int aoRadStep = 8;
    float curHeight;
    vec2 offsetCoord = vec2(0.0);
    vec4 samp = vec4(0.0);
    vec4 samp2 = vec4(0.0);
    vec2 newTC = vec2(0.0);
    float newRes = vec4(0.0);

    vec3 baseNorm = normalize((tex1.rgb-0.5)*2.0);
    vec3 baseNorm2 = normalize(reflect(lightVec,baseNorm));
    vec3 testNorm = vec3(0.0);

    float pi = 3.14159;
    float minRad = 2.0;
    float maxRad = 256.0;
    float curRad = 0.0;
    float minRotInc = pi;
    float maxRotInc = 0.01;
    float curRotInc = 0.0;
    float curRot = 0.0;

    float passMod = passNum*pi/2.0;

    float minV = 0.0;
    float maxV = 0.0;
    float curV = 0.0;

    float totMin = 0.0;
    float totMax = 0.0;
    float avgVal = 0.0;

    vec3 minMaxCur = vec3(0.0);
    vec3 avgVec = vec3(0.0);

    if (passNum < 2.0) {
        for (i = 0; i < aoRad; i++) {


                fi = float(i)/faoRad;

                curRad = mix(minRad,maxRad,fi);
                curRotInc = mix(minRotInc,maxRotInc,fi);

                offsetCoord.x = cos(curRot+passMod)*curRad;
                offsetCoord.y = sin(curRot+passMod)*curRad;
                
                newTC = TexCoord0.xy + offsetCoord/(bufferDim/newZoom);
                samp = texture2D(Texture2, newTC );
                samp2 = texture2D(Texture1, newTC );

                testNorm = normalize( (samp2.rgb-0.5)*2.0 );
                testNorm = normalize(reflect(lightVec,testNorm));
                //curHeight = unpack16(samp.rg);
                newRes += (tex2.a*samp.a)*samp.r*samp2.a*clamp(max(dot(baseNorm,testNorm),dot(baseNorm2,-testNorm)),0.0,1.0)*(1.25-fi); //
                totRays += (tex2.a*samp.a)*1.0*(1.25-fi);

                curRot += curRotInc;
        }

        newRes = clamp(newRes/(totRays/4.0),0.0,1.0);
        newRes = pow(newRes,2.0);
        vec3 finalCol = vec3(newRes+tex2.r);
        gl_FragData[0] = vec4(finalCol,1.0);

    }
    else {

        for (i = 0; i < aoRad; i++) {


                fi = float(i)/faoRad;

                curRad = mix(minRad,maxRad,fi);
                curRotInc = mix(minRotInc,maxRotInc,fi);

                offsetCoord.x = cos(curRot)*curRad;
                offsetCoord.y = sin(curRot)*curRad;
                
                newTC = TexCoord0.xy + offsetCoord/(bufferDim/newZoom);
                samp = texture2D(Texture2, newTC );


                if (samp.a > 0.0) {
                    avgVal += samp.r;
                    totMin += 1.0;
                }

                curRot += curRotInc;
        }

        avgVal /= totMin;

        totMin = 0.0;

        for (i = 0; i < aoRad; i++) {


                fi = float(i)/faoRad;

                curRad = mix(minRad,maxRad,fi);
                curRotInc = mix(minRotInc,maxRotInc,fi);

                offsetCoord.x = cos(curRot)*curRad;
                offsetCoord.y = sin(curRot)*curRad;
                
                newTC = TexCoord0.xy + offsetCoord/(bufferDim/newZoom);
                samp = texture2D(Texture2, newTC );


                if (samp.a > 0.0) {

                    if (samp.r < avgVal) {
                        minV += samp.r;
                        totMin += 1.0;
                    }
                    else {
                        maxV += samp.r;
                        totMax += 1.0;
                    }
                }

                curRot += curRotInc;
        }

        minMaxCur.r = minV / totMin;
        minMaxCur.g = maxV / totMax;
        minMaxCur.b = tex2.r;

        avgVec = vec3(avgVal);

        minMaxCur.b = mix(minMaxCur.b,avgVec.b,0.5);
        //minMaxCur = mix(minMaxCur,avgVec,0.75);

        newRes = tex2.a*(minMaxCur.b-minMaxCur.r)/(minMaxCur.g-minMaxCur.r);
        if (passNum == 3.0) {
            newRes = clamp(newRes,0.0,1.0);
        }

        vec3 finalCol = vec3(newRes);
        gl_FragData[0] = vec4(finalCol,tex2.a);
    }

    


    

}