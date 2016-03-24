// f00324_gamevoxelwrap.h
//

#include "f00324_gamevoxelwrap.e"
#define LZZ_INLINE inline
GameVoxelWrap::GameVoxelWrap ()
                        {
		lastFFSteps = 0;
		voxelBuffer = NULL;
		basePD = NULL;
		baseData = NULL;
		
		oneVec = vec3(1.0f,1.0f,1.0f);
		halfOff = vec3(0.5,0.5,0.5);
		crand0 = vec3(12.989, 78.233, 98.422);
		crand1 = vec3(93.989, 67.345, 54.256);
		crand2 = vec3(43.332, 93.532, 43.734);
	}
void GameVoxelWrap::init (Singleton * _singleton)
          {
		singleton = _singleton;
		//dimInVoxels = _dimInVoxels;
		// octInVoxels = dimInVoxels*2;
		
		// octOffsetInVoxels = ivec3(
		// 	(octInVoxels-dimInVoxels)/2,
		// 	(octInVoxels-dimInVoxels)/2,
		// 	(octInVoxels-dimInVoxels)/2	
		// );
		
		voxelsPerCell = singleton->voxelsPerCell;
		//fVoxelsPerCell = voxelsPerCell;
		cellsPerHolder = singleton->cellsPerHolder;
		cellsPerHolderPad = singleton->cellsPerHolderPad;
		paddingInCells = singleton->paddingInCells;
		paddingInVoxels = paddingInCells*voxelsPerCell;
		
		voxelsPerHolderPad = singleton->voxelsPerHolderPad;
		voxelsPerHolder = singleton->voxelsPerHolder;
		
		
		
	}
void GameVoxelWrap::fillVec (GamePageHolder * gph)
                                          {
		int totSize = voxelBuffer->visitIds.size();
		
		if (totSize <= 0) {
			return;
		}
		
		int q;
		int p;
		float fVPC = voxelsPerCell;
		fVPC = 1.0f/fVPC;
		
		ivec3 voxOffset;
		ivec3 cellOffset;
		ivec3 localVoxOffset;
		
		//vec3 fLocalVoxOffset;
		vec3 fVO;
		
		
		int ii;
		int jj;
		int kk;
		int ind;
		
		int xx;
		int yy;
		int zz;
		
		uint curFlags;
		uint tempFlags;
		uint normFlags;
		
		float tempData[16];
		
		CubeWrap* curCW;
		
		int tempInd;
		
		vec3 totNorm;
		
		for (p = 0; p < totSize; p++) {
			q = voxelBuffer->visitIds[p];
			kk = q/(voxelsPerHolderPad*voxelsPerHolderPad);
			jj = (q-kk*voxelsPerHolderPad*voxelsPerHolderPad)/voxelsPerHolderPad;
			ii = q-(kk*voxelsPerHolderPad*voxelsPerHolderPad + jj*voxelsPerHolderPad);
			
			if (voxelBuffer->getFlag(q,E_OCT_SURFACE)) {
				
				voxOffset.x = ii;
				voxOffset.y = jj;
				voxOffset.z = kk;
				
				voxOffset -= paddingInVoxels;
				if (inBounds(&voxOffset,0,voxelsPerHolder)) {
					
					
					curFlags = voxelBuffer->getFlags(q);
					
					totNorm.set(0.0f,0.0f,0.0f);
					
					for (zz = -NORM_RAD; zz <= NORM_RAD; zz++) {
						for (yy = -NORM_RAD; yy <= NORM_RAD; yy++) {
							for (xx = -NORM_RAD; xx <= NORM_RAD; xx++) {
								
								
								
								// if (
								// 	(zz == 0) && (yy == 0) && (xx == 0)	
								// ) {
									
								// }
								// else {
									
								// }
								
								tempInd = (zz+kk)*voxelsPerHolderPad*voxelsPerHolderPad + (yy+jj)*voxelsPerHolderPad + (xx + ii);
								tempFlags = voxelBuffer->getFlagsAtNode(tempInd);
								
								if ((tempFlags&E_OCT_SURFACE) > 0) {
									normFlags = (tempFlags&63);
									totNorm += BASE_NORMALS[normFlags];
								}
								
								
							}
						}
					}
					
					totNorm.normalize();
					
					voxOffset += paddingInVoxels;
					voxOffset += offsetInVoxels;
					
					fVO.x = voxOffset.x;
					fVO.y = voxOffset.y;
					fVO.z = voxOffset.z;
					fVO *= fVPC;
					
					tempData[0] = totNorm.x;
					tempData[1] = totNorm.y;
					tempData[2] = totNorm.z;
					
					gph->vboWrapper.vboBox(
						fVO.x, fVO.y, fVO.z,
						0.0f,fVPC,
						curFlags,
						tempData,
						4
					);
					
				}
				
				// if (inBounds(&voxOffset,0,voxelsPerHolder)) {
				// 	cellOffset = voxOffset/voxelsPerCell;
				// 	localVoxOffset = voxOffset-(cellOffset*voxelsPerCell);
				// 	//fLocalVoxOffset = toVEC(localVoxOffset);
				// 	//fLocalVoxOffset *= fVPC;
					
				// 	ind = cellOffset.z*cellsPerHolder*cellsPerHolder + cellOffset.y*cellsPerHolder + cellOffset.x;
					
				// 	if (gph->cubeData[ind] == CUBE_DATA_INVALID) {
				// 		gph->cubeWraps.push_back(CubeWrap());
				// 		gph->cubeData[ind] = (gph->cubeWraps.size()-1);
				// 		gph->cubeWraps[gph->cubeData[ind]].init();
				// 	}
					
				// 	curCW = &(gph->cubeWraps[gph->cubeData[ind]]);
					
				// 	curCW->insertValue(&localVoxOffset,voxelBuffer->getFlags(q));//, &fLocalVoxOffset);
				// }
				
				
				
				
				
				
				
				
				
				
				
				
				
			}
			
		}
	}
void GameVoxelWrap::process (GamePageHolder * gph)
                                          {
		ivec3 cellCoord;
		ivec3 voxResult;
		
		curPD = gph->curPD;
		
		offsetInCells.set(
			gph->gphMinInCells.getIX(),
			gph->gphMinInCells.getIY(),
			gph->gphMinInCells.getIZ()
		);
		
		offsetInVoxels = offsetInCells;
		offsetInVoxels *= voxelsPerCell;
		

		basePD = (&singleton->pdPool[curPD]);
		baseData = singleton->pdPool[curPD].data;
		
		voxelBuffer = &(basePD->voxelBuffer);
		voxelBuffer->clearAllNodes();
		
		while( findNextCoord(&voxResult) ) {
			floodFill(voxResult);
			goto DONE_WITH_IT;
		}
		
		DONE_WITH_IT:
		
		fillVec(gph);
		
	}
bool GameVoxelWrap::findNextCoord (ivec3 * voxResult)
                                             {
		int i;
		int j;
		int k;
		int q;
		int r;
		
		int ii;
		int jj;
		int kk;
		int ikk;
		
		ivec3 curVoxel;
		ivec3 localOffset;
		
		
		int minv = 0 + paddingInCells;
		int maxv = cellsPerHolderPad-paddingInCells;
		
		int lastPtr;
		int cellData;
		int cellData2;
		int voxelsPerCellM1 = voxelsPerCell-1;
		
		bool foundCell = false;
		
		for (i = minv; i < maxv; i++) {
			for (j = minv; j < maxv; j++) {
				for (k = minv; k < maxv; k++) {
					
						
						if (getPadData(i,j,k)->visited) {
							
						}
						else {
							getPadData(i,j,k)->visited = true;						
							
							cellData = getPadData(i,j,k)->cellVal;
							
							if (cellData == E_CD_SOLID) {
								
								foundCell = false;
								
								for (q = 0; q < NUM_ORIENTATIONS; q++) {
									cellData2 = getPadData(
										i + DIR_VECS_I[q][0],
										j + DIR_VECS_I[q][1],
										k + DIR_VECS_I[q][2]
									)->cellVal;
									
									if (cellData2 != E_CD_SOLID) {
										foundCell = true;
										break;
									}
									
								}
							
								if (foundCell) {
									
									foundCell = true;
									
									localOffset.set(i,j,k);
									localOffset *= voxelsPerCell;
									
									for (kk = 0; kk < voxelsPerCell; kk++) {
										
										ikk = voxelsPerCellM1-kk;
										
										for (r = 0; r < 4; r++) {
											switch (r) {
												case 0:
													curVoxel.set(kk, kk, ikk);
												break;
												case 1:
													curVoxel.set(ikk, kk, ikk);
												break;
												case 2:
													curVoxel.set(kk, ikk, ikk);
												break;
												case 3:
													curVoxel.set(ikk, ikk, ikk);
												break;
											}
											
											//curVoxel += offsetInVoxels;
											curVoxel += localOffset;
											if (isSurfaceVoxel(&curVoxel,lastPtr, false)) {
												voxResult->set(
													curVoxel.x, curVoxel.y, curVoxel.z
												);
												return true;
											}
											
										}
									}
									
									for (jj = 0; jj < voxelsPerCell; jj++) {
										for (ii = 0; ii < voxelsPerCell; ii++) {
											for (r = 0; r < 6; r++) {
												switch (r) {
													case 0:
														curVoxel.set(0, ii, jj);
													break;
													case 1:
														curVoxel.set(voxelsPerCellM1, ii, jj);
													break;
													case 2:
														curVoxel.set( ii, 0, jj );
													break;
													case 3:
														curVoxel.set( ii, voxelsPerCellM1, jj );
													break;
													case 4:
														curVoxel.set(ii,jj,0);
													break;
													case 5:
														curVoxel.set(ii,jj,voxelsPerCellM1);
													break;
												}
												
												//curVoxel += offsetInVoxels;
												curVoxel += localOffset;
												if (isSurfaceVoxel(&curVoxel,lastPtr,false)) {
													voxResult->set(
														curVoxel.x, curVoxel.y, curVoxel.z
													);
													return true;
												}
												
										}
									}
									
									
									
								}
							
							}
						}
					}
						
				}
			}
		}
		
		return false;
	}
bool GameVoxelWrap::inBounds (ivec3 * pos, int minB, int maxB)
                                                      {
		
		// ivec3 minB = offsetInVoxels - octOffsetInVoxels;
		// ivec3 maxB = offsetInVoxels + octOffsetInVoxels;
		
		// minB += (0);
		// maxB += (dimInVoxels - 0);
		
		
		
		return (
			(pos->x >= minB) && (pos->x < maxB)  &&
			(pos->y >= minB) && (pos->y < maxB)  &&
			(pos->z >= minB) && (pos->z < maxB)
		);
		
	}
int GameVoxelWrap::getNode (ivec3 * pos)
                                {
		return pos->x + pos->y*voxelsPerHolderPad + pos->z*voxelsPerHolderPad*voxelsPerHolderPad;
	}
void GameVoxelWrap::floodFill (ivec3 startVox)
                                       {
		basePD->fillStack.clear();
		basePD->fillStack.push_back(startVox);
		
		ivec3 curVox;
		ivec3 tempVox;
		int curNode;
		int tempNode;
		
		int q;
		int lastPtr;
		
		bool foundNext;
		
		lastFFSteps = 0;
		
		curNode = getNode(&startVox);
		voxelBuffer->setFlag(curNode,E_OCT_VISITED);
		
		while (basePD->fillStack.size() > 0) {
			
			lastFFSteps++;
			
			curVox = basePD->fillStack.back();
			basePD->fillStack.pop_back();
			
			curNode = getNode(&curVox);
			
			
			//foundNext = false;
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				tempVox = curVox + DIR_VECS_IV[q];
				
					
				
				if (isSurfaceVoxel(&tempVox,lastPtr,true)) {
					basePD->fillStack.push_back(tempVox);
					voxelBuffer->setFlag(lastPtr,E_OCT_VISITED);
					// foundNext = true;
					// goto NEXT_FF_ITERATION;
				}
				
			}
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				tempVox = curVox + DIR_VECS_IV[q];
				
				
				
				if (isInvSurfaceVoxel(&tempVox,curNode,lastPtr,true)) {
					basePD->fillStack.push_back(tempVox);
					voxelBuffer->setFlag(lastPtr,E_OCT_VISITED);
					// foundNext = true;
					// goto NEXT_FF_ITERATION;
				}
				
				
			}
			
// NEXT_FF_ITERATION:
			
// 			if (foundNext) {
// 				basePD->fillStack.push_back(tempVox);
// 			}
			
		}
		
	}
bool GameVoxelWrap::isInvSurfaceVoxel (ivec3 * pos, int ignorePtr, int & curPtr, bool checkVisited)
                                                                                          {
		int q;
		ivec3 tempVox;
		
		curPtr = getVoxelAtCoord(pos);
		if (curPtr < 0) {
			return false;
		}
		
		
		int tempPtr;
		
		if (checkVisited) {
			if (voxelBuffer->getFlag(curPtr,E_OCT_VISITED)) {
				return false;
			}
		}
		
		if ( !(voxelBuffer->getFlag(curPtr,E_OCT_SOLID)) ) {
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				
				tempVox = (*pos) + DIR_VECS_IV[q];
				tempPtr = getVoxelAtCoord(&tempVox);
				if (tempPtr >= 0) {
					if (tempPtr == ignorePtr) {
						// ignore the voxel we came from
					}
					else {
						if (voxelBuffer->getFlag(tempPtr,E_OCT_SOLID)) {
							
							return true;
						}
					}
				}
				
				
				
			}
		}
		
		return false;
	}
bool GameVoxelWrap::isSurfaceVoxel (ivec3 * pos, int & curPtr, bool checkVisited)
                                                                        {
		int q;
		ivec3 tempVox;
		
		curPtr = getVoxelAtCoord(pos);
		if (curPtr < 0) {
			return false;
		}
		
		if (checkVisited) {
			if (voxelBuffer->getFlag(curPtr,E_OCT_VISITED)) {
				return false;
			}
		}
		
		int tempPtr;
		
		bool isSurface = false;
		
		uint curSide = E_OCT_XP;
		
		if ( voxelBuffer->getFlag(curPtr,E_OCT_SOLID) ) {
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				
				tempVox = (*pos) + DIR_VECS_IV[q];
				tempPtr = getVoxelAtCoord(&tempVox);
				
				if (tempPtr >= 0) {
					if (voxelBuffer->getFlag(tempPtr,E_OCT_SOLID)) {
						
					}
					else {
						voxelBuffer->setFlag(curPtr, curSide);
						isSurface = true;
					}
				}
				
				curSide *= 2;
				
			}
		}
		
		if (isSurface) {
			voxelBuffer->setFlag(curPtr, E_OCT_SURFACE);
		}
		
		return isSurface;
	}
int GameVoxelWrap::getVoxelAtCoord (ivec3 * pos)
                                        {
		
		//int minB = 0;
		//int maxB = voxelsPerHolderPad;
		if (inBounds(pos,0,voxelsPerHolderPad)) {
			bool wasNew = false;
			int result = voxelBuffer->addNode(pos,wasNew);
			
			if (wasNew) {
				voxelBuffer->setFlag(result, E_OCT_NOTNEW);
				voxelBuffer->addIndex(result);
				calcVoxel(pos,result);
			}
			
			return result;
		}
		else {
			return -1;
		}
		
		
	}
float GameVoxelWrap::sampLinear (ivec3 * pos)
                                     {
		int q;
		int i;
		int j;
		int k;
		
		float res[8];
		
		int xv = pos->x/voxelsPerCell;
		int yv = pos->y/voxelsPerCell;
		int zv = pos->z/voxelsPerCell;
		
		float fx = pos->x-xv*voxelsPerCell;
		float fy = pos->y-yv*voxelsPerCell;
		float fz = pos->z-zv*voxelsPerCell;
		
		float fVPC = voxelsPerCell;
		
		
		
		fx /= fVPC;
		fy /= fVPC;
		fz /= fVPC;
		
		getPadData(xv,yv,zv)->visited = true;
		
		for (q = 0; q < 8; q++) {
			k = q/(2*2);
			j = (q-k*2*2)/2;
			i = q-(k*2*2 + j*2);
			
			res[q] = getPadData(xv+i,yv+j,zv+k)->terVal;
		}
		
		res[0] = res[0]*(1.0f-fz) + res[4]*fz;
		res[1] = res[1]*(1.0f-fz) + res[5]*fz;
		res[2] = res[2]*(1.0f-fz) + res[6]*fz;
		res[3] = res[3]*(1.0f-fz) + res[7]*fz;
		
		res[0] = res[0]*(1.0f-fy) + res[2]*fy;
		res[1] = res[1]*(1.0f-fy) + res[3]*fy;
		
		return res[0]*(1.0f-fx) + res[1]*fx;
		
	}
PaddedDataEntry * GameVoxelWrap::getPadData (int ii, int jj, int kk)
                                                            {
		
		
		int i = ii;
		int j = jj;
		int k = kk;
		int cphM1 = cellsPerHolderPad-1;
		
		if (i < 0) {
			i = 0;
		}
		if (i > cphM1) {
			i = cphM1;
		}
		
		if (j < 0) {
			j = 0;
		}
		if (j > cphM1) {
			j = cphM1;
		}
		
		if (k < 0) {
			k = 0;
		}
		if (k > cphM1) {
			k = cphM1;
		}
		
		
		return &(
			baseData[
				i + j*cellsPerHolderPad + k*cellsPerHolderPad*cellsPerHolderPad
			]	
		);
	}
vec3 GameVoxelWrap::randPN (vec3 co)
                             {
			
			vec3 myres = vec3(
					co.dot(crand0),
					co.dot(crand1),
					co.dot(crand2)
			);
			
			myres.doSin();
			myres *= 43758.8563f;
			myres.doFract();
			
			return myres*2.0f - oneVec;
	}
void GameVoxelWrap::getVoro (ivec3 * worldPos, ivec3 * worldClosestCenter, int iSpacing)
                                                                               {
		
		vec3 fWorldPos = vec3(
			worldPos->x,
			worldPos->y,
			worldPos->z
		);
		
		float fSpacing = iSpacing;
		
		fWorldPos *= 1.0f/fSpacing;
		
		vec3 fWorldCellPos = vec3(
			worldPos->x/iSpacing,
			worldPos->y/iSpacing,
			worldPos->z/iSpacing
		);
		
		fWorldPos -= vec3(
			fWorldCellPos.x,
			fWorldCellPos.y,
			fWorldCellPos.z
		);
		
		int i;
		
		vec3 testPos;
		float testDis;
		float variance = 0.5f;
		
		vec3 bestPos = VORO_OFFSETS[0] + randPN(fWorldCellPos+VORO_OFFSETS[0])*variance;
		float bestDis = fWorldPos.distance(bestPos);
		
		for (i = 1; i < 27; i++) {
			testPos = VORO_OFFSETS[i] + randPN(fWorldCellPos+VORO_OFFSETS[i])*variance;
			testDis = fWorldPos.distance(testPos);
			
			if (testDis < bestDis) {
				bestDis = testDis;
				bestPos = testPos;
			}
		}
		
		worldClosestCenter->set(
			(bestPos.x + fWorldCellPos.x)*fSpacing,
			(bestPos.y + fWorldCellPos.y)*fSpacing,
			(bestPos.z + fWorldCellPos.z)*fSpacing
		);
		
	}
void GameVoxelWrap::calcVoxel (ivec3 * pos, int octPtr)
                                               {
		
		ivec3 worldPos = (*pos) + offsetInVoxels;
		worldPos -= paddingInVoxels;
		
		ivec3 worldClosestCenter;// = worldPos;
		ivec3 localClosestCenter;
		
		getVoro(&worldPos,&worldClosestCenter, voxelsPerCell*2);
		localClosestCenter = worldClosestCenter - offsetInVoxels;
		localClosestCenter += paddingInVoxels;
		
		
		
		float terSamp = sampLinear(&localClosestCenter);
		
		float terSampOrig = sampLinear(pos);
		bool isSolid = (mixf(terSamp,terSampOrig,0.0f) >= 0.5f);
		//bool isSolid = (terSamp >= 0.5f);
		
		
		if (isSolid) {
			voxelBuffer->setFlag(octPtr, E_OCT_SOLID);
		}
		
		
		
	}
#undef LZZ_INLINE
 
