class GameChunk
{
public:

	Singleton *singleton;
	VBOWrapper vboWrapper[NUM_MIP_LEVELS_WITH_FIRST];

	int lastPointCount;
	int mipLev;
	int changeTick;
	
	bool readyToRender;
	bool listEmpty;
	bool changeFlag;
	
	int iHolderSize;
	int holdersPerChunk;
	GamePageHolder **holderData;
	
	FIVector4 offsetInChunks;
	FIVector4 chunkCenInCells;
	
	int chunkId;
	int blockId;

	GameChunk() {
		lastPointCount = 0;
		changeFlag = false;
		changeTick = 1;
		
		readyToRender = false;
		listEmpty = true;
		
		mipLev = 0;
		
	}

	void init(
		Singleton* _singleton,
		int _blockId,
		int _chunkId,
		
		int trueX,
		int trueY,
		int trueZ 
		
	) {
		singleton = _singleton;
		
		blockId = _blockId;
		chunkId = _chunkId;
		
		int i;
		
		float cellsPerChunk = singleton->cellsPerChunk;
		
		for (i = 0; i < NUM_MIP_LEVELS_WITH_FIRST; i++) {
			vboWrapper[i].setVI(&(singleton->chunkVI[i]),true);
		}
		
		holdersPerChunk = singleton->holdersPerChunk;
		
		offsetInChunks.setIXYZ(trueX,trueY,trueZ);
		chunkCenInCells.copyFrom(&offsetInChunks);
		chunkCenInCells.addXYZ(0.5f);
		chunkCenInCells.multXYZ(cellsPerChunk);
		
		iHolderSize = holdersPerChunk * holdersPerChunk * holdersPerChunk;
		holderData = new GamePageHolder*[iHolderSize];
		for (i = 0; i < iHolderSize; i++) {
			holderData[i] = NULL;
		}
		
	}

	VBOWrapper* getCurVBO() {
		return &(vboWrapper[mipLev]);
	}

	void drawLoadingHolders() {
		int i;
		GamePageHolder* curHolder;
		
		for (i = 0; i < iHolderSize; i++) {
			curHolder = holderData[i];
						
			if (curHolder == NULL) {
				
			}
			else {
				if (curHolder->lockWrite) {
					singleton->drawBox(&(curHolder->gphMinInCells),&(curHolder->gphMaxInCells));
				}
			}
		}
		
	}

	void checkHolders() {
		int i;
		
		FIVector4 testPos;
		
		float cellsPerChunk = singleton->cellsPerChunk;
		
		int testMip = -1;
		testPos.copyFrom(&offsetInChunks);
		testPos.addXYZ(0.5f);
		testPos.multXYZ(cellsPerChunk);
		
		float testDis = testPos.distance(singleton->cameraGetPosNoShake());
		
		testDis /= cellsPerChunk;
		
		for (i = 0; i < NUM_MIP_LEVELS_WITH_FIRST; i++) {
			if (
				(testDis >= singleton->mipDis[i]) &&
				(testDis < singleton->mipDis[i+1])
			) {
				testMip = i;
				break;
			}
		}
		
		if (testMip == -1) {
			testMip = NUM_MIP_LEVELS;
		}
		if (testMip > NUM_MIP_LEVELS) {
			testMip = NUM_MIP_LEVELS;
		}
		
		
		mipLev = testMip;
		
		int maxTicks = singleton->iGetConst(E_CONST_MAX_CHUNK_TICKS);
		
		
		if (singleton->updateHolders) {
			if (
				changeFlag
				// || (abs(testMip-mipLev) > 2) ||
				// (
				// 	(testMip == 0) &&
				// 	(mipLev != 0)	
				// ) ||
				// (
				// 	(testMip == NUM_MIP_LEVELS) &&
				// 	(mipLev != NUM_MIP_LEVELS)	
				// )
			) {
				changeTick++;
				
				if (((changeTick%maxTicks) == 0)) {
					changeFlag = false;
					fillVBO();
				}	
			}
		}
		
		
	}

	void reset() {
		int i;
		changeTick = 1;
		//changeCount = 0;
		for (i = 0; i < NUM_MIP_LEVELS_WITH_FIRST; i++) {
			vboWrapper[i].deallocVBO();
		}
	}
	
	void fillVBO() {
		
		//cout << "fillVBO a\n";
		
		readyToRender = false;
		
		int totFloats = 0;
		int i;
		int j;
		int k;
		
		GamePageHolder* curHolder;
		
		
		for (j = 0; j < NUM_MIP_LEVELS_WITH_FIRST; j++) {
			
			
			vboWrapper[j].beginFill();
			
			
			if (vboWrapper[j].hasInit) {
				
			}
			else {
				vboWrapper[j].init(
					2,
					GL_STATIC_DRAW
				);
			}
			
			
			for (i = 0; i < iHolderSize; i++) {
				curHolder = holderData[i];

				if (curHolder == NULL) {
					
				}
				else {
					if (curHolder->lockWrite) {
						
					}
					else {
						if (curHolder->listGenerated) {
							
							for (k = curHolder->begMip[j]; k < curHolder->endMip[j]; k++) {
								vboWrapper[j].vi->vertexVec.push_back(curHolder->vertexVec[k]);
							}
							
							// curHolder->vertexVec.clear();
							// curHolder->vertexVec.shrink_to_fit();
							
						}
					}
				}
			}

			if (j == 0) {
				listEmpty = (vboWrapper[j].vi->vertexVec.size() == 0);
				TOT_POINT_COUNT -= lastPointCount;
				TOT_POINT_COUNT += vboWrapper[j].getNumVerts();
				lastPointCount = vboWrapper[j].getNumVerts();
			}		
			
			
			vboWrapper[j].endFill();
			
			glFlush();
			glFinish();
			
			
			vboWrapper[j].clearVecs(false);
			
			
		}
		
		
		
		
		changeTick = 1;
		//changeCount = 0;
		
		readyToRender = true;
		
		//cout << "fillVBO b\n";
		
	}

};
