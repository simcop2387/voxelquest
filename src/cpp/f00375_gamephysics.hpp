
class GamePhysics {
public:
	Singleton* singleton;
	
	//MyShapeDrawer* myShapeDrawer;
	BenchmarkDemo* example;
	MyOGLApp* myOGLApp;
	GUIHelperInterface* guiHelper;
	
	//q3Scene* scene;
	//q3Body* body;
	
	//float dt;
	//float acc;
	//f32 accumulator;
	//Clock g_clock;
	
	
	unsigned long int stepTimeInMicroSec;
	
	GamePhysics() {
		stepTimeInMicroSec = 8000; // ~120 times per second
	}
	
	void init(Singleton* _singleton)
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
	
	
	void beginDrop() {
		
		cout << "GamePhysics:beginDrop()\n";
		
		example->beginDrop(
			singleton->cameraGetPosNoShake()->getFX(),
			singleton->cameraGetPosNoShake()->getFY(),
			singleton->cameraGetPosNoShake()->getFZ()
		);
	}
	
	
	
	// void init(Singleton* _singleton)
	// {
	// 	singleton = _singleton;
		
	// 	accumulator = 0;
	// 	dt = 1.0f / 60.0f;
		
	// 	q3Vec3 gravVec( r32( 0.0 ), r32( 0.0 ), r32( -9.8 ) );
		
	// 	scene = new q3Scene(dt);
	// 	scene->SetGravity(gravVec);
	// 	//scene->SetAllowSleep(false);
	// 	//scene->SetIterations(1);
		
		
	// 	acc = 0;

	// 	// Create the floor
	// 	// q3BodyDef bodyDef;
	// 	// q3Body* body = scene->CreateBody( bodyDef );
	// 	// q3BoxDef boxDef;
	// 	// boxDef.SetRestitution( 0 );
	// 	// q3Transform tx;
	// 	// q3Identity( tx );
	// 	// boxDef.Set( tx, q3Vec3( 50.0f, 50.0f, 1.0f ) );
	// 	// body->AddBox( boxDef );

	// 	// Create boxes
	// 	//for ( i32 i = 0; i < 10; ++i )
	// 	//{
	// 	//	bodyDef.position.Set( 0.0f, 1.2f * (i + 1), -0.0f );
	// 	//	//bodyDef.axis.Set( 0.0f, 1.0f, 0.0f );
	// 	//	//bodyDef.angle = q3PI * q3RandomFloat( -1.0f, 1.0f );
	// 	//	//bodyDef.angularVelocity.Set( 3.0f, 3.0f, 3.0f );
	// 	//	//bodyDef.linearVelocity.Set( 2.0f, 0.0f, 0.0f );
	// 	//	bodyDef.bodyType = eDynamicBody;
	// 	//	body = scene->CreateBody( bodyDef );
	// 	//	boxDef.Set( tx, q3Vec3( 1.0f, 1.0f, 1.0f ) );
	// 	//	body->AddBox( boxDef );
	// 	//}
	// }

	// void addRandFloor() {
		
	// 	q3BodyDef bodyDef;
	// 	bodyDef.position.Set(
	// 		singleton->cameraGetPosNoShake()->getFX() + singleton->lookAtVec[0]*16.0,
	// 		singleton->cameraGetPosNoShake()->getFY() + singleton->lookAtVec[1]*16.0,
	// 		singleton->cameraGetPosNoShake()->getFZ() + singleton->lookAtVec[2]*16.0	
	// 	);
	// 	q3Body* body = scene->CreateBody( bodyDef );
	// 	q3BoxDef boxDef;
	// 	boxDef.SetRestitution( 0 );
	// 	q3Transform tx;
	// 	q3Identity( tx );
	// 	boxDef.Set( tx, q3Vec3( 8.0f, 8.0f, 8.0f ) );
	// 	body->AddBox( boxDef );
	// }
	
	void remBoxFromObj(BaseObjType _uid) {
		
		BaseObj* ge = &(singleton->gw->gameObjects[_uid]);
		
		if (ge->body != NULL) {
			example->removeRigidBody(ge->body);
			ge->body = NULL;
		}
		
	}
	
	void addBoxFromObj(BaseObjType _uid) {
		
		BaseObj* ge = &(singleton->gw->gameObjects[_uid]);
		
		if (ge->isHidden) {
			return;
		}
		
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(ge->getCenterPoint(false)->getBTV());
		
		
		
		
		if (
			(ge->entType == E_ENTTYPE_NPC) ||
			(ge->entType == E_ENTTYPE_MONSTER)	
		) {
			btCapsuleShapeZ* capsuleShape = new btCapsuleShapeZ(1.0f,1.0f);
			ge->body = example->createRigidBody(ge->mass,trans,capsuleShape);
			ge->body->setAngularFactor(btVector3(0.0f,0.0f,0.0f));
		}
		else {
			btBoxShape* boxShape = new btBoxShape(btVector3(0.5f,0.5f,0.5f));
			ge->body = example->createRigidBody(ge->mass,trans,boxShape);
		}
		
		ge->body->setDamping(0.1f,0.99f);
		
		ge->body->bodyUID = _uid;
		
		ge->body->setContactProcessingThreshold(0.25f);
		
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
	


	void collideWithWorld() {
		
		
		
		int i;
		int j;
		int k;
		
		bool lastFalling;
		
		BaseObj* ge;
		
		FIVector4* curCenterPoint;
		btDiscreteDynamicsWorld* world = example->getWorld();
		
		btVector3 nv0;
		btVector3 nv1;
		
		bool hasContact = false;
		// bool isClose = false;
		// bool isFar = false;
		
		const btCollisionObject* bodies[2];
		
		int numManifolds = world->getDispatcher()->getNumManifolds();
		for (i=0;i<numManifolds;i++) {
			btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
			
			
			
			const btCollisionObject* obA = (contactManifold->getBody0());
			const btCollisionObject* obB = (contactManifold->getBody1());

			bodies[0] = obA;
			bodies[1] = obB;

			// isClose = false;
			// isFar = false;
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
				
				// if (pt.getDistance() > 0.2f) {
				// 	isFar = true;
				// }
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
						lastFalling = ge->isFalling;
						
						// if (isFar) {
						// 	ge->isFalling = true;
						// }
						
						// if (isClose) {
						// 	ge->isFalling = false;
						// }
						
						// if (hasContact) {
							
						// }
						// else {
							
						// }
						
						ge->isFalling = (!hasContact);// && (abs((float)(ge->body->getLinearVelocity().getZ())) > 4.0f);
						
						if (!(ge->isFalling)) {
							ge->isJumping = false;
						}
						
						// 	//&& (abs((float)(ge->body->getLinearVelocity().getZ())) > 4.0f)

						
						// if (ge->isFalling) {
							
						// }
						// else {
						// 	if (lastFalling != ge->isFalling) {
						// 		singleton->gw->fireEvent(ge->uid, EV_HIT_GROUND);
						// 	}
						// }
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
				
				
				nv0 = ge->body->getLinearVelocity();
				nv0.normalize();
				nv1 = ge->lastVel;
				nv1.normalize();
				
				
				
				
				if (
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
						clampfZO( (ge->lastVel.length()-0.5f)/16.0f )
					);
				}
				
				
				ge->lastVel = ge->body->getLinearVelocity();
				
				if (
					(singleton->selObjInd == ge->uid) &&
					singleton->markerFound &&
					singleton->isDraggingObject &&
					(singleton->draggingFromType == E_DT_WORLD_OBJECT)
				) {
					
					
					ge->applyImpulse( btVector3(
						( singleton->worldMarker.getFX() - ge->body->getCenterOfMassPosition().getX() )*0.25f,
						( singleton->worldMarker.getFY() - ge->body->getCenterOfMassPosition().getY() )*0.25f,
						-(ge->body->getCenterOfMassPosition().getZ() - (8.0f + singleton->worldMarker.getFZ()))*1.0f
					) );
					
				}
				
				ge->getCenterPoint(true);
				
			}
			
		}
		
	}

	// void update( )
	// {
		
		
	// 	collideWithWorld();
		
		
	// 	// if (singleton->lbDown) {
			
	// 	// }
	// 	// else {
	// 		return;
	// 	//}
		
	// 	acc += dt;

	// 	if ( acc > 0.125f ) {
	// 		acc = 0;
			
			
	// 		//addRandFloor();
			

	// 		q3BodyDef bodyDef;
	// 		bodyDef.position.Set(
	// 			singleton->cameraGetPosNoShake()->getFX() + singleton->lookAtVec[0]*16.0,
	// 			singleton->cameraGetPosNoShake()->getFY() + singleton->lookAtVec[1]*16.0,
	// 			singleton->cameraGetPosNoShake()->getFZ() + singleton->lookAtVec[2]*16.0	
	// 		);
	// 		bodyDef.axis.Set( q3RandomFloat( -1.0f, 1.0f ), q3RandomFloat( -1.0f, 1.0f ), q3RandomFloat( -1.0f, 1.0f ) );
	// 		bodyDef.angle = q3PI * q3RandomFloat( -1.0f, 1.0f );
	// 		bodyDef.bodyType = eDynamicBody;
	// 		bodyDef.angularVelocity.Set( q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ) );
	// 		bodyDef.angularVelocity *= q3Sign( q3RandomFloat( -1.0f, 1.0f ) );
	// 		bodyDef.linearVelocity.Set( q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ) );
	// 		bodyDef.linearVelocity *= q3Sign( q3RandomFloat( -1.0f, 1.0f ) );
	// 		q3Body* body = scene->CreateBody( bodyDef );

	// 		q3Transform tx;
	// 		q3Identity( tx );
	// 		q3BoxDef boxDef;
	// 		boxDef.Set( tx, q3Vec3( 1.0f, 1.0f, 1.0f ) );
	// 		body->AddBox( boxDef );
	// 	}
		
	// 	// q3Body* body = scene->m_bodyList;

	// 	// while ( body )
	// 	// {
	// 	// 	body->Render( render );
	// 	// 	body = body->m_next;
	// 	// }
		
		
	// 	/*
	// 	rayCast.Init( q3Vec3( 3.0f, 5.0f, 3.0f ), q3Vec3( -1.0f, -1.0f, -1.0f ) );
	// 	scene.RayCast( &rayCast, rayCast.data );

	// 	if ( rayCast.impactBody )
	// 	{
	// 		rayCast.impactBody->SetToAwake( );
	// 		rayCast.impactBody->ApplyForceAtWorldPoint( rayCast.data.dir * 20.0f, rayCast.data.GetImpactPoint( ) );
	// 	}
	// 	*/
		
		
	// }

	// void shutdown( )
	// {
	// 	scene->RemoveAllBodies();
	// }
	
	// void drawAll() {
	// 	scene->Render(&q3Rend);
	// }
	
	// void updateBase( f32 time )
	// {
	// 	// The time accumulator is used to allow the application to render at
	// 	// a frequency different from the constant frequency the physics sim-
	// 	// ulation is running at (default 60Hz).
		
	// 	accumulator += time;

	// 	accumulator = q3Clamp01( accumulator );
	// 	while ( accumulator >= dt )
	// 	{
			
	// 		scene->Step();
	// 		update();
			
	// 		// if ( !paused )
	// 		// {
	// 		// 	scene.Step( );
	// 		// 	demos[ currentDemo ]->Update( );
	// 		// }

	// 		// else
	// 		// {
	// 		// 	if ( singleStep )
	// 		// 	{
	// 		// 		scene.Step( );
	// 		// 		demos[ currentDemo ]->Update( );
	// 		// 		singleStep = false;
	// 		// 	}
	// 		// }

	// 		accumulator -= dt;
	// 	}
	// }
	
	void updateAll() {
		
		while (singleton->totTimePassedPhysics > stepTimeInMicroSec) {
			collideWithWorld();
			example->stepSimulation(stepTimeInMicroSec/500000.0f);
			singleton->totTimePassedPhysics -= stepTimeInMicroSec;
		}
	}
	
	
	
	~GamePhysics() {
		example->exitPhysics();
		delete example;
	}
	
	
};




