// f00324_gamevoxelwrap.h
//

#include "f00324_gamevoxelwrap.e"
#define LZZ_INLINE inline
GameVoxelWrap::GameVoxelWrap ()
                        {
		gameOct = NULL;
		basePD = NULL;
		baseData = NULL;
	}
void GameVoxelWrap::fillVec (GamePageHolder * gph)
                                          {
		int totSize = gameOct->octNodes.size();
		
		if (totSize <= 0) {
			return;
		}
		
		int i;
		float fVPC = voxelsPerCell;
		fVPC = 1.0f/fVPC;
		
		VectorI3 voxOffset;
		Vector3 fVO;
		OctNode* nodeArr = &(gameOct->octNodes[0]);
		
		for (i = 0; i < totSize; i++) {
			
			
			if (getFlag(i,E_OCT_SURFACE)) {
				
				voxOffset.x = nodeArr[i].x;
				voxOffset.y = nodeArr[i].y;
				voxOffset.z = nodeArr[i].z;
				
				voxOffset -= (octOffsetInVoxels - offsetInVoxels);
				
				fVO.x = voxOffset.x;
				fVO.y = voxOffset.y;
				fVO.z = voxOffset.z;
				
				fVO *= fVPC;
				
				gph->vertexVec.push_back(fVO.x);
				gph->vertexVec.push_back(fVO.y);
				gph->vertexVec.push_back(fVO.z);
				gph->vertexVec.push_back(1.0f);
				
				gph->vertexVec.push_back(fVO.x);
				gph->vertexVec.push_back(fVO.y);
				gph->vertexVec.push_back(fVO.z);
				gph->vertexVec.push_back(1.0f);
				
				
				
			}
			
		}
	}
void GameVoxelWrap::process (GamePageHolder * gph)
                                          {
		VectorI3 cellCoord;
		VectorI3 voxResult;
		
		curPD = gph->curPD;
		
		offsetInCells.set(
			gph->gphMinInCells.getIX(),
			gph->gphMinInCells.getIY(),
			gph->gphMinInCells.getIZ()
		);
		
		offsetInVoxels = offsetInCells;
		offsetInVoxels *= voxelsPerCell;
		
		gameOct = singleton->octPool[curPD];
		gameOct->clearAllNodes();
		
		
		basePD = (&singleton->pdPool[curPD]);
		baseData = singleton->pdPool[curPD].data;
		
		
		while( findNextCoord(&voxResult) ) {
			floodFill(voxResult);
			//goto DONE_WITH_IT;
		}
		
		//DONE_WITH_IT:
		
		fillVec(gph);
		
	}
void GameVoxelWrap::init (Singleton * _singleton, int _dimInVoxels)
          {
		singleton = _singleton;
		dimInVoxels = _dimInVoxels;
		octInVoxels = dimInVoxels*2;
		
		octOffsetInVoxels = VectorI3(
			(octInVoxels-dimInVoxels)/2,
			(octInVoxels-dimInVoxels)/2,
			(octInVoxels-dimInVoxels)/2	
		);
		
		voxelsPerCell = singleton->voxelsPerCell;
		cellsPerHolder = singleton->cellsPerHolder;
		cellsPerHolderPad = singleton->cellsPerHolderPad;
		paddingInCells = singleton->paddingInCells;
		paddingInVoxels = paddingInCells*voxelsPerCell;
		
		
	}
bool GameVoxelWrap::findNextCoord (VectorI3 * voxResult)
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
		
		VectorI3 curVoxel;
		VectorI3 localOffset;
		
		int cellsPerHolder = singleton->cellsPerHolder;
		int paddingInCells = singleton->paddingInCells;
		
		
		int minv = 0;
		int maxv = cellsPerHolder;
		
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
											
											curVoxel += offsetInVoxels;
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
												
												curVoxel += offsetInVoxels;
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
bool GameVoxelWrap::inBounds (VectorI3 * pos)
                                     {
		
		VectorI3 minB = offsetInVoxels - octOffsetInVoxels;
		VectorI3 maxB = offsetInVoxels + octOffsetInVoxels;
		
		minB += (2);
		maxB += (dimInVoxels - 2);
		
		return (
			(pos->x >= minB.x) && (pos->x < maxB.x)  &&
			(pos->y >= minB.y) && (pos->y < maxB.y)  &&
			(pos->z >= minB.z) && (pos->z < maxB.z)
		);
		
	}
int GameVoxelWrap::getNode (VectorI3 * pos)
                                   {
		VectorI3 voxOffset = (*pos) + octOffsetInVoxels - offsetInVoxels;
		
		return gameOct->getNode(&voxOffset);
	}
int GameVoxelWrap::addNode (VectorI3 * pos, bool & wasNew)
                                                 {
		bool wn;
		int res;
		
		VectorI3 voxOffset = (*pos) + octOffsetInVoxels - offsetInVoxels;
		
		res = gameOct->addNode(&voxOffset,wn);
		
		wasNew = wn;
		return res;
		
	}
void GameVoxelWrap::floodFill (VectorI3 startVox)
                                          {
		basePD->fillStack.clear();
		basePD->fillStack.push_back(startVox);
		
		VectorI3 curVox;
		VectorI3 tempVox;
		int curNode;
		int tempNode;
		
		int q;
		int lastPtr;
		
		bool foundNext;
				
		while (basePD->fillStack.size() > 0) {
			curVox = basePD->fillStack.back();
			basePD->fillStack.pop_back();
			
			curNode = getNode(&curVox);
			
			
			setFlag(curNode,E_OCT_VISITED);
			
			foundNext = false;
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				tempVox = curVox + DIR_VECS_IV[q];
				
				if (inBounds(&tempVox)) {
					if (isSurfaceVoxel(&tempVox,lastPtr,true)) {
						foundNext = true;
						goto NEXT_FF_ITERATION;
					}
				}
			}
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				tempVox = curVox + DIR_VECS_IV[q];
				
				if (inBounds(&tempVox)) {
					if (isInvSurfaceVoxel(&tempVox,curNode,lastPtr,true)) {
						foundNext = true;
						goto NEXT_FF_ITERATION;
					}
				}
			}
			
NEXT_FF_ITERATION:
			
			if (foundNext) {
				basePD->fillStack.push_back(tempVox);
			}
			
		}
		
	}
bool GameVoxelWrap::isInvSurfaceVoxel (VectorI3 * pos, int ignorePtr, int & curPtr, bool checkVisited)
                                                                                             {
		int q;
		VectorI3 tempVox;
		
		curPtr = getVoxelAtCoord(pos);
		int tempPtr;
		
		if (checkVisited) {
			if (getFlag(curPtr,E_OCT_VISITED)) {
				return false;
			}
		}
		
		if ( !(getFlag(curPtr,E_OCT_SOLID)) ) {
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				
				tempVox = (*pos) + DIR_VECS_IV[q];
				tempPtr = getVoxelAtCoord(&tempVox);
				
				if (tempPtr == ignorePtr) {
					// ignore the voxel we came from
				}
				else {
					if (getFlag(tempPtr,E_OCT_SOLID)) {
						
						return true;
					}
				}
				
				
				
			}
		}
		
		return false;
	}
bool GameVoxelWrap::isSurfaceVoxel (VectorI3 * pos, int & curPtr, bool checkVisited)
                                                                           {
		int q;
		VectorI3 tempVox;
		
		curPtr = getVoxelAtCoord(pos);
		
		if (checkVisited) {
			if (getFlag(curPtr,E_OCT_VISITED)) {
				return false;
			}
		}
		
		int tempPtr;
		
		if ( getFlag(curPtr,E_OCT_SOLID) ) {
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				
				tempVox = (*pos) + DIR_VECS_IV[q];
				tempPtr = getVoxelAtCoord(&tempVox);
				
				if (getFlag(tempPtr,E_OCT_SOLID)) {
					
				}
				else {
					setFlag(curPtr, E_OCT_SURFACE);
					return true;
				}
				
			}
		}
		
		clearFlag(curPtr, E_OCT_SURFACE);
		return false;
	}
int GameVoxelWrap::getVoxelAtCoord (VectorI3 * pos)
                                           {
		
		bool wasNew = false;
		int result = addNode(pos,wasNew);
		
		if (wasNew) {
			calcVoxel(pos,result);
		}
		
		return result;
	}
float GameVoxelWrap::sampLinear (VectorI3 * pos)
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
		
		
		int i = ii + paddingInCells;
		int j = jj + paddingInCells;
		int k = kk + paddingInCells;
		
		return &(
			baseData[
				i + j*cellsPerHolderPad + k*cellsPerHolderPad*cellsPerHolderPad
			]	
		);
	}
void GameVoxelWrap::calcVoxel (VectorI3 * pos, int octPtr)
                                                  {
		
		VectorI3 sampPos = (*pos) - offsetInVoxels;
		
		float terSamp = sampLinear(&sampPos);
		
		bool isSolid = (terSamp >= 0.5);
		
		//gameOct->octNodes[octPtr].flags = 0;
		
		if (isSolid) {
			setFlag(octPtr, E_OCT_SOLID);
		}
		
		
		
	}
#undef LZZ_INLINE
 
