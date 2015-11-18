
class GamePhysics {
public:
	Singleton* singleton;
	
	BenchmarkDemo* example;
	MyOGLApp* myOGLApp;
	GUIHelperInterface* guiHelper;
	
	GameActor* gameActor;
	btRigidBody* lastBodyPick;
	
	//unsigned long int stepTimeInMicroSec;
	
	GamePhysics() {
		lastBodyPick = NULL;
		gameActor = NULL;
		//8000; // ~120 times per second
	}
	
	void init(Singleton* _singleton)
	{
		
		cout << "GamePhysics:init()\n";
		
		singleton = _singleton;
		myOGLApp = new MyOGLApp("yo", 640, 480);
		guiHelper = new MyGLHelper(singleton, myOGLApp);
		example = new BenchmarkDemo(guiHelper,5);
		example->initPhysics();
		
	}
	
	void pickBody(btVector3 posWS1, btVector3 posWS2) {
		
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
		
		
		btVector3 begPos = singleton->cameraGetPosNoShake()->getBTV();
		btVector3 endPos;
		
		if (posWS1.distance(begPos) < posWS2.distance(begPos)) {
			endPos = posWS1;
		}
		else {
			endPos = posWS2;
		}
		
		
		
		
		// singleton->getRayTo(
		// 	singleton->lastMouseX,
		// 	singleton->lastMouseY
		// );
		
		lastBodyPick = example->bodyPick(begPos,endPos);
		
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
		
		int i;
		
		BaseObj* ge = &(singleton->gw->gameObjects[_uid]);
		
		if (ge->isHidden) {
			return;
		}
		
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(ge->startPoint);
		
		
		float objRad = 0.5f;
		
		if (
			(ge->entType == E_ENTTYPE_NPC) ||
			(ge->entType == E_ENTTYPE_MONSTER)	
		) {
			// btCapsuleShapeZ* capsuleShape = new btCapsuleShapeZ(1.0f,1.0f);
			// ge->body = example->createRigidBody(ge->mass,trans,capsuleShape);
			// ge->body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
			// ge->body->setLinearFactor(btVector3(0.0f,0.0f,0.0f));
			
			
			
			
			gameActor = new GameActor(
				singleton,
				example->getWorld(),
				ge->startPoint,
				false
			);
			for (i = 0; i < gameActor->actorJoints.size(); i++) {
				
				
				ge->bodies.push_back(BodyStruct());
				ge->bodies.back().body = gameActor->actorJoints[i].body;
				
				if (i == 0) {
					//ge->body = gameActor->actorJoints[i].body;
					//ge->body->setLinearFactor(btVector3(0.0f,0.0f,0.0f));
					ge->bodies.back().body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
				}
				else {
					
				}
			}
			
			
		}
		else {
			
			
			btBoxShape* boxShape = new btBoxShape(btVector3(objRad,objRad,objRad));
			ge->bodies.push_back(BodyStruct());
			ge->bodies.back().body = example->createRigidBody(MASS_PER_LIMB,trans,boxShape);
			
			if (ge->entType == E_ENTTYPE_DEBRIS) {
				
				ge->bodies[0].body->setAngularVelocity(btVector3(
					(fGenRand2()*2.0f-1.0f),
					(fGenRand2()*2.0f-1.0f),
					(fGenRand2()*2.0f-1.0f)	
				)*4.0f);
			}
			
			
			
			
			
		}
		
		int bodInd;
		for (bodInd = 0; bodInd < ge->bodies.size(); bodInd++) {
			ge->bodies[bodInd].body->bodyUID = _uid;
			ge->bodies[bodInd].body->limbUID = bodInd;
			ge->bodies[bodInd].body->setDamping(0.1f,0.99f);
			ge->bodies[bodInd].body->setContactProcessingThreshold(0.25f);
			
			
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
	

	
	void motorPreTickCallback(btScalar timeStep, GameActor* curActor) {
		
		if (curActor == NULL) {
			return;
		}
		
		curActor->stepSim(timeStep);
		
	}


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
		
		motorPreTickCallback(curStepTime, gameActor);
		
		
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
		
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (
				(ge->isHidden)
			) {
				
			}
			else {
				
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
						
						
						ge->moveToPoint(tempBTV + btVector3(0,0,1.0), bodInd);
						ge->applyImpulse(btVector3(0,0,1.0),false, bodInd);
						curBody->lastVel = curBody->body->getLinearVelocity();
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
								
								totMass = ge->getTotalMass();
								
								
								// ge->applyImpulse(
								// 	btVector3(
								// 		( singleton->worldMarker.getFX() - curBody->body->getCenterOfMassPosition().getX() ),
								// 		( singleton->worldMarker.getFY() - curBody->body->getCenterOfMassPosition().getY() ),
								// 		-(curBody->body->getCenterOfMassPosition().getZ() - (8.0f + singleton->worldMarker.getFZ()))
								// 	)*totMass*0.01f,
								// 	false,
								// 	bodInd
								// );
							}
						}
						
						
						
						
						
						
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
						
						ge->applyImpulse(dirForce, false, bodInd);
					}
					
					
					//////////////////////
					// END APPLY FORCES
					//////////////////////
					
					
					
					
					
					nv0 = curBody->body->getLinearVelocity();
					nv0.normalize();
					nv1 = curBody->lastVel;
					nv1.normalize();
					
					
					if (
						(!(curBody->isInside)) &&
						(
							curBody->lastVel.length() > 0.5f
						) &&
						(
							(nv0.dot(nv1)) < 0.8f
						)
						
					) {
						
						
						singleton->gw->fireEvent(
							ge->uid,
							EV_COLLISION,
							clampfZO( (curBody->lastVel.length()-0.5f)/16.0f )*
							(1.0f-clampfZO(ge->getCenterPointFIV(bodInd)->distance(singleton->cameraGetPosNoShake())/(50.0f)))
						);
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
			}
			
		}
		
		singleton->explodeStack.clear();
		
	}

	
	void updateAll() {
		
		while (singleton->totTimePassedPhysics > STEP_TIME_IN_MICRO_SEC) {
			collideWithWorld(STEP_TIME_IN_MICRO_SEC/400000.0f);
			example->stepSimulation(STEP_TIME_IN_MICRO_SEC/400000.0f);
			singleton->totTimePassedPhysics -= STEP_TIME_IN_MICRO_SEC;
		}
		
		flushImpulses();
	}
	
	
	
	~GamePhysics() {
		example->exitPhysics();
		delete example;
	}
	
	
};







