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

		isDirty = false;

		singleton = _singleton;
		thisHolderId = _thisHolderId;
		usingPoolId = -1;

		trueOffsetInHolders.setIXYZ(trueX,trueY,trueZ);
		offsetInHolders.setIXYZ(clampedX,clampedY,clampedZ);
		offsetInBlocks.copyFrom(&offsetInHolders);
		offsetInBlocks.intDivXYZ(singleton->blockSizeInHolders);
		

		holderSizeInPages = singleton->holderSizeInPages;
		iPageDataVolume = holderSizeInPages*holderSizeInPages*holderSizeInPages;
		pageData = new GamePage*[iPageDataVolume];

		for (i = 0; i < iPageDataVolume; i++) {
			pageData[i] = NULL;
		}



		
		fetchGeom();



		popTrace();

	}
void GamePageHolder::refreshChildren ()
                               {
		int i;

		if (usingPoolId == -1) {
			usingPoolId = singleton->requestPoolId(thisHolderId);
			gpuRes = singleton->holderPoolItems[usingPoolId];
		}

		// clear fbo by binding it with auto flag
		singleton->bindFBODirect(gpuRes->fboSet);
		singleton->unbindFBO();

		for (i = 0; i < iPageDataVolume; i++) {
			if (pageData[i] == NULL) {

			}
			else {
				pageData[i]->generateVolume();
			}
		}
	}
void GamePageHolder::fetchGeom ()
                         {
		int i;
		int j;
		int k;
		int bufSize = (singleton->visPageSizeInPixels*singleton->bufferMult);
		
		GameBlock* curBlock;
		GamePageHolder* gph;
		FIVector4 start;
		FIVector4 end;
		GameGeom* geom;

		containsGeomIds.clear();

		for (i = -1; i <= 1; i++) {
			for (j = -1; j <= 1; j++) {
				curBlock = singleton->gw->getBlockAtCoords(
					offsetInBlocks.getIX()+i,
					offsetInBlocks.getIY()+j,
					true
				);

				for (k = 0; k < curBlock->gameGeom.size(); k++) {


					geom = curBlock->gameGeom[k];


					start.copyFrom( &(geom->boundsMinInPixels) );
					end.copyFrom( &(geom->boundsMaxInPixels) );

					start.addXYZ(-bufSize);
					end.addXYZ(bufSize);

					start.intDivXYZ(singleton->holderSizeInPixels);
					end.intDivXYZ(singleton->holderSizeInPixels);

					start.clampZ(&(singleton->origin),&(singleton->worldSizeInHoldersM1));
					end.clampZ(&(singleton->origin),&(singleton->worldSizeInHoldersM1));

					if (offsetInHolders.inBoundsEqualXYZ(&start,&end)) {
						containsGeomIds.push_back(intPair());
						containsGeomIds.back().v0 = curBlock->thisIndex;
						containsGeomIds.back().v1 = k;
					}


					

				}
			}			
		}
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
 
