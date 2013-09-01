////////////////  RenderVolume  ////////////////  

//uniform sampler3D Texture0; //volume texture
uniform sampler2D Texture0;
uniform sampler2D Texture1;

//uniform sampler2D Texture0;
//uniform sampler3D Texture1;
varying vec3 TexCoord0;
//varying vec3 TexCoord1;

//uniform float curHeight;
//uniform vec3 worldMin;
//uniform vec3 worldMax;

uniform float curTime;
uniform float cameraZoom;

$

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {

    TexCoord0 = gl_MultiTexCoord0.xyz;
    //TexCoord1 = gl_MultiTexCoord1.xyz;
    
    vec4 myVert = gl_Vertex;
    vec4 tex0 = texture2D(Texture1, TexCoord0.xy);
    tex0.rgb = (tex0.rgb-0.5)*2.0;

    //curTime
    myVert.x += sin(1.0/500.0 + rand(vec2(gl_MultiTexCoord0.x*3333.0,gl_MultiTexCoord0.y*5443.0))*322.0 )*gl_MultiTexCoord0.z/128.0;
    myVert.y += abs(sin(1.0/500.0 + rand(vec2(gl_MultiTexCoord0.x*2334.0,gl_MultiTexCoord0.y*4432.0))*223.0 )*gl_MultiTexCoord0.z*cameraZoom/128.0);

    

    

    //myVert.x += 

    myVert.xyz += tex0.xyz*gl_MultiTexCoord0.z*cameraZoom/32.0;

    if (tex0.z <= 0.7) {
    	myVert.x = 0.0;
    	myVert.y = 0.0;
    	myVert.z = 0.0;
    }

    //myVert.xy *= cameraZoom;

    gl_Position = myVert;

}

$

void main() {

	vec4 dark = vec4(23.0/255.0, 54.0/255.0, 0.0/255.0, 1.0);
	vec4 light = vec4(145.0/255.0, 192.0/255.0, 62.0/255.0, 1.0);
	//23:54:0
	//145:192:62

	//vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
	gl_FragData[0] = mix(dark,light,TexCoord0.z);

	//tex0;//vec4(TexCoord0.z,0.0,0.0,1.0);
	//gl_FragData[1] = normAO;

}
