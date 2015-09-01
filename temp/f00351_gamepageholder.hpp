

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
	
	float cellsPerHolder;
	float halfCellsPerHolder;

	bool wasGenerated;
	bool justGenerated;

	GamePageHolder() {
		cellData = NULL;
		extrData = NULL;
		wasGenerated = false;
		justGenerated = false;
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


		cellsPerHolder = singleton->cellsPerHolder;
		halfCellsPerHolder = cellsPerHolder*0.5f;
		
		origOffset.setFXYZ(halfCellsPerHolder,halfCellsPerHolder,halfCellsPerHolder);

		offsetInHolders.setIXYZ(trueX,trueY,trueZ);
		offsetInBlocks.copyFrom(&offsetInHolders);
		offsetInBlocks.intDivXYZ(singleton->blockSizeInHolders);
		
		gphMinInPixels.copyFrom(&offsetInHolders);
		gphMaxInPixels.copyFrom(&offsetInHolders);

		gphMaxInPixels.addXYZ(1);

		gphMinInPixels.multXYZ(singleton->cellsPerHolder);
		gphMaxInPixels.multXYZ(singleton->cellsPerHolder);

		gphCenInPixels.averageXYZ(&gphMaxInPixels,&gphMinInPixels);
		

		
		
		fetchGeom();
		
	}

	void genCellData() {
		
		//cout << "genCellData()\n";
		
		int i;
		int j;
		int k;
		
		int q;
		
		int ind;
		
		float fi;
		float fj;
		float fk;
		
		float zv;
		
		float fiAbs;
		float fjAbs;
		float fkAbs;
		
		float terHeight;
		float simplexVal;
		float simplexVal1;
		float simplexVal2;
		
		float disVal;
		
		float fSimp;
		int iSimp;
		int iSimp2;
		
		int cellsPerHolder = singleton->cellsPerHolder;
		int cellDataSize = cellsPerHolder*cellsPerHolder*cellsPerHolder*4;
		
		
		float wspX = singleton->worldSizeInPixels[0]/2.0f;
		float wspY = singleton->worldSizeInPixels[1]/2.0f;
		
		float distanceBelowTer;
		
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
		
		float watHeight = singleton->getSLInPixels();
		
		float simplexRegion = 200.0f;
		float simplexMod = 0.0f;
		
		for (j = 0; j < cellsPerHolder; j++) {
			fj = abs(j + gphMinInPixels[1]);
			fjAbs = abs(wspY-fj);
			for (i = 0; i < cellsPerHolder; i++) {
				fi = abs(i + gphMinInPixels[0]);
				fiAbs = abs(wspX-fi);
				
				
				
				/*
				for x=0,bufferwidth-1,1 do
					for y=0,bufferheight-1,1 do
						s=x/bufferwidth
						t=y/bufferheight
				    dx=x2-x1
				    dy=y2-y1
						
						nx=x1+cos(s*2*pi)*dx/(2*pi)
						ny=y1+cos(t*2*pi)*dy/(2*pi)
						nz=x1+sin(s*2*pi)*dx/(2*pi)
						nw=y1+sin(t*2*pi)*dy/(2*pi)
						
						buffer:set(x,y,Noise4D(nx,ny,nz,nw))
					end
				end
				*/
				
				/*
				float simplexScaledNoise(
					const float octaves,
					const float persistence,
					const float scale,
					const float loBound,
					const float hiBound,
					const float x,
					const float y,
					const float z,
					const float w
				) {
					return simplexNoise(octaves, persistence, scale, x, y, z, w) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
				}
				*/
				
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
				
				
				terHeight = singleton->getHeightAtPixelPos(fi,fj);
				
				
				
				for (k = 0; k < cellsPerHolder; k++) {
					fk = (k + gphMinInPixels[2]);
					fkAbs = fk;//abs(j-singleton->heightmapMax*0.5f);
					
					
					simplexMod = (
						clampfZO(
							1.0f - 
							abs(
								fk-(terHeight-simplexRegion*0.5f)
							)/simplexRegion
						)	
					);
					
					distanceBelowTer = (
						clampfZO(
							
							(
								terHeight-fk
							)/simplexRegion
						)	
					);
					
					
					if (simplexMod > 0.0f) {
						simplexVal = clampfZO(
							simplexScaledNoise(
								4.0f, //octaves
								0.5f, //persistence (amount added in each successive generation)
								1.0f/64.0f, //scale (frequency)
								0.0f, // lo bound
								1.0f, // hi bound
								fiAbs,
								fjAbs,
								fkAbs
							) 
						);
					}
					else {
						simplexVal = 0.0f;
					}
					
					
					
					//simplexVal = simplexVal*2.0f-1.0f;
					
					// simplexVal2 = clampfZO(
					// 	simplexScaledNoise(
					// 		5.0f, //octaves
					// 		0.5f, //persistence (amount added in each successive generation)
					// 		1.0f/128.0f, //scale (frequency)
					// 		0.0f, // lo bound
					// 		1.0f, // hi bound
					// 		fiAbs,
					// 		fjAbs,
					// 		fkAbs
					// 	) 
					// );
					
					// disVal = (fi/wspX)*(fj/wspY);
					
					
					
					// simplexVal = mixf(simplexVal1,simplexVal2,disVal);
					
					if (
						((terHeight + simplexVal*simplexMod*800.0f) - (fk+100.0f+300.0f*distanceBelowTer)) > 0.0f
					) {
						iSimp = FLUID_UNIT_MAX;
					}
					else {
						iSimp = FLUID_UNIT_MIN;
					}
					
					if (iSimp == FLUID_UNIT_MAX) {
						iSimp2 = FLUID_UNIT_MIN;
					}
					else {
						if (fk < watHeight) {
							iSimp2 = FLUID_UNIT_MAX;
						}
						else {
							iSimp2 = FLUID_UNIT_MIN;
						}
					}
					
					
					
					
					
					
					// if (fSimp > 0.5) {
					// 	iSimp = FLUID_UNIT_MAX;
					// }
					// else {
					// 	iSimp = FLUID_UNIT_MIN;
					// }
					
					
					ind = (i + j*cellsPerHolder + k*cellsPerHolder*cellsPerHolder)*4;
					
					for (q = 0; q < 4; q++) {
						cellData[ind+q] = FLUID_UNIT_MIN;
						extrData[ind+q] = FLUID_UNIT_MIN;
					}
					
					
					cellData[ind+0] = iSimp;
					cellData[ind+1] = iSimp2;
					cellData[ind+2] = iSimp2;
					
		
		
				}
			}
		}
		
		
		wasGenerated = true;
		justGenerated = true;
	}

	
	void fetchGeom() {
		int i;
		int j;
		int k;
		int n;
		
		int m;
		
		GameBlock* curBlock;
		GamePageHolder* gph;
		FIVector4 start;
		FIVector4 end;
		GameEnt* gameEnt;


		for (n = 0; n < E_ET_LENGTH; n++) {
			containsEntIds[n].data.clear();
			
			for (i = -1; i <= 1; i++) {
				for (j = -1; j <= 1; j++) {
					for (k = -1; k <= 1; k++) {
						curBlock = singleton->gw->getBlockAtCoords(
							offsetInBlocks.getIX()+i,
							offsetInBlocks.getIY()+j,
							offsetInBlocks.getIZ()+k,
							true
						);

						for (m = 0; m < curBlock->gameEnts[n].data.size(); m++) {

							gameEnt = &(curBlock->gameEnts[n].data[m]);

							start.copyFrom( &(gameEnt->moveMinInPixels) );
							end.copyFrom( &(gameEnt->moveMaxInPixels) );

							start.clampZ(0.0,singleton->worldSizeInPixels.getFZ()-1.0f);
							end.clampZ(0.0,singleton->worldSizeInPixels.getFZ()-1.0f);

							if (FIVector4::intersectInt(&start,&end,&gphMinInPixels,&gphMaxInPixels)) {
								containsEntIds[n].data.push_back(intPair());
								containsEntIds[n].data.back().v0 = curBlock->blockId;
								containsEntIds[n].data.back().v1 = m;
							}
						}
					}
					
				}
			}
		}


		
	}


};

