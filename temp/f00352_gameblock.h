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

		float uvSize = 256.0;

		for (i = 0; i < 8; i++) {
			gameGeom.push_back(new GameGeom());
			
			x = (offsetInBlocks.getFX()+fGenRand()) * singleton->blockSizeInPixels;
			y = (offsetInBlocks.getFY()+fGenRand()) * singleton->blockSizeInPixels;

			x = floor(x/uvSize)*uvSize;

			z = singleton->getHeightAtPixelPos(x,y);
			z = floor(z/uvSize)*uvSize;


			gameGeom.back()->initRand(i,x,y,z,uvSize);

		}
		


	}
#undef LZZ_INLINE
 
