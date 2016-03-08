#version 330

// solidTargFBO
uniform sampler2D Texture0;

// rasterSourceFBO
uniform sampler2D Texture1;


uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;


uniform mat4 modelviewInverse;
uniform mat4 modelview;
uniform mat4 proj;



$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vtexcoord;

out vec4 worldPos;
out vec2 tc;
//out vec4 newCol;
//out float camDis;

void main() {
	
	tc = vtexcoord.xy;
	vec3 newPos = texture(Texture0, tc).xyz;
	worldPos = vec4(newPos,1.0); //vposition.xyz newPos.xyz
	vec4 screenPos = proj*modelview*worldPos;
	gl_Position = screenPos;
	
	// worldPos = vtexcoord;
	// gl_Position = vposition;
	
}

$

in vec4 worldPos;
in vec2 tc;
//in vec4 newCol;
//in float camDis;

layout(location = 0) out vec4 FragColor0;

void main() {

	float camDis = distance(cameraPos.xyz,worldPos.xyz);

	FragColor0 = vec4(
		//c*(1.0-gl_FragDepth), 0.0, 0.0,
		//mod(worldPos.xyz/100.0,1.0),
		//vec3(1.0-clamp(camDis*8.0/clipDist.y,0.0,1.0)),
		//(sin(worldPos.xyz*0.005)+1.0)*0.5,
		//newCol.rgb,
		texture(Texture1, tc).rgb,//Texture1//camDis, 0.0, 0.0, 
		1.0
	);

}
