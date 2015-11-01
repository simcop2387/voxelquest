
class GamePhysics {
public:
	Singleton* singleton;
	q3Scene* scene;
	//q3Body* body;
	
	float dt;
	float acc;
	f32 accumulator;
	Clock g_clock;
	
	GamePhysics() {
		
	}
	
	//virtual void Render( q3Render *debugDrawer ) { (void)debugDrawer; }
	
	void init(Singleton* _singleton)
	{
		singleton = _singleton;
		
		accumulator = 0;
		dt = 1.0f / 60.0f;
		
		q3Vec3 gravVec( r32( 0.0 ), r32( 0.0 ), r32( -9.8 ) );
		
		scene = new q3Scene(dt);
		scene->SetGravity(gravVec);
		//scene->SetAllowSleep(false);
		//scene->SetIterations(1);
		
		
		acc = 0;

		// Create the floor
		// q3BodyDef bodyDef;
		// q3Body* body = scene->CreateBody( bodyDef );
		// q3BoxDef boxDef;
		// boxDef.SetRestitution( 0 );
		// q3Transform tx;
		// q3Identity( tx );
		// boxDef.Set( tx, q3Vec3( 50.0f, 50.0f, 1.0f ) );
		// body->AddBox( boxDef );

		// Create boxes
		//for ( i32 i = 0; i < 10; ++i )
		//{
		//	bodyDef.position.Set( 0.0f, 1.2f * (i + 1), -0.0f );
		//	//bodyDef.axis.Set( 0.0f, 1.0f, 0.0f );
		//	//bodyDef.angle = q3PI * q3RandomFloat( -1.0f, 1.0f );
		//	//bodyDef.angularVelocity.Set( 3.0f, 3.0f, 3.0f );
		//	//bodyDef.linearVelocity.Set( 2.0f, 0.0f, 0.0f );
		//	bodyDef.bodyType = eDynamicBody;
		//	body = scene->CreateBody( bodyDef );
		//	boxDef.Set( tx, q3Vec3( 1.0f, 1.0f, 1.0f ) );
		//	body->AddBox( boxDef );
		//}
	}

	void addRandFloor() {
		
		q3BodyDef bodyDef;
		bodyDef.position.Set(
			singleton->cameraPos->getFX() + singleton->lookAtVec[0]*16.0,
			singleton->cameraPos->getFY() + singleton->lookAtVec[1]*16.0,
			singleton->cameraPos->getFZ() + singleton->lookAtVec[2]*16.0	
		);
		q3Body* body = scene->CreateBody( bodyDef );
		q3BoxDef boxDef;
		boxDef.SetRestitution( 0 );
		q3Transform tx;
		q3Identity( tx );
		boxDef.Set( tx, q3Vec3( 8.0f, 8.0f, 8.0f ) );
		body->AddBox( boxDef );
	}
	
	
	
	void addBoxFromObj(BaseObjType _uid) {
		
		BaseObj* ge = &(singleton->gw->gameObjects[_uid]);
		
		if (ge->isHidden) {
			return;
		}
		
		
		q3BodyDef bodyDef;
		bodyDef.position.Set(
			ge->getCenterPoint()->getFX(),
			ge->getCenterPoint()->getFY(),
			ge->getCenterPoint()->getFZ()	
		);
		
		if (ge->isUpright) {
			bodyDef.lockAxisX = true;
			bodyDef.lockAxisY = true;
			bodyDef.lockAxisZ = true;
		}
		
		
		//bodyDef.axis.Set( q3RandomFloat( -1.0f, 1.0f ), q3RandomFloat( -1.0f, 1.0f ), q3RandomFloat( -1.0f, 1.0f ) );
		//bodyDef.angle = q3PI * q3RandomFloat( -1.0f, 1.0f );
		bodyDef.bodyType = eDynamicBody;
		//bodyDef.angularVelocity.Set( q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ) );
		//bodyDef.angularVelocity *= q3Sign( q3RandomFloat( -1.0f, 1.0f ) );
		//bodyDef.linearVelocity.Set( q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ) );
		//bodyDef.linearVelocity *= q3Sign( q3RandomFloat( -1.0f, 1.0f ) );
		
		if (ge->body != NULL) {
			scene->RemoveBody(ge->body);
			ge->body = NULL;
		}
		
		ge->body = scene->CreateBody( bodyDef );

		q3Transform tx;
		q3Identity( tx );
		q3BoxDef boxDef;
		boxDef.Set( tx, q3Vec3(
			ge->diameterInCells.getFX(),
			ge->diameterInCells.getFY(),
			ge->diameterInCells.getFZ()
		) );
		boxDef.SetRestitution(ge->bounciness);
		ge->body->AddBox( boxDef );
		
		
	}
	


	void collideWithWorld() {
		
		q3Body* body = scene->m_bodyList;
		
		q3Box* box;
		
		
		int i;
		int j;
		int k;
		
		q3Vec3 corners[8];

		int cellVal[2];
		bool didHit[2];
		float avgRest[2];
		float totHits[2];
		
		BaseObj* ge;
		
		
		for(k = 0; k < singleton->gw->visObjects.size(); k++) {
			ge = &(singleton->gw->gameObjects[singleton->gw->visObjects[k]]);
			
			if (ge->isHidden) {
				
			}
			else {
				
				body = ge->body;
				
				if (body != NULL) {
					
					ge->updateTargets();
					
					box = body->m_boxes;
					
					for (j = 0; j < 2; j++) {
						avgRest[j] = 0.0f;
						totHits[j] = 0.0f;
						didHit[j] = false;
					}
					
					
					while ( box )
					{
						box->GetCorners(body->m_tx,corners);
						
						for (i = 0; i < 8; i++) {
							
							for (j = 0; j < 2; j++) {
								cellVal[j] = singleton->gw->getCellAtCoords(
									(int)(corners[i].x),
									(int)(corners[i].y),
									((int)(corners[i].z)) + j
								);
								if (cellVal[j] == E_CD_SOLID) {
									didHit[j] = true;
									avgRest[j] += box->restitution;
									totHits[j] += 1.0;
								}
							}
							
						}
						
						box = box->next;
					}
					
					for (j = 0; j < 2; j++) {
						if (totHits[j] > 0.0f) {
							avgRest[j] /= totHits[j];
						}
					}
					
					
					if (didHit[1]) {
						// back up to last good position
						// todo: handle case with no last position
						
						body->SetTransform(body->lastPos, body->lastQuat);
						body->m_linearVelocity = body->m_linearVelocity*(-1.0f)*avgRest[1];
					}
					else {
						
						if (didHit[0]) {
							
							if (ge->isInside) {
								body->m_linearVelocity.z = 0.0f;
							}
							else {
								if (ge->isFalling) {
									body->SetTransformZ(
										body->lastPos.z
									);
									body->m_linearVelocity.z = body->m_linearVelocity.z*(-1.0f)*avgRest[0];
								}
								
								if (abs(body->m_linearVelocity.z) < 0.1f) {
									if (ge->isFalling) {
										ge->isFalling = false;
										singleton->gw->fireEvent(ge->uid, EV_HIT_GROUND);
									}
									
								}
								
								
								ge->isInside = true;
								
							}
							
							
						}
						else {
							ge->isInside = false;
							
							// set last good position
							
							body->lastPos = body->m_worldCenter;
							body->lastQuat = body->m_q;
						}
						
						if (ge->isInside) {
							
							body->SetTransformZ(
								body->m_worldCenter.z + 0.5f
							);
						}
						
						
					}
					
					
					if (ge->isFalling) {
						body->m_linearVelocity.x *= ge->windResistance;
						body->m_linearVelocity.y *= ge->windResistance;
					}
					else {
						body->m_linearVelocity.x *= ge->friction;
						body->m_linearVelocity.y *= ge->friction;
					}
					
					
					ge->setCenterPointXYZ(
						body->m_worldCenter.x,
						body->m_worldCenter.y,
						body->m_worldCenter.z
						
					);
					
					
					
					
					
				}
			}
			
		}
		

		// while ( body )
		// {
		// 	if ( body->isStatic() ) {
				
		// 	}
		// 	else {}
			
		// 	body = body->m_next;
		// }
	}

	void update( )
	{
		
		
		collideWithWorld();
		
		
		// if (singleton->lbDown) {
			
		// }
		// else {
			return;
		//}
		
		acc += dt;

		if ( acc > 0.125f ) {
			acc = 0;
			
			
			//addRandFloor();
			

			q3BodyDef bodyDef;
			bodyDef.position.Set(
				singleton->cameraPos->getFX() + singleton->lookAtVec[0]*16.0,
				singleton->cameraPos->getFY() + singleton->lookAtVec[1]*16.0,
				singleton->cameraPos->getFZ() + singleton->lookAtVec[2]*16.0	
			);
			bodyDef.axis.Set( q3RandomFloat( -1.0f, 1.0f ), q3RandomFloat( -1.0f, 1.0f ), q3RandomFloat( -1.0f, 1.0f ) );
			bodyDef.angle = q3PI * q3RandomFloat( -1.0f, 1.0f );
			bodyDef.bodyType = eDynamicBody;
			bodyDef.angularVelocity.Set( q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ) );
			bodyDef.angularVelocity *= q3Sign( q3RandomFloat( -1.0f, 1.0f ) );
			bodyDef.linearVelocity.Set( q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ), q3RandomFloat( 1.0f, 3.0f ) );
			bodyDef.linearVelocity *= q3Sign( q3RandomFloat( -1.0f, 1.0f ) );
			q3Body* body = scene->CreateBody( bodyDef );

			q3Transform tx;
			q3Identity( tx );
			q3BoxDef boxDef;
			boxDef.Set( tx, q3Vec3( 1.0f, 1.0f, 1.0f ) );
			body->AddBox( boxDef );
		}
		
		// q3Body* body = scene->m_bodyList;

		// while ( body )
		// {
		// 	body->Render( render );
		// 	body = body->m_next;
		// }
		
		
		/*
		rayCast.Init( q3Vec3( 3.0f, 5.0f, 3.0f ), q3Vec3( -1.0f, -1.0f, -1.0f ) );
		scene.RayCast( &rayCast, rayCast.data );

		if ( rayCast.impactBody )
		{
			rayCast.impactBody->SetToAwake( );
			rayCast.impactBody->ApplyForceAtWorldPoint( rayCast.data.dir * 20.0f, rayCast.data.GetImpactPoint( ) );
		}
		*/
		
		
	}

	void shutdown( )
	{
		scene->RemoveAllBodies();
	}
	
	void drawAll() {
		scene->Render(&q3Rend);
	}
	
	void updateBase( f32 time )
	{
		// The time accumulator is used to allow the application to render at
		// a frequency different from the constant frequency the physics sim-
		// ulation is running at (default 60Hz).
		
		accumulator += time;

		accumulator = q3Clamp01( accumulator );
		while ( accumulator >= dt )
		{
			
			scene->Step();
			update();
			
			// if ( !paused )
			// {
			// 	scene.Step( );
			// 	demos[ currentDemo ]->Update( );
			// }

			// else
			// {
			// 	if ( singleStep )
			// 	{
			// 		scene.Step( );
			// 		demos[ currentDemo ]->Update( );
			// 		singleStep = false;
			// 	}
			// }

			accumulator -= dt;
		}
	}
	
	void updateAll() {
		f32 time = g_clock.Start( );
		updateBase(time);
		g_clock.Stop( );
	}
	
	
	
	void bulletTest() {
		MyGUIHelper noGfx;

		CommonExampleOptions options(&noGfx);
		CommonExampleInterface*    example = BasicExampleCreateFunc(options);
		
		example->initPhysics();
		example->stepSimulation(1.f/60.f);
		example->exitPhysics();

		delete example;
	}
	
	
};




