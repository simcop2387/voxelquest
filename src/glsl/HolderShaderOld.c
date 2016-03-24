#version 330

uniform usamplerBuffer Texture0;

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


uniform mat4 modelviewInverse;
uniform mat4 modelview;
uniform mat4 proj;



$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 data0;
layout(location = 2) in vec4 data1;
layout(location = 3) in vec4 data2;

out vec4 worldPos;
out vec4 vdata0; // x: index / 4096, y: remaining index
out vec4 vdata1; // boundsMin
out vec4 vdata2; // boundsMax

void main() {
	
	worldPos = vec4(vposition.xyz,1.0);
	vdata0 = data0;
	vdata1 = floor(data1+0.5);
	vdata2 = floor(data2+0.5);
	vec4 screenPos = 
		proj*modelview*worldPos;
		//worldPos;
	
	//gl_PointSize = (heightOfNearPlane / pow(screenPos.w,0.5))*0.2;
		
	gl_Position = screenPos;
	
}

$

in vec4 worldPos;
in vec4 vdata0;
in vec4 vdata1;
in vec4 vdata2;

layout(location = 0) out vec4 FragColor0;

vec3 globColor;
float globTotSteps;

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

// void getRay(in vec2 newTC, inout vec3 ro, inout vec3 rd) {
// 	float aspect = bufferDim.y/bufferDim.x;
// 	float NEAR = clipDist.x;
// 	float FAR = clipDist.y;

// 	float dx = tan(FOV*0.5f)*(newTC.x*2.0-1.0f)/aspect; //gl_FragCoord.x/(bufferDim.x*0.5)
// 	float dy = tan(FOV*0.5f)*((1.0f-newTC.y)*2.0-1.0f); //gl_FragCoord.y/(bufferDim.y*0.5)

// 	dx = -dx;

// 	vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
// 	vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);

// 	p1 = modelviewInverse*p1;
// 	p2 = modelviewInverse*p2;

// 	ro = p1.xyz;
// 	rd = normalize(p1.xyz-p2.xyz);
// }



int getMax(
	vec3 ro,
	vec3 rd,
	float maxDis	
) {
	vec3 p1 = (ro);
	vec3 p2 = (ro + rd*maxDis);
	vec3 d = p2-p1;
	vec3 dabs = abs(d);
	
	int n = int(
			max(max(dabs.x,dabs.y),dabs.z)
	)*2;
	
	return n;
	
	//vec3 s = d/float(n);
}

vec3 lineStep(
		vec3 ro,
		vec3 rd,
		float maxDis,
		int curOffset
) {
		
		
		int n = getMax(ro,rd,maxDis);
		int n2 = 0;
		
		vec3 oneVec = vec3(1.0);
		int i;
		int j;
		
		bool neverHit = false;
		
		ivec4 readVal0;
		ivec4 readVal1;
		
		
		int curRead0 = 0;
		int curRead1 = 0;
		
		vec2 hitBox = vec2(0.0);
		vec2 cellBox = vec2(0.0);
		vec2 cellBox2 = vec2(0.0);
		
		vec3 p = ro;
		
		vec3 minPlanes;
		vec3 maxPlanes;
		
		vec3 centerPoint = vec3(0.0);
		
		vec3 testPoint;
		
		
		float minPrec = 0.001;
		
		vec3 newRD = rd*minPrec;
		
		float cellBoxOffset = 0.00;
		
		bool alreadyExists = false;
		
		
		int myBaseInd = -1;

		ivec3 iCenterPoint = ivec3(0,0,0);

		int iVoxelsPerCell = int(voxelsPerCell);
		int iVoxelsPerCellM1 = iVoxelsPerCell-1;

		float voxelsPerCellM1 = voxelsPerCell - 1.0;
		vec3 minVB = vec3(0.0);
		vec3 maxVB = vec3(voxelsPerCellM1);

		int newIndex;
		
		ivec4 yzPlane = ivec4(0);
		ivec4 xzPlane = ivec4(0);
		ivec4 xyPlane = ivec4(0);
		
		int yzOffset = 0;
		int xzOffset = 0;
		int xyOffset = 0;
		
		float maxDis2 = 0.0;
		
		vec3 localPos = vec3(0.0);
		vec3 localPos2 = vec3(0.0);
		vec3 pv;
		
		vec3 vpMin = vec3(0.0);
		vec3 vpMax = vec3(cellsPerHolder);
		
		for (i = 0; i < n; i++) {
				
				
				testPoint = p + newRD;
				minPlanes = floor(testPoint);
				maxPlanes = minPlanes + oneVec;
				centerPoint = (minPlanes+maxPlanes)*0.5;
				iCenterPoint = ivec3(centerPoint);

				cellBox = aabbIntersect(ro,rd,minPlanes,maxPlanes); // -cellBoxOffset +cellBoxOffset

				if (
					all(greaterThan(centerPoint,vpMin)) &&
					all(lessThan(centerPoint,vpMax))
				) {
					
					newIndex = curOffset + iCenterPoint.z*cellsPerHolder*cellsPerHolder + iCenterPoint.y*cellsPerHolder + iCenterPoint.x;

					readVal0 = ivec4(texelFetch(Texture0, newIndex/4));
					curRead0 = readVal0[newIndex%4];

					
					if (curRead0 != CUBE_DATA_INVALID) {
						
						return ro + rd*cellBox.x;
						
						//readVal1 = ivec4(texelFetch(Texture0, curRead0/4));
						//curRead1 = readVal1[curRead0%4];
						
						
						localPos = ro + rd*cellBox.x - minPlanes;
						localPos = clamp(localPos*voxelsPerCell,minVB,maxVB);
						
						localPos2 = ro + rd*cellBox.y - minPlanes;
						localPos2 = clamp(localPos2*voxelsPerCell,minVB,maxVB);
						
						maxDis2 = distance(localPos,localPos2);
						n2 = getMax(localPos,rd,maxDis2);
						
						pv = localPos;
						
						
						for (j = 0; j < n2; j++) {
							//return ro + rd*cellBox.x;
							
							
							testPoint = pv + newRD;
							minPlanes = floor(testPoint);
							maxPlanes = minPlanes + oneVec;
							centerPoint = (minPlanes+maxPlanes)*0.5;
							iCenterPoint = ivec3(centerPoint);
							cellBox2 = aabbIntersect(localPos,rd,minPlanes,maxPlanes);
							
							yzOffset = curRead0 + (
								iCenterPoint.y + iCenterPoint.z*iVoxelsPerCell +
								0*iVoxelsPerCell*iVoxelsPerCell
							)*CUBE_WRAP_ENTRIES;
							xzOffset = curRead0 + (
								iCenterPoint.x + iCenterPoint.z*iVoxelsPerCell +
								1*iVoxelsPerCell*iVoxelsPerCell
							)*CUBE_WRAP_ENTRIES;
							xyOffset = curRead0 + (
								iCenterPoint.x + iCenterPoint.y*iVoxelsPerCell +
								2*iVoxelsPerCell*iVoxelsPerCell
							)*CUBE_WRAP_ENTRIES;
							
							
							// if (
							// 	( (yzOffset%4) != 0 ) ||
							// 	( (xzOffset%4) != 0 ) ||
							// 	( (xyOffset%4) != 0 )
							// ) {
							// 	globColor = vec3(1.0,0.0,0.0);
							// }
							
							yzPlane = ivec4( texelFetch(Texture0, yzOffset/4) ); // x up
							xzPlane = ivec4( texelFetch(Texture0, xzOffset/4) ); // y up
							xyPlane = ivec4( texelFetch(Texture0, xyOffset/4) ); // z up
							
							
							// globColor = vec3(
							// 	max(max(yzPlane.x,xzPlane.x),xyPlane.x)/voxelsPerCell,
							// 	min(min(yzPlane.y,xzPlane.y),xyPlane.y)/voxelsPerCell,
							// 	0.0
							// );
							
							// return ro + rd*cellBox.x;
							
							// if (yzPlane.x == CUBE_WRAP_INVALID) {
							// 	yzPlane.x = 0;
							// }
							// if (yzPlane.y == CUBE_WRAP_INVALID) {
							// 	yzPlane.y = iVoxelsPerCellM1;
							// }
							// if (xzPlane.x == CUBE_WRAP_INVALID) {
							// 	xzPlane.x = 0;
							// }
							// if (xzPlane.y == CUBE_WRAP_INVALID) {
							// 	xzPlane.y = iVoxelsPerCellM1;
							// }
							// if (xyPlane.x == CUBE_WRAP_INVALID) {
							// 	xyPlane.x = 0;
							// }
							// if (xyPlane.y == CUBE_WRAP_INVALID) {
							// 	xyPlane.y = iVoxelsPerCellM1;
							// }
							
							globTotSteps += 1.0;
							
							if (
								   (0 == yzPlane.x) && (iVoxelsPerCellM1 == yzPlane.y)
								&& (0 == xzPlane.x) && (iVoxelsPerCellM1 == xzPlane.y) 
								&& (0 == xyPlane.x) && (iVoxelsPerCellM1 == xyPlane.y)
							) {
								
							}
							else {
								if (
									   (iCenterPoint.x >= yzPlane.x) && (iCenterPoint.x <= yzPlane.y)
									&& (iCenterPoint.y >= xzPlane.x) && (iCenterPoint.y <= xzPlane.y) 
									&& (iCenterPoint.z >= xyPlane.x) && (iCenterPoint.z <= xyPlane.y)
								) {
									return ro + rd*cellBox.x + rd*cellBox2.x/voxelsPerCell;
								}
							}
							
							
							

							// if (globTotSteps > 32.0) {
							// 	return vec3(0.0);
							// }

							pv = localPos + rd*cellBox2.y;
							
							if (distance(pv,localPos) > maxDis2) {
								break;
							}
						}
						
						
						
						

						
					}
					
				}
				
				p = ro + rd*cellBox.y;
				
				if (distance(p,ro) > maxDis) {
					//neverHit = true;
					break;
				}
		}
		
		return vec3(0.0);
		
		// if (neverHit) {
		// 	return vec3(0.0);
		// }
		// else {
		// 	return p;
		// }

		
		
}

void main() {

	globColor = vec3(0.0);
	globTotSteps = 0.0;

	vec4 oneVec = vec4(1.0);

	vec2 baseCoords = gl_FragCoord.xy/(bufferDim.xy);
	vec3 ro = cameraPos.xyz;//vec3(0.0);
	vec3 rd = normalize(worldPos.xyz-cameraPos.xyz);//vec3(0.0);

	vec3 boxMin = vdata1.xyz;
	vec3 boxMax = vdata2.xyz;
	
	vec2 hitBox = aabbIntersect(ro,rd,boxMin,boxMax);

	if (
		all(greaterThanEqual(ro,boxMin)) &&
		all(lessThanEqual(ro,boxMax))	
	) {
		hitBox.x = 0.0;
	}

	//getRay(baseCoords, ro, rd);

	

	if (hitBox.x < hitBox.y) {
		
	}
	else {
		discard;
	}

	vec3 baseCol = mod((worldPos.xyz+0.01)/float(cellsPerHolder),1.0);
	


	vec3 curPos = ro + rd*hitBox.x;

	curPos -= vdata1.xyz;

	
	int curOffset = int(vdata0.x+0.005)*4096 + int(vdata0.y+0.005);

	vec3 res = lineStep(curPos, rd, hitBox.y, curOffset);

	

	if (
		dot(abs(res),oneVec.xyz) == 0.0
	) {
		discard;
	}

	vec3 finalPos = res + vdata1.xyz;



	FragColor0 = vec4(
		//mod((worldPos.xyz+0.01)/32.0,1.0),
		
		//globColor
		
		mod((finalPos.xyz+0.01)/float(cellsPerHolder),1.0)
		
		//vec3(globTotSteps/64.0) // + baseCol*0.25
		
		
		// * vec3(
		// 	1.0-distance(finalPos.xyz,cameraPos)/100.0	
		// )
		,
		1.0
	);

}









