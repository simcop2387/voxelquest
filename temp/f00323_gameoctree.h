// f00323_gameoctree.h
//

#include "f00323_gameoctree.e"
#define LZZ_INLINE inline
GameOctree::GameOctree ()
                     {
		
	}
void GameOctree::init (Singleton * _singleton, int _dimInVoxels, bool _hasTBO, int _maxSize, int _nodeSize)
          {
		singleton = _singleton;
		dimInVoxels = _dimInVoxels;
		hasTBO = _hasTBO;
		maxSize = _maxSize;
		nodeSize = _nodeSize;
		
		if (maxSize == -1) {
			maxSize = (128/4)*1024*1024;
		}
		if (nodeSize == -1) {
			nodeSize = 8;
		}
		
		maxDepth = intLogB2(dimInVoxels);
		
		data = new uint[maxSize];
		
		renderLevel = 0;
		nullPtr = 0;
		rootPtr = nodeSize;
		nextOpen = rootPtr+nodeSize;
		
		int i;
		
		for (i = 0; i < maxSize; i++) {
			data[i] = nullPtr;
		}
		
		if (hasTBO) {
			octTBO.init(false,NULL,data,maxSize*4);
		}
		
	}
void GameOctree::updateTBO ()
                         {
		octTBO.update(NULL, data, -1);
	}
void GameOctree::captureBuffer ()
                             {
		
		cout << "captureBuffer\n";

		FBOWrapper *fbow = singleton->getFBOWrapper("solidTargFBO", 0);
		fbow->getPixels();
		
		int i;
		
		int x;
		int y;
		int z;
		
		for (i = 0; i < fbow->numBytes; i += 4) {
			x = fbow->pixelsFloat[i+0];
			y = fbow->pixelsFloat[i+1];
			z = fbow->pixelsFloat[i+2];
			
			addNode(x,y,z,1);
		}

		cout << "newSize " << nextOpen << "\n";
		
	}
void GameOctree::modRenderLevel (int modVal)
                                        {
		renderLevel += modVal;
		if (renderLevel > maxDepth) {
			renderLevel = maxDepth;
		}
		if (renderLevel < 0) {
			renderLevel = 0;
		}
		
		cout << "renderLevel " << renderLevel << "\n";
	}
void GameOctree::addNode (int x, int y, int z, uint col)
                                                    {
		int curPtr = rootPtr;
		int curLevel = 0;
		bool doProc = true;
		
		int curX = x;
		int curY = y;
		int curZ = z;
		
		int subX;
		int subY;
		int subZ;
		
		int curDiv = dimInVoxels/2;
		
		int offset;
		
		if (nextOpen >= (maxSize-nodeSize)) {
			cout << "octree full\n";
			return;
		}
		
		do {
			subX = curX/curDiv;
			subY = curY/curDiv;
			subZ = curZ/curDiv;
			
			curX -= subX*curDiv;
			curY -= subY*curDiv;
			curZ -= subZ*curDiv;
			
			offset = subX + subY*2 + subZ*4;
			
			if (data[curPtr+offset] == nullPtr) {
				data[curPtr+offset] = nextOpen;
				nextOpen += nodeSize;
			}
			
			curPtr = data[curPtr+offset];
			
			curDiv = curDiv/2;
			
		} while (curDiv > 1);
		
		data[curPtr+0] = col;
		
		
	}
void GameOctree::remNode (uint index)
                                 {
		
	}
void GameOctree::startRender ()
                           {
		renderBB(0,0,0,rootPtr,0,dimInVoxels);
	}
void GameOctree::renderBB (int baseX, int baseY, int baseZ, int startIndex, int curLevel, int curDiv)
          {
		
		int i;
		int xm;
		int ym;
		int zm;
		
		if (curLevel > renderLevel) {
			return;
		}
		
		int curDiv2 = curDiv/2;
		
		singleton->drawBoxMinMax(
			btVector3(baseX,baseY,baseZ),
			btVector3(baseX+curDiv,baseY+curDiv,baseZ+curDiv)
		);
		
		for (i = 0; i < 8; i++) {
			zm = i/4;
			ym = (i-zm*4)/2;
			xm = (i-(zm*4 + ym*2));
			
			if (data[startIndex+i] == nullPtr) {
				
			}
			else {
				renderBB(
					baseX+xm*curDiv2,
					baseY+ym*curDiv2,
					baseZ+zm*curDiv2,
					data[startIndex+i],
					curLevel+1,
					curDiv2
				);
			}
		}
		
		
	}
#undef LZZ_INLINE
 
