
class GameLogic {
public:
	Singleton* singleton;	

	std::vector<PathResult> pathSearchStack;
	std::vector<PathResult> pathFinalStack;
	std::vector<PathResult> pathHolder;
	//std::vector<PathResult> readyHolderList;

	ThreadPoolWrapper* threadPoolPath;
	ThreadPoolWrapper* threadPoolList;

	FIVector4 minv;
	FIVector4 maxv;
	
	bool didFindPath;
	bool searchedForPath;
	
	int idCounter;
	
	// 
	GamePageHolder* globEndHolder;
	int globEndGroupId;
	bool globFoundTarg;

	GameLogic() {
		
	}

	void init(Singleton* _singleton) {
		singleton = _singleton;
		
		searchedForPath = false;
		didFindPath = false;
		idCounter = 0;
		
		threadPoolPath = new ThreadPoolWrapper();
		threadPoolPath->init(singleton, 8, true||SINGLE_THREADED);
		
		threadPoolList = new ThreadPoolWrapper();
		threadPoolList->init(singleton, 8, false||SINGLE_THREADED);
		
	}
	
	GamePageHolder* getHolderById(int blockId, int holderId) {
		GameBlock* curBlock = singleton->gw->blockData[blockId];
		GamePageHolder* curHolder;
		if (curBlock != NULL) {
			curHolder = curBlock->holderData[holderId];
			return curHolder;
		}
		else {
			return NULL;
		}
	}
	
	GamePageHolder* getHolderByPR(PathResult* pr) {
		int blockId = pr->blockId;
		int holderId = pr->holderId;
		
		GamePageHolder* curHolder;
		GameBlock* curBlock = singleton->gw->blockData[blockId];
		if (curBlock != NULL) {
			curHolder = curBlock->holderData[holderId];
			return curHolder;
		}
		else {
			return NULL;
		}
	}
	
	bool holdersEqual(GamePageHolder* h0, GamePageHolder* h1) {
		return (
			(h0->blockId == h1->blockId) &&
			(h0->holderId == h1->holderId)
		);
	}
	
	void addHolderToStack(GamePageHolder* curHolder, int targId) {
		pathSearchStack.push_back(PathResult());
		pathSearchStack.back().blockId = curHolder->blockId;
		pathSearchStack.back().holderId = curHolder->holderId;
		curHolder->visitId = targId;
	}
	
	void remHolderFromStack(int opCode, int targId) {
		
		GamePageHolder* curHolder = getHolderByPR(&(pathSearchStack.back()));
		
		switch(opCode) {
			case E_PFO_CLEAR_GROUPS:
				curHolder->clearGroupFlags(targId);
				if (holdersEqual(globEndHolder,curHolder)) {
					globFoundTarg = true;
				}
			break;
		}
		
		pathSearchStack.pop_back();
	}
	
	void fillAllPaths(
		GamePageHolder* begHolder,
		GamePageHolder* endHolder,
		int begInd,
		int endInd,
		int targId,
		int opCode
	) {
		int i;
		
		pathSearchStack.clear();
		
		GamePageHolder* curHolder;
		GamePageHolder* testHolder;
		
		addHolderToStack(begHolder,targId); //true
		
		bool notFound;
		
		while (pathSearchStack.size() > 0) {
			
			curHolder = getHolderByPR(&(pathSearchStack.back()));
			
			notFound = true;
			
			for (i = 0; i < NUM_ORIENTATIONS; i++) {
				testHolder = singleton->gw->getHolderAtCoords(
					curHolder->offsetInHolders.getIX()+DIR_VECS_I[i][0],
					curHolder->offsetInHolders.getIY()+DIR_VECS_I[i][1],
					curHolder->offsetInHolders.getIZ()+DIR_VECS_I[i][2],
					false
				);
				
				if (testHolder != NULL) {
					if (testHolder->pathsReady) {
						if (testHolder->visitId == targId) {
							
						}
						else {
							notFound = false;
							addHolderToStack(testHolder,targId);
						}
					}
				}
			}
			
			if (notFound) {
				remHolderFromStack(opCode, targId);
			}
			
		}
		
	}
	
	
	
	
	
	
	
	
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
	
	void addGroupToStack(
		GamePageHolder* curHolder,
		int groupId,
		GamePageHolder* lastHolder,
		int lastGroupId,
		int targId
	) {
		pathSearchStack.push_back(PathResult());
		PathResult* lastRes = &(pathSearchStack.back());
		
		lastRes->blockId = curHolder->blockId;
		lastRes->holderId = curHolder->holderId;
		lastRes->groupId = groupId;
		
		if (lastHolder == NULL) {
			lastRes->lastBlockId = -1;
			lastRes->lastHolderId = -1;
			lastRes->lastGroupId = -1;
		}
		else {
			lastRes->lastBlockId = lastHolder->blockId;
			lastRes->lastHolderId = lastHolder->holderId;
			lastRes->lastGroupId = groupId;
		}
		
		
		
		curHolder->groupInfoStack[groupId].visitId = targId;
	}
	
	void remGroupFromStack(int opCode, int targId) {
		
		GamePageHolder* curHolder = getHolderByPR(&(pathSearchStack.back()));
		int curGroupId = pathSearchStack.back().groupId;
		
		switch(opCode) {
			case E_PFO_CLEAR_GROUPS:
				// curHolder->clearGroupFlags(targId);
				// if (groupsEqual(globEndHolder,curHolder)) {
				// 	globFoundTarg = true;
				// }
			break;
			case E_PFO_SEARCH_GROUPS:
				if (
					holdersEqual(globEndHolder,curHolder) &&
					(curGroupId == globEndGroupId)	
				) {
					globFoundTarg = true;
				}
			break;
		}
		
		pathFinalStack.push_back(pathSearchStack.back());
		
		pathSearchStack.pop_back();
	}
	
	void fillAllGroups(
		GamePageHolder* begHolder,
		GamePageHolder* endHolder,
		int begInd,
		int endInd,
		int targId,
		int opCode
	) {
		int i;
		
		pathSearchStack.clear();
		pathFinalStack.clear();
		
		GamePageHolder* curHolder;
		GamePageHolder* testHolder;
		
		int begGroupId = begHolder->getGroupId(begInd);
		int endGroupId = endHolder->getGroupId(endInd);
		globEndGroupId = endGroupId;
		int curGroupId = 0;
		int testGroupId;
		
		ConnectingNodeStruct* testConNode;
		
		addGroupToStack(begHolder,begGroupId,NULL,-1,targId); //true
		
		bool notFound;
		
		while (pathSearchStack.size() > 0) {
			
			curHolder = getHolderByPR(&(pathSearchStack.back()));
			curGroupId = pathSearchStack.back().groupId;
			
			notFound = true;
			
			for (i = 0; i < curHolder->bestConnectingNodes.size();i++) {
				testConNode = &(curHolder->bestConnectingNodes[i]);
				
				if (testConNode->groupIdFrom == curGroupId) {
					
					testHolder = getHolderById(testConNode->blockIdTo,testConNode->holderIdTo);
					testGroupId = testConNode->groupIdTo;
					
					if (testHolder != NULL) {
						if (testHolder->pathsReady) {
							if (testHolder->groupInfoStack[testGroupId].visitId == targId) {
								// already visited current group
							}
							else {
								notFound = false;
								addGroupToStack(testHolder,testGroupId,curHolder,curGroupId,targId);
							}
						}
					}
					
				}
				
				
				
			}
			
			/*
			for (i = 0; i < NUM_ORIENTATIONS; i++) {
				testHolder = singleton->gw->getHolderAtCoords(
					curHolder->offsetInHolders.getIX()+DIR_VECS_I[i][0],
					curHolder->offsetInHolders.getIY()+DIR_VECS_I[i][1],
					curHolder->offsetInHolders.getIZ()+DIR_VECS_I[i][2],
					false
				);
				
				if (testHolder != NULL) {
					if (testHolder->pathsReady) {
						if (testHolder->visitId == targId) {
							
						}
						else {
							notFound = false;
							addGroupToStack(testHolder,targId);
						}
					}
				}
			}
			*/
			
			if (notFound) {
				remGroupFromStack(opCode, targId);
			}
			
		}
		
	}
	
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
	
	
	
	
	
	
	
	// bool insertNode(int blockId, int holderId) {
	// 	int i;
		
	// 	bool notPresent = true;
		
	// 	GamePageHolder* curHolder;
	// 	GamePageHolder* holderToInsert = getHolderById(blockId,holderId);
		
	// 	if (holderToInsert == NULL) {
	// 		return false;
	// 	}
		
	// 	for (i = 0; i < readyHolderList.size(); i++) {
			
	// 		curHolder = getHolderById(readyHolderList[i].blockId,readyHolderList[i].holderId);
			
	// 		if (curHolder != NULL) {
	// 			if (
	// 				(curHolder->blockId == holderToInsert->blockId) &&
	// 				(curHolder->holderId == holderToInsert->holderId)
	// 			) {
	// 				notPresent = false;
	// 				break;
	// 			}
	// 		}
			
			
			
	// 	}
	
	// 	if (notPresent) {
	// 		readyHolderList.push_back(HolderInfo());
	// 		readyHolderList.back().blockId = blockId;
	// 		readyHolderList.back().holderId = holderId;
	// 	}
	
	// }
	
	
	bool findBestPath(
		GamePageHolder* closestHolder,
		GamePageHolder* closestHolder2,
		int bestInd,
		int bestInd2
	) {
		
		// clear
		globEndHolder = closestHolder2;
		globFoundTarg = false;
		fillAllPaths(closestHolder, closestHolder2, bestInd, bestInd2, idCounter, E_PFO_CLEAR_GROUPS); idCounter++;
		if (globFoundTarg) {
			cout << "Found linking holders\n";
		}
		else {
			cout << "No available path, or path data not ready\n";
			return false;
		}
		
		globFoundTarg = false;
		fillAllGroups(closestHolder, closestHolder2, bestInd, bestInd2, idCounter, E_PFO_SEARCH_GROUPS); idCounter++;
		
		if (globFoundTarg) {
			cout << "Found linking groups\n";
		}
		else {
			cout << "Could not find linking groups\n";
			return false;
		}
		
		return true;
		
	}
	
	
	void update() {
		
		int i;
		int j;
		int k;
		
		GamePageHolder* curHolder = singleton->gw->getHolderAtCoords(
			singleton->lastHolderPos.getIX(),
			singleton->lastHolderPos.getIY(),
			singleton->lastHolderPos.getIZ(),
			true
		);
		
		// if (singleton->refreshPaths) {
		// 	singleton->refreshPaths = false;
		// 	curHolder->refreshPaths();
		// }
		
		
		////////////////////
		
		
		GamePageHolder* tempHolder;
		GamePageHolder* tempHolder2;
		
		int tempInd;
		int tempInd2;
		
		GamePageHolder* closestHolder;
		GamePageHolder* closestHolder2;
		int bestInd;
		int bestInd2;
		PathResult* curPR;
		
		if (singleton->pathfindingOn) {
			
			bestInd = getClosestPathInd(&(singleton->moveNodes[0]), closestHolder);
			bestInd2 = getClosestPathInd(&(singleton->moveNodes[1]), closestHolder2);
			
			
			drawPointAtIndex(closestHolder, bestInd, 0,255,0, 0.5f);
			drawPointAtIndex(closestHolder2, bestInd2, 255,0,0, 0.5f);
			
			if ((bestInd > -1)  && (bestInd2 >-1) && (!searchedForPath)) {
				// draw path between index
				
				didFindPath = findBestPath(closestHolder, closestHolder2, bestInd, bestInd2);
				searchedForPath = true;
			}
			
			if (didFindPath) {
				for (i = 0; i < pathFinalStack.size(); i++) {
					curPR = &(pathFinalStack[i]);
					
					tempHolder = getHolderById(curPR->blockId,curPR->holderId);
					
					
					if ((tempHolder != NULL)) {
						tempInd = tempHolder->groupInfoStack[curPR->groupId].centerInd;
						if (tempInd > -1) {
							drawPointAtIndex(tempHolder, tempInd, 64, 64, 255, 0.25f);
							
							if (curPR->lastGroupId == -1) {
								
							}
							else {
								tempHolder2 = getHolderById(curPR->lastBlockId,curPR->lastHolderId);
								
								if (tempHolder2 != NULL) {
									tempInd2 = tempHolder2->groupInfoStack[curPR->lastGroupId].centerInd;
									
									if (tempInd2 > -1) {
										drawLineAtIndices(
											tempHolder, tempInd,
											tempHolder2, tempInd2,
											64, 64, 255
										);
									}
									
								}
								
							}
							
						}
						
						
					}
					
					
				}
			}
			
			
			
			//bestInd = getClosestPathInd(&(singleton->mouseMovePD), closestHolder);
			//drawPaths(0, 0, 0, closestHolder, bestInd);
			
			// for (k = -1; k <= 1; k++) {
			// 	for (j = -1; j <= 1; j++) {
			// 		for (i = -1; i <= 1; i++) {
			// 			drawPaths(
			// 				i,
			// 				j,
			// 				k,
			// 				closestHolder,
			// 				bestInd
			// 			);
			// 		}
			// 	}
			// }
			
			
			
			
		}
		
		
		
		
		// for (i = 0; i < NUM_ORIENTATIONS; i++) {			
		// 	drawPaths(
		// 		DIR_VECS_I[i][0],
		// 		DIR_VECS_I[i][1],
		// 		DIR_VECS_I[i][2],
		// 		closestHolder,
		// 		bestInd
		// 	);
		// }
		
		
		////////////////////
	}
	
	
	void drawLineAtIndices(
		GamePageHolder* curPointHolder, int curPointIndex,
		GamePageHolder* curPointHolder2, int curPointIndex2,
		int r, int g, int b
	) {
		
		if (curPointIndex < 0) {
			return;
		}
		
		if (curPointHolder == NULL) {
			return;
		}
		
		int ii;
		int jj;
		int kk;
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		FIVector4 pVec1;
		FIVector4 pVec2;
		
		singleton->setShaderVec3("matVal", r, g, b);
		// draw highlighted cube here if necessary
		
		kk = curPointIndex/(cellsPerHolder*cellsPerHolder);
		jj = (curPointIndex-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		ii = curPointIndex-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
		
		pVec1.copyFrom(&(curPointHolder->gphMinInPixels));
		pVec1.addXYZ(ii,jj,kk);
		pVec1.addXYZ(0.5f);
		
		kk = curPointIndex2/(cellsPerHolder*cellsPerHolder);
		jj = (curPointIndex2-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		ii = curPointIndex2-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
		
		pVec2.copyFrom(&(curPointHolder2->gphMinInPixels));
		pVec2.addXYZ(ii,jj,kk);
		pVec2.addXYZ(0.5f);
		
		
		singleton->drawLine(&pVec1, &pVec2);
	}
	
	
	void drawPointAtIndex(GamePageHolder* curPointHolder, int curPointIndex, int r, int g, int b, float rad) {
		
		if (curPointIndex < 0) {
			return;
		}
		
		if (curPointHolder == NULL) {
			return;
		}
		
		int ii;
		int jj;
		int kk;
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		FIVector4 pVec1;
		
		singleton->setShaderVec3("matVal", r, g, b);
		// draw highlighted cube here if necessary
		
		kk = curPointIndex/(cellsPerHolder*cellsPerHolder);
		jj = (curPointIndex-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		ii = curPointIndex-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
		
		pVec1.copyFrom(&(curPointHolder->gphMinInPixels));
		pVec1.addXYZ(ii,jj,kk);
		pVec1.addXYZ(0.5f);
		singleton->drawCubeCentered(&pVec1, rad);
	}
	
	
	void drawPaths(
		int offX,
		int offY,
		int offZ,
		GamePageHolder* curPointHolder,
		int curPointIndex
	) {
		
		if (curPointHolder == NULL) {
			return;
		}
		
		
		int i;
		int j;
		int k;
		int n;
		int q;
		
		int ii;
		int jj;
		int kk;
		int ii2;
		int jj2;
		int kk2;
		
		int ind;
		int lastId;
		int curId;
		int curInd;
		int cameFromInd;
		int cellVal;
		
		int targetGroup;
		int targetBlockId;
		int targetHolderId;
		
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		bool doProc = false;
		
		FIVector4 lhPos;
		lhPos.copyFrom(&(singleton->lastHolderPos));
		lhPos.addXYZ(offX,offY,offZ);
		
		GamePageHolder* curHolder = singleton->gw->getHolderAtCoords(
			lhPos.getIX(),
			lhPos.getIY(),
			lhPos.getIZ(),
			true
		);
		
		bool sameHolder = (
			(curPointHolder->blockId = curHolder->blockId) &&
			(curPointHolder->holderId = curHolder->holderId)
		);
		
		float curPathCost;
		
		FIVector4 pVec1;
		FIVector4 pVec2;
		FIVector4 pVec3;
		
		
		if (curHolder->idealPathsReady) {
			
			singleton->setShaderFloat("isWire", 1.0);
			singleton->setShaderVec3("matVal", 255, 0, 0);
			minv.copyFrom(&lhPos);
			minv.multXYZ(cellsPerHolder);
			maxv.copyFrom(&minv);
			maxv.addXYZ(cellsPerHolder);
			singleton->drawBox(&minv, &maxv);
			singleton->setShaderFloat("isWire", 0.0);
			
			
			// draw all path points
			lastId = -1;
			for (q = 0; q < curHolder->groupIdStack.size(); q++) {
				
				curId = curHolder->groupIdStack[q].groupId;
				curInd = curHolder->groupIdStack[q].ind;
				
				if (lastId != curId) {
					singleton->setShaderfVec3("matVal", &(singleton->colVecs[curId%16]));
				}
				
				if (curInd == curHolder->groupInfoStack[curId].centerInd) {
					singleton->setShaderVec3("matVal", 254, 254, 254);
				}
				
				kk = curInd/(cellsPerHolder*cellsPerHolder);
				jj = (curInd-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
				ii = curInd-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
				
				pVec1.copyFrom(&minv);
				pVec1.addXYZ(ii,jj,kk);
				pVec1.addXYZ(0.5f);
				
				curPathCost = curHolder->getInfo(curInd)->pathCost;
				
				
				singleton->drawCubeCentered(
					&pVec1, 
					mixf(0.05f,0.45f,clampfZO(curPathCost/64.0f))
				);
				
				
				if (curInd == curHolder->groupInfoStack[curId].centerInd) {
					singleton->setShaderfVec3("matVal", &(singleton->colVecs[curId%16]));
				}
				
				lastId = curId;
			}
			
			doProc = false;
			
			
			
			
			// draw paths
			
			if (curPointIndex > -1) {
				
				
				
				singleton->setShaderVec3("matVal", 254, 254, 254);
				// draw highlighted cube here if necessary
				// ??
				
				
				targetGroup = curPointHolder->getGroupId(curPointIndex);
				targetBlockId = curPointHolder->blockId;
				targetHolderId = curPointHolder->holderId;
				
				
				
				
				if (curHolder->pathsReady && (targetGroup > -1)) {
					
					for (q = 0; q < curHolder->bestConnectingNodes.size(); q++) {
						//doProc = curHolder->pathsReady && (targetGroup > -1);
						
						doProc = false;
						if (sameHolder) {
							
							if (curHolder->bestConnectingNodes[q].groupIdFrom == targetGroup) {
								doProc = true;
								curInd = curHolder->bestConnectingNodes[q].cellIndFrom;
							}
						}
						else {
							if (
								
								(curHolder->bestConnectingNodes[q].groupIdTo == targetGroup) &&
								(curHolder->bestConnectingNodes[q].blockIdTo == targetBlockId) &&
								(curHolder->bestConnectingNodes[q].groupIdTo == targetHolderId)	
								
							) {
								doProc = true;
								curInd = curHolder->bestConnectingNodes[q].cellIndFrom;
							}
						}
						
						// must set curInd
						//curInd = curPointHolder->groupInfoStack[targetGroup].centerInd;
						
						
						if (doProc) {
							cameFromInd = curHolder->getInfo(curInd)->cameFromInd;
							while (
								(cameFromInd > -1) &&
								(curInd > -1)	
							) {
								
								cameFromInd = curHolder->getInfo(curInd)->cameFromInd;
								
								if (
									(curInd > -1) &&
									(cameFromInd > -1) &&
									(curInd < (cellsPerHolder*cellsPerHolder*cellsPerHolder)) &&
									(cameFromInd < (cellsPerHolder*cellsPerHolder*cellsPerHolder))
								) {
									kk = curInd/(cellsPerHolder*cellsPerHolder);
									jj = (curInd-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
									ii = curInd-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
									
									kk2 = cameFromInd/(cellsPerHolder*cellsPerHolder);
									jj2 = (cameFromInd-kk2*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
									ii2 = cameFromInd-(kk2*cellsPerHolder*cellsPerHolder + jj2*cellsPerHolder);
									
									
									pVec1.copyFrom(&minv);
									pVec1.addXYZ(ii,jj,kk);
									pVec1.addXYZ(0.5f);
									
									pVec2.copyFrom(&minv);
									pVec2.addXYZ(ii2,jj2,kk2);
									pVec2.addXYZ(0.5f);
									
									singleton->drawLine(&pVec1, &pVec2);
								}
								
								curInd = cameFromInd;
								
							}
						}
						
					}
				}
				
				
				
				
			}
			
			
			
			
			//singleton->setShaderVec3("matVal", 30, 30, 30);
			// for (q = 0; q < curHolder->groupIdStack.size(); q++) {
				
			// 	curId = curHolder->groupIdStack[q].groupId;
			// 	curInd = curHolder->groupIdStack[q].ind;
			// 	cameFromInd = curHolder->getInfo(curInd)->cameFromInd;
				
			// 	if (
			// 		(cameFromInd > -1) &&
			// 		(cameFromInd < (cellsPerHolder*cellsPerHolder*cellsPerHolder))
			// 	) {
			// 		kk = curInd/(cellsPerHolder*cellsPerHolder);
			// 		jj = (curInd-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			// 		ii = curInd-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
					
			// 		kk2 = cameFromInd/(cellsPerHolder*cellsPerHolder);
			// 		jj2 = (cameFromInd-kk2*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			// 		ii2 = cameFromInd-(kk2*cellsPerHolder*cellsPerHolder + jj2*cellsPerHolder);
					
					
			// 		pVec1.copyFrom(&minv);
			// 		pVec1.addXYZ(ii,jj,kk);
			// 		pVec1.addXYZ(0.5f);
					
			// 		pVec2.copyFrom(&minv);
			// 		pVec2.addXYZ(ii2,jj2,kk2);
			// 		pVec2.addXYZ(0.5f);
					
			// 		singleton->drawLine(&pVec1, &pVec2);
			// 	}
			// }
			
			
		}
	}
	
	int getClosestPathInd(FIVector4* closestPoint, GamePageHolder* &closestHolder) {
			
			
			
			int i;
			int j;
			int k;
			int n;
			int ind;
			
			int testInd;
			int testDis;
			int bestDis = 99999;
			int bestInd = -1;
			
			int cellsPerHolder = singleton->cellsPerHolder;
			int curInd = singleton->gw->getCellInd(closestPoint, closestHolder);
			
			if (closestHolder == NULL) {
				return -1;
			}
			
			// if (singleton->gameFluid[E_FID_BIG]->threadPoolPath.anyRunning()) {
			// 	return -1;
			// }
			
			if (closestHolder->idealPathsReady) {
				
			}
			else {
				return -1;
			}
			
			
			int kk = curInd/(cellsPerHolder*cellsPerHolder);
			int jj = (curInd-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			int ii = curInd-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
			
			
			for (n = 0; n < cellsPerHolder*cellsPerHolder*cellsPerHolder; n++) {
				ind = n;
				
				
				
				if (closestHolder->getGroupId(n) > -1) {
					k = ind/(cellsPerHolder*cellsPerHolder);
					j = (ind - k*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
					i = ind - ( j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder );
					
					testInd = ind;
					testDis = abs(i-ii) + abs(j-jj) + abs(k-kk);
					
					if (testDis < bestDis) {
						bestDis = testDis;
						bestInd = testInd;
					}
				}
				
			}
			
			return bestInd;
		}
	
	void loadNearestHolders() {
		
		FIVector4 tempVec;
		
		int i, j, k;
		int ii, jj, kk;
		int incVal;
		int maxLoadRad = 0;
		int genCount = 0;
		int mink;
		int maxk;
		int minj;
		int maxj;
		int mini;
		int maxi;
		int curLoadRadius;
		int radStep = 1;
		intPair curId;
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		switch(cellsPerHolder) {
			case 16:
				maxLoadRad = 4;
			break;
			
			case 32:
				maxLoadRad = 4;
			break;
			
			case 64:
				maxLoadRad = 2;
			break;
			
			case 128:
				maxLoadRad = 1;
			break;
		}
		
		bool doPaths;
		
		// check if any are done running and stop them if necessary
		threadPoolList->anyRunning();
		if (threadPoolPath->anyRunning()) {
			doPaths = false;
		}
		else {
			doPaths = true;
		}
		
		
		
		
		tempVec.copyFrom(singleton->cameraPos);
		tempVec.intDivXYZ(singleton->cellsPerHolder);

		GamePageHolder* curHolder;
		GameBlock *curBlock;

		singleton->gw->ensureBlocks();
		
		// if (singleton->pathfindingOn) {
			
		// }
		// else {
		// 	return;
		// }
		
		for (curLoadRadius = 0; curLoadRadius < maxLoadRad; curLoadRadius++) {
			
			mink = max(tempVec.getIZ() - curLoadRadius,0);
			maxk = min(tempVec.getIZ() + curLoadRadius,singleton->worldSizeInHolders.getIZ()-1);
			minj = tempVec.getIY() - curLoadRadius;
			maxj = tempVec.getIY() + curLoadRadius;
			mini = tempVec.getIX() - curLoadRadius;
			maxi = tempVec.getIX() + curLoadRadius;
			
			for (jj = minj; jj <= maxj; jj += radStep) {
				
				if (curLoadRadius <= 2) {
					incVal = 1;
				}
				else {
					if ( (jj == minj) || (jj == maxj) ) {
						incVal = radStep;
					}
					else {
						incVal = maxi - mini;
					}
				}
				
				for (ii = maxi; ii >= mini; ii -= incVal) {
					
					
					for (kk = mink; kk <= maxk; kk += radStep) {
						
						
						curHolder = singleton->gw->getHolderAtCoords(ii, jj, kk, true);
						curBlock = singleton->gw->getBlockAtId(curHolder->blockId);
						
						
						if (curBlock == NULL) {
							cout << "NULL BLOCK\n";
						}
						else {
							if (curHolder->wasGenerated) {
								
								
								if ((curLoadRadius < 2)&&(singleton->pathfindingOn)&&doPaths) {
									if (curHolder->pathsReady || curHolder->lockWrite) {
										
									}
									else {
										
										if(curHolder->prepPathRefresh(2)) {
											
											
											threadPoolPath->intData[0] = E_TT_GENPATHS;
											threadPoolPath->intData[1] = curHolder->blockId;
											threadPoolPath->intData[2] = curHolder->holderId;
											
											if (threadPoolPath->startThread()) {
												genCount++;
											}
											
											
										}
										
									}
								}
								
								if (curHolder->listGenerated || curHolder->lockWrite) {
									
								}
								else {
									//cout << "genList\n";
									
									if(curHolder->prepPathRefresh(1)) {
										
										
										
										threadPoolList->intData[0] = E_TT_GENLIST;
										threadPoolList->intData[1] = curHolder->blockId;
										threadPoolList->intData[2] = curHolder->holderId;
										
										if (threadPoolList->startThread()) {
											genCount++;
										}
										
										//curHolder->generateList();
									}
									
									
								}
								
								
							}
							else {
								// curHolder->genCellData();
								// genCount++;
							}
						}
						
						
						// if (genCount >= 2) {
						// 	return;
						// }
						
					}
					
				}
			}
			
		}
	}
	
	
};

