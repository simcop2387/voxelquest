// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// result fbo
uniform sampler2D Texture2;

// result fbo blurred
//uniform sampler2D Texture3;

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

float isNotGeom(vec2 num) {
    return float ( (unpack16(num)) < 32768.0);
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
    

    
    float curHeight;
    vec2 offsetCoord = vec2(0.0);
    vec4 samp = vec4(0.0);
    vec4 samp2 = vec4(0.0);
    vec4 samp3 = vec4(0.0);
    vec3 newTC = vec3(0.0);
    vec3 baseTC = vec3(0.0);
    vec3 newRes = vec3(0.0);

    vec3 baseNormOrig = normalize((tex1.rgb-0.5)*2.0);
    vec3 baseNormRef = normalize(reflect(lightVec,baseNormOrig));
    vec3 testNormOrig = vec3(0.0);
    vec3 testNormRef = vec3(0.0);

    float pi = 3.14159;

    float minRad = 2.0;
    float maxRad = bufferDim.y/1.0;
    float curRad = 0.0;
    
    float minRotInc = pi;
    float maxRotInc = 0.01;
    float curRotInc = 0.0;
    float curRot = 0.0;

    float minV = 0.0;
    float maxV = 0.0;
    float curV = 0.0;

    float totMin = 0.0;
    float totMax = 0.0;
    float avgVal = 0.0;

    vec3 minMaxCur = vec3(0.0);
    vec3 avgVec = vec3(0.0);

    baseTC.xy = TexCoord0.xy;
    baseTC.z = baseHeight/bufferDim.x;

    vec3 baseVec = vec3(0.0);
    vec3 testVec = vec3(0.0);
    
    const int numSteps = 256;
    float fNumSteps = float(numSteps);

    float finalRes = 0.0;

    for (i = 0; i < numSteps; i++) {


            fi = float(i)/fNumSteps;

            curRad = mix(minRad,maxRad,fi);
            curRotInc = mix(minRotInc,maxRotInc,fi);

            offsetCoord.x = cos(curRot)*curRad;
            offsetCoord.y = sin(curRot)*curRad;
            
            newTC.xy = TexCoord0.xy + offsetCoord/(bufferDim/newZoom);
            samp = texture2D(Texture2, newTC.xy );
            samp2 = texture2D(Texture1, newTC.xy );
            
            testNormOrig = normalize( (samp2.rgb-0.5)*2.0 );
            testNormRef = normalize(reflect(lightVec,testNormOrig));


            
            samp3 = texture2D(Texture0, newTC.xy );
            newTC.z = unpack16(samp3.rg)/bufferDim.x;
            testVec = normalize(baseTC-newTC);
            baseVec = -testVec;
            
            finalRes = (tex2.a*tex1.a*samp.a*samp2.a) * 
            clamp(dot(testNormOrig,testVec),0.0,1.0) * 
            clamp(dot(baseNormOrig,baseVec),0.0,1.0) * 
            (1.25-fi)*isNotGeom(samp3.ba); 
            //* float( dot(lightVec,baseVec) > 0.0 );// *(1.0-dot(oneVec,samp.rgb)/3.0);


            newRes += finalRes*samp.rgb*clamp(max(dot(baseNormOrig,testNormRef),dot(baseNormRef,-testNormRef)),0.0,1.0);// *(1.25-fi); //
            totRays += mix(finalRes,1.0,0.5); // *(1.25-fi);

            curRot += curRotInc;
    }

    newRes = clamp(newRes/(totRays/256.0),0.0,1.0);
    newRes = pow(newRes,vec3(2.0));
    gl_FragData[0] = vec4(newRes,1.0);

    
    


    

}