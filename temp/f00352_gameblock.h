// f00352_gameblock.h
//

#include "f00352_gameblock.e"
#define LZZ_INLINE inline
GameBlock::GameBlock ()
                    {

	}
void GameBlock::init (Singleton * _singleton, int ind, int _x, int _y)
                                                                  {
		singleton = _singleton;
		thisIndex = ind;
		offsetInBlocks.setIXYZ(_x,_y,0);


		int i;

		float x;
		float y;
		float z;

		blockSizeInHolders = singleton->blockSizeInHolders;



		for (i = 0; i < 8; i++) {
			gameGeom.push_back(new GameGeom());
			
			x = (offsetInBlocks.getFX()+fGenRand()) * singleton->blockSizeInPixels;
			y = (offsetInBlocks.getFY()+fGenRand()) * singleton->blockSizeInPixels;
			z = singleton->getHeightAtPixelPos(x,y);
			gameGeom.back()->initRand(i,x,y,z);

		}
		


	}
#undef LZZ_INLINE
 
