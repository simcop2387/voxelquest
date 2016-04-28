#version 330

uniform sampler3D Texture0; // pal fbo
uniform sampler2D Texture1; // hmFBO
uniform sampler2D Texture2; // cityFBO;
uniform sampler2D Texture3; // hmFBOLinear
uniform sampler2D Texture4; // imagevoro
//varying vec2 TexCoord0;

uniform vec4 blitCoords;
uniform float curTime;
uniform float cameraZoom;
//uniform float mapTrans;

uniform vec4 mapFreqs;
uniform vec4 mapAmps;

uniform float seaLevel;
uniform vec3 cameraPos;
uniform vec3 lookAtVec;
uniform vec2 bufferDim;
uniform vec2 mapDimInPixels;

uniform float mapPitch;
uniform float heightMapMaxInCells;
uniform float cellsPerWorld;

^INCLUDE:MATERIALS^

$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vtexcoord;
out vec4 TexCoord0;

void main() {

    TexCoord0 = vtexcoord;
    //TexCoord0 = gl_MultiTexCoord0.xy;
    //TexCoord0.y = TexCoord0.y;
    
    gl_Position = vposition;
    gl_Position.xy = mix(blitCoords.xy, blitCoords.zw, vtexcoord.xy);//vposition;
    
    
    //gl_Position = gl_Vertex;

    // vec3 finalVec;
    // vec4 pos = gl_Vertex;

    // vec3 myVert = gl_Vertex.xyz;// - vec3(0.0,0.0,cameraPos.z);
    // vec3 transVert;

    // transVert.x = (myVert.x-myVert.y);
    // transVert.y = (-(myVert.x/2.0) + -(myVert.y/2.0) + myVert.z);
    // transVert.z = myVert.z;

    
    // float newZoom = cameraZoom;//min(cameraZoom,1.0);
    
    // finalVec.x = (transVert.x)*newZoom/(bufferDim.x);
    // finalVec.y = (transVert.y)*newZoom/(bufferDim.y);
    // finalVec.z = gl_Vertex.z;//(transVert.z);


    // //float hv = mix(0.5,0.0,float(matVal == 4.0));
    // //*0.5 + hv

    // gl_Position = vec4(finalVec.xy, 0.5, gl_Vertex.w); //clamp( (1.0-finalVec.z/(256.0*255.0)) ,0.0,1.0)

}

$


in vec4 TexCoord0;
layout(location = 0) out vec4 FragColor0;

/*

vec3 COLOR_MASKS[16] = vec3[](
    vec3(  1.0,  0.0,  0.0  ),
    vec3(  1.0,  1.0,  0.0  ),
    vec3(  0.0,  1.0,  0.0  ),
    vec3(  0.0,  1.0,  1.0  ),
    vec3(  0.0,  0.0,  1.0  ),
    vec3(  1.0,  0.0,  1.0  ),

    vec3(  0.7,  0.0,  0.0  ),
    vec3(  0.7,  0.7,  0.0  ),
    vec3(  0.0,  0.7,  0.0  ),
    vec3(  0.0,  0.7,  0.7  ),
    vec3(  0.0,  0.0,  0.7  ),
    vec3(  0.7,  0.0,  0.7  ),
   
    vec3(  0.3,  0.0,  0.0  ),
    vec3(  0.3,  0.3,  0.0  ),
    vec3(  0.0,  0.3,  0.0  ),
    vec3(  0.0,  0.3,  0.3  )

);
*/

int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}

float getGrid(int val, vec2 gridVecBase, float thickness) {

    int testVal = val;

    int hasDown = testVal / 8;
    testVal -= hasDown * 8;
    
    int hasUp = testVal / 4;
    testVal -= hasUp*4;
    
    int hasRight = testVal/2;
    testVal -= hasRight*2;

    int hasLeft = testVal;

    float maxT = 1.0-thickness;
    float minT = thickness;

    return float(
        ( (gridVecBase.x >= maxT) && (hasLeft > 0) ) ||
        ( (gridVecBase.x <= minT) && (hasRight > 0)  ) ||
        ( (gridVecBase.y >= maxT) && (hasUp > 0)  ) ||
        ( (gridVecBase.y <= minT) && (hasDown > 0) )
    );
}

^INCLUDE:SampleFuncs^

^INCLUDE:TerHeightFunc^


vec2 opRotate( vec2 p, float theta )
{
    mat2 m = mat2(
        cos(theta),
        -sin(theta),
        sin(theta),
        cos(theta)
    );
    
    return m*p;
}

void main() {

    float newTime = curTime/100.0;

    vec2 camPosZO = cameraPos.xy/cellsPerWorld;

    vec2 tcFlip = TexCoord0.xy;
    tcFlip.y = 1.0-tcFlip.y;

    vec2 baseTC = (tcFlip.xy - 0.5);
    
    
    float M_PI = 3.14159265359;    

    baseTC = opRotate(baseTC,M_PI*0.5+2.0*M_PI-atan(lookAtVec.y,lookAtVec.x));

    vec2 newTC = 
        (baseTC + camPosZO*cameraZoom )/cameraZoom
        
        
        ;//(TexCoord0.xy + camPosZO*cameraZoom - 0.5)/cameraZoom;

    //vec2 newTC = ( (TexCoord0.xy - 0.5)-(camPosZO))/cameraZoom + 0.5;

    vec2 newLA = vec2(0.0,-1.0);//normalize(lookAtVec.xy);

    float posDis2 = distance(vec2(0.0), TexCoord0.xy - 0.5);
    float posDis = posDis2*distance(vec2(0.0) + newLA*0.2, TexCoord0.xy - 0.5);
    
    if (posDis2 > 0.1) {
        posDis = 1.0;
    }
    
    

    vec4 tex1 = texture( Texture1, newTC.xy);
    vec4 tex2 = texture( Texture2, newTC.xy);


    

    float testHeight = getTerHeight(Texture3,Texture4,newTC,newTC,0.0).y;
    float seaHeight = seaLevel;        

    float isAboveWater = float(testHeight > seaHeight);
    vec4 landRes = texture( Texture0, vec3(
            (testHeight-seaHeight)*2.0/(1.0-seaHeight),
            0.0,
            TEX_MAPLAND + 0.5/255.0
        )
    );
    vec4 seaRes = texture( Texture0, vec3(
            testHeight/seaHeight,
            0.0,
            TEX_MAPWATER + 0.5/255.0
        )
    );
    vec4 topoRes = mix(seaRes,landRes,isAboveWater);

    vec4 tex0 = topoRes;
    tex0.a = 1.0;


    float offsetAmount = (1.0)/mapDimInPixels.x;

    vec4 tex1u = texture(Texture1, vec2(newTC.x, newTC.y + offsetAmount) );
    vec4 tex1d = texture(Texture1, vec2(newTC.x, newTC.y - offsetAmount) );
    vec4 tex1l = texture(Texture1, vec2(newTC.x - offsetAmount, newTC.y) );
    vec4 tex1r = texture(Texture1, vec2(newTC.x + offsetAmount, newTC.y) );

    vec4 tex2u = texture(Texture2, vec2(newTC.x, newTC.y + offsetAmount) );
    vec4 tex2d = texture(Texture2, vec2(newTC.x, newTC.y - offsetAmount) );
    vec4 tex2l = texture(Texture2, vec2(newTC.x - offsetAmount, newTC.y) );
    vec4 tex2r = texture(Texture2, vec2(newTC.x + offsetAmount, newTC.y) );

    /*
    vec4 tex1ul = texture(Texture1, vec2(newTC.x - offsetAmount, newTC.y + offsetAmount) );
    vec4 tex1ur = texture(Texture1, vec2(newTC.x + offsetAmount, newTC.y + offsetAmount) );
    vec4 tex1dl = texture(Texture1, vec2(newTC.x - offsetAmount, newTC.y - offsetAmount) );
    vec4 tex1dr = texture(Texture1, vec2(newTC.x + offsetAmount, newTC.y - offsetAmount) );
    */

    float mod1 = 1.0;
    if (
        tex1u.g != tex1.g ||
        tex1d.g != tex1.g ||
        tex1l.g != tex1.g ||
        tex1r.g != tex1.g /*||

        tex1ul.g < tex1.g ||
        tex1ur.g < tex1.g ||
        tex1dl.g < tex1.g ||
        tex1dr.g < tex1.g*/

    ) {
        mod1 = 1.0;
    }


    // float mod2 = 0.0;
    // if (
    //     tex2u.b != tex2.b ||
    //     tex2d.b != tex2.b ||
    //     tex2l.b != tex2.b ||
    //     tex2r.b != tex2.b

    // ) {
    //     mod2 = 0.6;
    // }

    

    //float isBridge = float( abs(testHeight - seaHeight) < 10.0/255.0 );

    vec2 gridVecBase = ( (newTC.xy*mapDimInPixels.xy) - floor(newTC.xy*mapDimInPixels.xy) );
    vec2 gridVec = abs( gridVecBase - 0.5)*2.0;
    float gridTest = float( (gridVec.x >= 0.9) || (gridVec.y >= 0.9) )*0.5;

    
    float gv3 = getGrid( int(tex2.b*255.0), gridVecBase, 0.4 );
    float gv2 = getGrid( int(tex2.r*255.0), gridVecBase, 0.2 ); //stchannel
    float gv1 = getGrid( int(tex2.g*255.0), gridVecBase, 0.1 )*0.5; //btchannel
    
    float gridMod = mix( max(gv1,gv2), 0.0, 1.0-clamp(cameraZoom*0.01,0.0,1.0) );

    vec3 resCol = tex0.rgb;//(tex0.rgb + gridMod)*mod1;

    // resCol = mix(
    //     resCol,
        
    //     mix(
    //         resCol+vec3(0.0,0.125,0.25),
    //         resCol+0.5,//vec3(1.0,1.0,1.0),//vec3(0.3,0.1,0.0),
    //         isAboveWater
    //     ),
        
    //     gv3
    // );

    // if (tex2.a > 0.0) {
    //     resCol.rgb = resCol.rgb*0.75 + (mod(tex2.a*255.0,16.0)/15.0)*0.25;
    // }

    // float othick = 0.01;
    // vec2 fracVal = vec2( fract(newTC.x),fract(newTC.y) );
    // if (
    //     ((fracVal.x >= -othick)&&(fracVal.x <= othick)) ||
    //     ((fracVal.y >= -othick)&&(fracVal.y <= othick))
    // ) {
    //     resCol += 0.2;
    // }

    

    //gl_FragData[0] = vec4(tex0.rgb,mapTrans);

    // + tex2.a * 8.0;
    
    bool mapMod = (posDis < 0.01)&&(posDis2>0.03);
    bool mapMod2 = (posDis < 0.01)&&(posDis2>0.025);
    
    
    //resCol = mix(vec3(0.0,0.0,1.0),vec3(0.0,1.0,0.0),isAboveWater);
    
    if (mapMod) {
        resCol.r += 1.0;
    }
    else {
        if (mapMod2) {
            resCol.rgb += 1.0;
        }
    }
    
    

    FragColor0 = vec4(
    
        
    
        resCol//vec3(testHeight);
        , 1.0 );// + float(tex1.a > 0.0)*0.2;// + tex2.a*0.5;// + float(tex1.r < seaHeight);//*mod;// + vec4( float(clamp(1.0-tex1.b,0.0,1.0) > 0.6) ,0.0,0.0,0.0);//tex0*mod;//1.0-tex1.bbbb;//tex0*mod;//tex0;//1.0-tex1.bbbb;////1.0-tex1.bbbb;//(tex0)*mod; // + colMod

}


