
class PooledResource {
private:
	FBOSet* fboSet[MAX_LAYERS];
	uint* cpuSet[MAX_LAYERS];
public:

	intPair usedByHolderId;
	bool isCPU;
	Singleton* singleton;

	PooledResource() {
		int i;

		for (i = 0; i < MAX_LAYERS; i++) {
			fboSet[i] = NULL;
		}
		
	}

	FBOSet* getFBOS(int fboNum) {
		return fboSet[fboNum];
	}

	void init(Singleton* _singleton, bool _isCPU) {

		singleton = _singleton;
		isCPU = _isCPU;
		usedByHolderId.v0 = -1;
		usedByHolderId.v1 = -1;

		int i;

		if (isCPU) {
			for (i = 0; i < MAX_LAYERS; i++) {
				cpuSet[i] = new uint[
					(singleton->holderSizeInPixelsMod) *
					(singleton->holderSizeInPixelsMod)	
				];
			}
		}
		else {
			for (i = 0; i < MAX_LAYERS; i++) {
				fboSet[i] = new FBOSet();
			}

			for (i = 0; i < MAX_LAYERS; i++) {
				fboSet[i]->init(
					2,
					((singleton->holderSizeInPixelsMod)),
					((singleton->holderSizeInPixelsMod)),
					1,
					false //has depth
				);
			}
		}

		

		

	}
};
