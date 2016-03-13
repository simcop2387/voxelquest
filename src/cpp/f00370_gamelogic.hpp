
class GameLogic {
public:
	Singleton* singleton;

	std::vector<PathResult> pathSearchStack;
	std::vector<PathResult> pathFinalStack;
	//std::vector<PathResult> readyHolderList;

	ThreadPoolWrapper* threadPoolPath;
	ThreadPoolWrapper* threadPoolList;

	FIVector4 minv;
	FIVector4 maxv;
	
	// bool didFindPath;
	// bool searchedForPath;
	
	PathInfo testPath;
	std::vector<btVector3> tempStack;
	
	
	int idCounter;
	//int pathCount;
	
	// 
	GamePageHolder* globEndHolder;
	int globEndGroupId;
	bool globFoundTarg;

	GameLogic() {
		
	}

	void setEntTargPath(int sourceUID, int destUID) {
		
		if (sourceUID < 0) {
			return;
		}
		if (destUID < 0) {
			return;
		}
		
		BaseObj* sEnt = &(singleton->gem->gameObjects[sourceUID]);
		BaseObj* dEnt = &(singleton->gem->gameObjects[destUID]);

		sEnt->targPath.points[0] = sEnt->tbPos;
		sEnt->targPath.points[1] = dEnt->tbPos;
		sEnt->targPath.searchedForPath = false;
		sEnt->targPath.didFindPath = false;
		sEnt->targPath.nextInd = -1;
		sEnt->targPath.finalPoints.clear();
		
		getPath(&(sEnt->targPath));
		
	}

	void init(Singleton* _singleton) {
		singleton = _singleton;
		
		testPath.searchedForPath = false;
		testPath.didFindPath = false;
		testPath.points[0] = btVector3(0.0f,0.0f,0.0f);
		testPath.points[1] = btVector3(0.0f,0.0f,0.0f);
		
		idCounter = 1;
		//pathCount = 0;
		
		threadPoolPath = new ThreadPoolWrapper();
		threadPoolPath->init(singleton, 8, true||SINGLE_THREADED);
		
		threadPoolList = new ThreadPoolWrapper();
		threadPoolList->init(singleton, 8, false||SINGLE_THREADED);
		
	}
	
	
	void applyTBBehavior() {
		BaseObj* ca = singleton->gem->getActiveActor();
		
		if (ca == NULL) {
			return;
		}
		
		if (ca->isDead()) {
			return;
		}
		
		if (ca->isHidden) {
			return;
		}
		
		BaseObj* nearestEnemy = NULL;
		BaseObj* nearestWeapon = NULL;
		
		btVector3 xyzDisEnemy;
		btVector3 xyzDisWeapon;
		
		int xyDisEnemy = 9999;
		int xyDisWeapon = 9999;
		
		int nearestEnemyInd;
		int nearestWeaponInd;
		
		int i;
		
		bool findWeapon = false;
		bool doMove = false;
		
		btVector3 actorFinalVec;
		
		nearestEnemyInd = singleton->gem->getClosestActor(
				ca->uid,
				E_ENTTYPE_NPC,
				200.0f,
				E_CF_AREENEMIES|E_CF_ISALIVE
		);
		if (nearestEnemyInd > 0) {
			// hostiles nearby
			nearestEnemy = &(singleton->gem->gameObjects[nearestEnemyInd]);
			xyzDisEnemy = singleton->gem->getUnitDistance(ca->uid, nearestEnemy->uid);
			xyDisEnemy = (xyzDisEnemy.getX() + xyzDisEnemy.getY());
			
			nearestWeaponInd = singleton->gem->getClosestActor(
					ca->uid,
					E_ENTTYPE_WEAPON,
					200.0f,
					E_CF_NOTGRABBED
			);
			if (nearestWeaponInd > 0) {
				nearestWeapon = &(singleton->gem->gameObjects[nearestWeaponInd]);
				xyzDisWeapon = singleton->gem->getUnitDistance(ca->uid, nearestWeapon->uid);
				xyDisWeapon = (xyzDisWeapon.getX() + xyzDisWeapon.getY());
				
				
				if (ca->holdingWeapon(-1)) {
					// already holding weapon
				}
				else {
					// find nearest weapon
					
					//if (xyDisWeapon <= xyDisEnemy) {
						findWeapon = true;
					//}
				}
			}
			
			if (findWeapon) {
				if (xyDisWeapon <= 1) {
					singleton->gem->makeTurnTowardsTB(ca->uid, nearestWeapon->tbPos);
					singleton->gem->makeGrab(ca->uid, -1);
				}
				else {
					setEntTargPath(ca->uid,nearestWeapon->uid);
					doMove = true;
				}
			}
			else {
				if (xyDisEnemy <= 1) {
					singleton->gem->makeTurnTowardsTB(ca->uid, nearestEnemy->tbPos);
					singleton->gem->makeSwing(ca->uid, iGenRand(0,1));
				}
				else {
					setEntTargPath(ca->uid,nearestEnemy->uid);
					doMove = true;
				}
			}
			
			
		}
		else {
			// no hostiles nearby
		}
		
		
		if (doMove) {
			if (ca->targPath.didFindPath) {
				
				
				for (i = 0; i < ca->targPath.finalPoints.size(); i++) {
					
					if (
						singleton->gem->getUnitDisXY(
							ca->targPath.finalPoints[i],
							ca->tbPos
						) == 0
					) {
						break;
					}
					
				}
				
				i++;
				
				if (i >= ca->targPath.finalPoints.size()) {
					
				}
				else {
					ca->targPath.nextInd = i;
					
					singleton->gem->makeTurnTowardsTB(ca->uid, ca->targPath.finalPoints[ca->targPath.nextInd]);
					singleton->gem->makeMoveTB(ca->uid, 1);
					
				}
				
				
				
			}
			else {
				cout << "did not find path\n";
			}
		}
		
		
		
		
		
	}
	
	void applyBehavior() {
		int i;
		int j;
		//float targCount = 0.0f;
		float bestWepDis;
		float bestNPCDis;
		int bestWepUID;
		int bestNPCUID;
		
		float testDis;
		float deltaAng;
		float curDis;
		
		int curActor = singleton->gem->getCurActorUID();
		
		btVector3 tVec;
		btVector3 offsetVec;
		btVector3 readCenter;
		btVector3 writeCenter;
		
		BaseObj* writeObj;
		BaseObj* readObj;
		
		for (i = 0; i < singleton->gem->visObjects.size(); i++) {
			writeObj = &(singleton->gem->gameObjects[
				singleton->gem->visObjects[i]	
			]);
			writeObj->npcRepel = btVector3(0.0f,0.0f,0.0f);
			writeObj->behaviorTarget = btVector3(0.0f,0.0f,0.0f);
			//targCount = 0.0f;
			
			if (
				(writeObj->isHidden) ||
				(writeObj->entType != E_ENTTYPE_NPC) ||
				(writeObj->uid == curActor) // dont apply behavior to active actor
			) {
				
			}
			else {
				
				writeCenter = writeObj->getCenterPoint(E_BDG_CENTER);
				
				
				bestWepDis = 99999.0f;
				bestNPCDis = 99999.0f;
				bestWepUID = -1;
				bestNPCUID = -1;
				
				for (j = 0; j < singleton->gem->visObjects.size(); j++) {
					readObj = &(singleton->gem->gameObjects[
						singleton->gem->visObjects[j]
					]);
					
					if (
						(readObj->isHidden) ||
						(readObj->uid == writeObj->uid)
					) {
						
					}
					else {
						
						// found a unique pair of NPCs
						// determine seek, avoid, neutral
						
						readCenter = readObj->getCenterPoint(E_BDG_CENTER);
						
						if (readObj->entType == E_ENTTYPE_NPC) {
							
							offsetVec = writeCenter-readCenter;
							if (offsetVec.fuzzyZero()) {
								
							}
							else {
								offsetVec.normalize();
								curDis = readCenter.distance(writeCenter);
								writeObj->npcRepel += offsetVec*clampf(3.0f-curDis,-0.05f, 1.0f);
							}
							
							
							if (writeObj->holdingWeapon(-1)) {
								
								if (
									(readObj->isAlive())
									// && (readObj->uid == curActor) 	
								) {
									
									// has weapon, seek out human opponent
									
									testDis = readObj->getCenterPoint(E_BDG_CENTER).distance(
										writeObj->getCenterPoint(E_BDG_CENTER)
									);
								
									// if (bestNPCUID == curActor) {
										
									// }
									// else {
										if ((testDis < bestNPCDis)) { //||(readObj->uid == curActor)
											bestNPCDis = testDis;
											bestNPCUID = readObj->uid;
											writeObj->behaviorTarget = readObj->getCenterPoint(E_BDG_CENTER);
										}
									//}
									
									
									
									//targCount += 1.0f;
								}
								
							}
							else {
								
							}
						}
						else {
							
							if (writeObj->holdingWeapon(-1)) {
								
							}
							else {
								// find a weapon
								
								if (
									(readObj->entType == E_ENTTYPE_WEAPON) &&
									(readObj->isGrabbedById < 0)
								) {
									
									testDis = readObj->getCenterPoint(E_BDG_CENTER).distance(
										writeObj->getCenterPoint(E_BDG_CENTER)
									);
									if (testDis < bestWepDis) {
										bestWepDis = testDis;
										bestWepUID = readObj->uid;
										writeObj->behaviorTarget = readObj->getCenterPoint(E_BDG_CENTER);
									}
									
									
									//targCount += 1.0f;
								}
							}
							
							
							
						}
						
						
					}
					
				}
				
				// if (targCount > 0.0f) {
				// 	writeObj->behaviorTarget /= targCount;
				// }
				
			}
		}
		
		
		for (i = 0; i < singleton->gem->visObjects.size(); i++) {
			writeObj = &(singleton->gem->gameObjects[
				singleton->gem->visObjects[i]	
			]);
			
			if (
				(writeObj->isHidden) ||
				(writeObj->entType != E_ENTTYPE_NPC)
			) {
				
			}
			else {
				
				writeObj->bindingPower += singleton->conVals[E_CONST_BINDING_POW_INC];
				
				if (writeObj->isDead()) {
					writeObj->bindingPower += (
						singleton->conVals[E_CONST_BINDING_POW_ON_DEATH] -
						writeObj->bindingPower						
					)/128.0f;
				}
				
				if (writeObj->bindingPower > 1.0f) {
					writeObj->bindingPower = 1.0f;
				}
				
				if (writeObj->baseContact()) {
					writeObj->airCount = 0.0f;
				}
				else {
					writeObj->airCount += 1.0f;
				}
				
				writeObj->jumpCooldown--;
				if (writeObj->jumpCooldown < 0) {
					writeObj->jumpCooldown = 0;
				}
				
				writeObj->hitCooldown--;
				if (writeObj->hitCooldown < 0) {
					writeObj->hitCooldown = 0;
				}
				
				writeObj->setActionState(E_ACT_ISHIT,RLBN_NEIT,(writeObj->hitCooldown > 0));
				
				writeObj->setActionState(E_ACT_ISWALKING,RLBN_NEIT,false);
				
				
				if (writeObj->isAlive()) {
					
					if (singleton->gem->turnBased) {
						
						deltaAng = writeObj->turnTowardsTargAng(TBDIR_ARR[writeObj->tbDir]);
						
						singleton->gem->makeTurn(writeObj->uid,deltaAng*4.0f);
						
						// tVec = (writeObj->tbPos+btVector3(0.5f,0.5f,0.5f))-writeObj->getCenterPoint(E_BDG_CENTER);
						// tVec *= btVector3(1.0f,1.0f,0.0f);
						
						// singleton->gem->makeMoveVec(
						// 	writeObj->uid,
						// 	tVec
						// );
					}
					else {
						if (writeObj->behaviorTarget.isZero()) {
							
						}
						else {
							deltaAng = writeObj->turnTowardsPointDelta(writeObj->behaviorTarget);
							
							singleton->gem->makeTurn(writeObj->uid,deltaAng*16.0f);
							
							curDis = writeObj->behaviorTarget.distance(writeObj->getCenterPoint(E_BDG_CENTER));
							
							if (curDis > singleton->conVals[E_CONST_AI_SEEK_THRESH]) {
								singleton->gem->makeMove(writeObj->uid, btVector3(0.0f,1.0f,0.0f), true, true);
							}
							if (curDis < singleton->conVals[E_CONST_AI_REPEL_THRESH]) {
								singleton->gem->makeMove(writeObj->uid, btVector3(0.0f,-1.0f,0.0f), true, true);
							}
							
							singleton->gem->makeMoveVec(writeObj->uid,writeObj->npcRepel);
							
							if (curDis > 6.0f) {
								writeObj->blockCount += clampfZO(1.0 - abs(curDis - writeObj->lastBlockDis)*100.0f);
								
							}
							
							if (writeObj->holdingWeapon(-1)) {
								if (
									(curDis > 1.0f) &&
									(curDis < 5.0f)	
								) {
									writeObj->swingCount += 1.0f;
								
									if (writeObj->swingCount > singleton->conVals[E_CONST_SWING_DELAY]) {
										writeObj->swingCount = 0.0f;
										singleton->gem->makeSwing(writeObj->uid,iGenRand(0,1));
									}
									
								}
							}
							else {
								if (
									(curDis > 1.0f) &&
									(curDis < 4.0f)	
								) {
									singleton->gem->makeGrab(writeObj->uid, -1);
								}
							}
							
							
							
							if (writeObj->blockCount > 100.0f) {
								writeObj->blockCount = 0.0f;
								singleton->gem->makeJump(writeObj->uid, true,
									clampfZO(curDis-6.0f)*0.75f + 0.25f	
								);
							}
							writeObj->blockCount *= 0.999f;
							writeObj->lastBlockDis = curDis;
							
							
						}
					}
					
				}
				else { // is dead
					if (writeObj->holdingWeapon(RLBN_LEFT)) {
						singleton->gem->makeGrab(writeObj->uid, RLBN_LEFT);
					}
					
					if (writeObj->holdingWeapon(RLBN_RIGT)) {
						singleton->gem->makeGrab(writeObj->uid, RLBN_RIGT);
					}
				}
				
				
				
			}
			
			
			
		}
		
	}
	
	GamePageHolder* getHolderById(int blockId, int holderId) {
		
		if ((blockId < 0) || (holderId < 0)) {
			return NULL;
		}
		
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
	
	void addHolderToStack(GamePageHolder* curHolder) {
		pathSearchStack.push_back(PathResult());
		pathSearchStack.back().blockId = curHolder->blockId;
		pathSearchStack.back().holderId = curHolder->holderId;
		curHolder->visitId = idCounter;
	}
	
	void remHolderFromStack(int opCode) {
		
		GamePageHolder* curHolder = getHolderByPR(&(pathSearchStack.back()));
		
		switch(opCode) {
			case E_PFO_CLEAR_GROUPS:
				curHolder->clearGroupFlags(idCounter);
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
		int opCode
	) {
		int i;
		
		pathSearchStack.clear();
		
		GamePageHolder* curHolder;
		GamePageHolder* testHolder;
		
		addHolderToStack(begHolder); //true
		
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
						if (testHolder->visitId == idCounter) {
							
						}
						else {
							notFound = false;
							addHolderToStack(testHolder);
						}
					}
				}
			}
			
			if (notFound) {
				remHolderFromStack(opCode);
			}
			
		}
		
		
		idCounter++;
	}
	
	
	
	
	
	
	
	
	bool addGroupToStack(
		ConnectingNodeStruct* testConNode,
		GamePageHolder* curHolder,
		int groupId,
		GamePageHolder* lastHolder,
		int lastGroupId,
		int lastIndex
	) {
		
		
		
		bool foundIt = false;
		
		pathSearchStack.push_back(PathResult());
		PathResult* lastRes = &(pathSearchStack.back());
		
		lastRes->blockId = curHolder->blockId;
		lastRes->holderId = curHolder->holderId;
		lastRes->groupId = groupId;
		
		
		//curHolderFrom->getInfo(curInd)->cameFromInd
		
		if (testConNode == NULL) {
			
			lastRes->conNode.blockIdFrom = -1;
			lastRes->conNode.holderIdFrom = -1;
			lastRes->conNode.blockIdTo = -1;
			lastRes->conNode.holderIdTo = -1;
			lastRes->conNode.groupIdFrom = -1;
			lastRes->conNode.groupIdTo = -1;
			lastRes->conNode.cellIndFrom = -1;
			lastRes->conNode.cellIndTo = -1;
			lastRes->conNode.totCost = -1;
		}
		else {
			lastRes->conNode = *testConNode;
		}
		
		if (lastHolder == NULL) {
			lastRes->lastBlockId = -1;
			lastRes->lastHolderId = -1;
			lastRes->lastGroupId = -1;
			lastRes->lastIndex = -1;
		}
		else {
			lastRes->lastBlockId = lastHolder->blockId;
			lastRes->lastHolderId = lastHolder->holderId;
			lastRes->lastGroupId = groupId;
			lastRes->lastIndex = lastIndex;
		}
		
		
		curHolder->groupInfoStack[groupId].visitId = idCounter;
		
		
		//////////
		
		
		if (
			holdersEqual(globEndHolder,curHolder) &&
			(groupId == globEndGroupId)	
		) {
			globFoundTarg = true;
			foundIt = true;
		}
		
		return foundIt;
		
		
		/*
		GamePageHolder* curHolder = getHolderByPR(&(pathSearchStack.back()));
		int curGroupId = pathSearchStack.back().groupId;
		
		switch(opCode) {
			case E_PFO_CLEAR_GROUPS:
				// curHolder->clearGroupFlags(idCounter);
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
		*/
		
		
		
		
	}
	
	void remGroupFromStack(int opCode) {
		
		
		
		pathSearchStack.pop_back();
	}
	
	void fillAllGroups(
		GamePageHolder* begHolder,
		GamePageHolder* endHolder,
		int begInd,
		int endInd,
		int opCode
	) {
		int i;
		
		pathSearchStack.clear();
		pathFinalStack.clear();
		
		GamePageHolder* curHolder;
		GamePageHolder* testHolder;
		
		int begGroupId = begHolder->getGroupId(begInd);
		int endGroupId = endHolder->getGroupId(endInd);
		
		if ((begGroupId > -1) && (endGroupId > -1)) {
			
		}
		else {
			goto FILL_GROUPS_RETURN;
		}
		
		globEndGroupId = endGroupId;
		int curGroupId = 0;
		int groupIdTo = 0;
		
		ConnectingNodeStruct* testConNode;
		
		bool foundIt = false;
		
		int lastIndex = -1;
		
		foundIt = addGroupToStack(
			NULL,
			begHolder,
			begGroupId,
			NULL,
			-1,
			lastIndex
		);
		if (foundIt) {
			goto FOUND_TARG_GROUP;
		}
		
		bool notFound;
		
		int frontIndex = 0;
		
		while (
			(frontIndex < pathSearchStack.size()) && //(pathSearchStack.size() > 0) &&
			(globFoundTarg == false)	
		) {
			
			
			
			curHolder = getHolderByPR(&(pathSearchStack[frontIndex]));
			curGroupId = pathSearchStack[frontIndex].groupId;
			lastIndex = frontIndex;//pathSearchStack.size()-1;
			
			//frontIndex++;
			
			notFound = true;
			
			curHolder->sortConNodes(endHolder,endInd);
			
			for (i = 0; i < curHolder->bestConnectingNodes.size(); i++) {
				testConNode = &(curHolder->bestConnectingNodes[i]);
				
				if (testConNode->groupIdFrom == curGroupId) {
					
					testHolder = getHolderById(testConNode->blockIdTo,testConNode->holderIdTo);
					groupIdTo = testConNode->groupIdTo;
					
					if (testHolder != NULL) {
						if (testHolder->pathsReady) {
							if (testHolder->groupInfoStack[groupIdTo].visitId == idCounter) {
								// already visited current group
								
							}
							else {
								notFound = false;
								foundIt = addGroupToStack(
									testConNode,
									testHolder,
									groupIdTo,
									curHolder,
									curGroupId,
									lastIndex
								);
								if (foundIt) {
									goto FOUND_TARG_GROUP;
								}
							}
						}
					}
				}
			}
			
			if (notFound) {
				frontIndex++;
			//	remGroupFromStack(opCode);
			}
			
		}
		
		FOUND_TARG_GROUP:
		
		if (pathSearchStack.size() < 1) {
			goto FILL_GROUPS_RETURN;
		}
		
		
		lastIndex = pathSearchStack.back().lastIndex;
		pathFinalStack.push_back(pathSearchStack.back());
		
		while (lastIndex > -1) {
			pathFinalStack.push_back(pathSearchStack[lastIndex]);
			lastIndex = pathSearchStack[lastIndex].lastIndex;
		}
		
FILL_GROUPS_RETURN:
	idCounter++;
		
		
	}
	
	
	

	
	
	
	
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
	
	
	bool findNaivePath(PathInfo* pathInfo) {
		
		// attempt to find a naive greedy path
		
		int testInd;
		int deltaXA;
		int deltaYA;
		int deltaX;
		int deltaY;
		
		int toggleMod = 0;
		
		bool keepTesting = true;
		GamePageHolder* testHolder;
		
		btVector3 testPos = pathInfo->points[0];
		btVector3 destPos = pathInfo->points[1];
		
		btVector3 tempBTV;
		
		do {
			testInd = getClosestPathRad(testPos, testHolder);
			
			if (testInd > -1) {
				tempBTV = holderIndToBTV(testHolder, testInd, true);
				pathInfo->finalPoints.push_back(tempBTV);
				testPos.setZ(tempBTV.getZ());
			}
			
			deltaX = (destPos.getX() - testPos.getX());
			deltaY = (destPos.getY() - testPos.getY());
			deltaXA = abs(deltaX);
			deltaYA = abs(deltaY);
			
			keepTesting = (
				(testInd > -1) &&
				((deltaXA+deltaYA) > 0)
			);
			
			
			if ( (toggleMod%2) == 0 ) {
				if (deltaXA > 0) {
					testPos += btVector3(
						qSign(deltaX),
						0.0f,
						0.0f
					);
				}
				else if (deltaYA > 0) {
					testPos += btVector3(
						0.0f,
						qSign(deltaY),
						0.0f
					);
				}
			}
			else {
				if (deltaYA > 0) {
					testPos += btVector3(
						0.0f,
						qSign(deltaY),
						0.0f
					);
				}
				else if (deltaXA > 0) {
					testPos += btVector3(
						qSign(deltaX),
						0.0f,
						0.0f
					);
				}
			}
			
			toggleMod++;
			
		} while (keepTesting);
		
		return (
			(testInd > -1) &&
			((deltaXA+deltaYA) == 0)
		);
		
	}
	
	
	bool findBestPath(
		PathInfo* pathInfo,
		GamePageHolder* closestHolder,
		GamePageHolder* closestHolder2,
		int bestInd,
		int bestInd2
	) {
		
		
		
		// clear
		globEndHolder = closestHolder2;
		globFoundTarg = false;
		fillAllPaths(
			closestHolder,
			closestHolder2,
			bestInd,
			bestInd2,
			E_PFO_CLEAR_GROUPS
		);
		
		
		if (globFoundTarg) {
			//cout << "Found linking holders\n";
		}
		else {
			//cout << "No available path, or path data not ready\n";
			return false;
		}
		
		
		globFoundTarg = false;
		fillAllGroups(
			closestHolder,
			closestHolder2,
			bestInd,
			bestInd2,
			E_PFO_SEARCH_GROUPS
		);
		

		
		if (globFoundTarg) {
			//cout << "Found linking groups\n";
		}
		else {
			//cout << "Could not find linking groups\n";
			return false;
		}
		
		return true;
		
	}
	
	
	void drawFinalPath(PathInfo* pathInfo) {
		
		
		GamePageHolder* closestHolder;
		GamePageHolder* closestHolder2;
		
		int bestInd = getClosestPathRad(pathInfo->points[0], closestHolder);
		int bestInd2 = getClosestPathRad(pathInfo->points[1], closestHolder2);
		
		drawPointAtIndex(closestHolder, bestInd, 0,128+singleton->smoothTime*127.0f,0, singleton->smoothTime);
		drawPointAtIndex(closestHolder2, bestInd2, 128+singleton->smoothTime*127.0f,0,0, singleton->smoothTime);
		
		int i;
		
		singleton->setShaderVec3("matVal", 255, 0, 255);
		
		float curRad;
		double fi;
		
		for (i = 0; i < pathInfo->finalPoints.size(); i++) {
			fi = i;
			
			curRad = 0.25f + 0.15*sin( fi*0.5 + singleton->curTime/200.0 );
			
			if (i == pathInfo->nextInd) {
				singleton->setShaderVec3("matVal", 0, 255, 255);
			}
			
			singleton->drawBoxRad(pathInfo->finalPoints[i],btVector3(curRad,curRad,curRad));
			
			if (i == pathInfo->nextInd) {
				singleton->setShaderVec3("matVal", 255, 0, 255);
			}
			
		}
		
		
	}
	
	void getPath(PathInfo* pathInfo) {
		
		pathInfo->finalPoints.clear();
		
		int i;
		int j;
		int k;
		
		// GamePageHolder* curHolder = singleton->gw->getHolderAtCoords(
		// 	singleton->lastHolderPos.getIX(),
		// 	singleton->lastHolderPos.getIY(),
		// 	singleton->lastHolderPos.getIZ(),
		// 	true
		// );
		
		// if (singleton->refreshPaths) {
		// 	singleton->refreshPaths = false;
		// 	curHolder->refreshPaths();
		// }
		
		
		////////////////////
		
		
		GamePageHolder* conHolder1;
		GamePageHolder* conHolder2;
		
		GamePageHolder* tempHolder;
		GamePageHolder* tempHolder2;
		
		int tempInd;
		int tempInd2;
		
		GamePageHolder* closestHolder;
		GamePageHolder* closestHolder2;
		GamePageHolder* closestHolder3;
		int bestInd;
		int bestInd2;
		int bestInd3;
		PathResult* curPR;
		
		//pathCount = 0;
		
		
		
		bestInd = getClosestPathRad(pathInfo->points[0], closestHolder);
		bestInd2 = getClosestPathRad(pathInfo->points[1], closestHolder2);
		
		
		
		
		// current mouse position
		// bestInd3 = getClosestPathRad(&(singleton->mouseMovePD), closestHolder3);
		
		
		//drawPointAtIndex(closestHolder, bestInd, 0,128+singleton->smoothTime*127.0f,0, singleton->smoothTime);
		//drawPointAtIndex(closestHolder2, bestInd2, 128+singleton->smoothTime*127.0f,0,0, singleton->smoothTime);
		
		
		
		// if (
		// 	(bestInd > -1) &&
		// 	singleton->doPathReport
		// ) {
		// 	singleton->doPathReport = false;
		// 	;
			
		// 	drawPointAtIndex(closestHolder, bestInd, 0,255,0, 0.5f);
			
		// }
		
		// if (
		// 	(bestInd > -1)
		// ) {
		// 	if (closestHolder->getInfo(bestInd) != NULL) {
		// 		if (closestHolder->getInfo(bestInd)->cameFromInd > -1) {
		// 			drawPointAtIndex(closestHolder, closestHolder->getInfo(bestInd)->cameFromInd, 255,255,0, 0.5f);
		// 		}
		// 	}
		// }
		
		
		if ((bestInd > -1)  && (bestInd2 > -1)) {
			
			pathInfo->didFindPath = findNaivePath(pathInfo);
			
			if (pathInfo->didFindPath) {
				//cout << "found naive\n";
			}
			else {
				pathInfo->finalPoints.clear();
				
				pathInfo->didFindPath = findBestPath(
					pathInfo,
					closestHolder,
					closestHolder2,
					bestInd,
					bestInd2
				);
				
				if (pathInfo->didFindPath) {
					
					//cout << "found complex\n";
					
					getPointsForPath(closestHolder, bestInd, pathInfo, true);
					
					for (i = 0; i < pathFinalStack.size(); i++) {
						curPR = &(pathFinalStack[i]);
						
						tempHolder = getHolderById(curPR->blockId,curPR->holderId);
						if ((tempHolder != NULL)) {
							tempInd = tempHolder->groupInfoStack[curPR->groupId].centerInd;
							// if (tempInd > -1) {
							// 	drawPointAtIndex(tempHolder, tempInd, 255, 128, 0, singleton->smoothTime);	
							// }
						}
						
						conHolder1 = getHolderById(curPR->conNode.blockIdFrom, curPR->conNode.holderIdFrom);
						conHolder2 = getHolderById(curPR->conNode.blockIdTo, curPR->conNode.holderIdTo); 
						
						if (conHolder1 != NULL) {
							getPointsForPath(conHolder1, curPR->conNode.cellIndFrom, pathInfo, false);
						}
						if (conHolder2 != NULL) {
							getPointsForPath(conHolder2, curPR->conNode.cellIndTo, pathInfo, true);
						}
						
					}
					
					getPointsForPath(closestHolder2, bestInd2, pathInfo, false);
					
					
				}
			}
			
			
			
			pathInfo->searchedForPath = true;
		}
		
		
		
		// if (bestInd3 > -1) {
		// 	drawPathToPoint(closestHolder3, bestInd3, 255, 128, 0);
			
		// 	if (singleton->doPathReport) {
		// 		if (closestHolder3->getInfo(bestInd3) == NULL) {
		// 			cout << "NULL!!!\n";
		// 		}
		// 		else {
		// 			cout << closestHolder3->holderId << " " <<
		// 				closestHolder3->getInfo(bestInd3)->groupId << " " <<
		// 				closestHolder3->getInfoPD(bestInd3) << " " <<
		// 				bestInd3 << "/" << singleton->cellsPerHolder*singleton->cellsPerHolder*singleton->cellsPerHolder << " " <<
		// 				closestHolder3->groupInfoStack.size() << " " << 
		// 				"\n";
					
		// 		}
		// 		singleton->doPathReport = false;
		// 	}
			
		// }
		
		
		
		
		// for (k = -1; k <= 1; k++) {
		// 	for (j = -1; j <= 1; j++) {
		// 		for (i = -1; i <= 1; i++) {
		// 			drawRegions(
		// 				i,
		// 				j,
		// 				k
		// 			);
		// 		}
		// 	}
		// }
		
		
		
		
		
		
		
		
		
		
		////////////////////
		
	}
	
	void update() {
			
			int i;
			BaseObj* ca;
			
			if (singleton->pathfindingOn) {
				
				if (singleton->pathfindingTestOn) {
					if (
						(!testPath.searchedForPath) && (singleton->pathFindingStep == 2)
					) {
						getPath(&testPath);
					}
					drawFinalPath(&testPath);
				}
				
				
				if (singleton->drawTargPaths) {
					for (i = 0; i < singleton->gem->turnList.size(); i++) {
						
						ca = &(singleton->gem->gameObjects[ singleton->gem->turnList[i] ]); 
						
						if (ca->isHidden) {
							
						}
						else {
							if (ca->targPath.didFindPath) {
								drawFinalPath(&(ca->targPath));
							}
						}
						
						
					}
				}
					
			}
			
			
			
			
			
			
	}
	
	
	void drawLineAtIndices(
		GamePageHolder* curPointHolder, int curPointIndex,
		GamePageHolder* curPointHolder2, int curPointIndex2
		//,int r, int g, int b
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
		
		//singleton->setShaderVec3("matVal", r, g, b);
		// draw highlighted cube here if necessary
		
		kk = curPointIndex/(cellsPerHolder*cellsPerHolder);
		jj = (curPointIndex-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		ii = curPointIndex-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
		
		pVec1.copyFrom(&(curPointHolder->gphMinInCells));
		pVec1.addXYZ(ii,jj,kk);
		pVec1.addXYZ(0.5f);
		
		kk = curPointIndex2/(cellsPerHolder*cellsPerHolder);
		jj = (curPointIndex2-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		ii = curPointIndex2-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
		
		pVec2.copyFrom(&(curPointHolder2->gphMinInCells));
		pVec2.addXYZ(ii,jj,kk);
		pVec2.addXYZ(0.5f);
		
		
		singleton->drawLine(&pVec1, &pVec2);
	}
	
	btVector3 holderIndToBTV(GamePageHolder* curPointHolder, int curPointIndex, bool addHalfOff) {
		int ii;
		int jj;
		int kk;
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		btVector3 pVec1;
		
		kk = curPointIndex/(cellsPerHolder*cellsPerHolder);
		jj = (curPointIndex-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		ii = curPointIndex-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
		
		pVec1 = curPointHolder->gphMinInCells.getBTV();
		pVec1 += btVector3(ii,jj,kk);
		
		if (addHalfOff) {
			pVec1 += btVector3(0.5f,0.5f,0.5f);
		}
		
		return pVec1;
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
		
		pVec1.copyFrom(&(curPointHolder->gphMinInCells));
		pVec1.addXYZ(ii,jj,kk);
		pVec1.addXYZ(0.5f);
		singleton->drawCubeCentered(&pVec1, rad);
	}
	
	
	void getPointsForPath(GamePageHolder* curHolderFrom, int _curInd, PathInfo* pathInfo, bool reverseOrder) {
		
		//pathCount++;
		
		int curInd = _curInd;
		int cameFromInd;
		
		int ii;
		int jj;
		int kk;
		int ii2;
		int jj2;
		int kk2;
		
		int i;
		
		//int totPath = 0;
		
		
		btVector3 pVec1;
		
		//FIVector4 pVec1;
		//FIVector4 pVec2;
		
		if (curHolderFrom == NULL) {
			return;
		}
		
		if (curHolderFrom->wasGenerated) {
			
		}
		else {
			return;
		}
		
		if (curHolderFrom->pathsReady) {
			
		}
		else {
			return;
		}
		
		if (curHolderFrom->hasData) {
			
		}
		else {
			//cout << "NO DATA\n";
			return;
		}
		
		if (reverseOrder) {
			tempStack.clear();
		}
		
		btVector3 newMin = curHolderFrom->gphMinInCells.getBTV();
		
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		//singleton->setShaderVec3("matVal", rr, gg, bb);
		
		
		if (curHolderFrom->getInfo(curInd) != NULL) {
			cameFromInd = curHolderFrom->getInfo(curInd)->cameFromInd;
		}
		else {
			cameFromInd = -1;
		}
		
		while (
			(cameFromInd > -1) &&
			(curInd > -1)	
		) {
			
			
			//totPath++;
			
			if (curHolderFrom->getInfo(curInd) != NULL) {
				cameFromInd = curHolderFrom->getInfo(curInd)->cameFromInd;
			}
			else {
				cameFromInd = -1;
			}
			
			if (cameFromInd == curInd) {
				cameFromInd = -1;
			}
			
			
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
				
				
				pVec1 = newMin + btVector3(ii+0.5f,jj+0.5f,kk+0.5f);
				
				if (reverseOrder) {
					tempStack.push_back(pVec1);
				}
				else {
					pathInfo->finalPoints.push_back(pVec1);
				}
				
				
				
				// pVec2.copyFrom(&minv);
				// pVec2.addXYZ(ii2,jj2,kk2);
				// pVec2.addXYZ(0.5f);
				
				// singleton->drawLine(&pVec1, &pVec2);
				
				
				
			}
			
			curInd = cameFromInd;
			
		}
		
		if (reverseOrder) {
			for (i = (tempStack.size()-1); i >= 0; i--) {
				pathInfo->finalPoints.push_back(tempStack[i]);
			}
		}
		
		
		
		
		// if (totPath == 0) {
			
		// 	drawPointAtIndex(curHolderFrom, curInd, 0, 255, 255, singleton->smoothTime);	
			
			
		// 	// cout << "0 path " << cameFromInd << " " << curInd << "\n";
			
		// 	// if (curHolderFrom->getInfo(curInd) == NULL) {
		// 	// 		cout << "NULL IND DATA\n";
		// 	// }
			
		// }
		
	}
	
	
	
	
	void drawRegions(
		int offX,
		int offY,
		int offZ
		
	) {
		
		
		
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
		
		GamePageHolder* curHolderFrom = singleton->gw->getHolderAtCoords(
			lhPos.getIX(),
			lhPos.getIY(),
			lhPos.getIZ(),
			true
		);
		
		GamePageHolder* curHolderTo;
		
		
		minv.copyFrom(&(curHolderFrom->gphMinInCells));
		maxv.copyFrom(&minv);
		maxv.addXYZ(cellsPerHolder);
		
		if (curHolderFrom->pathsReady) {
			
		}
		else {
			return;
		}
		
		ConnectingNodeStruct* testConNode;
		float curPathCost;
		
		FIVector4 pVec1;
		FIVector4 pVec2;
		FIVector4 pVec3;
		
		
		// draw all path points
		lastId = -1;
		for (q = 0; q < curHolderFrom->groupIdStack.size(); q++) {
			
			curId = curHolderFrom->groupIdStack[q].groupId;
			curInd = curHolderFrom->groupIdStack[q].ind;
			
			if (lastId != curId) {
				singleton->setShaderfVec3("matVal", &(singleton->colVecs[curId%16]));
			}
			
			// if (curInd == curHolderFrom->groupInfoStack[curId].centerInd) {
			// 	singleton->setShaderVec3("matVal", 254, 128, 0);
			// }
			
			kk = curInd/(cellsPerHolder*cellsPerHolder);
			jj = (curInd-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			ii = curInd-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
			
			pVec1.copyFrom(&minv);
			pVec1.addXYZ(ii,jj,kk);
			pVec1.addXYZ(0.5f);
			
			if (curHolderFrom->getInfo(curInd) != NULL) {
				curPathCost = curHolderFrom->getInfo(curInd)->pathCost;
				singleton->drawCubeCentered(
					&pVec1, 
					0.15//mixf(0.05f,0.45f,clampfZO(curPathCost/32.0f))
				);
			}
			
			
			// if (curInd == curHolderFrom->groupInfoStack[curId].centerInd) {
			// 	singleton->setShaderfVec3("matVal", &(singleton->colVecs[curId%16]));
			// }
			
			lastId = curId;
		}
		
		
		
		singleton->setShaderVec3("matVal", singleton->smoothTime*255.0f, 0, 0);
		
		for (q = 0; q < curHolderFrom->bestConnectingNodes.size(); q++) {
			
			testConNode = &(curHolderFrom->bestConnectingNodes[q]);
			
			curId = testConNode->groupIdFrom;
			curInd = testConNode->cellIndFrom;
			
			
			curHolderTo = getHolderById(testConNode->blockIdTo,testConNode->holderIdTo);
			
			if (curHolderTo != NULL) {
				drawLineAtIndices(
					curHolderFrom, testConNode->cellIndFrom,
					curHolderTo, testConNode->cellIndTo
					
				);
			}
			
			
		}
		
		
		
	}
	
	
	
	int getClosestPathRad(btVector3 cpBTV, GamePageHolder* &closestHolder) {
		
		int rad = BASE_MOVEABLE_Z;
		
		btVector3 newCoord;
		
		//FIVector4 closestPoint;
		//
		
		int i;
		int j;
		int k;
		int n;
		int q;
		int ind;
		
		int testInd;
		int testDis;
		int bestDis = 99999;
		int bestInd = -1;
		
		int cellsPerHolder = singleton->cellsPerHolder;
		int curInd;
		
		
		for (q = -rad; q <= rad; q++) {
			newCoord = cpBTV + btVector3(0.0f,0.0f,q);
			//closestPoint.setBTV(newCoord);
			
			curInd = singleton->gw->getCellInd(newCoord, closestHolder);
			
			if (closestHolder != NULL) {
				if (closestHolder->idealPathsReady) {
					if (curInd > -1) {
						if (closestHolder->getGroupId(curInd) > -1) {
							return curInd;
						}
					} 
				}
			}
			
			
			
		}
		
		return -1;
	}
	
	
	void loadNearestHolders() {
		
		FIVector4 tempVec;
		
		int q;
		
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
		int curPD;
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
		if (threadPoolList->anyRunning()) {
			
		}
		else {
			
		}
		if (threadPoolPath->anyRunning()) {
			doPaths = false;
		}
		else {
			doPaths = true;
		}
		
		
		
		
		tempVec.copyFrom(singleton->cameraGetPosNoShake());
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
			maxk = min(
				tempVec.getIZ() + curLoadRadius,
				singleton->holdersPerWorld - 1
			);
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
								
								
								if ((curLoadRadius < 2)&&(singleton->pathfindingGen)&&doPaths) {
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
										
										curPD = -1;
										for (q = 0; q < MAX_PDPOOL_SIZE; q++) {
											if (singleton->pdPool[q].isFree) {
												singleton->pdPool[q].isFree = false;
												curPD = q;
												//cout << "locking pdPool " << q << "\n";
												break;
											}
										}
										
										if (curPD >= 0) {
											curHolder->curPD = curPD;
											
											threadPoolList->intData[0] = E_TT_GENLIST;
											threadPoolList->intData[1] = curHolder->blockId;
											threadPoolList->intData[2] = curHolder->holderId;
											
											if (threadPoolList->startThread()) {
												genCount++;
											}
											else {
												singleton->pdPool[curPD].isFree = true;
												//cout << "unlocking pdPool (thread not ready) " << curPD << "\n";
												curHolder->curPD = -1;
											}
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

