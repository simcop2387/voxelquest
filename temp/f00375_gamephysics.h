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
			singleton->cameraPos->getFX(),
			singleton->cameraPos->getFY(),
			singleton->cameraPos->getFZ()
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
void GamePhysics::collideWithWorld ()
                                {
		
		
		
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
 
