#version 330

// // solidTargFBO
// uniform sampler2D Texture0;

// // rasterSourceFBO
// uniform sampler2D Texture1;


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

void main() {
	
	worldPos = vec4(vposition.xyz,1.0);
	vec4 screenPos = proj*modelview*worldPos;
	gl_Position = screenPos;
	
}

$

in vec4 worldPos;

layout(location = 0) out vec4 FragColor0;

void main() {

	FragColor0 = vec4(
		(sin(worldPos.xyz/32.0)+1.0)*0.5,
		1.0
	);

}
