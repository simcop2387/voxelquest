#version 120

uniform sampler3D Texture0; //volume texture nearest
uniform sampler3D Texture1; //volume texture linear
uniform sampler2D Texture2; //terrain heightmap
uniform sampler2D Texture3; //terrainID

uniform float totLayers;
uniform float heightmapMax;
uniform float heightmapMin;
uniform float seaLevel;
uniform float maxSeaDepth;
uniform float volumePitch;
uniform float bufferedPageSizeInUnits;
uniform float bufferMult;
uniform float pixelsPerMeter;

uniform int numEntries;
uniform int paramsPerEntry;


//uniform float directPass;
//uniform float slicesPerPitch;

uniform bool hasTree;
uniform bool hasGeom;
uniform bool hasTerrain;

uniform vec2 resolution;
uniform vec3 worldSizeInPixels;
uniform vec3 worldMinVisInPixels;
uniform vec3 worldMaxVisInPixels;
uniform vec3 worldMinBufInPixels;
uniform vec3 worldMaxBufInPixels;
uniform vec3 paramArr[256];
uniform float matCountArr[16];
uniform float maxFloors;

uniform vec4 mapFreqs;
uniform vec4 mapAmps;

varying vec2 TexCoord0;
int iVolumePitch = int(volumePitch);

const float PI = 3.14159;
const float PIO2 = PI/2.0;
const float PIO4 = PI/4.0;
const float invalid = 99999.0;

const int E_GP_VISMININPIXELST = 0;
const int E_GP_VISMAXINPIXELST = 1;
const int E_GP_BOUNDSMININPIXELST = 2;
const int E_GP_BOUNDSMAXINPIXELST = 3;
const int E_GP_CORNERDISINPIXELS = 4;
const int E_GP_POWERVALS = 5;
const int E_GP_POWERVALS2 = 6;
const int E_GP_THICKVALS = 7;
const int E_GP_MATPARAMS = 8;
const int E_GP_LENGTH = 9;


const int E_TP_VISMININPIXELST = 0;
const int E_TP_VISMAXINPIXELST = 1;
const int E_TP_P0 = 2;
const int E_TP_P1 = 3;
const int E_TP_P2 = 4;
const int E_TP_POWERVALS = 5;
const int E_TP_POWERVALS2 = 6;
const int E_TP_THICKVALS = 7;
const int E_TP_MATPARAMS = 8;
const int E_TP_LENGTH = 9;


const float TEX_NULL = 		0.0;
const float TEX_GOLD = 		1.0;
const float TEX_DIRT = 		8.0;
const float TEX_STONE = 	10.0;
const float TEX_GRASS = 	12.0;
const float TEX_SAND = 		14.0;
const float TEX_MORTAR = 	16.0;
const float TEX_WOOD = 		18.0;
const float TEX_BRICK = 	20.0;
const float TEX_SHINGLE = 	22.0;
const float TEX_PLASTER = 	28.0;
const float TEX_DEBUG = 	30.0;
const float TEX_WATER = 	32.0;
const float TEX_METAL = 	33.0;
const float TEX_GLASS = 	35.0;
const float TEX_EARTH = 	36.0;
const float TEX_BARK =		41.0; 
const float TEX_TREEWOOD =  43.0;
const float TEX_LEAF =  	45.0;



const float E_MAT_PARAM_ROAD = 		0.0;
const float E_MAT_PARAM_DOCK = 		1.0;
const float E_MAT_PARAM_BUILDING = 	2.0;
const float E_MAT_PARAM_LANTERN = 	3.0;
const float E_MAT_PARAM_DOORWAY = 	4.0;
const float E_MAT_PARAM_DOOR = 		5.0;
const float E_MAT_PARAM_WINDOW = 	6.0;
const float E_MAT_PARAM_SLATS =		7.0;
const float E_MAT_PARAM_TREE = 		8.0;
const float E_MAT_PARAM_LENGTH = 	9.0;

float shingleMod;
vec2 dirVecs[4] = vec2[](
	vec2( 1.0,  0.0 ), // right
	vec2( -1.0,  0.0 ), // left
	vec2( 0.0,  1.0 ), // up
	vec2( 0.0,  -1.0 ) // down

);

vec3 dirVecs2[6] = vec3[](
	vec3( 1.0,  0.0, 0.0 ), // right
	vec3( -1.0, 0.0, 0.0 ), // left
	vec3( 0.0, 1.0, 0.0 ), // up
	vec3( 0.0, -1.0, 0.0 ), // down
	vec3( 0.0, 0.0, 1.0 ), // above
	vec3( 0.0, 0.0, -1.0 ) // below
);


mat2 dirMats[4] = mat2[](
	mat2( 0.0, 1.0, -1.0, 0.0 ),   // right
	mat2( 0.0, 1.0, 1.0, 0.0 ),  // left
	mat2( 1.0, 0.0, 0.0, -1.0 ), // up
	mat2( 1.0, 0.0, 0.0, 1.0 )   // down

);


vec3 voroOffsets[27] = vec3[](
	vec3(  -1.0,  -1.0,  -1.0  ),
	vec3(  -1.0,  -1.0,  0.0  ),
	vec3(  -1.0,  -1.0,  1.0  ),

	vec3(  -1.0,  0.0,  -1.0  ),
	vec3(  -1.0,  0.0,  0.0  ),
	vec3(  -1.0,  0.0,  1.0  ),

	vec3(  -1.0,  1.0,  -1.0  ),
	vec3(  -1.0,  1.0,  0.0  ),
	vec3(  -1.0,  1.0,  1.0  ),


	
	vec3(  0.0,  -1.0,  -1.0  ),
	vec3(  0.0,  -1.0,  0.0  ),
	vec3(  0.0,  -1.0,  1.0  ),

	vec3(  0.0,  0.0,  -1.0  ),
	vec3(  0.0,  0.0,  0.0  ),
	vec3(  0.0,  0.0,  1.0  ),

	vec3(  0.0,  1.0,  -1.0  ),
	vec3(  0.0,  1.0,  0.0  ),
	vec3(  0.0,  1.0,  1.0  ),



	vec3(  1.0,  -1.0,  -1.0  ),
	vec3(  1.0,  -1.0,  0.0  ),
	vec3(  1.0,  -1.0,  1.0  ),

	vec3(  1.0,  0.0,  -1.0  ),
	vec3(  1.0,  0.0,  0.0  ),
	vec3(  1.0,  0.0,  1.0  ),

	vec3(  1.0,  1.0,  -1.0  ),
	vec3(  1.0,  1.0,  0.0  ),
	vec3(  1.0,  1.0,  1.0  )

);




$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;

}

$

float unpack16(vec2 num) {
    return num.r*255.0 + num.g*65280.0;
}

int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}

float intModF(float v1, int rhs) {
    int lhs = int(v1);
    return float( lhs - ( (lhs/rhs)*rhs ) );
}

vec3 intModFV(vec3 v1, vec3 v2) {
	ivec3 rhs = ivec3(v2);
    ivec3 lhs = ivec3(v1);
    return vec3(lhs - ( (lhs/rhs)*rhs ));
}

float randf(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
float randf3(vec3 co) {
	return fract(sin(dot(co.xyz ,vec3(12.989, 78.233, 98.422))) * 43758.8563);
}


vec3 rand(vec3 co) {
    return vec3(
    	fract(sin(dot(co.xyz ,vec3(12.989, 78.233, 98.422))) * 43758.8563),
    	fract(sin(dot(co.zyx ,vec3(93.989, 67.345, 54.256))) * 24634.6345),
    	fract(sin(dot(co.yxz ,vec3(43.332, 93.532, 43.734))) * 56445.2345)
    );
}

float getHeightAtCoords(vec2 coord) {
	
	float terLerp = 0.0;
	float terMod = 0.0;
	vec3 terCenter = vec3(0.0);

	vec2 mapCoord = (coord / worldSizeInPixels.x);


	vec4 texHM0 =  texture2D(Texture2, mapFreqs.x * mapCoord );
	vec4 texHM1 =  texture2D(Texture2, mapFreqs.y * mapCoord );
	vec4 texHM2 =  texture2D(Texture2, mapFreqs.z * mapCoord );
	vec4 texHM3 =  texture2D(Texture2, mapFreqs.w * mapCoord );

	float toff = 0.1;
	vec2 tmod = vec2((0.5-toff)/4096,(0.5+toff)/4096.0);

	vec4 texLev = (
		texture2D(Texture3, mapCoord + tmod.xx ) + 
		texture2D(Texture3, mapCoord + tmod.xy ) + 
		texture2D(Texture3, mapCoord + tmod.yx ) + 
		texture2D(Texture3, mapCoord + tmod.yy )
	)/4.0;
	

	

	float testHeight0 = 
	mix(
		heightmapMin,
		heightmapMax,
		texHM0.r*mapAmps.x + texHM1.r*mapAmps.y + texHM2.r*mapAmps.z + texHM3.r*mapAmps.w
	);
	
	float testHeight1 = unpack16(texLev.rg);// + texHM3.r*3.0*pixelsPerMeter;
	
	//testHeight0 = mix(testHeight0,testHeight1,0.5);

	float testHeightFinal = mix(testHeight0, testHeight1, texLev.b);
	float sl = seaLevel-maxSeaDepth/2.0f;

	if (testHeightFinal < sl) {
		testHeightFinal = sl - ((sl-testHeightFinal)/sl)*maxSeaDepth;
	}
	return testHeightFinal;

	//return testHeight;//vec2(testHeight,testHeight0);
}

// float getNoiseAtCoords(vec2 coord) {
// 	vec2 mapCoord = (coord / worldSizeInPixels.x);

// 	vec4 texHM3 =  texture2D(Texture2, mapFreqs.w * mapCoord );

// 	return texHM3.r;
// }

vec3 getShingle(vec3 uvwBase, float uvscale, float baseDepth, float maxDepth) {
	vec3 color;
	vec3 position, useBrick;
	vec3 brickDim;
	vec3 uvw = uvwBase.xyz/uvscale;
	vec3 myResult = vec3(0.0);

	float numScales = 2.0;
	float finalDis = 0.0;
	
	ivec2 iUV = ivec2(uvw.xy*numScales);
	vec2 dis = uvw.xy*numScales-floor(uvw.xy*numScales);

	dis.y = 1.0-dis.y;

	float myMod;

	if ( intMod(iUV.x + iUV.y,2) == 0 ) {
		
	}
	else {
		dis.x = 1.0-dis.x;
	}

	myMod = float(intMod(iUV.y,2));

	if (length(dis) < 1.0) { //(dis.x+dis.y)

		myResult.x = myMod;
		finalDis = 1.0-(dis.y*0.5 + 0.5);
	}
	else {
		myResult.x = 1.0-myMod;
		finalDis = 1.0-dis.y*0.5;
	}

	shingleMod = finalDis;//+abs(sin( (uvwBase.x+uvwBase.y)/(2.0*pixelsPerMeter) ));//randf(vec2(finalDis,finalDis));


	if ( baseDepth > ( finalDis*maxDepth ) ) {
		myResult.y = TEX_SHINGLE;
	}
	else {
		myResult.y = TEX_NULL;
	}


	//myResult.w = randf(vec2(iUV.x+iUV.y);

	return myResult;

}

bool brick(vec3 uvw, vec3 uvscale, bool isVert) {
	vec3 color;
	vec3 position, useBrick;
	vec3 brickDim;

	
	vec3 bricksize;
	bricksize = vec3(1.0,0.5,1.0)*uvscale;

	vec3 brickPct = vec3(0.8,0.8,0.8);

	position = uvw.xyz / bricksize.xyz;


	if (isVert) {
		
		if (fract(position.y * 0.5) > 0.5) {
			position.x += 0.5;
		}
	}
	else {
		if (fract(position.y * 0.5) > 0.5) {
			position.x += 0.5;
		}
	}
    
	position = fract(position - 0.1);
	useBrick = step(position, brickPct.xyz);

	return (useBrick.x * useBrick.y * useBrick.z) > 0.0;
}


bool getInterval(float valInPix, float intervalInMeters, float widthInMeters ) {

	float intervalInPixels = intervalInMeters*pixelsPerMeter;
	float widthInPixels = widthInMeters*pixelsPerMeter;

	return ( abs(abs( mod (valInPix, intervalInPixels) ) - intervalInPixels/2.0)*2.0 > (intervalInPixels-widthInPixels) );
}

float getIntervalSpaced(float valInPix, float intervalInMeters, float spacing) {
	
	float r0 = 1.0-float(getInterval(valInPix + 0.5*intervalInMeters*pixelsPerMeter, intervalInMeters, spacing));
	float r1 = float(getInterval(valInPix, 2.0*intervalInMeters, intervalInMeters));

	return r0*(r1+1.0);

}

float getBoardSpaced(vec3 coords, vec3 dimInMeters, float boardSpacing) {
	

	vec4 res = vec4(0.0);
	vec3 zeroVec = vec3(0.0);

	res.y = getIntervalSpaced(
		coords.y,
		dimInMeters.y,
		boardSpacing
	);
	res.x = getIntervalSpaced(
		coords.x + (0.5*dimInMeters.x)*pixelsPerMeter*float(res.y>1.0),
		dimInMeters.x,
		boardSpacing
	);

	res.z = getIntervalSpaced(
		coords.z,
		dimInMeters.z,
		boardSpacing
	);


	return float(all(greaterThan(res.xyz,zeroVec)))*(res.x+res.y*2.0);	

	
}

float getWoodGrain(float normalUID, vec3 worldPosInPixels, float woodRad, float boardDir, float stretchAmount) {
	float woodDiam = woodRad*2.0;

	vec3 newPos = vec3(0.0);//worldPosInPixels.xyz;

	if (boardDir == 0.0) {
		newPos = worldPosInPixels.xyz;
	}
	if (boardDir == 1.0) {
		newPos = worldPosInPixels.yxz;
	}
	if (boardDir == 2.0) {
		newPos = worldPosInPixels.zyx;
	}
	if (boardDir == 3.0) {
		newPos = worldPosInPixels.zyx;
		//worldPosInPixels.y += worldPosInPixels.z;

	}
	
	vec2 woodCenter = floor( (newPos.xy+woodRad)/woodDiam)*woodDiam;
	vec2 woodVec = normalize(newPos.xy - woodCenter);
	float woodLen = newPos.z;


	woodCenter.y*=stretchAmount;
	newPos.y*=stretchAmount;
	

	float woodAngle = atan(woodVec.y,woodVec.x);
	float finalMod = (sin(
		
		( distance(newPos.xy + mod(normalUID,4.0)*pixelsPerMeter,woodCenter)+(woodRad/2.0) ) *
		( (8.0+sin(woodAngle*24.0 )*0.0625+sin(woodAngle*12.0 )*0.125+sin(woodLen/16.0)*0.5+sin(woodLen/4.0)*0.25  ) / (woodRad/sqrt(2.0)) )

	));
	if (finalMod < 0.0) {
		finalMod = (1.0-finalMod)/2.0;
	}

	return finalMod;
}

float getBoard(vec2 coords, float boardPitch, float boardLength) {
	
	if ( getInterval(coords.y,2.0*boardPitch,boardPitch) ) {
		if ( getInterval(coords.x,2.0*boardLength,boardLength) ) {
			return 0.0;
		}
		else {
			return 1.0;
		}
	}
	else {
		if ( getInterval(coords.x + (0.5*boardLength)*pixelsPerMeter,2.0*boardLength,boardLength) ) {
			return 2.0;
		}
		else {
			return 3.0;
		}
	}

	
}

vec4 getBuilding(
		vec4 newUVW,
		vec3 relPos,
		bvec3 bLessThanOrig,
		bool isVert,
		bool notScaff,
		float maxDisFromBottomInPixels,
		float nearestJointXY,
		float resXY,
		float thickness,
		float disCenterMinMax,
		float visDisXY
	) {

	

	float disFromBottomInPixels = relPos.z;
	float disFromInsideInPixels = newUVW.z;//max(newUVW.z,visDisXY*thickness);
	float capDis = thickness-visDisXY*thickness;
	float disFromOutsideInPixels = 0.0;
	float disFromOutsideRef = 0.0;
	float floorHeightInPixels = 4.0*pixelsPerMeter;
	
	float horzBoardThickness = 0.5;

	float topMod = mix(-1.0,1.0, float( (maxDisFromBottomInPixels-disFromBottomInPixels) < horzBoardThickness*pixelsPerMeter/2.0 )  );

	// float topMod = mix(0.25,-0.25,float(
	// 	maxDisFromBottomInPixels-disFromBottomInPixels < 0.25*pixelsPerMeter
	// ))*pixelsPerMeter;



	float floorNumber = floor(disFromBottomInPixels/floorHeightInPixels);//floor((disFromBottomInPixels+topMod)/floorHeightInPixels);

	bool isBasement = (floorNumber <= 0.0)&&(notScaff);
	bool notBasement = !isBasement;

	float wallThickness = (0.5+0.5*float(isBasement))*pixelsPerMeter;
	bool bIsOuterWall;
	bool bIsCapWall = (capDis < wallThickness);

	
	float shingleOffset = 16.0;
	float vertMod = 50.0*float(isVert);
	float offsetPerFloor = 0.25;
	float floorOffset = offsetPerFloor*(maxFloors-floorNumber) - float(isBasement)*offsetPerFloor*(2.0) + float(!notScaff)*1.0;

	bool bIsRoof = !bLessThanOrig.z;
	float fIsRoof = float(bIsRoof);

	

	disFromOutsideInPixels = (thickness-floorOffset*pixelsPerMeter ) - disFromInsideInPixels;
	bIsOuterWall = disFromOutsideInPixels < (wallThickness + 0.5 * fIsRoof * pixelsPerMeter );

	float shingleDepth = 0.25*pixelsPerMeter;
	bool bIsOuterWallBoards =  (disFromOutsideInPixels < (0.5*pixelsPerMeter)) && (disFromOutsideInPixels > (0.25*pixelsPerMeter));
	bool bIsShingle = (disFromOutsideInPixels < (shingleDepth));// && (disFromOutsideInPixels > (0.0*pixelsPerMeter));

	


	
	float notCorner = float(bLessThanOrig.x||bLessThanOrig.y);
	
	float isOuterWall = float(bIsOuterWall);
	bool isJoint = (nearestJointXY <= 0.1875*pixelsPerMeter);
	bool isNearJoint = (nearestJointXY <= 2.0*pixelsPerMeter);
	bool doBricks = false;
	bool bIsBasementFloor = false;
	float plasterDis = 0.0;
	float boardThickness = 0.25;
	float dirMod = 0.0;

	// if (bLessThanOrig.z) {
	// 	boardThickness = 0.25;
	// }

	if ( (capDis < 1.0*pixelsPerMeter)&&bIsOuterWall ) {
		boardThickness = 1.0;
	}

	

	vec4 myResult = vec4(0.0);
	myResult.y = TEX_WOOD;
	myResult.z = 1.0;

	float fVert = float(isVert);

	myResult.w = fVert;

	bool bIsOuterWallOrig = bIsOuterWall;

	if ((!bIsOuterWall) && bIsCapWall) {

		myResult.w = 1.0-fVert;

		disFromOutsideInPixels = capDis-0.0625*pixelsPerMeter;

		

		bIsOuterWall = bIsCapWall;
		isOuterWall = float(bIsCapWall);

		if (isVert) {
			newUVW.x = relPos.x;
		}
		else {
			newUVW.x = relPos.y;
		}
		
		newUVW.y = relPos.z;
		newUVW.z = disFromOutsideInPixels;
	}

	doBricks = isBasement;

	bool isVertBoard = false;
	bool isHorzBoard = false;

	if (notBasement) {
		
		if (
			getInterval(newUVW.y - (horzBoardThickness*0.5 + 0.25)*pixelsPerMeter, 4.0, 0.125) &&
			bLessThanOrig.z &&
			(!bIsOuterWall)
			
		) {
			
			// floor boards
			if (notScaff) {
				myResult.x = 6.0 + getBoard(newUVW.xz, 0.375, 2.0);
			}
			else {
				if (floorNumber > 1.0) {
					myResult.x = 6.0 + getBoard(newUVW.zx, 0.5, 4.0);
				}
			}


			
			
			
		}
		else {



			

			isVertBoard = getInterval( newUVW.x, 2.0, mix(boardThickness/pow(resXY*0.7+0.3,2.0),boardThickness,notCorner) );
			isHorzBoard = getInterval( newUVW.y + topMod*horzBoardThickness*pixelsPerMeter/2.0, 4.0, horzBoardThickness);

			if ( isHorzBoard && isVertBoard && bLessThanOrig.z && (floorNumber > 0.0) ) {
									
				// joists and rafters
				myResult.w = 1.0-fVert;
				myResult.x = 4.0;
			}
			else {
				if ( ( isVertBoard && bLessThanOrig.z && (!isNearJoint) ) || (bIsRoof && (isJoint||isVertBoard) ) ) {
					myResult.w = 2.0;
					myResult.x = 2.0*isOuterWall;
				}

				if ( (myResult.x == 0.0) ) { // && (bLessThanOrig.z)

					if (isHorzBoard) {
						// horizontal wall support beams

						if ( ( isJoint&&bLessThanOrig.z ) || (bIsOuterWall) ) {
							myResult.x = 3.0;
						}
					}
					else {

						if (bLessThanOrig.z) {
							if ( isJoint && bIsOuterWall ) {

								// is a joint welding
								myResult.x = 3.0;

								if (bLessThanOrig.z) {
									myResult.w = 2.0;
								}
							}


							if (( getInterval(newUVW.y+2.5*pixelsPerMeter, 4.0, horzBoardThickness/2.0) ) ) {
								myResult.x = 3.0*isOuterWall;
								myResult.y = TEX_WOOD;
							}
							else {

								
								// diagonal beams

								if (!getInterval(newUVW.x,8.0,4.0) && (notCorner == 1.0) && (notScaff)) {
									dirMod = (float(getInterval(newUVW.x,8.0,4.0))-0.5)*2.0;

									if ( getInterval(newUVW.x + dirMod*newUVW.y*0.5, 2.0, 0.25) && bIsOuterWall ) {
										myResult.x = 50.0;
										myResult.w = 2.0;
									}
								}

								
							}
						}

						


						
					}
					
				}
			}

				
				
		}
	}

	

	// inner pillars / beams
	if ( (!bIsOuterWallOrig) && ( notBasement || (isBasement&&(!bIsCapWall)) ) && (notScaff || ((!notScaff)&&(floorNumber <= 1.0))) ) {
		if (resXY*thickness < 0.25*pixelsPerMeter) {

			if ( getInterval(disCenterMinMax, 8.0, 0.25 ) ) {
				myResult.x = 5.0;
				myResult.w = 2.0;
			}

		}
	}

	// if (bIsOuterWallOrig && bIsCapWall) {
	// 	myResult.x = 3.0;
	// 	myResult.y = TEX_WOOD;
	// 	myResult.w = 2.0;
	// }

	if (myResult.x == 0.0) {
		myResult.y = TEX_NULL;
	}

	

	if (disFromOutsideInPixels < 0.0) {
		
		if (myResult.x == 4.0) {
			if (disFromOutsideInPixels < -0.0625*pixelsPerMeter ) {
				myResult.x = 0.0;
				myResult.y = TEX_NULL;
				myResult.z = 0.0;
			}	
		}
		else {
			myResult.x = 0.0;
			myResult.y = TEX_NULL;
			myResult.z = 0.0;
		}
		
	}

	if (bLessThanOrig.z) {
		doBricks = doBricks || (disFromOutsideInPixels >= 0.0);
	}
	else {
		

		
		if (bIsShingle) {
			////////////////////////////////////
			// SHINGLE MAT
			////////////////////////////////////

			//disFromOutsideInPixels
			myResult.xyz = getShingle(newUVW.xyz, 1.0*pixelsPerMeter, disFromOutsideInPixels, shingleDepth );
			myResult.x += shingleOffset;

			myResult.z = float(myResult.y != TEX_NULL);


		}
		else {
			if (bIsOuterWallBoards) { // roof boards

				myResult.x = 11.0 + getBoard(newUVW.xy, 1.0, 2.0);
				myResult.y = TEX_WOOD;
			}
			else {
				doBricks = doBricks || bIsCapWall;
			}
		}
		if (disFromOutsideInPixels < 0.0) {
			myResult.x = 0.0;
			myResult.y = TEX_NULL;
			doBricks = false;
		}
		
	}


	bIsBasementFloor = isBasement && (disFromBottomInPixels < 1.0*pixelsPerMeter);
	
	if ( bIsBasementFloor && (!bIsOuterWall) ) {
		
		disFromOutsideInPixels = disFromBottomInPixels;
		newUVW.xyz = newUVW.xzy;
	}

	if ( (bIsOuterWall||bIsBasementFloor) && (myResult.y == TEX_NULL) && doBricks && (notScaff||isBasement) ) {

		// myResult.x = 30.0;
		// myResult.y = TEX_PLASTER;

		//disFromOutsideInPixels = min(capDis)

		disFromOutsideInPixels = min(disFromOutsideInPixels,capDis);

		disFromOutsideRef = (1.0-abs(clamp( (disFromOutsideInPixels/wallThickness), 0.0, 1.0)-0.5)*2.0)*wallThickness;


		if (disFromOutsideRef < 0.125*pixelsPerMeter) {
			myResult.x = 0.0;
			myResult.y = TEX_NULL;
			//myResult.z = 0.0;
		}
		else {

			myResult.x = getBoardSpaced(newUVW.xyz-vec3(0.0,0.25,0.0)*pixelsPerMeter, vec3(1.0,0.5,1.0), 0.125 );
			myResult.x += 21.0;
			
			if ( (myResult.x != 21.0) ) {
				myResult.y = TEX_BRICK;
				plasterDis = 0.25;
			}
			else {
				plasterDis = 0.375;
				if ( disFromOutsideRef > 0.1875*pixelsPerMeter ) {//abs(res - (0.5 - texDepth) )*2.0 < (1.0-texDepth*2.0)  ) {
					myResult.y = TEX_MORTAR;
					
				}
			}


			if ( (disFromOutsideRef < (plasterDis)*pixelsPerMeter) ) {
				myResult.x = 0.0;
				myResult.y = TEX_PLASTER * float(notBasement);//float(disFromBottomInPixels/floorHeightInPixels > 1.4); // *float(floorNumber > 1.0);// 
			}

			
		}

		if (bIsBasementFloor && bIsOuterWall && (myResult.y == TEX_NULL) && (disFromOutsideInPixels > wallThickness*0.5) ) {
			myResult.y = TEX_MORTAR;
		}

		// if ( (disFromOutsideInPixels < 0.125*pixelsPerMeter) && myResult.y == TEX_NULL ) {
		// 	myResult.z = 0.0;
		// }

	}

	if ( ((disFromOutsideInPixels < wallThickness*0.5)||(!notScaff)) && myResult.y == TEX_NULL) {
		myResult.z = 0.0;
	}


	myResult.x += vertMod*float(myResult.x != 0.0)*float( myResult.y != TEX_SHINGLE );

	// if (myResult.y != TEX_NULL) {
	// 	myResult.z = 1.0;
	// }

	return myResult;

	

}


vec4 getCobble(vec3 worldPosInPixels, float voroSize, vec3 outDis, float minSpacing, float maxSpacing, bool isVert, bool doClamp, bool isVaried, int startInd, int endInd) {


	vec4 myResult = vec4(0.0);


	vec3 curPos;
	vec3 bestPos = vec3(0.0);
	vec3 nextBestPos = vec3(0.0);
	int startPos = 0;
	float curVoroDis;
	float notValid = 999.0;
	float minDis1 = notValid;
	float minDis2 = notValid;
	float gradVal;
	vec3 posFloored = vec3(0.0);
	vec3 randNum = vec3(0.0);
	vec3 oneVec = vec3(1.0);
	float bestRand;

	int i;
	
	vec3 newWP = (worldPosInPixels-vec3(0.0,0.0,outDis.z));

	posFloored = floor( newWP/voroSize );

	if ( mod(dot(posFloored,oneVec),2.0) > 0.0 ) {
		startPos = 0;
	}
	else {
		startPos = 1;
	}

	for (i = startPos+startInd; i < endInd; i += 2) {
		curPos = voroOffsets[i].zyx + posFloored;
		randNum = rand(curPos);
		curPos += randNum*0.75;
		curPos *= voroSize;

		curVoroDis = distance(curPos, newWP);

		if (curVoroDis < minDis1) {
			nextBestPos = bestPos;
			bestPos = curPos;

			minDis2 = minDis1;
			minDis1 = curVoroDis;
		}
		else {
			if (curVoroDis < minDis2) {
				nextBestPos = curPos;
				minDis2 = curVoroDis;
			}
		}

	}

	bestRand = randf3(bestPos);

	gradVal = clamp(1.0 - (minDis1*2.0/(minDis1+minDis2)),1.0/255.0,1.0);

	if (gradVal >  (minDis1+minDis2)/(minSpacing+bestRand*maxSpacing) ) {
		myResult.y = TEX_STONE;
	}


	if (doClamp) {
		if (isVert) {
			if (
				(distance(bestPos.x,outDis.x) > 1.5*pixelsPerMeter) ||
				(distance(worldPosInPixels.xz,outDis.xz) > 1.75*pixelsPerMeter)
			) {
				myResult.y = TEX_NULL;
			}
		}
		else {
			if (
				( distance(bestPos.y,outDis.y) > 1.5*pixelsPerMeter ) ||
				( distance(worldPosInPixels.yz,outDis.yz) > 1.75*pixelsPerMeter )
			) {
				myResult.y = TEX_NULL;
			}
		}

		// if ( distance(bestPos.z,outDis.z) > 1.5*pixelsPerMeter ) {
		// 	myResult.y = TEX_NULL;
		// }

		// if ( abs(worldPosInPixels.z-outDis.z) > 0.9*pixelsPerMeter ) {
		// 	myResult.y = TEX_NULL;
		// }
	}

	
	
	if (isVaried) {
		myResult.z = bestRand;
	}
	else {
		myResult.z = gradVal;
	}
	

	myResult.x = floor(myResult.z*16.0)+1.0;

	/*
	normalUID = matResult.x;
	finalMat = matResult.y;
	finalMod = matResult.z;
	*/

	return myResult;
	
}

vec4 getTerrain(vec3 worldPosInPixels) {



	float testHeight = getHeightAtCoords(worldPosInPixels.xy);
	//float testHeight = thVec.x;
	//float testHeightNoLevel = thVec.y;
	float sandEnd = (seaLevel + 1.0*pixelsPerMeter);
	float sandBeg = sandEnd - 0.5*pixelsPerMeter;
	float randVal = randf3(worldPosInPixels);//(testHeight-worldPosInPixels.z)/256.0 + ;
	float newHeight = 0.0;


	vec4 myResult = vec4(0.0);
	



	////////////////////////////////////
	// TERRAIN MAT
	////////////////////////////////////

	if (testHeight > worldPosInPixels.z ) {
		

		// sandBeg < sandEnd

		newHeight = 1.0-clamp( ((clamp(testHeight + (testHeight-worldPosInPixels.z) - randVal*(sandEnd-sandBeg),sandBeg,sandEnd)-sandBeg)/(sandEnd-sandBeg)), 0.0, 1.0);

		myResult.y = mix(TEX_DIRT,TEX_SAND,float(newHeight > 0.5));


		// if (testHeight > sandStart ) {
		// 	myResult.y = TEX_DIRT;

		// }
		// else {

		// 	if (testHeight > sandEnd) {

		// 		if (((testHeight-sandEnd)/(sandStart-sandEnd)) + randVal > 1.0) {
		// 			myResult.y = TEX_DIRT;
		// 		}
		// 		else {
		// 			myResult.y = TEX_SAND;
		// 		}

		// 	}
		// 	else {
		// 		myResult.y = TEX_SAND;
		// 	}
		// }

		if (myResult.y == TEX_DIRT) {
			myResult.y = mix(
							TEX_DIRT,
							TEX_GRASS,
							float( (abs(testHeight - worldPosInPixels.z) < 0.25*pixelsPerMeter) )
						);
		}

		/*if ( (myResult.y != TEX_NULL) && (testHeightNoLevel + randVal*(sandEnd-sandBeg) < seaLevel) ) {
			myResult.y = TEX_SAND;
		}*/

		

	}
	
	myResult.w = testHeight;

	return myResult;

}

vec4 getSlats(vec4 newUVW, float thickness, vec3 origin, vec3 worldPosInPixels, vec3 visMinInPixels, vec3 visMaxInPixels, bool isVert, float isWindow) {

	vec4 myResult = vec4(0.0);
	vec3 orig = vec3(0.0);
	vec3 rad = vec3(0.0);
	bool bDis1 = false;
	bool bDis2 = false;
	bool bDis3 = false;
	bool bIsWindow = bool(isWindow);

	float thickMod = 0.0;

	if (isVert) {
		
	}
	else {
		worldPosInPixels.xy = worldPosInPixels.yx;
		visMinInPixels.xy = visMinInPixels.yx;
		visMaxInPixels.xy = visMaxInPixels.yx;
		origin.xy = origin.yx;
	}

	orig = (visMaxInPixels+visMinInPixels)/2.0;
	rad = (visMaxInPixels-visMinInPixels)/2.0;


	thickMod = mix(0.1875,0.1875/2.0,isWindow);
	bDis2 = distance(worldPosInPixels.x,origin.x) < thickMod*pixelsPerMeter;



		

		bDis1 = (
			(distance(worldPosInPixels.z,orig.z+0.5*pixelsPerMeter) < thickMod*pixelsPerMeter/2.0) ||
			(distance(worldPosInPixels.z,orig.z-0.5*pixelsPerMeter) < thickMod*pixelsPerMeter/2.0)
		)&&(!bIsWindow);
		
		bDis3 = (distance(worldPosInPixels.z,orig.z) < thickMod*pixelsPerMeter/2.0)&&bIsWindow;

		if (bDis1||bDis2||bDis3) {
			myResult.y = mix(TEX_WOOD,TEX_METAL,isWindow);
			myResult.w = float(isVert);
			myResult.x = 20.0 + float(bDis2);
		}
		else {

			if (
				(distance(worldPosInPixels.y,orig.y) < rad.y/2.0) &&
				(!bDis2)
			) {
				myResult.y = mix(TEX_WOOD,TEX_GLASS,isWindow);
				myResult.w = 2.0;
				myResult.x = (1.0 + floor(distance(worldPosInPixels.x,visMinInPixels.x)/(0.375*pixelsPerMeter) ))*(1.0-isWindow);
			}
			else {
				myResult.y = TEX_NULL;
			}
			
			
		}

		
	

	if (myResult.x != 0.0) {
		myResult.x += float(worldPosInPixels.x > origin.x)*10.0;
	}

	

	return myResult;
}

// return vec2
// x: distance from line sp0->sp1
// y: percentage between sp0->sp1 (0.0->1.0)

vec2 pointSegDistance(vec3 testPoint, vec3 sp0, vec3 sp1)
{
	vec3 v = sp1 - sp0;
	vec3 w = testPoint - sp0;

	float d0 = distance(testPoint, sp0);
	float d1 = distance(testPoint, sp1);
	float d2 = distance(sp0,sp1);

	float c1 = dot(w,v);
	if ( c1 <= 0 ) {
		return vec2(d0, 0.0);
	}

	float c2 = dot(v,v);
	if ( c2 <= c1 ) {
		return vec2(d1, 1.0);
	}
	
	float b = c1 / c2;
	vec3 testPoint2 = sp0 + b * v; // testPoint2 is the nearest point on the line
	return vec2(distance(testPoint, testPoint2), distance(testPoint2,sp0)/d2 );
}


vec4 getTree(vec3 worldPosInPixels) {
	

	int i;
	int j;
	int k;

	int curInd = -1;

	int maxCount = 0;
	int baseInd = 0;
	
	
	float testDis = 0.0;
	float t;


	vec3 visMinInPixels = vec3(0.0);
	vec3 visMaxInPixels = vec3(0.0);
	vec3 matParams = vec3(0.0);

	vec4 matResult = vec4(0.0);

	vec3 p0;
	vec3 p1;
	vec3 p2;

	vec2 dres = vec2(0.0);
	vec2 dres1 = vec2(0.0);
	vec2 dres2 = vec2(0.0);

	
	vec3 tempv = vec3(0.0);

	vec3 roThetaPhi = vec3(0.0);
	vec3 roThetaPhi2 = vec3(0.0);


	vec3 bezTanP0;
	vec3 bezTanP1;

	vec4 tempResult = vec4(0.0);


	int bestInd = 0;
	vec2 bestRes = vec2(invalid);
	vec2 resArr = vec2(invalid);
	vec3 thickVals = vec3(0.0);

	float curThickness = 0.0;
	float sphereRad = 0.0;

	// p0: start point
	// p1: end point
	// p2: control point
	//      p2
	// p0        p1

	float totCount = 0.0;

	float leafRad = 8.0f;//0.5*pixelsPerMeter;

	vec3 maxv = vec3(invalid);

	vec3 newWP = vec3(0.0);

	for (i = 0; i < numEntries; i++) {

		baseInd = i*paramsPerEntry;
		matParams = paramArr[baseInd+E_TP_MATPARAMS];


		if (matParams.x == E_MAT_PARAM_TREE) {
			
			visMinInPixels = paramArr[baseInd+E_TP_VISMININPIXELST];
			visMaxInPixels = paramArr[baseInd+E_TP_VISMAXINPIXELST];
			

			if (
				all( lessThanEqual(worldPosInPixels,visMaxInPixels) ) && //+slack
				all( greaterThanEqual(worldPosInPixels,visMinInPixels) ) //-slack
			) {

				p0 = paramArr[baseInd+E_TP_P0];
				p1 = paramArr[baseInd+E_TP_P1];
				p2 = paramArr[baseInd+E_TP_P2];
				thickVals = paramArr[baseInd+E_TP_THICKVALS];

				sphereRad = thickVals.z;

				dres = pointSegDistance(worldPosInPixels,p0,p1);
				//dres2 = pointSegDistance(worldPosInPixels,p0,p2);
				//dres = mix(dres2, dres1, mix(dres1.y,dres2.y,0.5) );

				//if (dres.x < bestRes.x) {

					t =  dres.y;
					bezTanP0 = mix(p0,p2,t);
					bezTanP1 = mix(p2,p1,t);
					resArr = pointSegDistance(worldPosInPixels,bezTanP0,bezTanP1);

					curThickness = mix(thickVals.x,thickVals.y,t);

					if (resArr.x < curThickness + 0.0625*0.25*pixelsPerMeter*sin( (worldPosInPixels.x + worldPosInPixels.z +worldPosInPixels.y)/(0.125*pixelsPerMeter) ) ) {

						if (
							(abs(resArr.x - curThickness) < 0.125*pixelsPerMeter) &&
							(matResult.y != TEX_TREEWOOD)
						) {
							matResult.y = TEX_BARK;
						}
						else {
							matResult.y = TEX_TREEWOOD;
						}

						

						
						totCount = max( clamp(1.0-(resArr.x/curThickness),0.0,1.0), totCount);//abs( sin(dres.x/(curThickness*0.25)) );
						

						//totCount += 1.0;

						// bestRes = dres;
						// bestInd = baseInd;
					}

					
					


					//roThetaPhi2 = += roThetaPhi;


					
					
					if ( (distance(worldPosInPixels,p1) < sphereRad)&&(p1.z < maxv.z) ) { //

						// newWP = floor((worldPosInPixels)/leafRad)*leafRad + leafRad*0.5;
						// if (distance(worldPosInPixels.xy,newWP.xy) < leafRad*0.4 ) {
						// 	matResult.y = TEX_GRASS;
						// }

						maxv = p1;
						
						// if (distance(roThetaPhi.yz,roThetaPhi2.yz) < 0.3/leafRad ) {
						// 	matResult.y = TEX_GRASS;

						// }

						

						
					}

				//}				
			}
		}
	}

	if (maxv.z != invalid) {
		tempv = worldPosInPixels-maxv;

		roThetaPhi.z = sphereRad-length(tempv);
		roThetaPhi.x = atan(tempv.y,tempv.x);
		roThetaPhi.y = mix(-0.35,-0.5,randf(maxv.xy))*acos(tempv.z/length(tempv));

		tempv = getShingle(roThetaPhi*1024.0/PI, 1.0*pixelsPerMeter, roThetaPhi.z, 1.0*pixelsPerMeter*( (0.95-normalize(tempv).z) ) );

		if ( (matResult.y == TEX_NULL) && (tempv.y != TEX_NULL) ) {
			matResult.y = TEX_LEAF;
			matResult.w = (1.0-shingleMod); // *(1.0-abs(sin(roThetaPhi.z*PI*4.0/sphereRad)));
		}
	}

	


	if (matResult.y == TEX_TREEWOOD) {
		matResult.w = abs( sin(totCount*8.0) );
	}


	// if (matResult.y == TEX_BARK) {
	// 	tempResult = getCobble(worldPosInPixels*vec3(1.0,1.0,0.125), 0.5*pixelsPerMeter, vec3(0.0), 16000.0, 16000.0, false, false, false, 0, 27);
		
	// 	matResult.y = tempResult.y;
	// 	if (tempResult.y != TEX_NULL) {
	// 		//matResult.x = tempResult.x;
	// 		matResult.y = TEX_BARK;
	// 		matResult.w = tempResult.z;
	// 	}
		
		
	// }






	/*
	for (i = 0; i < numEntries; i++) {

		baseInd = i*paramsPerEntry;
		matParams = paramArr[baseInd+E_TP_MATPARAMS];


		if (matParams.x == E_MAT_PARAM_TREE) {
			
			visMinInPixels = paramArr[baseInd+E_TP_VISMININPIXELST];
			visMaxInPixels = paramArr[baseInd+E_TP_VISMAXINPIXELST];
			

			if (
				all( lessThanEqual(worldPosInPixels,visMaxInPixels) ) && //+slack
				all( greaterThanEqual(worldPosInPixels,visMinInPixels) ) //-slack
			) {

				p0 = paramArr[baseInd+E_TP_P0];
				p1 = paramArr[baseInd+E_TP_P1];
				p2 = paramArr[baseInd+E_TP_P2];

				dres = pointSegDistance(worldPosInPixels,p0,p1);				

				if (dres.x < bestRes[0].x) {
					bestRes[1] = bestRes[0];
					bestRes[0] = dres;

					bestInd[1] = bestInd[0];
					bestInd[0] = baseInd;

				}
				else {
					if (dres.x < bestRes[1].x) {
						bestRes[1] = dres;
						bestInd[1] = baseInd;
					}
				}
				
			}
		}
	}

	maxCount = 2;
	if (bestRes[0] == invalid) {
		maxCount = 0;
	}
	else {
		if (bestRes[1] == invalid) {
			maxCount = 1;
		}
	}
	


	for (i = 0; i < maxCount; i++) {
		baseInd = bestInd[i];

		p0 = paramArr[baseInd+E_TP_P0];
		p1 = paramArr[baseInd+E_TP_P1];
		p2 = paramArr[baseInd+E_TP_P2];
		thickVals[i] = paramArr[baseInd+E_TP_THICKVALS];


		t =  bestRes[i].y;

		bezTanP0 = mix(p0,p2,t);
		bezTanP1 = mix(p2,p1,t);

		resArr[i] = pointSegDistance(worldPosInPixels,bezTanP0,bezTanP1);
	}

	if (maxCount == 0) {
		curInd = -1;
	}
	if (maxCount == 1) {
		curInd = 0;
	}
	if (maxCount == 2) {
		if (resArr[0].x < resArr[1].x) {
			curInd = 0;
		}
		else {
			curInd = 1;
		}
	}

	if (resArr[curInd].x < mix(thickVals[curInd].x,thickVals[curInd].y,bestRes[curInd].y)) {
		matResult.y = TEX_STONE;
	}
	*/






	// matResult.x = normalUID;
	// matResult.y = finalMat;
	// matResult.z = float(isInside[0] || isInside[1]);
	// matResult.w = finalMod;

	return matResult;
}

vec4 getGeom(vec3 worldPosInPixels, float curMat, float terHeight) {


	int i;
	int j;
	int k;
	int n;
	int nMax = 1;

	int baseInd;
	int bestInd[2];
	int tallestInd = 0;
	bestInd[0] = -1;
	bestInd[1] = -1;// = int[](-1,-1);
	int bestI = 0;

	float bestHeight[2];
	bestHeight[0] = 0.0;
	bestHeight[1] = 0.0;

	
	float counter = 0.0;
	float m = 0.0;
	float resXY = 0.0;
	float res = 0.0;
	float resNoRef = 0.0;
	float resNoRefVisDis = 0.0;
	float resMin = 0.0;
	float resMax = 0.0;
	float visDis = 0.0;
	float visDisXY = 0.0;
	float tempf1 = 0.0;
	float tempf2 = 0.0;
	float curDis = 0.0;
	float curRad = 0.0;
	float curThe = 0.0;
	float curPhi = 0.0;
	float airVal = 0.0;
	float normalUID = 0.0;
	float thickness = 0.0;
	float wThickness = 0.0;
	float disCenterMinMax = 0.0;
	
	//float nearestJoint = invalid;
	float bestDis = invalid;
	float nextBestDis = invalid;
	float testDis = 0.0;

	

	float nearestJointXY = invalid;
	float bestDisXY = invalid;
	float nextBestDisXY = invalid;
	float testDisXY = 0.0;
	float testHeight = 0.0;
	float tallestHeight = 0.0;

	float finalMod = 0.0;
	float finalMat = TEX_NULL;
	
	float maxDisInPixels = 0.0;
	float disInPixels = 0.0;
	float sqrt3 = sqrt(3.0);
	float resXYInside = 0.0;
	float resInside = 0.0;
	float woodRes = 0.0;

	float divDis = 0.0625*pixelsPerMeter;

	bool doProc = false;

	bool isRoof[2];
	isRoof[0] = false;
	isRoof[1] = false;

	bool isInside[2];
	isInside[0] = false;
	isInside[1] = false;

	// bool isTower[2];
	// isTower[0] = false;
	// isTower[1] = false;

	bool isTower = false;


	vec2 topVals = vec2(0.0);

	vec3 worldPosInPixelsOrig = worldPosInPixels;
	vec3 coefMin = vec3(0.0);
	vec3 coefMax = vec3(0.0);
	vec3 boundsMinInPixels = vec3(0.0);
	vec3 boundsMaxInPixels = vec3(0.0);
	vec3 visMinInPixels = vec3(0.0);
	vec3 visMaxInPixels = vec3(0.0);
	vec3 cornerDisInPixels = vec3(0.0);
	vec3 powerVals = vec3(0.0);
	vec3 powerVals1 = vec3(0.0);
	vec3 powerVals2 = vec3(0.0);
	vec3 thickVals = vec3(0.0);
	vec3 matParams = vec3(0.0);
	//vec3 coefficients = vec3(0.0);
	//vec3 squareVals = vec3(0.0);
	//vec3 minMaxMat1 = vec3(0.0);
	//vec3 minMaxMat2 = vec3(0.0);
	//vec3 dirFlags = vec3(0.0);

	vec3 newVisMinInPixels = vec3(0.0);
	vec3 newVisMaxInPixels = vec3(0.0);
	vec3 innerDisInPixels = vec3(0.0);

	vec3 originInPixels = vec3(0.0);
	vec3 dis = vec3(0.0);
	vec3 absDis = vec3(0.0);
	vec3 disNorm = vec3(0.0);
	vec3 absDisNorm = vec3(0.0);
	vec3 absDisNormalized = vec3(0.0);
	vec3 newCornerDis = vec3(0.0);
	vec3 radInPixels = vec3(0.0);
	
	vec3 boundsMinCorner = vec3(0.0);
	vec3 boundsMaxCorner = vec3(0.0);
	//vec3 slack = vec3(0.0,0.0,0.0)*pixelsPerMeter; //vec3(0.125,0.125,0.0)*pixelsPerMeter;//vec3(0.0,0.0,0.0)*pixelsPerMeter; //
	
	vec3 radInPixelsTopUV = vec3(0.0);
	vec3 newCornerDisTopUV = vec3(0.0);
	vec3 originInPixelsTopUV = vec3(0.0);
	vec3 disTopUV = vec3(0.0);
	vec3 disNormTopUV = vec3(0.0);
	vec3 absDisNormTopUV = vec3(0.0);

	vec4 matResult = vec4(0.0); //x: normalID, y: material
	


	vec3 centerMin = vec3(0.0);
	vec3 centerMax = vec3(0.0);

	vec3 tempVec = vec3(0.0);
	vec3 tempVec2 = vec3(0.0);
	vec3 tempVec3 = vec3(0.0);
	vec3 orig = vec3(0.0);
	vec3 origVis = vec3(0.0);
	vec3 normLen = vec3(0.0);
	vec3 eqRes = vec3(0.0);
	vec3 eqResMax = vec3(0.0);
	vec3 oneVec = vec3(1.0);
	vec3 zeroVec = vec3(0.0);
	vec4 newUVW = vec4(0.0);

	bvec3 bLessThanOrig;
	bool isVert;
	bool isDivider = false;
	bool isInsideCorners = false;
	bool testRes = false;
	bool thickInside = false;
	bool bestVert = false;
	bool nextBestVert = false;

	bool tempb = false;
	
	int visCount = 0;
	

	


	for (i = 0; i < numEntries; i++) {

		baseInd = i*paramsPerEntry;
		matParams = paramArr[baseInd+E_GP_MATPARAMS];

		if (matParams.x == curMat) {
			
			boundsMinInPixels = paramArr[baseInd+E_GP_BOUNDSMININPIXELST];
			boundsMaxInPixels = paramArr[baseInd+E_GP_BOUNDSMAXINPIXELST];
			visMinInPixels = paramArr[baseInd+E_GP_VISMININPIXELST];
			visMaxInPixels = paramArr[baseInd+E_GP_VISMAXINPIXELST];
			thickVals = paramArr[baseInd+E_GP_THICKVALS];

			if (thickVals.y != 0.0) {
				visMinInPixels.z = terHeight - thickVals.y;
				visMaxInPixels.z = terHeight + thickVals.y;

				boundsMinInPixels.z = visMinInPixels.z;
				boundsMaxInPixels.z = visMaxInPixels.z;
			}
			

			if (
				all( lessThanEqual(worldPosInPixels,visMaxInPixels) ) && //+slack
				all( greaterThanEqual(worldPosInPixels,visMinInPixels) ) //-slack
			) {

				
				orig = (boundsMaxInPixels + boundsMinInPixels)/2.0;
				//testDis = distance(worldPosInPixels.xy,orig.xy);
				testDisXY = distance(worldPosInPixels.xy,orig.xy);



				// radInPixelsTopUV = (boundsMaxInPixels - boundsMinInPixels)*0.5;
				// newCornerDisTopUV.xy = vec2(min(radInPixelsTopUV.x,radInPixelsTopUV.y));
				// newCornerDisTopUV.z = min(radInPixelsTopUV.z,newCornerDisTopUV.x);
				// centerMin = boundsMinInPixels + newCornerDisTopUV;
				// centerMax = boundsMaxInPixels - newCornerDisTopUV;
				// originInPixelsTopUV = clamp(worldPosInPixels, centerMin, centerMax );
				// testDis = distance(worldPosInPixels,originInPixelsTopUV);

				

				testHeight = boundsMaxInPixels.z - boundsMinInPixels.z;


				if (testDisXY < bestDisXY) {
					//nextBestDis = bestDis;
					//bestDis = testDis;
					
					nextBestVert = bestVert;
					bestVert = isVert;

					bestHeight[1] = bestHeight[0];
					bestHeight[0] = testHeight;

					bestInd[1] = bestInd[0];
					bestInd[0] = i;

					nextBestDisXY = bestDisXY;
					bestDisXY = testDisXY;
				}
				else {
					if (testDisXY < nextBestDisXY) {
						bestInd[1] = i;
						bestHeight[1] = testHeight;
						nextBestVert = isVert;
						nextBestDisXY = testDisXY;
					}
				}


				if (testHeight > tallestHeight) {
					tallestHeight = testHeight;
					tallestInd = i;
				}


				visCount++;

			}

			
		}

		worldPosInPixels = worldPosInPixelsOrig;
		
		
	}


	if ( (bestDisXY == invalid) || (nextBestDisXY == invalid) ) {
		//nearestJoint = invalid;
		nearestJointXY = invalid;
	}
	else {

		



		nMax = 1 + int(bestHeight[0] != bestHeight[1]);



		if (nMax == 1) {
			if (tallestHeight > bestHeight[0]) {
				nMax = 2;
				bestInd[1] = tallestInd;
				bestHeight[1] = tallestHeight;
			}
		}

		if (curMat == E_MAT_PARAM_ROAD || curMat == E_MAT_PARAM_DOCK) {
			nMax = 1;
		}


		// isTaller[0] = (bestHeight[0] > bestHeight[1]);
		// isTaller[1] = (bestHeight[1] > bestHeight[0]);


		// //bestHeight 1 is higher
		// if (bestHeight[0] > bestHeight[1]) {
		// 	tempf1 = bestHeight[0];
		// 	bestHeight[0] = bestHeight[1];
		// 	bestHeight[1] = tempf1;

		// 	i = bestInd[0];
		// 	bestInd[0] = bestInd[1];
		// 	bestInd[1] = i;
		// }

		//nearestJoint = abs(nextBestDis-bestDis);
		nearestJointXY = abs(nextBestDisXY-bestDisXY);

		if (bestVert == nextBestVert) {
			//nearestJoint = invalid;
			nearestJointXY = invalid;
		}
		
		

	}

	nMax = int(min(float(nMax),float(visCount)));



	for (n = 0; n < nMax; n++) {




		baseInd = bestInd[n]*paramsPerEntry;

		visMinInPixels = paramArr[baseInd+E_GP_VISMININPIXELST];
		visMaxInPixels = paramArr[baseInd+E_GP_VISMAXINPIXELST];

		
		

		// if (
		// 	all( lessThanEqual(worldPosInPixels,visMaxInPixels) ) &&
		// 	all( greaterThanEqual(worldPosInPixels,visMinInPixels) )
		// ) {}





		boundsMinInPixels = paramArr[baseInd+E_GP_BOUNDSMININPIXELST];
		boundsMaxInPixels = paramArr[baseInd+E_GP_BOUNDSMAXINPIXELST];
		cornerDisInPixels = paramArr[baseInd+E_GP_CORNERDISINPIXELS];
		powerVals1 = paramArr[baseInd+E_GP_POWERVALS];
		powerVals2 = paramArr[baseInd+E_GP_POWERVALS2];
		thickVals = paramArr[baseInd+E_GP_THICKVALS];
		matParams = paramArr[baseInd+E_GP_MATPARAMS];
		
		wThickness = thickVals.x;
		//thickness = thickVals.x;


		if (thickVals.y != 0.0) {
			visMinInPixels.z = terHeight - thickVals.y;
			visMaxInPixels.z = terHeight + thickVals.y;

			boundsMinInPixels.z = visMinInPixels.z;
			boundsMaxInPixels.z = visMaxInPixels.z;
		}
		

		tempVec = vec3(equal(visMinInPixels, boundsMinInPixels));
		tempVec2 = vec3(equal(visMaxInPixels, boundsMaxInPixels));

		newVisMinInPixels = visMinInPixels - tempVec*9999.0;
		newVisMaxInPixels = visMaxInPixels + tempVec2*9999.0;


		orig = (boundsMaxInPixels + boundsMinInPixels)/2.0;




		origVis = (newVisMaxInPixels + newVisMinInPixels)/2.0;
		radInPixels = (boundsMaxInPixels - boundsMinInPixels)/2.0;
		isVert = radInPixels.x < radInPixels.y;

		thickness = min(min(radInPixels.x,radInPixels.y),radInPixels.z);
		

		




		isTower = radInPixels.x == radInPixels.y;

		if (isTower) {
			nearestJointXY = invalid;
		}


		radInPixelsTopUV = (boundsMaxInPixels - boundsMinInPixels)*0.5;// - cornerDisInPixels;
		innerDisInPixels = radInPixels-cornerDisInPixels;


		boundsMinCorner = boundsMinInPixels + cornerDisInPixels;
		boundsMaxCorner = boundsMaxInPixels - cornerDisInPixels;


		newCornerDisTopUV.xy = vec2(min(radInPixelsTopUV.x,radInPixelsTopUV.y));
		newCornerDisTopUV.z = min(radInPixelsTopUV.z,newCornerDisTopUV.x);

		tempVec = abs( abs(newVisMaxInPixels - origVis) - abs(worldPosInPixels - origVis) );
		visDis = 1.0-clamp( (min(tempVec.x,tempVec.y),tempVec.z)/thickness, 0.0, 1.0);
		visDisXY = 1.0-clamp( min(tempVec.x,tempVec.y)/thickness, 0.0, 1.0);

		

		centerMin = boundsMinInPixels + newCornerDisTopUV;
		centerMax = boundsMaxInPixels - newCornerDisTopUV;

		if (isVert) {

			tempf1 = clamp( (worldPosInPixels.y-centerMin.y)/(centerMax.y-centerMin.y), 0.0, 1.0);

			// if ( worldPosInPixels.y > orig.y ) {
			// 	powerVals.x = powerVals.z;
			// }
		}
		else {

			tempf1 = clamp( (worldPosInPixels.x-centerMin.x)/(centerMax.x-centerMin.x), 0.0, 1.0);

			// if ( worldPosInPixels.x > orig.x ) {
			// 	powerVals.x = powerVals.z;
			// }
		}

		powerVals = mix(powerVals1,powerVals2,tempf1);

		originInPixelsTopUV = clamp(worldPosInPixels, centerMin, centerMax );
		disTopUV = worldPosInPixels - originInPixelsTopUV;
		disNormTopUV = (disTopUV/newCornerDisTopUV);
		absDisNormTopUV = abs( disNormTopUV );

		disCenterMinMax = distance(originInPixelsTopUV.xy,centerMin.xy); // /distance(centerMin.xy,centerMax.xy);


		originInPixels = clamp(worldPosInPixels, boundsMinCorner, boundsMaxCorner );
		bLessThanOrig = lessThanEqual( abs(worldPosInPixels-orig), abs(originInPixels-orig) );
		isInsideCorners = all(bLessThanOrig.xyz);


		dis = worldPosInPixels - originInPixels;
		absDis = abs(dis);
		disNorm = (dis/cornerDisInPixels);
		absDisNorm = abs( disNorm );
		absDisNormalized = normalize(absDisNorm);

					

		

		

		// cornerdis must be less than thickness


		tempVec = cornerDisInPixels-thickness;
		thickInside = any(lessThanEqual(tempVec,zeroVec));
		tempVec = max(tempVec,oneVec);
		coefMin = tempVec;
		coefMax = cornerDisInPixels;

		
		






		
		curRad = length(disNorm);
		curPhi = atan(disNorm.y, disNorm.x);
		curThe = acos(disNorm.z/curRad);
		newUVW = vec4(curPhi/PIO2,1.0-curThe/PIO2,0.0,0.0);
		newUVW.x *= (cornerDisInPixels.x+cornerDisInPixels.y);
		newUVW.y *= (cornerDisInPixels.z) + (cornerDisInPixels.x+cornerDisInPixels.y)/2.0;

		tempVec3 = worldPosInPixels - boundsMinInPixels;


		if ( all(bLessThanOrig.xy) ) {

			bestDis = -99.0;
			for (i = 0; i < 4; i++) {
				testDis = dot(normalize(disNormTopUV.xy), dirVecs[i]);
				if (testDis > bestDis) {
					bestDis = testDis;
					topVals.xy = tempVec3.xy * dirMats[i];
				}
			}



			if (bLessThanOrig.z) {

				bestDis = -99.0;
				for (i = 0; i < 6; i++) {
					testDis = dot(normalize(disNormTopUV), dirVecs2[i]);
					if (testDis > bestDis) {
						bestDis = testDis;
						bestI = i;
					}
				}

				if (bestI >= 4) {
					newUVW.xy = topVals;
				}
				else {
					if (bestI <= 1) {
						newUVW.xy = tempVec3.yz;
						//newUVW.y += uvScale.y/2.0;
					}
					else {
						newUVW.xy = tempVec3.xz;
						//newUVW.y += uvScale.y/2.0;
					}
				}


			}
			else {
				newUVW.xy = topVals;

				isDivider = (abs( abs(disTopUV.x) - abs(disTopUV.y) ) <= divDis) && (abs(disTopUV.x) + abs(disTopUV.y) > divDis*2.0 ) ;

			}


		}
		else {
			
			

			if ( all(bLessThanOrig.yz) ) {
				newUVW.xy = tempVec3.yz;
			}
			else {
				if ( all(bLessThanOrig.xz) ) {
					newUVW.xy = tempVec3.xz;
				}
				else {

					if (bLessThanOrig.x) {
						newUVW.x = tempVec3.x;
					}
					if (bLessThanOrig.y) {
						newUVW.x = tempVec3.y;
					}
					if (bLessThanOrig.z) {
						newUVW.y = tempVec3.z;
					}

				}
			}



		}
		

		tempf1 = clamp(
			min(
				min(
					abs(originInPixels.x-boundsMinInPixels.x),
					abs(boundsMaxInPixels.x-originInPixels.x)
				),
				min(
					abs(originInPixels.y-boundsMinInPixels.y),
					abs(boundsMaxInPixels.y-originInPixels.y)
				)
			),
			0.0,
			thickness
		);

		tempf2 = clamp(
			min(
				tempf1,
				min(
					abs(originInPixels.z-boundsMinInPixels.z),
					abs(boundsMaxInPixels.z-originInPixels.z)
				)
			),
			0.0,
			thickness
		);
		resXYInside = 1.0-tempf1/thickness;
		resInside = 1.0-tempf2/thickness;


		if (thickInside && all(bLessThanOrig.xy) ) {
			resXY = resXYInside;
		}
		else {
			tempVec = absDis/coefMin;
			resMin = pow(dot(pow(tempVec.xy,powerVals.xx),oneVec.xy),1.0/powerVals.x);
			tempVec = absDis/coefMax;
			resMax = pow(dot(pow(tempVec.xy,powerVals.xx),oneVec.xy),1.0/powerVals.x);
			resXY = clamp(  ( 1.0-(1.0/resMin) ) / ( (1.0/resMax)-(1.0/resMin) ), 0.0, 1.0  );

			resXY = resXY*(1.0-resXYInside) + resXYInside;
		}

		if (thickInside&&isInsideCorners) {
			res = resInside;
		}
		else {
			tempVec = absDis/coefMin;
			tempVec2.x = pow(dot(pow(tempVec.xy,powerVals.xx),oneVec.xy),1.0/powerVals.x);
			tempVec2.y = tempVec.z;
			resMin = pow(dot(pow(tempVec2.xy,powerVals.yy),oneVec.xy),1.0/powerVals.y);

			tempVec = absDis/coefMax;
			tempVec2.x = pow(dot(pow(tempVec.xy,powerVals.xx),oneVec.xy),1.0/powerVals.x);
			tempVec2.y = tempVec.z;
			resMax = pow(dot(pow(tempVec2.xy,powerVals.yy),oneVec.xy),1.0/powerVals.y);

			res = ( 1.0-(1.0/resMin) ) / ( (1.0/resMax)-(1.0/resMin) );

			res = res*(1.0-resInside) + resInside;

		}

		
		


		




		resNoRef = clamp(res,0.0,1.0);

		resNoRefVisDis = max(resNoRef,visDis);

		res = abs(resNoRef-0.5)*2.0;
		res = max(res,visDis);
		
		//resNoRef = max(resNoRef,visDis);
		
		testRes = ((resMin >= 1.0) || thickInside) && (resMax <= 1.0);


		
		tempVec3 = worldPosInPixels-boundsMinCorner;
		tempVec3.xy = abs(tempVec3.xy-(boundsMaxCorner.xy-boundsMinCorner.xy)/2.0);



		//newUVW.xy = intModFV(newUVW.xyz,uvScale).xy;
		//newUVW.xy += uvScale.xy * vec2(lessThan(newUVW.xy,zeroVec.xy));
		newUVW.z = resNoRef*thickness;
		newUVW.w = res*thickness;

		isInside[n] = testRes;
		isRoof[n] = !bLessThanOrig.z;

		if ( testRes ) {// && (airVal != 1.0)  - isRidge.x/8.0 - isRidge.y/8.0 ) ) {

			

			if ( (matParams.x == E_MAT_PARAM_BUILDING) || (matParams.x == E_MAT_PARAM_DOCK) ) {

				

				////////////////////////////////////
				// BOARD MAT
				////////////////////////////////////


				// if (newUVW.z < visDisXY*thickness) {
				// 	newUVW.x = newUVW.z;
				// 	newUVW.z = visDisXY*thickness;

				// }
				// else {

				// }

				


				matResult = getBuilding(
					newUVW,
					tempVec3,
					bLessThanOrig,
					isVert,
					(matParams.x == E_MAT_PARAM_BUILDING), // notScaff
					boundsMaxCorner.z-boundsMinCorner.z,
					nearestJointXY,
					resXY,
					thickness,
					disCenterMinMax,
					visDisXY
				);

				isInside[n] = bool(matResult.z);

				doProc = false;


				if (n == 0) {
					if (isRoof[0] || isTower){

					}
					else {
						n++;
					}
					doProc = true;
				}
				else {





					if (isInside[1]) {
						
						doProc = true;

						if ( isRoof[1] && isInside[0] ) {
							doProc = false;
						}

					}
					else {
						doProc = false;
					}

					if (
						isInside[0] &&
						isInside[1] &&
						(isRoof[0]||isRoof[1]) &&
						(matResult.y == TEX_PLASTER || matResult.y == TEX_MORTAR || matResult.y == TEX_BRICK)
					) {
						doProc = true;
						matResult.x = 0.0;
						matResult.y = TEX_NULL;
					}

					// if (finalMat == TEX_SHINGLE) {
					// 	finalMat = TEX_NULL;
					// }
					// if (matResult.y == TEX_SHINGLE) {
					// 	doProc = false;
					// }
			
				}

				if (doProc) {
					normalUID = matResult.x;
					finalMat = matResult.y;
					woodRes = matResult.w;

					
					if (finalMat == TEX_SHINGLE) {
						finalMat += matParams.y;
					}
				}
				


			}
			if (matParams.x == E_MAT_PARAM_ROAD) {

				//TODO: terCenter = terrain orig
				
				matResult = getCobble(worldPosInPixels, 0.5*pixelsPerMeter, orig, 1000.0, 4000.0, isVert, true, true, 9, 18);

				normalUID = matResult.x;
				finalMat = matResult.y;
				finalMod = matResult.z;


				////////////////////////////////////
				// BRICK MAT
				////////////////////////////////////

				// normalUID = getBoardSpaced(newUVW.xyz, vec3(1.0,0.5,0.5), 0.125 );
				// if (  (normalUID != 0.0) &&  (nearestJointXY > divDis) && (!isDivider)   ) {
				// 	finalMat = TEX_BRICK;
				// }
				// else {

				// 	finalMat = mix(TEX_NULL, TEX_MORTAR, float( res*thickness < (thickness-pixelsPerMeter/8.0) ) );

					
				// }



				isInside[0] = finalMat != TEX_NULL;

			}

			tempb = (matParams.x == E_MAT_PARAM_SLATS);

			if ((matParams.x == E_MAT_PARAM_DOORWAY) || tempb) {
				

				if ( (resNoRefVisDis*thickness) > (thickness - wThickness) ) {
					tempf1 = float(worldPosInPixels.z-boundsMinCorner.z > 0.25*pixelsPerMeter);
					finalMat = mix(
						mix(TEX_WOOD,TEX_METAL,float(tempb)*matParams.y),
						mix(TEX_BRICK,TEX_WOOD,matParams.y),
						float(matParams.x == E_MAT_PARAM_DOORWAY)
					);

					
					woodRes = mix(0.0 + float(isVert), 2.0, tempf1);
					normalUID = 24.0 + float(bLessThanOrig.z) + tempf1 + 4.0*float(tempb);

				}
				else {

					if (tempb) {
						matResult = getSlats(newUVW, thickness, orig, worldPosInPixels, visMinInPixels, visMaxInPixels, isVert, matParams.y);
						normalUID = matResult.x;
						finalMat = matResult.y;
						woodRes = matResult.w;
					}
					else {
						finalMat = TEX_NULL;
					}


					
				}


				if (finalMat == TEX_BRICK) {
					normalUID = getBoardSpaced(newUVW.xyz, vec3(1.0,0.5,0.5), 0.125 );
					if ( normalUID != 0.0 ) {
						finalMat = TEX_BRICK;
					}
					else {
						finalMat = mix(TEX_NULL, TEX_MORTAR, float( resNoRef*thickness < (thickness-pixelsPerMeter/32.0) ) );
					}
				}
				
				
			}

			if (matParams.x == E_MAT_PARAM_LANTERN) {

				if (  all(bLessThanOrig.xz) || all(bLessThanOrig.yz) ) {
					finalMat = TEX_GLASS;
				}
				else {
					finalMat = TEX_METAL;
					normalUID = 20.0;
				}

				
			}

			// if (matParams.x == E_MAT_PARAM_SLATS) {
				
			// 	finalMat = TEX_WOOD;
				
			// 	matResult = getSlats(newUVW, worldPosInPixels,visMinInPixels,visMaxInPixels, isVert);
			// 	normalUID = matResult.x;
			// 	finalMat = matResult.y;
			// 	woodRes = matResult.w;

			// 	// isInside[0] = true;

			// 	// woodRes = mix(0.0 + float(isVert), 2.0, tempf1);
			// 	// normalUID = 24.0 + float(bLessThanOrig.z) + tempf1;

			// }


		}

		// todo: move this to main function
		if (finalMat == TEX_WOOD) {
			finalMod = getWoodGrain(normalUID, worldPosInPixels, 2.0*pixelsPerMeter, woodRes, 4.0 );
		}

		// if (finalMat == TEX_METAL) {
		// 	finalMod = getWoodGrain(normalUID, worldPosInPixels, 2.0*pixelsPerMeter, 3.0, 2.0 );
		// }


		
		
		

	}


	matResult.x = normalUID;
	matResult.y = finalMat;
	matResult.z = float(isInside[0] || isInside[1]);
	matResult.w = finalMod;

	return matResult;

}






void main() {
	
	
	int i;
	int j;

	
	shingleMod = 0.0;
	
	vec3 newCoords = vec3(0.0);
	j = int( volumePitch*volumePitch*TexCoord0.y );
	newCoords.x = TexCoord0.x;
	newCoords.y = float(intMod(j,iVolumePitch))/volumePitch;
	newCoords.z = float(j/iVolumePitch)/volumePitch;
	vec3 worldPosInPixels = vec3(
		mix(worldMinBufInPixels.x, worldMaxBufInPixels.x, newCoords.x),
		mix(worldMinBufInPixels.y, worldMaxBufInPixels.y, newCoords.y),
		mix(worldMinBufInPixels.z, worldMaxBufInPixels.z, newCoords.z)
	);

	float randVal = randf(TexCoord0);
	float finalMod = 0.0;
	float finalMat = TEX_NULL;
	float finalNormUID = 0.0;
	float finalInside = 0.0;
	float finalLayer = 0.0;
	float fj;

	int iMatCount = 0;

	vec4 finalRes = vec4(0.0);
	vec4 matResultTer = vec4(0.0);
	vec4 matResultTree = vec4(0.0);
	vec4 matResultGeom = vec4(0.0);
	vec4 matResult = vec4(0.0);
	vec4 tempResult;

	
	if (hasTerrain) {
		matResultTer = getTerrain(worldPosInPixels);
	}

	if (hasGeom) {

		for (j = 0; j < E_MAT_PARAM_TREE; j++) {
			fj = float(j);

			iMatCount = int(matCountArr[j]);

			if (iMatCount > 0) {
				tempResult = getGeom(worldPosInPixels, fj, matResultTer.w-0.75*pixelsPerMeter );
				if ( (tempResult.z == 1.0) || (tempResult.y != TEX_NULL) ) {
					matResultGeom = tempResult;
				}
			}
		}
	}





	


	if ( (matResultTer.y != TEX_NULL) && (matResultGeom.z != 1.0) ) {
		matResult = matResultTer;
	}
	else {
		matResult = matResultGeom;
	}

	if (hasTree) {
		matResultTree = getTree(worldPosInPixels);
		if (matResultTree.y != TEX_NULL) {
			matResult = matResultTree;
		}

	}


	finalInside = matResult.z;
	finalNormUID = matResult.x;
	finalMat = matResult.y;
	finalMod = matResult.w;


	if (finalMat == TEX_MORTAR) {
		finalNormUID = 0.0;
	}


	// if (isInside[0] || isInside[1]) {

	// }
	// else {
	// 	matResult = getTerrain(worldPosInPixels, randVal);
	// 	finalMod = matResult.x;
	// 	if (matResult.y != TEX_NULL) {
	// 		finalMat = matResult.y;
	// 	}
		
	// }


	vec4 tex2 =  texture3D(Texture1, newCoords);
	if (tex2.a > 0.5) {
		finalMat = TEX_DIRT;
	}
	else {
		if (tex2.b > 0.5) {
			finalMat = TEX_NULL;
		}
	}

	// if (finalMat == TEX_BRICK || finalMat == TEX_PLASTER || finalMat == TEX_MORTAR) {
	// 	finalMat = TEX_GLASS;
	// }

	if (finalMat == TEX_MORTAR) {
		finalMat = TEX_SAND;
	}

	if (finalMat == TEX_SAND || finalMat == TEX_MORTAR || finalMat == TEX_BRICK || finalMat == TEX_METAL) { // || finalMat == TEX_GRASS
		finalMod = randVal;
	}

	if (finalMat == TEX_GRASS) {
		finalMod = randVal*0.25+0.5;
	}

	if ( (finalMat >= TEX_SHINGLE) && (finalMat < TEX_PLASTER) ) {
		finalMod = clamp(shingleMod,0.0,1.0);
	}

	
	if (finalMat == TEX_DIRT) {
		fj = clamp((matResultTer.w-worldPosInPixels.z)/512.0,0.0,3.0);
		finalMat = TEX_EARTH + floor(fj);
		finalMod = fj-floor(fj);
	}


	if ((finalMat == TEX_NULL) && (worldPosInPixels.z <= seaLevel) && (finalInside == 0.0)) {
		finalLayer = 1.0;
		finalMat = TEX_WATER;
		finalNormUID = 254.0;
		finalMod = 0.0;
	}

	// if ((finalMat == TEX_NULL)) {
	// 	finalMat = TEX_STONE;
	// 	finalNormUID = 254.0;
	// 	finalMod = 0.0;
	// }


	if (finalMat == TEX_GLASS) {
		finalLayer = 1.0;
		finalNormUID = 254.0;
		finalMod = 0.0;
	}

	finalLayer = min(finalLayer,totLayers-1.0);
	
	finalRes.a = finalMat/255.0;

	// if (directPass==1.0) {
	// 	finalRes.rgb = vec3(1.0);
	// }
	// else {
		
	// }

	finalRes.r = finalLayer;
	finalRes.b = (finalMod*127.0)/255.0;
	finalRes.g = finalNormUID/255.0;
	
	

	gl_FragData[0] = finalRes;


}

