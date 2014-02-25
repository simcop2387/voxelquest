#version 120

// WaterShaderCombine
uniform sampler2D Texture0;
uniform sampler2D Texture1;

varying vec2 TexCoord0;

uniform float cameraZoom;
uniform vec3 cameraPos;
uniform vec2 bufferDim;
uniform vec2 mapDimInPixels;


const float TEX_WATER = 34.0/255.0;
const float TEX_GLASS = 35.0/255.0;


$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;// = ;//TexCoord0 = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
}

$


void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    float newZoom = min(cameraZoom,1.0);    

    int i;
    float fi;

    const int aoRad = 64;
    const float faoRad = float(aoRad);
    const int aoRadStep = 1;
    float pi = 3.14159;
    
    float minRad = 2.0;
    float maxRad = 16.0;
    float curRad = 0.0;
    
    float minRotInc = pi;
    float maxRotInc = 0.01;
    float curRotInc = 0.0;

    vec2 totHits = vec2(0.0);

    float maxHits = 0.0;
    float waterHits = 0.0;
    
    float curRot = 0.0;

    float temp = 0.0;

    vec2 newTC = vec2(2.0);
    vec3 newNorm = vec3(0.0);
    vec2 offsetCoord = vec2(0.0);

    vec4 samp = vec4(0.0);

    for (i = 0; i < aoRad; i++) {

            fi = float(i)/faoRad;

            curRad = mix(minRad,maxRad,fi);
            curRotInc = mix(minRotInc,maxRotInc,fi);

            offsetCoord.x = cos(curRot)*curRad;
            offsetCoord.y = sin(curRot)*curRad;
            
            newTC = TexCoord0.xy + offsetCoord/(bufferDim/newZoom);
            samp = texture2D(Texture0, newTC );

            temp = float(samp.a == TEX_WATER);

            totHits += temp*normalize(offsetCoord.xy);

            maxHits += 1.0;
            waterHits += temp;

            curRot += curRotInc;
    }

    gl_FragData[0] = tex0;    

    if (tex0.a == TEX_WATER) {
        newNorm = normalize((tex1.rgb-0.5)*2.0);


        newNorm.xy = ( mix(normalize(-totHits), newNorm.xy, waterHits/maxHits) );

        newNorm = normalize(newNorm);

        gl_FragData[1].a = tex1.a;
        gl_FragData[1].rgb = (newNorm + 1.0)/2.0;
    }
    else {
        gl_FragData[1] = tex1;
    }
    
}


