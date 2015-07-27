

class GamePageHolder {
public:

	
	int blockId;
	int holderId;
	
	int* cellData;
	int* extrData;
	
	
	std::vector<GameEnt *> entityGeom;
	int entityGeomCounter;
	FIVector4 offsetInHolders;

	FIVector4 gphMinInPixels;
	FIVector4 gphMaxInPixels;
	FIVector4 gphCenInPixels;

	FIVector4 offsetInBlocks;
	FIVector4 origOffset;
	FIVector4 tempVec;
	FIVector4 tempVec2;


	Singleton* singleton;

	intPairVec containsEntIds[E_ET_LENGTH];
	
	float holderSizeInPixels;
	float halfHolderSizeInPixels;

	bool wasGenerated;

	GamePageHolder() {
		cellData = NULL;
		extrData = NULL;
		wasGenerated = false;
	}
	

	void init(
		Singleton* _singleton,
		int _blockId, 			// MUST BE UNIQUE FOR ENTITES
		int _holderId,
		
		int trueX,
		int trueY,
		int trueZ

	) {

		
		entityGeomCounter = 0;

		int i;
		int j;
		int k;
		int ind;
		
		


		blockId = _blockId;
		holderId = _holderId;

		singleton = _singleton;


		holderSizeInPixels = singleton->holderSizeInPixels;
		halfHolderSizeInPixels = holderSizeInPixels*0.5f;
		
		origOffset.setFXYZ(halfHolderSizeInPixels,halfHolderSizeInPixels,halfHolderSizeInPixels);

		offsetInHolders.setIXYZ(trueX,trueY,trueZ);
		offsetInBlocks.copyFrom(&offsetInHolders);
		offsetInBlocks.intDivXYZ(singleton->blockSizeInHolders);
		
		gphMinInPixels.copyFrom(&offsetInHolders);
		gphMaxInPixels.copyFrom(&offsetInHolders);

		gphMaxInPixels.addXYZ(1);

		gphMinInPixels.multXYZ(singleton->holderSizeInPixels);
		gphMaxInPixels.multXYZ(singleton->holderSizeInPixels);

		gphCenInPixels.copyFrom(&gphMaxInPixels);
		gphCenInPixels.addXYZRef(&gphMinInPixels);
		gphCenInPixels.multXYZ(0.5f);
		

		
		
		fetchGeom();
		
	}

	void genCellData() {
		int i;
		int j;
		int k;
		
		int q;
		
		int ind;
		
		float fi;
		float fj;
		float fk;
		
		float zv;
		
		float fSimp[4];
		int iSimp[4];
		
		int cellsPerHolder = singleton->cellsPerHolder;
		int cellDataSize = cellsPerHolder*cellsPerHolder*cellsPerHolder*4;
		
		if (cellData == NULL) {
			cellData = new int[cellDataSize];
		}
		
		if (extrData == NULL) {
			extrData = new int[cellDataSize];
		}
		
		for (i = 0; i < cellDataSize; i++) {
			cellData[i] = FLUID_UNIT_MIN;
			extrData[i] = FLUID_UNIT_MIN;
		}
		
		for (j = 0; j < cellsPerHolder; j++) {
			fj = (j + gphMinInPixels[1]);
			for (i = 0; i < cellsPerHolder; i++) {
				fi = (i + gphMinInPixels[0]);
				
				// - singleton->worldSizeInPixels[0]*0.5f
				
				// zv = simplexScaledNoise(
				// 		4.0f, //octaves
				// 		0.5f, //persistence (amount added in each successive generation)
				// 		1.0f/16.0f, //scale (frequency)
				// 		0.0f, // lo bound
				// 		1.0f, // hi bound
				// 		fi + 2333.2,
				// 		fj + 1352.4,
				// 		4222.3
				// 	);
				
				for (k = 0; k < cellsPerHolder; k++) {
					fk = k + gphMinInPixels[2];
					
					
					// land
					fSimp[0] = clampfZO(
						simplexScaledNoise(
							5.0f, //octaves
							0.5f, //persistence (amount added in each successive generation)
							1.0f/128.0f, //scale (frequency)
							0.0f, // lo bound
							1.0f, // hi bound
							fi,
							fj,
							fk
						) // - (zv*fk*0.5f/fVSP) + (1.0-fk/fVSP)*0.25
						
					);
					
					// water
					// fSimp[1] = clampfZO(
					// 	simplexScaledNoise(
					// 		4.0f, //octaves
					// 		0.5f, //persistence (amount added in each successive generation)
					// 		1.0f/16.0f, //scale (frequency)
					// 		0.0f, // lo bound
					// 		1.0f, // hi bound
					// 		fi+124.0f,
					// 		fj+23.0f,
					// 		fk+53.0f
					// 	)// - (zv*fk*0.5f/fVSP) + (1.0-fk/fVSP)*0.5
					// );
					
					// if (fk/fVSP > 0.9) {
					// 	fSimp[0] = 0.0;
					// }
					
					// if (fk/fVSP > 0.5) {
					// 	fSimp[1] = 0.0;
					// }
					
					
					if (fSimp[0] > 0.5) {
						iSimp[0] = FLUID_UNIT_MAX;
					}
					else {
						iSimp[0] = FLUID_UNIT_MIN;
					}
					
					// if (fSimp[1] > 0.75) {
					// 	iSimp[1] = FLUID_UNIT_MAX;
					// }
					// else {
					// iSimp[1] = FLUID_UNIT_MIN;
					//}
					
					ind = (i + j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder)*4;
					
					for (q = 0; q < 4; q++) {
						cellData[ind+q] = FLUID_UNIT_MIN;
						extrData[ind+q] = FLUID_UNIT_MIN;
					}
					
					
					cellData[ind+0] = iSimp[0];
					// cellData[ind+1] = FLUID_UNIT_MIN;//iSimp[1];
					// cellData[ind+2] = FLUID_UNIT_MIN;//iSimp[1];
					// cellData[ind+3] = FLUID_UNIT_MIN;
		
		
				}
			}
		}
		
		
		wasGenerated = true;
	}

	
	void fetchGeom() {
		int i;
		int j;
		int k;
		int n;
		
		GameBlock* curBlock;
		GamePageHolder* gph;
		FIVector4 start;
		FIVector4 end;
		GameEnt* gameEnt;


		for (n = 0; n < E_ET_LENGTH; n++) {
			containsEntIds[n].data.clear();
			
			for (i = -1; i <= 1; i++) {
				for (j = -1; j <= 1; j++) {
					curBlock = singleton->gw->getBlockAtCoords(
						offsetInBlocks.getIX()+i,
						offsetInBlocks.getIY()+j,
						true
					);

					for (k = 0; k < curBlock->gameEnts[n].data.size(); k++) {

						gameEnt = &(curBlock->gameEnts[n].data[k]);

						start.copyFrom( &(gameEnt->moveMinInPixels) );
						end.copyFrom( &(gameEnt->moveMaxInPixels) );

						start.clampZ(0.0,singleton->worldSizeInPixels.getFZ()-1.0f);
						end.clampZ(0.0,singleton->worldSizeInPixels.getFZ()-1.0f);

						if (FIVector4::intersectInt(&start,&end,&gphMinInPixels,&gphMaxInPixels)) {
							containsEntIds[n].data.push_back(intPair());
							containsEntIds[n].data.back().v0 = curBlock->blockId;
							containsEntIds[n].data.back().v1 = k;
						}
					}
				}
			}
		}


		
	}


};

