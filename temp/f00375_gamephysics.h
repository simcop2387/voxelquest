// f00375_gamephysics.h
//

#include "f00375_gamephysics.e"
#define LZZ_INLINE inline
GamePhysics::GamePhysics ()
                      {
		
	}
void GamePhysics::init (Singleton * _singleton)
        {
		
		cout << "GamePhysics:init()\n";
		
		singleton = _singleton;
		myOGLApp = new MyOGLApp("yo", 640, 480);
		guiHelper = new MyGLHelper(singleton, myOGLApp);
		example = 
			new BenchmarkDemo(guiHelper,5);
			// new BasicExample(guiHelper);
		
		
		example->initPhysics();
		
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
		
		BaseObj* ge = &(singleton->gw->gameObjects[_uid]);
		
		if (ge->isHidden) {
			return;
		}
		
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(ge->getCenterPoint(false)->getBTV());
		btCapsuleShape* capsuleShape = new btCapsuleShapeZ(1.0f,1.0f);
		ge->body = example->createRigidBody(ge->mass,trans,capsuleShape);
		ge->body->bodyUID = _uid;
		
		// q3BodyDef bodyDef;
		// bodyDef.position.Set(
		// 	ge->getCenterPoint()->getFX(),
		// 	ge->getCenterPoint()->getFY(),
		// 	ge->getCenterPoint()->getFZ()	
		// );
		
		// if (ge->isUpright) {
		// 	bodyDef.lockAxisX = true;
		// 	bodyDef.lockAxisY = true;
		// 	bodyDef.lockAxisZ = true;
		// }
		
		// bodyDef.bodyType = eDynamicBody;
		
		// if (ge->body != NULL) {
		// 	scene->RemoveBody(ge->body);
		// 	ge->body = NULL;
		// }
		
		// ge->body = scene->CreateBody( bodyDef );

		// q3Transform tx;
		// q3Identity( tx );
		// q3BoxDef boxDef;
		// boxDef.Set( tx, q3Vec3(
		// 	ge->diameterInCells.getFX(),
		// 	ge->diameterInCells.getFY(),
		// 	ge->diameterInCells.getFZ()
		// ) );
		// boxDef.SetRestitution(ge->bounciness);
		// ge->body->AddBox( boxDef );
		
		
	}
void GamePhysics::collideWithWorld ()
                                {
		
		
		
		int i;
		int j;
		int k;
		
		bool lastFalling;
		
		BaseObj* ge;
		
		FIVector4* curCenterPoint;
		btDiscreteDynamicsWorld* world = example->getWorld();
		
		
		bool hasContact = false;
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
				if (pt.getDistance() < 1.0f) {
					
					hasContact = true;
					
					// const btVector3& ptA = pt.getPositionWorldOnA();
					// const btVector3& ptB = pt.getPositionWorldOnB();
					// const btVector3& normalOnB = pt.m_normalWorldOnB;
				}
			}
			
			
			// if (hasContact) {
				
				
				
			// }
			
			
			for (k = 0; k < 2; k++) {
				if (bodies[k]->bodyUID > -1) {
					ge = &(singleton->gw->gameObjects[bodies[k]->bodyUID]);
					
					
					if (
						(ge->isHidden) ||
						(ge->body == NULL)
					) {
						
					}
					else {
						lastFalling = ge->isFalling;
						
						ge->isFalling = 
							(!hasContact)
							//&& (abs((float)(ge->body->getLinearVelocity().getZ())) > 4.0f)
							;
						
						if (ge->isFalling) {
							
						}
						else {
							if (lastFalling != ge->isFalling) {
								singleton->gw->fireEvent(ge->uid, EV_HIT_GROUND);
							}
						}
					}
					
					
					
				}
			}
			
		}
		
		
		
		
		
		
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (
				(ge->isHidden) ||
				(ge->body == NULL)
			) {
				
			}
			else {
				
				//lastFalling = ge->isFalling;
				// ge->isFalling = (abs((float)(ge->body->getLinearVelocity().getZ())) > 4.0f);
				// if (ge->isFalling) {
				
				// }
				// else {
				// 	if (lastFalling != ge->isFalling) {
				// 		singleton->gw->fireEvent(ge->uid, EV_HIT_GROUND);
				// 	}
				// }
				
				if (
					(singleton->selObjInd == ge->uid) &&
					singleton->markerFound &&
					singleton->isDraggingObject
				) {
					
					
					ge->applyImpulse( btVector3(
						( singleton->worldMarker.getFX() - ge->body->getCenterOfMassPosition().getX() )*0.25f,
						( singleton->worldMarker.getFY() - ge->body->getCenterOfMassPosition().getY() )*0.25f,
						-(ge->body->getCenterOfMassPosition().getZ() - (4.0f + singleton->worldMarker.getFZ()))*2.0f
					) );
					
				}
			}
			
		}
		
	}
void GamePhysics::updateAll ()
                         {
		// f32 time = g_clock.Start( );
		// updateBase(time);
		// g_clock.Stop( );
		
		collideWithWorld();
		
		example->stepSimulation(1.f/60.f);
	}
GamePhysics::~ GamePhysics ()
                       {
		example->exitPhysics();
		delete example;
	}
#undef LZZ_INLINE
 
