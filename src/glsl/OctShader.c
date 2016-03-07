#version 330

uniform usamplerBuffer Texture0;

uniform float dimInVoxels;
uniform int renderLevel;
uniform int vDataSize;
uniform int rootPtr;
uniform int nodeSize;

uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform mat4 modelviewInverse;



$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vtexcoord;

out vec4 worldPos;

void main() {
    worldPos = vtexcoord;
    gl_Position = vposition;
}

$

// qqqq

const int MAX_STEPS = 128;
const int MAX_DEPTH = 16;

int octStack[MAX_DEPTH];
int kStack[MAX_DEPTH];
vec3 xyzStack[MAX_DEPTH];

const vec3 nodeOffsets[8] = vec3[](
		vec3(0.0,0.0,0.0),
		vec3(1.0,0.0,0.0),
		vec3(0.0,1.0,0.0),
		vec3(1.0,1.0,0.0),
		
		vec3(0.0,0.0,1.0),
		vec3(1.0,0.0,1.0),
		vec3(0.0,1.0,1.0),
		vec3(1.0,1.0,1.0)
);

in vec4 worldPos;

layout(location = 0) out vec4 FragColor0;

vec2 aabbIntersect(vec3 rayOrig, vec3 rayDir, vec3 minv, vec3 maxv) {
	float t0;
	float t1;

	vec3 invR = 1.0 / rayDir;
	vec3 tbot = invR * (minv-rayOrig);
	vec3 ttop = invR * (maxv-rayOrig);
	vec3 tmin = min(ttop, tbot);
	vec3 tmax = max(ttop, tbot);
	vec2 t = max(tmin.xx, tmin.yz);
	t0 = max(t.x, t.y);
	t = min(tmax.xx, tmax.yz);
	t1 = min(t.x, t.y);
	return vec2(t0,t1); // if (t0 <= t1) { did hit } else { did not hit }
}

void getRay(in vec2 newTC, inout vec3 ro, inout vec3 rd) {
	float aspect = bufferDim.y/bufferDim.x;
	float NEAR = clipDist.x;
	float FAR = clipDist.y;

	float dx = tan(FOV*0.5f)*(newTC.x*2.0-1.0f)/aspect; //gl_FragCoord.x/(bufferDim.x*0.5)
	float dy = tan(FOV*0.5f)*((1.0f-newTC.y)*2.0-1.0f); //gl_FragCoord.y/(bufferDim.y*0.5)

	dx = -dx;

	vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
	vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);

	p1 = modelviewInverse*p1;
	p2 = modelviewInverse*p2;

	ro = p1.xyz;
	rd = normalize(p1.xyz-p2.xyz);
}

void swap(inout int v1, inout int v2) {
	int temp = v1;
	v1 = v2;
	v2 = temp;
}

void main() {

	vec2 baseCoords = gl_FragCoord.xy/(bufferDim.xy);
	vec3 ro = vec3(0.0);
	vec3 rd = vec3(0.0);

	getRay(baseCoords, ro, rd);

	
	int indexArr[8];
	
	


	int i;
	int j;
	int k;
	int curPtr;
	int stackInd;
	int curDepth = 0;
	
	for (i = 0; i < 8; i++) {
		indexArr[i] = i;
	}
	
	
	if (rd.z < 0.0) {
		swap(indexArr[0], indexArr[4]);
		swap(indexArr[1], indexArr[5]);
		swap(indexArr[2], indexArr[6]);
		swap(indexArr[3], indexArr[7]);
	}
	
	if (rd.y < 0.0) {
		swap(indexArr[0], indexArr[2]);
		swap(indexArr[1], indexArr[3]);
		swap(indexArr[4], indexArr[6]);
		swap(indexArr[5], indexArr[7]);
	}
	
	if (rd.x < 0.0) {
		swap(indexArr[0], indexArr[1]);
		swap(indexArr[2], indexArr[3]);
		swap(indexArr[4], indexArr[5]);
		swap(indexArr[6], indexArr[7]);
	}
	
	
	
	
	
	
	vec3 curDiv = vec3(dimInVoxels);
	vec3 curDiv2 = curDiv/2.0;
	vec3 baseXYZ = vec3(0.0);
	vec3 testXYZ = vec3(0.0);
	vec3 testXYZ2 = vec3(0.0);

	for (i = 0; i < MAX_DEPTH; i++) {
		octStack[i] = 0;
		kStack[i] = 0;
		xyzStack[i] = vec3(0.0);
	}

	stackInd = 0;
	curPtr = rootPtr/4;
	octStack[0] = curPtr;
	xyzStack[0] = baseXYZ;
	kStack[0] = 0;
	
	
	
	
	ivec4 readVal0;
	ivec4 readVal1;
	
	int childPtrs[8];
	int curChild;
	int minK;
	int maxK;
	vec3 curNodeOffset;
	
	

	vec2 hitBox = aabbIntersect(ro,rd,baseXYZ,baseXYZ+curDiv);
	vec2 curHitBox = vec2(0.0);
	
	bool didFail = false;
	bool myTest = false;
	bool doBreak = false;
	
	ivec4 oneVec = ivec4(0);

	int disRL;
	float maxDepth = renderLevel;
	float minDepth = 2.0;


	if (hitBox.x < hitBox.y) {
		
		for (i = 0; i < MAX_STEPS; i++) {

			if (stackInd >= MAX_DEPTH) {
				didFail = true;
				break;
			}

			minK = kStack[stackInd];
			curPtr = octStack[stackInd];
			baseXYZ = xyzStack[stackInd];

			readVal0 = ivec4(texelFetch(Texture0, curPtr))/4;
			readVal1 = ivec4(texelFetch(Texture0, curPtr+1))/4;


			childPtrs[0] = readVal0.x;
			childPtrs[1] = readVal0.y;
			childPtrs[2] = readVal0.z;
			childPtrs[3] = readVal0.w;
			
			childPtrs[4] = readVal1.x;
			childPtrs[5] = readVal1.y;
			childPtrs[6] = readVal1.z;
			childPtrs[7] = readVal1.w;
			

			for (k = minK; k < 8; k++) {
				
				
				
				j = indexArr[k];
				
				curChild = childPtrs[j];
				
				
				if (curChild != 0) {
					
					
					curNodeOffset = nodeOffsets[j];
					
					testXYZ = baseXYZ + curNodeOffset*curDiv2;
					testXYZ2 = testXYZ+curDiv2;
					
					
					curHitBox = aabbIntersect(ro,rd,testXYZ,testXYZ2);
					
					// if (all(lessThanEqual(ro, testXYZ2))&&all(greaterThanEqual(ro, testXYZ))) {
					// 	curHitBox.x = 0.0;
					// 	curHitBox.y = 1.0;
					// }
					
					
					
					if (curHitBox.x < curHitBox.y) {
						disRL = int(
							
							mix(
								maxDepth,
								minDepth,
								clamp(curHitBox.x*2.0/clipDist.y,0.0,1.0)
							)
						);
						
						if (stackInd >= disRL) {
							doBreak = true;
						}
						else {
							kStack[stackInd] = k+1;
							stackInd++;
							
							kStack[stackInd] = 0;
							xyzStack[stackInd] = testXYZ;
							octStack[stackInd] = curChild;
							
							curDiv /= 2.0;
							curDiv2 /= 2.0;
						}
						break;
						
					}
					
				}
				
			}
			
			
			
			if (doBreak) {
				break;
			}
			
			if (k == 8) {
				stackInd--;
				curDiv *= 2.0;
				curDiv2 *= 2.0;
				
				if (stackInd <= 0) {
					break;
				}
			}

		}
		
	}
	else  {
		didFail = true;
	}

	
	vec3 pos = ro + rd*curHitBox.x;

	//texelRes1 = texelFetch(Texture0, primDataInd);

	if (didFail) {
		FragColor0 = vec4(1.0,0.0,0.0,1.0);
	}
	else {
		
		//if (doBreak) {
			FragColor0 = vec4(vec3(
				float(i)/float(MAX_STEPS)
				//mod(pos/256.0,1.0)
			),1.0);//vec4((rd+1.0)*0.5,1.0);
		// }
		// else {
		// 	FragColor0 = vec4(0.0,1.0,0.0,1.0);
		// }
		
		
		
		
	}

	

}
