// f00323_gameoctree.h
//

#include "f00323_gameoctree.e"
#define LZZ_INLINE inline
GameOctree::GameOctree ()
                     {
		//vData = NULL;
	}
GameOctree::~ GameOctree ()
                      {
		
	}
void GameOctree::init (Singleton * _singleton, int _dimInVoxels, int reserveSize)
          {
		singleton = _singleton;
		dimInVoxels = _dimInVoxels;
		// hasTBO = _hasTBO;
		// hasVBO = _hasVBO;
		//maxVerts = _maxVerts;
		//nodeSize = 8;
		//numNeighbors = 6;
		
		//nodeSize = _nodeSize;
		
		// if (maxSize == -1) {
		// 	maxSize = (128/4)*1024*1024;
		// }
		// if (nodeSize == -1) {
			
		// }
		
		//indexCount = 0;
		
		//vertComponents = 2;
		//vDataSize = maxVerts*nodeSize;
		//nDataSize = maxVerts*numNeighbors;
		
		maxDepth = intLogB2(dimInVoxels);
		
		//vData = new uint[vDataSize];
		
		
		
		
		//renderLevel = 12;
		//nullPtr = 0;
		//rootPtr = nodeSize;
		//nextOpen = rootPtr+nodeSize;
		
		int i;
		
		// for (i = 0; i < vDataSize; i++) {
		// 	vData[i] = nullPtr;
		// }
		
		// if (hasTBO) {
		// 	octTBO.init(false,NULL,vData,vDataSize*4);
		// }
		
		// if (hasVBO) {
		// 	//vertexVec.clear();
		// 	//vertexVec.reserve(maxVerts*vertComponents*4);
			
		// 	//indexVec.clear();
		// 	//indexVec.reserve(maxVerts);
			
			
		// }
		
		//octNodes.reserve(reserveSize);
		clearAllNodes();
		
		
	}
int GameOctree::allocNode (int parent)
                                  {
		octNodes.push_back(OctNode());
		int newInd = -1;
		newInd = octNodes.size()-1;
		octNodes[newInd].init(parent); //.back()
		return newInd;
	}
int GameOctree::getNode (VectorI3 * pos)
                                   {
		int curPtr = rootPtr;
		int curLevel = 0;
		bool doProc = true;
		
		VectorI3 curPos = *pos;
		VectorI3 subPos;
		int curDiv = dimInVoxels/2;
		
		int offset;
		
		bool wasNew = false;
		
		do {
			subPos = (curPos/curDiv);
			curPos -= (subPos*curDiv);
			
			offset = subPos.x + subPos.y*2 + subPos.z*4;
			
			if (octNodes[curPtr].children[offset] == -1) {
				return -1;
			}
			
			curPtr = octNodes[curPtr].children[offset];
			
			curDiv = curDiv/2;
			
		} while (curDiv >= 1);
		
		
		return curPtr;
	}
int GameOctree::addNode (VectorI3 * pos, bool & wasNew)
                                                 {
		int curPtr = rootPtr;
		int curLevel = 0;
		bool doProc = true;
		
		VectorI3 curPos = *pos;
		VectorI3 subPos;
		
		int curDiv = dimInVoxels/2;
		
		int offset;
		
		wasNew = false;
		
		do {
			subPos = (curPos/curDiv);
			curPos -= (subPos*curDiv);
			
			offset = subPos.x + subPos.y*2 + subPos.z*4;
			
			if (octNodes[curPtr].children[offset] == -1) {
				octNodes[curPtr].children[offset] = allocNode(curPtr);
				wasNew = true;
			}
			
			curPtr = octNodes[curPtr].children[offset];
			
			curDiv = curDiv/2;
			
		} while (curDiv >= 1);
		
		octNodes[curPtr].x = pos->x;
		octNodes[curPtr].y = pos->y;
		octNodes[curPtr].z = pos->z;
		
		return curPtr;//wasNew;
	}
void GameOctree::clearAllNodes ()
                             {
		octNodes.clear();
		rootPtr = allocNode(-1);
	}
#undef LZZ_INLINE
 
