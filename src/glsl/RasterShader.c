#version 330

//uniform usamplerBuffer Texture0;

uniform float dimInVoxels;
uniform int renderLevel;
uniform int maxSize;
uniform int rootPtr;
uniform int nodeSize;
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
out float camDis;

void main() {
	worldPos = vec4(vposition.xyz,1.0);
	vec4 screenPos = proj*modelview*worldPos;
	gl_PointSize = (heightOfNearPlane / screenPos.w)*2.0;
	camDis = distance(cameraPos.xyz,worldPos.xyz);
	gl_Position = screenPos; ///clipDist.y
}

$

in vec4 worldPos;
in float camDis;

layout(location = 0) out vec4 FragColor0;

void main() {

	vec2 mv = (gl_PointCoord.xy - 0.5)*2.0;
	float c = 1.0 - length(mv);
	
	gl_FragDepth = camDis/clipDist.y + length(mv)*0.001;
	
	if (c < 0.0) {
	    discard;
	}
	    
	//
	

	FragColor0 = vec4(
		c, 0.0, 0.0,
		//mod(worldPos.xyz/100.0,1.0),
		1.0
	);

}
