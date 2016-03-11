
class GameVoxelWrap {
public:
	Singleton* singleton;
	
	uint* vData;
	uint* nData;
	
	GameOctree* gameOct;
	
	int numNeighbors;
	int vDataSize;
	int nDataSize;
	
	int indexCount;
	int dimInVoxels;
	int octInVoxels;
	int maxDepth;
	
	int nullPtr;
	int rootPtr;
	int nodeSize;
	int nextOpen;
	int renderLevel;
	int maxVerts;
	int maxVertSize;
	int vertComponents;
	
	bool hasTBO;
	bool hasVBO;
	bool hasNeighbors;
	
	int curPD;
	
	PaddedDataEntry* baseData;
	
	int voxelsPerCell;
	int cellsPerHolder;
	int cellsPerHolderPad;
	int paddingInCells;
	
	VectorI3 offsetInCells;
	VectorI3 offsetInVoxels;
	
	//std::vector<uint> indexVec;
	std::vector<float> vertexVec;
	VBOWrapper vboWrapper;
	TBOWrapper octTBO;
	
	std::vector<VoxelSlice*> voxelSlices;
	std::vector<VoxelNode> nodeList;
	
	GameVoxelWrap() {
		
	}
	
	
	void init(
		Singleton* _singleton,
		int _dimInVoxels,
		bool _hasTBO,
		bool _hasVBO,
		bool _hasNeighbors
	) {
		singleton = _singleton;
		dimInVoxels = _dimInVoxels;
		octInVoxels = dimInVoxels*2;
		maxDepth = intLogB2(octInVoxels);
		hasTBO = _hasTBO;
		hasVBO = _hasVBO;
		hasNeighbors = _hasNeighbors;
		nodeSize = 8;
		numNeighbors = 6;
		vData = NULL;
		nData = NULL;
		renderLevel = 12;
		
		voxelsPerCell = singleton->voxelsPerCell;
		cellsPerHolder = singleton->cellsPerHolder;
		cellsPerHolderPad = singleton->cellsPerHolderPad;
		paddingInCells = singleton->paddingInCells;
		
		gameOct = new GameOctree();
		gameOct->init(singleton,);
		
	}
	
	void update(
		int _maxVerts	
	) {
		int i;
		
		
		indexCount = 0;
		maxVerts = _maxVerts;
		vertComponents = 2;
		vDataSize = maxVerts*nodeSize;
		nDataSize = maxVerts*numNeighbors;
		maxVertSize = maxVerts*vertComponents*4;
		
		vData = new uint[vDataSize];
		
		if (hasNeighbors) {
			nData = new uint[nDataSize];
		}
		else {
			nData = NULL;
		}
		
		nullPtr = 0;
		rootPtr = nodeSize;
		nextOpen = rootPtr+nodeSize;
		
		for (i = 0; i < vDataSize; i++) {
			vData[i] = nullPtr;
		}
		
		if (hasTBO) {
			octTBO.init(false,NULL,vData,vDataSize*4);
		}
		
		if (hasVBO) {
			vertexVec.clear();
			vertexVec.reserve(maxVertSize);
			
			//indexVec.clear();
			//indexVec.reserve(maxVerts);
			
			
		}
	}
	
	
	void updateVBO() {
		int i;
		
		if (!hasVBO) {
			return;
		}
		
		if (vertexVec.size() < 1) {
			return;
		}
		
		if (vboWrapper.hasInit) {
			vboWrapper.update(
				&(vertexVec[0]),
				vertexVec.size(),
				NULL,//&(indexVec[0]),
				0 //indexVec.size()
			);
		}
		else {
			vboWrapper.init(
				&(vertexVec[0]),
				vertexVec.size(),
				maxVertSize,
				NULL,//&(indexVec[0]),
				0,//indexVec.size()
				0,//maxVerts
				vertComponents,
				GL_STATIC_DRAW
			);
		}
		
	}
	
	void updateTBO() {
		if (!hasTBO) {
			return;
		}
		
		octTBO.update(NULL, vData, -1);
	}
	
	void captureBuffer(bool getPoints) {
		
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
	
	void modRenderLevel(int modVal) {
		renderLevel += modVal;
		if (renderLevel > maxDepth) {
			renderLevel = maxDepth;
		}
		if (renderLevel < 0) {
			renderLevel = 0;
		}
		
		cout << "renderLevel " << renderLevel << "\n";
	}
	
	bool addNode(int x, int y, int z, float r, float g, float b) {
		int curPtr = rootPtr;
		int curLevel = 0;
		bool doProc = true;
		
		int curX = x;
		int curY = y;
		int curZ = z;
		
		int subX;
		int subY;
		int subZ;
		
		int curDiv = octInVoxels/2;
		
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
	
	void remNode(uint index) {
		
	}
	
	
	void startRender() {
		renderBB(0,0,0,rootPtr,0,dimInVoxels);
	}
	
	void renderBB(
		int baseX,
		int baseY,
		int baseZ,
		int startIndex,
		int curLevel,
		int curDiv
	) {
		
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
	
	
	inline PaddedDataEntry* getPadData(int ii, int jj, int kk) {
		
		
		int i = ii + paddingInCells;
		int j = jj + paddingInCells;
		int k = kk + paddingInCells;
		
		return &(
			baseData[
				i + j*cellsPerHolderPad + k*cellsPerHolderPad*cellsPerHolderPad
			]	
		);
	}
	
	
	
	
	bool findNextCoord(VectorI3* voxResult) {
		int i;
		int j;
		int k;
		int q;
		int r;
		
		int ii;
		int jj;
		int kk;
		int ikk;
		
		VectorI3 curVoxel;
		VectorI3 localOffset;
		
		int cellsPerHolder = singleton->cellsPerHolder;
		int paddingInCells = singleton->paddingInCells;
		
		// int procFlags[6];
		// procFlags[0] = 1;
		// procFlags[1] = 2;
		// procFlags[2] = 4;
		// procFlags[3] = 8;
		// procFlags[4] = 16;
		// procFlags[5] = 32;
		// int procFlag = 0;
		
		int minv = 0;
		int maxv = cellsPerHolder;
		
		int cellData;
		int cellData2;
		int voxelsPerCellM1 = voxelsPerCell-1;
		
		bool foundCell = false;
		
		for (i = minv; i < maxv; i++) {
			for (j = minv; j < maxv; j++) {
				for (k = minv; k < maxv; k++) {
					
					if (getPadData(i,j,k)->visited) {
						
					}
					else {
						getPadData(i,j,k)->visited = true;						
						
						cellData = getPadData(i,j,k)->cellVal;
						
						if (cellData == E_CD_SOLID) {
							
							foundCell = false;
							
							for (q = 0; q < NUM_ORIENTATIONS; q++) {
								cellData2 = getPadData(
									i + DIR_VECS_I[q][0],
									j + DIR_VECS_I[q][1],
									k + DIR_VECS_I[q][2]
								)->cellVal;
								
								if (cellData2 != E_CD_SOLID) {
									foundCell = true;
									break;
								}
								
							}
						
							if (foundCell) {
								
								foundCell = true;
								
								localOffset.set(i,j,k);
								localOffset *= voxelsPerCell;
								
								for (kk = 0; kk < voxelsPerCell; kk++) {
									
									ikk = voxelsPerCellM1-kk;
									
									for (r = 0; r < 4; r++) {
										switch (r) {
											case 0:
												curVoxel.set(kk, kk, ikk);
											break;
											case 1:
												curVoxel.set(ikk, kk, ikk);
											break;
											case 2:
												curVoxel.set(kk, ikk, ikk);
											break;
											case 3:
												curVoxel.set(ikk, ikk, ikk);
											break;
										}
										
										curVoxel += offsetInVoxels;
										curVoxel += localOffset;
										if (isSurfaceVoxel(&curVoxel)) {
											voxResult->set(
												curVoxel.x, curVoxel.y, curVoxel.z
											);
											return true;
										}
										
									}
								}
								
								for (jj = 0; jj < voxelsPerCell; jj++) {
									for (ii = 0; ii < voxelsPerCell; ii++) {
										for (r = 0; r < 6; r++) {
											switch (r) {
												case 0:
													curVoxel.set(0, ii, jj);
												break;
												case 1:
													curVoxel.set(voxelsPerCellM1, ii, jj);
												break;
												case 2:
													curVoxel.set( ii, 0, jj );
												break;
												case 3:
													curVoxel.set( ii, voxelsPerCellM1, jj );
												break;
												case 4:
													curVoxel.set(ii,jj,0);
												break;
												case 5:
													curVoxel.set(ii,jj,voxelsPerCellM1);
												break;
											}
											
											curVoxel += offsetInVoxels;
											curVoxel += localOffset;
											if (isSurfaceVoxel(&curVoxel)) {
												voxResult->set(
													curVoxel.x, curVoxel.y, curVoxel.z
												);
												return true;
											}
											
									}
								}
								
								
								
							}
						
						}
					}
				}
			}
		}
		
		return false;
	}
	
	void process(GamePageHolder* gph) {
		VectorI3 cellCoord;
		VectorI3 voxResult;
		
		curPD = gph->curPD;
		
		offsetInCells.set(
			gph->gphMinInCells.getIX(),
			gph->gphMinInCells.getIY(),
			gph->gphMinInCells.getIZ()
		);
		
		offsetInVoxels = offsetInCells;
		offsetInVoxels *= voxelsPerCell;
		
		baseData = singleton->pdPool[curPD].data;
		
		
		while( findNextCoord(&voxResult) ) {
			floodFill(&voxResult);
		}
		
	}
	
	bool isSurfaceVoxel(VectorI3* voxCoord) {
		int q;
		VectorI3 tempVox;
		
		if (getVoxelAtCoord(voxCoord)) {
			
			for (q = 0; q < NUM_ORIENTATIONS; q++) {
				tempVox.x = DIR_VECS_I[q][0];
				tempVox.y = DIR_VECS_I[q][1];
				tempVox.z = DIR_VECS_I[q][2];
				
				tempVox += *voxCoord;
				
				if (getVoxelAtCoord(tempVox)) {
					return true;
				}
				
			}
		}
		return false;
	}
	
	bool getVoxelAtCoord(VectorI3* voxCoord) {
		return false;
	}
	
	
	
};







