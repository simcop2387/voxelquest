//uniform sampler3D Texture0; //volume texture
uniform sampler2D Texture0;
uniform sampler2D Texture1;

//uniform sampler2D Texture0;
//uniform sampler3D Texture1;
varying vec4 TexCoord0;
//varying vec3 TexCoord1;

//uniform float curHeight;
//uniform vec3 worldMin;
//uniform vec3 worldMax;

uniform float curTime;
uniform float cameraZoom;
uniform vec3 cameraPos;
uniform float bufferWidth;

varying vec4 resVert;
varying vec3 normMod;
varying float baseHeight;


const float grassHeight = 1.0/64.0;
const float grassWidth = 2.0/1024.0;


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





$

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


vec3 worldFromTC(vec2 tc, float bh, float zoom ) {

    //float newZoom = min(cameraZoom,1.0);

    vec3 wp = vec3(0.0,0.0,0.0);
    vec2 tcMod = (vec2(tc.x,1.0-tc.y)*2.0-1.0 )*bufferWidth/(zoom);
    tcMod.y -= cameraPos.z;
    wp.x = tcMod.y + tcMod.x/2.0 + (bh);
    wp.y = tcMod.y - tcMod.x/2.0 + (bh);
    wp.z = bh;
    wp.x += cameraPos.x;
    wp.y += cameraPos.y;

    return wp;
}


void main() {

    TexCoord0 = gl_MultiTexCoord0.xyzw;
    //TexCoord1 = gl_MultiTexCoord1.xyz;
    
    //float newZoom = max(cameraZoom,1.0);
    float newZoom = mix( max(cameraZoom,1.0), min(cameraZoom,1.0), float(cameraZoom >= 1.0) );

    vec2 newTC = ((TexCoord0.xy)*newZoom+1.0)/2.0;

    vec4 myVert = gl_Vertex;
    vec4 tex0 = texture2D(Texture0, newTC );
    vec4 tex1 = texture2D(Texture1, newTC );
    tex1.rgb = (tex1.rgb-0.5)*2.0;

    baseHeight = unpack16(tex0.rg);

    vec3 worldPos = TexCoord0.xyz;//worldFromTC(TexCoord0.xy,baseHeight,newZoom)/1000.0;


    float randModX = rand(vec2(worldPos.x,worldPos.y));
    float randModY = rand(vec2(worldPos.x,worldPos.y));
    //curTime


    normMod = vec3(1.0,0.0,0.0);

    normMod.x = (sin(worldPos.x*16.0 + 1.0/300.0 + randModX*322.0  + randModY*2.0));
    normMod.y = (randModX+1.0)*( (abs(  sin(  worldPos.x*32.0 + 1.0/800.0 + randModY*223.0 ) + randModX) + 1.0));
    normMod.z = 1.0;
    normMod = normalize(normMod);

    float isValid = float( tex0.b == 2.0/255.0 );


    
    

    float wind = (sin( curTime/700.0 + worldPos.x*8.0 + worldPos.y*6.0 ));

    vec2 windVec = sqrt( abs( vec2(wind,0.0) ) );

    vec2 grassVec = normMod.xy;
    grassVec.x *= -1.0;

    float windAmount = abs(dot(grassVec,windVec));//clamp(dot(grassVec,windVec),0.0,1.0);

    vec2 resVec = normalize(mix(grassVec,windVec,windAmount));



    myVert.x = gl_Vertex.x + ((gl_MultiTexCoord0.w)*grassWidth  + (resVec.x*2.0 - 0.25)*gl_MultiTexCoord0.z*grassHeight*2.0)*isValid;
    myVert.y = gl_Vertex.y + ((gl_MultiTexCoord0.z)*grassHeight + gl_MultiTexCoord0.z*grassHeight)*isValid;
    myVert.z = gl_MultiTexCoord0.z;


    //normMod.x += grassVec.x;
    //normMod.y += 1.0-grassVec.y;
    //normMod.z += grassVec.y;
    normMod = normalize(normMod);


    myVert.xy *= newZoom;
    resVert = myVert;
    gl_Position = myVert;

}

$



void main() {

    //float newZoom = max(cameraZoom,1.0);
    float newZoom = mix( max(cameraZoom,1.0), min(cameraZoom,1.0), float(cameraZoom >= 1.0) );


    vec2 newTC = (TexCoord0.xy*newZoom+1.0)/2.0;
    //vec4 tex2 = texture2D(Texture2, newTC );
    vec4 tex0 = texture2D(Texture0, newTC );
    vec4 tex1 = texture2D(Texture1, newTC );


    float tot = tex0.r + tex0.g + tex0.b + tex0.a;

    if (tot == 0.0 || tex1.a == 0.0) {
        discard;
    }



    //float baseHeight = tex0.r + tex0.g*255.0 + TexCoord0.z*grassHeight;
    //float bhr = tex0.r + TexCoord0.z*grassHeight;//mod(baseHeight*256.0,256.0)/255.0;
    //float bhg = tex0.g + float(bhr > 1.0)/255.0;//baseHeight/256.0;
    //bhr = mod(bhr*256.0,256.0)/255.0;


    
    float newHeight = baseHeight + TexCoord0.z*grassHeight*bufferWidth;

   

    vec3 curNorm = (tex1.rgb-0.5)*2.0;

    //vec3 curNorm2 = normMod;
    //curNorm2.z = TexCoord0.z;
    //curNorm2 = normalize(curNorm2);

    vec3 upVec = vec3(0.0,0.0,1.0);

    vec3 resNorm = mix(curNorm,normMod,0.5);//mix( , upVec, TexCoord0.z); //
    resNorm = normalize(resNorm);
    resNorm = (resNorm + 1.0)/2.0;


    vec2 packed = pack16(newHeight);

    gl_FragData[0] = vec4(packed.rg,3.0/255.0,tex0.a);
    gl_FragData[1] = vec4(resNorm.rgb, (TexCoord0.z+tex1.a)/2.0 );

}
