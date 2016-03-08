// f00324_gamevoxelwrap.h
//

#include "f00324_gamevoxelwrap.e"
#define LZZ_INLINE inline
GameVoxelWrap::GameVoxelWrap ()
                        {
		
	}
void GameVoxelWrap::init (Singleton * _singleton, int _dimInVoxels, bool _hasTBO, bool _hasVBO, bool _hasNeighbors, int _maxVerts)
          {
		singleton = _singleton;
		dimInVoxels = _dimInVoxels;
		hasTBO = _hasTBO;
		hasVBO = _hasVBO;
		hasNeighbors = _hasNeighbors;
		maxVerts = _maxVerts;
		nodeSize = 8;
		numNeighbors = 6;
		
		//nodeSize = _nodeSize;
		
		// if (maxSize == -1) {
		// 	maxSize = (128/4)*1024*1024;
		// }
		// if (nodeSize == -1) {
			
		// }
		
		indexCount = 0;
		
		vertComponents = 2;
		vDataSize = maxVerts*nodeSize;
		nDataSize = maxVerts*numNeighbors;
		
		maxDepth = intLogB2(dimInVoxels);
		
		vData = new uint[vDataSize];
		
		if (hasNeighbors) {
			nData = new uint[nDataSize];
		}
		else {
			nData = NULL;
		}
		
		
		renderLevel = 12;
		nullPtr = 0;
		rootPtr = nodeSize;
		nextOpen = rootPtr+nodeSize;
		
		int i;
		
		for (i = 0; i < vDataSize; i++) {
			vData[i] = nullPtr;
		}
		
		if (hasTBO) {
			octTBO.init(false,NULL,vData,vDataSize*4);
		}
		
		if (hasVBO) {
			vertexVec.clear();
			vertexVec.reserve(maxVerts*vertComponents*4);
			
			//indexVec.clear();
			//indexVec.reserve(maxVerts);
			
			vboWrapper.init(
				&(vertexVec[0]),
				vertexVec.size()*vertComponents*4,
				maxVerts*vertComponents*4,
				NULL,//&(indexVec[0]),
				0,//indexVec.size()
				0,//maxVerts
				vertComponents,
				GL_STATIC_DRAW
			);
		}
		
	}
void GameVoxelWrap::updateVBO ()
                         {
		if (!hasVBO) {
			return;
		}
		
		vboWrapper.update(
			&(vertexVec[0]),
			vertexVec.size()*vertComponents*4,
			NULL,//&(indexVec[0]),
			0 //indexVec.size()
		);
	}
void GameVoxelWrap::updateTBO ()
                         {
		if (!hasTBO) {
			return;
		}
		
		octTBO.update(NULL, vData, -1);
	}
void GameVoxelWrap::captureBuffer (bool getPoints)
                                           {
		
		cout << "captureBuffer\n";

		FBOWrapper *fbow = singleton->getFBOWrapper("solidTargFBO", 0);
		fbow->getPixels();
		
		FBOWrapper *fbow2 = singleton->getFBOWrapper("resultFBO0", 0);
		fbow2->getPixels();
		
		int i;
		
		int x;
		int y;
		int z;
		
		float r, g, b;
		
		btVector3 myPoint;
		btVector3 camPoint = singleton->cameraGetPosNoShake()->getBTV();
		
		float maxDis = singleton->clipDist[1]-50.0f;
		
		bool didFail = false;
		bool wasNew = false;
		
		for (i = 0; i < fbow->numBytes; i += 4) {
			x = fbow->pixelsFloat[i+0];
			y = fbow->pixelsFloat[i+1];
			z = fbow->pixelsFloat[i+2];
			
			r = fbow2->pixelsChar[i+0];
			g = fbow2->pixelsChar[i+1];
			b = fbow2->pixelsChar[i+2];
			
			r /= 255.0f;
			g /= 255.0f;
			b /= 255.0f;
			
			
			myPoint = btVector3(x,y,z);
			
			if (nextOpen >= (vDataSize-nodeSize)) {
				didFail = true;
				break;
			}
			
			if (camPoint.distance(myPoint) < maxDis) {
				wasNew = addNode(x,y,z,r,g,b);
				
				if (getPoints&&wasNew) {
					vertexVec.push_back(x);
					vertexVec.push_back(y);
					vertexVec.push_back(z);
					vertexVec.push_back(1.0f);
					
					vertexVec.push_back(r);
					vertexVec.push_back(g);
					vertexVec.push_back(b);
					vertexVec.push_back(0.0f);
					
					//indexVec.push_back(indexCount);
					//indexCount++;
					
				}
				
			}
			
		}
		
		if (didFail) {
			cout << "octree full\n";
			
			// todo: wrap back to start of buffer and overwrite?
			// wont work - would leave invalid pointers
			// instead, keep linear list of inserted points and reform octree
			// wrap this linear list
			
		}

		cout << "points " << vertexVec.size()/8 << "\n";
		
	}
void GameVoxelWrap::modRenderLevel (int modVal)
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
bool GameVoxelWrap::addNode (int x, int y, int z, float r, float g, float b)
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
		
		bool wasNew = false;
		
		do {
			subX = curX/curDiv;
			subY = curY/curDiv;
			subZ = curZ/curDiv;
			
			curX -= subX*curDiv;
			curY -= subY*curDiv;
			curZ -= subZ*curDiv;
			
			offset = subX + subY*2 + subZ*4;
			
			if (vData[curPtr+offset] == nullPtr) {
				vData[curPtr+offset] = nextOpen;
				nextOpen += nodeSize;
				wasNew = true;
			}
			
			curPtr = vData[curPtr+offset];
			
			curDiv = curDiv/2;
			
		} while (curDiv > 4);
		
		vData[curPtr+0] = 1;
		
		return wasNew;
	}
void GameVoxelWrap::remNode (uint index)
                                 {
		
	}
void GameVoxelWrap::startRender ()
                           {
		renderBB(0,0,0,rootPtr,0,dimInVoxels);
	}
void GameVoxelWrap::renderBB (int baseX, int baseY, int baseZ, int startIndex, int curLevel, int curDiv)
          {
		
		int i;
		int xm;
		int ym;
		int zm;
		
		if (curLevel > renderLevel) {
			return;
		}
		
		int curDiv2 = curDiv/2;
		
		if (curLevel == renderLevel) {
			singleton->drawBoxMinMax(
				btVector3(baseX,baseY,baseZ),
				btVector3(baseX+curDiv,baseY+curDiv,baseZ+curDiv)
			);
		}
		
		
		
		for (i = 0; i < 8; i++) {
			zm = i/4;
			ym = (i-zm*4)/2;
			xm = (i-(zm*4 + ym*2));
			
			if (vData[startIndex+i] == nullPtr) {
				
			}
			else {
				renderBB(
					baseX+xm*curDiv2,
					baseY+ym*curDiv2,
					baseZ+zm*curDiv2,
					vData[startIndex+i],
					curLevel+1,
					curDiv2
				);
			}
		}
		
		
	}
#undef LZZ_INLINE
 
