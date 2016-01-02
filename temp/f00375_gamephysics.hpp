
class GamePhysics {
public:
	Singleton* singleton;
	
	BenchmarkDemo* example;
	MyOGLApp* myOGLApp;
	GUIHelperInterface* guiHelper;
	
	float myMat[16];
	float BASE_ENT_HEIGHT;
	float BASE_ENT_RAD;
	float CONTACT_THRESH;
	double totTime;
	
	Matrix4 myMatrix4;
	Vector4 myVector4;
	Vector4 resVector4;
	
	//GameActor* gameActor;
	btRigidBody* lastBodyPick;
	int lastBodyUID;
	
	//unsigned long int stepTimeInMicroSec;
	
	GamePhysics() {
		lastBodyPick = NULL;
		//gameActor = NULL;
		//8000; // ~120 times per second
		
		lastBodyUID = -1;
	}
	
	void init(Singleton* _singleton)
	{
		CONTACT_THRESH = 0.2f;
		totTime = 0.0;
		BASE_ENT_HEIGHT = 2.0f;
		BASE_ENT_RAD = 1.0f;
		
		cout << "GamePhysics:init()\n";
		
		singleton = _singleton;
		myOGLApp = new MyOGLApp("yo", 640, 480);
		guiHelper = new MyGLHelper(singleton, myOGLApp);
		example = new BenchmarkDemo(guiHelper,5);
		example->initPhysics();
		
		
		example->getWorld()->getPairCache()->setOverlapFilterCallback(new CustFilterCallback());
		
		
	}
	
	void pickBody(FIVector4* mouseMoveOPD) { //btVector3 posWS1, btVector3 posWS2) {
		
		if (!(singleton->editPose)) {
			lastBodyPick = NULL;
			lastBodyUID = -1;
			return;
		}
		
		int bodyUID = mouseMoveOPD->getFW();
		int limbUID = mouseMoveOPD->getFZ();
		BaseObj* ge;
		
		if (
			(bodyUID > 0) &&
			(limbUID > -1)	
		) {
			ge = &(singleton->gw->gameObjects[bodyUID]);
			lastBodyPick = ge->bodies[limbUID].body;
			lastBodyUID = bodyUID;
		}
		else {
			lastBodyPick = NULL;
			lastBodyUID = -1;
		}
		
		
		// btVector3 begPos = btVector3(0.0f,0.0f,0.0f);
		// btVector3 endPos = btVector3(0.0f,0.0f,0.0f);
		// btVector3 rayDir = btVector3(0.0f,0.0f,0.0f);
		
		// singleton->getRay(
		// 	singleton->lastMouseX,
		// 	singleton->lastMouseY,
		// 	begPos,
		// 	endPos,
		// 	rayDir
		// );
		
		
		// btVector3 begPos = singleton->cameraGetPosNoShake()->getBTV();
		// btVector3 endPos;
		
		// if (posWS1.distance(begPos) < posWS2.distance(begPos)) {
		// 	endPos = posWS1;
		// }
		// else {
		// 	endPos = posWS2;
		// }
		
		// lastBodyPick = example->bodyPick(begPos,endPos);
		
		
		// singleton->getRayTo(
		// 	singleton->lastMouseX,
		// 	singleton->lastMouseY
		// );
		
		
		
		// if (lastBodyPick != NULL) {
		// 	cout << "objID " << lastBodyPick->bodyUID << "\n";
		// 	cout << "limbUID " << lastBodyPick->limbUID << "\n\n";
		// }
		
		
	}
	
	
	void collectDebris() {
		int i;
		FIVector4 tempVec;
		int entNum;
		
		for (i = 0; i < singleton->debrisStack.size(); i++) {
			
			tempVec.setBTV(singleton->debrisStack[i].pos);
			entNum = singleton->placeNewEnt(false, E_ENTTYPE_DEBRIS, &tempVec);
			
			//addDebris(singleton->debrisStack[i].pos);
		}
		singleton->debrisStack.clear();
	}
	
	void beginDrop() {
		
		cout << "GamePhysics:beginDrop()\n";
		
		example->beginDrop(
			singleton->cameraGetPosNoShake()->getFX(),
			singleton->cameraGetPosNoShake()->getFY(),
			singleton->cameraGetPosNoShake()->getFZ()
		);
	}
	
	
	void remBoxFromObj(BaseObjType _uid) {
		
		BaseObj* ge = &(singleton->gw->gameObjects[_uid]);
		
		int bodInd;
		for (bodInd = 0; bodInd < ge->bodies.size(); bodInd++) {
			example->removeRigidBody(ge->bodies[bodInd].body);
		}
		ge->bodies.clear();
	}
	
	void addBoxFromObj(BaseObjType _uid) {
		
		//cout << "\n\nADD BOX\n\n";
		
		int i;
		int curOrgType = -1;
		GameOrg* curOrg = NULL;
		
		BaseObj* ge = &(singleton->gw->gameObjects[_uid]);
		
		if (ge->isHidden) {
			return;
		}
		
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(ge->startPoint);
		
		GameActor* curActor;
		
		float objRad = 0.25f;
		bool isOrg = false;
		
		switch (ge->entType) {
			case E_ENTTYPE_NPC:
			// case E_ENTTYPE_MONSTER:
			case E_ENTTYPE_WEAPON:
				{
					
					isOrg = true;
					
					
					if (ge->entType == E_ENTTYPE_WEAPON) {
						curOrgType = E_ORGTYPE_WEAPON;
					}
					else {
						curOrgType = E_ORGTYPE_HUMAN;
					}
					
					////////////////////////////
					
					
					for (i = 0; i < E_BDG_LENGTH; i++) {
						//btCapsuleShapeZ* capsuleShapeZ = new btCapsuleShapeZ(1.0f,BASE_ENT_HEIGHT);
						
						switch (i) {
							case E_BDG_CENTER:
								trans.setIdentity();
								trans.setOrigin(ge->startPoint);
							break;
							// case E_BDG_LFOOT:
							// 	trans.setIdentity();
							// 	trans.setOrigin(ge->startPoint + btVector3(-1.0f, 0.0f, -1.0f));
							// break;
							// case E_BDG_RFOOT:
							// 	trans.setIdentity();
							// 	trans.setOrigin(ge->startPoint + btVector3( 1.0f, 0.0f, -1.0f));
							// break;
						}
						
						ge->bodies.push_back(BodyStruct());
						
						if (i == E_BDG_CENTER) {
							ge->bodies.back().body = example->createRigidBodyMask(
								MASS_PER_LIMB, // 0.1
								trans,
								new btCapsuleShapeZ(BASE_ENT_RAD,BASE_ENT_HEIGHT),//capsuleShapeZ, //btSphereShape(BASE_ENT_HEIGHT),//
								COL_MARKER,
								markerCollidesWith
							);
						}
						else {
							// ge->bodies.back().body = example->createRigidBodyMask(
							// 	MASS_PER_LIMB, // 0.1
							// 	trans,
							// 	new btSphereShape(0.25f),//new btCapsuleShapeZ(1.0f,BASE_ENT_HEIGHT),//capsuleShapeZ,
							// 	COL_MARKER,
							// 	markerCollidesWith
							// );
						}
						
						
						ge->bodies.back().body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
						ge->bodies.back().boneId = -1;
						ge->bodies.back().jointType = E_JT_CONT;
					}
					
					
					
					////////////////////////////
					
					
					
					
					singleton->gameOrgs.push_back(new GameOrg());
					singleton->gameOrgs.back()->init(singleton, ge->uid,curOrgType);
					ge->orgId = singleton->gameOrgs.size()-1;
					
					singleton->gameActors.push_back(new GameActor(
						singleton,
						ge->uid,
						example->getWorld(),
						ge->startPoint,
						false	
					));
					
					curActor = (singleton->gameActors.back());
					ge->actorId = singleton->gameActors.size()-1;
					
					for (i = 0; i < curActor->actorJoints.size(); i++) {
						
						
						ge->bodies.push_back(BodyStruct());
						ge->bodies.back().body = curActor->actorJoints[i].body;
						ge->bodies.back().boneId = curActor->actorJoints[i].boneId;
						ge->bodies.back().jointType = curActor->actorJoints[i].jointType;
						ge->bodies.back().rad = curActor->actorJoints[i].rad;
						ge->bodies.back().length = curActor->actorJoints[i].length;
						
						
						// if (i == 0) {
						// 	//ge->body = curActor->actorJoints[i].body;
						// 	//ge->body->setLinearFactor(btVector3(0.0f,0.0f,0.0f));
						// 	ge->bodies.back().body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
						// }
						// else {
							
						// }
					}
				}
			break;
			default:
				{
					btBoxShape* boxShape = new btBoxShape(btVector3(objRad,objRad,objRad));
					ge->bodies.push_back(BodyStruct());
					ge->bodies.back().body = example->createRigidBodyMask(
						MASS_PER_LIMB,
						trans,
						boxShape
						,COL_DYN,
						dynCollidesWith
					);
					ge->bodies.back().boneId = -1;
					
					if (ge->entType == E_ENTTYPE_DEBRIS) {
						
						ge->bodies.back().body->setAngularVelocity(btVector3(
							(fGenRand2()*2.0f-1.0f),
							(fGenRand2()*2.0f-1.0f),
							(fGenRand2()*2.0f-1.0f)	
						)*4.0f);
					}
					
					
				}
			break;
		}
		
		
		int bodInd;
		for (bodInd = 0; bodInd < ge->bodies.size(); bodInd++) {
			ge->bodies[bodInd].body->bodyUID = _uid;
			ge->bodies[bodInd].body->limbUID = bodInd;
			
			
			ge->bodies[bodInd].body->setDamping(0.1f,0.9f);
			/*
			if (ge->bodies[bodInd].boneId > -1) {
				switch(ge->bodies[bodInd].boneId) {
					default:
					
						if (ge->entType == E_ENTTYPE_WEAPON) {
							ge->bodies[bodInd].body->setDamping(0.1f,0.9f);
						}
						else {
							ge->bodies[bodInd].body->setDamping(0.999f,0.9f);
						}
					
						
					break;
				}
			}
			else {
				switch (bodInd) {
					case E_BDG_CENTER:
						ge->bodies[bodInd].body->setDamping(0.1f,0.9f);
						
					break;
					case E_BDG_LFOOT:
						ge->bodies[bodInd].body->setDamping(0.1f,0.9f);
					break;
					case E_BDG_RFOOT:
						ge->bodies[bodInd].body->setDamping(0.1f,0.9f);
					break;
					default:
					
						ge->bodies[bodInd].body->setDamping(0.1f,0.9f);
						
						// switch(ge->bodies[bodInd].boneId) {
							
						// 	case E_BONE_L_UPPERARM:
						// 	case E_BONE_L_LOWERARM:
						// 	case E_BONE_L_METACARPALS:
						// 	case E_BONE_L_UPPERLEG:
						// 	case E_BONE_L_LOWERLEG:
						// 	case E_BONE_L_TALUS:
							
						// 	case E_BONE_R_UPPERARM:
						// 	case E_BONE_R_LOWERARM:
						// 	case E_BONE_R_METACARPALS:
						// 	case E_BONE_R_UPPERLEG:
						// 	case E_BONE_R_LOWERLEG:
						// 	case E_BONE_R_TALUS:
						// 		ge->bodies[bodInd].body->setDamping(0.99f,0.9f);
						// 	break;
						// 	case -1:
						// 	case E_BONE_WEAPON_BASE:
						// 	case E_BONE_WEAPON_END:
						// 	case E_BONE_WEAPON_0:
						// 	case E_BONE_WEAPON_1:
						// 	case E_BONE_WEAPON_2:
						// 	case E_BONE_WEAPON_3:
						// 	case E_BONE_WEAPON_4:
						// 	case E_BONE_WEAPON_5:
						// 	case E_BONE_WEAPON_6:
						// 	case E_BONE_WEAPON_7:
						// 	case E_BONE_WEAPON_8:
						// 		ge->bodies[bodInd].body->setDamping(0.1f,0.9f);
						// 	break;
							
						// 	default:
						// 		// linear, angular
						// 		ge->bodies[bodInd].body->setDamping(0.999f,0.9f);
						// 	break;
						// }
					
						
					
					break;
				}
			}
			*/
			
			
			
			
			
			
			
			ge->bodies[bodInd].body->setContactProcessingThreshold(CONTACT_THRESH);
			
			ge->bodies[bodInd].isVisible = true;
			
			if (isOrg) {
				
				curOrg = singleton->gameOrgs[ge->orgId];
				
				// if (curOrg->orgType == E_ORGTYPE_HUMAN) {
				// 	// if (
				// 	// 	(bodInd == 0)
				// 	// 	|| (
				// 	// 		(ge->bodies[bodInd].boneId == E_BONE_C_BASE) &&
				// 	// 		(!(ge->bodies[bodInd].isBall))	
				// 	// 	)
				// 	// ) {
				// 	// 	ge->bodies[bodInd].isVisible = false;
				// 	// }
					
				// 	ge->bodies[bodInd].isVisible = false;
				// }
				
				// if (curOrg->orgType == E_ORGTYPE_WEAPON) {
				// 	if (ge->bodies[bodInd].boneId == E_BONE_WEAPON_BASE) {
				// 		ge->bodies[bodInd].isVisible = false;
				// 	}
				// }
				
				// if (ge->bodies[bodInd].jointType != E_JT_LIMB) {
				// 	ge->bodies[bodInd].isVisible = false;
				// }
				
				ge->bodies[bodInd].isVisible = false;//(bodInd == 0);// false;//(bodInd < E_BDG_LENGTH);//
				
			}
			
			
			ge->bodies[bodInd].mass = MASS_PER_LIMB;
			ge->bodies[bodInd].hasContact = false;
			ge->bodies[bodInd].isInside = false;
			ge->bodies[bodInd].isFalling = true;
			ge->bodies[bodInd].inWater = false;
			ge->bodies[bodInd].lastVel = btVector3(0.0f,0.0f,0.0f);
			ge->bodies[bodInd].totAV = btVector3(0.0f,0.0f,0.0f);
			ge->bodies[bodInd].totLV = btVector3(0.0f,0.0f,0.0f);
			
			
		}
		
		if (curOrgType == E_ORGTYPE_HUMAN) {
			singleton->gameOrgs[ge->orgId]->loadFromFile(poseStrings[E_PK_NON_POSE], true);
		}
		
	}
	
	
	

	
	// void motorPreTickCallback(btScalar timeStep) {
		
	// 	// int i;
		
	// 	// for (i = 0; i < singleton->gameActors.size(); i++) {
	// 	// 	singleton->gameActors[i]->stepSim(timeStep);
	// 	// }
		
		
		
	// }


	void flushImpulses() {
		
		int k;
		BaseObj* ge;
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (
				(ge->isHidden)
			) {
				
			}
			else {
				ge->flushImpulses();
			}
		}
	}

	void collideWithWorld(double curStepTime) {
		
		
		int i;
		int j;
		int k;
		int m;
		int p;
		int q;
		
		int otherK;
		
		int curBone;
		int bodInd;
		float cellVal[2];
		btVector3 segPos[2];
		int segCount;
		bool lastInside;
		BodyStruct* curBody;
		
		BaseObj* ge;
		
		BaseObj* geArr[2];
		BodyStruct* curBodyArr[2];
		
		
		BaseObj* grabber;
		
		FIVector4* curCenterPoint;
		btDiscreteDynamicsWorld* world = example->getWorld();
		
		btVector3 tempBTV;
		btVector3 tempBTV2;
		btVector3 nv0;
		btVector3 nv1;
		
		
		bool doProc = false;
		bool hasContact = false;
		
		int entNum;
		
		FIVector4 tempVec;
		
		btVector3 halfOffset = btVector3(-0.5f,-0.5f,-0.5f);
		
		
		float curDis;
		float totMass;
		float totForce;
		btVector3 dirForce;
		GameOrg* curOrg = NULL;
		GameOrgNode* curOrgNode = NULL;
		GameActor* curActor = NULL;
		btVector3 basePos;
		btVector3 targPos;
		int otherUID;
		
		
		collectDebris();
		
		
		
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (
				(ge->isHidden)
			) {
				
			}
			else {
				
				for (bodInd = 0; bodInd < ge->bodies.size(); bodInd++) {
					ge->bodies[bodInd].hasContact = false;
					ge->bodies[bodInd].isFalling = true;
				}
				
			}
		}
		
		
		const btCollisionObject* bds[2];
		
		int numManifolds = world->getDispatcher()->getNumManifolds();
		for (i=0;i<numManifolds;i++) {
			btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
			
			
			
			const btCollisionObject* obA = (contactManifold->getBody0());
			const btCollisionObject* obB = (contactManifold->getBody1());

			bds[0] = obA;
			bds[1] = obB;

			hasContact = false;
			
			int numContacts = contactManifold->getNumContacts();
			for (j=0;j<numContacts;j++) {
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance() <= CONTACT_THRESH) {
					
					hasContact = true;
					
					// const btVector3& ptA = pt.getPositionWorldOnA();
					// const btVector3& ptB = pt.getPositionWorldOnB();
					// const btVector3& normalOnB = pt.m_normalWorldOnB;
				}
			}
			
			
			geArr[0] = NULL;
			geArr[1] = NULL;
			curBodyArr[0] = NULL;
			curBodyArr[1] = NULL;
			
			if (bds[0]->bodyUID == bds[1]->bodyUID) {
				// don't register contacts within same entity
			}
			else {
				for (k = 0; k < 2; k++) {
					if (
						(bds[k]->bodyUID > -1) &&
						(bds[k]->limbUID > -1)
					) {
						ge = &(singleton->gw->gameObjects[ bds[k]->bodyUID ]);
						curBody = &(ge->bodies[ bds[k]->limbUID ]);
						
						
						
						if (
							(ge->isHidden)
						) {
							
						}
						else {
							
							if (hasContact) {
								geArr[k] = ge;
								curBodyArr[k] = curBody;
								
								curBody->hasContact = true;
								curBody->isFalling = false;
							}
							
						}
					}
				}
				
				// if (
				// 	(geArr[0] == NULL) ||
				// 	(geArr[1] == NULL) ||	
				// 	(curBodyArr[0] == NULL) ||
				// 	(curBodyArr[1] == NULL)
				// ) {
					
				// }
				// else {
					
					for (k = 0; k < 2; k++) {
						otherK = 1-k;
						
						if (geArr[k] == NULL) {
							
						}
						else {
							
							if (geArr[otherK] == NULL) {
								// hit static obj
								//singleton->playSoundEnt("metalhit5",geArr[k],0.2,0.5f);
								otherUID = -1;
							}
							else {
								otherUID = geArr[otherK]->uid;
								
								if (
									(geArr[k]->entType == E_ENTTYPE_NPC) &&
									(geArr[otherK]->entType == E_ENTTYPE_NPC)	
								) {
									
									if (curBodyArr[k] == NULL) {
										
									}
									else {
										
										
										curBone = getCorrectedName(curBodyArr[k]->boneId);
										doProc = false;
										
										switch(curBone) {
											case E_BONE_L_METACARPALS:
											case E_BONE_L_TALUS:
												if (geArr[k]->isSwinging[E_HAND_L]) {
													if (curBone == E_BONE_L_METACARPALS) {
														if (singleton->isPunching(geArr[k]->uid, E_HAND_L)) {
															doProc = true;
														}
													}
													else {
														if (singleton->isKicking(geArr[k]->uid, E_HAND_L)) {
															doProc = true;
														}
													}
												}
											break;
											case E_BONE_R_TALUS:
											case E_BONE_R_METACARPALS:
												if (geArr[k]->isSwinging[E_HAND_R]) {
													if (curBone == E_BONE_R_METACARPALS) {
														if (singleton->isPunching(geArr[k]->uid, E_HAND_R)) {
															doProc = true;
														}
													}
													else {
														if (singleton->isKicking(geArr[k]->uid, E_HAND_R)) {
															doProc = true;
														}
													}
												}
											break;
										}
									}
									if (doProc) {
										singleton->makeHit(geArr[k]->uid, geArr[otherK]->uid, -1);
									}
									
								}
								
							}
							
							
							if (geArr[k]->entType == E_ENTTYPE_WEAPON) {
								
								
								
								if (
									(geArr[k]->isGrabbedById > -1) &&
									(geArr[k]->isGrabbedById != otherUID)
								) {
									grabber = &(singleton->gw->gameObjects[geArr[k]->isGrabbedById]);
									
									doProc = false;
									if (grabber->isSwinging[E_HAND_L]) {
										if (singleton->isSwingingWeapon(grabber->uid,E_HAND_L)) {
											doProc = true;
										}
									}
									if (grabber->isSwinging[E_HAND_R]) {
										if (singleton->isSwingingWeapon(grabber->uid,E_HAND_R)) {
											doProc = true;
										}
									}
									
									if (doProc) {
										if (geArr[otherK] == NULL) {
											singleton->makeHit(grabber->uid, -1, geArr[k]->uid);
										}
										else {
											singleton->makeHit(grabber->uid, geArr[otherK]->uid, geArr[k]->uid);
										}
									}
									
									
									
									
									
								}
								
								
								
							}
						}
						
						
						
					}
					
					
				//}
				
			}
		}
		
		
		
		btVector3 difVec = btVector3(0.0,0.0,0.0);
		//btVector3 totVec = btVector3(0.0,0.0,0.0);
		bool hasRig = false;
		bool animatedRig = false;
		doProc = false;
		
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (
				(ge->isHidden)
			) {
				
			}
			else {
				for (bodInd = 0; bodInd < ge->bodies.size(); bodInd++) {
					curBody = &(ge->bodies[bodInd]);
					
					switch (curBody->jointType) {
						case E_JT_LIMB:
							segCount = 0;
						break;
						case E_JT_BALL:
							segCount = 1;
							segPos[0] = curBody->body->getCenterOfMassPosition();
						break;
						case E_JT_NORM:
							segCount = 0;
						break;
						case E_JT_CONT:
							segCount = 1;
							segPos[0] = curBody->body->getCenterOfMassPosition() -
								btVector3(0.0f,0.0f,1.0f);
							// segPos[1] = curBody->body->getCenterOfMassPosition() +
							// 	btVector3(0.0f,0.0f,1.0f);
							
							
							
						break;
					}
					
					
					for (p = 0; p < segCount; p++) {
						
						tempBTV = curBody->body->getLinearVelocity();
						tempBTV *= btVector3(1.0f,1.0f,0.0f);
						
						if (isFuzzy(tempBTV)) {
							
						}
						else {
							tempBTV.normalize();
						}
						tempBTV *= 0.5f;
						
						
						cellVal[0] = singleton->gw->getCellAtCoordsLin(
							segPos[p] + btVector3(0.0,0.0,-0.5f) + halfOffset
						);
						
						
						tempBTV += btVector3(0.0f,0.0f,1.5f);
						
						
						cellVal[1] = singleton->gw->getCellAtCoordsLin(
							segPos[p] + tempBTV + halfOffset
						);
						
						
						
						
						
						lastInside = curBody->isInside;
						curBody->isInside = (cellVal[0] > 0.3f);
						
						curBody->hasContact = (curBody->hasContact)||(cellVal[0] > 0.1f);
						curBody->isFalling = !(curBody->hasContact);
						
						
						
						curBody->body->setGravity(
							btVector3(
								0.0f,
								0.0f,
								mixf(-10.0f,10.0f,cellVal[0])
							)
						);
						if (
							(cellVal[0] > 0.5f) &&
							(curBody->body->getLinearVelocity().getZ() < 0.0)
						) {
							ge->multVel(bodInd, btVector3(1.0f,1.0f,0.0f));
						}
						
						if (cellVal[1] > 0.1) {
							ge->multVel(bodInd, btVector3(1.0f-cellVal[1],1.0f-cellVal[1],1.0f));
							//ge->addVel(bodInd, btVector3(1.0f,1.0f,0.0f));
						}
						
						
						
						//if (lastInside != curBody->isInside) {
							// if (curBody->isInside) {
								
							// 	if (q == 0) {
							// 		//ge->multVelocity(bodInd, btVector3(1.0f,1.0f,0.0f));
							// 		curBody->body->setGravity(btVector3(0.0f,0.0f,10.0f));
							// 	}
							// 	else {
							// 		//ge->multVelocity(bodInd, btVector3(-0.9f,-0.9f,1.0f));
							// 	}
								
								
							// }
							// else {
							// 	curBody->body->setGravity(btVector3(0.0f,0.0f,-10.0f));
							// }
						//}
						
						
						
						
						
					}
					
					
				}
			}
		}
		
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			hasRig = false;
			animatedRig = false;
			
			if (
				(ge->isHidden)
			) {
				
			}
			else {
				
				
				hasRig = (
					(ge->orgId > -1) &&
					(ge->actorId > -1)	
				);
				
				
				//totVec = btVector3(0.0,0.0,0.0);
				
				
				if (hasRig) {
					curActor = singleton->gameActors[ge->actorId];
					curOrg = singleton->gameOrgs[ge->orgId];
					animatedRig = (curOrg->orgType == E_ORGTYPE_HUMAN);
					ge->clearAABB(&(ge->aabbMinSkel),&(ge->aabbMaxSkel));
					
					if (animatedRig) {
						
						if (ge->isWalking) {
							ge->bodies[E_BDG_CENTER].body->setFriction(singleton->conVals[E_CONST_WALKING_FRIC]);
						}
						else {
							ge->bodies[E_BDG_CENTER].body->setFriction(singleton->conVals[E_CONST_STANDING_FRIC]);
						}
						
						// if (ge->baseContact()&&ge->isWalking) {
							
						// 	ge->bodies[E_BDG_CENTER].body->setGravity(btVector3(
						// 		0.0f,
						// 		0.0f,
						// 		singleton->conVals[E_CONST_WALKING_GRAV]
						// 	));
						// }
						// else {
							
						// 	//ge->contactCount = 0;
							
						// 	ge->bodies[E_BDG_CENTER].body->setGravity(btVector3(0.0f,0.0f,-10.0f));
						// }
						
						if (ge->isDead()) {
							curOrg->targetPoseGroup = E_PG_DEAD;
						}
						else {
							
							
							if (
								ge->isJumping &&
								(ge->bodies[E_BDG_CENTER].body->getLinearVelocity().getZ() >= 0.0f)
							) {
								curOrg->targetPoseGroup = E_PG_JUMP;
							}
							else {
								if (ge->isWalking || (ge->getPlanarVel() > 0.1f)) {
									curOrg->targetPoseGroup = E_PG_WALK;
								}
								else {
									curOrg->targetPoseGroup = E_PG_IDLE;
								}
							}
							
							if (ge->isPickingUp) {
								curOrg->targetPoseGroup = E_PG_PICKUP;
							}
							else {
								if (ge->isSwinging[E_HAND_R]) {
									curOrg->targetPoseGroup = ge->swingType[E_HAND_R];
								}
								else {
									if (ge->isSwinging[E_HAND_L]) {
										curOrg->targetPoseGroup = ge->swingType[E_HAND_L];
									}
								}
							}
						}
						
						if (curOrg->targetPoseGroup != E_PG_JUMP) {
							ge->isJumping = false;
						}
						
						
						
						
						
						
						curOrg->updatePose(curStepTime);
						
						
						if (ge->isGrabbingId[E_HAND_L] > -1) {
							ge->updateWeapon(
								E_HAND_L,
								curOrg->allNodes[getCorrectedName(E_BONE_L_METACARPALS)]->orgTrans[1].getBTV(),
								curOrg->allNodes[getCorrectedName(E_BONE_L_METACARPALS)]->tbnTrans[1].getBTV(),
								curStepTime,
								2.0f // todo: update this to actual weapon length
							);
						}
						
						if (ge->isGrabbingId[E_HAND_R] > -1) {
							ge->updateWeapon(
								E_HAND_R,
								curOrg->allNodes[getCorrectedName(E_BONE_R_METACARPALS)]->orgTrans[1].getBTV(),
								curOrg->allNodes[getCorrectedName(E_BONE_R_METACARPALS)]->tbnTrans[1].getBTV(),
								curStepTime,
								2.0f // todo: update this to actual weapon length
							);
						}
						
						
						ge->wakeAll();
						
						
						
					}
					
					
				}
				
				for (bodInd = 0; bodInd < ge->bodies.size(); bodInd++) {
					curBody = &(ge->bodies[bodInd]);
					
					
					
					
					//$$$$$$$$$$$
					
					
					
					//////////////////////
					// APPLY FORCES
					//////////////////////
					
					ge->applyImpulses(curStepTime, bodInd);
					
					
					
					if (
						hasRig
						&& animatedRig
					) {
						
						if (curBody->boneId > -1) {
							curOrgNode = curOrg->allNodes[curBody->boneId];
						}
						else {
							curOrgNode = NULL;
						}
						
						
						
						if (curOrgNode == NULL) {
							
							// difVec = btVector3(0.0f,0.0f,0.0f);
							
							// switch (bodInd) {
							// 	case E_BDG_CENTER:
									
							// 		// targPos = 
							// 		// 	ge->bodies[E_BDG_LFOOT].body->getCenterOfMassPosition()*0.5f +
							// 		// 	ge->bodies[E_BDG_RFOOT].body->getCenterOfMassPosition()*0.5f
							// 		// 	//+ btVector3(0.0f, 0.0f, 1.5f)
							// 		// ;
									
							// 		// difVec = targPos - curBody->body->getCenterOfMassPosition();
							// 		// //difVec = basePos - curBody->body->getCenterOfMassPosition();
							// 	break;
							// 	case E_BDG_LFOOT:
							// 		targPos = ge->bodies[E_BDG_CENTER].body->getCenterOfMassPosition();
							// 		curDis = ge->bodies[E_BDG_LFOOT].body->getCenterOfMassPosition().distance(
							// 			targPos
							// 		);
							// 		if (curDis > 2.5f) {
							// 			difVec = targPos - curBody->body->getCenterOfMassPosition();
							// 		}
							// 		if (curDis < 0.5f) {
							// 			difVec = -(targPos - curBody->body->getCenterOfMassPosition());
							// 		}
									
							// 	break;
							// 	case E_BDG_RFOOT:
							// 		targPos = ge->bodies[E_BDG_CENTER].body->getCenterOfMassPosition();
							// 		curDis = ge->bodies[E_BDG_RFOOT].body->getCenterOfMassPosition().distance(
							// 			targPos
							// 		);
							// 		if (curDis > 2.5f) {
							// 			difVec = targPos - curBody->body->getCenterOfMassPosition();
							// 		}
							// 		if (curDis < 0.5f) {
							// 			difVec = -(targPos - curBody->body->getCenterOfMassPosition());
							// 		}
							// 	break;
							// }
							
							// ge->applyImpulse(
							// 	difVec*curStepTime*curBody->mass*200.0f, // *MASS_PER_LIMB*2.0f*10.0f*curStepTime,
							// 	false,
							// 	bodInd
							// );
							
						}
						else {
							
							
							ge->bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);
							myMatrix4 = Matrix4(myMat);
							
							switch(curBody->jointType) {
								case E_JT_LIMB:
									tempBTV = curOrgNode->orgTrans[1].getBTV();
								break;
								case E_JT_BALL:
									tempBTV = curOrgNode->orgTrans[2].getBTV();
								break;
								case E_JT_NORM:
									
								break;
							}
							
							
							
								myVector4 = Vector4(
									tempBTV.getX(),
									tempBTV.getY(),
									tempBTV.getZ(),
									1.0f
								);
								resVector4 = myMatrix4*myVector4;
								basePos = btVector3(resVector4.x,resVector4.y,resVector4.z);
								
								
								
								ge->addAABBPoint(&(ge->aabbMinSkel), &(ge->aabbMaxSkel), basePos);
								
								basePos += ge->skelOffset;// - btVector3(0.0f, 0.0f, BASE_ENT_HEIGHT*0.5f);
								
								
								
								// if (ge->weaponActive) {
									
								// 	switch (curBody->boneId) {
										
										
										
								// 		case E_BONE_L_METACARPALS:
										
								// 			if (true) {
								// 				if (ge->rightHandTop) {
								// 					difVec = ge->weaponVec0 - curBody->body->getCenterOfMassPosition();
								// 				}
								// 				else {
								// 					difVec = ge->weaponVec0*0.8 + ge->weaponVec1*0.2 - curBody->body->getCenterOfMassPosition();
								// 				}
								// 			}
								// 			else {
								// 				difVec = btVector3(0.0f,0.0f,0.0f);
												
								// 			}
										
											
										
											
								// 		break;
								// 		case E_BONE_R_METACARPALS:
										
								// 			if (true) {
								// 				if (ge->rightHandTop) {
								// 					difVec = ge->weaponVec0*0.8 + ge->weaponVec1*0.2 - curBody->body->getCenterOfMassPosition();
								// 				}
								// 				else {
								// 					difVec = ge->weaponVec0 - curBody->body->getCenterOfMassPosition();
								// 				}
								// 			}
								// 			else {
								// 				difVec = btVector3(0.0f,0.0f,0.0f);
												
								// 			}
										
											
								// 		break;
								// 		case E_BONE_L_LOWERARM:
								// 		case E_BONE_R_LOWERARM:
								// 		case E_BONE_L_UPPERARM:
								// 		case E_BONE_R_UPPERARM:
								// 			difVec = btVector3(0.0f,0.0f,0.0f);
								// 		break;
								// 		default:
								// 			difVec = basePos - curBody->body->getCenterOfMassPosition();
								// 		break;
								// 	}
									
									
								// }
								// else {
									difVec = basePos - curBody->body->getCenterOfMassPosition();
								//}
								
								
								
								// move limbs towards pose
										
								//totVec += difVec;
								
								
								
								// ge->applyImpulse(
								// 	difVec*curStepTime*curBody->mass*singleton->conVals[E_CONST_LIMB_IMPULSE]*ge->bindingPower, // *MASS_PER_LIMB*2.0f*10.0f*curStepTime,
								// 	false,
								// 	bodInd
								// );
								
								ge->setLinVel(
									curBody->body->getLinearVelocity()*(1.0f-ge->bindingPower)
									+ difVec*20.0f*ge->bindingPower,
									bodInd
								);
								
								
								
								// switch(curBody->jointType) {
								// 	case E_JT_LIMB:
								// 		moveToOrientation(
								// 			curBody->body,
								// 			curOrgNode->tbnTrans[0].getBTV().normalize(),
								// 			curOrgNode->tbnTrans[1].getBTV().normalize(),
								// 			curOrgNode->tbnTrans[2].getBTV().normalize(),
								// 			0.1f
								// 		);
								// 	break;
								// 	case E_JT_BALL:
										
								// 	break;
								// 	case E_JT_NORM:
										
								// 	break;
								// }
								
								
							
							
							
							//
						}
						
						
					}
					
					if (
						(ge->isGrabbedById > -1) &&
						(ge->isGrabbedByHand > -1)	
					) {
						
						grabber = &(singleton->gw->gameObjects[ge->isGrabbedById]);
						
						doProc = true;
						
						if (curBody->boneId == E_BONE_WEAPON_END) {
							
							switch(curBody->jointType) {
								case E_JT_LIMB:
									basePos = 
										grabber->weaponVec0[ge->isGrabbedByHand]*0.5f +
										grabber->weaponVec1[ge->isGrabbedByHand]*0.5f;
								break;
								case E_JT_BALL:
									basePos = grabber->weaponVec1[ge->isGrabbedByHand];
								break;
								case E_JT_NORM:
									
								break;
							}
							
						}
						else {
							if (curBody->boneId == E_BONE_WEAPON_BASE) {
								basePos = grabber->weaponVec0[ge->isGrabbedByHand];
							}
							else {
								doProc = false;
							}
						}
						
						if (doProc) {
							difVec = basePos - curBody->body->getCenterOfMassPosition();
							
							// move limbs weapon
							
							//if (curBody->boneId == E_BONE_WEAPON_BASE) {
								ge->setLinVel(
									difVec*20.0f,
									bodInd
								);
							// }
							// else {
							// 	ge->applyImpulse(
							// 		difVec*curStepTime*curBody->mass*singleton->conVals[E_CONST_LIMB_IMPULSE], // *MASS_PER_LIMB*2.0f*10.0f*curStepTime,
							// 		false,
							// 		bodInd
							// 	);
							// }
							
							
							
							
							
						}
						
						
						
						
					}
					
					
					if (
						(singleton->selObjInd == ge->uid) &&
						singleton->markerFound &&
						singleton->isDraggingObject &&
						(singleton->draggingFromType == E_DT_WORLD_OBJECT)
					) {
						
						if (lastBodyPick == NULL) {
							
						}
						else {
							if (
								lastBodyPick->limbUID ==
								curBody->body->limbUID
							) {
								
								//totMass = ge->getTotalMass();
								
								//ge->applyAngularImpulse(btVector3(0,0,-0.02)/STEP_TIME_IN_SEC, false, curBody->body->limbUID);
								
								
							}
						}
						
						
						// ge->applyImpulse(
						// 	btVector3(
						// 		( singleton->worldMarker.getFX() - curBody->body->getCenterOfMassPosition().getX() ),
						// 		( singleton->worldMarker.getFY() - curBody->body->getCenterOfMassPosition().getY() ),
						// 		-(curBody->body->getCenterOfMassPosition().getZ() - (8.0f + singleton->worldMarker.getFZ()))
						// 	)*totMass*100.0f*curStepTime,
						// 	false,
						// 	0
						// );
						
						
						
						
						
					}
					
					
					
					for (m = 0; m < singleton->sphereStack.size(); m++) {
						tempBTV = curBody->body->getCenterOfMassPosition();
						tempBTV2 = singleton->sphereStack[m].position.getBTV();
						
						totForce = (
							1.0f-clampfZO(
								tempBTV.distance(tempBTV2)/(singleton->sphereStack[m].curRad*5.0f)	
							)
						)*5.0f; // * singleton->sphereStack[m].power;
						dirForce = tempBTV-tempBTV2;
						dirForce.normalize();
						dirForce = dirForce*totForce;
						
						dirForce.setZ(totForce);
						
						ge->applyImpulse(dirForce*curStepTime*5.0f*curBody->mass, false, bodInd);
					}
					
					
					//////////////////////
					// END APPLY FORCES
					//////////////////////
					
					
					
					
					
					nv0 = curBody->body->getLinearVelocity();
					nv1 = curBody->lastVel;
					
					if (isFuzzy(nv0) || isFuzzy(nv1)) {
						
					}
					else {
						nv0.normalize();
						nv1.normalize();
						
						
						if (
							//(!(curBody->isInside)) &&
							(
								curBody->lastVel.length() > 0.5f
							) &&
							(
								(nv0.dot(nv1)) < 0.8f
							) &&
							(bodInd == 0) &&
							(ge->isGrabbedById < 0) 
							
						) {
							
							
							singleton->gw->fireEvent(
								ge->uid,
								EV_COLLISION,
								clampfZO( (curBody->lastVel.length()-0.5f)/16.0f )*
								(1.0f-clampfZO(ge->getCenterPointFIV(bodInd)->distance(singleton->cameraGetPosNoShake())/(50.0f)))
							);
						}
					}
					
					
					
					curBody->lastVel = curBody->body->getLinearVelocity();
					
					
					
					if (ge->entType == E_ENTTYPE_BULLET) {
						if (
							(!(curBody->isFalling)) && 
							(curBody->body->getLinearVelocity().length() < 0.5)
						) {
							singleton->explodeBullet(ge);
						}
					}
					
					
					
					
					//$$$$$$$$$$$
					
				}
				
				ge->zeroZ = false;
				
				//ge->skelOffset *= 0.99;
				
				if (ge->baseContact()&&hasRig&&animatedRig) {
					
					
					
					ge->skelOffset = btVector3(
						0.0f,
						0.0f,
						
						
							
						(
							(ge->getCenterPoint(0).getZ()-(BASE_ENT_HEIGHT+BASE_ENT_RAD)*0.5f) -
							ge->aabbMinSkel.getZ()	
						)
							
						
							
					);
					
					if (singleton->editPose) {
						ge->skelOffset += btVector3(
							0.0,
							0.0,
							1.0f
						);
					}
					else {
						// ge->skelOffset += btVector3(
						// 	0.0,
						// 	0.0,
						// 	-0.75f
						// );
					}
					
										
					// if (totVec.getZ() < 0.0f) {
						
					// 	ge->skelOffset += btVector3(
					// 			0.0f,
					// 			0.0f,
					// 			-totVec.getZ()*0.3f	
					// 	);
					// }
					
					
					
					
					// if ( singleton->feetContact(ge) ) {
					// 	ge->skelOffset += btVector3(
					// 			0.0f,
					// 			0.0f,
					// 			0.003f
					// 	);
					// }
					// else {
					// 	ge->skelOffset += btVector3(
					// 			0.0f,
					// 			0.0f,
					// 			-0.002f	
					// 	);
					// }
					
					
				}
				else {
					ge->skelOffset *= 0.99;
				}
				
			}
			
			
		}
		
		singleton->explodeStack.clear();
		
		
		
	}

	
	void updateAll() {
		
		while (singleton->totTimePassedPhysics > STEP_TIME_IN_MICRO_SEC) {
			totTime += STEP_TIME_IN_SEC;
			collideWithWorld(STEP_TIME_IN_SEC); //STEP_TIME_IN_MICRO_SEC/400000.0f
			example->stepSimulation(STEP_TIME_IN_SEC); //STEP_TIME_IN_MICRO_SEC/400000.0f
			singleton->totTimePassedPhysics -= STEP_TIME_IN_MICRO_SEC;
		}
		
		flushImpulses();
	}
	
	
	
	~GamePhysics() {
		example->exitPhysics();
		delete example;
	}
	
	
};







