// f00375_gamephysics.h
//

#include "f00375_gamephysics.e"
#define LZZ_INLINE inline
GamePhysics::GamePhysics ()
                      {
		lastBodyPick = NULL;
		//gameActor = NULL;
		//8000; // ~120 times per second
	}
void GamePhysics::init (Singleton * _singleton)
        {
		
		cout << "GamePhysics:init()\n";
		
		singleton = _singleton;
		myOGLApp = new MyOGLApp("yo", 640, 480);
		guiHelper = new MyGLHelper(singleton, myOGLApp);
		example = new BenchmarkDemo(guiHelper,5);
		example->initPhysics();
		
	}
void GamePhysics::pickBody (FIVector4 * mouseDownOPD)
                                               { //btVector3 posWS1, btVector3 posWS2) {
		
		if (!(singleton->editPose)) {
			lastBodyPick = NULL;
			return;
		}
		
		int bodyUID = mouseDownOPD->getFW();
		int limbUID = mouseDownOPD->getFZ();
		BaseObj* ge;
		
		if (
			(bodyUID > 0) &&
			(limbUID > -1)	
		) {
			ge = &(singleton->gw->gameObjects[bodyUID]);
			lastBodyPick = ge->bodies[limbUID].body;
		}
		else {
			lastBodyPick = NULL;
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
void GamePhysics::collectDebris ()
                             {
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
void GamePhysics::beginDrop ()
                         {
		
		cout << "GamePhysics:beginDrop()\n";
		
		example->beginDrop(
			singleton->cameraGetPosNoShake()->getFX(),
			singleton->cameraGetPosNoShake()->getFY(),
			singleton->cameraGetPosNoShake()->getFZ()
		);
	}
void GamePhysics::remBoxFromObj (BaseObjType _uid)
                                             {
		
		BaseObj* ge = &(singleton->gw->gameObjects[_uid]);
		
		int bodInd;
		for (bodInd = 0; bodInd < ge->bodies.size(); bodInd++) {
			example->removeRigidBody(ge->bodies[bodInd].body);
		}
		ge->bodies.clear();
	}
void GamePhysics::addBoxFromObj (BaseObjType _uid)
                                             {
		
		int i;
		
		BaseObj* ge = &(singleton->gw->gameObjects[_uid]);
		
		if (ge->isHidden) {
			return;
		}
		
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(ge->startPoint);
		
		GameActor* curActor;
		
		float objRad = 0.5f;
		
		int bodyCollidesWith = COL_STATIC|COL_DYN;
		int terCollidesWith = COL_NOTHING;
		int markerCollidesWith = COL_STATIC|COL_DYN;
		int dynCollidesWith = COL_STATIC|COL_DYN|COL_BODY|COL_MARKER;
		
		bool isOrg = false;
		
		if (
			(ge->entType == E_ENTTYPE_NPC) ||
			(ge->entType == E_ENTTYPE_MONSTER)	
		) {
			
			isOrg = true;
			
			btCapsuleShapeZ* capsuleShape = new btCapsuleShapeZ(0.25f,0.25f);
			ge->bodies.push_back(BodyStruct());
			ge->bodies.back().body = example->createRigidBodyMask(
				MASS_PER_LIMB,
				trans,
				capsuleShape,
				COL_MARKER,
				markerCollidesWith
			);
			ge->bodies.back().body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
			
			
			singleton->gameOrgs.push_back(new GameOrg());
			singleton->gameOrgs.back()->init(singleton);
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
				ge->bodies.back().isBall = curActor->actorJoints[i].isBall;
				
				// if (i == 0) {
				// 	//ge->body = curActor->actorJoints[i].body;
				// 	//ge->body->setLinearFactor(btVector3(0.0f,0.0f,0.0f));
				// 	ge->bodies.back().body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
				// }
				// else {
					
				// }
			}
			
			
		}
		else {
			
			
			btBoxShape* boxShape = new btBoxShape(btVector3(objRad,objRad,objRad));
			ge->bodies.push_back(BodyStruct());
			ge->bodies.back().body = example->createRigidBodyMask(
				MASS_PER_LIMB,
				trans,
				boxShape
				,COL_DYN,
				dynCollidesWith
			);
			
			if (ge->entType == E_ENTTYPE_DEBRIS) {
				
				ge->bodies[0].body->setAngularVelocity(btVector3(
					(fGenRand2()*2.0f-1.0f),
					(fGenRand2()*2.0f-1.0f),
					(fGenRand2()*2.0f-1.0f)	
				)*4.0f);
				ge->bodies[0].boneId = -1;
			}
			
			
			
			
			
		}
		
		int bodInd;
		for (bodInd = 0; bodInd < ge->bodies.size(); bodInd++) {
			ge->bodies[bodInd].body->bodyUID = _uid;
			ge->bodies[bodInd].body->limbUID = bodInd;
			
			if (bodInd == 0) {
				ge->bodies[bodInd].body->setDamping(0.1f,0.9f);
				
			}
			else {
				
				if (bodInd < 7) {
					ge->bodies[bodInd].body->setDamping(0.99f,0.9f);
				}
				else {
					ge->bodies[bodInd].body->setDamping(0.99f,0.9f);
				}
				
				
			}
			
			
			ge->bodies[bodInd].body->setContactProcessingThreshold(0.25f);
			
			ge->bodies[bodInd].isVisible = ((bodInd > 0)&&isOrg)||(!isOrg);
			ge->bodies[bodInd].mass = MASS_PER_LIMB;
			ge->bodies[bodInd].hasContact = false;
			ge->bodies[bodInd].isInside = false;
			ge->bodies[bodInd].isFalling = true;
			ge->bodies[bodInd].inWater = false;
			ge->bodies[bodInd].lastVel = btVector3(0.0f,0.0f,0.0f);
			ge->bodies[bodInd].totAV = btVector3(0.0f,0.0f,0.0f);
			ge->bodies[bodInd].totLV = btVector3(0.0f,0.0f,0.0f);
			
			
		}
		
		
		
	}
void GamePhysics::motorPreTickCallback (btScalar timeStep)
                                                     {
		
		// int i;
		
		// for (i = 0; i < singleton->gameActors.size(); i++) {
		// 	singleton->gameActors[i]->stepSim(timeStep);
		// }
		
		
		
	}
void GamePhysics::flushImpulses ()
                             {
		
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
void GamePhysics::collideWithWorld (double curStepTime)
                                                  {
		
		
		
		int i;
		int j;
		int k;
		int m;
		
		int bodInd;
		int cellVal;
		BodyStruct* curBody;
		
		BaseObj* ge;
		
		FIVector4* curCenterPoint;
		btDiscreteDynamicsWorld* world = example->getWorld();
		
		btVector3 tempBTV;
		btVector3 tempBTV2;
		btVector3 nv0;
		btVector3 nv1;
		
		
		bool hasContact = false;
		
		int entNum;
		
		FIVector4 tempVec;
		
		
		collectDebris();
		
		motorPreTickCallback(curStepTime);
		
		
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
		
		
		const btCollisionObject* bodies[2];
		
		int numManifolds = world->getDispatcher()->getNumManifolds();
		for (i=0;i<numManifolds;i++) {
			btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
			
			
			
			const btCollisionObject* obA = (contactManifold->getBody0());
			const btCollisionObject* obB = (contactManifold->getBody1());

			bodies[0] = obA;
			bodies[1] = obB;

			hasContact = false;
			
			int numContacts = contactManifold->getNumContacts();
			for (j=0;j<numContacts;j++) {
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance() < 0.1f) {
					
					hasContact = true;
					
					// const btVector3& ptA = pt.getPositionWorldOnA();
					// const btVector3& ptB = pt.getPositionWorldOnB();
					// const btVector3& normalOnB = pt.m_normalWorldOnB;
				}
			}
			
			
			if (bodies[0]->bodyUID == bodies[1]->bodyUID) {
				// don't register contacts within same entity
			}
			else {
				for (k = 0; k < 2; k++) {
					if (
						(bodies[k]->bodyUID > -1) &&
						(bodies[k]->limbUID > -1)
					) {
						ge = &(singleton->gw->gameObjects[ bodies[k]->bodyUID ]);
						curBody =  &(ge->bodies[ bodies[k]->limbUID ]);
						
						
						if (
							(ge->isHidden)
						) {
							
						}
						else {
							
							if (hasContact) {
								curBody->hasContact = true;
								curBody->isFalling = false;
							}
							
							
						}
						
					}
				}
			}
			
			
			
		}
		
		
		
		float totMass;
		float totForce;
		btVector3 dirForce;
		GameOrg* curOrg;
		GameOrgNode* curOrgNode;
		GameActor* curActor;
		btVector3 basePos;
		
		btVector3 difVec = btVector3(0.0,0.0,0.0);
		btVector3 totVec = btVector3(0.0,0.0,0.0);
		
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (
				(ge->isHidden)
			) {
				
			}
			else {
				
				totVec = btVector3(0.0,0.0,0.0);
				
				for (bodInd = 0; bodInd < ge->bodies.size(); bodInd++) {
					curBody = &(ge->bodies[bodInd]);
					
					
					
					
					//$$$$$$$$$$$
					
					
					
					//////////////////////
					// APPLY FORCES
					//////////////////////
					
					ge->applyImpulses(curStepTime, bodInd);
					
					tempBTV = curBody->body->getCenterOfMassPosition();
					
					cellVal = singleton->gw->getCellAtCoords(
						tempBTV.getX(),
						tempBTV.getY(),
						tempBTV.getZ()
					);
					
					
					curBody->inWater = (cellVal == E_CD_WATER);
					curBody->isInside = (cellVal == E_CD_SOLID);
					
					// push out from underground
					
					if (curBody->isInside) {
						
						
						// ge->moveToPoint(tempBTV + btVector3(0,0,1.0), bodInd);
						// ge->applyImpulse(btVector3(0,0,1.0),false, bodInd);
						//curBody->lastVel = curBody->body->getLinearVelocity();
					}
					
					
					if (
						(ge->orgId > -1) &&
						(ge->actorId > -1) &&
						(curBody->boneId > -1) &&
						(ge->bodies.size() > 0)
						&& (bodInd > 1)
						// && (curBody->boneId == E_BONE_C_SKULL)
						//false
					) {
						curActor = singleton->gameActors[ge->actorId];
						curOrg = singleton->gameOrgs[ge->orgId];
						curOrgNode = curOrg->allNodes[curBody->boneId];
						
						if (curOrgNode == NULL) {
							
						}
						else {
							
							
							// if (bodInd == 0) {
							// 	//basePos = curOrgNode->orgTrans[1].getBTV();
							// }
							// else {
								
							// }
							
							
							// if (bodInd == 0) {
							// 	basePos = btVector3(0.0,0.0,0.0);
							// }
							// else {
							// 	basePos = 
							// 		curOrgNode->orgTrans[1].getBTV() * 
							// 		ge->bodies[0].body->getWorldTransform().getBasis() +
							// 		ge->bodies[0].body->getWorldTransform().getOrigin();
							// }
							
							
							
							
							ge->bodies[0].body->getWorldTransform().getOpenGLMatrix(myMat);
							myMatrix4 = Matrix4(myMat);
							
							if (curBody->isBall) {
								tempBTV = curOrgNode->orgTrans[2].getBTV();
								
								
								
							// }
							// else {
							// 	tempBTV = curOrgNode->orgTrans[1].getBTV();
							// }
							
							
								myVector4 = Vector4(
									tempBTV.getX(),
									tempBTV.getY(),
									tempBTV.getZ(),
									1.0f
								);
								resVector4 = myMatrix4*myVector4;
								basePos = btVector3(resVector4.x,resVector4.y,resVector4.z);
								basePos += ge->skelOffset;
								
								// move limbs towards pose
								
								difVec = basePos - 
										(
											curBody->body->getCenterOfMassPosition()
											// - ge->bodies[0].body->getWorldTransform().getOrigin()
										);
										
								totVec += difVec;
								
								ge->applyImpulse(
									difVec*curStepTime*MASS_PER_LIMB*200.0f, // *MASS_PER_LIMB*2.0f*10.0f*curStepTime,
									false,
									bodInd
								);
							
							
							}
							//
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
						
						ge->applyImpulse(dirForce*curStepTime*60.0, false, bodInd);
					}
					
					
					//////////////////////
					// END APPLY FORCES
					//////////////////////
					
					
					
					
					
					nv0 = curBody->body->getLinearVelocity();
					nv1 = curBody->lastVel;
					
					if (nv0.isZero() || nv1.isZero()) {
						
					}
					else {
						nv0.normalize();
						nv1.normalize();
						
						
						if (
							(!(curBody->isInside)) &&
							(
								curBody->lastVel.length() > 0.5f
							) &&
							(
								(nv0.dot(nv1)) < 0.8f
							) &&
							(bodInd == 0)
							
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
				
				//ge->otherImpulses *= 0.4;
				
				ge->skelOffset *= 0.99;
				
				// ge->curGrav -= btVector3(0.0f, 0.0f, 0.01f);
				// if (ge->curGrav.getZ() < -10.0f) {
				// 	ge->curGrav = btVector3(0.0f,0.0f,-10.0f);
				// }
				
				if (ge->allFalling()) {
					
					//ge->curGrav -= btVector3(0.0f, 0.0f, 0.03f);
					
					//ge->curGrav = btVector3(0.0f,0.0f,-10.0f);
					
					//ge->bodies[0].body->setDamping(0.5f,0.9f);
				}
				else {
					
					//ge->bodies[0].body->setDamping(0.99f,0.9f);
					
					if (difVec.getZ() < 0.0f) {
						
						// ge->otherImpulses += 0.1;
						// ge->otherImpulses *= 2.0;
						
						//ge->bodies[0].body->setGravity(btVector3(0.0,0.0,-10.0));
						
						//ge->curGrav += btVector3(0.0f, 0.0f, 0.03f);
						
						
						ge->skelOffset += btVector3(
								0.0f,
								0.0f,
								-difVec.getZ()	
						);
						
						// ge->applyImpulse(
						// 	btVector3(
						// 		0.0f,
						// 		0.0f,
						// 		-difVec.getZ()*100.0f	
						// 	),
						// 	false,
						// 	0
						// );
					}
				}
				
				//ge->bodies[0].body->setGravity(ge->curGrav);
				
			}
			
		}
		
		singleton->explodeStack.clear();
		
	}
void GamePhysics::updateAll ()
                         {
		
		while (singleton->totTimePassedPhysics > STEP_TIME_IN_MICRO_SEC) {
			collideWithWorld(STEP_TIME_IN_MICRO_SEC/400000.0f);
			example->stepSimulation(STEP_TIME_IN_MICRO_SEC/400000.0f);
			singleton->totTimePassedPhysics -= STEP_TIME_IN_MICRO_SEC;
		}
		
		flushImpulses();
	}
GamePhysics::~ GamePhysics ()
                       {
		example->exitPhysics();
		delete example;
	}
#undef LZZ_INLINE
 
