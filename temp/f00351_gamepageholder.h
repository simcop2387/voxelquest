// f00351_gamepageholder.h
//

#include "f00351_gamepageholder.e"
#define LZZ_INLINE inline
GamePageHolder::GamePageHolder ()
                         {
		usingPoolId = -1;
	}
void GamePageHolder::init (Singleton * _singleton, int _thisHolderId, int trueX, int trueY, int trueZ, int clampedX, int clampedY, int clampedZ)
          {

		pushTrace("GamePageHolder init()");

		int i;

		singleton = _singleton;
		thisHolderId = _thisHolderId;
		usingPoolId = -1;

		trueOffsetInHolders.setIXYZ(trueX,trueY,trueZ);
		offsetInHolders.setIXYZ(clampedX,clampedY,clampedZ);

		

		holderSizeInPages = singleton->holderSizeInPages;
		iPageDataVolume = holderSizeInPages*holderSizeInPages*holderSizeInPages;
		pageData = new GamePage*[iPageDataVolume];

		for (i = 0; i < iPageDataVolume; i++) {
			pageData[i] = NULL;
		}



		popTrace();

	}
void GamePageHolder::unbindGPUResources ()
                                  {
		//pushTraceND("unbindGPUResources");

		usingPoolId = -1;
		gpuRes = NULL;

		singleton->gw->threadpool.joinAll();

		int i;
		for (i = 0; i < iPageDataVolume; i++) {
			if (pageData[i] == NULL) {

			}
			else {
				if (pageData[i]->curState >= E_STATE_GENERATEVOLUME_LAUNCH) {
					pageData[i]->curState = E_STATE_CREATESIMPLEXNOISE_END;
					pageData[i]->nextState = E_STATE_WAIT;
				}
			}
		}


		// TODO: clear color/depth buffers on release

		//popTraceND();

	}
#undef LZZ_INLINE
 
