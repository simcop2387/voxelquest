#version 120

uniform sampler2D Texture0; // combine1
uniform sampler2D Texture1; // combine2

// geom fbo
uniform sampler2D Texture2;
uniform sampler2D Texture3;

uniform sampler2D Texture4; // resultFBO


varying vec2 TexCoord0;

uniform float heightmapMax;
uniform float curTime;
uniform float cameraZoom;
uniform float mapTrans;
uniform float seaLevel;
uniform vec3 worldSizeInPixels;

uniform vec4 fogPos;
uniform vec3 lightPosWS;
uniform vec3 cameraPos;
uniform vec2 bufferDim;
uniform vec2 mapDimInPixels;
uniform vec3 maxBoundsInPixels;
varying vec3 finalVec;




//curTime*speed[i]*2*pi/wavelength[i]

const float pi = 3.14159;
const int numWaves = 8;
float amplitude[8] = float[](0.4,0.2,0.05,0.05, 0.04,0.03,0.02,0.01 );
float wavelength[8] = float[]( 1992.0,1442.0,1332.0,535.0, 322.0, 162.0, 422.0, 233.0);
float speed[8] = float[]( 0.25,0.19,0.15,0.18, 0.13,0.11,0.1,0.05);
vec2 direction[8] = vec2[](
    vec2(-0.7,0.3),
    vec2(0.4,0.6),
    vec2(0.1,-0.9),
    vec2(0.25,-0.75),
    vec2(-0.2,0.4),
    vec2(0.3,0.6),
    vec2(-0.1,0.9),
    vec2(-0.25,0.75)
);



$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    
    vec4 pos = gl_Vertex;
    vec3 myVert = gl_Vertex.xyz - vec3(0.0,0.0,cameraPos.z);
    vec3 transVert;

    transVert.x = (myVert.x-myVert.y);
    transVert.y = (-(myVert.x/2.0) + -(myVert.y/2.0) + myVert.z);
    transVert.z = myVert.z;

    float newZoom = min(cameraZoom,1.0);
    
    finalVec.x = (transVert.x)*newZoom/(bufferDim.x);
    finalVec.y = (transVert.y)*newZoom/(bufferDim.y);
    finalVec.z = gl_Vertex.z;//(transVert.z);



    gl_Position = vec4(finalVec.xy, 0.5, gl_Vertex.w); //clamp( (1.0-finalVec.z/(256.0*255.0)) ,0.0,1.0)

}

$


int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}
float unpack16(vec2 num) {
    return num.r*255.0 + num.g*65280.0;
}



float wave(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];  
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    return amplitude[i] * sin(theta * frequency + curTime * phase);
}

float waveHeight(float x, float y) {
    float height = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        height += wave(i, x, y);
    }
    return height;
}

float dWavedx(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].x * frequency*1000.0;
    return A * cos(theta * frequency + curTime * phase);
}

float dWavedy(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].y * frequency*1000.0;
    return A * cos(theta * frequency + curTime * phase);
}

vec3 waveNormal(float x, float y) {
    float dx = 0.0;
    float dy = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    vec3 n = vec3(-dx, -dy, 1.0);
    return normalize(n);
}

void main() {
    float bufferDim2 = bufferDim*2.0;

    vec2 newCoords = gl_FragCoord.xy/bufferDim2;
    vec4 tex0 = texture2D(Texture0, newCoords );
    vec4 tex1 = texture2D(Texture1, newCoords );
    vec4 tex2 = texture2D(Texture2, newCoords );
    vec4 tex3 = texture2D(Texture3, newCoords );
    vec4 tex4 = texture2D(Texture4, newCoords );



    vec4 refVal;
    float baseHeight = unpack16(tex0.rg);
    float baseHeight2 = unpack16(tex2.rg);
    float aoVal = tex1.a;
    float lval = 0.0;
    float newZoom = min(cameraZoom,1.0);
    float curSeaLev;
    float dif;
    float lightVal;
    vec3 worldPosition = vec3(0.0);
    vec3 worldPositionOcean = vec3(0.0);
    vec2 tcMod = (vec2(newCoords.x,1.0-newCoords.y)*2.0-1.0 );
    vec3 seaVal;
    vec3 resColor;
    vec3 lightVec;
    vec3 worldNormal;
    float newBase = 0.0;
    vec3 upVec = vec3(1.0,0.0,0.0);
    vec3 fogColor = vec3(0.6, 0.6, 0.7);

    float tot = float(tex0.r + tex0.g + tex0.b + tex0.a > 0.0);
    if (tot == 0.0) {
        gl_FragData[0] = tex4;//vec4(fogColor,1.0);
    }
    else {
        tcMod.x *= bufferDim.x/(newZoom);
        tcMod.y *= bufferDim.y/(newZoom);
        tcMod.y -= cameraPos.z;
        worldPosition.x = tcMod.y + tcMod.x/2.0 + (baseHeight);
        worldPosition.y = tcMod.y - tcMod.x/2.0 + (baseHeight);
        worldPosition.z = baseHeight;
        worldPosition.x += cameraPos.x;
        worldPosition.y += cameraPos.y;

        worldPositionOcean = worldPosition;

        curSeaLev = seaLevel*heightmapMax + waveHeight(worldPosition.x, worldPosition.y)*32.0;
        worldPositionOcean.z = curSeaLev;

        lightVec = normalize(lightPosWS-worldPositionOcean);
        worldNormal = waveNormal(worldPositionOcean.x, worldPositionOcean.y);

        newBase = max(baseHeight,baseHeight2);
        if (aoVal == 0.0) {
            dif = 9999.0;
            lval = (tex4.r+tex4.g+tex4.b)/3.0;
        }
        else {
            dif = (curSeaLev-newBase);
            refVal = texture2D(
                Texture4,
                (
                    gl_FragCoord.xy -
                    (worldNormal.xy-worldNormal.z)*clamp(dif,0.0,30.0)
                )/bufferDim2
            );

            lval = (refVal.r + refVal.g + refVal.b)/3.0;
        }

        //lval *= 1.3;


        lval = lval*0.5 + (1.0-(abs(dot(upVec,worldNormal))))*lval*0.5;
        
        seaVal = 
        mix(

            vec3(lval)*4.0,
            mix(
                vec3( 0.3, 1.0, 1.0 )*lval,
                vec3( 2.0/9.0, 2.0/5.0, 2.0/3.0 )*lval,
                clamp(dif/128.0, 0.0,1.0 )
            ),
            clamp( dif/16.0, 0.0,1.0 )
        );

        lightVal = clamp(dot(worldNormal,lightVec),0.0,1.0)*(1.0-clamp(distance(lightPosWS,worldPositionOcean)/10000.0,0.0,1.0));

        seaVal += pow(lightVal,12.0)*2.0*float(aoVal>0.0);


        vec3 newFog;

        if ( worldPositionOcean.z < worldPosition.z) {
            newFog = (fogPos.xyz-worldPosition.xyz);
        }
        else {
            newFog = (fogPos.xyz-worldPositionOcean.xyz);
        }
        newFog /= 2048.0;
        
        newFog.xy /= 2.0;
        vec3 fogXYZ = 1.0-clamp( newFog, 0.0, 1.0);
        float fogLen = 1.0-clamp(1.0-(fogXYZ.x*fogXYZ.y),0.0,1.0);
        float hfog = min(clamp(sqrt(fogLen),0.0,1.0),fogXYZ.z);
        hfog *= float(baseHeight > 0.0);
        hfog = pow(hfog, 2.0);
        
        resColor = mix(
            tex4.rgb,
            seaVal,
            float(newBase < curSeaLev)
        );


        resColor = mix(
            tex4.rgb,
            seaVal,
            float(newBase < curSeaLev)
        );

        

        gl_FragData[0] = vec4(mix( fogColor, resColor, hfog ),1.0);
    }

    
}


