#version 330

//uniform usamplerBuffer Texture0;

uniform float voxelsPerCell;
uniform int cellsPerHolder;
uniform int CUBE_WRAP_INVALID;
uniform int CUBE_WRAP_ENTRIES;
uniform int CUBE_DATA_INVALID;
uniform float heightOfNearPlane;
uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform vec3 lightVec;


uniform mat4 modelviewInverse;
uniform mat4 modelview;
uniform mat4 proj;



$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 data0;
//layout(location = 2) in vec4 data1;
//layout(location = 3) in vec4 data2;

out vec4 worldPos;
out vec4 vdata0; // x: index / 4096, y: remaining index
//out vec4 vdata1; // boundsMin
//out vec4 vdata2; // boundsMax

void main() {
	
	worldPos = vec4(vposition.xyz,1.0);
	vdata0 = data0;
	//vdata1 = floor(data1+0.5);
	//vdata2 = floor(data2+0.5);
	vec4 screenPos = 
		proj*modelview*worldPos;
		//worldPos;
	
	//gl_PointSize = (heightOfNearPlane / pow(screenPos.w,0.5))*0.2;
		
	gl_Position = screenPos;
	
}

$

in vec4 worldPos;
in vec4 vdata0;
//in vec4 vdata1;
//in vec4 vdata2;

layout(location = 0) out vec4 FragColor0;

void main() {

	// globColor = vec3(0.0);
	// globTotSteps = 0.0;

	// vec4 oneVec = vec4(1.0);

	// vec2 baseCoords = gl_FragCoord.xy/(bufferDim.xy);
	// vec3 ro = cameraPos.xyz;//vec3(0.0);
	// vec3 rd = normalize(worldPos.xyz-cameraPos.xyz);//vec3(0.0);


	FragColor0 = vec4(
		//mod((worldPos.xyz+0.01)/32.0,1.0),
		
		//globColor
		
		//mod((worldPos.xyz+0.001)/float(cellsPerHolder),1.0)
		
		vec3(
			dot(
				vdata0.xyz,
				-lightVec
			)	
		)
		//(vdata0.xyz + 1.0)*0.5
		
		//vec3(globTotSteps/64.0) // + baseCol*0.25
		
		
		// * vec3(
		// 	1.0-distance(finalPos.xyz,cameraPos)/100.0	
		// )
		,
		1.0
	);

}









