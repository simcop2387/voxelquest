// f00375_gamephysics.h
//

#include "f00375_gamephysics.e"
#define LZZ_INLINE inline
GamePhysics::GamePhysics ()
                      {
		gameActor = NULL;
		ragDoll = NULL;
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
		
		if (ge->body != NULL) {
			example->removeRigidBody(ge->body);
			ge->body = NULL;
		}
		
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
		trans.setOrigin(ge->getCenterPoint(false)->getBTV());
		
		
		float objRad = 0.5f;
		
		if (
			(ge->entType == E_ENTTYPE_NPC) ||
			(ge->entType == E_ENTTYPE_MONSTER)	
		) {
			// btCapsuleShapeZ* capsuleShape = new btCapsuleShapeZ(1.0f,1.0f);
			// ge->body = example->createRigidBody(ge->mass,trans,capsuleShape);
			// ge->body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
			//ge->body->setLinearFactor(btVector3(0.0f,0.0f,0.0f));
			
			
			
			
			gameActor = new GameActor(
				example->getWorld(),
				ge->getCenterPoint(false)->getBTV(),
				false,
				_uid
			);
			for (i = 0; i < BODYPART_COUNT_GA; i++) {
				if (i == 0) {
					ge->body = gameActor->actorJoints[i].body;
					//ge->body->setLinearFactor(btVector3(0.0f,0.0f,0.0f));
					ge->body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
				}
				else {
					ge->limbs.push_back(gameActor->actorJoints[i].body);
				}
			}
			
			
			
			// ragDoll = new GameRagDoll(
			// 	example->getWorld(),
			// 	ge->getCenterPoint(false)->getBTV(),
			// 	4.0f,
			// 	_uid
			// );
			
			// for (i = 0; i < ragDoll->BODYPART_COUNT; i++) {
			// 	if (i == 0) {
			// 		ge->body = ragDoll->m_bodies[i];
			// 	}
			// 	else {
			// 		ge->limbs.push_back(ragDoll->m_bodies[i]);
			// 	}
				
			// }
			
		}
		else {
			
			
			btBoxShape* boxShape = new btBoxShape(btVector3(objRad,objRad,objRad));
			ge->body = example->createRigidBody(ge->mass,trans,boxShape);
			
			if (ge->entType == E_ENTTYPE_DEBRIS) {
				ge->body->setAngularVelocity(btVector3(
					(fGenRand2()*2.0f-1.0f),
					(fGenRand2()*2.0f-1.0f),
					(fGenRand2()*2.0f-1.0f)	
				)*4.0f);
			}
			
			
			
			
			
		}
		
		ge->body->bodyUID = _uid;
		ge->body->setDamping(0.1f,0.99f);
		ge->body->setContactProcessingThreshold(0.25f);
		
	}
void GamePhysics::motorPreTickCallback (btScalar timeStep, GameActor * curActor)
                                                                          {
		
		if (curActor == NULL) {
			return;
		}
		
		curActor->stepSim(timeStep);
		
	}
void GamePhysics::flushImpulses ()
                             {
		
		int k;
		BaseObj* ge;
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (
				(ge->isHidden) ||
				(ge->body == NULL)
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
		
		int cellVal;
		
		bool lastFalling;
		
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
		
		motorPreTickCallback(curStepTime, gameActor);
		
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (
				(ge->isHidden) ||
				(ge->body == NULL)
			) {
				
			}
			else {
				ge->hasContact = false;
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
			
			
			
			
			for (k = 0; k < 2; k++) {
				if (bodies[k]->bodyUID > -1) {
					ge = &(singleton->gw->gameObjects[bodies[k]->bodyUID]);
					
					
					
					if (
						(ge->isHidden) ||
						(ge->body == NULL)
					) {
						
					}
					else {
						
						if (hasContact) {
							ge->hasContact = true;
						}
						
						lastFalling = ge->isFalling;
						
						ge->isFalling = (!(ge->hasContact));// && (abs((float)(ge->body->getLinearVelocity().getZ())) > 4.0f);
						
						// if (!(ge->isFalling)) {
						// 	//ge->isJumping = false;
						// }
						
					}
					
				}
			}
			
		}
		
		
		
		
		float totForce;
		btVector3 dirForce;
		
		
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (
				(ge->isHidden) ||
				(ge->body == NULL)
			) {
				
			}
			else {
				
				
				//////////////////////
				// APPLY FORCES
				//////////////////////
				
				ge->applyImpulses(curStepTime);
				
				tempBTV = ge->body->getCenterOfMassPosition();
				
				cellVal = singleton->gw->getCellAtCoords(
					tempBTV.getX(),
					tempBTV.getY(),
					tempBTV.getZ()
				);
				
				
				ge->inWater = (cellVal == E_CD_WATER);
				ge->isInside = (cellVal == E_CD_SOLID);
				
				// push out from underground
				
				if (ge->isInside) {
					ge->moveToPoint(tempBTV + btVector3(0,0,2));
					
					ge->applyImpulse(btVector3(0,0,5),false);
					ge->lastVel = ge->body->getLinearVelocity();
				}
				
				if (
					(singleton->selObjInd == ge->uid) &&
					singleton->markerFound &&
					singleton->isDraggingObject &&
					(singleton->draggingFromType == E_DT_WORLD_OBJECT)
				) {
					
					
					ge->applyImpulse(
						btVector3(
							( singleton->worldMarker.getFX() - ge->body->getCenterOfMassPosition().getX() )*0.2f,
							( singleton->worldMarker.getFY() - ge->body->getCenterOfMassPosition().getY() )*0.2f,
							-(ge->body->getCenterOfMassPosition().getZ() - (8.0f + singleton->worldMarker.getFZ()))*0.2f
						),
						false
					);
					
					
					
				}
				
				
				
				for (m = 0; m < singleton->sphereStack.size(); m++) {
					tempBTV = ge->body->getCenterOfMassPosition();
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
					
					ge->applyImpulse(dirForce, false);
				}
				
				// for (m = 0; m < singleton->explodeStack.size(); m++) {
				// 	tempBTV = ge->body->getCenterOfMassPosition();
				// 	totForce = (
				// 		1.0f-clampfZO(
				// 			tempBTV.distance(singleton->explodeStack[m].pos)/singleton->explodeStack[m].radius	
				// 		)
				// 	)*singleton->explodeStack[m].power;
				// 	dirForce = tempBTV-singleton->explodeStack[m].pos;
				// 	dirForce.normalize();
				// 	dirForce = dirForce*totForce;
					
				// 	dirForce.setZ(totForce);
					
				// 	ge->applyImpulse(dirForce, false);
				// }
				
				
				//////////////////////
				// END APPLY FORCES
				//////////////////////
				
				
				
				
				
				nv0 = ge->body->getLinearVelocity();
				nv0.normalize();
				nv1 = ge->lastVel;
				nv1.normalize();
				
				
				if (
					(!(ge->isInside)) &&
					(
						ge->lastVel.length() > 0.5f
					) &&
					(
						(nv0.dot(nv1)) < 0.8f
					)
					
				) {
					
					singleton->gw->fireEvent(
						ge->uid,
						EV_COLLISION,
						clampfZO( (ge->lastVel.length()-0.5f)/16.0f )*
						(1.0f-clampfZO(ge->getCenterPoint()->distance(singleton->cameraGetPosNoShake())/(50.0f)))
					);
				}
				
				
				ge->lastVel = ge->body->getLinearVelocity();
				
				ge->getCenterPoint(true);
				
				
				if (ge->entType == E_ENTTYPE_BULLET) {
					if (
						(!(ge->isFalling)) && 
						(ge->body->getLinearVelocity().length() < 0.5)
					) {
							singleton->explodeBullet(ge);
					}
				}
				
				
			}
			
		}
		
		singleton->explodeStack.clear();
		
	}
void GamePhysics::updateAll ()
                         {
		
		while (singleton->totTimePassedPhysics > STEP_TIME_IN_MICRO_SEC) {
			collideWithWorld(STEP_TIME_IN_MICRO_SEC/500000.0f);
			example->stepSimulation(STEP_TIME_IN_MICRO_SEC/500000.0f);
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
 
