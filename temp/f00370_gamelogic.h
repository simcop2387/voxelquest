// f00370_gamelogic.h
//

#include "f00370_gamelogic.e"
#define LZZ_INLINE inline
GameLogic::GameLogic ()
                    {
		
	}
void GameLogic::init (Singleton * _singleton)
                                         {
		singleton = _singleton;
		
		searchedForPath = false;
		didFindPath = false;
		idCounter = 1;
		pathCount = 0;
		
		threadPoolPath = new ThreadPoolWrapper();
		threadPoolPath->init(singleton, 8, true||SINGLE_THREADED);
		
		threadPoolList = new ThreadPoolWrapper();
		threadPoolList->init(singleton, 8, false||SINGLE_THREADED);
		
	}
void GameLogic::applyBehavior ()
                             {
		int i;
		int j;
		float targCount = 0.0f;
		float deltaAng;
		float curDis;
		
		int curActor = singleton->getCurActorUID();
		
		btVector3 offsetVec;
		btVector3 readCenter;
		btVector3 writeCenter;
		
		BaseObj* writeObj;
		BaseObj* readObj;
		
		for (i = 0; i < singleton->gw->visObjects.size(); i++) {
			writeObj = &(singleton->gw->gameObjects[
				singleton->gw->visObjects[i]	
			]);
			writeObj->npcRepel = btVector3(0.0f,0.0f,0.0f);
			writeObj->behaviorTarget = btVector3(0.0f,0.0f,0.0f);
			targCount = 0.0f;
			
			if (
				(writeObj->isHidden) ||
				(writeObj->entType != E_ENTTYPE_NPC) ||
				(writeObj->uid == curActor) // dont apply behavior to active actor
			) {
				
			}
			else {
				
				writeCenter = writeObj->getCenterPoint(E_BDG_CENTER);
				
				for (j = 0; j < singleton->gw->visObjects.size(); j++) {
					readObj = &(singleton->gw->gameObjects[
						singleton->gw->visObjects[j]
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
							if (isFuzzy(offsetVec)) {
								
							}
							else {
								offsetVec.normalize();
								curDis = readCenter.distance(writeCenter);
								writeObj->npcRepel += offsetVec*clampf(4.0f-curDis,-0.05f, 1.0f);
							}
							
							
							if (writeObj->holdingWeapon(-1)) {
								
								if (
									(readObj->isAlive()) &&
									(readObj->uid == curActor) 	
								) {
									
									// has weapon, seek out human opponent
									
									writeObj->behaviorTarget += readObj->getCenterPoint(E_BDG_CENTER);
									targCount += 1.0f;
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
									writeObj->behaviorTarget += readObj->getCenterPoint(E_BDG_CENTER);
									targCount += 1.0f;
								}
							}
							
							
							
						}
						
						
					}
					
				}
				
				if (targCount > 0.0f) {
					writeObj->behaviorTarget /= targCount;
				}
				
			}
		}
		
		
		for (i = 0; i < singleton->gw->visObjects.size(); i++) {
			writeObj = &(singleton->gw->gameObjects[
				singleton->gw->visObjects[i]	
			]);
			
			if (
				(writeObj->isHidden) ||
				(writeObj->entType != E_ENTTYPE_NPC)
			) {
				
			}
			else {
				
				writeObj->bindingPower += 0.0001f;
				
				if (writeObj->isDead()) {
					writeObj->bindingPower = min(writeObj->bindingPower,0.0125f);
				}
				
				if (writeObj->bindingPower > 1.0f) {
					writeObj->bindingPower = 1.0f;
				}
				
				writeObj->jumpCooldown--;
				if (writeObj->jumpCooldown < 0) {
					writeObj->jumpCooldown = 0;
				}
				
				writeObj->isWalking = false;
				
				
				if (writeObj->isAlive()) {
					
					if (writeObj->behaviorTarget.isZero()) {
						
					}
					else {
						deltaAng = writeObj->turnTowardsPointDelta(writeObj->behaviorTarget);
						
						singleton->makeTurn(writeObj->uid,deltaAng*16.0f);
						
						curDis = writeObj->behaviorTarget.distance(writeObj->getCenterPoint(E_BDG_CENTER));
						
						if (curDis > 4.0f) {
							singleton->makeMove(writeObj->uid, btVector3(0.0f,1.0f,0.0f)*clampfZO(curDis-4.0f), true);
						}
						if (curDis < 2.0f) {
							singleton->makeMove(writeObj->uid, btVector3(0.0f,-1.0f,0.0f), true);
						}
						
						singleton->makeMoveVec(writeObj->uid,writeObj->npcRepel);
						
						if (curDis > 6.0f) {
							writeObj->blockCount += clampfZO(1.0 - abs(curDis - writeObj->lastBlockDis)*100.0f);
							
						}
						
						if (writeObj->holdingWeapon(-1)) {
							if (
								(curDis > 1.0f) &&
								(curDis < 5.0f)	
							) {
								writeObj->swingCount += 1.0f;
							
								if (writeObj->swingCount > 200.0f) {
									writeObj->swingCount = 0.0f;
									singleton->makeSwing(writeObj->uid,iGenRand(0,1));
								}
								
							}
						}
						else {
							if (
								(curDis > 1.0f) &&
								(curDis < 5.0f)	
							) {
								singleton->grabThrowObj(writeObj->uid, -1);
							}
						}
						
						
						
						if (writeObj->blockCount > 100.0f) {
							writeObj->blockCount = 0.0f;
							singleton->makeJump(writeObj->uid, true,
								clampfZO(curDis-6.0f)*0.75f + 0.25f	
							);
						}
						writeObj->blockCount *= 0.999f;
						writeObj->lastBlockDis = curDis;
						
						
					}
					
				}
				else { // is dead
					if (writeObj->holdingWeapon(E_HAND_L)) {
						singleton->grabThrowObj(writeObj->uid, E_HAND_L);
					}
					
					if (writeObj->holdingWeapon(E_HAND_R)) {
						singleton->grabThrowObj(writeObj->uid, E_HAND_R);
					}
				}
				
				
				
			}
			
			
			
		}
		
	}
GamePageHolder * GameLogic::getHolderById (int blockId, int holderId)
                                                                 {
		
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
GamePageHolder * GameLogic::getHolderByPR (PathResult * pr)
                                                      {
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
bool GameLogic::holdersEqual (GamePageHolder * h0, GamePageHolder * h1)
                                                                  {
		return (
			(h0->blockId == h1->blockId) &&
			(h0->holderId == h1->holderId)
		);
	}
void GameLogic::addHolderToStack (GamePageHolder * curHolder, int targId)
                                                                     {
		pathSearchStack.push_back(PathResult());
		pathSearchStack.back().blockId = curHolder->blockId;
		pathSearchStack.back().holderId = curHolder->holderId;
		curHolder->visitId = targId;
	}
void GameLogic::remHolderFromStack (int opCode, int targId)
                                                        {
		
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
void GameLogic::fillAllPaths (GamePageHolder * begHolder, GamePageHolder * endHolder, int begInd, int endInd, int targId, int opCode)
          {
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
bool GameLogic::addGroupToStack (ConnectingNodeStruct * testConNode, GamePageHolder * curHolder, int groupId, GamePageHolder * lastHolder, int lastGroupId, int lastIndex, int targId)
          {
		
		bool foundIt = false;
		
		pathSearchStack.push_back(PathResult());
		PathResult* lastRes = &(pathSearchStack.back());
		
		lastRes->blockId = curHolder->blockId;
		lastRes->holderId = curHolder->holderId;
		lastRes->groupId = groupId;
		
		
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
		
		
		curHolder->groupInfoStack[groupId].visitId = targId;
		
		
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
		*/
		
		
		
		
	}
void GameLogic::remGroupFromStack (int opCode, int targId)
                                                       {
		
		
		
		pathSearchStack.pop_back();
	}
void GameLogic::fillAllGroups (GamePageHolder * begHolder, GamePageHolder * endHolder, int begInd, int endInd, int targId, int opCode)
          {
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
			return;
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
			lastIndex,
			targId
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
			
			frontIndex++;
			
			notFound = true;
			
			for (i = 0; i < curHolder->bestConnectingNodes.size();i++) {
				testConNode = &(curHolder->bestConnectingNodes[i]);
				
				if (testConNode->groupIdFrom == curGroupId) {
					
					testHolder = getHolderById(testConNode->blockIdTo,testConNode->holderIdTo);
					groupIdTo = testConNode->groupIdTo;
					
					if (testHolder != NULL) {
						if (testHolder->pathsReady) {
							if (testHolder->groupInfoStack[groupIdTo].visitId == targId) {
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
									lastIndex,
									targId
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
			//	remGroupFromStack(opCode, targId);
			}
			
		}
		
		FOUND_TARG_GROUP:
		
		if (pathSearchStack.size() < 1) {
			return;
		}
		
		
		lastIndex = pathSearchStack.back().lastIndex;
		pathFinalStack.push_back(pathSearchStack.back());
		
		while (lastIndex > -1) {
			pathFinalStack.push_back(pathSearchStack[lastIndex]);
			lastIndex = pathSearchStack[lastIndex].lastIndex;
		}
		
		
	}
bool GameLogic::findBestPath (GamePageHolder * closestHolder, GamePageHolder * closestHolder2, int bestInd, int bestInd2)
          {
		
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
void GameLogic::update ()
                      {
		
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
		
		
		GamePageHolder* conHolder1;
		GamePageHolder* conHolder2;
		
		GamePageHolder* tempHolder;
		GamePageHolder* tempHolder2;
		
		int tempInd;
		int tempInd2;
		
		GamePageHolder* closestHolder;
		GamePageHolder* closestHolder2;
		int bestInd;
		int bestInd2;
		PathResult* curPR;
		
		pathCount = 0;
		
		if (singleton->pathfindingOn) {
			
			bestInd = getClosestPathInd(&(singleton->moveNodes[0]), closestHolder);
			bestInd2 = getClosestPathInd(&(singleton->moveNodes[1]), closestHolder2);
			
			
			drawPointAtIndex(closestHolder, bestInd, 0,128+singleton->smoothTime*127.0f,0, singleton->smoothTime);
			drawPointAtIndex(closestHolder2, bestInd2, 128+singleton->smoothTime*127.0f,0,0, singleton->smoothTime);
			
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
			
			
			if ((bestInd > -1)  && (bestInd2 >-1) && (!searchedForPath) && (singleton->pathFindingStep == 2)) {
				// draw path between index
				
				didFindPath = findBestPath(closestHolder, closestHolder2, bestInd, bestInd2);
				searchedForPath = true;
			}
			
			if (didFindPath) {
				
				
				for (i = 0; i < pathFinalStack.size(); i++) {
					curPR = &(pathFinalStack[i]);
					
					// if (curPR->conNode.blockIdTo < 0) {
						
					// }
					// else {
						
					// 	tempHolder = getHolderById(curPR->testConNode,curPR->holderId)
						
					// 	drawPathToPoint(tempHolder, curPR->conNode.cellIndFrom);
					// 	drawPathToPoint(tempHolder2, curPR->conNode.cellIndTo);
					// }
					
					tempHolder = getHolderById(curPR->blockId,curPR->holderId);
					if ((tempHolder != NULL)) {
						tempInd = tempHolder->groupInfoStack[curPR->groupId].centerInd;
						if (tempInd > -1) {
							drawPointAtIndex(tempHolder, tempInd, 255, 128, 0, singleton->smoothTime);
							
							
							// if (curPR->lastGroupId == -1) {
								
							// }
							// else {
							// 	tempHolder2 = getHolderById(curPR->lastBlockId,curPR->lastHolderId);
								
							// 	if (tempHolder2 != NULL) {
							// 		tempInd2 = tempHolder2->groupInfoStack[curPR->lastGroupId].centerInd;
									
							// 		if (tempInd2 > -1) {
										
							// 			if (curPR->conNode.blockIdFrom < 0) {
											
							// 			}
							// 			else {
											
											
							// 				// drawPathToPoint(tempHolder, curPR->conNode.cellIndTo);
							// 				// drawPathToPoint(tempHolder2, curPR->conNode.cellIndFrom);
							// 			}
										
							// 			// drawPaths(
							// 			// 	tempHolder,
							// 			// 	curPR->groupId,
											
							// 			// 	tempHolder2,
							// 			// 	curPR->lastGroupId
							// 			// );
							// 			// drawPaths(
							// 			// 	tempHolder2,
							// 			// 	curPR->lastGroupId,
											
							// 			// 	tempHolder,
							// 			// 	curPR->groupId
							// 			// );
										
									
							// 		}
									
							// 	}
								
							// }
							
						}
						
						
					}
					
					
					
					conHolder1 = getHolderById(curPR->conNode.blockIdFrom, curPR->conNode.holderIdFrom);
					conHolder2 = getHolderById(curPR->conNode.blockIdTo, curPR->conNode.holderIdTo); 
					
					if (conHolder1 != NULL) {
						drawPathToPoint(conHolder1, curPR->conNode.cellIndFrom);
					}
					if (conHolder2 != NULL) {
						drawPathToPoint(conHolder2, curPR->conNode.cellIndTo);
					}
					
				}
				
				drawPathToPoint(closestHolder, bestInd);
				drawPathToPoint(closestHolder2, bestInd2);
				
				
				
				
				
			}
			
			
			// if (
			// 	singleton->doPathReport
			// ) {
			// 	singleton->doPathReport = false;
				
			// 	cout << "Path Count " << pathCount << "\n";
			// }
			
			
			
			
			for (k = -1; k <= 1; k++) {
				for (j = -1; j <= 1; j++) {
					for (i = -1; i <= 1; i++) {
						drawRegions(
							i,
							j,
							k
						);
					}
				}
			}
			
			
			
			
		}
		
		
		
		
		
		////////////////////
	}
void GameLogic::drawLineAtIndices (GamePageHolder * curPointHolder, int curPointIndex, GamePageHolder * curPointHolder2, int curPointIndex2)
          {
		
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
void GameLogic::drawPointAtIndex (GamePageHolder * curPointHolder, int curPointIndex, int r, int g, int b, float rad)
                                                                                                                 {
		
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
void GameLogic::drawPathToPoint (GamePageHolder * curHolderFrom, int _curInd)
                                                                         {
		
		pathCount++;
		
		int curInd = _curInd;
		int cameFromInd;
		
		int ii;
		int jj;
		int kk;
		int ii2;
		int jj2;
		int kk2;
		
		
		FIVector4 pVec1;
		FIVector4 pVec2;
		
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
		
		
		minv.copyFrom(&(curHolderFrom->gphMinInPixels));
		
		int cellsPerHolder = singleton->cellsPerHolder;
		
		singleton->setShaderVec3("matVal", 255, 0, 255);
		
		
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
void GameLogic::drawRegions (int offX, int offY, int offZ)
          {
		
		
		
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
		
		
		minv.copyFrom(&(curHolderFrom->gphMinInPixels));
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
			
			if (curInd == curHolderFrom->groupInfoStack[curId].centerInd) {
				singleton->setShaderVec3("matVal", 254, 128, 0);
			}
			
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
			
			
			if (curInd == curHolderFrom->groupInfoStack[curId].centerInd) {
				singleton->setShaderfVec3("matVal", &(singleton->colVecs[curId%16]));
			}
			
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
			
			
			
			// kk = curInd/(cellsPerHolder*cellsPerHolder);
			// jj = (curInd-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
			// ii = curInd-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
			
			// pVec1.copyFrom(&minv);
			// pVec1.addXYZ(ii,jj,kk);
			// pVec1.addXYZ(0.5f);
			
			
			// drawLine
			
			// singleton->drawCubeCentered(
			// 	&pVec1,
			// 	singleton->smoothTime
			// );
			
			
		}
		
		
		
	}
void GameLogic::drawPaths (GamePageHolder * curHolderFrom, int groupIdFrom, GamePageHolder * curHolderTo, int groupIdTo)
          {
		
		
		if (
			(curHolderTo == NULL) ||
			(curHolderFrom == NULL) ||
			(groupIdFrom == -1) ||
			(groupIdTo == -1)
		) {
			return;
		}
		
		if (curHolderFrom->pathsReady) {
			
		}
		else {
			return;
		}
		if (curHolderTo->pathsReady) {
			
		}
		else {
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
		
		// FIVector4 lhPos;
		// lhPos.copyFrom(&(singleton->lastHolderPos));
		// lhPos.addXYZ(offX,offY,offZ);
		
		// GamePageHolder* curHolderFrom = singleton->gw->getHolderAtCoords(
		// 	lhPos.getIX(),
		// 	lhPos.getIY(),
		// 	lhPos.getIZ(),
		// 	true
		// );
		
		// bool sameHolder = (
		// 	(curHolderTo->blockId = curHolderFrom->blockId) &&
		// 	(curHolderTo->holderId = curHolderFrom->holderId)
		// );
		
		float curPathCost;
		
		FIVector4 pVec1;
		FIVector4 pVec2;
		FIVector4 pVec3;
		
		
		
		
		singleton->setShaderFloat("isWire", 1.0);
		singleton->setShaderVec3("matVal", 255, 0, 0);
		minv.copyFrom(&(curHolderFrom->gphMinInPixels));
		//minv.multXYZ(cellsPerHolder);
		maxv.copyFrom(&minv);
		maxv.addXYZ(cellsPerHolder);
		singleton->drawBox(&minv, &maxv);
		singleton->setShaderFloat("isWire", 0.0);
		
		
		// draw all path points
		lastId = -1;
		// for (q = 0; q < curHolderFrom->groupIdStack.size(); q++) {
			
		// 	curId = curHolderFrom->groupIdStack[q].groupId;
		// 	curInd = curHolderFrom->groupIdStack[q].ind;
			
		// 	if (lastId != curId) {
		// 		singleton->setShaderfVec3("matVal", &(singleton->colVecs[curId%16]));
		// 	}
			
		// 	if (curInd == curHolderFrom->groupInfoStack[curId].centerInd) {
		// 		singleton->setShaderVec3("matVal", 254, 254, 254);
		// 	}
			
		// 	kk = curInd/(cellsPerHolder*cellsPerHolder);
		// 	jj = (curInd-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		// 	ii = curInd-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
			
		// 	pVec1.copyFrom(&minv);
		// 	pVec1.addXYZ(ii,jj,kk);
		// 	pVec1.addXYZ(0.5f);
			
		// 	if (curHolderFrom->getInfo(curInd) != NULL) {
		// 		curPathCost = curHolderFrom->getInfo(curInd)->pathCost;
		// 		singleton->drawCubeCentered(
		// 			&pVec1, 
		// 			mixf(0.05f,0.45f,clampfZO(curPathCost/128.0f))
		// 		);
		// 	}
			
			
			
			
			
		// 	if (curInd == curHolderFrom->groupInfoStack[curId].centerInd) {
		// 		singleton->setShaderfVec3("matVal", &(singleton->colVecs[curId%16]));
		// 	}
			
		// 	lastId = curId;
		// }
		
		doProc = false;
		
		
		
		
		// draw paths
		
		
		
		
		
		
		// draw highlighted cube here if necessary
		// ??
		
		
		targetGroup = groupIdTo;//curHolderTo->getGroupId(curPointIndex);
		targetBlockId = curHolderTo->blockId;
		targetHolderId = curHolderTo->holderId;
		
		
		
		for (q = 0; q < curHolderFrom->bestConnectingNodes.size(); q++) {
			//doProc = curHolderFrom->pathsReady && (targetGroup > -1);
			
			doProc = false;
			// if (sameHolder) {
				
			// 	if (curHolderFrom->bestConnectingNodes[q].groupIdFrom == targetGroup) {
			// 		doProc = true;
			// 		curInd = curHolderFrom->bestConnectingNodes[q].cellIndFrom;
			// 	}
			// }
			// else {
				if (
					
					(curHolderFrom->bestConnectingNodes[q].groupIdTo == targetGroup) &&
					(curHolderFrom->bestConnectingNodes[q].blockIdTo == targetBlockId) &&
					(curHolderFrom->bestConnectingNodes[q].holderIdTo == targetHolderId)	
					
				) {
					doProc = true;
					curInd = curHolderFrom->bestConnectingNodes[q].cellIndFrom;
				}
			//}
			
			// must set curInd
			//curInd = curHolderTo->groupInfoStack[targetGroup].centerInd;
			
			
			if (doProc) {
				
				
				/////////////
				kk = curInd/(cellsPerHolder*cellsPerHolder);
				jj = (curInd-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
				ii = curInd-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
				
				pVec1.copyFrom(&minv);
				pVec1.addXYZ(ii,jj,kk);
				pVec1.addXYZ(0.5f);
				
				singleton->setShaderVec3("matVal", 255, 255, 0);
				if (curHolderFrom->getInfo(curInd) != NULL) {
					singleton->drawCubeCentered(
						&pVec1, 
						singleton->smoothTime
					);
				}
				/////////////
				
				
				
				drawPathToPoint(curHolderFrom, curInd);
				
				doProc = false;
			}
			
		}
		
		
		
		
		
		
		
		
		// if (curHolderFrom->idealPathsReady) {
		// 	singleton->setShaderVec3("matVal", 30, 30, 30);
		// 	for (q = 0; q < curHolderFrom->groupIdStack.size(); q++) {
				
		// 		//curId = curHolderFrom->groupIdStack[q].groupId;
		// 		curInd = curHolderFrom->groupIdStack[q].ind;
				
		// 		if (curHolderFrom->getInfo(curInd) != NULL) {
		// 				cameFromInd = curHolderFrom->getInfo(curInd)->cameFromInd;
						
		// 				if (
		// 					(cameFromInd > -1) &&
		// 					(cameFromInd < (cellsPerHolder*cellsPerHolder*cellsPerHolder))
		// 				) {
		// 					kk = curInd/(cellsPerHolder*cellsPerHolder);
		// 					jj = (curInd-kk*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		// 					ii = curInd-(kk*cellsPerHolder*cellsPerHolder + jj*cellsPerHolder);
							
		// 					kk2 = cameFromInd/(cellsPerHolder*cellsPerHolder);
		// 					jj2 = (cameFromInd-kk2*cellsPerHolder*cellsPerHolder)/cellsPerHolder;
		// 					ii2 = cameFromInd-(kk2*cellsPerHolder*cellsPerHolder + jj2*cellsPerHolder);
							
							
		// 					pVec1.copyFrom(&minv);
		// 					pVec1.addXYZ(ii,jj,kk);
		// 					pVec1.addXYZ(0.5f);
							
		// 					pVec2.copyFrom(&minv);
		// 					pVec2.addXYZ(ii2,jj2,kk2);
		// 					pVec2.addXYZ(0.5f);
							
		// 					singleton->drawLine(&pVec1, &pVec2);
		// 				}
		// 			}
		// 		}
		// }
		
		
			
			
		
		
		
	}
int GameLogic::getClosestPathInd (FIVector4 * closestPoint, GamePageHolder * & closestHolder)
                                                                                       {
			
			
			
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
void GameLogic::loadNearestHolders ()
                                  {
		
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
#undef LZZ_INLINE
 
