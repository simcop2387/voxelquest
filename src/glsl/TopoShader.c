#version 120

uniform sampler2D Texture0; // palette
uniform sampler2D Texture1; // heightmap
uniform sampler2D Texture2; // cityFBO;
//uniform sampler2D Texture2; // combineFBO0
//uniform sampler2D Texture3; // combineFBO1
varying vec2 TexCoord0;

uniform float curTime;
uniform float cameraZoom;
uniform float mapTrans;

uniform float seaLevel;
uniform vec3 cameraPos;
uniform vec2 bufferDim;
uniform vec2 mapDimInPixels;

uniform vec3 maxBoundsInPixels;

varying vec3 finalVec;


$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;
    
    vec4 pos = gl_Vertex;

    //pos.xy = pos.xy*0.4;
    //pos.x += 0.58;
    //pos.y -= 0.58;



    vec3 myVert = gl_Vertex.xyz;// - vec3(0.0,0.0,cameraPos.z);
    vec3 transVert;

    transVert.x = (myVert.x-myVert.y);
    transVert.y = (-(myVert.x/2.0) + -(myVert.y/2.0) + myVert.z);
    transVert.z = myVert.z;

    
    float newZoom = cameraZoom;//min(cameraZoom,1.0);
    
    finalVec.x = (transVert.x)*newZoom/(bufferDim.x);
    finalVec.y = (transVert.y)*newZoom/(bufferDim.y);
    finalVec.z = gl_Vertex.z;//(transVert.z);


    //float hv = mix(0.5,0.0,float(matVal == 4.0));
    //*0.5 + hv

    gl_Position = vec4(finalVec.xy, 0.5, gl_Vertex.w); //clamp( (1.0-finalVec.z/(256.0*255.0)) ,0.0,1.0)

}

$

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


void main() {

    float newTime = curTime/20000.0;

    vec2 newTC = TexCoord0.xy+(cameraPos.xy/maxBoundsInPixels.xy)+0.5;


    vec4 tex1 = texture2D( Texture1, newTC.xy);
    vec4 tex2 = texture2D( Texture2, newTC.xy);


    vec4 texHM0 =  texture2D(Texture1, (newTC.xy*1.0) );
    vec4 texHM1 =  texture2D(Texture1, (newTC.xy*4.0) );
    vec4 texHM2 =  texture2D(Texture1, (newTC.xy*16.0) );
    //vec4 texHM3 =  texture2D(Texture1, (newTC.xy*32.0) );

    float testHeight = (texHM0.r);//*0.7 +  texHM1.r*0.2 + texHM2.r*0.1);//(texHM0.r*0.6 + texHM1.r*0.2 + texHM2.r*0.07 + texHM3.r*0.03);
    //testHeight = pow(testHeight,2.0);

    float isAboveWater = float(testHeight > seaLevel);
    vec4 landRes = texture2D( Texture0, vec2( (testHeight-seaLevel)/(1.0-seaLevel), (5.0 + 0.5)/255.0 ) ); //vec4(testHeight);//
    vec4 seaRes = texture2D( Texture0, vec2(testHeight/seaLevel, (6.0 + 0.5)/255.0 ) );
    vec4 topoRes = mix(seaRes,landRes,isAboveWater);

    vec4 tex0 = topoRes;
    tex0.a = 1.0;


    float offsetAmount = (1.0)/mapDimInPixels.x;

    vec4 tex1u = texture2D(Texture1, vec2(newTC.x, newTC.y + offsetAmount) );
    vec4 tex1d = texture2D(Texture1, vec2(newTC.x, newTC.y - offsetAmount) );
    vec4 tex1l = texture2D(Texture1, vec2(newTC.x - offsetAmount, newTC.y) );
    vec4 tex1r = texture2D(Texture1, vec2(newTC.x + offsetAmount, newTC.y) );

    vec4 tex2u = texture2D(Texture2, vec2(newTC.x, newTC.y + offsetAmount) );
    vec4 tex2d = texture2D(Texture2, vec2(newTC.x, newTC.y - offsetAmount) );
    vec4 tex2l = texture2D(Texture2, vec2(newTC.x - offsetAmount, newTC.y) );
    vec4 tex2r = texture2D(Texture2, vec2(newTC.x + offsetAmount, newTC.y) );

    /*
    vec4 tex1ul = texture2D(Texture1, vec2(newTC.x - offsetAmount, newTC.y + offsetAmount) );
    vec4 tex1ur = texture2D(Texture1, vec2(newTC.x + offsetAmount, newTC.y + offsetAmount) );
    vec4 tex1dl = texture2D(Texture1, vec2(newTC.x - offsetAmount, newTC.y - offsetAmount) );
    vec4 tex1dr = texture2D(Texture1, vec2(newTC.x + offsetAmount, newTC.y - offsetAmount) );
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

    

    //float isBridge = float( abs(testHeight - seaLevel) < 10.0/255.0 );

    vec2 gridVecBase = ( (newTC.xy*mapDimInPixels.xy) - floor(newTC.xy*mapDimInPixels.xy) );
    vec2 gridVec = abs( gridVecBase - 0.5)*2.0;
    float gridTest = float( (gridVec.x >= 0.9) || (gridVec.y >= 0.9) )*0.5;

    
    float gv3 = getGrid( int(tex2.b*255.0), gridVecBase, 0.4 );
    float gv2 = getGrid( int(tex2.r*255.0), gridVecBase, 0.2 ); //stchannel
    float gv1 = getGrid( int(tex2.g*255.0), gridVecBase, 0.1 )*0.5; //btchannel
    
    float gridMod = mix( 0.0, max(gv1,gv2), clamp(cameraZoom/0.01,0.0,1.0) );

    vec3 resCol = (tex0.rgb + gridMod)*mod1;

    // mix(
    //     vec3(testHeight/5.0,testHeight/2.0,testHeight),
    //     (tex0.rgb + gridMod)*mod,
    //     isAboveWater
    // );
    resCol = mix(
        resCol,
        
        mix(
            resCol+vec3(0.0,0.125,0.25),
            resCol+0.5,//vec3(1.0,1.0,1.0),//vec3(0.3,0.1,0.0),
            isAboveWater
        ),
        
        gv3
    );

    if (tex2.a > 0.0) {
        resCol.rgb = resCol.rgb*0.75 + (mod(tex2.a*255.0,16.0)/15.0)*0.25;
    }

    

    //gl_FragData[0] = vec4(tex0.rgb,mapTrans);

    // + tex2.a * 8.0;

    gl_FragData[0] = vec4( resCol, mapTrans );// + float(tex1.a > 0.0)*0.2;// + tex2.a*0.5;// + float(tex1.r < seaLevel);//*mod;// + vec4( float(clamp(1.0-tex1.b,0.0,1.0) > 0.6) ,0.0,0.0,0.0);//tex0*mod;//1.0-tex1.bbbb;//tex0*mod;//tex0;//1.0-tex1.bbbb;////1.0-tex1.bbbb;//(tex0)*mod; // + colMod

}


