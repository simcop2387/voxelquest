// f00339_gameorg.h
//

#include "f00339_gameorg.e"
#define LZZ_INLINE inline
float GameOrg::gv (float * vals)
                              {
		float lerp = fGenRand();
		return vals[0]*lerp + vals[1]*(1.0f-lerp);
	}
float const GameOrg::baseMat = 12.0f;
GameOrg::GameOrg ()
                  {
		basePose.group = -1;
		basePose.RLBN = -1;
		basePose.step = -1;
		targetPose.RLBN = RLBN_NEIT;
		targetPose.group = -1;
		rootObj = NULL;
		defVecLength = 0.05f*ORG_SCALE_BASE;
	}
void GameOrg::init (Singleton * _singleton, int _ownerUID, int _entType, int _subType)
          {
		singleton = _singleton;

		stepCount = 0;

		ownerUID = _ownerUID;

		entType = _entType;
		subType = _subType;

		// GameOrgNode(
		// 	GameOrgNode* _parent,
		// 	int _nodeName,
		// 	float _boneLength,
		// 	float _tanLengthInCells,
		// 	float _bitLengthInCells,
		// 	float _norLengthInCells,
			
		// 	float _tanX, float _tanY, float _tanZ,
		// 	float _bitX, float _bitY, float _bitZ,
		// 	float _norX, float _norY, float _norZ
			
			
		// )
		
		int i;
		
		for (i = 0; i < E_BONE_C_END; i++) {
			allNodes[i] = NULL;
		}
		
		switch (entType) {
			case E_ENTTYPE_NPC:
				initHuman();
			break;
			case E_ENTTYPE_WEAPON:
				initWeapon();
			break;
		}
		
		singleton->curOrgId++;
		
		
	}
void GameOrg::jsonToNode (JSONValue * * parentObj, GameOrgNode * curNode, bool notThePose)
                                                                                      {
		
		int i;
		
		
		curNode->nodeName = (*parentObj)->Child("id")->number_value;
		
		JSONValue* tempVal;
		
		bool doProc;
		
		tempVal = (*parentObj)->Child("orgVecs");
		
		int mv1 = tempVal->CountChildren()/4;
		int mv2 = E_OV_LENGTH;
		
		int numChildren = min(mv1, mv2);
		
		for (i = 0; i < numChildren; i++) {	
		
			doProc = false;
			if (notThePose) {
				switch (i) {
					//case E_OV_TANGENT:
					//case E_OV_BITANGENT:
					//case E_OV_NORMAL:
					case E_OV_TBNRAD0:
					case E_OV_TBNRAD1:
					//case E_OV_THETAPHIRHO:
					//case E_OV_TPRORIG:
					case E_OV_MATPARAMS:
						doProc = true;
					break;
				}
			}
			else {
				doProc = true;
			}
			
		
			if (doProc) {
				
				curNode->orgVecs[i].setFXYZW(
					tempVal->array_value[i*4 + 0]->number_value,
					tempVal->array_value[i*4 + 1]->number_value,
					tempVal->array_value[i*4 + 2]->number_value,
					tempVal->array_value[i*4 + 3]->number_value
				);
			}
		
			
			
		}
		
		
		int totSize = 0;
		
		
		if ((*parentObj)->HasChild("children")) {
			totSize = (*parentObj)->Child("children")->array_value.size();
			
			for (i = 0; i < totSize; i++) {
				
				
				if (i >= curNode->children.size()) {
					
					//curNode->children.push_back(new GameOrgNode())
				}
				
				jsonToNode(
					&( (*parentObj)->Child("children")->array_value[i] ),
					curNode->children[i],
					notThePose
				);
				
				
			}
		}
		
		
		
		
	}
void GameOrg::setBinding (int actorId, bool val)
                                               {
		
		if (actorId < 0) {
			return;
		}
		
		int i;
		for (i = 0; i < RLBN_LENGTH; i++) {
			singleton->gem->bindPose(actorId, i, val);
		}
	}
int GameOrg::getPoseUID ()
                         {
		
		BaseObj* ca;
		
		if (singleton->gem->curActorUID < 0) {
			return -1;
		}
		else {
			ca = &(singleton->gem->gameObjects[singleton->gem->curActorUID]);
			if (ca->entType != E_ENTTYPE_NPC) {
				return -1;
			}
		}
		
		return singleton->gem->curActorUID;
	}
void GameOrg::loadOrgFromFile (string fileName, bool notThePose)
                                                               {
		
		
		int actorId = getPoseUID();
		
		setBinding(actorId,false);
		
		singleton->loadJSON(
			"..\\data\\orgdata\\" + fileName + ".js",
			&rootObj
		);
		
		jsonToNode(&rootObj, baseNode, notThePose);
		
		setBinding(actorId,true);
		
	}
void GameOrg::saveOrgToFile (string fileName)
                                            {
		
		cout << "saveOrgToFile " << fileName << "\n";
		
		int actorId = getPoseUID();
		
		
		setBinding(actorId,false);
		
		if (rootObj != NULL)
		{
			delete rootObj;
			rootObj = NULL;
		}
		
		rootObj = new JSONValue(JSONObject());
		
		nodeToJSON(&rootObj, baseNode); //(rootObj->object_value["rootVal"])
		
		
		singleton->saveFileString(
			"..\\data\\orgdata\\" + fileName + ".js",
			&(rootObj->Stringify())
		);
		
		setBinding(actorId,true);
		
	}
BaseObj * GameOrg::getOwner ()
                            {
		
		if (ownerUID < 0) {
			return NULL;
		}
		
		return &(singleton->gem->gameObjects[ownerUID]);
	}
void GameOrg::setTPG (int _targetPoseGroup, int _targetPoseRLBN)
                                                               {
		
		if (
			(targetPose.group == _targetPoseGroup) &&
			(targetPose.RLBN == _targetPoseRLBN)	
		) {
			// same pose, let it finish
		}
		else {
			
			targetPose.group = _targetPoseGroup;
			targetPose.RLBN = _targetPoseRLBN;
			stepCount = 0;
			
			totTime = 0.0;
		}
		
		
	}
void GameOrg::setToPose (GameOrg * otherOrg, float lerpAmount, int boneId)
          {
		int i;
		int j;
		
		int begInd;
		int endInd;
		
		if (otherOrg == NULL) {
			cout << "ARGH\n";
			//return;
		}
		
		
		GameOrgNode* sourceNode;
		GameOrgNode* destNode;
		
		if (boneId == -1) {
			begInd = 0;
			endInd = E_BONE_C_END;
		}
		else {
			begInd = boneId;
			endInd = boneId+1;
		}
		
		
		
		for (i = begInd; i < endInd; i++) {
			sourceNode = otherOrg->allNodes[i];
			destNode = allNodes[i];
			
			if (
				(sourceNode != NULL) &&
				(destNode != NULL)	
			) {
				for (j = 0; j < E_OV_LENGTH; j++) {
					
					if (j == E_OV_MATPARAMS) {
						destNode->orgVecs[j].copyFrom(&(sourceNode->orgVecs[j]));
					}
					else {
						destNode->orgVecs[j].lerpXYZW(&(sourceNode->orgVecs[j]), lerpAmount);
					}
					
					
				}
			}
		}
	}
void GameOrg::updatePose (double curTimeStep)
                                            {
		totTime += curTimeStep;
		
		float timeInterval = 1.0f;
		float lerpSpeed = 0.005f;
		
		int actionToStop;
		
		float* curData;
		
		float curVelXY;
		
		BaseObj* curOwner = getOwner();
		
		if (singleton->gem->editPose) {
			
		}
		else {
			if (targetPose.group > -1) {
				
				curData = &(singleton->gem->gamePoseInfo[targetPose.group].data[0]);
				
				
				lerpSpeed = curData[E_PIK_LERPSPEED]*singleton->conVals[E_CONST_ANIMLERP_MULT];
				timeInterval = curData[E_PIK_TIMEINTERVAL]*singleton->conVals[E_CONST_TIMEINTERVAL_MULT];
				
				if (targetPose.group == E_PG_WALKFORWARD) {
					curVelXY = curOwner->getPlanarVel();
					
					if (curOwner->getActionState(E_ACT_ISWALKING,RLBN_NEIT)) {
						curVelXY = max(curVelXY,singleton->conVals[E_CONST_MIN_WALK_ANIM_VEL]);
					}
					
					if (curOwner->airCount > singleton->conVals[E_CONST_AIRANIM_THRESH]) {
						curVelXY = 0.0f;
					}
					
					lerpSpeed *= (curVelXY*singleton->conVals[E_CONST_WALKANIM_LERP_MOD]);
					
					if (curVelXY > 0.0) {
						timeInterval /= (curVelXY*singleton->conVals[E_CONST_WALKANIM_INTERVAL_MOD]);
					}
					
				}
				
				targetPose.step = stepCount;
				
				if (curData[E_PIK_DOLOOP] == 1.0f) {
					targetPose.step = stepCount%((int)(curData[E_PIK_NUMSTEPS]));
				}
				else {
					if (targetPose.step >= curData[E_PIK_NUMSTEPS]) {
						targetPose.step = curData[E_PIK_NUMSTEPS]-1;
					}
					
					
					
					if (stepCount > (curData[E_PIK_NUMSTEPS] + curData[E_PIK_EXTRASTEPS])) {
						
						actionToStop = singleton->gem->getActionStateFromPose(targetPose.group);
						
						curOwner->setActionState(
							actionToStop,
							targetPose.RLBN,
							false
						);
						if (actionToStop == E_ACT_ISSWINGING) {
							curOwner->setActionState(
								E_ACT_HASNOTHIT,
								targetPose.RLBN,
								false
							);
						}
						
						if (curOwner->isDead()) {
							
						}
						else {
							setTPG(E_PG_IDLE,RLBN_NEIT);
							targetPose.step = 0;
						}
						
						
					}
				}
				
				if (totTime > timeInterval) {
					totTime -= timeInterval;
					stepCount++;					
				}
				
				setToPose(
					
					singleton->gem->getPose(
						targetPose.group,targetPose.RLBN,targetPose.step
					),
					
					lerpSpeed
				);
			}
		}
		
		
		
		
		singleton->gem->transformOrg(this, NULL);
		
	}
void GameOrg::nodeToJSON (JSONValue * * parentObj, GameOrgNode * curNode)
                                                                     {
		
		int i;
		int j;
		
		float mult = 1.0f;
		
		JSONValue* tempVal;
		
		(*parentObj)->object_value["id"] = new JSONValue((double)(curNode->nodeName) );
		(*parentObj)->object_value["name"] = new JSONValue(boneStrings[curNode->nodeName]);
		
		(*parentObj)->object_value["orgVecs"] = new JSONValue(JSONArray());
		
		
		for (i = 0; i < E_OV_LENGTH; i++) {
			
			mult = 1.0f;
			
			switch (i) {
				//case E_OV_TANGENT:
				//case E_OV_BITANGENT:
				//case E_OV_NORMAL:
				case E_OV_TBNRAD0:
				case E_OV_TBNRAD1:
				//case E_OV_THETAPHIRHO:
				//case E_OV_TPRORIG:
				//case E_OV_MATPARAMS:
				case E_OV_TBNOFFSET:
					mult = ORG_SCALE_DELTA;
				break;
			}
			
			
			for (j = 0; j < 4; j++) {
				
				if (j == 3) {
					mult = 1.0f;
				}
				
				(*parentObj)->object_value["orgVecs"]->array_value.push_back(new JSONValue(
					(double)(curNode->orgVecs[i][j]*mult)	
				));
				
			}
			
		}
		
		if (curNode->children.size() > 0) {
			
			(*parentObj)->object_value["children"] = new JSONValue(JSONArray());
			for (i = 0; i < curNode->children.size(); i++) {
				(*parentObj)->object_value["children"]->array_value.push_back(new JSONValue(JSONObject()));
				
				nodeToJSON(
					&((*parentObj)->object_value["children"]->array_value.back()),
					curNode->children[i]
				);
				
				
			}
			
			
		}
		
	}
void GameOrg::updateHandleOffset ()
                                  {
		
		if (entType != E_ENTTYPE_WEAPON) {
			return;
		}
		
		
		allNodes[E_BONE_C_BASE]->orgVecs[E_OV_TBNOFFSET].setFXYZ(
			-(
					allNodes[E_BONE_WEAPON_POMMEL]->orgVecs[E_OV_TBNRAD0].getFX() +
					allNodes[E_BONE_WEAPON_POMMEL]->orgVecs[E_OV_TBNRAD1].getFX() +
					allNodes[E_BONE_WEAPON_HANDLE]->orgVecs[E_OV_TBNRAD0].getFX()
			),
			0.0f,
			0.0f
		);
		
	}
void GameOrg::initWeapon ()
                          {
		
		int i;
		int j;
		int curName;
		
		float dirMod = 1.0f;
		
		
		baseNode = allNodes[E_BONE_C_BASE] = new GameOrgNode(
			NULL,
			E_BONE_C_BASE,
			
			baseMat, 0.0f, 0.0f, M_PI/2.0f, 
			0.01f, defVecLength, defVecLength,
			0.01f, defVecLength, defVecLength,
			
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
		
		GameOrgNode* curNode = baseNode;
		
		GameOrgNode* centerNode;
		
		
		
		
		wepLengths[E_BONE_WEAPON_POMMEL] = 0.125f*ORG_SCALE_BASE;
		wepLengths[E_BONE_WEAPON_HANDLE] = 0.3f*ORG_SCALE_BASE;
		wepLengths[E_BONE_WEAPON_CENTER] = 0.125f*ORG_SCALE_BASE;
		wepLengths[E_BONE_WEAPON_CROSSR] = 0.5f*ORG_SCALE_BASE;
		wepLengths[E_BONE_WEAPON_BLADER] = 0.5f*ORG_SCALE_BASE;
		wepLengths[E_BONE_WEAPON_CROSSL] = 0.5f*ORG_SCALE_BASE;
		wepLengths[E_BONE_WEAPON_BLADEL] = 0.5f*ORG_SCALE_BASE;
		wepLengths[E_BONE_WEAPON_BLADEU] = 1.0f*ORG_SCALE_BASE;
		
		
		
		
		curName = E_BONE_WEAPON_POMMEL;
		curNode = allNodes[curName] = curNode->addChild( curName,
			baseMat, 0.0f, 0.0f, 0.0f,
			wepLengths[curName], defVecLength, defVecLength,
			wepLengths[curName], defVecLength, defVecLength,
			1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f
		);
		
		curName = E_BONE_WEAPON_HANDLE;
		curNode = allNodes[curName] = curNode->addChild( curName,
			baseMat, 0.0f, 0.0f, 0.0f,
			wepLengths[curName], defVecLength, defVecLength,
			wepLengths[curName], defVecLength, defVecLength,
			1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f
		);
		
		curName = E_BONE_WEAPON_CENTER;
		centerNode = curNode = allNodes[curName] = curNode->addChild( curName,
			baseMat, 0.0f, 0.0f, 0.0f,
			0.125f, defVecLength, defVecLength,
			0.125f, defVecLength, defVecLength,
			1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f
		);
		
		curName = E_BONE_WEAPON_CROSSR;
		curNode = allNodes[curName] = centerNode->addChild( curName,
			baseMat, 0.0f, 0.0f, M_PI/2.0f,
			wepLengths[curName], defVecLength, defVecLength,
			wepLengths[curName], defVecLength, defVecLength,
			1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f
		);
		
		curName = E_BONE_WEAPON_BLADER;
		curNode = allNodes[curName] = curNode->addChild( curName,
			baseMat, 0.0f, 0.0f, 0.0f,
			wepLengths[curName], defVecLength, defVecLength,
			wepLengths[curName], defVecLength, defVecLength,
			1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f
		);
		
		curName = E_BONE_WEAPON_CROSSL;
		curNode = allNodes[curName] = centerNode->addChild( curName,
			baseMat, 0.0f, 0.0f, -M_PI/2.0f,
			wepLengths[curName], defVecLength, defVecLength,
			wepLengths[curName], defVecLength, defVecLength,
			1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f
		);
		
		curName = E_BONE_WEAPON_BLADEL;
		curNode = allNodes[curName] = curNode->addChild( curName,
			baseMat, 0.0f, 0.0f, 0.0f,
			wepLengths[curName], defVecLength, defVecLength,
			wepLengths[curName], defVecLength, defVecLength,
			1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f
		);
		
		curName = E_BONE_WEAPON_BLADEU;
		curNode = allNodes[curName] = centerNode->addChild( curName,
			baseMat, 0.0f, 0.0f, 0.0f,
			wepLengths[curName], defVecLength, defVecLength,
			wepLengths[curName], defVecLength, defVecLength,
			1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f
		);
		
		
		
		baseNode->doTransform(singleton, NULL);
		
	}
void GameOrg::initHuman ()
                         {
		
		int i;
		int j;
		int lrMod;
		
		float dirMod = 1.0f;
		
		
		baseNode = allNodes[E_BONE_C_BASE] = new GameOrgNode(
			NULL,
			E_BONE_C_BASE,
			
			baseMat, 0.0f, 0.0f, 0.0f,
			0.01f, defVecLength, defVecLength,
			0.01f, defVecLength, defVecLength,
			
			0.0f, 1.0f, 0.0f,			
			1.0f, 0.0f, 0.0f,
			0.0f,0.0f,1.0f
		);
		
		GameOrgNode* curNode = baseNode;
		

		float numSpineSegs = E_BONE_C_SKULL-E_BONE_C_SPINE0;
		
		for (i = E_BONE_C_SPINE0; i < E_BONE_C_SKULL; i++) {
			curNode = allNodes[i] = curNode->addChild(
				i,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.75f*ORG_SCALE_BASE/numSpineSegs, defVecLength, defVecLength,
				0.75f*ORG_SCALE_BASE/numSpineSegs, defVecLength, defVecLength,
				
				0.0f,0.0f,1.0f,
				0.0f,1.0f,0.0f,
				1.0f,0.0f,0.0f
				
			);
		}
		
		curNode = allNodes[E_BONE_C_SKULL] = curNode->addChild(
			E_BONE_C_SKULL,
			
			baseMat, 0.0f, 0.0f, 0.0f,
			0.25f*ORG_SCALE_BASE,  defVecLength, defVecLength,
			0.25f*ORG_SCALE_BASE,  defVecLength, defVecLength,
			
			0.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,
			1.0f,0.0f,0.0f
		);
		
		
		for (j = 0; j < 2; j++) {
			
			if (j == 0) { // left limbs
				lrMod = 0;
				dirMod = 1.0f;
			}
			else { // right limbs
				lrMod = E_BONE_R_BEG - E_BONE_L_BEG;
				dirMod = -1.0f;
			}
			
			curNode = baseNode->getNode(E_BONE_C_SKULL-2);
			
			
			curNode = allNodes[E_BONE_L_SHOULDER + lrMod] = curNode->addChild(
				E_BONE_L_SHOULDER + lrMod,

				baseMat, 0.0f, 0.0f, 0.0f,
				0.20f*ORG_SCALE_BASE,  defVecLength, defVecLength,
				0.20f*ORG_SCALE_BASE,  defVecLength, defVecLength,
				
				dirMod*1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			curNode = allNodes[E_BONE_L_UPPERARM + lrMod] = curNode->addChild(
				E_BONE_L_UPPERARM + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.25f*ORG_SCALE_BASE, defVecLength, defVecLength,
				0.25f*ORG_SCALE_BASE, defVecLength, defVecLength,
				
				dirMod*1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			curNode = allNodes[E_BONE_L_LOWERARM + lrMod] = curNode->addChild(
				E_BONE_L_LOWERARM + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.25f*ORG_SCALE_BASE, defVecLength, defVecLength,
				0.25f*ORG_SCALE_BASE, defVecLength, defVecLength,
				
				dirMod*1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			curNode = allNodes[E_BONE_L_METACARPALS + lrMod] = curNode->addChild(
				E_BONE_L_METACARPALS + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.1f*ORG_SCALE_BASE, defVecLength, defVecLength,
				0.1f*ORG_SCALE_BASE, defVecLength, defVecLength,
				
				dirMod*1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			
			
			curNode = baseNode;
			
			curNode = allNodes[E_BONE_L_HIP + lrMod] = curNode->addChild(
				E_BONE_L_HIP + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.1f*ORG_SCALE_BASE, defVecLength, defVecLength,
				0.1f*ORG_SCALE_BASE, defVecLength, defVecLength,
				
				dirMod*1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			curNode = allNodes[E_BONE_L_UPPERLEG + lrMod] = curNode->addChild(
				E_BONE_L_UPPERLEG + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.45f*ORG_SCALE_BASE, defVecLength, defVecLength,
				0.45f*ORG_SCALE_BASE, defVecLength, defVecLength,
				
				0.0f,0.0f,-1.0f,
				0.0f,1.0f,0.0f,
				dirMod*1.0f,0.0f,0.0f
			);
			curNode = allNodes[E_BONE_L_LOWERLEG + lrMod] = curNode->addChild(
				E_BONE_L_LOWERLEG + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.45f*ORG_SCALE_BASE, defVecLength, defVecLength,
				0.45f*ORG_SCALE_BASE, defVecLength, defVecLength,
				
				0.0f,0.0f,-1.0f,
				0.0f,1.0f,0.0f,
				dirMod*1.0f,0.0f,0.0f
			);
			curNode = allNodes[E_BONE_L_TALUS + lrMod] = curNode->addChild(
				E_BONE_L_TALUS + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.2f*ORG_SCALE_BASE, defVecLength, defVecLength,
				0.2f*ORG_SCALE_BASE, defVecLength, defVecLength,
				
				0.0f,1.0f,0.0f,
				dirMod*1.0f,0.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			
		}
		
		baseNode->doTransform(singleton, NULL);
		
		
	}
#undef LZZ_INLINE
 
