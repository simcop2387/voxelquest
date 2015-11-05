
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
	
	GamePhysics() {
		
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
			singleton->cameraPos->getFX(),
			singleton->cameraPos->getFY(),
			singleton->cameraPos->getFZ()
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
	// 		singleton->cameraPos->getFX() + singleton->lookAtVec[0]*16.0,
	// 		singleton->cameraPos->getFY() + singleton->lookAtVec[1]*16.0,
	// 		singleton->cameraPos->getFZ() + singleton->lookAtVec[2]*16.0	
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
		btCapsuleShape* capsuleShape = new btCapsuleShapeZ(1.0f,1.0f);
		ge->body = example->createRigidBody(ge->mass,trans,capsuleShape);
		ge->body->bodyId = _uid;
		
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
		
		BaseObj* ge;
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (
				(ge->isHidden) ||
				(ge->body == NULL)
			) {
				
			}
			else {
				if (singleton->selObjInd == ge->uid) {
					
					ge->body->applyCentralImpulse( btVector3(
						0.0f,//(ge->body->getCenterOfMassPosition().getX() - (singleton->worldMarker.getFX()))*20.0f,
						0.0f,//(ge->body->getCenterOfMassPosition().getY() - (singleton->worldMarker.getFY()))*20.0f,
						200.0f//-(ge->body->getCenterOfMassPosition().getZ() - (4.0f + singleton->worldMarker.getFZ()))*200.0f
					) );
					
				}
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
	// 			singleton->cameraPos->getFX() + singleton->lookAtVec[0]*16.0,
	// 			singleton->cameraPos->getFY() + singleton->lookAtVec[1]*16.0,
	// 			singleton->cameraPos->getFZ() + singleton->lookAtVec[2]*16.0	
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
		// f32 time = g_clock.Start( );
		// updateBase(time);
		// g_clock.Stop( );
		
		collideWithWorld();
		
		example->stepSimulation(1.f/60.f);
	}
	
	
	
	~GamePhysics() {
		example->exitPhysics();
		delete example;
	}
	
	
};




