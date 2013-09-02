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

varying vec4 resVert;
varying vec3 normMod;

const float grassHeight = 1.0/50.0;

$

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {

    TexCoord0 = gl_MultiTexCoord0.xyzw;
    //TexCoord1 = gl_MultiTexCoord1.xyz;
    
    float newZoom = max(cameraZoom,1.0);

    vec2 newTC = ((TexCoord0.xy+4.0/1024.0)*newZoom+1.0)/2.0;

    vec4 myVert = gl_Vertex;
    vec4 tex0 = texture2D(Texture0, newTC );
    vec4 tex1 = texture2D(Texture1, newTC );
    tex1.rgb = (tex1.rgb-0.5)*2.0;


    float randModX = rand(vec2(gl_MultiTexCoord0.x*3333.0,gl_MultiTexCoord0.y*5443.0));
    float randModY = rand(vec2(gl_MultiTexCoord0.x*2334.0,gl_MultiTexCoord0.y*4432.0));
    //curTime


    normMod.x = (sin(gl_MultiTexCoord0.x*16.0 + curTime/300.0 + randModX*322.0 ) + randModY*2.0) * gl_MultiTexCoord0.z*grassHeight/2.0;
    normMod.y = ( abs(sin(  gl_MultiTexCoord0.x*32.0 + curTime/800.0 + randModY*223.0 ) + randModX + 1.0) * gl_MultiTexCoord0.z*grassHeight);
    normMod.z = 1.0;

    myVert.x += normMod.x;
    myVert.y += normMod.y;

    normMod.x *= rand(normMod.xy*2333.0);
    normMod.y *= rand(normMod.xy*3245.0);
    normMod.z += rand(normMod.xy*5333.0);

    //normMod.x = TexCoord0.w;

    normMod = normalize(normMod);

    //myVert.x += 

    //myVert.xyz += tex1.xyz*gl_MultiTexCoord0.z*grassHeight;


    myVert.xyz = mix(vec3(TexCoord0.xy*newZoom,0.0), myVert.xyz, float( tex0.b == 2.0/255.0 ) );
    //clamp(tex1.z-0.7,0.0,1.0)/0.3
    //float( (tex1.z*2.0+randModX+randModY)/4.0 > 0.7)*

    myVert.xy *= newZoom;


    resVert = myVert;

    gl_Position = myVert;

}

$

void main() {

    float newZoom = max(cameraZoom,1.0);
    vec2 newTC = (TexCoord0.xy*newZoom+1.0)/2.0;
    //vec4 tex2 = texture2D(Texture2, newTC );
    vec4 tex0 = texture2D(Texture0, newTC );
    vec4 tex1 = texture2D(Texture1, newTC );


    float tot = tex0.r + tex0.g + tex0.b + tex0.a;

    if (tot == 0.0) {
        discard;
    }



    //float baseHeight = tex0.r + tex0.g*255.0 + TexCoord0.z*grassHeight;


    float bhr = tex0.r + TexCoord0.z*grassHeight;//mod(baseHeight*256.0,256.0)/255.0;
    float bhg = tex0.g + float(bhr > 1.0)/255.0;//baseHeight/256.0;

    bhr = mod(bhr*256.0,256.0)/255.0;



    vec3 curNorm = (tex1.rgb-0.5)*2.0;

    vec3 curNorm2 = normMod;
    curNorm2.z = TexCoord0.z;
    curNorm2 = normalize(curNorm2);


    vec3 resNorm = mix(curNorm2,curNorm,TexCoord0.z);
    resNorm = normalize(resNorm);
    resNorm = (resNorm + 1.0)/2.0;

    /////////////////////


    

    gl_FragData[0] = vec4(bhr,bhg,3.0/255.0,tex0.a);
    gl_FragData[1] = vec4(resNorm.rgb, (TexCoord0.z+tex1.a)/2.0 );




    /////////////////////






    /*
	vec4 dark = vec4(10.0/255.0, 25.0/255.0, 0.0/255.0, 1.0);
	vec4 light = vec4(145.0/255.0, 192.0/255.0, 62.0/255.0, 1.0);
	//23:54:0
	//145:192:62


    float hfog = clamp( ( (baseHeight+TexCoord0.z*grassHeight)*256.0-cameraPos.z)/1024.0,0.0,1.0);
    hfog = pow(hfog, 2.0);

    float lightRes = 1.0;//(TexCoord0.z + tex2.a)/2.0;
    lightRes *= hfog;
    vec4 fogColor = vec4(0.6, 0.6, 0.7, 1.0);


	//vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
	gl_FragData[0] = mix(fogColor, mix(dark,light, lightRes), hfog);

	//tex0;//vec4(TexCoord0.z,0.0,0.0,1.0);
	//gl_FragData[1] = normAO;
    */

}
