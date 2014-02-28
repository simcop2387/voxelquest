
class PooledResource {
private:
	FBOSet* fboSet[MAX_LAYERS];

public:

	intPair usedByHolderId;

	
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

	void init(Singleton* _singleton) {

		singleton = _singleton;
		usedByHolderId.v0 = -1;
		usedByHolderId.v1 = -1;

		int i;

		for (i = 0; i < MAX_LAYERS; i++) {
			fboSet[i] = new FBOSet();
		}

		/*
		void init(
			int _numBufs,
			int _width,
			int _height,
			int _bytesPerChannel,
			bool _hasDepth,
			int filterEnum=GL_NEAREST,
			int clampEnum=GL_CLAMP_TO_EDGE
		)
		*/

		for (i = 0; i < MAX_LAYERS; i++) {
			fboSet[i]->init(
				2,
				((singleton->holderSizeInPixels)),
				((singleton->holderSizeInPixels)),
				1,
				false //has depth
			);
		}

		

	}
};
