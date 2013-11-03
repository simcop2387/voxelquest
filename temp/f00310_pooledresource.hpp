
class PooledResource {
public:

	int usedByHolderId;

	FBOSet* fboSet;
	Singleton* singleton;

	PooledResource() {
		fboSet = NULL;
	}

	void init(Singleton* _singleton) {

		singleton = _singleton;
		usedByHolderId = -1;

		fboSet = new FBOSet();

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
		fboSet->init(
			2,
			((singleton->holderSizeInPixels)*2),
			((singleton->holderSizeInPixels)*2),
			1,
			true
		);

	}
};
