#version 120

uniform sampler2D Texture0; // combine1
uniform sampler2D Texture1; // combine2

// geom fbo
uniform sampler2D Texture2;
uniform sampler2D Texture3;

uniform sampler2D Texture4; // resultFBO
uniform sampler2D Texture5; // resultFBOBlurred

uniform sampler2D Texture6; // clouds

uniform sampler2D Texture7; // water
uniform sampler2D Texture8; // water

uniform sampler2D Texture9; // palfbo


varying vec2 TexCoord0;

uniform float heightmapMax;
uniform float curTime;
uniform float cameraZoom;
uniform float mapTrans;
uniform float seaLevel;
uniform vec3 worldSizeInPixels;

uniform vec3 lightPosWS;
uniform vec3 cameraPos;
uniform vec2 bufferDim;
uniform vec2 mapDimInPixels;
uniform vec3 maxBoundsInPixels;
varying vec3 finalVec;

const float timeScale = 0.0002;


const float TEX_WATER = 32.0/255.0;
const float TEX_GLASS = 35.0/255.0;

const float pi = 3.14159;
const int numWaves = 8;
float amplitude[8] = float[]( 1.0/2.0, 1.0/4.0, 1.0/8.0, 1.0/16.0, 1.0/32.0, 1.0/64.0, 1.0/128.0, 1.0/256.0 );
float wavelength[8] = float[]( 48.0/1.0, 48.0/5.0, 48.0/9.0, 48.0/19.0, 48.0/29.0, 48.0/41.0, 48.0/53.0, 48.0/68.0 );
float speed[8] = float[]( 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);
vec2 direction[8] = vec2[](
    vec2(cos(-0.7),sin(-0.7)),
    vec2(cos(0.4),sin(0.4)),
    vec2(cos(0.1),sin(0.1)),
    vec2(cos(-0.75),sin(-0.75)),
    vec2(cos(-0.2),sin(-0.2)),
    vec2(cos(0.3),sin(0.3)),
    vec2(cos(-0.1),sin(-0.1)),
    vec2(cos(-0.25),sin(-0.25))
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

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


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
    return amplitude[i] * sin(theta * frequency + curTime*timeScale * phase);
}

float waveHeight(vec2 param) {

    float x = param.x;
    float y = param.y;

    float height = 0.0;
    for (int i = 0; i < numWaves; ++i)
        height += wave(i, x, y);
    return height;
}

float dWavedx(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].x * frequency;
    float res = A * cos(theta * frequency + curTime*timeScale * phase);
    return res*32.0;
}

float dWavedy(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].y * frequency;
    float res =  A * cos(theta * frequency + curTime*timeScale * phase);
    return res*32.0;
}

vec3 waveNormal(vec2 param) {
    float x = param.x;
    float y = param.y;
    float dx = 0.0;
    float dy = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    vec3 n = vec3(-dx, -dy, 1.0);
    return normalize(n);
}

vec3 unpackColor(vec2 num, float lightVal) {
    return texture2D( Texture9, vec2(lightVal, (num.g*255.0+num.r*2.0 + 0.5)/255.0 ) ).rgb;
}


void main() {

    vec2 newCoords = gl_FragCoord.xy/bufferDim;
    vec4 tex0 = texture2D(Texture0, newCoords );
    vec4 tex1 = texture2D(Texture1, newCoords );
    vec4 tex4 = texture2D(Texture4, newCoords );

    vec4 tex7 = texture2D(Texture7, newCoords );
    vec4 tex8 = texture2D(Texture8, newCoords );

    vec4 tex7orig = tex7;
    vec4 tex8orig = tex8;
    vec3 transTex = vec3(0.0);

    vec4 refVal;
    float baseHeight = unpack16(tex0.rg);

    float baseHeight3 = 0.0;
    float isWater = 0.0;
    float waterAO = 0.0;//tex8.a;

    float aoVal = tex1.a;
    float lval = 0.0;
    float newZoom = min(cameraZoom,1.0);
    float curSeaLev;
    float dif;
    float lightVal;
    float waveh = 0.0;
    float backLight = 0.0;
    float topLight = 0.0;
    float bottomLight = 0.0;

    vec3 seaNorm = vec3(0.0);

    vec3 worldPosition = vec3(0.0);
    vec3 worldPositionOcean = vec3(0.0);
    vec2 tcMod = (vec2(newCoords.x,1.0-newCoords.y)*2.0-1.0 );
    vec3 seaVal;
    vec3 resColor;
    vec3 lightVec;
    vec3 worldNormal;
    vec3 upVec = vec3(1.0,0.0,0.0);
    vec4 cloudVal = vec4(0.0);

    vec2 woff = vec2(0.0);

    vec4 texR0 = vec4(0.0);
    float baseHeightR = 0.0;
    float difR = 0.0;
    vec2 refCoords = vec2(0.0);

    vec2 testPos = vec2(0.0);

    float waveMod = 0.0;


    float tot = float(tex0.r + tex0.g + tex0.b + tex0.a > 0.0);

    float baseHeight3orig = unpack16(tex7.rg);
    baseHeight3 = baseHeight3orig;

    isWater = float(
        (baseHeight3>baseHeight)
    );

    float isStillWater = float(baseHeight3 > baseHeight)*float(tex7orig.a == TEX_WATER);

    bool notWater = ( (aoVal == 0.0) && (isWater == 0.0)  );

    tcMod.x *= bufferDim.x/(newZoom);
    tcMod.y *= bufferDim.y/(newZoom);
    tcMod.y -= cameraPos.z;
    worldPosition.x = tcMod.y + tcMod.x/2.0 + (baseHeight);
    worldPosition.y = tcMod.y - tcMod.x/2.0 + (baseHeight);
    worldPosition.z = baseHeight;
    worldPosition.x += cameraPos.x;
    worldPosition.y += cameraPos.y;

    waveMod = -(seaLevel*heightmapMax - baseHeight)*1.0;

    float waveScale = 1.0/256.0;

    

    waveh = abs(waveHeight(  (worldPosition.xy-worldPosition.z)*waveScale ))*128.0;

    


    woff.y = waveh*newZoom/bufferDim.y;

    tex7 = texture2D(Texture7, newCoords + woff );
    tex8 = texture2D(Texture8, newCoords + woff );

    seaNorm = normalize((tex8.rgb-0.5)*2.0);


    baseHeight3 = unpack16(tex7.rg);

    isWater = float(
        (baseHeight3>baseHeight)
    );
    waterAO = tex8.a;

    testPos = (worldPosition.xy-worldPosition.z)*waveScale + 1024.0*waterAO + baseHeight3*(1.0-waterAO)/256.0;
    worldNormal = waveNormal( testPos );
    if (worldNormal.z < 0.0) {
        worldNormal.z *= -1.0;
    }
    worldNormal = normalize(worldNormal);

    worldPositionOcean = worldPosition;

    lightVec = normalize(lightPosWS-worldPosition);
    lightVal = clamp(dot( normalize((tex8orig.rgb-0.5)*2.0),lightVec),0.0,1.0);
    transTex = unpackColor(tex7orig.ba,lightVal);


    curSeaLev = seaLevel*heightmapMax + waveh;
    worldPositionOcean.z = curSeaLev;
    

    lightVec = normalize(lightPosWS-worldPositionOcean);
    

    if (aoVal == 0.0) {
        dif = 9999.0;
        lval = (tex4.r+tex4.g+tex4.b)/3.0;
    }
    else {

        dif = (curSeaLev-baseHeight);

        
        refCoords = 
            (
                gl_FragCoord.xy +
                (worldNormal.xy-worldNormal.z)*clamp(dif,0.0,32.0)*(newZoom/1.0)*waterAO
                + seaNorm.xy*(1.0-waterAO)*80.0
            )/bufferDim;


        refCoords = mix(gl_FragCoord.xy/bufferDim, refCoords, clamp(dif/32.0,0.0,1.0));


        refVal = texture2D(Texture5, refCoords);

        //refVal = mix(tex4,refVal,clamp(dif/32.0,0.0,1.0));

        lval = (refVal.r + refVal.g + refVal.b)/3.0;

        cloudVal = texture2D(
            Texture6,
            (
                (worldPosition.xy-worldPosition.z) -
                (worldNormal.xy-worldNormal.z)*clamp(dif,0.0,60.0)*(newZoom/1.0)
                + curTime*0.2
            )/(bufferDim*2.0)
        );

    }

    if (dif == 9999.0) {
        difR = 9999.0;
    }
    else {
        texR0 = texture2D(Texture0, refCoords );
        baseHeightR = unpack16(texR0.rg);
        difR = max(curSeaLev-baseHeightR,0.0);

        difR = mix(dif,difR,clamp(difR/128.0,0.0,1.0));

        if (waterAO == 0.0) {
            difR = (baseHeight3-baseHeight);
        }

    }

    lval = lval * (0.5 + clamp((1.0 - (abs(  dot(lightVec,worldNormal) ))),0.0,1.0) * 0.5);
    
    seaVal = 
    mix(

        mix(
            
            tex4.rgb,
            vec3(lval)*2.0,
            clamp( (dif)/8.0, 0.0,1.0 )
        ),
        mix(
            vec3( 0.3, 1.0, 1.0 )*lval*1.4,
            vec3( 2.0/9.0, 2.0/5.0, 2.0/3.0 )*lval,
            clamp(difR/256.0, 0.0,1.0 )
        ),
        clamp( difR/32.0 - rand(TexCoord0.xy*curTime/10000.0)/4.0 , 0.0,1.0 )
    );

    if (waterAO > 0.0) {
        seaVal = mix(vec3(lval)*2.0, seaVal, clamp( (baseHeight3orig-baseHeight)/64.0, 0.0, 1.0 ) );
    }
    
    lightVal = clamp(dot(worldNormal,lightVec),0.0,1.0);

    lightVal *= (1.0-clamp(distance(lightPosWS,worldPositionOcean)/8000.0,0.0,1.0));

    seaVal = ( seaVal + (pow(lightVal,3.0)*vec3(0.2,0.8,1.0) - cloudVal.rgb/8.0)*waterAO ) * isWater; // *clamp(dot(upVec,worldNormal),0.0,1.0)

    
    resColor = mix(
        tex4.rgb,
        seaVal + clamp(dot(seaNorm,lightVec)*isWater*(1.0-waterAO)*0.25,0.0,1.0)*vec3(0.2,0.8,1.0),
        isWater*isStillWater
    );



    if ( notWater ) {
        resColor = tex4.rgb;
    }

    if (
        (baseHeight3orig > baseHeight) &&
        (tex7orig.a == TEX_GLASS)
    ) {
        resColor = transTex*0.5 + resColor*0.5;
    } 

    gl_FragData[0] = vec4(resColor,tex4.a);
    

    
}


