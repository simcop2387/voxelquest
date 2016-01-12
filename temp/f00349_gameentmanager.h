// f00349_gameentmanager.h
//

#include "f00349_gameentmanager.e"
#define LZZ_INLINE inline
GameEntManager::GameEntManager ()
                         {
		
	}
void GameEntManager::init (Singleton * _singleton)
                                         {
		singleton = _singleton;
		
		int i;
		int j;
		int k;
		
		weaponToPlace = E_PG_WPSWORD;
		
		currentActorUID = -1;
		
		activeNode = NULL;
		selectedNode = NULL;
		lastSelNode = NULL;
		currentActor = NULL;
		poseRootJS = NULL;
		
		curPoseType = -1;
		highlightedLimb = -1;
		
		curActorNeedsRefresh = false;
		destroyTerrain = GEN_DEBRIS;
		editPose = false;
		EDIT_POSE = editPose;
		combatOn = true;
		mirrorOn = true;
		orgOn = false;
		isDraggingObject = false;
		firstPerson = false;
		showHealth = false;
		
		lastSubjectDistance = 0.0f;
		
		lastObjInd = 0;
		actObjInd = 0;
		setSelInd(0);
		
		draggingFromInd = 0;
		draggingToInd = 0;
		draggingFromType = E_DT_NOTHING;
		draggingToType = E_DT_NOTHING;
		gameObjCounter = E_OBJ_LENGTH;
		
		
		for (i = 0; i < E_ENTTYPE_LENGTH; i++) {
			entPoolStack[i].curIndex = 0;
			
			switch (i) {
				case E_ENTTYPE_BULLET:
					k = 10;
				break;
				case E_ENTTYPE_TRACE:
					k = 10;
				break;
				case E_ENTTYPE_DEBRIS:
					k = MAX_DEBRIS;
				break;
				default:
					k = 0;
				break;
				
			}

			entPoolStack[i].maxCount = k;
			
			for (j = 0; j < entPoolStack[i].maxCount; j++) {
				placeNewEnt(false, i, &singleton->origin, true);
			}
		}
		
		initAllObjects();
		loadPoseInfo(false);
		
	}
void GameEntManager::checkActorRefresh ()
                                 {
		if (curActorNeedsRefresh&&editPose) {
			refreshActor(getCurActorUID());
			curActorNeedsRefresh = false;
		}
	}
void GameEntManager::closeAllContainers ()
                                  {
		BaseObj* curCont;
		
		bool oldOpen;
		bool didClose = false;
		
		for (itBaseObj iterator = gameObjects.begin(); iterator != gameObjects.end(); iterator++) {
			// iterator->first = key
			// iterator->second = value
			
			
			
			curCont = &(gameObjects[iterator->first]);
			oldOpen = curCont->isOpen;
			
			curCont->isOpen = false;
			
			
			if (oldOpen != curCont->isOpen) {
				didClose = true;
			}
		}
		
		if (didClose) {
			singleton->playSoundEnt("leather0", NULL, 0.1);
		}
	}
bool GameEntManager::anyContainerOpen ()
                                {
		BaseObj* curCont;
		for (itBaseObj iterator = gameObjects.begin(); iterator != gameObjects.end(); iterator++) {
			// iterator->first = key
			// iterator->second = value
			
			curCont = &(gameObjects[iterator->first]);
			
			if (curCont->isOpen) {
				return true;
			}
		}
		return false;
	}
void GameEntManager::togglePoseEdit ()
                              {
		editPose = !editPose;
		EDIT_POSE = editPose;
		
		cout << "editPose " << editPose << "\n";
		
		if (editPose) {
			loadCurrentPose();
		}
	}
void GameEntManager::loadDefaultPose (int actorId)
                                          {
		BaseObj* ca = &(gameObjects[actorId]);
		
		if (ca->defaultPose.group < 0) {
			return;
		}
		
		string tempPoseString = getPoseString(
			ca->defaultPose.group,
			ca->defaultPose.RLBN,
			ca->defaultPose.step
		);
		
		if ( tempPoseString.compare("") == 0 ) {
			
		}
		else {
			gameOrgs[ca->orgId]->loadOrgFromFile(
				tempPoseString,
				false
			);
			gameOrgs[ca->orgId]->updateHandleOffset();
			transformOrg(gameOrgs[ca->orgId], NULL);
		}
		
		
		
	}
void GameEntManager::applyNonPoseData ()
                                {
		if (curPose[curPoseType].group == E_PG_NONPOSE) {
			saveCurrentPose();
			loadNonPoseData(E_PG_NONPOSE, RLBN_NEIT, 0);
		}
		else {
			cout << "Error, switch to E_PG_NONPOSE\n";
		}
	}
void GameEntManager::setFirstPerson (bool _newVal)
                                          {
		
		bool newVal = _newVal;
		
		if (currentActor == NULL) {
			newVal = false;	
		}
		
		if (firstPerson) {
			subjectDistance = lastSubjectDistance;
		}
		
		firstPerson = newVal;
		
		if (firstPerson) {
			lastSubjectDistance = subjectDistance;
		}
		
		cout << "firstPerson " << firstPerson << "\n";
	}
int GameEntManager::getCurActorUID ()
                             {
		if (currentActor == NULL) {
			return -1;
		}
		else {
			return currentActor->uid;
		}
	}
void GameEntManager::updateOrgMat (UIComponent * comp)
                                             {
		
		GameOrgNode* tmpNode = NULL;
		
		if (selectedNode != NULL) {
			
			selectedNode->orgVecs[E_OV_MATPARAMS].setFX(comp->index);
			tmpNode = getMirroredNode(selectedNode);
			if (tmpNode != NULL) {
				tmpNode->orgVecs[E_OV_MATPARAMS].setFX(comp->index);
			}
			makeDirty();
		}
	}
void GameEntManager::doDrag ()
                      {
		performDrag(
			singleton->gameNetwork->isConnected,
			draggingFromInd,
			draggingFromType,
			draggingToInd,
			draggingToType,
			&(singleton->worldMarker)
		);
	}
void GameEntManager::endDrag (int upInd)
                                {
		if (isDraggingObject) {
			
			if (upInd == 0) {
				draggingToInd = 0;
				draggingToType = E_DT_NOTHING;
				performDrag(
				singleton->gameNetwork->isConnected,
					draggingFromInd,
					draggingFromType,
					draggingToInd,
					draggingToType,
					&(singleton->worldMarker)
				);
			}
			else {
				if (upInd >= E_OBJ_LENGTH) {
					
						if (isContainer[gameObjects[upInd].objectType]) {
							draggingToInd = upInd;
							draggingToType = E_DT_WORLD_OBJECT;
						}
						else {
							draggingToInd = 0;
							draggingToType = E_DT_NOTHING;
						}
						
						performDrag(
							singleton->gameNetwork->isConnected,
							draggingFromInd,
							draggingFromType,
							draggingToInd,
							draggingToType,
							&(singleton->worldMarker)
						);
				}						
			}
			
			
			
		}
	}
bool GameEntManager::handleGUI (UIComponent * comp, bool mouseUpEvent, bool mouseDownEvent, bool noTravel, bool wasDoubleClick)
          {
		
		bool isCon = singleton->gameNetwork->isConnected;
		
		int i;
		
		if (comp->uid.compare("#contItemParent") == 0) {
			if (comp->jvNodeNoTemplate != NULL) {
				if (comp->jvNodeNoTemplate->HasChild("objectId")) {
					if (mouseUpEvent) {
						if (isDraggingObject) {
							if (noTravel) {
								
							}
							else {
								draggingToType = E_DT_INV_OBJECT_PARENT;
								draggingToInd = comp->jvNodeNoTemplate->Child("objectId")->number_value;
								doDrag();
							}
						}
					}
				}
			}
			return true;			
		}
		else if (comp->uid.compare("#contItem") == 0) {
			
			
			
			if (comp->jvNodeNoTemplate != NULL) {
				
				
				if (comp->jvNodeNoTemplate->HasChild("objectId")) {
					
					// !!
					
					if (mouseUpEvent) {
						
						if (wasDoubleClick) {
							i = comp->jvNodeNoTemplate->Child("objectId")->number_value;
							if (isContainer[gameObjects[i].objectType]) {
								toggleCont(i, false);
							}
							else {
								gameObjects[i].isEquipped = !(gameObjects[i].isEquipped);
								if (gameObjects[i].isEquipped) {
									singleton->playSoundEvent("showGUI");
								}
								else {
									singleton->playSoundEvent("hideGUI");
								}
								singleton->refreshContainers(false);
							}
							
							
						}
						else if (isDraggingObject) {
							if (noTravel) {
								
							}
							else {
								draggingToType = E_DT_INV_OBJECT;
								draggingToInd = comp->jvNodeNoTemplate->Child("objectId")->number_value;
								doDrag();
							}
							
						}
					}
					else if (mouseDownEvent&&(!editPose)) {
						
						isDraggingObject = true;
						draggingFromType = E_DT_INV_OBJECT;
						draggingFromInd = comp->jvNodeNoTemplate->Child("objectId")->number_value;
					}
				}
			}
			
			return true;
			
		}
		else if (mouseUpEvent) {
			if (comp->uid.compare("#contMenu.close") == 0) {		
				i = comp->getParent()->getChild(1)->jvNodeNoTemplate->Child("objectId")->number_value;
				closeContainer(i);
			}
			else if (comp->uid.compare("ddMenu.removeEntity") == 0) {
				removeEntity(isCon, selObjInd);
			}
			else if (comp->uid.compare("ddMenu.placeEntity.npc") == 0) {
				placeNewEnt(isCon, E_ENTTYPE_NPC, &singleton->lastCellPos);
			}
			else if (comp->uid.compare("ddMenu.placeEntity.sword") == 0) {
				weaponToPlace = E_PG_WPSWORD;
				placeNewEnt(isCon, E_ENTTYPE_WEAPON, &singleton->lastCellPos);
			}
			else if (comp->uid.compare("ddMenu.placeEntity.axe") == 0) {
				weaponToPlace = E_PG_WPAXE;
				placeNewEnt(isCon, E_ENTTYPE_WEAPON, &singleton->lastCellPos);
			}
			else if (comp->uid.compare("ddMenu.placeEntity.mace") == 0) {
				weaponToPlace = E_PG_WPMACE;
				placeNewEnt(isCon, E_ENTTYPE_WEAPON, &singleton->lastCellPos);
			}
			else if (comp->uid.compare("ddMenu.placeEntity.hammer") == 0) {
				weaponToPlace = E_PG_WPHAMMER;
				placeNewEnt(isCon, E_ENTTYPE_WEAPON, &singleton->lastCellPos);
			}
			else if (comp->uid.compare("ddMenu.placeEntity.staff") == 0) {
				weaponToPlace = E_PG_WPSTAFF;
				placeNewEnt(isCon, E_ENTTYPE_WEAPON, &singleton->lastCellPos);
			}
			else if (comp->uid.compare("ddMenu.placeEntity.spear") == 0) {
				weaponToPlace = E_PG_WPSPEAR;
				placeNewEnt(isCon, E_ENTTYPE_WEAPON, &singleton->lastCellPos);
			}
			
			// else if (comp->uid.compare("ddMenu.placeEntity.object") == 0) {
			// 	placeNewEnt(singleton->gameNetwork->isConnected,E_ENTTYPE_OBJ, (int)E_SUB_DEFAULT, &singleton->lastCellPos);
			// }
			else {
				return false;
			}
		}
		else {
			return false;
		}
		
		
		return true;
	}
BaseObj * GameEntManager::getEquipped (BaseObj * parentObj)
                                                 {
		int i;
		
		int curChild;
		
		for (i = 0; i < parentObj->children.size();i++) {
			curChild = parentObj->children[i];
			if (gameObjects[curChild].isEquipped) {
				return &(gameObjects[curChild]);
			}
		}
		
		return NULL;
	}
void GameEntManager::updateDragInfo (int bestInd, bool wasDoubleClick)
                                                              {
		setSelInd(bestInd);
		
		if (selObjInd != 0) {
			if (lastObjInd == selObjInd) {
				
			}
		}
		
		lastObjInd = selObjInd;
		
		
		
		draggingFromInd = 0;
		draggingFromType = E_DT_NOTHING;
		
		if (wasDoubleClick&&(currentActor == NULL)) {
			toggleCont(selObjInd, true);
		}
		
		if ((bestInd >= E_OBJ_LENGTH)&&(!editPose)) {
			
			isDraggingObject = true;
			//singleton->markerFound = true;
			draggingFromInd = selObjInd;
			draggingFromType = E_DT_WORLD_OBJECT;
			
			// todo: make sure bestInd exists
			
		}
		else {
			
			// if (bCtrl) {
			// 	if (bestInd <= 0) {
					
			// 	}
			// 	else {
			// 		activeObject = (E_OBJ)(bestInd);
			// 		hitObject = true;
			// 	}
			// }
			
			// if (hitObject) {
				
			// }
			// else {
			// 	//setCurrentActor(NULL);
			// }
			
			
		}
	}
int GameEntManager::getRandomContId ()
                              {
		return iGenRand2(360,419);
	}
int GameEntManager::getRandomNPCId ()
                             {
		return (iGenRand2(1432,1671)/4)*4;
	}
int GameEntManager::getRandomMonsterId ()
                                 {
		return (iGenRand2(1240,1431)/2)*2;
	}
int GameEntManager::getRandomObjId ()
                             {
		return iGenRand2(0,907);
		
	}
void GameEntManager::fillWithRandomObjects (int parentUID, int gen)
                                                           {
		
		int i;
		int maxObj = iGenRand2(2,16);
		BaseObj* tmpObj = NULL;
		
		int curId;
		
		
		for (i = 0; i < maxObj; i++) {
			gameObjects[gameObjCounter] = BaseObj();
			tmpObj = &(gameObjects[gameObjCounter]);
			
			curId = getRandomObjId();
			
			
			// if (
			// 	(curId%5 == 0) &&
			// 	(gen <= 1)
			// ) {
				
			// 	curId = getRandomContId();
			// }
			
			tmpObj->init(
				gameObjCounter,
				parentUID,
				curId,
				E_ENTTYPE_OBJ,
				(int)E_SUB_DEFAULT,
				&singleton->lastCellPos
			);
			
			gameObjects[parentUID].children.push_back(gameObjCounter);
			
			gameObjCounter++;
			
			if (isContainer[curId] && (gen < 1)) {
				fillWithRandomObjects(gameObjCounter-1, gen + 1);
			}
			
		}
		
	}
void GameEntManager::removeEntity (bool isReq, int ind)
                                               {
		
		if (isReq) {
			singleton->naIntData[0] = ind;
			singleton->gameNetwork->addNetworkAction(
				E_NO_REM_ENT,
				singleton->naUintData,
				singleton->naIntData,
				singleton->naFloatData
			);
			return;
		}
		
		if (ind >= E_OBJ_LENGTH) {
			if (removeVisObject(ind, false)) {
				setSelInd(0);
			}
		}
	}
bool GameEntManager::isRecycledFunc (int poolId)
                                        {
		if (
			(entPoolStack[poolId].maxCount == 0) ||
			(entPoolStack[poolId].entIds.size() < entPoolStack[poolId].maxCount)	
		) {
			return false;
		}
		else {
			return true;
		}
	}
BaseObjType GameEntManager::placeNewEnt (bool isReq, int et, FIVector4 * cellPos, bool isHidden)
          {
		
		BaseObj* tmpObj = NULL;
		
		int newType = 0;
		int poolId = et;
		int xv = 1;
		int yv = 1;
		int zv = 1;
		
		int curEntId;
		
		int mf = 0;
		
		int curSubType = (int)E_SUB_DEFAULT;
		
		bool isRecycled = false;
		
		if (isReq) {
			singleton->naFloatData[0] = cellPos->getFX();
			singleton->naFloatData[1] = cellPos->getFY();
			singleton->naFloatData[2] = cellPos->getFZ();
			singleton->naIntData[0] = et;
			singleton->gameNetwork->addNetworkAction(E_NO_ADD_ENT,singleton->naUintData,singleton->naIntData,singleton->naFloatData);
			return - 1;
		}
		
		float bounciness = 0.0f;
		float friction = 0.9;
		float windResistance = 0.9;
		
		switch (et) {
			case E_ENTTYPE_OBJ:
				newType = getRandomObjId();
				friction = 0.1;
				windResistance = 1.0f;
				bounciness = 0.3;
			break;
			// case E_ENTTYPE_MONSTER:
			// 	newType = getRandomMonsterId();
			// 	mf = 2;
			// 	zv = 2;
			// break;
			case E_ENTTYPE_NPC:
				newType = getRandomNPCId();
				mf = 4;
				zv = 2;
			break;
			
			case E_ENTTYPE_BULLET:
			case E_ENTTYPE_TRACE:
				
				if (singleton->waterBulletOn) {
					newType = 1103;
				}
				else {
					newType = 1139;
				}
				
				// if (et == E_ENTTYPE_TRACE) {
					
				// }
				
			break;
			
			case E_ENTTYPE_DEBRIS:
				newType = 0;
			break;
			case E_ENTTYPE_WEAPON:
				newType = 0;
				
			break;
			
			
		}
		
		
		if (
			isRecycledFunc(et)
		) {
			
			
			isRecycled = true;
			curEntId = entPoolStack[poolId].entIds[
				entPoolStack[poolId].curIndex
			];
			
			entPoolStack[poolId].curIndex++;
			if (entPoolStack[poolId].curIndex == entPoolStack[poolId].maxCount) {
				entPoolStack[poolId].curIndex = 0;
			}
			
		}
		else {
			isRecycled = false;
			curEntId = gameObjCounter;
			entPoolStack[poolId].entIds.push_back(gameObjCounter);
			
		}
		
		FIVector4 newPos;
		newPos.copyFrom(cellPos);
		
		if (isRecycled) {
			removeVisObject(curEntId,true);
		}
		else {
			newPos.addXYZ(0.0f,0.0f,4.0f);
		}
		
		
		
		
		gameObjects[curEntId] = BaseObj();
		tmpObj = &(gameObjects[curEntId]);
		tmpObj->init(
			curEntId,
			0,
			newType,
			et,
			curSubType,
			&newPos
		);
		
		tmpObj->defaultPose.group = -1;
		tmpObj->defaultPose.RLBN = RLBN_NEIT;
		tmpObj->defaultPose.step = 0;
		
		switch (et) {
			case E_ENTTYPE_NPC:
				tmpObj->defaultPose.group = E_PG_NONPOSE;
			break;
			case E_ENTTYPE_WEAPON:
				tmpObj->defaultPose.group = weaponToPlace;
			break;
		}
		
		
		if (
			(et == E_ENTTYPE_BULLET) ||
			(et == E_ENTTYPE_TRACE)
		) {
			
			bounciness = 0.0f;
			friction = 0.0f;
			windResistance = 1.0f;
			
		}
		
		
		tmpObj->isHidden = isHidden;
		tmpObj->bounciness = bounciness;
		tmpObj->friction = friction;
		tmpObj->windResistance = windResistance;
		
		tmpObj->maxFrames = mf;
		
		BaseObjType thisObjId = curEntId;
		
		if (isRecycled) {
			addVisObject(curEntId, true);
		}
		else {
			addVisObject(curEntId, false);
			
			gameObjCounter++;
			
			if (isContainer[newType]) {
				fillWithRandomObjects(gameObjCounter-1, 0);
			}
		}
		
		
		
		return thisObjId;
		
	}
void GameEntManager::performDrag (bool isReq, int _draggingFromInd, int _draggingFromType, int _draggingToInd, int _draggingToType, FIVector4 * _worldMarker)
          {
		
		int i;
		int bestPos;
		bool moveCont = false;
		bool draggedIntoWorldObj = false;
		BaseObj* sourceObj = NULL;
		BaseObj* destObj = NULL;
		
		vector<BaseObjType>* myVec;
		
		if (isReq) {
			singleton->naFloatData[0] = _worldMarker->getFX();
			singleton->naFloatData[1] = _worldMarker->getFY();
			singleton->naFloatData[2] = _worldMarker->getFZ();
			singleton->naIntData[0] = _draggingFromInd;
			singleton->naIntData[1] = _draggingFromType;
			singleton->naIntData[2] = _draggingToInd;
			singleton->naIntData[3] = _draggingToType;
			singleton->gameNetwork->addNetworkAction(E_NO_DRAG_ENT,singleton->naUintData,singleton->naIntData,singleton->naFloatData);
			return;
		}
		
		
		cout << "from " << dragStrings[_draggingFromType] << " to " << dragStrings[_draggingToType] << "\n";
		
		switch (_draggingFromType) {
			case E_DT_NOTHING:
				
			break;
			case E_DT_WORLD_OBJECT:
			
				sourceObj = &(gameObjects[_draggingFromInd]);
			
				switch (_draggingToType) {
					case E_DT_NOTHING:
						
						// singleton->lastCellPos.copyFrom(_worldMarker);
						// singleton->lastCellPos.addXYZ(0,0,5);
						// sourceObj->setCenterPoint(&singleton->lastCellPos);
						
						
					break;
					case E_DT_WORLD_OBJECT:
					case E_DT_INV_OBJECT:
					case E_DT_INV_OBJECT_PARENT:
						destObj = &(gameObjects[_draggingToInd]);
						moveCont = true;
					break;
				}
			break;
			
			case E_DT_INV_OBJECT:
			
				sourceObj = &(gameObjects[_draggingFromInd]);
			
				switch (_draggingToType) {
					case E_DT_NOTHING:
						
						
						singleton->lastCellPos.copyFrom(_worldMarker);
						singleton->lastCellPos.addXYZ(0,0,5);
						sourceObj->startPoint = singleton->lastCellPos.getBTV();
						
						
						gameObjects[sourceObj->parentUID].removeChild(sourceObj->uid);
						addVisObject(sourceObj->uid, false);
						sourceObj->parentUID = 0;
						
						
					break;
					case E_DT_WORLD_OBJECT:
					case E_DT_INV_OBJECT:
					case E_DT_INV_OBJECT_PARENT:
						destObj = &(gameObjects[_draggingToInd]);
						moveCont = true;
					break;
				}
			break;
			case E_DT_INV_OBJECT_PARENT:
				
			break;
		}
		
		
		if (moveCont) {
			
			if (isContainer[destObj->objectType]) {	
				if (_draggingFromInd == _draggingToInd) {
					goto PERFORM_DRAG_END;
				}
			}
			else {
				if (_draggingFromInd == destObj->parentUID) {
					goto PERFORM_DRAG_END;
				}
			}
			
			
			gameObjects[sourceObj->parentUID].removeChild(sourceObj->uid);
			
			if (isContainer[destObj->objectType]) {
				destObj->children.push_back(sourceObj->uid);
				sourceObj->parentUID = destObj->uid;
				draggedIntoWorldObj = true;
			}
			else {
				
				myVec = &(gameObjects[destObj->parentUID].children);
				
				bestPos = 0;
				for (i = 0; i < myVec->size(); i++) {
					if ((*myVec)[i] == destObj->uid) {
						bestPos = i+1;
					}
				}
				myVec->insert(
					myVec->begin()+bestPos,
					sourceObj->uid
				);
				
				sourceObj->parentUID = destObj->parentUID;
			}
			
			if (_draggingFromType == E_DT_WORLD_OBJECT) {
				removeVisObject(sourceObj->uid, false);
			}
			
		}
		
		
		
		if (
			(_draggingFromType == E_DT_INV_OBJECT) ||
			(_draggingToType == E_DT_INV_OBJECT) ||
			(_draggingToType == E_DT_INV_OBJECT_PARENT) ||
			draggedIntoWorldObj
		) {
			singleton->refreshContainers(false);
		}
		
		
		PERFORM_DRAG_END:		
		singleton->markerFound = false;
		isDraggingObject = false;
	}
void GameEntManager::setCurrentActor (BaseObj * ge)
                                          {
		
		
		currentActor = ge;
		if (currentActor == NULL) {
			actObjInd = 0;
			currentActorUID = -1;
			setFirstPerson(false);
		}
		else {
			currentActorUID = ge->uid;
			actObjInd = ge->uid;
			subjectDistance = singleton->BTV2FIV(
				currentActor->getCenterPoint(E_BDG_CENTER)
			)->distance(singleton->cameraGetPosNoShake());
			
			curPoseType = currentActor->entType;
			
		}
		
	}
void GameEntManager::toggleFirstPerson ()
                                 {
		setFirstPerson(!firstPerson);
	}
void GameEntManager::toggleActorSel ()
                              {
		singleton->targetSubjectZoom = 1.0f;
		singleton->subjectZoom = singleton->targetSubjectZoom;
		
		if (selObjInd >= E_OBJ_LENGTH) {
			if (selObjInd == actObjInd) {
				setCurrentActor(NULL);
			}
			else {
				setCurrentActor(&(gameObjects[selObjInd]));
				
				if (currentActor != NULL) {
					
				}
				
				
				singleton->playSoundEnt(
					"swimming0",
					currentActor,
					0.0f,
					0.0f,
					true
				);
				
				singleton->playSoundEnt(
					"walkinggravel0",
					currentActor,
					0.0f,
					0.0f,
					true
				);
				
			}
		}
		else {
			setCurrentActor(NULL);
		}
	}
void GameEntManager::setSelInd (int ind)
                                {
		selObjInd = ind;
	}
void GameEntManager::closeContainer (int i)
                                   {
		singleton->playSoundEnt("leather0", NULL, 0.1);
		gameObjects[i].isOpen = false;
		singleton->refreshContainers(false);
	}
void GameEntManager::toggleCont (int contIndex, bool onMousePos)
                                                        {
		
		if (
			isContainer[gameObjects[contIndex].objectType]
		) {
			singleton->playSoundEnt("leather0", NULL, 0.1);
			gameObjects[contIndex].isOpen = !(gameObjects[contIndex].isOpen);
			singleton->refreshContainers(onMousePos);
		}
		else {
			singleton->playSoundEnt("bump0");	
		}
		
		
	}
void GameEntManager::addVisObject (BaseObjType _uid, bool isRecycled)
                                                             {
		
		BaseObj* ge = &(gameObjects[_uid]);
		
		if (isRecycled) {
			
		}
		else {
			visObjects.push_back(_uid);
		}
		
		if (ge->isHidden) {
			
		}
		else {
			singleton->gamePhysics->addBoxFromObj(_uid, false);
		}
		
	}
bool GameEntManager::removeVisObject (BaseObjType _uid, bool isRecycled)
                                                                {
		int i;
		
		BaseObj* ge = &(gameObjects[_uid]);
		
		singleton->gamePhysics->remBoxFromObj(_uid);
		
		// if (ge->body != NULL) {
		// 	//singleton->gamePhysics->scene->RemoveBody(ge->body);
		// 	ge->body = NULL;
		// }
		
		if (isRecycled) {
			ge->isHidden = true;
			return true;
		}
		else {
			for (i = 0; i < visObjects.size(); i++) {
				if (visObjects[i] == _uid) {
					visObjects.erase(visObjects.begin() + i);
					return true;
				}
			}
		}
		
		
		return false;
	}
int GameEntManager::getClosestObj (int actorId, FIVector4 * basePoint, bool ignoreNPC, float maxDis)
          {
		
		int i;
		float bestDis = maxDis;
		float testDis;
		int testInd;
		int bestInd = -1;
		
		BaseObj* testObj;
		
		for(i = 0; i < visObjects.size(); i++) {
			
			testInd = visObjects[i];
			testObj = &(gameObjects[testInd]);
			
			// dont grab self, or another grabbed object
			if (
				(testInd == actorId) ||
				(testObj->isGrabbedById >= 0) ||
				(testObj->getVel(0)->length() > 2.0f) ||
				(testObj->entType == E_ENTTYPE_BULLET) ||
				(testObj->entType == E_ENTTYPE_TRACE) ||
				(testObj->isHidden) ||
				
				(
					ignoreNPC && (testObj->entType == E_ENTTYPE_NPC)	
				)
			) {
				
			}
			else {
				
				
				
				testDis = testObj->getCenterPointFIV(0)->distance(basePoint);
				
				if (testDis < bestDis) {
					bestDis = testDis;
					bestInd = testInd;
				}
			}
		}
		
		return bestInd;
		
	}
GameOrg * GameEntManager::getCurOrg ()
                             {
		if (currentActor == NULL) {
			return NULL;
		}
		if (currentActor->orgId < 0) {
			return NULL;
		}
		return gameOrgs[currentActor->orgId];
	}
BaseObj * GameEntManager::getActorRef (int uid)
                                      {
		if (uid < 0) {
			return NULL;
		}
		else {
			return &(gameObjects[uid]);
		}
	}
bool GameEntManager::combatMode ()
                          {
		return (currentActor!=NULL)&&combatOn&&(!editPose);
	}
bool GameEntManager::isSwingingWeapon (int actorId, int handNum)
                                                        {
		if (actorId < 0) {
			return false;
		}
		BaseObj* ca = &(gameObjects[actorId]);
		PoseInfo* curPI = &(gamePoseInfo[ca->swingType[handNum]]);
		return (curPI->data[E_PIK_SUBTYPE] == E_SUB_SWING);
	}
bool GameEntManager::isPunching (int actorId, int handNum)
                                                  {
		if (actorId < 0) {
			return false;
		}
		BaseObj* ca = &(gameObjects[actorId]);
		PoseInfo* curPI = &(gamePoseInfo[ca->swingType[handNum]]);
		return (curPI->data[E_PIK_SUBTYPE] == E_SUB_PUNCH);
	}
bool GameEntManager::isKicking (int actorId, int handNum)
                                                 {
		if (actorId < 0) {
			return false;
		}
		BaseObj* ca = &(gameObjects[actorId]);
		PoseInfo* curPI = &(gamePoseInfo[ca->swingType[handNum]]);
		return (curPI->data[E_PIK_SUBTYPE] == E_SUB_KICK);
	}
void GameEntManager::setSwing (float _mx, float _my, int actorId, int handNum, bool isKick)
          {
		
		float mx = _mx;
		float my = -_my;
		
		
		
		if (actorId < 0 ) {
			return;
		}
		
		//int handMod = 0;
		
		BaseObj* ca = &(gameObjects[actorId]);
		
		bool isHolding = (ca->isGrabbingId[handNum] > -1);
		
		// if (handNum == RLBN_RIGT) {
		// 	mx *= -1.0f;
		// }
		
		// if (handNum == RLBN_LEFT) {
		// 	handMod = 1;
		// }
		
		int curAttack = E_PG_SLSH;
		
		if (abs(my) > abs(mx)) {
			if (my > 0.0f) {
				// top
				
				if (isKick) {
					curAttack = E_PG_BKIK;
				}
				else {
					if (isHolding) {
						curAttack = E_PG_HACK;
					}
					else {
						curAttack = E_PG_UPPR;
					}
				}
				
				
			}
			else {
				// bottom
				
				if (isKick) {
					curAttack = E_PG_FRNT;
				}
				else {
					if (isHolding) {
						curAttack = E_PG_STAB;
					}
					else {
						curAttack = E_PG_JABP;
					}
				}
				
			}
		}
		else {
			if (mx > 0.0f) {
				// right
				
				if (isKick) {
					curAttack = E_PG_ROUN;
				}
				else {
					if (isHolding) {
						curAttack = E_PG_SLSH;
					}
					else {
						curAttack = E_PG_HOOK;
					}
				}
				
			}
			else {
				// left
				
				if (isKick) {
					curAttack = E_PG_REVR;
				}
				else {
					if (isHolding) {
						curAttack = E_PG_BACK;
					}
					else {
						curAttack = E_PG_ELBO;
					}
				}
				
			}
		}
		
		//curAttack += handMod;
		
		ca->swingType[handNum] = curAttack;
		
	}
void GameEntManager::nextSwing (int actorId, int handNum)
                                                 {
		
		if (actorId < 0 ) {
			return;
		}
		
		BaseObj* ca = &(gameObjects[actorId]);
		
		ca->swingType[handNum]++;
		
		if (ca->swingType[handNum] > (E_PG_FRNT)) {
			ca->swingType[handNum] = (E_PG_SLSH);
		}
		
		
	}
void GameEntManager::makeShoot (int actorId, int bulletType)
                                                    {
		
		int entNum;
		
		int vx;
		int vy;
		
		
		FIVector4 newCellPos;
		FIVector4 newVel;
		
		if (actorId < 0) {
			return;
		}
		
		
		
		BaseObj* ca = &(gameObjects[actorId]);
		
		if (ca == NULL) {
			
		}
		else {
			
			if (ca->hasBodies()) {
			
				newCellPos.setBTV(ca->getCenterPoint(E_BDG_CENTER));
				
				// vx = cos(ca->ang)*3.0f;
				// vx = vx;
				
				// vy = sin(ca->ang)*3.0f;
				// vy = vy;
				
				
				btVector3 tempBTV = multByOtherRot(
					btVector3(0.0f,3.0f,3.0f),
					ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis()
				);
				newCellPos.addXYZ(tempBTV.getX(), tempBTV.getY(), tempBTV.getZ());
				
				entNum = placeNewEnt(false, bulletType, &newCellPos);
				
				
				
				// gameObjects[entNum].setVel(
				// 	cos(ca->ang)*20.0f,
				// 	sin(ca->ang)*20.0f,
				// 	30.0f
				// );
			
			
				gameObjects[entNum].applyImpulseOtherRot(
					btVector3(0.0,200,200)*gameObjects[entNum].getTotalMass(),
					ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis(),
					true,
					0
				);
			}
			
			
			
			if (bulletType != E_ENTTYPE_TRACE) {
				singleton->playSoundEnt(
					"woosh0",
					ca,
					0.2f
				);
			}
			
			
			
		}
	}
void GameEntManager::bindPose (int actorId, int handNum, bool bindOn)
                                                             {
		
		if (actorId < -1) {
			return;
		}
		
		BaseObj* ca = &(gameObjects[actorId]);
		
		if ((ca->entType != E_ENTTYPE_NPC) || (ca->orgId < 0)) {
			return;
		}
		
		GameOrg* curOrg = gameOrgs[ca->orgId];
		
		BaseObj* grabObj;
		GameOrg* grabObjOrg;
		
		if (ca->isGrabbingId[handNum] > -1) {
			grabObj = &(gameObjects[ca->isGrabbingId[handNum]]);
			grabObjOrg = gameOrgs[grabObj->orgId];
			
			
			
			
			if (bindOn) {
				if (handNum == RLBN_LEFT) {
					curOrg->allNodes[
						getCorrectedName(E_BONE_L_METACARPALS)
					]->children.push_back(
						grabObjOrg->allNodes[E_BONE_C_BASE]
					);
					grabObjOrg->allNodes[E_BONE_C_BASE]->parent = 
						curOrg->allNodes[
							getCorrectedName(E_BONE_L_METACARPALS)
						];
					
					//grabObjOrg->allNodes[E_BONE_C_BASE]->setTangent(-1.0f);
				}
				else {
					curOrg->allNodes[
						getCorrectedName(E_BONE_R_METACARPALS)
					]->children.push_back(
						grabObjOrg->allNodes[E_BONE_C_BASE]
					);
					grabObjOrg->allNodes[E_BONE_C_BASE]->parent = 
						curOrg->allNodes[
							getCorrectedName(E_BONE_R_METACARPALS)
						];
					
					//grabObjOrg->allNodes[E_BONE_C_BASE]->setTangent(1.0f);
				}
			}
			else {
				if (handNum == RLBN_LEFT) {
					curOrg->allNodes[
						getCorrectedName(E_BONE_L_METACARPALS)
					]->children.pop_back();
				}
				else {
					curOrg->allNodes[
						getCorrectedName(E_BONE_R_METACARPALS)
					]->children.pop_back();
				}
				grabObjOrg->allNodes[E_BONE_C_BASE]->parent = NULL;
				
			}
			
			
			
			
		}
		
	}
void GameEntManager::makeGrab (int actorId, int _handNum)
                                                 {
		
		int res;
		
		if (actorId < 0) {
			return;
		}
		
		int handNum = _handNum;
		
		BaseObj* ca = &(gameObjects[actorId]);
		GameOrg* curOrg = gameOrgs[ca->orgId];
		
		BaseObj* grabObj;
		GameOrg* grabObjOrg;
		
		if (handNum < 0) {
			handNum = 0;
			
			if (ca->isGrabbingId[handNum] > -1) {
				handNum = 1;
			}
			
		}
		
		if (ca->isGrabbingId[handNum] < 0) {
			
			// find obj to pickup
			
			res = getClosestObj(
				actorId,
				singleton->BTV2FIV(ca->getCenterPoint(E_BDG_CENTER)),
				true,
				5.0f
			);
			
			if (res < 0) {
				
			}
			else {
				
				curOrg->stepCount = 0;
				curOrg->totTime = 0;
				ca->setActionState(E_ACT_ISPICKINGUP,RLBN_NEIT,true);
				
				grabObj = &(gameObjects[res]);
				grabObjOrg = gameOrgs[grabObj->orgId];
				
				singleton->playSoundEnt(
					"scrape0",
					ca,
					0.2f
				);
				
				ca->isGrabbingId[handNum] = res;
				grabObj->setGrabbedBy(actorId, handNum);
				
				bindPose(actorId,handNum,true);
				
				
			}
			
			
		}
		
		
	}
void GameEntManager::makeDropAll (int actorId)
                                      {
		int i;
		
		
		
		if (actorId < 0) {
			return;
		}
		
		BaseObj* ca = &(gameObjects[actorId]);
		
		for (i = 0; i < 2; i++) {
			if (ca->isGrabbingId[i] > -1) {
				makeThrow(ca->uid,i);
			}
		}
	}
void GameEntManager::makeThrow (int actorId, int _handNum)
                                                  {
		
		int res;
		
		if (actorId < 0) {
			return;
		}
		
		int handNum = _handNum;
		
		BaseObj* ca = &(gameObjects[actorId]);
		GameOrg* curOrg = gameOrgs[ca->orgId];
		
		if (ca->entType != E_ENTTYPE_NPC) {
			return;
		}
		
		BaseObj* grabObj;
		GameOrg* grabObjOrg;
		
		if (handNum < 0) {
			handNum = 0;
			
			if (ca->isGrabbingId[handNum] < 0) {
				handNum = 1;
			}
			
		}
		
		
		if (ca->isGrabbingId[handNum] > -1) {
			// throw current obj
			
			grabObj = &(gameObjects[ca->isGrabbingId[handNum]]);
			grabObjOrg = gameOrgs[grabObj->orgId];
			
			
			if (ca->hasBodies()) {
				grabObj->applyImpulseOtherRot(
					btVector3(
						0.0,
						singleton->conVals[E_CONST_THROW_STRENGTHXY],
						singleton->conVals[E_CONST_THROW_STRENGTHZ]
					)*grabObj->getMarkerMass(),
					ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis(),
					false,
					0
				);
			}
			
			singleton->playSoundEnt(
				"woosh0",
				ca,
				0.2f
			);
			
			bindPose(actorId,handNum,false);
			grabObj->setGrabbedBy(-1, -1);
			ca->isGrabbingId[handNum] = -1;
			
			
		}
		
	}
void GameEntManager::makeSwing (int actorId, int handNum)
                                                 {
		
		if (editPose) {
			return;
		}
		
		if (actorId < 0 ) {
			return;
		}
		
		BaseObj* ca = &(gameObjects[actorId]);
		
		if (ca->entType != E_ENTTYPE_NPC) {
			return;
		}
		
		GameOrg* curOrg;
		
		if (ca->isDead()) {
			return;
		}
		
		int i;
		
		if (ca->getActionState(E_ACT_ISSWINGING,handNum) || (ca->bindingPower < 0.01f)) {
			
		}
		else {
			
			for (i = 0; i < RLBN_LENGTH; i++) {
				if (i != handNum) {
					ca->setActionState(E_ACT_ISSWINGING,i,false);
					ca->setActionState(E_ACT_HASNOTHIT,i,false);
				}
			}
			
			
			
			//if (ca->weaponActive) {
				
				ca->setActionState(E_ACT_ISSWINGING,handNum,true);
				ca->setActionState(E_ACT_HASNOTHIT,handNum,true);
				curOrg = gameOrgs[ca->orgId];
				curOrg->stepCount = 0;
				curOrg->totTime = 0;
				singleton->playSoundEnt("woosh0", ca, 0.25f);
				singleton->playSoundEnt("gruntm0", ca, 0.25f, 0.1f);
				
				
				
				if (ca->baseContact()) {
					makeMove( actorId,
						btVector3(
							0.0f,
							singleton->conVals[E_CONST_DASH_AMOUNT],
							singleton->conVals[E_CONST_DASH_UP_AMOUNT]
						),
						true,
						false
					);
				}
				
				if (ca->uid != getCurActorUID()) {
					nextSwing(actorId,RLBN_LEFT);
					nextSwing(actorId,RLBN_RIGT);
				}
				
				
				
			//}
			
			
		}
		
		
	}
void GameEntManager::makeTurn (int actorId, float dirFactor)
                                                    {
		
		BaseObj* ca = &(gameObjects[actorId]);
		
		if (ca->bodies.size() < 0) {
			return;
		}
		
		ca->applyAngularImpulse(btVector3(0,0,dirFactor), true, 0);
	}
void GameEntManager::makeMoveVec (int actorId, btVector3 moveVec)
                                                         {
		BaseObj* ca = &(gameObjects[actorId]);
		
		if (ca->bodies.size() < 0) {
			return;
		}
		
		float walkAmount; 
		
		if (ca->baseContact()) {
			walkAmount = singleton->conVals[E_CONST_WALK_AMOUNT];
		}
		else {
			walkAmount = singleton->conVals[E_CONST_WALK_AMOUNT_AIR];
		}
		
		if (ca->hasBodies()) {
			ca->setActionState(E_ACT_ISWALKING,RLBN_NEIT,true);
			ca->applyImpulse(
				moveVec*walkAmount*ca->getMarkerMass(),
				true,
				0
			);
		}
	}
void GameEntManager::makeMove (int actorId, btVector3 moveDir, bool relative, bool delayed)
                                                                                   {
		BaseObj* ca = &(gameObjects[actorId]);
		
		btVector3 newMoveDir = moveDir;
		
		if (ca->bodies.size() < 0) {
			return;
		}
		
		float walkAmount; 
		
		if (ca->baseContact()) {
			walkAmount = singleton->conVals[E_CONST_WALK_AMOUNT];
		}
		else {
			walkAmount = singleton->conVals[E_CONST_WALK_AMOUNT_AIR];
		}
		
		if (ca->hasBodies()) {
			ca->setActionState(E_ACT_ISWALKING,RLBN_NEIT,true);
			
			
			newMoveDir *= walkAmount;
			
			if (ca->baseContact()) {
				newMoveDir +=	btVector3(
					0.0f,
					0.0f,
					singleton->conVals[E_CONST_WALK_UP_AMOUNT]
				);
			}
			
			
			if (relative) {
				ca->applyImpulseOtherRot(
					newMoveDir*ca->getMarkerMass(),
					ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis(),
					delayed,
					0
				);
			}
			else {
				ca->applyImpulse(
					newMoveDir*ca->getMarkerMass(),
					delayed,
					0
				);
			}
		}
	}
void GameEntManager::makeJump (int actorId, int isUp, float jumpFactor)
                                                               {
		
		BaseObj* ge = &(gameObjects[actorId]);
		
		if (ge->bodies.size() < 0) {
			return;
		}
		
		if (ge->jumpCooldown > 0) {
			return;
		}
		
		if (ge->baseContact()) {
			
		}
		else {
			return;
		}
		
		float jumpAmount = singleton->conVals[E_CONST_JUMP_AMOUNT]*ge->getMarkerMass()*jumpFactor;
		
		
		if (isUp == 1) {
			if (ge->bodies[E_BDG_CENTER].inWater) {
				
				
				if (
					singleton->gw->getCellAtCoords(
						ge->getCenterPoint(E_BDG_CENTER).getX(),
						ge->getCenterPoint(E_BDG_CENTER).getY(),
						ge->getCenterPoint(E_BDG_CENTER).getZ() + 1.0f
					) == E_CD_EMPTY
				) {
					
					
					// at water surface
					
					ge->applyImpulse(btVector3(0.0f,0.0f,jumpAmount), false, 0);
					
					
					
				}
				else {
					
					// underwater
					
					
					ge->applyImpulse(btVector3(0.0f,0.0f,jumpAmount), false, 0);
					
					singleton->playSoundEnt(
						"bubble0",
						ge,
						0.3f,
						0.5f
					);
				}
				
				ge->setActionState(E_ACT_ISJUMPING,RLBN_NEIT,true);
				ge->jumpCooldown = 100;
				
			}
			else {
				if (ge->allFalling()) {
					
				}
				else {
					
					
					ge->applyImpulse(btVector3(0.0f,0.0f,jumpAmount), false, 0);
					ge->zeroZ = true;
					
					singleton->playSoundEnt(
						"jump0",
						ge,
						0.1f,
						0.2f
					);
					
					ge->setActionState(E_ACT_ISJUMPING,RLBN_NEIT,true);
					ge->jumpCooldown = 100;
					
				}
			}
		}
		else {
			if (ge->bodies[E_BDG_CENTER].inWater) {
				ge->applyImpulse(btVector3(0.0f,0.0f,-jumpAmount), false, 0);
				
				singleton->playSoundEnt(
					"bubble0",
					ge,
					0.3f,
					0.5f
				);
			}
		}
		
		
		
	}
void GameEntManager::makeHit (int attackerId, int victimId, int weaponId)
          {
		
		btVector3 impVec;
		
		BaseObj* geAttacker = getActorRef(attackerId);
		BaseObj* geVictim = getActorRef(victimId);
		BaseObj* geWeapon = getActorRef(weaponId);
		
		int lastHealth;
		
		GameOrg* curOrg = NULL;
		
		int i;
		
		if (geAttacker == NULL) {
			return;
		}
		
		if (geAttacker->isDead()) {
			return;
		}
		
		if (geVictim == NULL) {
			
		}
		else {
			if (
				(geVictim->entType == E_ENTTYPE_WEAPON) &&
				(geWeapon != NULL)
			) {
				if (geWeapon->isGrabbedById == geVictim->isGrabbedById) {
					// owners two weapons hit each other
					return;
				}
			}
		}
		
		
		
		
		for (i = 0; i < RLBN_LENGTH; i++) {
			
			
			if (geAttacker->getActionState(E_ACT_HASNOTHIT,i)) {
				if (geAttacker->orgId > -1) {
					curOrg = gameOrgs[geAttacker->orgId];
					
					if (curOrg->stepCount > 1) {
						geAttacker->setActionState(E_ACT_HASNOTHIT,i,false);
						
						
						if (geVictim == NULL) {
							// hit static obj
							
							if (geWeapon != NULL) {
								singleton->playSoundEnt("metalhit5",geAttacker,0.2,0.5f);
								
								if (destroyTerrain) {
									tempVec1.setBTV(geWeapon->getCenterPoint(E_BDG_CENTER));
									singleton->gameFluid[E_FID_BIG]->pushExplodeBullet(true,&tempVec1,0,4.0f);
								}
								
								
							}
							
							
						}
						else {
							
							if (geVictim->entType == E_ENTTYPE_WEAPON) {
								singleton->playSoundEnt("clang0",geAttacker,0.1,1.0f);
								geAttacker->setActionState(E_ACT_ISSWINGING,i,false);
							}
							
							if (geVictim->entType == E_ENTTYPE_NPC) {
								singleton->playSoundEnt("hit0",geVictim,0.3,1.0f);
								if (geVictim->isAlive()) {
									singleton->playSoundEnt("grunthitm0",geVictim,0.15,0.2f);
								}
							}
							
							
							
							
							if (geVictim->entType == E_ENTTYPE_NPC) {
								geVictim->setActionState(E_ACT_ISHIT,RLBN_NEIT,true);
								geVictim->bindingPower = singleton->conVals[E_CONST_BINDING_POW_ON_HIT];
								lastHealth = geVictim->curHealth;
								geVictim->curHealth -= 32;
								if (geVictim->curHealth < 0) {
									geVictim->curHealth = 0;
								}
								
								if (geVictim->isDead() && (lastHealth > 0)) {
									// just died
									
									
									// geVictim->bodies[E_BDG_CENTER].body->setAngularFactor(
									// 	btVector3(1.0f,1.0f,1.0f)
									// );
									// geVictim->bodies[E_BDG_CENTER].body->setAngularVelocity(btVector3(1.0f,1.0f,0.0f)*20.0f);
									
									makeDropAll(geVictim->uid);
									
									singleton->playSoundEnt("dyingm0",geVictim,0.15,0.2f);
									
								}
								
								impVec = geVictim->getCenterPoint(E_BDG_CENTER) - geAttacker->getCenterPoint(E_BDG_CENTER);
								impVec *= btVector3(1.0f,1.0f,0.0f);
								impVec.normalize();
								//impVec += btVector3(0.0f,0.0f,2.0f);
								geVictim->applyImpulse(
									impVec*geVictim->getMarkerMass()*singleton->conVals[E_CONST_HIT_STRENGTH],
									false,
									E_BDG_CENTER
								);
							}
						}
					}
				}
			}
			
			
			
		}
		
	}
GameOrgNode * GameEntManager::getMirroredNode (GameOrgNode * curNode)
                                                           {
		if (getCurOrg() == NULL) {
			return NULL;
		}
		GameOrg* testOrg = getCurOrg();
		
		if (mirrorOn) {
			
			
			if (curNode->nodeName < E_BONE_C_BEG) {
				if (curNode->nodeName <= E_BONE_L_END) {
					return testOrg->baseNode->getNode(
						curNode->nodeName+(E_BONE_R_BEG-E_BONE_L_BEG)
					);
				}
				else {
					return testOrg->baseNode->getNode(
						curNode->nodeName-(E_BONE_R_BEG-E_BONE_L_BEG)
					);
				}
			}
			else {
				
				
				switch(curNode->nodeName) {
					
					case E_BONE_WEAPON_CROSSR:
						return testOrg->allNodes[E_BONE_WEAPON_CROSSL];
					break;
					case E_BONE_WEAPON_CROSSL:
						return testOrg->allNodes[E_BONE_WEAPON_CROSSR];
					break;
					case E_BONE_WEAPON_BLADER:
						return testOrg->allNodes[E_BONE_WEAPON_BLADEL];
					break;
					case E_BONE_WEAPON_BLADEL:
						return testOrg->allNodes[E_BONE_WEAPON_BLADER];
					break;
					
				}
				
			}
		}
		
		
		
		return NULL;
		
	}
void GameEntManager::refreshActor (int actorId)
                                       {
		
		int q;
		
		if (actorId < 0) {
			return;
		}
		BaseObj* ca = &(gameObjects[actorId]);
		GameOrg* curOrg = (gameOrgs[ca->orgId]);
		
		int grabberId = ca->isGrabbedById;
		
		for (q = 0; q < 2; q++) {
			bindPose(grabberId, q, false);
		}
		
		transformOrg(curOrg, NULL);
		
		curOrg->updateHandleOffset();
		
		singleton->gamePhysics->addBoxFromObj(actorId, true);
		
		transformOrg(curOrg, NULL);
		
		for (q = 0; q < 2; q++) {
			bindPose(grabberId, q, true);
		}
		
		ca->setGrabbedBy(grabberId,ca->isGrabbedByHand);
		
		transformOrg(curOrg, NULL);
	}
void GameEntManager::applyNodeChanges (GameOrgNode * _curNode, float dx, float dy)
                                                                         {
		
		GameOrgNode* curNode = _curNode;
		
		int i;
		int j;
		int k;
		
		int q;
		
		float xm = 0.0f;
		float ym = 0.0f;
		float zm = 0.0f;
		
		float dirMod = 1.0f;
		
		int cuid = getCurActorUID();
		if (cuid < 0) {
			return;
		}
		
		//GameActor* curActor = (gameActors[currentActor->actorId]);
		//BodyStruct* curBody;
		
		GameOrg* testOrg = getCurOrg();
		
		if (
			(
				(curNode->nodeName < E_BONE_C_BEG) ||
				(
					(curNode->nodeName >= E_BONE_WEAPON_CROSSR) &&
					(curNode->nodeName <= E_BONE_WEAPON_BLADEL)	
				)
			) &&
			(mirrorOn)
		) {
			j = 2;
		}
		else {
			j = 1;
		}
		
		for (i = 0; i < j; i++) {
			
			
			if (i == 1) {
				curNode = getMirroredNode(curNode);
				
				dirMod = -1.0f;
			}
			
			
			if (singleton->abDown) {
				makeDirty();
			}
			
			xm = dx/100.0f;
			ym = dy/100.0f;
			
			if (singleton->bShift) {
								
				if (singleton->lbDown) {
					curNode->orgVecs[E_OV_TBNRAD0].addXYZ(0.0f,xm,ym);
				}
				if (singleton->rbDown) {
					curNode->orgVecs[E_OV_TBNRAD1].addXYZ(0.0f,xm,ym);
				}
				if (singleton->mbDown) {
					curNode->orgVecs[E_OV_TBNRAD0].addXYZ(ym, 0.0f, 0.0f);
					curNode->orgVecs[E_OV_TBNRAD1].addXYZ(ym, 0.0f, 0.0f);
					
					curActorNeedsRefresh = true;
				}
			}
			else if (singleton->bCtrl) {
				if (singleton->lbDown) {
					curNode->orgVecs[E_OV_POWVALS].addXYZW(xm, ym, 0.0f, 0.0f);
				}
				if (singleton->rbDown) {
					curNode->orgVecs[E_OV_POWVALS].addXYZW(0.0f, 0.0f, xm, ym);
				}
				
				if (singleton->mbDown) {
					curNode->orgVecs[E_OV_TBNOFFSET].addXYZW(0.0f, 0.0f, 0.0f, ym);
				}
				
				// if (singleton->mbDown) {
				// 	curNode->orgVecs[E_OV_POWVALS].addXYZ(ym, 0.0f, 0.0f);
				// 	curNode->orgVecs[E_OV_POWVALS].addXYZ(ym, 0.0f, 0.0f);
				// }
			}
			else {
				if (singleton->lbDown) {
					curNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(dirMod*ym,0.0,0.0); //dirMod*ym
					//curNode->orgVecs[E_OV_TBNOFFSET].addXYZ(dirMod*ym,0.0,0.0);
				}
				if (singleton->rbDown) {
					curNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(0.0,0.0,dirMod*ym);
				}
				if (singleton->mbDown) {
					curNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(0.0,dirMod*ym,0.0);
				}
			}
			
			// if (applyToChildren) {
			// 	for (k = 0; k < curNode->children.size(); k++) {
			// 		applyNodeChanges(curNode->children[k], dx, dy);
			// 	}
			// }
			
			
		}
			
		
	}
void GameEntManager::transformOrg (GameOrg * curOrg, GameOrgNode * tempParent)
          {
		curOrg->baseNode->doTransform(singleton, tempParent);
	}
void GameEntManager::resetActiveNode ()
                               {
		if (editPose) {
			
			
			
			
			GameOrgNode* curNode = NULL;
			
			GameOrg* testOrg = getCurOrg();
			
			if (selectedNode == NULL) {
				
				curNode = lastSelNode;
			}
			else {
				
				curNode = selectedNode;
			}
			
			if (curNode != NULL) {
				
				if (testOrg != NULL) {
					
					switch(curPoseType) {
						case E_ENTTYPE_NPC:
							testOrg->setToPose(getPose(E_PG_TPOSE,RLBN_NEIT,0),1.0,curNode->nodeName);
						break;
						case E_ENTTYPE_WEAPON:
							testOrg->setToPose(getPose(E_PG_WPTPOSE,RLBN_NEIT,0),1.0,curNode->nodeName);
							refreshActor(getCurActorUID());
						break;
					}
					
				}
				
				//curNode->rotThe = 0.0f;
				//curNode->rotPhi = 0.0f;
				//curNode->rotRho = 0.0f;
				
				//curNode->tbnRadScale0.setFXYZ(1.0f,1.0f,1.0f);
				//curNode->tbnRadScale1.setFXYZ(1.0f,1.0f,1.0f);
				//makeDirty();
				
				
				
			}
			
		}
		
	}
bool GameEntManager::updateNearestOrgNode (bool setActive, FIVector4 * mousePosWS)
                                                                         {
		// tempVec3.setFXYZRef(mousePosWS);
		// tempVec3.addXYZRef(&(testOrg->basePosition),-1.0f);
		
		//worldToScreenBase(&tempVec1, mousePosWS);
		
		GameOrgNode* mirNode = NULL;
		
		if (getCurOrg() == NULL) {
			return false;
		}
		GameOrg* testOrg = getCurOrg();
		
		bestNode = NULL;
		
		int boneId;
		
		highlightedLimb2 = -1;
		highlightedLimb = -1;
		
		singleton->gamePhysics->pickBody(&singleton->mouseMoveOPD);
		
		if (
			(singleton->gamePhysics->lastBodyPick == NULL) ||
			(singleton->gamePhysics->lastBodyUID == -1) ||
			(singleton->gamePhysics->lastBodyUID != getCurActorUID())
		) {
			
		}
		else {
			highlightedLimb = singleton->gamePhysics->lastBodyPick->limbUID;
			
			if (highlightedLimb > -1) {
				boneId = currentActor->bodies[highlightedLimb].boneId;
				
				
				if (boneId > -1) {
					bestNode = testOrg->allNodes[boneId];
					
					// if (mirrorOn) {
					// 	mirNode = getMirroredNode(bestNode);
						
					// 	if (mirNode == NULL) {
							
					// 	}
					// 	else {
					// 		mirNode->nodeName
					// 	}
						
					// }
					
					setSelNode(bestNode);
					if (setActive) {
						activeNode = bestNode;
					}
					return true;
				}
			}
			
		}
		
		
		bestNode = NULL;
		activeNode = NULL;
		setSelNode(NULL);
		return false;
		
	}
void GameEntManager::saveOrgFromMenu (string currentFieldString)
                                                        {
		if (getCurOrg() == NULL) {
			return;
		}
		GameOrg* testOrg = getCurOrg();
		
		float tempVal;
		
		
		tempVal = testOrg->baseNode->orgVecs[E_OV_THETAPHIRHO].getFZ();
		
		testOrg->baseNode->orgVecs[E_OV_THETAPHIRHO].setFZ(0.0f);
		transformOrg(testOrg, NULL);
		
		testOrg->saveOrgToFile(currentFieldString);
		
		testOrg->baseNode->orgVecs[E_OV_THETAPHIRHO].setFZ(tempVal);
		transformOrg(testOrg, NULL);
	}
void GameEntManager::loadOrgFromMenu (string currentFieldString)
                                                        {
		if (getCurOrg() == NULL) {
			return;
		}
		GameOrg* testOrg = getCurOrg();
		
		float tempVal;
		
		testOrg->loadOrgFromFile(currentFieldString, false);
		//orientRotation();
		if (currentActor != NULL) {
			//currentActor->curRot = 1;
		}
		transformOrg(testOrg, NULL);
		makeDirty();
	}
void GameEntManager::makeDirty ()
                         {
		
		// if (currentActor != NULL) {
		// 	currentActor->wakeAll();
		// }
		
		//testOrg->gph->childrenDirty = true;
	}
void GameEntManager::setSelNode (GameOrgNode * newNode)
                                              {
		
		selectedNode = newNode;
		
		// if (selectedNode != NULL) {
		// 	cout << boneStrings[selectedNode->nodeName] << "\n";
		// }
		
		
		
		if (selectedNode != lastSelNode) {
			makeDirty();
		}
		lastSelNode = newNode;
		
	}
bool GameEntManager::hasRLBN (int rlbnRes, int k)
                                         {
		
		bool doProc = false;
		
		
		
		switch (k) {
			case 0:
				doProc = ((rlbnRes&RLBN_FLAG_RIGHT) > 0);
			break;
			case 1:
				doProc = ((rlbnRes&RLBN_FLAG_LEFT) > 0);
			break;
			case 2:
				doProc = ((rlbnRes&RLBN_FLAG_BOTH) > 0);
			break;
			case 3:
				doProc = ((rlbnRes&RLBN_FLAG_NEITHER) > 0);
			break;
		}
		
		return doProc;
		
	}
void GameEntManager::loadPoseInfo (bool justRefresh)
                                            {
		int i;
		int j;
		int k;
		int m;
		
		bool doProc;
		bool allowLoad = true;
		
		int numChildren;
		int numSteps;
		
		JSONValue* jv = NULL;
		JSONValue* curTempl = NULL;
		JSONValue* tempJV = NULL;
		JSONValue* poses = NULL;
		JSONValue* templates = NULL;
		int rlbnRes;
		int poseCount = 0;
		
		string curString;
		
		gamePoses.clear();
		
		
		if (singleton->loadJSON("..\\data\\poseinfo.js", &poseRootJS)) {
			
			templates = poseRootJS->Child("templates");
			poses = poseRootJS->Child("poses");
			//numChildren = poses->CountChildren();
			
			for (i = 0; i < E_PG_LENGTH; i++) {
				
				allowLoad = true;
				
				jv = poses->Child(E_POSE_GROUP_STRINGS[i]);
				
				curTempl = NULL;
				
				if (jv->HasChild("template")) {
					if (
						templates->HasChild(jv->Child("template")->string_value)
					) {
						curTempl = templates->Child(jv->Child("template")->string_value);
					}
					else {
						cout << "invalid template \n";
					}
				}
				
				// gamePoseInfo.push_back(PoseInfo());
				
				
				for (j = 0; j < 2; j++) {
					if (curTempl != NULL) {
						
						
						for (k = 0; k < E_PIK_LENGTH; k++) {
							if (
								curTempl->HasChild(E_PIK_STRINGS[k])
							) {
								
								if (curTempl->Child(E_PIK_STRINGS[k])->IsNumber()) {
									gamePoseInfo[i].data[k] = curTempl->Child(E_PIK_STRINGS[k])->number_value;
								}
								else {
									gamePoseInfo[i].data[k] = stringToEnum(
										E_SUBTYPE_STRINGS,
										E_SUB_LENGTH,
										curTempl->Child(E_PIK_STRINGS[k])->string_value
									);
									gamePoseInfo[i].stringData[k] = curTempl->Child(E_PIK_STRINGS[k])->string_value;
								}
							}
						}
					}
					curTempl = jv;
				}
				
				rlbnRes = gamePoseInfo[i].data[E_PIK_RLBN];
				numSteps = gamePoseInfo[i].data[E_PIK_NUMSTEPS];
				
				for (k = 0; k < 4; k++) {
					for (m = 0; m < MAX_POSE_STEPS; m++) {
						gamePoseInfo[i].poseSteps[k].fileString[m] = "";
						gamePoseInfo[i].poseSteps[k].gamePoseIndex[m] = -1;
					}
				}
				
				if (i == E_PG_WPSWORD) {
				//	allowLoad = false;
				}
				
				
				
				for ( k = 0; k < 4; k++ ) {
					
					if (hasRLBN(rlbnRes,k)) {
						
						for (m = 0; m < numSteps; m++) {
							curString = E_POSE_GROUP_STRINGS[i];
							curString.append("_");
							curString.append(poseSideStrings[k]);
							curString.append(std::to_string(m));
							
							gamePoseInfo[i].poseSteps[k].fileString[m] = curString;
							gamePoseInfo[i].poseSteps[k].gamePoseIndex[m] = poseCount;
							
							if (justRefresh) {
								
							}
							else {
								gamePoses.push_back(new GameOrg());
							}
							
							
							
							gamePoses[poseCount]->init(
								singleton,
								-1,
								gamePoseInfo[i].data[E_PIK_POSETYPE],
								gamePoseInfo[i].data[E_PIK_SUBTYPE]
								
							);
							
							if (allowLoad) {
								gamePoses[poseCount]->loadOrgFromFile(curString, false);
							}
							
							transformOrg(gamePoses[poseCount], NULL);
							
							
							
							gamePoses[poseCount]->basePose.group = i;
							gamePoses[poseCount]->basePose.RLBN = k;
							gamePoses[poseCount]->basePose.step = m;
							
							poseCount++;
						}
					}
					
				}
				
				
				
				
				
				
				
				
				
			}
			
			
			
			for (i = 0; i < E_ENTTYPE_LENGTH; i++) {
				
				curPose[i].RLBN = RLBN_NEIT;
				curPose[i].step = 0;
				
				switch (i) {
					case E_ENTTYPE_NPC:
						curPose[i].group = E_PG_TPOSE;
					break;
					case E_ENTTYPE_WEAPON:
						curPose[i].group = E_PG_WPSWORD;
					break;
					default:
						curPose[i].group = E_PG_TPOSE;
					break;
					
				}
				getIndexForPose(&(curPose[i]));
				
			}
			
			
		}
		
		
		
	}
GameOrg * GameEntManager::getPose (int targPoseGroup, int targRLBN, int targStep)
          {
		int targPose = gamePoseInfo[targPoseGroup].poseSteps[targRLBN].gamePoseIndex[targStep];
		
		if (targPose > -1) {
			return gamePoses[targPose];
		}
		else {
			return NULL;
		}
	}
string GameEntManager::getPoseString (int targPoseGroup, int targRLBN, int targStep)
          {
		return gamePoseInfo[targPoseGroup].poseSteps[targRLBN].fileString[targStep];
	}
GameOrg * GameEntManager::getCurrentPose ()
                                  {
		return getPose(curPose[curPoseType].group,curPose[curPoseType].RLBN,curPose[curPoseType].step);
	}
string GameEntManager::getCurrentPoseString ()
                                      {
		return getPoseString(curPose[curPoseType].group,curPose[curPoseType].RLBN,curPose[curPoseType].step);
	}
int GameEntManager::getActionStateFromPose (int poseNum)
                                                {
		switch (poseNum) {
			
			case E_PG_JUMP:
				return E_ACT_ISJUMPING;
			break;
			case E_PG_PICKUP:
				return E_ACT_ISPICKINGUP;
			break;
			case E_PG_WALKFORWARD:
				return E_ACT_ISWALKING;
			break;
			
			case E_PG_SLSH:
			case E_PG_BACK:
			case E_PG_HACK:
			case E_PG_STAB:
			case E_PG_HOOK:
			case E_PG_ELBO:
			case E_PG_UPPR:
			case E_PG_JABP:
			case E_PG_ROUN:
			case E_PG_REVR:
			case E_PG_BKIK:
			case E_PG_FRNT:
				return E_ACT_ISSWINGING;
			break;
			
			default:
				return E_ACT_NULL;
			break;
		}
	}
void GameEntManager::changePose (int amount)
                                    {
		
		GameOrg* testOrg = getCurOrg();
		
		do {
			curPose[curPoseType].index += amount;
			
			cout << "poseInd " << " " << curPoseType << " " << curPose[curPoseType].index << "\n";
			
			if (curPose[curPoseType].index == gamePoses.size()) {
				curPose[curPoseType].index = 0;
			}
			if (curPose[curPoseType].index < 0) {
				curPose[curPoseType].index = gamePoses.size()-1;
			}
			
			
		} while(getPoseType(curPose[curPoseType].index) != curPoseType);
		
		
		
		int j;
		float* curData;
		
		cout << "Current Pose: " << getCurrentPoseString() << "\n";
		
		setPoseFromIndex(curPose[curPoseType].index);
		
		
		if (testOrg != NULL) {
					
			testOrg->setTPG(curPose[curPoseType].group, curPose[curPoseType].RLBN);
			
			if (editPose) {
				loadCurrentPose();
			}
			
			if (curPoseType == E_ENTTYPE_WEAPON) {
				refreshActor(getCurActorUID());
			}
			
			
		}
	}
void GameEntManager::saveCurrentPose ()
                               {
		GameOrg* testOrg = getCurOrg();
		
		if (editPose) {
			
			if (testOrg != NULL) {
				testOrg->saveOrgToFile(getCurrentPoseString());
				getCurrentPose()->loadOrgFromFile(getCurrentPoseString(), false);
				transformOrg(getCurrentPose(), NULL);
				
				cout << "Saved Pose " << getCurrentPoseString() << "\n";
				
			}
		}
	}
void GameEntManager::getIndexForPose (PoseKey * tempPose)
                                                {
		tempPose->index = gamePoseInfo[
			tempPose->group
		].poseSteps[
			tempPose->RLBN
		].gamePoseIndex[
			tempPose->step
		];
	}
void GameEntManager::setPoseFromIndex (int i)
                                     {
		curPose[curPoseType].index = i;
		curPose[curPoseType].group = gamePoses[curPose[curPoseType].index]->basePose.group;
		curPose[curPoseType].RLBN = gamePoses[curPose[curPoseType].index]->basePose.RLBN;
		curPose[curPoseType].step = gamePoses[curPose[curPoseType].index]->basePose.step;
	}
int GameEntManager::getPoseType (int poseIndex)
                                       {
		int testPoseInd = gamePoses[poseIndex]->basePose.group;
		return gamePoseInfo[testPoseInd].data[E_PIK_POSETYPE];
	}
void GameEntManager::loadNonPoseData (int npdPose, int npdSide, int npdStep)
                                                                    {
		GameOrg* testOrg = getCurOrg();
		
		int i;
		int j;
		int k;
		
		float* curData;/// = gamePoseInfo[targetPose].data;
		
		if (editPose) {
			if (testOrg != NULL) {
				
				for (i = 0; i < gamePoses.size(); i++) {
					
					if (getPoseType(i) == curPoseType) {
						setPoseFromIndex(i);
						loadCurrentPose();
						getCurrentPose()->loadOrgFromFile(getPoseString(npdPose,npdSide,npdStep), true);
						transformOrg(getCurrentPose(), NULL);
						testOrg->setToPose(getCurrentPose(),1.0f);
						transformOrg(testOrg, NULL);
						makeDirty();
						cout << "Loaded Non Pose " << getCurrentPoseString() << "\n";
						saveCurrentPose();
					}
					
					
					
				}
				
				curPose[curPoseType].group = npdPose;
				curPose[curPoseType].RLBN = npdSide;
				curPose[curPoseType].step = npdStep;
				getIndexForPose(&(curPose[curPoseType]));
				
				loadCurrentPose();
			}
		}
	}
void GameEntManager::loadCurrentPose ()
                               {
		GameOrg* testOrg = getCurOrg();
		
		if (editPose) {
			if (testOrg != NULL) {
				
				getCurrentPose()->loadOrgFromFile(getCurrentPoseString(), false);
				transformOrg(getCurrentPose(), NULL);
				testOrg->setToPose(getCurrentPose(),1.0f);
				transformOrg(testOrg, NULL);
				makeDirty();
				cout << "Loaded Pose " << getCurrentPoseString() << "\n";
				
			}
		}
	}
int GameEntManager::numberIcons (int pCurCount, int x1, int y1, int x2, int y2)
                                                                       {
	  int i;
	  int j;
	  int curCount = pCurCount;
	  
	  for (j = y1; j <= y2; j++) {
	    for (i = x1; i <= x2; i++) {
				if (curCount >= MAX_OBJ_TYPES) {
					cout << "error curCount " << curCount << "\n";
				}
				else {
					entIdToIcon[curCount] = i + j * ITEMS_PER_ROW;
				}
	    	
	    	if ((i + j * ITEMS_PER_ROW) >= MAX_ICON_ID) {
	    		cout << "error i + j * ITEMS_PER_ROW "  << (i + j * ITEMS_PER_ROW) << "\n";
	    	}
	    	else {
	    		iconToEntId[i + j * ITEMS_PER_ROW] = curCount;
	    	}
	      
	      
	      curCount++;
	    }
	  }
	  
	  
	  return curCount;
	  
	}
string GameEntManager::getStringForObjectId (int objectId)
                                                  {
		int objType = gameObjects[objectId].objectType;
		int iconNum = entIdToIcon[objType];
		
		// if (isContainer[objType]) {
		// 	return i__s(iconNum) + "& Test Container ";
		// }
		// else {
		// 	if (gameObjects[objectId].isEquipped) {
		// 		return "(E) " + i__s(iconNum) + "& Test Object ";
		// 	}
		// 	else {
		// 		return i__s(iconNum) + "& Test Object ";
		// 	}
		// }
		
		if (gameObjects[objectId].isEquipped) {
			return "(E) " + i__s(iconNum) + "& "+objStrings[objType]+" ";
		}
		else {
			return i__s(iconNum) + "& "+objStrings[objType]+" ";
		}
		
	}
void GameEntManager::initAllObjects ()
                              {
		int i;
		int j;
		int k;
		int itemCount = 0;
		
		for (i = 0; i < MAX_ICON_ID; i++) {
			iconToEntId[i] = 0;
		}
		
		for (i = 0; i < MAX_OBJ_TYPES; i++) {
			entIdToIcon[i] = 0;
		}
		
		
		
		itemCount = numberIcons(itemCount,0,0,11,20);
		itemCount = numberIcons(itemCount,12,0,23,15);
		itemCount = numberIcons(itemCount,24,0,35,16);
		itemCount = numberIcons(itemCount,12,16,21,20);
		itemCount = numberIcons(itemCount,22,17,35,31);
		itemCount = numberIcons(itemCount,0,21,15,31);
		itemCount = numberIcons(itemCount,16,21,21,22);
		itemCount = numberIcons(itemCount,0,32,35,35);
		itemCount = numberIcons(itemCount,0,36,15,47);
		itemCount = numberIcons(itemCount,16,36,35,47);
		
		
		for (i = 0; i < MAX_OBJ_TYPES; i++) {
			isContainer[i] = false;
			objStrings[i] = "";
		}
		for (i = 360; i <= 419; i++ ) {
			isContainer[i] = true;
		}
		for (i = 1240; i <= 1671; i++ ) {
			isContainer[i] = true;
		}
		for (i = 525; i <= 527; i++ ) {
			isContainer[i] = true;
		}
		for (i = 537; i <= 539; i++ ) {
			isContainer[i] = true;
		}
		
		
		for (i = 0; i <= 35; i++) {
			objStrings[i] = gemStrings[i%12] + " and Gold Ring";
		}
		
		for (i = 36; i <= 71; i++) {
			objStrings[i] = gemStrings[i%12] + " and Silver Ring";
		}
		
		for (i = 180; i <= 191; i++) {
			objStrings[i] = gemStrings[i%12] + " Necklace";
		}
		
		for (i = 420; i <= 431; i++) {
			objStrings[i] = gemStrings[i%12] + " Ore";
		}
		
		for (i = 432; i <= 443; i++) {
			objStrings[i] = "Polished " + gemStrings[i%12];
		}
		
		for (i = 72; i <= 419; i++) {
			
			j = i/12;
			
			if (j == 15) {
				
			}
			else {
				objStrings[i] = colorStrings[i%12] + " ";
			}
			
			
			switch (j) {
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
					objStrings[i] += "Cloak";
				break;
				case 11:
					objStrings[i] += "Plate Armor";
				break;
				case 12:
					objStrings[i] += "Leather Armor";
				break;
				case 13:
					objStrings[i] += "Buckler";
				break;
				case 14:
					objStrings[i] += "Kite Shield";
				break;
				case 15:
				
				break;
				case 16:
					objStrings[i] += "Bandana";
				break;
				case 17:
					objStrings[i] += "Boot";
				break;
				case 18:
					objStrings[i] += "Pointed Cap";
				break;
				case 19:
					objStrings[i] += "Plumed Helm";
				break;
				case 20:
					objStrings[i] += "Sailor Cap";
				break;
				case 21:
				case 22:
				case 23:
					objStrings[i] += "Book";
				break;
				case 24:
					objStrings[i] += "Bound Scroll";
				break;
				case 25:
					objStrings[i] += "Scroll";
				break;
				case 26:
				case 27:
					objStrings[i] += "Parchment";
				break;
				case 28:
				case 29:
					objStrings[i] += "Scroll";
				break;
				case 30:
					objStrings[i] += "Bag";
				break;
				case 31:
				case 32:
					objStrings[i] += "Satchel";
				break;
				case 33:
				case 34:
					objStrings[i] += "Gift Box";
				break;
			}
		}
		
		for (i = 698; i <= 907; i++) {
			objStrings[i] = colorStrings[(i+2)%14] + " Potion";
		}
		
		
		for (i = 648; i <= 697; i++) {
			
			j = (i+2)%10;
			
			k = ((i+2)/10) - 65;
			
			objStrings[i] = metalStrings[k] + " ";
			
			
			switch (j) {
				case 0:
					objStrings[i] += "Bullion";
				break;
				case 1:
					objStrings[i] += "Denarii (100)";
				break;
				case 2:
					objStrings[i] += "Denarii (10)";
				break;
				case 3:
					objStrings[i] += "Denarius";
				break;
				case 4:
					objStrings[i] += "Decima";
				break;
				case 5:
					objStrings[i] += "Cent";
				break;
				case 6:
					objStrings[i] += "Medal";
				break;
				case 7:
				case 8:
				case 9:
					objStrings[i] += "Key";
				break;
			}
		}
		
		for (i = 1084; i <= 1095; i++) {
			objStrings[i] = "Parchment";
		}
		
		
		
		for (i = 908; i <= 1083; i++) {
			
			j = (i+4)%16;
			k = ((i+4)/16) - 57;
			
			
			objStrings[i] = elementStrings[j] + " " + weaponStrings[k];
		}
		
		
		objStrings[444] = "Leather Helm";
		objStrings[445] = "Leather Helm";
		objStrings[446] = "Leather Helm";
		objStrings[447] = "Leather Helm";
		objStrings[448] = "Iron Helm";
		objStrings[449] = "Iron Helm";
		objStrings[450] = "Iron Helm";
		objStrings[451] = "Artifact Helm";
		objStrings[452] = "Artifact Helm";
		objStrings[453] = "Artifact Helm";
		objStrings[454] = "Pointed Cap";
		objStrings[455] = "Feathered Cap";
		objStrings[456] = "Cap";
		objStrings[457] = "Mask";
		objStrings[458] = "Top Hat";
		objStrings[459] = "Feathered Hat";
		objStrings[460] = "Cat Ears";
		objStrings[461] = "Rabbit Ears";
		objStrings[462] = "Headband";
		objStrings[463] = "Crown";
		objStrings[464] = "Fur Cap";
		objStrings[465] = "Cap";
		objStrings[466] = "Hat";
		objStrings[467] = "Leather Cap";
		objStrings[468] = "Tunic";
		objStrings[469] = "Tunic";
		objStrings[470] = "Fur Coat";
		objStrings[471] = "Overcoat";
		objStrings[472] = "Leather Armor";
		objStrings[473] = "Leather Armor";
		objStrings[474] = "Iron Plated Armor";
		objStrings[475] = "Iron Plated Armor";
		objStrings[476] = "Iron Plated Armor";
		objStrings[477] = "Artifact Armor";
		objStrings[478] = "Artifact Armor";
		objStrings[479] = "Artifact Armor";
		objStrings[480] = "Slipper";
		objStrings[481] = "Boot";
		objStrings[482] = "Buckled Boot";
		objStrings[483] = "Fur Boot";
		objStrings[484] = "Buckled Boot";
		objStrings[485] = "Shoe";
		objStrings[486] = "Iron Plated Boot";
		objStrings[487] = "Iron Plated Boot";
		objStrings[488] = "Iron Plated Boot";
		objStrings[489] = "Artifact Boot";
		objStrings[490] = "Artifact Boot";
		objStrings[491] = "Artifact Boot";
		objStrings[492] = "Wooden Buckler";
		objStrings[493] = "Wooden Divoted Buckler";
		objStrings[494] = "Wood and Iron Buckler";
		objStrings[495] = "Iron Buckler";
		objStrings[496] = "Iron Buckler";
		objStrings[497] = "Iron Tower Shield";
		objStrings[498] = "Wooden Heater Shield";
		objStrings[499] = "Wood and Iron Heater Shield";
		objStrings[500] = "Iron Heater Shield";
		objStrings[501] = "Decorated Heater Shield";
		objStrings[502] = "Kite Shield";
		objStrings[503] = "Decorated Kite Shield";
		objStrings[504] = "Artifact Shield";
		objStrings[505] = "Artifact Shield";
		objStrings[506] = "Artifact Shield";
		objStrings[507] = "Artifact Shield";
		objStrings[508] = "Belt";
		objStrings[509] = "Belt";
		objStrings[510] = "Belt";
		objStrings[511] = "Artifact Necklace";
		objStrings[512] = "Artifact Necklace";
		objStrings[513] = "Artifact Necklace";
		objStrings[514] = "Artifact Necklace";
		objStrings[515] = "Artifact Necklace";
		objStrings[516] = "Gold Ring";
		objStrings[517] = "Gold Ring";
		objStrings[518] = "Gold Ring";
		objStrings[519] = "Gold Ring";
		objStrings[520] = "Gold Ring";
		objStrings[521] = "Gold Ring";
		objStrings[522] = "Gold Ring";
		objStrings[523] = "Gold Ring";
		objStrings[524] = "Gold Ring";
		objStrings[525] = "Box";
		objStrings[526] = "Box";
		objStrings[527] = "Bag";
		objStrings[528] = "Silver Ring";
		objStrings[529] = "Silver Ring";
		objStrings[530] = "Silver Ring";
		objStrings[531] = "Silver Ring";
		objStrings[532] = "Silver Ring";
		objStrings[533] = "Silver Ring";
		objStrings[534] = "Silver Ring";
		objStrings[535] = "Silver Ring";
		objStrings[536] = "Silver Ring";
		objStrings[537] = "Box";
		objStrings[538] = "Box";
		objStrings[539] = "Bag";
		
		objStrings[540] = "Canine";
		objStrings[541] = "Patch of Fur";
		objStrings[542] = "Hide";
		objStrings[543] = "Claw";
		objStrings[544] = "Feather";
		objStrings[545] = "Horn";
		objStrings[546] = "Mushroom Cap";
		objStrings[547] = "Shell";
		objStrings[548] = "Bone";
		objStrings[549] = "Eyeball";
		objStrings[550] = "Tentacle";
		objStrings[551] = "Bat Wing";
		
		objStrings[552] = "Molar";
		objStrings[553] = "Patch of Fur";
		objStrings[554] = "Hide";
		objStrings[555] = "Claw";
		objStrings[556] = "Feather";
		objStrings[557] = "Horn";
		objStrings[558] = "Mushroom Cap";
		objStrings[559] = "Shell";
		objStrings[560] = "Bone";
		objStrings[561] = "Eyeball";
		objStrings[562] = "Tentacle";
		objStrings[563] = "Bat Wing";
		
		objStrings[564] = "Candle";
		objStrings[565] = "Jelly";
		objStrings[566] = "Mirror";
		objStrings[567] = "Flask";
		objStrings[568] = "Yarn";
		objStrings[569] = "Button";
		objStrings[570] = "Cloth";
		objStrings[571] = "Bell";
		objStrings[572] = "Wood";
		objStrings[573] = "Beak";
		objStrings[574] = "Tail";
		objStrings[575] = "Claw";
		
		objStrings[576] = "Candle";
		objStrings[577] = "Jelly";
		objStrings[578] = "Mirror";
		objStrings[579] = "Flask";
		objStrings[580] = "Yarn";
		objStrings[581] = "Button";
		objStrings[582] = "Cloth";
		objStrings[583] = "Bell";
		objStrings[584] = "Wood";
		objStrings[585] = "Beak";
		objStrings[586] = "Tail";
		objStrings[587] = "Claw";
		
		objStrings[588] = "Apple";
		objStrings[589] = "Lime";
		objStrings[590] = "Orange";
		objStrings[591] = "Passion Fruit";
		objStrings[592] = "Pineapple";
		objStrings[593] = "Banana";
		objStrings[594] = "Cherries";
		objStrings[595] = "Watermelon";
		objStrings[596] = "Bread";
		objStrings[597] = "Cooked Lamb Shank";
		objStrings[598] = "Cooked Egg";
		objStrings[599] = "Cooked Fish";
		objStrings[600] = "Cooked Chicken";
		objStrings[601] = "Sandwich";
		objStrings[602] = "Sliced Potato";
		objStrings[603] = "Steak";
		objStrings[604] = "Sliced Apple";
		objStrings[605] = "Sliced Lime";
		objStrings[606] = "Sliced Orange";
		objStrings[607] = "Sliced Passion Fruit";
		objStrings[608] = "Sliced Pineapple";
		objStrings[609] = "Peeled Banana";
		objStrings[610] = "Cherry";
		objStrings[611] = "Sliced Watermelon";
		objStrings[612] = "Cookie";
		objStrings[613] = "Candy";
		objStrings[614] = "Candy Cane";
		objStrings[615] = "Slice of Cake";
		objStrings[616] = "Dark Chocolate";
		objStrings[617] = "Lollipop";
		objStrings[618] = "Icecream";
		objStrings[619] = "Honey";
		objStrings[620] = "Half Cookie";
		objStrings[621] = "Candy";
		objStrings[622] = "Candy Cane";
		objStrings[623] = "Cake";
		objStrings[624] = "Milk Chocolate";
		objStrings[625] = "Lollipop";
		objStrings[626] = "Icecream";
		objStrings[627] = "Water";
		objStrings[628] = "Cheese";
		objStrings[629] = "Raw Lamb Shank";
		objStrings[630] = "Raw Egg";
		objStrings[631] = "Raw Fish";
		objStrings[632] = "Cooked Chicken";
		objStrings[633] = "Sandwich";
		objStrings[634] = "Potato";
		objStrings[635] = "Raw Steak";
		objStrings[636] = "Cut Emerald";
		objStrings[637] = "Cut Ruby";
		objStrings[638] = "Cut Sapphire";
		objStrings[639] = "Cut Amethyst";
		objStrings[640] = "Cut Beryl";
		objStrings[641] = "Cut Topaz";
		objStrings[642] = "Cut Onyx";
		objStrings[643] = "Cut Diamond";
		objStrings[644] = "Glass";
		objStrings[645] = "Pearl";
		objStrings[646] = "Ash";
		objStrings[647] = "Flint";
		
		
		
		for (i = 0; i < 3; i++) {
			j = i*16;
			objStrings[1240 + j] = "Giant Ant";
			objStrings[1242 + j] = "Giant Rat";
			objStrings[1244 + j] = "Slime";
			objStrings[1246 + j] = "Giant Larva";
			objStrings[1248 + j] = "Giant Wasp";
			objStrings[1250 + j] = "Dread Knight";
			objStrings[1252 + j] = "Carnivorous Plant";
			objStrings[1254 + j] = "Haunted Stump";
			
			objStrings[1288 + j] = "Floating Eye";
			objStrings[1290 + j] = "Gazer";
			objStrings[1292 + j] = "Skeleton";
			objStrings[1294 + j] = "Ghost";
			objStrings[1296 + j] = "Animated Fungus";
			objStrings[1298 + j] = "Necromancer";
			objStrings[1300 + j] = "Electric Eye";
			objStrings[1302 + j] = "Mimic";
			
			objStrings[1336 + j] = "Fire Elemental";
			objStrings[1338 + j] = "Wind Elemental";
			objStrings[1340 + j] = "Earth Elemental";
			objStrings[1342 + j] = "Water Elemental";
			objStrings[1344 + j] = "Golem";
			objStrings[1346 + j] = "Zombie";
			objStrings[1348 + j] = "Imp";
			objStrings[1350 + j] = "Cyclopes";
			
			objStrings[1384 + j] = "Hatchling";
			objStrings[1386 + j] = "Giant Crab";
			objStrings[1388 + j] = "Giant Snake";
			objStrings[1390 + j] = "Giant Frog";
			objStrings[1392 + j] = "Giant Snail";
			objStrings[1394 + j] = "Dark Lord";
			objStrings[1396 + j] = "Animated Armor";
			objStrings[1398 + j] = "Banshee";
		}
		
		for (i = 0; i < 4; i++) {
			j = i*20;
			
			objStrings[1432 + j] = "Female Townsperson";
			objStrings[1436 + j] = "Male Townsperson";
			objStrings[1440 + j] = "Priest";
			objStrings[1444 + j] = "Rogue";
			objStrings[1448 + j] = "Theif";
			
			objStrings[1512 + j] = "Guard";
			objStrings[1516 + j] = "Townsperson";
			objStrings[1520 + j] = "Knight";
			objStrings[1524 + j] = "Assassin";
			objStrings[1528 + j] = "Warlord";
			
			objStrings[1592 + j] = "Ninja";
			objStrings[1596 + j] = "Old Man";
			objStrings[1600 + j] = "Old Woman";
			objStrings[1604 + j] = "Paladin";
			objStrings[1608 + j] = "Wizard";
			
		}
		
	}
#undef LZZ_INLINE
 
