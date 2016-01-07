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
		basePoseGroup = -1;
		basePoseRLBN = -1;
		basePoseStep = -1;
		targetPoseRLBN = RLBN_NEIT;
		targetPoseGroup = -1;
		rootObj = NULL;
		defVecLength = 0.05f;
	}
void GameOrg::init (Singleton * _singleton, int _ownerUID, int _orgType)
          {
		singleton = _singleton;

		stepCount = 0;

		ownerUID = _ownerUID;

		orgType = _orgType;

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
		
		switch (orgType) {
			case E_ORGTYPE_HUMAN:
				initHuman();
			break;
			case E_ORGTYPE_WEAPON:
				initWeapon();
			break;
		}
		
		singleton->curOrgId++;
		
		
	}
void GameOrg::loadFromFile (string fileName, bool notThePose)
                                                            {
		singleton->loadJSON(
			"..\\data\\orgdata\\" + fileName + ".js",
			&rootObj
		);
		
		jsonToNode(&rootObj, baseNode, notThePose);
		
	}
void GameOrg::jsonToNode (JSONValue * * parentObj, GameOrgNode * curNode, bool notThePose)
                                                                                      {
		
		int i;
		
		
		curNode->nodeName = (*parentObj)->Child("id")->number_value;
		
		JSONValue* tempVal;
		
		bool doProc;
		
		for (i = 0; i < E_OV_LENGTH; i++) {	
		
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
				tempVal = (*parentObj)->Child("orgVecs");
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
void GameOrg::saveToFile (string fileName)
                                         { //
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
		
		
	}
BaseObj * GameOrg::getOwner ()
                            {
		
		if (ownerUID < 0) {
			return NULL;
		}
		
		return &(singleton->gw->gameObjects[ownerUID]);
	}
void GameOrg::setTPG (int _targetPoseGroup, int _targetPoseRLBN)
                                                               {
		
		if (
			(targetPoseGroup == _targetPoseGroup) &&
			(targetPoseRLBN == _targetPoseRLBN)	
		) {
			// same pose, let it finish
		}
		else {
			
			//cout << "setTPG()" << targetPoseGroup << " " << targetPoseRLBN << "\n";
			
			targetPoseGroup = _targetPoseGroup;
			targetPoseRLBN = _targetPoseRLBN;
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
		
		int stepCountMod;
		
		float* curData;
		
		BaseObj* curOwner = getOwner();
		
		if (singleton->editPose) {
			
		}
		else {
			if (targetPoseGroup > -1) {
				
				curData = &(singleton->gamePoseInfo[targetPoseGroup].data[0]);
				
				
				lerpSpeed = curData[E_PIK_LERPSPEED];
				timeInterval = curData[E_PIK_TIMEINTERVAL];
				
				stepCountMod = stepCount;
				
				if (curData[E_PIK_DOLOOP] == 1.0f) {
					stepCountMod = stepCount%((int)(curData[E_PIK_NUMSTEPS]));
				}
				else {
					if (stepCountMod >= curData[E_PIK_NUMSTEPS]) {
						stepCountMod = curData[E_PIK_NUMSTEPS]-1;
					}
					
					if (stepCount > (curData[E_PIK_NUMSTEPS] + curData[E_PIK_EXTRASTEPS])) {
						curOwner->setActionState(
							singleton->getActionStateFromPose(targetPoseGroup),
							targetPoseRLBN,
							false
						);
						setTPG(E_PG_IDLE,RLBN_NEIT);
						stepCountMod = 0;
					}
				}
				
				if (totTime > timeInterval) {
					totTime -= timeInterval;
					stepCount++;					
				}
				
				setToPose(
					
					singleton->getPose(
						targetPoseGroup,targetPoseRLBN,stepCountMod
					),
					
					lerpSpeed
				);
			}
		}
		
		
		
		
		singleton->transformOrg(this, NULL);
		
	}
void GameOrg::nodeToJSON (JSONValue * * parentObj, GameOrgNode * curNode)
                                                                     {
		
		int i;
		int j;
		
		
		JSONValue* tempVal;
		
		(*parentObj)->object_value["id"] = new JSONValue((double)(curNode->nodeName) );
		(*parentObj)->object_value["name"] = new JSONValue(boneStrings[curNode->nodeName]);
		
		(*parentObj)->object_value["orgVecs"] = new JSONValue(JSONArray());
		
		
		for (i = 0; i < E_OV_LENGTH; i++) {
			
			for (j = 0; j < 4; j++) {
				(*parentObj)->object_value["orgVecs"]->array_value.push_back(new JSONValue(
					(double)(curNode->orgVecs[i][j])	
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
void GameOrg::initWeapon ()
                          {
		
		int i;
		int j;
		int lrMod;
		
		float dirMod = 1.0f;
		
		
		baseNode = allNodes[E_BONE_WEAPON_BASE] = new GameOrgNode(
			NULL,
			E_BONE_WEAPON_BASE,
			
			baseMat, 0.0f, 0.0f, M_PI/2.0f, 
			0.01f, defVecLength, defVecLength,
			0.01f, defVecLength, defVecLength,
			
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
		
		GameOrgNode* curNode = baseNode;
		
		curNode = allNodes[E_BONE_WEAPON_HANDLE] = curNode->addChild(
			E_BONE_WEAPON_HANDLE,
			
			baseMat, 0.0f, 0.0f, 0.0f,
			1.0f, defVecLength, defVecLength,
			1.0f, defVecLength, defVecLength,
			
			// 0.0f,0.0f,1.0f,
			// 0.0f,1.0f,0.0f,
			// 1.0f,0.0f,0.0f
			
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
		
		allNodes[E_BONE_WEAPON_0] = curNode->addChild(
			E_BONE_WEAPON_0,
			
			baseMat, 0.0f, 0.0f, 0.0f,
			0.25f, defVecLength, defVecLength,
			0.25f, defVecLength, defVecLength,
			
			// 0.0f,0.0f,1.0f,
			// 0.0f,1.0f,0.0f,
			// 1.0f,0.0f,0.0f
			
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
		
		allNodes[E_BONE_WEAPON_1] = curNode->addChild(
			E_BONE_WEAPON_1,
			
			baseMat, 0.0f, 0.0f, 0.0f,
			0.25f, defVecLength, defVecLength,
			0.25f, defVecLength, defVecLength,
			
			// 0.0f,0.0f,1.0f,
			// 0.0f,1.0f,0.0f,
			// 1.0f,0.0f,0.0f
			
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
		
		
		// for (i = E_BONE_WEAPON_0; i <= E_BONE_WEAPON_8; i++ ) {
		// 	curNode = allNodes[i] = curNode->addChild(
		// 		i,
				
		// 		baseMat, 0.0f, 0.0f, 0.0f,
		// 		0.25f, defVecLength, defVecLength,
		// 		0.25f, defVecLength, defVecLength,
				
		// 		0.0f,0.0f,1.0f,
		// 		0.0f,1.0f,0.0f,
		// 		1.0f,0.0f,0.0f
		// 	);
		// }
		
		
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
				0.75f/numSpineSegs, defVecLength, defVecLength,
				0.75f/numSpineSegs, defVecLength, defVecLength,
				
				0.0f,0.0f,1.0f,
				0.0f,1.0f,0.0f,
				1.0f,0.0f,0.0f
				
			);
		}
		
		curNode = allNodes[E_BONE_C_SKULL] = curNode->addChild(
			E_BONE_C_SKULL,
			
			baseMat, 0.0f, 0.0f, 0.0f,
			0.25f,  defVecLength, defVecLength,
			0.25f,  defVecLength, defVecLength,
			
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
				0.20f,  defVecLength, defVecLength,
				0.20f,  defVecLength, defVecLength,
				
				dirMod*1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			curNode = allNodes[E_BONE_L_UPPERARM + lrMod] = curNode->addChild(
				E_BONE_L_UPPERARM + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.25f, defVecLength, defVecLength,
				0.25f, defVecLength, defVecLength,
				
				dirMod*1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			curNode = allNodes[E_BONE_L_LOWERARM + lrMod] = curNode->addChild(
				E_BONE_L_LOWERARM + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.25f, defVecLength, defVecLength,
				0.25f, defVecLength, defVecLength,
				
				dirMod*1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			curNode = allNodes[E_BONE_L_METACARPALS + lrMod] = curNode->addChild(
				E_BONE_L_METACARPALS + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.1f, defVecLength, defVecLength,
				0.1f, defVecLength, defVecLength,
				
				dirMod*1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			
			
			curNode = baseNode;
			
			curNode = allNodes[E_BONE_L_HIP + lrMod] = curNode->addChild(
				E_BONE_L_HIP + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.1f, defVecLength, defVecLength,
				0.1f, defVecLength, defVecLength,
				
				dirMod*1.0f,0.0f,0.0f,
				0.0f,1.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			curNode = allNodes[E_BONE_L_UPPERLEG + lrMod] = curNode->addChild(
				E_BONE_L_UPPERLEG + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.45f, defVecLength, defVecLength,
				0.45f, defVecLength, defVecLength,
				
				0.0f,0.0f,-1.0f,
				0.0f,1.0f,0.0f,
				dirMod*1.0f,0.0f,0.0f
			);
			curNode = allNodes[E_BONE_L_LOWERLEG + lrMod] = curNode->addChild(
				E_BONE_L_LOWERLEG + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.45f, defVecLength, defVecLength,
				0.45f, defVecLength, defVecLength,
				
				0.0f,0.0f,-1.0f,
				0.0f,1.0f,0.0f,
				dirMod*1.0f,0.0f,0.0f
			);
			curNode = allNodes[E_BONE_L_TALUS + lrMod] = curNode->addChild(
				E_BONE_L_TALUS + lrMod,
				
				baseMat, 0.0f, 0.0f, 0.0f,
				0.2f, defVecLength, defVecLength,
				0.2f, defVecLength, defVecLength,
				
				0.0f,1.0f,0.0f,
				dirMod*1.0f,0.0f,0.0f,
				0.0f,0.0f,1.0f
			);
			
		}
		
		baseNode->doTransform(singleton, NULL);
		
		
	}
#undef LZZ_INLINE
 
