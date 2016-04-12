#version 330

uniform float heightOfNearPlane;
uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
//uniform vec3 cameraPos;

uniform mat4 pmMatrix;

$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 data0;

out vec4 worldPos;
out vec4 vdata0;

void main() {
	
	worldPos = vec4(vposition.xyz,1.0);
	vdata0 = data0;
	vdata0.w = floor(vdata0.w+1.0); // *65281.0 + 0.1)/65280.0;
	vec4 screenPos = pmMatrix*worldPos;
	worldPos.w = vposition.w;
	gl_Position = screenPos;
	
}

$

in vec4 worldPos;
in vec4 vdata0;

layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;

void main() {

	FragColor0 = worldPos;//vec4(worldPos.xyz,vdata0.w);
	FragColor1 = vdata0;

}









