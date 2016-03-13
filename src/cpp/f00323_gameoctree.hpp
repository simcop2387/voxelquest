
class GameOctree {
public:
	Singleton* singleton;
	
	int rootPtr;
	
	vector<OctNode> octNodes;
	
	int dimInVoxels;
	int maxDepth;
	
	//uint* vData;
	//uint* nData;
	
	//int numNeighbors;
	//int vDataSize;
	//int nDataSize;
	
	//int indexCount;
	
	
	//int nullPtr;
	//int rootPtr;
	//int nodeSize;
	//int nextOpen;
	//int renderLevel;
	//int maxVerts;
	//int vertComponents;
	
	//bool hasTBO;
	//bool hasVBO;
	
	
	//std::vector<uint> indexVec;
	//std::vector<float> vertexVec;
	//VBOWrapper vboWrapper;
	//TBOWrapper octTBO;
	
	GameOctree() {
		//vData = NULL;
	}
	
	~GameOctree() {
		
	}
	
	void init(
		Singleton* _singleton,
		int _dimInVoxels,
		int reserveSize
		// bool _hasTBO,
		// bool _hasVBO,
		// int _maxVerts
		//int _maxSize = -1,
		//int _nodeSize = -1
	) {
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
	
	int allocNode(int parent) {
		octNodes.push_back(OctNode());
		int newInd = -1;
		newInd = octNodes.size()-1;
		octNodes[newInd].init(parent); //.back()
		return newInd;
	}
	
	
	// void updateVBO() {
	// 	if (!hasVBO) {
	// 		return;
	// 	}
		
	// 	if (vertexVec.size() < 1) {
	// 		return;
	// 	}
		
	// 	if (vboWrapper.hasInit) {
	// 		vboWrapper.update(
	// 			&(vertexVec[0]),
	// 			vertexVec.size(),
	// 			NULL,//&(indexVec[0]),
	// 			0 //indexVec.size()
	// 		);
	// 	}
	// 	else {
	// 		vboWrapper.init(
	// 			&(vertexVec[0]),
	// 			vertexVec.size(),
	// 			maxVerts*vertComponents*4,
	// 			NULL,//&(indexVec[0]),
	// 			0,//indexVec.size()
	// 			0,//maxVerts
	// 			vertComponents,
	// 			GL_STATIC_DRAW
	// 		);
	// 	}
		
	// }
	
	// void updateTBO() {
	// 	if (!hasTBO) {
	// 		return;
	// 	}
		
	// 	octTBO.update(NULL, vData, -1);
	// }
	
	// void captureBuffer(bool getPoints) {
		
	// 	cout << "captureBuffer\n";

	// 	FBOWrapper *fbow = singleton->getFBOWrapper("solidTargFBO", 0);
	// 	fbow->getPixels();
		
	// 	FBOWrapper *fbow2 = singleton->getFBOWrapper("resultFBO0", 0);
	// 	fbow2->getPixels();
		
	// 	int i;
		
	// 	int x;
	// 	int y;
	// 	int z;
		
	// 	float r, g, b;
		
	// 	btVector3 myPoint;
	// 	btVector3 camPoint = singleton->cameraGetPosNoShake()->getBTV();
		
	// 	float maxDis = singleton->clipDist[1]-50.0f;
		
	// 	bool didFail = false;
	// 	bool wasNew = false;
		
	// 	for (i = 0; i < fbow->numBytes; i += 4) {
	// 		x = fbow->pixelsFloat[i+0];
	// 		y = fbow->pixelsFloat[i+1];
	// 		z = fbow->pixelsFloat[i+2];
			
	// 		r = fbow2->pixelsChar[i+0];
	// 		g = fbow2->pixelsChar[i+1];
	// 		b = fbow2->pixelsChar[i+2];
			
	// 		r /= 255.0f;
	// 		g /= 255.0f;
	// 		b /= 255.0f;
			
			
	// 		myPoint = btVector3(x,y,z);
			
	// 		if (nextOpen >= (vDataSize-nodeSize)) {
	// 			didFail = true;
	// 			break;
	// 		}
			
	// 		if (camPoint.distance(myPoint) < maxDis) {
	// 			wasNew = addNode(x,y,z,r,g,b);
				
	// 			if (getPoints&&wasNew) {
	// 				vertexVec.push_back(x);
	// 				vertexVec.push_back(y);
	// 				vertexVec.push_back(z);
	// 				vertexVec.push_back(1.0f);
					
	// 				vertexVec.push_back(r);
	// 				vertexVec.push_back(g);
	// 				vertexVec.push_back(b);
	// 				vertexVec.push_back(0.0f);
					
	// 				//indexVec.push_back(indexCount);
	// 				//indexCount++;
					
	// 			}
				
	// 		}
			
	// 	}
		
	// 	if (didFail) {
	// 		cout << "octree full\n";
			
	// 		// todo: wrap back to start of buffer and overwrite?
	// 		// wont work - would leave invalid pointers
	// 		// instead, keep linear list of inserted points and reform octree
	// 		// wrap this linear list
			
	// 	}

	// 	cout << "points " << vertexVec.size()/8 << "\n";
		
	// }
	
	// void modRenderLevel(int modVal) {
	// 	renderLevel += modVal;
	// 	if (renderLevel > maxDepth) {
	// 		renderLevel = maxDepth;
	// 	}
	// 	if (renderLevel < 0) {
	// 		renderLevel = 0;
	// 	}
		
	// 	cout << "renderLevel " << renderLevel << "\n";
	// }
	
	
	int getNode(VectorI3* pos) {
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
	
	int addNode(VectorI3* pos, bool &wasNew) {
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
	
	void clearAllNodes() {
		octNodes.clear();
		rootPtr = allocNode(-1);
	}
	
	// void remNode(uint index) {
		
	// }
	
	
	// void startRender() {
	// 	renderBB(0,0,0,rootPtr,0,dimInVoxels);
	// }
	
	// void renderBB(
	// 	int baseX,
	// 	int baseY,
	// 	int baseZ,
	// 	int startIndex,
	// 	int curLevel,
	// 	int curDiv
	// ) {
		
	// 	int i;
	// 	int xm;
	// 	int ym;
	// 	int zm;
		
	// 	if (curLevel > renderLevel) {
	// 		return;
	// 	}
		
	// 	int curDiv2 = curDiv/2;
		
	// 	if (curLevel == renderLevel) {
	// 		singleton->drawBoxMinMax(
	// 			btVector3(baseX,baseY,baseZ),
	// 			btVector3(baseX+curDiv,baseY+curDiv,baseZ+curDiv)
	// 		);
	// 	}
		
		
		
	// 	for (i = 0; i < 8; i++) {
	// 		zm = i/4;
	// 		ym = (i-zm*4)/2;
	// 		xm = (i-(zm*4 + ym*2));
			
	// 		if (vData[startIndex+i] == nullPtr) {
				
	// 		}
	// 		else {
	// 			renderBB(
	// 				baseX+xm*curDiv2,
	// 				baseY+ym*curDiv2,
	// 				baseZ+zm*curDiv2,
	// 				vData[startIndex+i],
	// 				curLevel+1,
	// 				curDiv2
	// 			);
	// 		}
	// 	}
		
		
	// }
	
	
};







