// f00375_gamephysics.h
//

#include "f00375_gamephysics.e"
#define LZZ_INLINE inline
GamePhysics::GamePhysics ()
                      {
		
	}
void GamePhysics::init (Singleton * _singleton)
        {
		
		singleton = _singleton;
		myOGLApp = new MyOGLApp("yo", 640, 480);
		guiHelper = new MyGLHelper(singleton, myOGLApp);
		example = 
			new BenchmarkDemo(guiHelper,5);
			// new BasicExample(guiHelper);
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
 
