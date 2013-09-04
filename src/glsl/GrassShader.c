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

const float grassHeight = 1.0/140.0;
const float grassWidth = 2.0/1024.0;

$

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main() {

    TexCoord0 = gl_MultiTexCoord0.xyzw;
    //TexCoord1 = gl_MultiTexCoord1.xyz;
    
    float newZoom = max(cameraZoom,1.0);

    vec2 newTC = ((TexCoord0.xy)*newZoom+1.0)/2.0;

    vec4 myVert = gl_Vertex;
    vec4 tex0 = texture2D(Texture0, newTC );
    vec4 tex1 = texture2D(Texture1, newTC );
    tex1.rgb = (tex1.rgb-0.5)*2.0;


    float randModX = rand(vec2(gl_MultiTexCoord0.x*3333.0,gl_MultiTexCoord0.y*5443.0));
    float randModY = rand(vec2(gl_MultiTexCoord0.x*2334.0,gl_MultiTexCoord0.y*4432.0));
    //curTime


    normMod = vec3(1.0,0.0,0.0);

    normMod.x = (sin(gl_MultiTexCoord0.x*16.0 + 1.0/300.0 + randModX*322.0  + randModY*2.0));
    normMod.y = (randModX+1.0)*( (abs(  sin(  gl_MultiTexCoord0.x*32.0 + 1.0/800.0 + randModY*223.0 ) + randModX) + 1.0));
    normMod.z = 1.0;


    float isValid = float( tex0.b == 2.0/255.0 );

    float wind = sin( curTime/500.0 + gl_MultiTexCoord0.x*20.0 + gl_MultiTexCoord0.y*16.0 );
    float wind2 = sin( curTime/400.0 + gl_MultiTexCoord0.x*11.0 + gl_MultiTexCoord0.y*12.0 );

    myVert.x = gl_Vertex.x + ((gl_MultiTexCoord0.w)*grassWidth + (normMod.x+(wind+wind2)/3.0 )*gl_MultiTexCoord0.z*grassHeight*2.5)*isValid;
    myVert.y = gl_Vertex.y + ((gl_MultiTexCoord0.z)*grassHeight + gl_MultiTexCoord0.z*normMod.y*grassHeight)*isValid;
    myVert.z = gl_MultiTexCoord0.z;

    //myVert.xyz = mix(vec3(TexCoord0.xy*newZoom,0.0), myVert.xyz, float( tex0.b == 2.0/255.0 ) );

    /*
    normMod.x *= rand(normMod.xy*2333.0);
    normMod.y *= rand(normMod.xy*3245.0);
    normMod.z += rand(normMod.xy*5333.0);
    */

    normMod = normalize(normMod);


    myVert.xy *= newZoom;
    resVert = myVert;
    gl_Position = myVert;

    /*
    normMod.x = (sin(gl_MultiTexCoord0.x*16.0 + curTime/300.0 + randModX*322.0  + randModY*2.0)) * gl_MultiTexCoord0.z*grassHeight/2.0;
    normMod.y = (randModX+1.0)*( (abs(  sin(  gl_MultiTexCoord0.x*32.0 + 1.0/800.0 + randModY*223.0 ) + randModX) + 1.0) * 0.5 * gl_MultiTexCoord0.z*grassHeight);
    normMod.z = 1.0;

    //myVert.x += normMod.x;
    //myVert.y += normMod.y - 8.0/1024.0;

    //myVert.y += gl_MultiTexCoord0.z*grassHeight;

    normMod.x *= rand(normMod.xy*2333.0);
    normMod.y *= rand(normMod.xy*3245.0);
    normMod.z += rand(normMod.xy*5333.0);

    normMod = normalize(normMod);

    //myVert.x += 

    //myVert.xyz += tex1.xyz*gl_MultiTexCoord0.z*grassHeight;


    
    //clamp(tex1.z-0.7,0.0,1.0)/0.3
    //float( (tex1.z*2.0+randModX+randModY)/4.0 > 0.7)*


    myVert.x = gl_Vertex.x + (gl_MultiTexCoord0.z*normMod.x*grassHeight+gl_MultiTexCoord0.w)*grassWidth;
    myVert.y = gl_Vertex.y + gl_MultiTexCoord0.z*grassHeight;
    myVert.z = gl_MultiTexCoord0.z;

    myVert.xy *= newZoom;



    resVert = myVert;

    gl_Position = myVert;
    */

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

    float newZoom = max(cameraZoom,1.0);
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


    float baseHeight = unpack16(tex0.rg);
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
