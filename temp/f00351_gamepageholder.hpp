

class GamePageHolder {
private:
	int* pathData;
	int* cellData;
	int* extrData;
public:

	bool listGenerated;
	bool listEmpty;
	bool hasData;
	bool hasPath;
	
	bool lockWrite;
	bool lockRead;
	
	VBOWrapper vboWrapper;
	
	VolumeWrapper* terVW;
	
	//GLuint holderDL;
	
	int blockId;
	int holderId;
	
	
	int pathSize;
	int totIdealNodes;
	int totGroupIds;
	
	
	
	int cellDataSize;
	int cellsPerHolder;
	
	int visitId;
	
	bool pathsInvalid;
	bool idealPathsInvalid;
	
	bool pathsReady;
	bool idealPathsReady;
	
	uint holderFlags;
	
	std::vector<int> indexStack; // temp buffer used for flood fill
	std::vector<GroupIdStruct> groupIdStack; // stores all ids within a group
	std::vector<GroupInfoStruct> groupInfoStack; // stores all info about one group
	std::vector<ConnectingNodeStruct> bestConnectingNodes; // best connections between groups
	
	std::vector<float> vertexVec;
	std::vector<uint> indexVec;
	
	std::vector<GameEnt *> entityGeom;
	int entityGeomCounter;
	FIVector4 offsetInHolders;

	FIVector4 gphMinInPixels;
	FIVector4 gphMaxInPixels;
	FIVector4 gphCenInPixels;

	FIVector4 offsetInBlocks;
	FIVector4 origOffset;

	


	Singleton* singleton;

	intPairVec containsEntIds[E_ET_LENGTH];
	
	bool wasGenerated;





	




	GamePageHolder() {
		
		hasData = true;
		hasPath = true;
		
		terVW = NULL;
		
		holderFlags = E_CD_UNKNOWN;
		
		listEmpty = true;
		listGenerated = false;
		pathsInvalid = true;
		idealPathsInvalid = true;
		
		pathsReady = false;
		idealPathsReady = false;
		cellData = NULL;
		extrData = NULL;
		pathData = NULL;
		wasGenerated = false;
	}
	
	
	
	void init(
		Singleton* _singleton,
		int _blockId, 			// MUST BE UNIQUE FOR ENTITES
		int _holderId,
		
		int trueX,
		int trueY,
		int trueZ

	) {

		//cout << "gph init\n";

		lockRead = false;
		lockWrite = false;

		entityGeomCounter = 0;
		totIdealNodes = 0;
		totGroupIds = 0;

		visitId = 0;

		int i;
		int j;
		int k;
		int ind;
		
		

		blockId = _blockId;
		holderId = _holderId;

		singleton = _singleton;


		cellsPerHolder = singleton->cellsPerHolder;
		
		pathSize = cellsPerHolder*cellsPerHolder*cellsPerHolder;
		cellDataSize = cellsPerHolder*cellsPerHolder*cellsPerHolder*4;

		origOffset.setFXYZ(cellsPerHolder/2,cellsPerHolder/2,cellsPerHolder/2);

		offsetInHolders.setIXYZ(trueX,trueY,trueZ);
		offsetInBlocks.copyFrom(&offsetInHolders);
		offsetInBlocks.intDivXYZ(singleton->holdersPerBlock);
		
		gphMinInPixels.copyFrom(&offsetInHolders);
		gphMaxInPixels.copyFrom(&offsetInHolders);

		gphMaxInPixels.addXYZ(1);

		gphMinInPixels.multXYZ(cellsPerHolder);
		gphMaxInPixels.multXYZ(cellsPerHolder);

		gphCenInPixels.averageXYZ(&gphMaxInPixels,&gphMinInPixels);
		

		
		
		fetchHolderGeom();
		
	}
	
	
	

	int getCellAtInd(
		int ind
	) {
		
		
		int q;
		
		
		if (wasGenerated) {
			
		}
		else {
			//cout << debugVal << " attempted getCellAtCoords without generation\n";
			//genCellData();
			
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
				(extrData[ind+E_PTT_BLD] != FLUID_UNIT_MIN)
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


	// void setCellAtInd(
	// 	int ind
	// ) {
			
	// 		int q;
		
	// 		if (wasGenerated) {
				
	// 		}
	// 		else {
	// 			cout << "attempted setCellAtInd without generation\n";
	// 			//genCellData();
	// 			return;
	// 		}
	// 		//GamePage* gp = getPageAtCoords(x2, y2, z2, false);
			
	// 		if (
	// 			(holderFlags == E_CD_EMPTY) || 
	// 			(holderFlags == E_CD_WATER) || 
	// 			(holderFlags == E_CD_SOLID)	
	// 		) {
	// 			// todo: fix this
	// 			// cout << "Holder is all one type\n";
	// 			return;
	// 		}
			
			
			
	// }
	
	
	
	void getArrAtInd(
		int ind,
		int* tempCellData,
		int* tempCellData2
	) {
		
		
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
						tempCellData[E_PTT_LST] = FLUID_UNIT_MAX;
					break;
					case E_CD_SOLID:
						tempCellData[E_PTT_TER] = FLUID_UNIT_MAX;
					break;
				}
			}
			
			
		}
		
		
	}

	void setArrAtInd(
		int ind,
		int* tempCellData = NULL,
		int* tempCellData2 = NULL
	) {
			
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
	
	
	
	
	void clearPathPreserve() {
		int i;
		
		if (hasPath) {
			for (i = 0; i < pathSize; i++) {
					if (pathData[i] > 0) {
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
	
	void clearPathSizes() {
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
	
	void checkData(bool checkPath) {
		
		if (hasData) {
			if (cellData == NULL) {
				cellData = new int[cellDataSize];
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
	
	void clearGroupFlags(int targId) {
		int i;
		for (i = 0; i < groupInfoStack.size(); i++) {
			groupInfoStack[i].visitId = targId;
		}
	}

	void floodFillAtInd(int firstInd, int newId, bool findCenter, GroupInfoStruct* curGI) {
		
		
		
		
		indexStack.clear();
		indexStack.push_back(firstInd);
		groupIdStack.push_back(GroupIdStruct());
		groupIdStack.back().ind = firstInd;
		groupIdStack.back().groupId = newId;
		groupIdStack.back().cameFromInd = -1;
		groupIdStack.back().pathCost = 0;
		
		bool doProc;
		
		int groupIdStackInd;
		int dir;
		int ind;
		int testInd;
		int i;
		int j;
		int k;
		int n;
		int q;
		int foundInd;
		int testI;
		int testJ;
		int testK;
		int cellsPerHolderM1 = cellsPerHolder-1;
		int lastCost;
		
		// for (i = 0; i < 6; i++) {
		// 	curGI->touchesFace[i] = false;
		// }
		curGI->begInd = groupIdStack.size() - 1;
		
		
		// find contiguous regions
		while (indexStack.size() > 0) {
			
			ind = indexStack.back();
			
			//.groupId
			pathData[ind] = groupIdStack.size()-1;
			
			k = ind/(cellsPerHolder*cellsPerHolder);
			j = (ind - k*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			i = ind - ( j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder );
			
			foundInd = -1;
			
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
						
						
						foundInd = testInd;
						goto NEXT_FILL_POINT;
					}
				}
			}

NEXT_FILL_POINT:
			if (foundInd >= 0) {
				indexStack.push_back(foundInd);
				
				lastCost = groupIdStack.back().pathCost;
				
				groupIdStack.push_back(GroupIdStruct());
				groupIdStack.back().ind = foundInd;
				groupIdStack.back().groupId = newId;
				groupIdStack.back().cameFromInd = ind;
				groupIdStack.back().pathCost = lastCost+1;
			}
			else {
				indexStack.pop_back();
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
	}
	
	void findIdealNodes() {
			idealPathsReady = false;
			
			
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
								3,
								gphMinInPixels.getIX()+i,
								gphMinInPixels.getIY()+j,
								gphMinInPixels.getIZ()+k
							);
							cellValAbove = singleton->gw->getCellAtCoords(
								4,
								gphMinInPixels.getIX()+i,
								gphMinInPixels.getIY()+j,
								gphMinInPixels.getIZ()+k+1
							);
							// cellValAbove2 = singleton->gw->getCellAtCoords(
							// 	gphMinInPixels.getIX()+i,
							// 	gphMinInPixels.getIY()+j,
							// 	gphMinInPixels.getIZ()+k+2
							// );
							cellValBelow = singleton->gw->getCellAtCoords(
								5,
								gphMinInPixels.getIX()+i,
								gphMinInPixels.getIY()+j,
								gphMinInPixels.getIZ()+k-1
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
				
				
				
				
				
				for (i = 0; i < pathSize; i++) {
					if (
						//.groupId
						(pathData[i] == GROUP_ID_UNMARKED_IDEAL)
						// && ((pathData[i].flags&E_PVT_LAND_BELOW) != 0)
					) {
						
						groupInfoStack.push_back(GroupInfoStruct());
						groupInfoStack.back().visitId = 0;
						floodFillAtInd(i,totGroupIds,true,&(groupInfoStack.back()));
						totGroupIds++;
					}
				}
				
				clearPathPreserve();
				
				for (i = 0; i < groupInfoStack.size(); i++) {
					floodFillAtInd(groupInfoStack[i].centerInd,totGroupIds,false,&(groupInfoStack[i]));
					totGroupIds++;
				}
			}
			
			
			
			
			
			idealPathsReady = true;
			
	}
	
	int getGroupId(int pathDataIndex) {
		
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
	
	GroupIdStruct* getInfo(int pathDataIndex) {
		
		if (hasPath) {
			if (pathData[pathDataIndex] < 0) {
				cout << "pathDataIndex < 0\n";
				return NULL;
			}
			
			return &(groupIdStack[pathData[pathDataIndex]]);
			
		}
		else {
			return NULL;
		}
		
		
	}
	
	
	void linkRegions() {
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
		
		
		int ind;
		int mo2;
		
		int holderInd;
		
		int cellsPerHolderM1 = cellsPerHolder-1;
		GamePageHolder* holderArr[NUM_ORIENTATIONS];
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
		
		boundsArr[0] = cellsPerHolderM1;
		boundsArr[1] = 0;
		boundsArr[2] = cellsPerHolderM1;
		boundsArr[3] = 0;
		boundsArr[4] = cellsPerHolderM1;
		boundsArr[5] = 0;
		
		for (i = 0; i < NUM_ORIENTATIONS; i++) {
			holderArr[i] = singleton->gw->getHolderAtCoords(
				offsetInHolders.getIX()+DIR_VECS_I[i][0],
				offsetInHolders.getIY()+DIR_VECS_I[i][1],
				offsetInHolders.getIZ()+DIR_VECS_I[i][2],
				true
			);
		}
		
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
			
			for (m = 0; m < NUM_ORIENTATIONS; m++) {
				mo2 = m/2;
				
				if (indArr[mo2] == boundsArr[m]) {
					
					indArrInv[0] = i;
					indArrInv[1] = j;
					indArrInv[2] = k;
					
					if (mo2 == 0) {indArrInv[0] = cellsPerHolderM1-i;}
					if (mo2 == 1) {indArrInv[1] = cellsPerHolderM1-j;}
					if (mo2 == 2) {indArrInv[2] = cellsPerHolderM1-k;}
					
					targetInd = indArrInv[0] + indArrInv[1]*cellsPerHolder + indArrInv[2]*cellsPerHolder*cellsPerHolder;
					
					
					
					targetGroupId = holderArr[m]->getGroupId(targetInd);
					
					if (targetGroupId > -1) {
						targetCost = holderArr[m]->getInfo(targetInd)->pathCost + getInfo(ind)->pathCost;
						
						// first see if another connection to this block/holder/groupId exists
						
						notFound = true;
						foundBetter = false;
						
						for (q = 0; q < bestConnectingNodes.size(); q++) {
							cNode = &(bestConnectingNodes[q]);
							
							if (
								(cNode->blockIdTo == holderArr[m]->blockId) &&
								(cNode->holderIdTo == holderArr[m]->holderId) &&
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
							
							cNode->blockIdTo = holderArr[m]->blockId;
							cNode->holderIdTo = holderArr[m]->holderId;
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
	
	bool prepPathRefresh(int rad) {
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
	
	void refreshPaths() {
		
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
		
		
		if (groupInfoStack.size() == 0) {
			if (pathData != NULL) {delete[] pathData;}
			pathData = NULL;
			hasPath = false;
		}
		
		
		
		
		
		pathsReady = true;
		
		
		
		
		//singleton->gameLogic->insertNode(holderId,blockId);
		
	}
	
	
	void genCellData() {
		
		
		int i;
		int j;
		int k;
		
		int ii;
		int jj;
		int kk;
		
		int p;
		int q;
		
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
		int iSimp;
		int iSimp2;
		
		checkData(false);
		
		for (i = 0; i < cellDataSize; i++) {
			cellData[i] = FLUID_UNIT_MIN;
			extrData[i] = FLUID_UNIT_MIN;
		}
		
		
		
		VolumeWrapper* curVW = (singleton->volumeWrappers[E_VW_HOLDER]);
		FBOWrapper* fbow = curVW->fboSet.getFBOWrapper(0);
		singleton->gw->drawVol(curVW, &gphMinInPixels, &gphMaxInPixels, true, true);
		
		
		// if (terVW == NULL) {
		// 	terVW = new VolumeWrapper();
		// 	terVW->init(cellsPerHolder,GL_REPEAT,false);
		// }
		// FBOWrapper* fbow = terVW->fboSet.getFBOWrapper(0);
		// singleton->gw->drawVol(terVW, &gphMinInPixels, &gphMaxInPixels, true, true);
		
		
		unsigned char* vdPtr = fbow->pixelsChar;
		
		
		float watHeight = singleton->getSeaHeightScaled();
		
		int cdo4 = cellDataSize/4;
		
		for (p = 0; p < cdo4; p++) {
			
			kk = p/(cellsPerHolder*cellsPerHolder);
			jj = (p-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			ii = p-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
			
			fk = gphMinInPixels[2] + kk;
			
			ind = p*4;
			
			if (
				vdPtr[ind + 0] > 128 // 0.5f
			) {
				iSimp = FLUID_UNIT_MAX;
			}
			else {
				iSimp = FLUID_UNIT_MIN;
			}
			
			if (iSimp == FLUID_UNIT_MAX) {
				iSimp2 = FLUID_UNIT_MIN;
			}
			else {
				if (fk < watHeight) {
					iSimp2 = FLUID_UNIT_MAX;
				}
				else {
					iSimp2 = FLUID_UNIT_MIN;
				}
			}
			
			
			for (q = 0; q < 4; q++) {
				cellData[ind+q] = FLUID_UNIT_MIN;
				extrData[ind+q] = FLUID_UNIT_MIN;
			}
			
			
			cellData[ind+E_PTT_TER] = iSimp;
			cellData[ind+E_PTT_WAT] = iSimp2;
			cellData[ind+E_PTT_LST] = iSimp2;
		}
		
		
		wasGenerated = true;
		
	}

	// void genCellData2() {
		
	// 	//cout << "genCellData()\n";
		
	// 	int i;
	// 	int j;
	// 	int k;
		
	// 	int q;
		
	// 	int ind;
		
	// 	float fi;
	// 	float fj;
	// 	float fk;
		
	// 	float zv;
		
	// 	float fiAbs;
	// 	float fjAbs;
	// 	float fkAbs;
		
	// 	float terHeight;
	// 	float simplexVal;
	// 	float simplexVal1;
	// 	float simplexVal2;
		
	// 	float disVal;
		
	// 	float fSimp;
	// 	int iSimp;
	// 	int iSimp2;
		
		
		
		
	// 	float wspX = singleton->worldSizeInCells[0]/2.0f;
	// 	float wspY = singleton->worldSizeInCells[1]/2.0f;
		
	// 	float distanceBelowTer;
		
	// 	checkData();
		
	// 	for (i = 0; i < cellDataSize; i++) {
	// 		cellData[i] = FLUID_UNIT_MIN;
	// 		extrData[i] = FLUID_UNIT_MIN;
	// 	}
		
		
	// 	float watHeight = singleton->getSeaHeightScaled();
		
	// 	float simplexRegion = 200.0f;
	// 	float simplexMod = 0.0f;
		
	// 	for (j = 0; j < cellsPerHolder; j++) {
	// 		fj = abs(j + gphMinInPixels[1]);
	// 		fjAbs = abs(wspY-fj);
	// 		for (i = 0; i < cellsPerHolder; i++) {
	// 			fi = abs(i + gphMinInPixels[0]);
	// 			fiAbs = abs(wspX-fi);
				
				
				
	// 			/*
	// 			for x=0,bufferwidth-1,1 do
	// 				for y=0,bufferheight-1,1 do
	// 					s=x/bufferwidth
	// 					t=y/bufferheight
	// 			    dx=x2-x1
	// 			    dy=y2-y1
						
	// 					nx=x1+cos(s*2*pi)*dx/(2*pi)
	// 					ny=y1+cos(t*2*pi)*dy/(2*pi)
	// 					nz=x1+sin(s*2*pi)*dx/(2*pi)
	// 					nw=y1+sin(t*2*pi)*dy/(2*pi)
						
	// 					buffer:set(x,y,Noise4D(nx,ny,nz,nw))
	// 				end
	// 			end
	// 			*/
				
	// 			/*
	// 			float simplexScaledNoise(
	// 				const float octaves,
	// 				const float persistence,
	// 				const float scale,
	// 				const float loBound,
	// 				const float hiBound,
	// 				const float x,
	// 				const float y,
	// 				const float z,
	// 				const float w
	// 			) {
	// 				return simplexNoise(octaves, persistence, scale, x, y, z, w) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
	// 			}
	// 			*/
				
	// 			// - singleton->worldSizeInCells[0]*0.5f
				
	// 			// zv = simplexScaledNoise(
	// 			// 		4.0f, //octaves
	// 			// 		0.5f, //persistence (amount added in each successive generation)
	// 			// 		1.0f/16.0f, //scale (frequency)
	// 			// 		0.0f, // lo bound
	// 			// 		1.0f, // hi bound
	// 			// 		fi + 2333.2,
	// 			// 		fj + 1352.4,
	// 			// 		4222.3
	// 			// 	);
				
				
	// 			terHeight = singleton->getHeightAtPixelPos(fi,fj);
				
				
				
	// 			for (k = 0; k < cellsPerHolder; k++) {
	// 				fk = (k + gphMinInPixels[2]);
	// 				fkAbs = fk;//abs(j-singleton->heightMapMaxInCells*0.5f);
					
					
	// 				simplexMod = (
	// 					clampfZO(
	// 						1.0f - 
	// 						abs(
	// 							fk-(terHeight-simplexRegion*0.5f)
	// 						)/simplexRegion
	// 					)	
	// 				);
					
	// 				distanceBelowTer = (
	// 					clampfZO(
							
	// 						(
	// 							terHeight-fk
	// 						)/simplexRegion
	// 					)	
	// 				);
					
					
	// 				if (simplexMod > 0.0f) {
	// 					simplexVal = clampfZO(
	// 						simplexScaledNoise(
	// 							4.0f, //octaves
	// 							0.5f, //persistence (amount added in each successive generation)
	// 							1.0f/64.0f, //scale (frequency)
	// 							0.0f, // lo bound
	// 							1.0f, // hi bound
	// 							fiAbs,
	// 							fjAbs,
	// 							fkAbs
	// 						) 
	// 					);
	// 				}
	// 				else {
	// 					simplexVal = 0.0f;
	// 				}
					
					
					
	// 				//simplexVal = simplexVal*2.0f-1.0f;
					
	// 				// simplexVal2 = clampfZO(
	// 				// 	simplexScaledNoise(
	// 				// 		5.0f, //octaves
	// 				// 		0.5f, //persistence (amount added in each successive generation)
	// 				// 		1.0f/128.0f, //scale (frequency)
	// 				// 		0.0f, // lo bound
	// 				// 		1.0f, // hi bound
	// 				// 		fiAbs,
	// 				// 		fjAbs,
	// 				// 		fkAbs
	// 				// 	) 
	// 				// );
					
	// 				// disVal = (fi/wspX)*(fj/wspY);
					
					
					
	// 				// simplexVal = mixf(simplexVal1,simplexVal2,disVal);
					
	// 				if (
	// 					//fk < terHeight
	// 					((terHeight + simplexVal*simplexMod*800.0f) - (fk+100.0f+300.0f*distanceBelowTer)) > 0.0f
	// 				) {
	// 					iSimp = FLUID_UNIT_MAX;
	// 				}
	// 				else {
	// 					iSimp = FLUID_UNIT_MIN;
	// 				}
					
	// 				if (iSimp == FLUID_UNIT_MAX) {
	// 					iSimp2 = FLUID_UNIT_MIN;
	// 				}
	// 				else {
	// 					if (fk < watHeight) {
	// 						iSimp2 = FLUID_UNIT_MAX;
	// 					}
	// 					else {
	// 						iSimp2 = FLUID_UNIT_MIN;
	// 					}
	// 				}
					
					
					
					
					
					
	// 				// if (fSimp > 0.5) {
	// 				// 	iSimp = FLUID_UNIT_MAX;
	// 				// }
	// 				// else {
	// 				// 	iSimp = FLUID_UNIT_MIN;
	// 				// }
					
					
	// 				ind = (i + j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder)*4;
					
	// 				for (q = 0; q < 4; q++) {
	// 					cellData[ind+q] = FLUID_UNIT_MIN;
	// 					extrData[ind+q] = FLUID_UNIT_MIN;
	// 				}
					
					
	// 				cellData[ind+0] = iSimp;
	// 				cellData[ind+1] = iSimp2;
	// 				cellData[ind+2] = iSimp2;
					
		
		
	// 			}
	// 		}
	// 	}
		
		
	// 	wasGenerated = true;
	// }

	
	void fetchHolderGeom() {
		int i;
		int j;
		int k;
		int n;
		
		int m;
		
		GameBlock* curBlock;
		GamePageHolder* gph;
		FIVector4 start;
		FIVector4 end;
		GameEnt* gameEnt;


		for (n = 0; n < E_ET_LENGTH; n++) {
			containsEntIds[n].data.clear();
			
			for (i = -1; i <= 1; i++) {
				for (j = -1; j <= 1; j++) {
					for (k = -1; k <= 1; k++) {
						curBlock = singleton->gw->getBlockAtCoords(
							offsetInBlocks.getIX()+i,
							offsetInBlocks.getIY()+j,
							offsetInBlocks.getIZ()+k,
							true
						);

						for (m = 0; m < curBlock->gameEnts[n].data.size(); m++) {

							gameEnt = &(curBlock->gameEnts[n].data[m]);

							start.copyFrom( &(gameEnt->moveMinInPixels) );
							end.copyFrom( &(gameEnt->moveMaxInPixels) );

							start.clampZ(0.0,singleton->worldSizeInCells.getFZ()-1.0f);
							end.clampZ(0.0,singleton->worldSizeInCells.getFZ()-1.0f);

							if (FIVector4::intersectInt(&start,&end,&gphMinInPixels,&gphMaxInPixels)) {
								containsEntIds[n].data.push_back(intPair());
								containsEntIds[n].data.back().v0 = curBlock->blockId;
								containsEntIds[n].data.back().v1 = m;
							}
						}
					}
					
				}
			}
		}

	}
	
	
	
	// #################
	
	// void bindHolderDL() {
	// 	if (listGenerated) {
	// 		glDeleteLists(holderDL, 1);
	// 	}
		
	// 	holderDL = glGenLists(1);
	// 	glNewList(holderDL, GL_COMPILE);
	// }
	// void unbindHolderDL() {
	// 	glEndList();
	// 	listGenerated = true;
	// }
	
	
	inline void getIndVal(int procCount) {
		indexVec.push_back(0+procCount*4);
		indexVec.push_back(1+procCount*4);
		indexVec.push_back(2+procCount*4);
		indexVec.push_back(2+procCount*4);
		indexVec.push_back(1+procCount*4);
		indexVec.push_back(3+procCount*4);
	}
	
	
	
	inline void getIndVal2(int procCount) {
		indexVec.push_back(2+procCount*4);
		indexVec.push_back(1+procCount*4);
		indexVec.push_back(0+procCount*4);
		indexVec.push_back(3+procCount*4);
		indexVec.push_back(1+procCount*4);
		indexVec.push_back(2+procCount*4);
	}
	
	inline void getPixVal(
		//FBOWrapper* fbow0,
		//FBOWrapper* fbow1,
		//int ind,
		float xb, float yb, float zb,
		float xm, float ym, float zm
	) {
		
		
		vertexVec.push_back(xb+xm);
		vertexVec.push_back(yb+ym);
		vertexVec.push_back(zb+zm);
		vertexVec.push_back(1.0f);
		
		vertexVec.push_back(xb+xm);
		vertexVec.push_back(yb+ym);
		vertexVec.push_back(zb+zm);
		vertexVec.push_back(1.0f);
		
		
		
		
		// glMultiTexCoord3f(
		// 	GL_TEXTURE0,
		// 	xb+xm,
		// 	yb+ym,
		// 	zb+zm
		// );
		// glMultiTexCoord4f(
		// 	GL_TEXTURE1,
		// 	fbow1->getPixelAtIndex(ind,R_CHANNEL)/255.0f,
		// 	fbow1->getPixelAtIndex(ind,G_CHANNEL)/255.0f,
		// 	fbow1->getPixelAtIndex(ind,B_CHANNEL)/255.0f,
			
		// 	fbow0->getPixelAtIndex(ind,B_CHANNEL) +
		// 	fbow0->getPixelAtIndex(ind,A_CHANNEL)*256
			
		// );
		
		//glVertex3f(xb+xm,yb+ym,zb+zm);
	}


	void fillVBO() {
		
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
		
		
#ifdef GEN_POLYS
		
		if (listEmpty) {
			
		}
		else {
			vboWrapper.init(
				&(vertexVec[0]),
				vertexVec.size(),
				&(indexVec[0]),
				indexVec.size()
			);
			// todo: not needed?
			//glFlush();
			//glFinish();
		}
		
#endif
		
		
		listGenerated = true;
		
	}

	void generateList() { //int fboNum
		
		
		int cellsPerHolderM1 = cellsPerHolder-1;
		float fres = cellsPerHolder;
		int i, j, k, m, q;
		
		//singleton->vgtFBOArr[fboNum].fbos[0].getPixels();
		//singleton->vgtFBOArr[fboNum].fbos[1].getPixels();
		
		int procCount = 0;
		
		bool edgeK;
		bool edgeJ;
		bool edgeI;
		
		
		
		float iv0 = 0.0f;
		float iv1 = 1.0f;
		
		
		
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
		
		
		//FBOWrapper* fbow0 = &(singleton->vgtFBOArr[fboNum].fbos[0]);
		//FBOWrapper* fbow1 = &(singleton->vgtFBOArr[fboNum].fbos[1]);
		
		bool doProc[6];// = false;

		uint tempHF = E_CD_UNKNOWN;

		// bool hasAir = false;
		// bool hasSolid = false;
		// bool allSolid = true;
		
		// for (i = 0; i < cellsPerHolder*cellsPerHolder*cellsPerHolder; i++) {
			
		// 	if (fbow0->getPixelAtIndex(i,A_CHANNEL) == 0) {
		// 		hasAir = true;
		// 	}
		// 	else {
		// 		hasSolid = true;
		// 	}
			
		// 	if (fbow0->getPixelAtIndex(i,R_CHANNEL) != 63) {
		// 		allSolid = false;
		// 	}
		// }
		
		// hasSomething = (hasAir||(!allSolid))&&hasSolid;
		
		// if (hasSomething) {
			
		// }
		// else {
		// 	goto DO_CLEANUP;
		// }
		
		
		
		
		vertexVec.clear();
		indexVec.clear();
		
		//bindHolderDL();
		
		
		//glBegin(GL_QUADS);
		
		
		for (k = -1; k <= cellsPerHolder; k++) {
			for (j = -1; j <= cellsPerHolder; j++) {
				for (i = -1; i <= cellsPerHolder; i++) {
					
					iX = gphMinInPixels.getIX() + i;
					iY = gphMinInPixels.getIY() + j;
					iZ = gphMinInPixels.getIZ() + k;
					
					cellVal = singleton->gw->getCellAtCoords(20,iX,iY,iZ);
					tempHF = tempHF|cellVal;
				}
			}	
		}
		
#ifdef GEN_POLYS
		
		if (
			(tempHF == E_CD_SOLID) ||
			(tempHF == E_CD_EMPTY)	
		) {
			
		}
		else {
			for (k = 0; k < cellsPerHolder; k++) {
				
				for (j = 0; j < cellsPerHolder; j++) {
					
					for (i = 0; i < cellsPerHolder; i++) {
						
						
						iX = gphMinInPixels.getIX() + i;
						iY = gphMinInPixels.getIY() + j;
						iZ = gphMinInPixels.getIZ() + k;
						bpX = iX;
						bpY = iY;
						bpZ = iZ;
						
						cellVal = singleton->gw->getCellAtCoords(6,iX,iY,iZ);
						
						
						if ( cellVal == E_CD_SOLID ) {
							
							for (q = 0; q < NUM_ORIENTATIONS; q++) {
								cellVal2 = singleton->gw->getCellAtCoords(7,
									iX + DIR_VECS_I[q][0],
									iY + DIR_VECS_I[q][1],
									iZ + DIR_VECS_I[q][2]
								);
								doProc[q] = cellVal2 != E_CD_SOLID;
								
							}
							
							
							if (doProc[0]) { // x+
								
								getPixVal(bpX,bpY,bpZ, iv1,iv1,iv1);
								getPixVal(bpX,bpY,bpZ, iv1,iv0,iv1);
								getPixVal(bpX,bpY,bpZ, iv1,iv1,iv0);
								getPixVal(bpX,bpY,bpZ, iv1,iv0,iv0);
								
								getIndVal(procCount);
								procCount++;
								
								
							}
							if (doProc[1]) { // x-
								
								getPixVal(bpX,bpY,bpZ, iv0,iv1,iv1);
								getPixVal(bpX,bpY,bpZ, iv0,iv0,iv1);
								getPixVal(bpX,bpY,bpZ, iv0,iv1,iv0);
								getPixVal(bpX,bpY,bpZ, iv0,iv0,iv0);
								
								getIndVal2(procCount);
								procCount++;
								
							}
							if (doProc[2]) { // y+
								
								getPixVal(bpX,bpY,bpZ, iv1,iv1,iv1);
								getPixVal(bpX,bpY,bpZ, iv0,iv1,iv1);
								getPixVal(bpX,bpY,bpZ, iv1,iv1,iv0);
								getPixVal(bpX,bpY,bpZ, iv0,iv1,iv0);
								
								getIndVal2(procCount);
								procCount++;
								
							}
							if (doProc[3]) { // y-
								
								
								getPixVal(bpX,bpY,bpZ, iv1,iv0,iv1);
								getPixVal(bpX,bpY,bpZ, iv0,iv0,iv1);
								getPixVal(bpX,bpY,bpZ, iv1,iv0,iv0);
								getPixVal(bpX,bpY,bpZ, iv0,iv0,iv0);
								
								getIndVal(procCount);
								procCount++;
							}
							if (doProc[4]) { // z+
								
								getPixVal(bpX,bpY,bpZ, iv1,iv1,iv1);
								getPixVal(bpX,bpY,bpZ, iv0,iv1,iv1);
								getPixVal(bpX,bpY,bpZ, iv1,iv0,iv1);
								getPixVal(bpX,bpY,bpZ, iv0,iv0,iv1);
								
								getIndVal(procCount);
								procCount++;
							}
							if (doProc[5]) { // z-
								
								getPixVal(bpX,bpY,bpZ, iv1,iv1,iv0);
								getPixVal(bpX,bpY,bpZ, iv0,iv1,iv0);
								getPixVal(bpX,bpY,bpZ, iv1,iv0,iv0);
								getPixVal(bpX,bpY,bpZ, iv0,iv0,iv0);
								
								getIndVal2(procCount);
								procCount++;
							}
							
						}
					}
				}
			}
		}

#endif
		
		
		listEmpty = (vertexVec.size() == 0);
		holderFlags = tempHF;
		
		
		
		
		
		
		
	}



};




