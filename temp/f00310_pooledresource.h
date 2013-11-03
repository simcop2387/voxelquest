// f00310_pooledresource.h
//

#include "f00310_pooledresource.e"
#define LZZ_INLINE inline
PooledResource::PooledResource ()
                         {
		fboSet = NULL;
	}
void PooledResource::init (Singleton * _singleton)
                                         {

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
#undef LZZ_INLINE
 
