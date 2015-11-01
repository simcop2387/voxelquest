// f00351_gamepageholder.h
//

#include "f00351_gamepageholder.e"
#define LZZ_INLINE inline
GamePageHolder::GamePageHolder ()
                         {
		
		hasData = true;
		hasPath = true;
		
		terVW = NULL;
		
		holderFlags = E_CD_UNKNOWN;
		
		listEmpty = true;
		preGenList = false;
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
void GamePageHolder::init (Singleton * _singleton, int _blockId, int _holderId, int trueX, int trueY, int trueZ, bool _isBlockHolder)
          {

		isBlockHolder = _isBlockHolder;

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

		
		if (isBlockHolder) {
			cellsPerHolder = singleton->blocksPerWorld;
		}
		else {
			cellsPerHolder = singleton->cellsPerHolder;
		}
		
		
		
		pathSize = cellsPerHolder*cellsPerHolder*cellsPerHolder;
		cellDataSize = cellsPerHolder*cellsPerHolder*cellsPerHolder*4;

		origOffset.setFXYZ(cellsPerHolder/2,cellsPerHolder/2,cellsPerHolder/2);

		offsetInHolders.setIXYZ(trueX,trueY,trueZ);
		//offsetInBlocks.copyFrom(&offsetInHolders);
		//offsetInBlocks.intDivXYZ(singleton->holdersPerBlock);
		
		gphMinInPixels.copyFrom(&offsetInHolders);
		gphMaxInPixels.copyFrom(&offsetInHolders);

		gphMaxInPixels.addXYZ(1);

		
		
		if (isBlockHolder) {
			gphMinInPixels.multXYZ(singleton->cellsPerWorld);
			gphMaxInPixels.multXYZ(singleton->cellsPerWorld);
			
			doTraceVecND("gphMin",&gphMinInPixels);
			doTraceVecND("gphMax",&gphMaxInPixels);
			
		}
		else {
			gphMinInPixels.multXYZ(cellsPerHolder);
			gphMaxInPixels.multXYZ(cellsPerHolder);
		}
		

		gphCenInPixels.averageXYZ(&gphMaxInPixels,&gphMinInPixels);
		
		if (GEN_COLLISION) {
			// q3BodyDef bodyDef;
			// bodyDef.position.Set( gphMinInPixels[0], gphMinInPixels[1], gphMinInPixels[2] );
			// //bodyDef.axis.Set( 0.0f, 0.0f, 1.0f );
			// //bodyDef.angle = 0.0f;
			// //bodyDef.bodyType = eStaticBody;
			// //bodyDef.angularVelocity.Set( 0.0f, 0.0f, 0.0f );
			// //bodyDef.linearVelocity.Set( 0.0f, 0.0f, 0.0f );
			// body = singleton->gamePhysics->scene->CreateBody( bodyDef );
			
			// q3Transform tx;
			// q3Identity( tx );
			// tx.position.Set(
			// 	cellsPerHolder/2,
			// 	cellsPerHolder/2,
			// 	cellsPerHolder/2
			// );
			
			// q3BoxDef boxDef;
			// boxDef.Set( tx, q3Vec3( 8.0f, 8.0f, 8.0f ) );
			// boxDef.SetRestitution( 0 );
			// body->AddBox( boxDef );
			
			
			q3BodyDef bodyDef;
			bodyDef.position.Set(
				gphMinInPixels[0], gphMinInPixels[1], gphMinInPixels[2]
			);
			body = singleton->gamePhysics->scene->CreateBody( bodyDef );
			
			
			
			
		}
		
		// if (GEN_COLLISION) {
		// 	q3BodyDef bodyDef;
		// 	bodyDef.position.Set(
		// 		gphMinInPixels[0], gphMinInPixels[1], gphMinInPixels[2]
		// 	);
		// 	body = singleton->gamePhysics->scene->CreateBody( bodyDef );
		// }
		
		
		
		
		
		//fetchHolderGeom();
		
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
						tempCellData[E_PTT_LST] = FLUID_UNIT_MAX;
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
void GamePageHolder::clearGroupFlags (int targId)
                                         {
		int i;
		for (i = 0; i < groupInfoStack.size(); i++) {
			groupInfoStack[i].visitId = targId;
		}
	}
void GamePageHolder::floodFillAtInd (int firstInd, int newId, bool findCenter, GroupInfoStruct * curGI)
          {
		
		
		
		
		indexStack.clear();
		
		indexStack.push_back(firstInd);
		groupIdStack.push_back(GroupIdStruct());
		groupIdStack.back().ind = firstInd;
		groupIdStack.back().groupId = newId;
		groupIdStack.back().cameFromInd = -1;
		groupIdStack.back().pathCost = 0;
		
		pathData[firstInd] = groupIdStack.size()-1;
		
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
		int frontInd = 0;
		
		// for (i = 0; i < 6; i++) {
		// 	curGI->touchesFace[i] = false;
		// }
		curGI->begInd = groupIdStack.size()-1;
		
		
		// find contiguous regions
		while (frontInd < indexStack.size()) {//while (indexStack.size() > 0) {
			
			//ind = indexStack.back();
			
			ind = indexStack[frontInd];
			frontInd++;
			
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
						// goto NEXT_FILL_POINT;
					
						if (foundInd >= 0) {
							indexStack.push_back(foundInd);
							
							lastCost = groupIdStack.back().pathCost;
							
							groupIdStack.push_back(GroupIdStruct());
							groupIdStack.back().ind = foundInd;
							groupIdStack.back().groupId = newId;
							groupIdStack.back().cameFromInd = ind;
							groupIdStack.back().pathCost = lastCost+1;
							
							//.groupId
							pathData[foundInd] = groupIdStack.size()-1;
							
						}
					
					}
				}
			}

// NEXT_FILL_POINT:
			if (foundInd >= 0) {
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
void GamePageHolder::findIdealNodes ()
                              {
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
								gphMinInPixels.getIX()+i,
								gphMinInPixels.getIY()+j,
								gphMinInPixels.getIZ()+k
							);
							cellValAbove = singleton->gw->getCellAtCoords(
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
				
				
				//cout << "\n";
				
				for (i = 0; i < pathSize; i++) {
					if (
						//.groupId
						(pathData[i] == GROUP_ID_UNMARKED_IDEAL)
						// && ((pathData[i].flags&E_PVT_LAND_BELOW) != 0)
					) {
						
						groupInfoStack.push_back(GroupInfoStruct());
						groupInfoStack.back().visitId = 0;
						floodFillAtInd(
							i,
							totGroupIds,
							true,
							&(groupInfoStack.back())
						);
						totGroupIds++;
					}
				}
				
				//cout << "groupIdStack.size()1 " << groupIdStack.size() << "\n";
				
				clearPathPreserve();
				
				for (i = 0; i < groupInfoStack.size(); i++) {
					floodFillAtInd(
						groupInfoStack[i].centerInd,
						totGroupIds,
						false,
						&(groupInfoStack[i])
					);
					totGroupIds++;
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
				
				baseX = i + gphMinInPixels.getIX();
				baseY = j + gphMinInPixels.getIY();
				baseZ = k + gphMinInPixels.getIZ();
				
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
									cNode->blockIdTo = targetHolder->blockId;
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
		
		if (isBlockHolder) {
			curVW = (singleton->volumeWrappers[E_VW_WORLD]);
			cout << "genBlockHolder\n";
		}
		else {
			curVW = (singleton->volumeWrappers[E_VW_HOLDER]);
		}
		
		
		singleton->gw->drawVol(curVW, &gphMinInPixels, &gphMaxInPixels, true, true);
		FBOWrapper* fbow = curVW->fboSet.getFBOWrapper(0);
		
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
			
			if (isBlockHolder) {
				fk = gphMinInPixels[2] + kk*singleton->cellsPerBlock;
			}
			else {
				fk = gphMinInPixels[2] + kk;
			}
			
			
			
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
			cellData[ind+E_PTT_LST] = iWat;
		}
		
		
		wasGenerated = true;
		
	}
void GamePageHolder::fillVBO ()
                       {
		
		int q;
		
		int p;
		int ii;
		int jj;
		int kk;
		
		int p2;
		// int ii2;
		// int jj2;
		int kk2;
		
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
		
		
		/////////////////////
		
		
		
		// if (GEN_COLLISION) {
		// 	q3BoxDef boxDef;
		// 	boxDef.SetRestitution( 0 );
		// 	q3Transform tx;
		// 	q3Identity( tx );
		// 	tx.position.Set(2,2,2);
		// 	boxDef.Set( tx, q3Vec3( 8.0f, 8.0f, 8.0f ) );
		// 	body->AddBox( boxDef );
		// }
		
		float fk;
		
		if (GEN_COLLISION) {
			for (q = 0; q < collideIndices.size(); q += 2) {
				
				p = collideIndices[q];
				p2 = collideIndices[q+1];
				
				kk = p/(cellsPerHolder*cellsPerHolder);
				jj = (p-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
				ii = p-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
				
				kk2 = p2/(cellsPerHolder*cellsPerHolder);
				
				fk = (kk2-kk)+1;
				
				q3BoxDef boxDef;
				boxDef.SetRestitution( 0 );
				q3Transform tx;
				q3Identity( tx );
				tx.position.Set(ii,jj,(kk+kk2)*0.5f);
				boxDef.Set( tx, q3Vec3( 1.0f, 1.0f, fk ) );
				body->AddBox( boxDef );
				
				
				// q3BoxDef boxDef;
				// boxDef.SetRestitution( 0 );
				// q3Transform tx;
				// q3Identity( tx );
				// tx.position.Set(ii,jj,kk);
				// boxDef.Set( tx, q3Vec3( 1.0f, 1.0f, 1.0f ) );
				// body->AddBox( boxDef );
				
				
				// q3Identity( tx );
				// tx.position.Set(ii,jj,kk);
				// // tranform, extents
				// boxDef.Set( tx, q3Vec3( 1.0f, 1.0f, 1.0f ) );
				// boxDef.SetRestitution( 0 );
				// body->AddBox( boxDef );
				
			}
		}
		
		//cout << "collideIndices.size() " << collideIndices.size() << "\n";
		
		
		
		////////////////////
		
		
		if (
			(isBlockHolder&&GEN_POLYS_WORLD) ||
			((!isBlockHolder)&&GEN_POLYS_HOLDER)	
		) {
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
		}
		
		
		
		
		
		listGenerated = true;
		
	}
void GamePageHolder::generateList ()
                            { //int fboNum
		
		preGenList = false;
		
		int cellsPerHolderM1 = cellsPerHolder-1;
		float fres = cellsPerHolder;
		int i, j, k, m, q;
		
		int curInd;
		
		int procCount = 0;
		
		bool edgeK;
		bool edgeJ;
		bool edgeI;
		
		
		
		
		
		float cellPitch;
		
		if (isBlockHolder) {
			cellPitch = singleton->cellsPerBlock;
		}
		else {
			cellPitch = 1.0f;
		}
		
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
		
		
		bool doProcAny;
		bool doProc[6];// = false;

		uint tempHF = E_CD_UNKNOWN;

		if (GEN_COLLISION) {
			collideIndices.clear();
		}
		
		vertexVec.clear();
		indexVec.clear();
		
		//bindHolderDL();
		
		
		//glBegin(GL_QUADS);
		
		
		
		int empCount = 0;
		int watCount = 0;
		int solCount = 0;
		int errCount = 0;
		
		int minRad = -1;
		int maxRad = cellsPerHolder;
		
		if (isBlockHolder) {
			minRad = 0;
			maxRad = cellsPerHolder-1;
		}
		
		
		for (k = minRad; k <= maxRad; k++) {
			for (j = minRad; j <= maxRad; j++) {
				for (i = minRad; i <= maxRad; i++) {
					
					iX = gphMinInPixels.getIX() + i;
					iY = gphMinInPixels.getIY() + j;
					iZ = gphMinInPixels.getIZ() + k;
					
					if (isBlockHolder) {
						cellVal = getCellAtCoordsLocal(iX,iY,iZ);
						
						switch (cellVal) {
							case E_CD_EMPTY:
								empCount++;
							break;
							case E_CD_WATER:
								watCount++;
							break;
							case E_CD_SOLID:
								solCount++;
							break;
							default:
								errCount++;
							break;
						}
						
					}
					else {
						cellVal = singleton->gw->getCellAtCoords(iX,iY,iZ);
					}
					
					
					tempHF = tempHF|cellVal;
				}
			}	
		}
		
		
		
		if (isBlockHolder) {
			
			cout << "\n\n\n";
			cout << "cellsPerHolder" << cellsPerHolder << "\n";
			cout << "empCount " << empCount << "\n";
			cout << "watCount " << watCount << "\n";
			cout << "solCount " << solCount << "\n";
			cout << "errCount " << errCount << "\n";
			cout << "\n\n\n";
		}
		
		bool fillPolys = 
			(isBlockHolder&&GEN_POLYS_WORLD) ||
			((!isBlockHolder)&&GEN_POLYS_HOLDER);
		
		
		
		bool rleOn = false;
		bool isLast = false;
		int begInd;
		int endInd;
		
		
		if (GEN_COLLISION) {
			
			if (
				(tempHF == E_CD_SOLID) ||
				(tempHF == E_CD_EMPTY)	
			) {
				
			}
			else {
				for (i = 0; i < cellsPerHolder; i++) {
					for (j = 0; j < cellsPerHolder; j++) {
						for (k = 0; k < cellsPerHolder; k++) {
							
							isLast = (k == (cellsPerHolder-1));
							
							curInd = (i + j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder);
							
							cellVal = getCellAtInd(curInd*4);
							
							if (cellVal == E_CD_SOLID) {
								if (rleOn) {
									
								}
								else {
									rleOn = true;
									begInd = curInd;
									collideIndices.push_back(begInd);
								}
							}
							else {
								if (rleOn) {
									rleOn = false;
									endInd = curInd-(cellsPerHolder*cellsPerHolder);
									
									collideIndices.push_back(endInd);
								}
							}
							
							if (rleOn&&isLast) {
								rleOn = false;
								endInd = curInd;
								collideIndices.push_back(endInd);
							}
						}
					}
				}
			}
				
				
		}
		
		
		// if (GEN_COLLISION) {
		// 	if (doProcAny) {
		// 		collideIndices.push_back(i + j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder);
		// 	}
		// }
		
		
		if (fillPolys) {
			
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
							bpX = iX*cellPitch;
							bpY = iY*cellPitch;
							bpZ = iZ*cellPitch;
							
							if (isBlockHolder) {
								cellVal = getCellAtCoordsLocal(iX,iY,iZ);
							}
							else {
								cellVal = singleton->gw->getCellAtCoords(iX,iY,iZ);
							}
							
							
							if ( cellVal == E_CD_SOLID ) {
								
								doProcAny = false;
								
								for (q = 0; q < NUM_ORIENTATIONS; q++) {
									
									if (isBlockHolder) {
										cellVal2 = getCellAtCoordsLocal(
											iX + DIR_VECS_I[q][0],
											iY + DIR_VECS_I[q][1],
											iZ + DIR_VECS_I[q][2]	
										);
									}
									else {
										cellVal2 = singleton->gw->getCellAtCoords(
											iX + DIR_VECS_I[q][0],
											iY + DIR_VECS_I[q][1],
											iZ + DIR_VECS_I[q][2]
										);
									}
									
									
									doProc[q] = cellVal2 != E_CD_SOLID;
									
									doProcAny = doProcAny | doProc[q];
									
								}
								
								// if (GEN_COLLISION) {
								// 	if (doProcAny) {
								// 		collideIndices.push_back(i + j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder);
								// 	}
								// }
								
								
								if (fillPolys) {
									
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
			}
			
			
		}
		
		

		
		listEmpty = (vertexVec.size() == 0);
		holderFlags = tempHF;
		
		preGenList = true;
		
	}
#undef LZZ_INLINE
 
