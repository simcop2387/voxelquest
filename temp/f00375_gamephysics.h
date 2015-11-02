// f00375_gamephysics.h
//

#include "f00375_gamephysics.e"
#define LZZ_INLINE inline
GamePhysics::GamePhysics ()
                      {
		
	}
void GamePhysics::init (Singleton * _singleton)
        {
		// singleton = _singleton;
		// myShapeDrawer = new MyShapeDrawer(singleton);
		
		// CommonExampleOptions options(&noGfx);
		// example = new BasicExample(options.m_guiHelper);
		
		// example->initPhysics();
		
		
		
		singleton = _singleton;
		//myShapeDrawer = new MyShapeDrawer(singleton);
		//myShapeDrawer->init(singleton);
		
		//myDummyApp = new DummyApp("yo", 640, 480);
		
		myOGLApp = new MyOGLApp("yo", 640, 480);
		
		noGfx = new MyGLHelper(singleton, myOGLApp);
		
		//CommonExampleOptions options(noGfx);
		example = new BasicExample(noGfx);
		
		example->initPhysics();
		
		
	}
void GamePhysics::updateAll ()
                         {
		// f32 time = g_clock.Start( );
		// updateBase(time);
		// g_clock.Stop( );
		
		example->stepSimulation(1.f/60.f);
	}
GamePhysics::~ GamePhysics ()
                       {
		example->exitPhysics();
		delete example;
	}
#undef LZZ_INLINE
 
