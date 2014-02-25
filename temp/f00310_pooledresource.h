// f00310_pooledresource.h
//

#include "f00310_pooledresource.e"
#define LZZ_INLINE inline
PooledResource::PooledResource ()
                         {
		int i;

		for (i = 0; i < MAX_LAYERS; i++) {
			fboSet[i] = NULL;
		}
		
	}
FBOSet * PooledResource::getFBOS (int fboNum)
                                    {
		return fboSet[fboNum];
	}
void PooledResource::init (Singleton * _singleton)
                                         {

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
				true
			);
		}

		

	}
#undef LZZ_INLINE
 
