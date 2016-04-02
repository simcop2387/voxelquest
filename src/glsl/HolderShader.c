#version 330

//uniform usamplerBuffer Texture0;

//uniform float voxelsPerCell;
//uniform int cellsPerHolder;
// uniform int CUBE_WRAP_INVALID;
// uniform int CUBE_WRAP_ENTRIES;
// uniform int CUBE_DATA_INVALID;
uniform float heightOfNearPlane;
uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
//uniform float curTime;


uniform mat4 pmMatrix;
//uniform mat4 modelviewInverse;
// uniform mat4 modelview;
// uniform mat4 proj;



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
	//vec4 worldPos2 = worldPos;
	//worldPos2.xyz += sin(curTime)*0.02;
	vdata0 = data0;
	//vdata1 = floor(data1+0.5);
	//vdata2 = floor(data2+0.5);
	vec4 screenPos = 
		pmMatrix*worldPos; //proj*modelview
		//worldPos;
	
	//gl_PointSize = (heightOfNearPlane / pow(screenPos.w,0.5))*0.02;
		
	gl_Position = screenPos;
	
}

$

in vec4 worldPos;
in vec4 vdata0;
//in vec4 vdata1;
//in vec4 vdata2;

layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;






// void getRay(in vec2 newTC, inout vec3 ro, inout vec3 rd) {
//  float aspect = bufferDim.y/bufferDim.x;
//  float NEAR = clipDist.x;
//  float FAR = clipDist.y;

//  float dx = tan(FOV*0.5f)*(newTC.x*2.0-1.0f)/aspect; //gl_FragCoord.x/(bufferDim.x*0.5)
//  float dy = tan(FOV*0.5f)*((1.0f-newTC.y)*2.0-1.0f); //gl_FragCoord.y/(bufferDim.y*0.5)

//  dx = -dx;

//  vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
//  vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);

//  p1 = modelviewInverse*p1;
//  p2 = modelviewInverse*p2;

//  ro = p1.xyz;
//  rd = normalize(p1.xyz-p2.xyz);
// }

// vec2 aabbIntersect(vec3 rayOrig, vec3 rayDir, vec3 minv, vec3 maxv) {
//     float t0;
//     float t1;

//     vec3 invR = 1.0 / rayDir;
//     vec3 tbot = invR * (minv-rayOrig);
//     vec3 ttop = invR * (maxv-rayOrig);
//     vec3 tmin = min(ttop, tbot);
//     vec3 tmax = max(ttop, tbot);
//     vec2 t = max(tmin.xx, tmin.yz);
//     t0 = max(t.x, t.y);
//     t = min(tmax.xx, tmax.yz);
//     t1 = min(t.x, t.y);
//     return vec2(t0,t1); // if (t0 <= t1) { did hit } else { did not hit }
// }



void main() {

	// vec2 TexCoord0 = gl_FragCoord.xy/(bufferDim.xy);

	// vec3 ro = vec3(0.0);
	// vec3 rd = vec3(0.0);
	
	// getRay(TexCoord0,ro,rd);


	// float voxelWidth = 0.5/voxelsPerCell;

	// vec2 boxRes = aabbIntersect(ro,rd,worldPos.xyz-voxelWidth, worldPos.xyz+voxelWidth);

	// if (boxRes.x > boxRes.y) {
	// 	discard;
	// }

	// FragColor0 = vec4(
	// 	//mod((worldPos.xyz+0.01)/32.0,1.0),
		
	// 	//globColor
		
	// 	//mod((worldPos.xyz+0.001)/float(cellsPerHolder),1.0)
		
	// 	// vec3(
	// 	// 	dot(
	// 	// 		vdata0.xyz,
	// 	// 		-lightVec
	// 	// 	)*pow(clamp(1.0-distance(worldPos.xyz,cameraPos.xyz)/50.0,0.0,1.0),0.5)
	// 	// )
		
	// 	worldPos.xyz
		
	// 	//  + vec3(
	// 	// 	mod(vdata0.w,255.0)/255.0,
	// 	// 	0.0,
	// 	// 	0.0
	// 	// )
	// 	//(vdata0.xyz + 1.0)*0.5
		
	// 	//vec3(globTotSteps/64.0) // + baseCol*0.25
		
		
	// 	// * vec3(
	// 	// 	1.0-distance(finalPos.xyz,cameraPos)/100.0	
	// 	// )
	// 	,
	// 	1.0
	// );
	
	
	FragColor0 = worldPos;
	FragColor1 = vdata0;

}









