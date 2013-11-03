

class GamePageHolder {
public:

	int iPageDataVolume;
	int holderSizeInPages;
	int usingPoolId;
	int thisHolderId;

	FIVector4 offsetInHolders;
	FIVector4 trueOffsetInHolders;

	PooledResource* gpuRes;

	Singleton* singleton;

	std::vector<int> containsGeomIds;
	GamePage** pageData;

	GamePageHolder() {
		usingPoolId = -1;
	}

	void init(
		Singleton* _singleton,
		int _thisHolderId,
		
		int trueX,
		int trueY,
		int trueZ,

		int clampedX,
		int clampedY,
		int clampedZ

	) {

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

	void unbindGPUResources() {
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

	/*
	int getPageIndex(int x, int y, int z) {

		if (
			(x < 0) || (x >= holderSizeInPages) ||
			(y < 0) || (y >= holderSizeInPages) ||
			(z < 0) || (z >= holderSizeInPages)
		) {
			return -1;
		}
		else {

			return z*holderSizeInPages*holderSizeInPages + y*holderSizeInPages + x;
		}
	}
	*/


};



/*

possible borders:

street / bridge
alley
adjoining house
wall / gate
waterway


*/

