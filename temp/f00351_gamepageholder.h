// f00351_gamepageholder.h
//

#include "f00351_gamepageholder.e"
#define LZZ_INLINE inline
void GamePageHolder::reset (bool destroyCache)
                                      {
		vertexVec.clear();
		vertexVec.shrink_to_fit();
		unbindPD();
		//isDirty = false;
		wasStacked = false;
		hasCache = false;
		hasData = true;
		hasPath = true;
		holderFlags = E_CD_UNKNOWN;
		listEmpty = true;
		listGenerated = false;
		readyToRender = false;
		pathsInvalid = true;
		idealPathsInvalid = true;
		pathsReady = false;
		idealPathsReady = false;
		wasGenerated = false;
		lockWrite = false;
		
		if (destroyCache) {
			singleton->checkCacheEntry(blockId,chunkId,holderId,true);
		}
	}
GamePageHolder::GamePageHolder ()
                         {
		
		int i;
		
		for (i = 0; i < NUM_MIP_LEVELS_WITH_FIRST; i++) {
			begMip[i] = 0;
			endMip[i] = 0;
		}
		
		
		
		isDirty = false;
		
		
		
		
		terVW = NULL;
		curPD = -1;
		
		//cubeData = NULL;
		cellData = NULL;
		extrData = NULL;
		pathData = NULL;
		
		reset(false);
		
	}
void GamePageHolder::init (Singleton * _singleton, int _blockId, int _chunkId, int _holderId, int trueX, int trueY, int trueZ)
          {

		curPD = -1;

		totIdealNodes = 0;
		totGroupIds = 0;

		visitId = 0;

		int i;
		int j;
		int k;
		int ind;
		
		

		blockId = _blockId;
		chunkId = _chunkId;
		holderId = _holderId;

		singleton = _singleton;

		
		// if (isBlockHolder) {
		// 	cellsPerHolder = singleton->blocksPerWorld;
		// }
		// else {
			cellsPerHolder = singleton->cellsPerHolder;
		//}
		
		
		//cubeDataSize = cellsPerHolder*cellsPerHolder*cellsPerHolder;
		pathSize = cellsPerHolder*cellsPerHolder*cellsPerHolder;
		cellDataSize = cellsPerHolder*cellsPerHolder*cellsPerHolder*4;

		origOffset.setFXYZ(cellsPerHolder/2,cellsPerHolder/2,cellsPerHolder/2);

		offsetInHolders.setIXYZ(trueX,trueY,trueZ);
		//offsetInBlocks.copyFrom(&offsetInHolders);
		//offsetInBlocks.intDivXYZ(singleton->holdersPerBlock);
		
		gphMinInCells.copyFrom(&offsetInHolders);
		gphMaxInCells.copyFrom(&offsetInHolders);

		gphMaxInCells.addXYZ(1);

		
		
		// if (isBlockHolder) {
		// 	gphMinInCells.multXYZ(singleton->cellsPerWorld);
		// 	gphMaxInCells.multXYZ(singleton->cellsPerWorld);
			
			
		// }
		// else {
			gphMinInCells.multXYZ(cellsPerHolder);
			gphMaxInCells.multXYZ(cellsPerHolder);
		//}
		

		gphCenInCells.averageXYZ(&gphMaxInCells,&gphMinInCells);
		

		voxelWrap = new GameVoxelWrap();
		voxelWrap->init(singleton);

		
	}
void GamePageHolder::makeDirty ()
                         {
		reset(true);
		//cout << "makeDirty\n";
		isDirty = true;
	}
void GamePageHolder::gatherObjects ()
                             {
		
		//PaddedData* pd = &(singleton->pdPool[curPD]);
		
		tempObjects.clear();
		objectOrder.clear();
		
		int chunkRad = singleton->iGetConst(E_CONST_CHUNK_GATHER_RAD);
		GameChunk* parentChunk = singleton->gw->getChunkAtId(blockId,chunkId);
		GameChunk* curChunk;
		
		int i;
		int j;
		int k;
		
		int q;
		
		intPair ip;
		
		FIVector4 tempVec;
		
		tempVec.averageNegXYZ(&gphMaxInCells,&gphMinInCells);
		
		vec3 gphCen = gphCenInCells.getVec3();
		vec3 gphRad = tempVec.getVec3();
		vec3 cenDif;
		vec3 radAdd;
		
		float fPadding = singleton->paddingInCells;
		
		ObjectStruct* curObj;
		
		gphRad += fPadding;
		
		if (parentChunk != NULL) {
			for (k = -chunkRad; k <= chunkRad; k++) {
				for (j = -chunkRad; j <= chunkRad; j++) {
					for (i = -chunkRad; i <= chunkRad; i++) {
						curChunk = singleton->gw->getChunkAtCoords(
							parentChunk->offsetInChunks.getIX()+i,
							parentChunk->offsetInChunks.getIY()+j,
							parentChunk->offsetInChunks.getIZ()+k,
							false
						);
						
						if (curChunk != NULL) {
							
							for (q = 0; q < curChunk->localObjects.size(); q++) {
								curObj = &(curChunk->localObjects[q]);
								cenDif = gphCen-curObj->data[E_OSD_CENTER];
								cenDif.doAbs();
								radAdd = curObj->data[E_OSD_RADIUS] + gphRad;
								
								if (
									(cenDif.x <= radAdd.x) &&
									(cenDif.y <= radAdd.y) &&
									(cenDif.z <= radAdd.z)
								) {
									tempObjects.push_back(*curObj);
								
									ip.v0 = tempObjects.size()-1;
									ip.v1 = tempObjects.back().globalId;
								
									objectOrder.push_back(ip);
								}
								
							}
							
						}
					}
				}
			}
		}
		
		if (tempObjects.size() > 0) {
			
			
			sort(objectOrder.begin(),objectOrder.end(),sortByV1);
		}
		
		
	}
int GamePageHolder::getCellAtCoordsLocal (int xx, int yy, int zz)
                                                         {
		int ii = xx;
		int jj = yy;
		int kk = zz;
		
		if (ii < 0) {
			ii += cellsPerHolder;
		}
		if (ii >= cellsPerHolder) {
			ii -= cellsPerHolder;
		}
		
		if (jj < 0) {
			jj += cellsPerHolder;
		}
		if (jj >= cellsPerHolder) {
			jj -= cellsPerHolder;
		}
		
		if (kk < 0) {
			kk += cellsPerHolder;
		}
		if (kk >= cellsPerHolder) {
			kk -= cellsPerHolder;
		}
		
		return getCellAtInd((ii + jj*cellsPerHolder + kk*cellsPerHolder*cellsPerHolder)*4);
	}
int GamePageHolder::getCellAtInd (int ind)
          {
		
		
		int q;
		
		
		if (wasGenerated) {
			
		}
		else {
			//cout << debugVal << " attempted getCellAtCoords without generation\n";
			
			return E_CD_SOLID;
		}
		//GamePage* gp = getPageAtCoords(x2, y2, z2, false);
		
		// todo: fix this
		//cout << "Holder is all one type\n";
		
		switch (holderFlags) {
			case E_CD_EMPTY:
				return E_CD_EMPTY;
			break;
			case E_CD_WATER:
				return E_CD_WATER;
			break;
			case E_CD_SOLID:
				return E_CD_SOLID;
			break;
		}
		
		if (cellData == NULL) {
			return E_CD_EMPTY;
		}
		else {
			
			
			if (
				(cellData[ind+E_PTT_TER] != FLUID_UNIT_MIN) ||
				(cellData[ind+E_PTT_BLD] != FLUID_UNIT_MIN)
			) {
				return E_CD_SOLID;
			}
			else {
				
				if (cellData[ind+E_PTT_WAT] != FLUID_UNIT_MIN) {
					return E_CD_WATER;
				}
				else {
					return E_CD_EMPTY;
				}
				
				
			}
		}
	}
void GamePageHolder::getArrAtInd (int ind, int * tempCellData, int * tempCellData2)
          {
		
		
		int q;
		
		if (
			wasGenerated
		) {
			
			if (hasData && (cellData != NULL)) {
				for (q = 0; q < 4; q++) {
					tempCellData[q] = cellData[ind+q];
					tempCellData2[q] = extrData[ind+q];
				}
			}
			else {
				switch (holderFlags) {
					case E_CD_WATER:
						tempCellData[E_PTT_WAT] = FLUID_UNIT_MAX;
						tempCellData2[E_PTT_LST] = FLUID_UNIT_MAX;
					break;
					case E_CD_SOLID:
						tempCellData[E_PTT_TER] = FLUID_UNIT_MAX;
					break;
				}
			}
		}
	}
void GamePageHolder::setArrAtInd (int ind, int * tempCellData, int * tempCellData2)
          {
			
			int q;
			
			if (
				hasData && wasGenerated && (cellData != NULL)
			) {
				for (q = 0; q < 4; q++) {
					cellData[ind+q] = tempCellData[q];
					extrData[ind+q] = tempCellData2[q];
				}
			}
			
			
	}
void GamePageHolder::clearPathPreserve ()
                                 {
		int i;
		
		if (hasPath) {
			for (i = 0; i < pathSize; i++) {
					if (pathData[i] >= 0) {
						pathData[i] = GROUP_ID_UNMARKED_IDEAL;
					}
					else {
						pathData[i] = GROUP_ID_UNMARKED;
					}
			}
		}
		
		groupIdStack.clear();
		totIdealNodes = 0;
		totGroupIds = 0;
		
	}
void GamePageHolder::clearPathSizes ()
                              {
		int i;
		
		if (hasPath) {
			for (i = 0; i < pathSize; i++) {
				pathData[i] = GROUP_ID_UNMARKED;
			}
		}
		
		groupIdStack.clear();
		groupInfoStack.clear();
		totIdealNodes = 0;
		totGroupIds = 0;
		
	}
void GamePageHolder::checkData (bool checkPath)
                                       {
		int i;
		
		if (hasData) {
			if (cellData == NULL) {
				//cubeData = new uint[CUBE_DATA_SIZE];
				cellData = new int[cellDataSize];
				
				// if (cubeWraps.size() == 0) {
				// 	cubeWraps.push_back(CubeWrap());
				// }
				
				
				// for (i = 0; i < CUBE_DATA_SIZE; i++) {
				// 	cubeData[i] = CUBE_DATA_INVALID;
				// }
				
			}
			
			if (extrData == NULL) {
				extrData = new int[cellDataSize];
			}
				
		}
		
		if (hasPath&&checkPath) {
			if (pathData == NULL) {
				pathData = new int[pathSize];
			}
		}
		
		
		
		
	}
void GamePageHolder::clearGroupFlags (int targId)
                                         {
		int i;
		for (i = 0; i < groupInfoStack.size(); i++) {
			groupInfoStack[i].visitId = targId;
		}
	}
int GamePageHolder::floodFillAtInd (int firstInd, int newId, bool findCenter, GroupInfoStruct * curGI)
          {
		
		if (firstInd < 0) {
			cout << "firstInd " << firstInd << "\n";
		}
		
		
		indexStack.clear();
		
		indexStack.push_back(firstInd);
		groupIdStack.push_back(GroupIdStruct());
		groupIdStack.back().ind = firstInd;
		groupIdStack.back().groupId = newId;
		groupIdStack.back().cameFromInd = -1;
		groupIdStack.back().pathCost = 0;
		
		pathData[firstInd] = groupIdStack.size()-1;
		
		bool doProc;
		
		int fillCount = 1;
		
		int groupIdStackInd;
		int dir;
		int ind;
		int testInd;
		int i;
		int j;
		int k;
		int n;
		int q;
		//int foundInd;
		bool didFind = false;
		int testI;
		int testJ;
		int testK;
		int cellsPerHolderM1 = cellsPerHolder-1;
		int lastCost;
		int frontInd = 0;
		
		// for (i = 0; i < 6; i++) {
		// 	curGI->touchesFace[i] = false;
		// }
		curGI->begInd = groupIdStack.size()-1;
		
		
		// find contiguous regions
		while (frontInd < indexStack.size()) {//while (indexStack.size() > 0) {
			
			//ind = indexStack.back();
			
			ind = indexStack[frontInd];
			//frontInd++;
			
			k = ind/(cellsPerHolder*cellsPerHolder);
			j = (ind - k*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			i = ind - ( j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder );
			
			//foundInd = -1;
			didFind = false;
			
			for (dir = 0; dir < NUM_MOVEABLE_DIRS; dir++) {
				testI = (i+DIR_VECS_MOVE[dir][0]);
				testJ = (j+DIR_VECS_MOVE[dir][1]);
				testK = (k+DIR_VECS_MOVE[dir][2]);
				
				
				if (
					(testI >= 0) &&
					(testI < cellsPerHolder) &&
					(testJ >= 0) &&
					(testJ < cellsPerHolder) &&
					(testK >= 0) &&
					(testK < cellsPerHolder)
				) {
					
					
					
					testInd = 
						testI +
						testJ*cellsPerHolder +
						testK*cellsPerHolder*cellsPerHolder;
					
					if (
							//.groupId
							(pathData[testInd] == GROUP_ID_UNMARKED_IDEAL)
							//&& ((pathData[testInd].flags&E_PVT_LAND_BELOW) != 0)
					) {
						
						
						// foundInd = testInd;
						// goto NEXT_FILL_POINT;
					
						//if (testInd >= 0) {}
					
						
						didFind = true;
						
						indexStack.push_back(testInd);
						
						lastCost = groupIdStack.back().pathCost;
						
						groupIdStack.push_back(GroupIdStruct());
						groupIdStack.back().ind = testInd;
						if (groupIdStack.back().ind < 0) {
							cout << "groupIdStack.back().ind " << groupIdStack.back().ind << "\n";
						}
						
						groupIdStack.back().groupId = newId;
						groupIdStack.back().cameFromInd = ind;
						groupIdStack.back().pathCost = lastCost+1;
						
						//.groupId
						pathData[testInd] = groupIdStack.size()-1;
						
						
						fillCount++;
						
					
					}
				}
			}

// NEXT_FILL_POINT:
			if (didFind) {
				// indexStack.push_back(foundInd);
				
				// lastCost = groupIdStack.back().pathCost;
				
				// groupIdStack.push_back(GroupIdStruct());
				// groupIdStack.back().ind = foundInd;
				// groupIdStack.back().groupId = newId;
				// groupIdStack.back().cameFromInd = ind;
				// groupIdStack.back().pathCost = lastCost+1;
				
				// //.groupId
				// pathData[foundInd] = groupIdStack.size()-1;
				
			}
			else {
				frontInd++;
				//indexStack.pop_back();
			}
			
		}
		
		curGI->endInd = groupIdStack.size();
		
		
		
		
		
		// // if region touches one or fewer faces of bounding volume, remove it
		// // todo: maybe evaluate all regions regardless of pruning criteria
		// for (n = curGI->begInd; n < curGI->endInd; n++) {
			
		// 	ind = groupIdStack[n].ind;
			
		// 	k = ind/(cellsPerHolder*cellsPerHolder);
		// 	j = (ind - k*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		// 	i = ind - ( j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder );
			
		// 	if (i == 0) 								{curGI->touchesFace[0] = true;}
		// 	if (i == cellsPerHolderM1) 	{curGI->touchesFace[1] = true;}
		// 	if (j == 0) 								{curGI->touchesFace[2] = true;}
		// 	if (j == cellsPerHolderM1) 	{curGI->touchesFace[3] = true;}
		// 	if (k == 0) 								{curGI->touchesFace[4] = true;}
		// 	if (k == cellsPerHolderM1) 	{curGI->touchesFace[5] = true;}
			
		// }
		
		// q = 0;
		// for (n = 0; n < 6; n++) {
		// 	if (curGI->touchesFace[n]) {q++;}
		// }
		
		// // if region touches less than x faces prune it
		// if (q < 1) {
			
		// 	doProc = true;
		// 	while (doProc) {
		// 		if (groupIdStack.size() == 0) {
		// 			doProc = false;
		// 		}
		// 		else {
		// 			if (groupIdStack.back().groupId == newId) {
		// 				groupIdStack.pop_back();
		// 			}
		// 			else {
		// 				doProc = false;
		// 			}
		// 		}
		// 	}
			
		// 	groupInfoStack.pop_back();
			
		// 	return false;
		// }
		
		
		// find the center of the region
		int avgX = 0;
		int avgY = 0;
		int avgZ = 0;
		
		int totCount = 0;
		int testDis;
		int bestDis = 99999;
		int bestInd = -1;
		
		if (findCenter) {
			for (n = curGI->begInd; n < curGI->endInd; n++) {
				ind = groupIdStack[n].ind;
				
				k = ind/(cellsPerHolder*cellsPerHolder);
				j = (ind - k*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
				i = ind - ( j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder );
				
				avgX += i;
				avgY += j;
				avgZ += k;
				
				totCount++;
			}
			
			avgX /= totCount;
			avgY /= totCount;
			avgZ /= totCount;
			
			for (n = curGI->begInd; n < curGI->endInd; n++) {
				ind = groupIdStack[n].ind;
				
				k = ind/(cellsPerHolder*cellsPerHolder);
				j = (ind - k*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
				i = ind - ( j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder );
				
				testInd = ind;
				testDis = abs(i-avgX) + abs(j-avgY) + abs(k-avgZ);
				
				if (testDis < bestDis) {
					bestDis = testDis;
					bestInd = testInd;
				}
				
			}
			
			curGI->centerInd = bestInd;
		}
		
		
		
		
		// // clear visited
		// for (n = curGI->begInd; n < curGI->endInd; n++) {
		// 	ind = groupIdStack[n].ind;
		// 	//.groupId
		// 	pathData[ind] = GROUP_ID_UNMARKED_IDEAL;
		// }
		
		
		// // flood fill again, this time from center point and storing dir traveled and cost
		// indexStack.clear();
		// indexStack.push_back(curGI->centerInd);
		
		
		// groupIdStackInd = pathData[curGI->centerInd];
		
		// if (groupIdStackInd >= 0) {
		// 	groupIdStack[groupIdStackInd].cameFromInd = -1;
		// 	groupIdStack[groupIdStackInd].pathCost = 0;
		// }
		
		// //groupIdStack[bestN] = -1
		// //pathData[curGI->centerInd].cameFromInd = -1;
		// //pathData[curGI->centerInd].pathCost = 0;
		
		// //pathData[curGI->centerInd] = newId;
		
		// int frontInd = 0;
		
		// while (frontInd < indexStack.size()) {
			
		// 	ind = indexStack[frontInd];
		// 	frontInd++;
		// 	//indexStack.pop_front();
			

			
			
		// 	k = ind/(cellsPerHolder*cellsPerHolder);
		// 	j = (ind - k*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		// 	i = ind - ( j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder );
			
			
		// 	for (dir = 0; dir < NUM_MOVEABLE_DIRS; dir++) {
		// 		testI = (i+DIR_VECS_MOVE[dir][0]);
		// 		testJ = (j+DIR_VECS_MOVE[dir][1]);
		// 		testK = (k+DIR_VECS_MOVE[dir][2]);
				
				
		// 		if (
		// 			(testI >= 0) &&
		// 			(testI < cellsPerHolder) &&
		// 			(testJ >= 0) &&
		// 			(testJ < cellsPerHolder) &&
		// 			(testK >= 0) &&
		// 			(testK < cellsPerHolder)
		// 		) {
					
					
		// 			testInd = 
		// 				testI +
		// 				testJ*cellsPerHolder +
		// 				testK*cellsPerHolder*cellsPerHolder;
					
		// 			if (
		// 					(pathData[testInd] == GROUP_ID_UNMARKED_IDEAL)
		// 					//&& ((pathData[testInd].flags&E_PVT_LAND_BELOW) != 0)
		// 			) {
		// 				pathData[testInd].cameFromInd = ind;
		// 				pathData[testInd].pathCost = pathData[ind].pathCost+1;
		// 				pathData[testInd].groupId = newId;
		// 				indexStack.push_back(testInd);
		// 			}
		// 		}
		// 	}
			
			
		// }
		
		
		
		
		//return true;
		
		return fillCount;
		
	}
void GamePageHolder::findIdealNodes ()
                              {
			idealPathsReady = false;
			
			
			int curFillCount;
			
			int i;
			int j;
			int k;
			int cellVal;
			int cellValAbove;
			//int cellValAbove2;
			int cellValBelow;
			
			int curX;
			int curY;
			int curZ;
			int curInd;
			
			bool doProc;
			
			
			
			if (hasPath) {
				checkData(true);
				clearPathSizes();
				
				for (k = 0; k < cellsPerHolder; k++) {
					for (j = 0; j < cellsPerHolder; j++) {
						for (i = 0; i < cellsPerHolder; i++) {
							cellVal = singleton->gw->getCellAtCoords(
								gphMinInCells.getIX()+i,
								gphMinInCells.getIY()+j,
								gphMinInCells.getIZ()+k
							);
							cellValAbove = singleton->gw->getCellAtCoords(
								gphMinInCells.getIX()+i,
								gphMinInCells.getIY()+j,
								gphMinInCells.getIZ()+k+1
							);
							// cellValAbove2 = singleton->gw->getCellAtCoords(
							// 	gphMinInCells.getIX()+i,
							// 	gphMinInCells.getIY()+j,
							// 	gphMinInCells.getIZ()+k+2
							// );
							cellValBelow = singleton->gw->getCellAtCoords(
								gphMinInCells.getIX()+i,
								gphMinInCells.getIY()+j,
								gphMinInCells.getIZ()+k-1
							);
							
							
							
							// doProc = (
							// 	(cellVal == E_CD_WATER) &&
							// 	(
							// 		(cellValAbove == E_CD_WATER) || (cellValAbove==E_CD_EMPTY)
							// 	)
							// ) || (
							// 	(cellVal == E_CD_EMPTY)&&
							// 	(
							// 		(cellValAbove == E_CD_WATER) || (cellValAbove==E_CD_EMPTY)
							// 	) &&
							// 	(
							// 		cellValBelow == E_CD_SOLID	
							// 	)
							// );
							
							doProc =
								(cellVal < E_CD_SOLID) &&
								(cellValAbove < E_CD_SOLID) &&
								//(cellValAbove2 < E_CD_SOLID) &&
								(cellValBelow == E_CD_SOLID);
							
							
							if (doProc) {
								curInd = i + j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder;
								//.groupId
								pathData[curInd] = GROUP_ID_UNMARKED_IDEAL;
								//pathData[curInd].flags |= E_PVT_LAND_BELOW;
								totIdealNodes++;		
							}
							
							
							
						}
					}
				}
				
				
				//cout << "\n";
				
				curFillCount = 0;
				
				for (i = 0; i < pathSize; i++) {
					if (
						//.groupId
						(pathData[i] == GROUP_ID_UNMARKED_IDEAL)
						// && ((pathData[i].flags&E_PVT_LAND_BELOW) != 0)
					) {
						
						groupInfoStack.push_back(GroupInfoStruct());
						groupInfoStack.back().visitId = 0;
						curFillCount += floodFillAtInd(
							i,
							totGroupIds,
							true,
							&(groupInfoStack.back())
						);
						totGroupIds++;
						
						if (curFillCount >= totIdealNodes) {
							goto FIRST_FILL_DONE;
						}
						
					}
				}
				
FIRST_FILL_DONE:
				
				//cout << "groupIdStack.size()1 " << groupIdStack.size() << "\n";
				
				clearPathPreserve();
				
				// while (groupInfoStack.size() > 1) {
				// 	groupInfoStack.pop_back();
				// }
				
				
				
				for (i = 0; i < groupInfoStack.size(); i++) {
					floodFillAtInd(
						groupInfoStack[i].centerInd,
						i,//totGroupIds,
						false,
						&(groupInfoStack[i])
					);
					//totGroupIds++;
				}
			}
			
			// cout << "groupIdStack.size()2 " << groupIdStack.size() << "\n";
			// cout << "\n";
			
			
			idealPathsReady = true;
			
	}
int GamePageHolder::getGroupId (int pathDataIndex)
                                          {
		
		if (hasPath) {
			if (pathData[pathDataIndex] < 0) {
				
				return pathData[pathDataIndex];
			}
			else {
				return groupIdStack[pathData[pathDataIndex]].groupId;
			}
		}
		else {
			return GROUP_ID_NONEXISTENT;
		}
		
		
		
		
	}
int GamePageHolder::getInfoPD (int pathDataIndex)
                                         {
		
		if (hasPath) {
			return pathData[pathDataIndex];
		}
		else {
			return -100;
		}
		
	}
GroupIdStruct * GamePageHolder::getInfo (int pathDataIndex)
                                                  {
		
		if (hasPath) {
			if (pathData[pathDataIndex] < 0) {
				//cout << "pathDataIndex < 0\n";
				return NULL;
			}
			
			return &(groupIdStack[pathData[pathDataIndex]]);
			
		}
		else {
			return NULL;
		}
		
		
	}
void GamePageHolder::getInfoReport (int pathDataIndex)
                                              {
		
		cout << "\n";
		
		if (hasPath) {
			if (pathData[pathDataIndex] < 0) {
				cout << "pathDataIndex < 0\n";
				return;
			}
			
			cout << "success\n";
			
		}
		else {
			cout << "hasPath==false\n";
			return;
		}
		
		
	}
btVector3 GamePageHolder::holderIndToBTV (GamePageHolder * curPointHolder, int curPointIndex)
                                                                                    {
		btVector3 pVec1;
		
		int ii;
		int jj;
		int kk;
		
		kk = curPointIndex/(cellsPerHolder*cellsPerHolder);
		jj = (curPointIndex-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		ii = curPointIndex-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
		
		pVec1 = curPointHolder->gphMinInCells.getBTV();
		pVec1 += btVector3(ii,jj,kk);
		
		return pVec1;
	}
void GamePageHolder::sortConNodes (GamePageHolder * endHolder, int endInd)
                                                                 {
		
		btVector3 endPoint = holderIndToBTV(endHolder, endInd);
		btVector3 curPoint;
		
		ConnectingNodeStruct tempCN;
		GamePageHolder* curHolder;
		
		int i;
		
		for (i = 0; i < bestConnectingNodes.size(); i++) {
			curHolder = singleton->gameLogic->getHolderById(
				bestConnectingNodes[i].blockIdTo,
				bestConnectingNodes[i].chunkIdTo,
				bestConnectingNodes[i].holderIdTo
			);
			
			curPoint = holderIndToBTV(
				curHolder,
				curHolder->groupInfoStack[bestConnectingNodes[i].groupIdTo].centerInd
			);
			
			bestConnectingNodes[i].dist = curPoint.distance(endPoint);
		}
		
		sort(bestConnectingNodes.begin(), bestConnectingNodes.end(), sortByDist);
		
		
	}
void GamePageHolder::linkRegions ()
                           {
		int i;
		int j;
		int k;
		
		int ii;
		int jj;
		int kk;
		
		int ic;
		int jc;
		int kc;
		
		int iic;
		int jjc;
		int kkc;
		
		int m;
		int n;
		int q;
		
		int zm;
		
		int ind;
		int mo2;
		
		int baseX;
		int baseY;
		int baseZ;
		
		int holderInd;
		
		bool doProc = false;
		
		int cellsPerHolderM1 = cellsPerHolder-1;
		//GamePageHolder* holderArr[NUM_MOVEABLE_DIRS_ONE_AWAY];
		int boundsArr[NUM_ORIENTATIONS];
		int indArr[NUM_ORIENTATIONS/2];
		int indArrInv[NUM_ORIENTATIONS/2];
		int targetInd;
		int targetGroupId;
		int targetCost;
		int curGroupId;
		
		bool notFound;
		bool foundBetter;
		
		ConnectingNodeStruct* cNode;
		GamePageHolder* targetHolder;
		
		boundsArr[0] = cellsPerHolderM1;
		boundsArr[1] = 0;
		boundsArr[2] = cellsPerHolderM1;
		boundsArr[3] = 0;
		boundsArr[4] = cellsPerHolderM1;
		boundsArr[5] = 0;
		
		
		// for (i = 0; i < NUM_MOVEABLE_DIRS_ONE_AWAY; i++) {
		// 	holderArr[i] = singleton->gw->getHolderAtCoords(
		// 		offsetInHolders.getIX()+DIR_VECS_MOVE[i][0],
		// 		offsetInHolders.getIY()+DIR_VECS_MOVE[i][1],
		// 		offsetInHolders.getIZ()+DIR_VECS_MOVE[i][2],
		// 		true
		// 	);
		// }
		
		bestConnectingNodes.clear();
		
		// find cells that touch side of holder
		
		
		
		for (n = 0; n < groupIdStack.size(); n++) {
			
			ind = groupIdStack[n].ind;
			
			
			curGroupId = groupIdStack[n].groupId;
			
			k = ind/(cellsPerHolder*cellsPerHolder);
			j = (ind - k*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			i = ind - ( j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder );
			
			indArr[0] = i;
			indArr[1] = j;
			indArr[2] = k;
			
			doProc = false;
			
			
			
			for (m = 0; m < NUM_ORIENTATIONS; m++) {
				mo2 = m/2;
				
				
				
				if (m <= 3) {
					// m is planar
					
					if (indArr[mo2] == boundsArr[m]) {
						// its on the planar edge
						doProc = true;
						break;
					}
					
				}
				else {
					// m is in z direction
					
					if ( abs(indArr[mo2] - boundsArr[m]) <= BASE_MOVEABLE_Z ) {
						doProc = true;
						break;
					}
				}
				
			}
			
			if (doProc) {
				
				baseX = i + gphMinInCells.getIX();
				baseY = j + gphMinInCells.getIY();
				baseZ = k + gphMinInCells.getIZ();
				
				for (m = 0; m < NUM_PLANAR_ORIENTATIONS; m++) {
					
					for (zm = -BASE_MOVEABLE_Z; zm <= BASE_MOVEABLE_Z; zm++) {
						
						

						// indArrInv[0] = i;
						// indArrInv[1] = j;
						// indArrInv[2] = k;

						// if (mo2 == 0) {indArrInv[0] = cellsPerHolderM1-i;}
						// if (mo2 == 1) {indArrInv[1] = cellsPerHolderM1-j;}
						// if (mo2 == 2) {indArrInv[2] = cellsPerHolderM1-k;}

						targetInd = singleton->gw->getCellInd(
							targetHolder,
							baseX + DIR_VECS_I[m][0],
							baseY + DIR_VECS_I[m][1],
							baseZ + zm
						);//indArrInv[0] + indArrInv[1]*cellsPerHolder + indArrInv[2]*cellsPerHolder*cellsPerHolder;
						
						
						if (
							(targetHolder->holderId == holderId) &&
							(targetHolder->chunkId == chunkId) &&
							(targetHolder->blockId == blockId)
						) {
							// same holder, do nothing
						}
						else {
							
							
							
							targetGroupId = targetHolder->getGroupId(targetInd);

							if (targetGroupId > -1) {
								targetCost = targetHolder->getInfo(targetInd)->pathCost + getInfo(ind)->pathCost;
								
								// first see if another connection to this block/holder/groupId exists
								
								notFound = true;
								foundBetter = false;
								
								for (q = 0; q < bestConnectingNodes.size(); q++) {
									cNode = &(bestConnectingNodes[q]);
									
									if (
										(cNode->blockIdTo == targetHolder->blockId) &&
										(cNode->chunkIdTo == targetHolder->chunkId) &&
										(cNode->holderIdTo == targetHolder->holderId) &&
										(cNode->groupIdFrom == curGroupId) &&
										(cNode->groupIdTo == targetGroupId)
									) {
										notFound = false;
									
										if (targetCost <= cNode->totCost) {
											
											// tie breaker
											if (targetCost == cNode->totCost) {
												kk = targetInd/(cellsPerHolder*cellsPerHolder);
												jj = (targetInd-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
												ii = targetInd-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
												
												//cNode->cellIndFrom
												kc = cNode->cellIndFrom/(cellsPerHolder*cellsPerHolder);
												jc = (cNode->cellIndFrom-kc*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
												ic = cNode->cellIndFrom-(kc*cellsPerHolder*cellsPerHolder + jc*cellsPerHolder);
												
												//cNode->cellIndTo
												kkc = cNode->cellIndTo/(cellsPerHolder*cellsPerHolder);
												jjc = (cNode->cellIndTo-kkc*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
												iic = cNode->cellIndTo-(kkc*cellsPerHolder*cellsPerHolder + jjc*cellsPerHolder);
												
												
												
												if ((k+kk) < (kc+kkc)) {foundBetter = true;} else {
													if ((j+jj) < (jc+jjc)) {foundBetter = true;} else {
														if ((i+ii) < (ic+iic)) {foundBetter = true;}
													}
												}
											}
											else {
												foundBetter = true;
											}
										}

										break;
									}
								}
								
								if (notFound) {
									bestConnectingNodes.push_back(ConnectingNodeStruct());
									cNode = &(bestConnectingNodes.back());
								}
								else {
									cNode = &(bestConnectingNodes[q]);
								}
								
								if (foundBetter||notFound) {
									
									cNode->blockIdFrom = blockId;
									cNode->holderIdFrom = holderId;
									cNode->chunkIdFrom = chunkId;
									cNode->blockIdTo = targetHolder->blockId;
									cNode->chunkIdTo = targetHolder->chunkId;
									cNode->holderIdTo = targetHolder->holderId;
									cNode->groupIdFrom = curGroupId;
									cNode->groupIdTo = targetGroupId;
									cNode->cellIndFrom = ind;
									cNode->cellIndTo = targetInd;
									cNode->totCost = targetCost;
									
								}
								
								
								
								// otherwise push back a new block/holder/groupId
								
							}

							
							
							
						}
						

						
						
						
						
						
						
					}
					
				}
								
				
			}
			
			
		}
		
	}
bool GamePageHolder::prepPathRefresh (int rad)
                                      {
		int i;
		int j;
		int k;
		
		GamePageHolder* curHolder;
		
		for (k = -rad; k <= rad; k++) {
			for (j = -rad; j <= rad; j++) {
				for (i = -rad; i <= rad; i++) {
					curHolder = singleton->gw->getHolderAtCoords(
						offsetInHolders.getIX()+i,
						offsetInHolders.getIY()+j,
						offsetInHolders.getIZ()+k,
						true
					);
					
					if (curHolder->wasGenerated) {
						
					}
					else {
						return false;
						
					}
					
				}
			}
		}
		
		return true;
		
	}
void GamePageHolder::refreshPaths ()
                            {
		
		pathsReady = false;
		
		int i;
		int j;
		int k;
		
		GamePageHolder* curHolder;
		
		int rad;
		
		rad = 2;
		for (k = -rad; k <= rad; k++) {
			for (j = -rad; j <= rad; j++) {
				for (i = -rad; i <= rad; i++) {
					curHolder = singleton->gw->getHolderAtCoords(
						offsetInHolders.getIX()+i,
						offsetInHolders.getIY()+j,
						offsetInHolders.getIZ()+k,
						true
					);
					
					if (curHolder->idealPathsInvalid) {
						curHolder->findIdealNodes();
						curHolder->idealPathsInvalid = false;
					}
					
				}
			}
		}
		
		rad = 1;
		for (k = -rad; k <= rad; k++) {
			for (j = -rad; j <= rad; j++) {
				for (i = -rad; i <= rad; i++) {
					curHolder = singleton->gw->getHolderAtCoords(
						offsetInHolders.getIX()+i,
						offsetInHolders.getIY()+j,
						offsetInHolders.getIZ()+k,
						true
					);
					
					if (curHolder->pathsInvalid) {
						curHolder->linkRegions();
						curHolder->pathsInvalid = false;
					}
				}
			}
		}
		
		
		// findIdealNodes();
		// // fetch all neighboring paths
		// for (i = 0; i < NUM_ORIENTATIONS; i++) {
		// 	curHolder = singleton->gw->getHolderAtCoords(
		// 		offsetInHolders.getIX()+DIR_VECS_I[i][0],
		// 		offsetInHolders.getIY()+DIR_VECS_I[i][1],
		// 		offsetInHolders.getIZ()+DIR_VECS_I[i][2],
		// 		true
		// 	);
			
		// 	curHolder->findIdealNodes();
		// }
		//linkRegions();
		
		// find complete array map of regions for just this holder
		
		
		// if (groupInfoStack.size() == 0) {
		// 	if (pathData != NULL) {delete[] pathData;}
		// 	pathData = NULL;
		// 	hasPath = false;
		// }
		
		
		
		
		
		pathsReady = true;
		
		
		
		
	}
void GamePageHolder::genCellData ()
                           {
		
		
		int i;
		int j;
		int k;
		
		int ii;
		int jj;
		int kk;
		
		int p;
		int q;
		int r;
		
		int ind;
		
		float fi;
		float fj;
		float fk;
		
		float zv;
		
		float fiAbs;
		float fjAbs;
		float fkAbs;
		
		float terHeight;
		float simplexVal;
		float simplexVal1;
		float simplexVal2;
		
		float disVal;
		
		float fSimp;
		int iTer;
		int iWat;
		
		checkData(false);
		
		for (i = 0; i < cellDataSize; i++) {
			cellData[i] = FLUID_UNIT_MIN;
			extrData[i] = FLUID_UNIT_MIN;
		}
		
		
		
		
		
		VolumeWrapper* curVW;
		
		// if (isBlockHolder) {
		// 	curVW = (singleton->volumeWrappers[E_VW_WORLD]);
		// 	cout << "genBlockHolder\n";
		// }
		// else {
			curVW = (singleton->volumeWrappers[E_VW_HOLDER]);
		//}
		
		
		singleton->gw->drawVol(
			curVW,
			&gphMinInCells,
			&gphMaxInCells,
			true,
			true,
			false
			//,isBlockHolder
		);
		FBOWrapper* fbow = curVW->fboSet.getFBOWrapper(0);
		
		// if (isBlockHolder) {
		// 	
		// 	return;
		// }
		
		// if (terVW == NULL) {
		// 	terVW = new VolumeWrapper();
		// 	terVW->init(cellsPerHolder,GL_REPEAT,false);
		// }
		// FBOWrapper* fbow = terVW->fboSet.getFBOWrapper(0);
		// singleton->gw->drawVol(terVW, &gphMinInCells, &gphMaxInCells, true, true);
		
		
		unsigned char* vdPtr = fbow->pixelsChar;
		
		
		float watHeight = singleton->getSeaHeightScaled();
		
		int cdo4 = cellDataSize/4;
		int curInd;
		ObjectStruct* curObj;
		
		gatherObjects();
		
		
		
		
		
		for (p = 0; p < cdo4; p++) {
			
			kk = p/(cellsPerHolder*cellsPerHolder);
			jj = (p-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			ii = p-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
			
			// if (isBlockHolder) {
			// 	fk = gphMinInCells[2] + kk*singleton->cellsPerBlock;
			// }
			// else {
				fk = gphMinInCells[2] + kk;
			//}
			
			
			
			ind = p*4;
			
			
			
			if (
				vdPtr[ind + 0] > 128 // 0.5f
			) {
				iTer = FLUID_UNIT_MAX;
			}
			else {
				iTer = FLUID_UNIT_MIN;
			}
			
			if (iTer == FLUID_UNIT_MAX) {
				iWat = FLUID_UNIT_MIN;
			}
			else {
				if (fk < watHeight) {
					iWat = FLUID_UNIT_MAX;
				}
				else {
					iWat = FLUID_UNIT_MIN;
				}
			}
			
			
			
			
			for (q = 0; q < 4; q++) {
				cellData[ind+q] = FLUID_UNIT_MIN;
				extrData[ind+q] = FLUID_UNIT_MIN;
			}
			
			
			cellData[ind+E_PTT_TER] = iTer;
			cellData[ind+E_PTT_WAT] = iWat;
			extrData[ind+E_PTT_LST] = iWat;
			
			
			
			
			
			
			
			
			
			
		}
		
		
		
		
		
		
		
		
		
		int cph2 = cellsPerHolder/2;
		int readInd;
		
		for (k = 0; k < cph2; k++) {
			for (j = 0; j < cph2; j++) {
				for (i = 0; i < cph2; i++) {
					
					for (kk = 0; kk < 2; kk++) {
						for (jj = 0; jj < 2; jj++) {
							for (ii = 0; ii < 2; ii++) {
								p = 
									(k*2+kk)*cellsPerHolder*cellsPerHolder +
									(j*2+jj)*cellsPerHolder +
									(i*2+ii);
								ind = p*4;
								
								p = 
									(k*2)*cellsPerHolder*cellsPerHolder +
									(j*2)*cellsPerHolder +
									(i*2);
								readInd = p*4;
								
								for (q = 0; q < 4; q++) {
									cellData[ind+q] = cellData[readInd+q];
									extrData[ind+q] = extrData[readInd+q];
								}
								
							}
						}
					}
					
				}
			}
		}
		
		
		
		
		
		
		vec3 gphMin = gphMinInCells.getVec3();
		vec3 fWorldPosCell;
		float primRes;
		vec3 halfOff = vec3(0.5f);
		
		for (p = 0; p < cdo4; p++) {
			
			kk = p/(cellsPerHolder*cellsPerHolder);
			jj = (p-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			ii = p-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
			
			ind = p*4;
			
			fWorldPosCell = gphMin + vec3(ii,jj,kk) + halfOff;
			
			for (r = 0; r < objectOrder.size(); r++) {
				curInd = objectOrder[r].v0;
				curObj = &(tempObjects[curInd]);
				
				primRes = primDis(fWorldPosCell,curObj);
				
				if (
					primRes <= 0.0f
					//fWorldPosCell.distance(curObj->data[E_OSD_CENTER]) <= curObj->data[E_OSD_RADIUS].x
				) {
										
					switch (curObj->addType) {
						case E_BRUSH_MOVE:
							
						break;
						case E_BRUSH_ADD:
							cellData[ind+curObj->objType] = FLUID_UNIT_MAX;
						break;
						case E_BRUSH_SUB:
							cellData[ind+curObj->objType] = FLUID_UNIT_MIN;
						break;
					}
					
				}
				
				
			}
			
			
		}
		
		
		
		
		
		
		
		
		wasGenerated = true;
		
		
		
	}
void GamePageHolder::bindPD (int pd)
                            {
		curPD = pd;
		singleton->pdPool[curPD].isFree = false;
		
		singleton->pdPool[curPD].boundToHolder.v0 = blockId;
		singleton->pdPool[curPD].boundToHolder.v1 = chunkId;
		singleton->pdPool[curPD].boundToHolder.v2 = holderId;
	}
void GamePageHolder::unbindPD ()
                        {
		if (curPD != -1) {
			singleton->pdPool[curPD].isFree = true;
		}
		
		curPD = -1;
	}
void GamePageHolder::applyFill ()
                         {
		bool res;
		
		if (hasCache) {
			res = singleton->loadCacheEntry(blockId,chunkId,holderId);
			listEmpty = (vertexVec.size() == 0); //vboWrapper.
			
			// if (res) {
			// 	cout << "loaded cache\n";
			// }
			// else {
			// 	cout << "did not load cache\n";
			// }
		}
		
		if (
			listEmpty || (!hasData)
			// && (holderFlags != E_CD_SOLID)
		) {
			
		}
		else {
			
			if (POLYS_FOR_CELLS||DO_VOXEL_WRAP) {
				if (hasCache) {
					
				}
				else {
					
					if (DO_CACHE) {
						res = singleton->saveCacheEntry(blockId,chunkId,holderId);
					}
					
					
					// if (res) {
					// 	//cout << "saved cache\n";
					// }
					// else {
					// 	//cout << "did not save cache\n";
					// }
				}
			}
			
			
			
			// else if ((!isBlockHolder)&&POLY_COLLISION) {
			// 	createMesh();
			// }
			// else if (isBlockHolder&&GEN_POLYS_WORLD) {
				
			// }
			
		}
		
		
	}
void GamePageHolder::fillVBO ()
                       {
		
		if (singleton->gamePhysics == NULL) {
			return;
		}
		
		// if (curPD > -1) {
		// 	cout << "nodeCount " << singleton->octPool[curPD]->nodeCount << "\n";
		// }
		
		
		int q;
		
		int p;
		int ii;
		int jj;
		int kk;
		
		int p2;
		// int ii2;
		// int jj2;
		int kk2;
		
		
		
		
		
		/////////////////////
		
		if (
			(holderFlags == E_CD_SOLID) ||
			(holderFlags == E_CD_EMPTY)	
		) {
			if (cellData != NULL) {delete[] cellData;}
			if (extrData != NULL) {delete[] extrData;}
			
			cellData = NULL;
			extrData = NULL;
			
			hasData = false;
		}
		else {
			//hasData = true;
		}
		
		
		
		/////////////////////
		
		
		//float fk;
		
		// if (GEN_COLLISION) {
		// 	for (q = 0; q < collideIndices.size(); q += 2) {
				
		// 		p = collideIndices[q];
		// 		p2 = collideIndices[q+1];
				
		// 		kk = p/(cellsPerHolder*cellsPerHolder);
		// 		jj = (p-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		// 		ii = p-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
				
		// 		kk2 = p2/(cellsPerHolder*cellsPerHolder);
				
		// 		fk = (kk2-kk)+1;
				
				
		// 		btTransform trans;
		// 		trans.setOrigin(btVector3(
		// 			ii,jj,(kk+kk2)*0.5f
		// 		));
				
		// 		// todo: mem leak
				
		// 		btCapsuleShapeZ* capsuleShape = new btCapsuleShapeZ(0.5f,fk);
		// 		btRigidBody* myBody = singleton->gamePhysics->example->createRigidBody(0,trans,capsuleShape);
		// 		myBody->setFriction(btScalar(0.9));
				
		// 		// q3BoxDef boxDef;
		// 		// boxDef.SetRestitution( 0 );
		// 		// q3Transform tx;
		// 		// q3Identity( tx );
		// 		// tx.position.Set(ii,jj,(kk+kk2)*0.5f);
		// 		// boxDef.Set( tx, q3Vec3( 1.0f, 1.0f, fk ) );
		// 		// body->AddBox( boxDef );
				
				
		// 	}
		// }
		
		//singleton->gamePhysics->example->updateGraphicsObjects();
		
		//cout << "collideIndices.size() " << collideIndices.size() << "\n";
		
		
		
		////////////////////
		
		applyFill();
		
		
		
		
	}
int GamePageHolder::gatherData ()
                         {
		int i;
		int j;
		int k;
		
		int cellVal;
		uint tempHF = E_CD_UNKNOWN;
		
		// int empCount = 0;
		// int watCount = 0;
		// int solCount = 0;
		// int errCount = 0;
		
		PaddedDataEntry* curData;
		
		int iX;
		int iY;
		int iZ;
		
		int minRad = 0;
		int cellsPerHolderPad = singleton->cellsPerHolderPad;
		int maxRad = cellsPerHolderPad;
		int paddingInCells = singleton->paddingInCells;
		
		float terVal = 0.0f;
		
		int cData0[4];
		int cData1[4];
		
		// if (isBlockHolder) {
		// 	minRad = 0;
		// 	maxRad = cellsPerHolder-1;
		// }
		
		
		for (k = minRad; k < maxRad; k++) {
			for (j = minRad; j < maxRad; j++) {
				for (i = minRad; i < maxRad; i++) {
					
					iX = gphMinInCells.getIX() + i;
					iY = gphMinInCells.getIY() + j;
					iZ = gphMinInCells.getIZ() + k;
					
					// if (isBlockHolder) {
						
						
					// 	//cellVal = getCellAtCoordsLocal(iX,iY,iZ);
						
					// 	// switch (cellVal) {
					// 	// 	case E_CD_EMPTY:
					// 	// 		empCount++;
					// 	// 	break;
					// 	// 	case E_CD_WATER:
					// 	// 		watCount++;
					// 	// 	break;
					// 	// 	case E_CD_SOLID:
					// 	// 		solCount++;
					// 	// 	break;
					// 	// 	default:
					// 	// 		errCount++;
					// 	// 	break;
					// 	// }
						
					// }
					// else {
						cellVal = singleton->gw->getCellAtCoords(
							iX - paddingInCells,
							iY - paddingInCells,
							iZ - paddingInCells
						);
					//}
					
					singleton->gw->getArrAtCoords(
						iX - paddingInCells,
						iY - paddingInCells,
						iZ - paddingInCells,
						cData0,
						cData1
					);
					
					
					tempHF = tempHF|cellVal;
					
					if (cData0[E_PTT_TER] != FLUID_UNIT_MIN) {
						terVal = 1.0f;
					}
					else {
						terVal = 0.0f;
					}
					
					curData = &(
						singleton->pdPool[curPD].data[
							i + j*cellsPerHolderPad + k*cellsPerHolderPad*cellsPerHolderPad
						]	
					);
					
					curData->terVal = terVal;
					curData->cellVal = cellVal;
					curData->visited = false;
					
				}
			}	
		}
		
		return tempHF;
	}
bool GamePageHolder::checkCache ()
                          {
		if (DO_CACHE) {
			hasCache = singleton->checkCacheEntry(blockId,chunkId,holderId);
		}
		else {
			hasCache = false;
		}
		
		return hasCache;
	}
void GamePageHolder::generateList ()
                            {
		//PaddedData* pd = &(singleton->pdPool[curPD]);
		
		if (singleton->gamePhysics == NULL) {
			return;
		}
		
		
		uint tempHF = gatherData();		
		
		vertexVec.clear();//beginFill();
		
		if (
			(
				(tempHF == E_CD_SOLID) ||
				(tempHF == E_CD_EMPTY) ||
				(tempHF == E_CD_WATER)	
			) &&
			(
				(tempObjects.size() == 0)
			)
		) {
			
		}
		else {
			
			if (DO_VOXEL_WRAP) {
				if (hasCache) {
					
				}
				else {
					voxelWrap->process(this);
				}
				
			}
			
			if (POLYS_FOR_CELLS) {
				wrapPolys();
			}
			
		}
		
		
		listEmpty = (vertexVec.size() == 0); //vboWrapper.
		
		// if (DO_VOXEL_WRAP) {
		// 	listEmpty = (cubeWraps.size() <= 0);
		// }
		
		holderFlags = tempHF;
		
		
	}
void GamePageHolder::wrapPolys ()
                         {
		
		int cellsPerHolderM1 = cellsPerHolder-1;
		float fres = cellsPerHolder;
		int i, j, k, m, q;
		
		int curInd;
		
		
		
		
		bool edgeK;
		bool edgeJ;
		bool edgeI;
		
		
		int ii;
		int jj;
		int kk;
		
		
		float cellPitch;
		
		// if (isBlockHolder) {
		// 	cellPitch = singleton->cellsPerBlock;
		// }
		// else {
			cellPitch = 1.0f;
		// }
		
		float iv0 = 0.0f;
		float iv1 = cellPitch;
		
		
		float fi = 0.0f;
		float fj = 0.0f;
		float fk = 0.0f;
		
		int iX;
		int iY;
		int iZ;
		
		float bpX = 0.0f;
		float bpY = 0.0f;
		float bpZ = 0.0f;
		
		int cellVal;
		int cellVal2;
		
		
		uint dirFlags;
		uint flagVals[6];
		flagVals[0] = 1;
		flagVals[1] = 2;
		flagVals[2] = 4;
		flagVals[3] = 8;
		flagVals[4] = 16;
		flagVals[5] = 32;
		
		const uint AIR_VAL = 0;
		
		
		bool doProcAny = false;
		bool doProc[6];// = false;
		
		for (i = 0; i < 6; i++) {
			doProc[i] = false;
		}
		
		
		uint procFlag = 0;

		
		
		bool rleOn = false;
		bool isLast = false;
		int begInd;
		int endInd;
		
		int baseInd;
		int tempVal;
		int cellGrid[27];
		int maskVals[8];
		int newInd;
		
		
		
		
		for (k = 0; k < cellsPerHolder; k++) {
			
			for (j = 0; j < cellsPerHolder; j++) {
				
				for (i = 0; i < cellsPerHolder; i++) {
					
					
					iX = gphMinInCells.getIX() + i;
					iY = gphMinInCells.getIY() + j;
					iZ = gphMinInCells.getIZ() + k;
					bpX = iX;
					bpY = iY;
					bpZ = iZ;
					
					// if (isBlockHolder) {
					// 	cellVal = getCellAtCoordsLocal(iX,iY,iZ);
					// }
					// else {
						cellVal = getPadData(i,j,k)->cellVal;
					//}
					
					newInd = i + j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder;
					
					//extrData[newInd*4 + E_PTT_FLG] = 0;
					
					if ( cellVal == E_CD_SOLID ) {
						
						doProcAny = false;
						
						procFlag = 0;
						
						for (q = 0; q < NUM_ORIENTATIONS; q++) {
							
							// if (isBlockHolder) {
							// 	cellVal2 = getCellAtCoordsLocal(
							// 		iX + DIR_VECS_I[q][0],
							// 		iY + DIR_VECS_I[q][1],
							// 		iZ + DIR_VECS_I[q][2]	
							// 	);
							// }
							// else {
								cellVal2 = getPadData(
									i + DIR_VECS_I[q][0],
									j + DIR_VECS_I[q][1],
									k + DIR_VECS_I[q][2]
								)->cellVal;
							//}
							
							
							doProc[q] = cellVal2 != E_CD_SOLID;
							if (doProc[q]) {
								procFlag = procFlag | procFlags[q];
							}
							
							
							doProcAny = doProcAny | doProc[q];
							
						}
						
						// if (doProcAny) {
						// 	extrData[newInd*4 + E_PTT_FLG] =
						// 		extrData[newInd*4 + E_PTT_FLG] |
						// 		E_PTTF_SURFACE |
						// 		procFlag;
						// }
						
						// if (GEN_COLLISION) {
						// 	if (doProcAny) {
						// 		collideIndices.push_back(i + j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder);
						// 	}
						// }
						
						
						
						// if (doProcAny) {
							
						// 	// gather nearest 27 points for mask
							
						// 	for (kk = -1; kk <= 1; kk++) {
						// 		for (jj = -1; jj <= 1; jj++) {
						// 			for (ii = -1; ii <= 1; ii++) {
										
						// 				if (
						// 					singleton->gw->getCellAtCoords(
						// 						iX + ii,
						// 						iY + jj,
						// 						iZ + kk
						// 					) == E_CD_SOLID
						// 				) {
						// 					tempVal = 1;
						// 				}
						// 				else {
						// 					tempVal = 0;
						// 				}
										
										
						// 				cellGrid[
						// 					(ii+1) +
						// 					(jj+1)*3 +
						// 					(kk+1)*9	
						// 				] = tempVal;
						// 			}	
						// 		}
						// 	}
							
						// 	for (kk = 0; kk < 2; kk++) {
						// 		for (jj = 0; jj < 2; jj++) {
						// 			for (ii = 0; ii < 2; ii++) {
						// 				baseInd = ii + jj*3 + kk*9;
										
										
						// 				maskVals[ii+jj*2+kk*4] = 
						// 				((cellGrid[baseInd+0+0+0])<<0) |
						// 				((cellGrid[baseInd+1+0+0])<<1) |
						// 				((cellGrid[baseInd+0+3+0])<<2) |
						// 				((cellGrid[baseInd+1+3+0])<<3) |
										
						// 				((cellGrid[baseInd+0+0+9])<<4) |
						// 				((cellGrid[baseInd+1+0+9])<<5) |
						// 				((cellGrid[baseInd+0+3+9])<<6) |
						// 				((cellGrid[baseInd+1+3+9])<<7);
										
						// 			}	
						// 		}
						// 	}
							
						// }
						
						if (doProcAny) {
							// vboWrapper.vboBox(
							// 	bpX,bpY,bpZ,
							// 	iv0,iv1,
							// 	procFlag,
							// 	ZERO_FLOATS,
							// 	4
							// );
						}
						
						
					}
				}
			}
		}
		
		
		
	}
#undef LZZ_INLINE
 
