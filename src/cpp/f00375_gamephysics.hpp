
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
		
		bool hasRig = (
			(ge->orgId > -1) &&
			(ge->actorId > -1)	
		);
		
		GameActor* curActor;
		
		int i;
		
		for (i = 0; i < 2; i++) {
			if (ge->isGrabbingId[i] > -1) {
				singleton->grabThrowObj(ge->uid,i);
			}
		}
		
		
		
		if (hasRig) {
			curActor = singleton->gameActors[ge->actorId];
			curActor->removeAllBodies();
			
			while (ge->bodies.size() > E_BDG_LENGTH ) {
				ge->bodies.pop_back();
			}
			
		}
		
		for (bodInd = 0; bodInd < ge->bodies.size(); bodInd++) { // E_BDG_LENGTH
			example->removeRigidBody(ge->bodies[bodInd].body);
		}
		
		
		
		
		
		// if (hasRig) {
		// 	//curActor = singleton->gameActors[ge->actorId];
		// 	delete singleton->gameActors[ge->actorId];
		// 	singleton->gameActors[ge->actorId] = NULL;
			
		// 	delete singleton->gameOrgs[ge->orgId];
		// 	singleton->gameOrgs[ge->orgId] = NULL;
		// }
		
		// ge->actorId = -1;
		// ge->orgId = -1;
		
		ge->bodies.clear();
	}
	
	void addBoxFromObj(BaseObjType _uid, bool refreshLimbs) {
		
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
						
						
						////////////////////////////
						
						if (refreshLimbs) {
							// todo: add stuff
						}
						else {
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
						}
						
						
						
						
						////////////////////////////
						
						
					}
					
					
					
					
					if (refreshLimbs) {
						
					}
					else {
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
						ge->actorId = singleton->gameActors.size()-1;
					}
					
					
					curActor = (singleton->gameActors[ge->actorId]);
					
					if (refreshLimbs) {
						curActor->reinit();	
					}
					
					
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
			
			ge->bodies[bodInd].body->setContactProcessingThreshold(CONTACT_THRESH);
			
			ge->bodies[bodInd].isVisible = true;
			
			if (isOrg) {
				
				curOrg = singleton->gameOrgs[ge->orgId];
								
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
			singleton->gameOrgs[ge->orgId]->loadFromFile(
				singleton->getPoseString(E_PG_NONPOSE, RLBN_NEIT, 0),
				true
			);
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
	
	
	void procCol(
		BaseObj** geArr,
		BodyStruct** curBodyArr	
	) {
		
		int k;
		int otherK;
		BaseObj* grabber;
		int otherUID;
		bool doProc;
		int curBone;
		
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
									if (geArr[k]->getActionState(E_ACT_ISSWINGING,RLBN_LEFT)) {
										if (curBone == E_BONE_L_METACARPALS) {
											if (singleton->isPunching(geArr[k]->uid, RLBN_LEFT)) {
												doProc = true;
											}
										}
										else {
											if (singleton->isKicking(geArr[k]->uid, RLBN_LEFT)) {
												doProc = true;
											}
										}
									}
								break;
								case E_BONE_R_TALUS:
								case E_BONE_R_METACARPALS:
									if (geArr[k]->getActionState(E_ACT_ISSWINGING,RLBN_RIGT)) {
										if (curBone == E_BONE_R_METACARPALS) {
											if (singleton->isPunching(geArr[k]->uid, RLBN_RIGT)) {
												doProc = true;
											}
										}
										else {
											if (singleton->isKicking(geArr[k]->uid, RLBN_RIGT)) {
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
						if (grabber->getActionState(E_ACT_ISSWINGING,RLBN_LEFT)) {
							if (singleton->isSwingingWeapon(grabber->uid,RLBN_LEFT)) {
								doProc = true;
							}
						}
						if (grabber->getActionState(E_ACT_ISSWINGING,RLBN_RIGT)) {
							if (singleton->isSwingingWeapon(grabber->uid,RLBN_RIGT)) {
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
	}
	

	void collideWithWorld(double curStepTime) {
		
		
		int i;
		int j;
		int k;
		int m;
		int p;
		int q;
		
		
		
		
		int bodInd;
		float totForce;
		float bindingPower;
		float cellVal[4];
		btVector3 newVel;
		btVector3 segPos[2];
		btVector3 norVal;
		btVector3 norVal2;
		int segCount;
		bool lastInside;
		BodyStruct* curBody;
		
		BaseObj* ge;
		
		BaseObj* geArr[2];
		BodyStruct* curBodyArr[2];
		
		GameOrg* grabberOrg = NULL;
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
		
		btVector3 halfOffset = btVector3(0.5f,0.5f,0.5f);
		
		
		float curDis;
		float totMass;
		btVector3 dirForce;
		GameOrg* curOrg = NULL;
		GameOrgNode* curOrgNode = NULL;
		GameActor* curActor = NULL;
		btVector3 basePos;
		btVector3 targPos;
		
		
		
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
					
					
				procCol(geArr,curBodyArr);
					
				//}
				
			}
		}
		
		
		
		btVector3 difVec = btVector3(0.0,0.0,0.0);
		//btVector3 totVec = btVector3(0.0,0.0,0.0);
		bool hasRig = false;
		bool animatedRig = false;
		doProc = false;
		float angDamp = singleton->conVals[E_CONST_ANGDAMP];
		
		
		if (VOXEL_COLLISION) {
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
								segCount = 1;
								segPos[0] = curBody->body->getCenterOfMassPosition() + halfOffset -
									btVector3(0.0f,0.0f,singleton->conVals[E_CONST_COLDEPTH_LIMB]);
							break;
							case E_JT_BALL:
								segCount = 1;
								segPos[0] = curBody->body->getCenterOfMassPosition() + halfOffset -
									btVector3(0.0f,0.0f,singleton->conVals[E_CONST_COLDEPTH_LIMB]);
							break;
							case E_JT_NORM:
								segCount = 0;
							break;
							case E_JT_CONT:
								segCount = 2;
								segPos[0] = curBody->body->getCenterOfMassPosition() + halfOffset -
									btVector3(0.0f,0.0f,singleton->conVals[E_CONST_COLDEPTH_CONT]);
									
								newVel = curBody->body->getLinearVelocity();
								if (isFuzzy(newVel)) {
									
								}
								else {
									newVel.normalize();
								}
								
									
								segPos[1] = 
									curBody->body->getCenterOfMassPosition() +
									halfOffset +
									newVel;
									
								// segPos[1] = curBody->body->getCenterOfMassPosition() +
								// 	btVector3(0.0f,0.0f,1.0f);
								
								
								
							break;
						}
						
						
						for (p = 0; p < segCount; p++) {
							
							norVal = singleton->gw->getNormalAtCoord(
								segPos[p], cellVal
							);
							
							if (p == 0) {
								// collision below body
								
								lastInside = curBody->isInside;
								curBody->isInside = (cellVal[3] > 0.5f);
								
								curBody->hasContact = (curBody->hasContact)||(cellVal[3] > 0.01f);
								curBody->isFalling = !(curBody->hasContact);
								
								if (cellVal[3] > 0.01f) {
									ge->multVel(bodInd, btVector3(0.999f,0.999f,1.0f));
								}
								
								if (cellVal[3] > 0.1f) {
									
									ge->multVelAng(bodInd, btVector3(angDamp,angDamp,angDamp));
									
									curBody->body->setGravity(
										btVector3(
											0.0f,
											0.0f,
											cellVal[3]*singleton->conVals[E_CONST_ANTIGRAV]
										)
									);
									
									if (
										(curBody->body->getLinearVelocity().getZ() < 0.0f)	
									) {
										ge->multVel(bodInd, btVector3(1.0f,1.0f,0.0f));
										//ge->moveOffset(btVector3(0,0,clampfZO(cellVal[3]-0.1f)), bodInd);
									}
									
									
									if (
										(ge->entType == E_ENTTYPE_WEAPON) &&
										(ge->isGrabbedById > -1) &&
										(cellVal[3] > 0.5f)								
									) {
										geArr[0] = ge;
										geArr[1] = NULL;
										curBodyArr[0] = curBody;
										curBodyArr[1] = NULL;
										procCol(geArr,curBodyArr);
									}
									
									// if (cellVal[3] > 0.2f) {
									// 	ge->multVel(bodInd, btVector3(1.0f,1.0f,-1.0f));
									// }
									
									
								}
								else {
									curBody->body->setGravity(
										btVector3(
											0.0f,
											0.0f,
											-10.0f
										)
									);
								}
								curBody->body->setActivationState(ACTIVE_TAG);
							}
							else {
								// collision in direction of body velocity
								
								if (isFuzzy(norVal)) {
									
								}
								else {
									newVel = curBody->body->getLinearVelocity();
									if (isFuzzy(newVel)) {
										
									}
									else {
										newVel.normalize();
										ge->multVel(
											bodInd,
											btVector3(
												newVel.getX()*norVal.getX(),
												newVel.getY()*norVal.getY(),
												1.0f // newVel.getZ()*norVal.getZ()	
											)
										);
									}
								}
								
								
							}
							
							
							// newVel = curBody->body->getLinearVelocity();
							// if (isFuzzy(newVel)) {
								
							// }
							// else {
							// 	newVel.normalize();
								
							// 	if (
							// 		(cellVal[3] > 0.5f)
							// 		//&& (newVel.dot(norVal) < -0.2f)	
							// 	) {
							// 		ge->setLinVel(norVal*10.0f,bodInd);
							// 	}
								
							// }
							
							// if (norVal.getZ() > 0.0f) {
							// 	ge->multVel(bodInd, btVector3(abs(norVal.getZ()),abs(norVal.getZ()),1.0f));
								
							// 	//ge->addVel(bodInd, btVector3(0.0f,0.0f,(1.0-norVal.getZ()));
							// }
							
							
							
							
							
							// ge->applyImpulse(
							// 	cellVal[3]*norVal*curStepTime*singleton->conVals[E_CONST_PUSH_UP_AMOUNT],
							// 	bodInd,
							// 	false
							// );
							
							
							// ge->multVel(bodInd, btVector3(
							// 	1.0f-cellVal[0],
							// 	1.0f-cellVal[1],
							// 	1.0f-cellVal[2]
							// ));
							
							//ge->addVel(bodInd, norVal2*curStepTime*200.0f);
							
							// curBody->body->setGravity(
							// 	btVector3(
							// 		0.0f,
							// 		0.0f,
							// 		mixf(-10.0f,10.0f,cellVal[3])
							// 	)
							// );
							// if (
							// 	(cellVal[3] > 0.5f)
							// 	// && (curBody->body->getLinearVelocity().getZ() < 0.0)
							// ) {
							// 	ge->multVel(bodInd, btVector3(-1.0f,-1.0f,-1.0f));
							// }
							
							//ge->addVel(bodInd, norVal*curStepTime*20.0f);
							
							
							
							
							// tempBTV = curBody->body->getLinearVelocity();
							// tempBTV *= btVector3(1.0f,1.0f,0.0f);
							
							// if (isFuzzy(tempBTV)) {
								
							// }
							// else {
							// 	tempBTV.normalize();
							// }
							// tempBTV *= 0.5f;
							
							
							// cellVal[0] = singleton->gw->getCellAtCoordsLin(
							// 	segPos[p] + btVector3(0.0,0.0,-0.5f)
							// );
							
							
							// tempBTV += btVector3(0.0f,0.0f,1.5f);
							
							
							// cellVal[1] = singleton->gw->getCellAtCoordsLin(
							// 	segPos[p] + tempBTV
							// );
							
							
							
							
							
							
							
							
							
							// curBody->body->setGravity(
							// 	btVector3(
							// 		0.0f,
							// 		0.0f,
							// 		mixf(-10.0f,10.0f,cellVal[0])
							// 	)
							// );
							// if (
							// 	(cellVal[0] > 0.5f) &&
							// 	(curBody->body->getLinearVelocity().getZ() < 0.0)
							// ) {
							// 	ge->multVel(bodInd, btVector3(1.0f,1.0f,0.0f));
							// }
							
							// if (cellVal[1] > 0.1) {
							// 	ge->multVel(bodInd, btVector3(1.0f-cellVal[1],1.0f-cellVal[1],1.0f));
							// 	ge->addVel(bodInd, btVector3(1.0f,1.0f,0.0f));
							// }
							
							
							
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
						
						if (ge->getActionState(E_ACT_ISWALKING,RLBN_NEIT)) {
							ge->bodies[E_BDG_CENTER].body->setFriction(singleton->conVals[E_CONST_WALKING_FRIC]);
						}
						else {
							ge->bodies[E_BDG_CENTER].body->setFriction(singleton->conVals[E_CONST_STANDING_FRIC]);
						}
						
						
						
						
						if (ge->isDead()) {
							curOrg->setTPG(E_PG_DEAD,RLBN_NEIT);
						}
						else if (ge->getActionState(E_ACT_ISSWINGING,RLBN_RIGT)) {
							curOrg->setTPG(ge->swingType[RLBN_RIGT], RLBN_RIGT);
						}
						else if (ge->getActionState(E_ACT_ISSWINGING,RLBN_LEFT)) {
							curOrg->setTPG(ge->swingType[RLBN_LEFT], RLBN_LEFT);
						}
						else if (
							ge->getActionState(E_ACT_ISJUMPING,RLBN_NEIT) &&
							(ge->bodies[E_BDG_CENTER].body->getLinearVelocity().getZ() >= 0.0f)
						) {
							curOrg->setTPG(E_PG_JUMP,RLBN_NEIT);
						}
						else if (ge->getActionState(E_ACT_ISPICKINGUP,RLBN_NEIT)) {
							curOrg->setTPG(E_PG_PICKUP,RLBN_NEIT);
						}
						else if (
							ge->getActionState(E_ACT_ISWALKING,RLBN_NEIT) ||
							(ge->getPlanarVel() > 0.1f)
						) {
							curOrg->setTPG(E_PG_WALKFORWARD,RLBN_NEIT);
						}
						else {
							curOrg->setTPG(E_PG_IDLE,RLBN_NEIT);
						}
						
						if (curOrg->targetPose.group != E_PG_JUMP) {
							ge->setActionState(E_ACT_ISJUMPING,RLBN_NEIT,false);
						}
						
						
						curOrg->updatePose(curStepTime);
						
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
						&& (
							animatedRig ||
							(ge->isGrabbedById > -1)
						)
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
							
							
							if (ge->isGrabbedById > -1) {
								grabber = &(singleton->gw->gameObjects[ge->isGrabbedById]);
								bindingPower = 1.0f;
							}
							else {
								grabber = ge;
								bindingPower = ge->bindingPower;
							}
							
							
							
							grabber->bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);
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
								
								basePos += grabber->skelOffset;
								
								difVec = basePos - curBody->body->getCenterOfMassPosition();
								
								
								// move limbs towards pose
								
								
								ge->setLinVel(
									curBody->body->getLinearVelocity()*(1.0f-bindingPower)
									+ difVec*20.0f*bindingPower,
									bodInd
								);
								
								
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







