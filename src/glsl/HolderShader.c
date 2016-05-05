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
//layout(location = 2) in vec4 data1;

flat out vec4 worldPos;
flat out vec4 vdata0;
//flat out vec4 vdata1;

void main() {
	
	worldPos = vec4(vposition.xyz,1.0);
	vdata0 = data0;
	//vdata1 = data1;
	
	// gl_PointSize = 1.0;
	
	vec4 screenPos = pmMatrix*worldPos;
	worldPos.w = vposition.w;
	gl_Position = screenPos;
	
}

$

flat in vec4 worldPos;
flat in vec4 vdata0;
//flat in vec4 vdata1;

layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;
layout(location = 2) out vec4 FragColor2;

void main() {

	FragColor0 = worldPos;//vec4(worldPos.xyz,vdata0.w);
	FragColor1 = vdata0;
	FragColor2 = vec4(0.0);//vdata1;

}









