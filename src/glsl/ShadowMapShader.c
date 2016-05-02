#version 330
// #extension GL_EXT_frag_depth : enable

uniform vec2 clipDist;
uniform vec2 bufferDim;
//uniform vec3 cameraPos;
uniform vec3 lightPos;

// uniform vec3 minBounds;
// uniform vec3 maxBounds;
// uniform vec3 lightVec;
uniform mat4 lightSpaceMatrix;


$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 data0;

out vec4 worldPos;
//out float fragDep;

// vec4 worldToScreen(in vec4 wp, inout float fd) {
// 	vec4 screenPos = wp;
	
// 	screenPos.xyz -= minBounds;
// 	screenPos.xyz /= (maxBounds-minBounds);
	
// 	fd = 1.0-screenPos.z;
	
// 	screenPos.xy += lightVec.xy*(fd);
	
// 	screenPos.xy -= 0.5;
// 	screenPos.xy *= 2.0;
	
// 	screenPos.z = 1.0;//length(screenPos.xy)/100.0;
// 	screenPos.w = 1.0;//screenPos.z;
// 	//screenPos.w = vec4(pmMatrix*vec4(vposition.xyz,1.0)).w;
	
// 	return screenPos;
// }

void main() {
	worldPos = vec4(vposition.xyz,1.0);
	//fragDep = 1.0;
	//gl_Position = worldToScreen(worldPos,fragDep);
	
	gl_PointSize = 5.0;
	
	vec4 screenPos = lightSpaceMatrix*worldPos;
	
	screenPos.z /= 2.0;
	
	gl_Position = screenPos;
	
}

$

in vec4 worldPos;
//in float fragDep;

layout(location = 0) out vec4 FragColor0;

void main() {

	//gl_FragDepthEXT = fragDep;

	FragColor0 = vec4(worldPos.xyz,distance(worldPos.xyz,lightPos.xyz)); //, distance(worldPos.xyz,lightPos.xyz)
		

}









