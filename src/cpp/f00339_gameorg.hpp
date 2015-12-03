
class GameOrg {

public:

	Singleton* singleton;
	GameOrgNode* baseNode;
	
	GameOrgNode* allNodes[E_BONE_C_END];
	
	FIVector4 basePosition;
	
	JSONValue *rootObj;
	
	int ownerUID;
	
	int stepCount;
	int targetPose;
	int targetPoseGroup;
	
	double totTime;
	float defVecLength;

	float gv(float* vals) {
		float lerp = fGenRand();
		return vals[0]*lerp + vals[1]*(1.0f-lerp);
	}
	
	const static float baseMat = 12.0f;
	

	GameOrg() {
		
		targetPoseGroup = -1;
		targetPose = -1;
		rootObj = NULL;
		defVecLength = 0.05f;
	}


	void init(
		Singleton* _singleton,
		int _ownerUID
	) {
		singleton = _singleton;

		stepCount = 0;

		ownerUID = _ownerUID;

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
		
		

		baseNode = new GameOrgNode(
			NULL,
			E_BONE_C_BASE,
			
			baseMat, 0.0f, 0.0f, 0.0f,
			0.01f, defVecLength, defVecLength,
			0.01f, defVecLength, defVecLength,
			
			0.0f, 1.0f, 0.0f,			
			1.0f, 0.0f, 0.0f,
			0.0f,0.0f,1.0f
		);
		
		initHuman();
		
		
		singleton->curOrgId++;
		
		
	}
	
	
	
	
	void loadFromFile(string fileName) {
		singleton->loadJSON(
			"..\\data\\orgdata\\" + fileName + ".js",
			&rootObj
		);
		
		jsonToNode(&rootObj, baseNode);
		
	}
	
	void jsonToNode(JSONValue** parentObj, GameOrgNode* curNode) {
		
		int i;
		
		
		curNode->nodeName = (*parentObj)->Child("id")->number_value;
		
		JSONValue* tempVal;
		
		
		for (i = 0; i < E_OV_LENGTH; i++) {	
		
			tempVal = (*parentObj)->Child("orgVecs");
			curNode->orgVecs[i].setFXYZW(
				tempVal->array_value[i*4 + 0]->number_value,
				tempVal->array_value[i*4 + 1]->number_value,
				tempVal->array_value[i*4 + 2]->number_value,
				tempVal->array_value[i*4 + 3]->number_value
			);
			
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
					curNode->children[i]
				);
				
				
			}
		}
		
		
		
		
	}
	
	
	
	void saveToFile(string fileName) { //
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
	
	BaseObj* getOwner() {
		
		if (ownerUID < 0) {
			return NULL;
		}
		
		return &(singleton->gw->gameObjects[ownerUID]);
	}
	
	void setToPose(GameOrg* otherOrg, float lerpAmount, int boneId = -1) {
		int i;
		int j;
		
		int begInd;
		int endInd;
		
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
					destNode->orgVecs[j].lerpXYZW(&(sourceNode->orgVecs[j]), lerpAmount);
				}
			}
		}
	}
	
	void updatePose(double curTimeStep) {
		totTime += curTimeStep;
		
		float timeInterval = 1.0f;
		float lerpSpeed = 0.005f;
		
		BaseObj* curOwner = getOwner();
		
		if (singleton->editPose) {
			
		}
		else {
			if (targetPoseGroup > -1) {
				
				
				
				switch (targetPoseGroup) {
					case E_PG_IDLE:
						lerpSpeed = 0.003f;
						targetPose = E_PK_IDLE_LOW + (stepCount%2);
						timeInterval = 1.0;
					break;
					case E_PG_WALK:
						lerpSpeed = 0.005f;
						timeInterval = 0.5;
						targetPose = E_PK_L_FORWARD + (stepCount%4);
					break;
					case E_PG_JUMP:
						lerpSpeed = 0.005f;
						timeInterval = 1.5;
						targetPose = E_PK_JUMP;
					break;
				}
				
				if (totTime > timeInterval) {
					totTime -= timeInterval;
					stepCount++;
					
					switch (targetPoseGroup) {
						case E_PG_JUMP:
							if (curOwner != NULL) {
								curOwner->isJumping = false;
							}
							
						break;
					}
					
				}
				
				setToPose(singleton->gamePoses[targetPose],lerpSpeed);
			}
		}
		
		
		
		
		
		
		// float angMod1 = sin(totTime)*0.5f;
		// float angMod2 = sin(totTime)*0.5f;
		// float angMod3 = -(sin(totTime*2.0)+1.0f)*0.5f;
		// float angMod4 = (sin(totTime*2.0)+1.0f)*0.5f;
		
		// GameOrgNode* curNode;
		
		// curNode = allNodes[E_BONE_L_UPPERLEG];
		// curNode->orgVecs[E_OV_THETAPHIRHO].copyFrom(&(
		// 	curNode->orgVecs[E_OV_TPRORIG]
		// ));
		// curNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(0.0,0.0,angMod1);
		
		// curNode = allNodes[E_BONE_R_UPPERLEG];
		// curNode->orgVecs[E_OV_THETAPHIRHO].copyFrom(&(
		// 	curNode->orgVecs[E_OV_TPRORIG]
		// ));
		// curNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(0.0,0.0,angMod2);
		
		
		// curNode = allNodes[E_BONE_L_LOWERLEG];
		// curNode->orgVecs[E_OV_THETAPHIRHO].copyFrom(&(
		// 	curNode->orgVecs[E_OV_TPRORIG]
		// ));
		// curNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(0.0,0.0,angMod3);
		
		// curNode = allNodes[E_BONE_R_LOWERLEG];
		// curNode->orgVecs[E_OV_THETAPHIRHO].copyFrom(&(
		// 	curNode->orgVecs[E_OV_TPRORIG]
		// ));
		// curNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(0.0,0.0,angMod4);
		
		
		
		
		
		singleton->transformOrg(this);
		
	}
	
	void nodeToJSON(JSONValue** parentObj, GameOrgNode* curNode) {
		
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
	
	
	void initHuman() {
		
		int i;
		int j;
		int lrMod;
		
		float dirMod = 1.0f;
		
		GameOrgNode* curNode;
		curNode = baseNode;
		
		
		for (i = 0; i < E_BONE_C_END; i++) {
			allNodes[i] = NULL;
		}
		

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
		
		baseNode->doTransform(singleton);
		
		
	}
	
	

};
