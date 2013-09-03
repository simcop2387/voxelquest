//uniform sampler3D Texture0; //volume texture
//uniform sampler2D Texture0;
//uniform sampler2D Texture1;

//uniform sampler2D Texture0;
//uniform sampler3D Texture1;
//varying vec4 TexCoord0;
//varying vec3 TexCoord1;

//uniform float curHeight;
//uniform vec3 worldMin;
//uniform vec3 worldMax;

uniform float curTime;
uniform float cameraZoom;
uniform vec3 cameraPos;
uniform float bufferWidth;

varying vec3 finalVec;

$

void main() {

    //TexCoord0 = gl_MultiTexCoord0.xyzw;
    //TexCoord1 = gl_MultiTexCoord1.xyz;

    //vec2 newTC = ((TexCoord0.xy)*newZoom+1.0)/2.0;



    vec3 myVert = gl_Vertex.xyz - cameraPos.xyz;
    vec3 transVert;

    transVert.x = (myVert.x-myVert.y);
    transVert.y = (-(myVert.x/2.0) + -(myVert.y/2.0) + myVert.z);
    transVert.z = myVert.z;

    
    
    finalVec.x = (transVert.x)*2.0*cameraZoom/(bufferWidth);
    finalVec.y = (transVert.y)*2.0*cameraZoom/(bufferWidth);
    finalVec.z = gl_Vertex.z;//(transVert.z);



    gl_Position = vec4(finalVec.xy, clamp( 1.0-finalVec.z/(256.0*255.0),0.0,1.0),gl_Vertex.w);

}

$

void main() {
    
    float bhr = mod(finalVec.z,256.0);
    float bhg = finalVec.z/(256.0);

    gl_FragData[0] = vec4(bhr/255.0,bhg/255.0,4.0/255.0,1.0);//vec4(bhr,bhg,3.0/255.0,tex0.a);
    gl_FragData[1] = vec4(0.0,0.0,1.0,1.0);//vec4(resNorm.rgb, (TexCoord0.z+tex1.a)/2.0 );

}
