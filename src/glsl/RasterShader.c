#version 330

uniform float heightOfNearPlane;
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
//out vec4 newCol;
out float camDis;

void main() {
	worldPos = vec4(vposition.xyz,1.0);
	
	//worldPos.z += sin((curTime*0.01+(worldPos.x+worldPos.y)*0.1))*5.0;
	
	//newCol = vtexcoord;
	vec4 screenPos = proj*modelview*worldPos;
	// gl_PointSize = (heightOfNearPlane / pow(screenPos.w,0.5))*4.0;
	camDis = distance(cameraPos.xyz,worldPos.xyz);
	gl_Position = screenPos; ///clipDist.y
}

$

in vec4 worldPos;
//in vec4 newCol;
in float camDis;

layout(location = 0) out vec4 FragColor0;

void main() {

	vec2 mv = (gl_PointCoord.xy - 0.5)*2.0;
	float c = 1.0 - length(mv);
	
	gl_FragDepth = camDis/clipDist.y + length(mv)*0.00075;
	
	if (c < 0.0) {
	    discard;
	}
	

	FragColor0 = vec4(
		//c*(1.0-gl_FragDepth), 0.0, 0.0,
		mod(worldPos.xyz/32.0,1.0),
		//vec3(1.0-clamp(camDis*8.0/clipDist.y,0.0,1.0)),
		//(sin(worldPos.xyz*0.005)+1.0)*0.5,
		//newCol.rgb,
		//camDis, 0.0, 0.0, 
		1.0
	);

}
