
class GameFluid {
public:
	Singleton* singleton;

	PRIM_FORMAT* volDataPrim[E_PL_LENGTH];

	int forceFullRefresh;

	int volSizePrim;
	int volSizePrimBuf;
	int bufAmount;
	int vspMin;
	int vspMax;
	int* fluidData;
	int* extraData;
	
	std::vector<int> indexStack;
	
	int curTick;
	int maxTicks;
	
	int UNIT_MIN;
	int UNIT_MAX;
	int UNIT_INSIDE;
	
	int watchMinX;
	int watchMaxX;
	int watchMinY;
	int watchMaxY;
	int watchMinZ;
	int watchMaxZ;
	int totSize;
	
	FIVector4 readMIP;
	
	FIVector4 lastDirtyMin;
	FIVector4 lastDirtyMax;
	
	FIVector4 dirtyMin;
	FIVector4 dirtyMax;
	
	FIVector4 tempMin;
	FIVector4 tempMax;
	
	FIVector4 minV;
	FIVector4 maxV;
	
	bool hasRead;
	bool invalidated;
	
	float F_UNIT_MAX;
	float F_UNIT_MIN;
	
	std::vector<FluidStruct> fsVec;
	//std::vector<FluidPlane> fsPlaneVec;
	std::vector<ModUnitStruct> modStack;

	FluidPlane fluidPlane;	

	GameFluid() {
		
		hasRead = false;
		
		UNIT_MIN = FLUID_UNIT_MIN;
		UNIT_MAX = FLUID_UNIT_MAX;
		
		UNIT_INSIDE = FLUID_UNIT_MIN + 8;
		
		invalidated = true;
		
		F_UNIT_MIN = UNIT_MIN;
		F_UNIT_MAX = UNIT_MAX;
		
		watchMinX = -1;
		watchMaxX = -1;
		watchMinY = -1;
		watchMaxY = -1;
		watchMinZ = -1;
		watchMaxZ = -1;
		
	}

	void init(Singleton* _singleton) {
		singleton = _singleton;
		volSizePrim = singleton->volSizePrim;
		bufAmount = 1;
		volSizePrimBuf = volSizePrim+bufAmount*2;
		
		forceFullRefresh = 2;
		
		curTick = 0;
		maxTicks = 1;//volSizePrim/8;
		
		vspMin = bufAmount;
		vspMax = volSizePrimBuf-bufAmount;
		
		fluidData = new int[volSizePrimBuf*volSizePrimBuf*volSizePrimBuf*4];
		extraData = new int[volSizePrimBuf*volSizePrimBuf*volSizePrimBuf*4];
		
		totSize = volSizePrimBuf*volSizePrimBuf*volSizePrimBuf;
		
		int i;
		
		for (i = 0; i < E_PL_LENGTH; i++) {
			
			volDataPrim[i] = new uint[volSizePrim*volSizePrim*volSizePrim];
			
		}
		
		lastDirtyMax.setFXYZ(vspMin);
		lastDirtyMin.setFXYZ(vspMax);
		maxDirtyRegion();
		
		
	}
	
	void getPrimData(int n) {
		//cout << "start\n";
		
		int i;
		int j;
		int k;
		int c;
		
		uint v0;
		uint v1;
		uint v2;
		uint v3;
		
		int indSrc;
		int indDest;
		
		int iMin = clamp(dirtyMin.getIX(), vspMin, vspMax);
		int iMax = clamp(dirtyMax.getIX(), vspMin, vspMax);
		int jMin = clamp(dirtyMin.getIY(), vspMin, vspMax);
		int jMax = clamp(dirtyMax.getIY(), vspMin, vspMax);
		int kMin = clamp(dirtyMin.getIZ(), vspMin, vspMax);
		int kMax = clamp(dirtyMax.getIZ(), vspMin, vspMax);
		
		if (forceFullRefresh > 0) {
		 	forceFullRefresh--;
			
			iMin = vspMin;
			iMax = vspMax;
			jMin = vspMin;
			jMax = vspMax;
			kMin = vspMin;
			kMax = vspMax;
		}
		
		
		float NEW_UNIT_MAX = UNIT_MAX + 1;
		
		uint* vdpPtr = (volDataPrim[n]);
		
		switch (n) {
			case E_PL_TERRAIN:
				
				for (i = iMin; i < iMax; i++) {
					for (j = jMin; j < jMax; j++) {
						for (k = kMin; k < kMax; k++) {
							indSrc = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf)*4;
							indDest = ((i-bufAmount) + (j-bufAmount)*volSizePrim + (k-bufAmount)*volSizePrim*volSizePrim);
							
							v0 = ((fluidData[indSrc+0]+1)*255)/NEW_UNIT_MAX;
							v1 = ((fluidData[indSrc+1]+1)*255)/NEW_UNIT_MAX;
							v2 = ((fluidData[indSrc+2]+1)*255)/NEW_UNIT_MAX;
							v3 = ((fluidData[indSrc+3]+1)*255)/NEW_UNIT_MAX;
							
							vdpPtr[indDest] = 
								(v0) |
								(v1 << 8) |
								(v2 << 16) |
								(v3 << 24);
								
							
							
						}
					}
				}
				
				
			break;
			
			// case E_PL_PRIMIDS:
				
			// break;
			
		}
		//cout << "end\n\n";
	}
	
	
	
	
	
	bool writeFluidData(int writePosStart, int writePosEnd, bool writeDirty) {
		int i;
		int j;
		int k;
		
		int ind;
		
		if (writePosStart >= vspMax) {
			return true;
		}
		
		
		//tempMin.copyFrom(&lastDirtyMin);
		//tempMax.copyFrom(&lastDirtyMax);
		//FIVector4::growBoundary(&tempMin,&tempMax,&dirtyMin,&dirtyMax);
		
		tempMin.copyFrom(&dirtyMin);
		tempMax.copyFrom(&dirtyMax);
		
		int iMin = clamp(tempMin.getIX()-1, vspMin, vspMax);
		int iMax = clamp(tempMax.getIX()+1, vspMin, vspMax);
		int jMin = clamp(tempMin.getIY()-1, vspMin, vspMax);
		int jMax = clamp(tempMax.getIY()+1, vspMin, vspMax);
		int kMin = clamp(tempMin.getIZ()-1, vspMin, vspMax);
		int kMax = clamp(tempMax.getIZ()+1, vspMin, vspMax);
		
		
		
		if (writeDirty) {
			
		}
		else {
			iMin = vspMin;
			iMax = vspMax;
			jMin = vspMin;
			jMax = vspMax;
			kMin = writePosStart;
			kMax = writePosEnd;
		}
		
		bool endGreater = (writePosEnd >= vspMax);
		
		int ox = singleton->volMinInPixels.getIX();
		int oy = singleton->volMinInPixels.getIY();
		int oz = singleton->volMinInPixels.getIZ();
		
		
		if (hasRead) {
			
			
			for (k = kMin; k < kMax; k++) {
				
				for (j = jMin; j < jMax; j++) {
					
					for (i = iMin; i < iMax; i++) {
						
						
						ind = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf)*4;
												
						singleton->gw->setCellAtCoords(
							i-bufAmount+ox,
							j-bufAmount+oy,
							k-bufAmount+oz,
							ind,
							fluidData,
							extraData
							
						);
						
						
					}
					
					
				}	
			}
			
			return endGreater||writeDirty;
		}
		else {
			return true;
		}
	}
	
	void prereadFluidData() {
		
		GamePageHolder* curHolder;
		
		int i;
		int j;
		int k;
		int q;
		
		int ox = singleton->volMinInPixels.getIX()/singleton->cellsPerHolder;
		int oy = singleton->volMinInPixels.getIY()/singleton->cellsPerHolder;
		int oz = singleton->volMinInPixels.getIZ()/singleton->cellsPerHolder;
		
		int maxVal = (volSizePrimBuf/singleton->cellsPerHolder) + 1;
		
		for (k = -1; k < maxVal; k++) {
			
			for (j = -1; j < maxVal; j++) {
				
				for (i = -1; i < maxVal; i++) {
					
					
					curHolder = singleton->gw->getHolderAtCoords(
						i+ox,
						j+oy,
						k+oz,
						true
					);
					
					if (curHolder->wasGenerated) {
						
					}
					else {
						
						if (singleton->threadPoolCount == MAX_THREADS) {
							
							//cout << "joining threadPool\n";
							
							for (q = 0; q < MAX_THREADS; q++) {
								singleton->stopTP(i);
							}
							singleton->threadPoolCount = 0;
						}
						
						singleton->threadPool[singleton->threadPoolCount].threadDataInt[0] = curHolder->blockId;
						singleton->threadPool[singleton->threadPoolCount].threadDataInt[1] = curHolder->holderId;
						
						singleton->startTP(singleton->threadPoolCount);
						
						
						
						singleton->threadPoolCount++;
						
					}
					
				}
				
				
			}
		}
	}
	
	void readFluidData() {
		
		int i;
		int j;
		int k;
		int q;
		
		
		int ind;
		int ind2;
		
		
		
		for (i = 0; i < totSize; i++) {
			ind = i*4;
			
			for (q = 0; q < 4; q++) {
				fluidData[ind+q] = UNIT_MIN;
				extraData[ind+q] = UNIT_MIN;
			}
			fluidData[ind+E_PTT_TER] = UNIT_MAX;
			
		}
		
		
		
		
		for (k = vspMin; k < vspMax; k++) {
			
			for (j = vspMin; j < vspMax; j++) {
				
				for (i = vspMin; i < vspMax; i++) {
					
					
					ind = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf)*4;
					
					singleton->gw->getCellAtCoords(
						(i-bufAmount)+readMIP.getIX(),
						(j-bufAmount)+readMIP.getIY(),
						(k-bufAmount)+readMIP.getIZ(),
						&(fluidData[ind]),
						&(extraData[ind])
					);
					
					
				}
				
				
			}
		}
		
		
	}
	
	
	void applyMods() {
		while (modStack.size() > 0) {
			applyUnitModification(
				&(modStack.back().basePos),
				modStack.back().brushAction,
				modStack.back().modType,
				modStack.back().radius
			);
			modStack.pop_back();
		}
	}
	
	
	void updateFluidData() {
		
		int i;
		int j;
		int k;
		int c;
		int dir;
		int n;
		int p;
		int q;
		
		int tempv;
		
		int testInd;
		int testInd2;
		int testInd3;
		int testI;
		int testJ;
		int testK;
		
		int numCells;
		
		int ind;
		int ind2;
		
		int totWat;
		
		float fVSP = volSizePrimBuf;
		
		bool doProc;
		
		float fi;
		float fj;
		float fk;
		
		int ox;
		int oy;
		int oz;
		
		int curId;
		int tempi;
		
		int* bldVal;
		int* terVal;
		int* watVal;
		int* watVal2;
		int* watVal3;
		int* terVal2;
		int* bldVal2;
		int* ideVal;
		int* edgVal;
		
		int curCollectedWater;
		
		int fluidBodyCount;
		
		
		// k is largest at the top
		
		bool bTouchesAir;
		
		// float fSimp[4];
		// int iSimp[4];
		
		float disFromTop;
		float disFromBot;
		float spanZ;
		float minZ;
		
		float maxDif = 1.0f/4.0f;// /8.0f;
		
		float zv;
		
		bool isAir;
		bool isEmptyWater;
		bool didPrint = false;
		
		bool isInBounds;
		
		int minV0 = min( ((curTick)*totSize)/maxTicks, totSize-1 );
		int minV1 = min( ((curTick+1)*totSize)/maxTicks, totSize-1 );
		
		
		if (hasRead) {
			
		}
		else {
			return;
		}
		
		
		//if (curTick == 0) {
			// clear water ids and edges
			for (i = 0; i < totSize; i++) {
				ind = i;
				extraData[ind*4+E_PTT_IDE] = 0;
				extraData[ind*4+E_PTT_STB] = 0;
				fluidData[ind*4+E_PTT_LST] = fluidData[ind*4+E_PTT_WAT];
			}
			
			for (i = 0; i < fsVec.size(); i++) {
				fsVec[i].fluidIds.clear();
				fsVec[i].idealCellIds.clear();
			}
			fsVec.clear();
			
			// for (i = 0; i < fsPlaneVec.size(); i++) {
			// 	fsPlaneVec[i].fluidIds.clear();
			// }
			// fsPlaneVec.clear();
			
			
			
		//}
		
		
		
		
		
		// find stable regions
		fluidBodyCount = 1;
		for (i = minV0; i <= minV1; i++) {
			ind = i;
			
			
			watVal = &(fluidData[ind*4+E_PTT_WAT]);
			ideVal = &(extraData[ind*4+E_PTT_STB]);
			
			if (
				(*watVal >= 0) &&
				(*ideVal == 0)
			) {
				
				if (findStableRegions(ind,fluidBodyCount)) {
					fluidBodyCount++;
				}
			}
		}
		
		
		
		// flood fill water ids
		fluidBodyCount = 1;
		for (i = minV0; i <= minV1; i++) {
			ind = i;
			
			watVal = &(fluidData[ind*4+E_PTT_WAT]);
			ideVal = &(extraData[ind*4+E_PTT_IDE]);
			
			if (
				(*watVal >= 0) &&
				(*ideVal == 0)
			) {
				
				if (floodFillId(ind,fluidBodyCount)) {
					fluidBodyCount++;
				}
				
			}
			
		}
		
		
		
		
		
		
		// find most ideal areas to distribute water
		for (n = 0; n < fsVec.size(); n++) {
			
			spanZ = fsVec[n].maxZ - fsVec[n].minZ;
			
			#ifdef DEBUG_BOUNDS
			isInBounds = false;
			#endif
			
			for (p = 0; p < fsVec[n].fluidIds.size(); p++) {
				ind = fsVec[n].fluidIds[p];
				
				k = ind/(volSizePrimBuf*volSizePrimBuf);
				j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
				i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
				
				#ifdef DEBUG_BOUNDS
				isInBounds = isInBounds||inBounds(i,j,k);
				#endif
				
				
				bldVal = &(extraData[ind*4+E_PTT_BLD]);
				terVal = &(fluidData[ind*4+E_PTT_TER]);
				watVal = &(fluidData[ind*4+E_PTT_WAT]);
				
				isAir = 
					(*watVal == UNIT_MIN) &&
					(*bldVal == UNIT_MIN) &&
					(*terVal == UNIT_MIN);
				
				isEmptyWater = (*watVal == 0);
				
				#ifdef DEBUG_BOUNDS
				if (isInBounds) {
					if (isAir) {cout << "isAir\n";}
					cout << "watVal " << *watVal << "\n";
				}
				#endif
				
				
				if (
					isAir
					// && ((k != fsVec[n].maxZ)||(spanZ==1)) // water on top is not ideal for redist
				) {
					// check for water above
					testInd = ind + (volSizePrimBuf*volSizePrimBuf);
					watVal2 = &(fluidData[testInd*4+E_PTT_WAT]);
					
					if (*watVal2 > 0) {
						fsVec[n].idealCellIds.push_back(ind);
					}
					else {
						for (dir = 0; dir < 4; dir++) {
							testI = (i+DIR_VECS_I[dir][0]);
							testJ = (j+DIR_VECS_I[dir][1]);
							testK = (k+DIR_VECS_I[dir][2]);
							testInd2 = 
								testI +
								testJ*volSizePrimBuf +
								testK*volSizePrimBuf*volSizePrimBuf;
							
							watVal3 = &(fluidData[testInd2*4+E_PTT_WAT]);
							
							// has (partially) full water cell to side
							if (*watVal3 >= 0) {
								
								// if unit below that one is earth
								testInd3 = testInd2 - volSizePrimBuf*volSizePrimBuf;
								if (
									(fluidData[testInd3*4+E_PTT_TER] > UNIT_MIN) ||
									(extraData[testInd3*4+E_PTT_BLD] > UNIT_MIN)
									// || (fluidData[testInd3*4+E_PTT_WAT] == UNIT_MIN)
								) {
									fsVec[n].idealCellIds.push_back(ind);
									break;
								}
							}
						}
					}
				}
			}
			
			#ifdef DEBUG_BOUNDS
			if (isInBounds) {
				didPrint = true;
				cout << "cells considered " << fsVec[n].fluidIds.size() << "\n";
				cout << "cells pushed " << fsVec[n].idealCellIds.size() << "\n";
			}
			#endif
			
			
		}
		
		/*
		if (fsVec[n].idealCellIds.size() != 0) {
			
		}
		*/
		
	
		// remove water from top of fluid body
		for (n = 0; n < fsVec.size(); n++) {
			
			if (fsVec[n].idealCellIds.size() != 0) {
				fsVec[n].collectedWater = 0;
				curCollectedWater = 0;
				
				minZ = fsVec[n].minZ;
				spanZ = fsVec[n].maxZ - fsVec[n].minZ;
				
				#ifdef DEBUG_BOUNDS
				isInBounds = false;
				#endif
				
				totWat = 0;
				
				for (p = 0; p < fsVec[n].fluidIds.size(); p++) {
					ind = fsVec[n].fluidIds[p];
					
					k = ind/(volSizePrimBuf*volSizePrimBuf);
					j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
					i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
					
					#ifdef DEBUG_BOUNDS
					isInBounds = isInBounds||inBounds(i,j,k);
					#endif
					
					
					
					
					
					fk = k;
					disFromBot = (fk-minZ)/(spanZ);
					disFromTop = 1.0f - disFromBot;
					
					disFromBot = mixf(0.25,0.75,disFromBot);
					disFromTop = mixf(0.25,0.75,disFromTop);
					
					if (spanZ <= 3.0f) {
						disFromBot = 1.0f;
					}
					
					
					//terVal = &(fluidData[ind*4+E_PTT_TER]);
					watVal = &(fluidData[ind*4+E_PTT_WAT]);
					//ideVal = &(extraData[ind*4+E_PTT_IDE]);
					
					totWat += max(*watVal,0);
					
					// make sure no water is above or that there is earth above
					testInd = ind + (volSizePrimBuf*volSizePrimBuf);
					watVal2 = &(fluidData[testInd*4+E_PTT_WAT]);
					terVal2 = &(fluidData[testInd*4+E_PTT_TER]);
					bldVal2 = &(extraData[testInd*4+E_PTT_BLD]);
					
					if (
						(*watVal > 0) &&
						(
							(*watVal2 <= 0) ||
							(*terVal2 != UNIT_MIN) ||
							(*bldVal2 != UNIT_MIN)
						)
					) {
						curCollectedWater = 
							min(
								((int)(disFromBot*F_UNIT_MAX*maxDif + 1.0f)),
								*watVal
							);
						*watVal -= curCollectedWater;
						fsVec[n].collectedWater += curCollectedWater;
					}
				}
				
				
				
				// cout << "fsVec[n].collectedWater " << fsVec[n].collectedWater << "\n";
				
				// if (fsVec[n].collectedWater == 0) {
				// 	cout << "fsVec[n].fluidIds.size() " << fsVec[n].fluidIds.size() << "\n";
				// }
				
				
				
				#ifdef DEBUG_BOUNDS
				if (isInBounds) {
					didPrint = true;
					cout << "totWat " << totWat << "\n";
					cout << "fsVec[n].collectedWater " << fsVec[n].collectedWater << "\n";
				}
				#endif
				
			}
			
			
			
		}
		
		
		
		
		
		// add water to ideal spots
		for (n = 0; n < fsVec.size(); n++) {
			
			numCells = fsVec[n].idealCellIds.size();
			
			if (numCells > 0) {
				
				for (p = 0; p < numCells; p++) {
					ind = fsVec[n].idealCellIds[p];
					watVal = &(fluidData[ind*4+E_PTT_WAT]);
					
					if (*watVal == UNIT_MIN) {
						*watVal = 0;
					}
				}
				
				
				
				if ( (fsVec[n].collectedWater/numCells) > UNIT_MAX ) {
					curCollectedWater = UNIT_MAX;
				}
				else {
					curCollectedWater = fsVec[n].collectedWater/numCells;
				}
				
				//cout << "curCollectedWater " << curCollectedWater << "\n";
				
				curCollectedWater = curCollectedWater*maxDif;
				
				#ifdef DEBUG_BOUNDS
				isInBounds = false;
				#endif
				
				
				for (p = 0; p < numCells; p++) {
					ind = fsVec[n].idealCellIds[p];
					
					k = ind/(volSizePrimBuf*volSizePrimBuf);
					j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
					i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
					
					#ifdef DEBUG_BOUNDS
					isInBounds = isInBounds||inBounds(i,j,k);
					#endif
					
					
					watVal = &(fluidData[ind*4+E_PTT_WAT]);
					
					
					if (curCollectedWater > 0) {
						
						tempi = min(UNIT_MAX - *watVal,curCollectedWater);
						
						*watVal += tempi;
						fsVec[n].collectedWater -= tempi;
					}
					
					
					
					
					
					
					if (fsVec[n].collectedWater <= 0) {
						break;
					}
					
				}
				
				#ifdef DEBUG_BOUNDS
				if (isInBounds) {
					didPrint = true;
					cout << "dist water " << fsVec[n].collectedWater << "\n\n";
				}
				#endif
				
			}
		}
		
		
		tempMax.setFXYZ(vspMin);
		tempMin.setFXYZ(vspMax);
		
		
		// add water to bottom of fluid body
		for (n = 0; n < fsVec.size(); n++) {
			
			if (fsVec[n].idealCellIds.size() != 0) {
				curCollectedWater = 0;
				minZ = fsVec[n].minZ;
				
				
				while (fsVec[n].collectedWater > 0) {
					
					
					for (q = fsVec[n].minZ; q <= fsVec[n].maxZ; q++) {
						for (p = 0; p < fsVec[n].fluidIds.size(); p++) {
							ind = fsVec[n].fluidIds[p];
							
							k = ind/(volSizePrimBuf*volSizePrimBuf);
							watVal = &(fluidData[ind*4+E_PTT_WAT]);
							
							if (k == q) {
								
								if (fsVec[n].collectedWater > UNIT_MAX) {
									tempv = UNIT_MAX;
								}
								else {
									tempv = fsVec[n].collectedWater;
								}
								
								//if (fluidData[ind*4+E_PTT_TER] == UNIT_MIN) {
									
									curCollectedWater = min(
										((int)((UNIT_MAX - max(*watVal,0) )*maxDif + 1.0f )),
										tempv
									);
									
									if (curCollectedWater > 0) {
										if (*watVal == UNIT_MIN) {
											*watVal = 0;
										}
										
										tempi = min(UNIT_MAX - *watVal,curCollectedWater);
										
										*watVal += tempi;
										fsVec[n].collectedWater -= tempi;
									}
									
								//}
							}
							
							
							// out of water, break both parent loops
							if (fsVec[n].collectedWater <= 0) {
								p = fsVec[n].fluidIds.size() + 1;
								q = fsVec[n].maxZ + 1;
							}
						}
					}
					
					
				}
			}
			
			
		}
	
		
		if (didPrint) {
			watchMinX = -1;
		}
		
		
		curTick++;
		if (curTick == maxTicks) {
			curTick = 0;
		}
		
		
		
		/////////////
		
		
		for (n = 0; n < fsVec.size(); n++) {
			
			if (
				(fsVec[n].idealCellIds.size() != 0)
				// || (
				// 	(fsVec[n].fluidIds.size() > 0) &&
				// 	(fsVec[n].fluidIds.size() < 8)
				// )
				
			) {
				
				minV.setIXYZ(fsVec[n].minX,fsVec[n].minY,fsVec[n].minZ);
				maxV.setIXYZ(fsVec[n].maxX,fsVec[n].maxY,fsVec[n].maxZ);
				FIVector4::growBoundary(&tempMin,&tempMax,&minV,&maxV);
				
			}
		}
		
		tempMin.addXYZ(-1.0f);
		tempMax.addXYZ(1.0f);
		
		FIVector4::growBoundary(&dirtyMin,&dirtyMax,&tempMin,&tempMax);
		
		// int iMin = clamp(tempMin.getIX(), vspMin, vspMax);
		// int iMax = clamp(tempMax.getIX(), vspMin, vspMax);
		// int jMin = clamp(tempMin.getIY(), vspMin, vspMax);
		// int jMax = clamp(tempMax.getIY(), vspMin, vspMax);
		// int kMin = clamp(tempMin.getIZ(), vspMin, vspMax);
		// int kMax = clamp(tempMax.getIZ(), vspMin, vspMax);
		
		// int iMin2 = vspMax;
		// int iMax2 = vspMin;
		// int jMin2 = vspMax;
		// int jMax2 = vspMin;
		// int kMin2 = vspMax;
		// int kMax2 = vspMin;
		
		// int indSrc;
		
		// for (k = kMin; k <= kMax; k++) {
		// 	for (j = jMin; j <= jMax; j++) {
		// 		for (i = iMin; i <= iMax; i++) {
		// 			indSrc = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf)*4;
		// 			if (
					
		// 				//true
					
		// 				fluidData[indSrc + E_PTT_WAT] != fluidData[indSrc + E_PTT_LST]
						
		// 				// (
		// 				// 	(fluidData[indSrc + E_PTT_WAT] == UNIT_MIN) &&
		// 				// 	(fluidData[indSrc + E_PTT_LST] != UNIT_MIN)	
		// 				// ) ||
		// 				// (
		// 				// 	(fluidData[indSrc + E_PTT_WAT] != UNIT_MIN) &&
		// 				// 	(fluidData[indSrc + E_PTT_LST] = UNIT_MIN)	
		// 				// )
						
		// 			) {
						
		// 				if (i < iMin2) {iMin2 = i;}
		// 				if (j < jMin2) {jMin2 = j;}
		// 				if (k < kMin2) {kMin2 = k;}
		// 				if (i > iMax2) {iMax2 = i;}
		// 				if (j > jMax2) {jMax2 = j;}
		// 				if (k > kMax2) {kMax2 = k;}
						
		// 			}
		// 		}
		// 	}
		// }
		
		// minV.setIXYZ(iMin2,jMin2,kMin2);
		// maxV.setIXYZ(iMax2,jMax2,kMax2);
		// FIVector4::growBoundary(&dirtyMin,&dirtyMax,&minV,&maxV);
		
	}
	
	
	
	
	
	
	
	
	
	bool findStableRegions(int startInd, int newId) {
		indexStack.clear();
		indexStack.push_back(startInd);
		
		int dir;
		
		int ind;
		int testInd;
		int testInd2;
		int* bldVal;
		int* bldVal2;
		int* terVal;
		int* terVal2;
		int* watVal;
		int* watVal2;
		int* ideVal;
		
		int i;
		int j;
		int k;
		int n;
		
		int* curVec;
		
		int foundInd;
		
		int testI;
		int testJ;
		int testK;
		
		bool notFound;
		bool isAir;
		bool terBelow;
		
		int watCount;
		int maxSize;
		
		long long int totWat;
		long long int totWat2;
		long long int divWat;
		
		//fsPlaneVec.push_back(FluidPlane());
		
		
		FluidPlane* fsPtr = &fluidPlane;//&(fsPlaneVec.back());
		fsPtr->fluidIds.clear();
		
		int emptyWaterCount = 0;
		int airCount = 0;
		int targWat;
		
		while (indexStack.size() > 0) {
			
			ind = indexStack.back();
			extraData[ind*4+E_PTT_STB] = newId;
			fsPtr->fluidIds.push_back(ind);
			
			if (fluidData[ind*4+E_PTT_WAT] == 0) {
				emptyWaterCount++;
			}
			
			
			k = ind/(volSizePrimBuf*volSizePrimBuf);
			j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
			i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
						
			
			foundInd = -1;
			
			for (dir = 0; dir < 4; dir++) {
				testI = (i+DIR_VECS_I[dir][0]);
				testJ = (j+DIR_VECS_I[dir][1]);
				testK = (k+DIR_VECS_I[dir][2]);
				testInd = 
					testI +
					testJ*volSizePrimBuf +
					testK*volSizePrimBuf*volSizePrimBuf;
				
				testInd2 = testInd - volSizePrimBuf*volSizePrimBuf;
				
				terVal = &(fluidData[testInd*4+E_PTT_TER]);
				watVal = &(fluidData[testInd*4+E_PTT_WAT]);
				ideVal = &(extraData[testInd*4+E_PTT_STB]);
				bldVal = &(extraData[testInd*4+E_PTT_BLD]);
				
				
				isAir = 
					(*watVal == UNIT_MIN) &&
					(*terVal == UNIT_MIN) &&
					(*bldVal == UNIT_MIN);
					
				//terVal2 = &(fluidData[testInd2*4+E_PTT_TER]);
				//watVal2 = &(fluidData[testInd2*4+E_PTT_WAT]);
				// terBelow = 
				// 	//(*watVal2 == UNIT_MIN) &&
				// 	(*terVal2 == UNIT_MIN);
				// if (isAir&&terBelow) {
				// 	airCount++;
				// }
				
				if (
					(*ideVal == 0) &&
					(*watVal >= 0)
				) {
					foundInd = testInd;
				}
			}
			
			if (foundInd >= 0) {
				indexStack.push_back(foundInd);
			}
			else {
				indexStack.pop_back();
			}
			
		}
		
		
		maxSize = fsPtr->fluidIds.size();
		
		
		
		if (
			(emptyWaterCount == maxSize)
			// && (airCount == 0)	
		) {
			// all fluid cells in this plane are empty,
			// and there are no adjacent air cells,
			// so get rid of all empty water cells
			
			
			for (i = 0; i < maxSize; i++) {
				ind = fsPtr->fluidIds[i];
				
				fluidData[ind*4+E_PTT_WAT] = UNIT_MIN;
			}
			
			fsPtr->fluidIds.clear();
			
		}
		else {
			
			
			/*
			
			
			// equalize fluid density in this plane
			
			if (maxSize > 0) {
				
				totWat = 0;
				for (i = 0; i < maxSize; i++) {
					ind = fsPtr->fluidIds[i];
					watVal = &(fluidData[ind*4+E_PTT_WAT]);
					totWat += *watVal;
				}
				
				divWat = maxSize;
				if (divWat > 0) {
					
					targWat = (totWat/divWat);
					
					if ( targWat > 0 ) {
						
						totWat2 = 0;
						for (i = 0; i < maxSize; i++) {
							ind = fsPtr->fluidIds[i];
							watVal = &(fluidData[ind*4+E_PTT_WAT]);
							*watVal += (targWat - *watVal)/2;
							totWat2 += *watVal;
						}
						
						i = 0;
						
						while (totWat > totWat2) {
							
							ind = fsPtr->fluidIds[i];
							watVal = &(fluidData[ind*4+E_PTT_WAT]);
							
							*watVal += 1;
							totWat2 += 1;
							
							i++;
							if (i==maxSize) { i = 0; }
							
						}
						
						while (totWat < totWat2) {
							
							ind = fsPtr->fluidIds[i];
							watVal = &(fluidData[ind*4+E_PTT_WAT]);
							
							*watVal -= 1;
							totWat2 -= 1;
							
							i++;
							if (i==maxSize) { i = 0; }
						}
						
					}
				}
				
			}
			
			
			
			
			
			
			*/
			
			
			
		}
		
		//fsPlaneVec.pop_back();
		
		if ( fsPtr->fluidIds.size() == 0 ) {
			
			return false;
		}
		else {
			return true;
		}
		
		
		
	}
	
	
	bool floodFillId(int startInd, int newId) {
		
		indexStack.clear();
		indexStack.push_back(startInd);
		
		int dir;
		
		int ind;
		int testInd;
		int* bldVal;
		int* terVal;
		int* watVal;
		int* watVal2;
		int* ideVal;
		
		int i;
		int j;
		int k;
		int n;
		
		int* curVec;
		
		int foundInd;
		
		int testI;
		int testJ;
		int testK;
		
		bool notFound;
		bool isAir;
		bool isEmptyWater;
		
		fsVec.push_back(FluidStruct());
		
		
		FluidStruct* fsPtr = &(fsVec.back());
		
		
		fsPtr->minX = volSizePrimBuf*2;
		fsPtr->minY = volSizePrimBuf*2;
		fsPtr->minZ = volSizePrimBuf*2;
		fsPtr->maxX = -volSizePrimBuf*2;
		fsPtr->maxY = -volSizePrimBuf*2;
		fsPtr->maxZ = -volSizePrimBuf*2;
		
		while (indexStack.size() > 0) {
			
			ind = indexStack.back();
			extraData[ind*4+E_PTT_IDE] = newId;
			
			k = ind/(volSizePrimBuf*volSizePrimBuf);
			j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
			i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
			
			watVal2 = &(fluidData[ind*4+E_PTT_WAT]);
			
			
			
			notFound = true;
			if (*watVal2 < UNIT_MAX) {
				// if water is partially filled, add to fluidId stack
				fsPtr->fluidIds.push_back(ind);
				notFound = false;
			}
			else {
				// if its full and there is earth above, add it
				testInd = ind + volSizePrimBuf*volSizePrimBuf;
				
				if (
					(fluidData[testInd*4+E_PTT_TER] != UNIT_MIN) ||
					(extraData[testInd*4+E_PTT_BLD] != UNIT_MIN)	
				) {
					fsPtr->fluidIds.push_back(ind);
					notFound = false;
				}
				
			}
			
			//fsPtr->fluidIds.push_back(ind);
			
			foundInd = -1;
			
			
			for (dir = 0; dir < 6; dir++) {
				testI = (i+DIR_VECS_I[dir][0]);
				testJ = (j+DIR_VECS_I[dir][1]);
				testK = (k+DIR_VECS_I[dir][2]);
				testInd = 
					testI +
					testJ*volSizePrimBuf +
					testK*volSizePrimBuf*volSizePrimBuf;
				
				bldVal = &(extraData[testInd*4+E_PTT_BLD]);
				terVal = &(fluidData[testInd*4+E_PTT_TER]);
				watVal = &(fluidData[testInd*4+E_PTT_WAT]);
				ideVal = &(extraData[testInd*4+E_PTT_IDE]);
				
				isAir = 
					(*watVal == UNIT_MIN) &&
					(*terVal == UNIT_MIN) &&
					(*bldVal == UNIT_MIN);
				
				isEmptyWater = (*watVal == 0);
				
				// check if water touches air or an empty water cell, if so add to fluidId stack
				if (
					(isAir||isEmptyWater)	&&
					notFound
				) {
					fsPtr->fluidIds.push_back(ind);
					notFound = false;
				}
				
				if (*ideVal == 0) {
					if (isAir) {
						// it is an unmarked air value or empty water cell adjacent to a water cell
						
						fsPtr->fluidIds.push_back(testInd);
						*ideVal = newId;
						
					}
					
					if (*watVal >= 0) {
						foundInd = testInd;
					}
					
				}
			}
			
			if (foundInd >= 0) {
				indexStack.push_back(foundInd);
			}
			else {
				indexStack.pop_back();
			}
			
		}
		
		
		for (n = 0; n < fsPtr->fluidIds.size(); n++) {
			
			ind = fsPtr->fluidIds[n];
			
			k = ind/(volSizePrimBuf*volSizePrimBuf);
			j = (ind - k*volSizePrimBuf*volSizePrimBuf)/volSizePrimBuf;
			i = ind - ( j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf );
			
			bldVal = &(extraData[ind*4+E_PTT_BLD]);
			terVal = &(fluidData[ind*4+E_PTT_TER]);
			watVal = &(fluidData[ind*4+E_PTT_WAT]);
			
			// remove ids from any air values so they can be reused
			if (
				(*watVal == UNIT_MIN) &&
				(*terVal == UNIT_MIN) &&
				(*bldVal == UNIT_MIN)
			) {
				extraData[ind*4+E_PTT_IDE] = 0;
			}
			
			//fluidData[ind*4+E_PTT_EDG] = UNIT_MAX;
			
			
			if (i < fsPtr->minX) {fsPtr->minX = i;}
			if (j < fsPtr->minY) {fsPtr->minY = j;}
			if (k < fsPtr->minZ) {fsPtr->minZ = k;}
			if (i > fsPtr->maxX) {fsPtr->maxX = i;}
			if (j > fsPtr->maxY) {fsPtr->maxY = j;}
			if (k > fsPtr->maxZ) {fsPtr->maxZ = k;}
			
		}
		
		if ( fsPtr->fluidIds.size() == 0) {
			fsVec.pop_back();
			return false;
		}
		else {
			return true;
		}
		
	}
	
	bool inBounds(int i, int j, int k) {
		return (
			(watchMinX >= 0) &&
			(i >= watchMinX) &&
			(i <= watchMaxX) &&
			(j >= watchMinY) &&
			(j <= watchMaxY) &&
			(k >= watchMinZ) &&
			(k <= watchMaxZ)
		);
	}
	
	void modifyUnit(
		FIVector4 *fPixelWorldCoordsBase,
		int brushAction,
		int modType,
		int radius
	) {
		modStack.push_back(ModUnitStruct());
		modStack.back().basePos.copyFrom(fPixelWorldCoordsBase);
		modStack.back().brushAction = brushAction;
		modStack.back().modType = modType;
		modStack.back().radius = radius;
	}
	
	
	void roundBox(
		
		FIVector4* absVecFromCenter,
		FIVector4* innerBoxRad,
		FIVector4* cornerDisThicknessPower,
		
		bool &isInObj,
		bool &isInside
		
		//float vectorFromCenterX, float vectorFromCenterY, float vectorFromCenterZ,
		//float box_dimX, float box_dimY, float box_dimZ, float box_dimW, 
		//float box_powerX, float box_powerY,
		//float wallThickness
	) {
		
		//doTraceVecND("absVecFromCenter ", absVecFromCenter);
		//doTraceVecND("innerBoxRad ", innerBoxRad);
		
		
		FIVector4 newP;
		FIVector4 orig1;
		FIVector4 orig2;
		
		orig1.setFXYZ(0.0f,0.0f,0.0f);
		
		//vec3 newP = abs(max( absVecFromCenter-(box_dim.xyz),0.0));
		
		orig2.copyFrom(absVecFromCenter);
		orig2.addXYZRef(innerBoxRad,-1.0f);
		newP.maxXYZ(&orig2,&orig1);
		newP.absXYZ();
		
		float powX = cornerDisThicknessPower->getFZ();
		float powY = cornerDisThicknessPower->getFW();
		
		
		

		//newP.xy = pow(newP.xy, box_power.xx );
		newP.powXYZ(powX,powX,1.0f);		
		//newP.x = pow( newP.x + newP.y, 1.0/box_power.x );
		newP.setFX(newP[0] + newP[1]);
		newP.powXYZ(1.0/powX,1.0f,1.0f);

		//newP.xz = pow(newP.xz, box_power.yy );
		newP.powXYZ(powY,1.0f,powY);
		//newP.x = pow( newP.x + newP.z, 1.0/box_power.y );
		newP.setFX(newP[0] + newP[2]);
		newP.powXYZ(1.0/powY,1.0f,1.0f);
		
		//newP.setFX(newP.length());
		
		float rad = cornerDisThicknessPower->getFX();
		float wallThickness = cornerDisThicknessPower->getFY();
		
		// float dis = 
		// 	max(
		// 		(newP[0]-rad),
		// 		( (rad-wallThickness)-newP[0] )	 //newP[0] - (rad-wallThickness)
		// 	);
		
		// //cout << newP[0] << " - " << cornerDisThicknessPower->getFX() << " = " << dis << "\n";
		
		// return dis < 0.0f;
		
		isInObj = (newP[0]-rad) < 0.0f;
		isInside = (newP[0] - (rad-wallThickness)) < 0.0f;

	}
	
	
	void clearAllGeom() {
		int i;
		int ind;
		for (i = 0; i < totSize; i++) {
			ind = i;
			extraData[ind*4+E_PTT_BLD] = UNIT_MIN;
		}
	}
	void clearInsideValues() {
		int i;
		int ind;
		for (i = 0; i < totSize; i++) {
			ind = i;
			if (extraData[ind*4+E_PTT_BLD] == UNIT_INSIDE) {
				extraData[ind*4+E_PTT_BLD] = UNIT_MIN;
			}
			
		}
	}
	
	void fillCurrentGeom(int templateId, FIVector4* templatePos) {
		
		
		
		
		int i;
		int j;
		int k;
		
		
		FIVector4* paramArrGeom = &(singleton->primTemplateStack[templateId*E_PRIMTEMP_LENGTH]);
		
		// for (i = 0; i < E_PRIMTEMP_LENGTH; i++) {
		// 	paramArrGeom[i].setFXYZW(
		// 		singleton->paramArrGeom[i*4+0],
		// 		singleton->paramArrGeom[i*4+1],
		// 		singleton->paramArrGeom[i*4+2],
		// 		singleton->paramArrGeom[i*4+3]
		// 	);
		// }
		
		FIVector4 innerBoxRad;
		FIVector4 absVecFromCenter;
		//FIVector4 centerCoord;
		
		FIVector4 baseVec;
		baseVec.averageXYZ(
			&(paramArrGeom[E_PRIMTEMP_BOUNDSMIN]),
			&(paramArrGeom[E_PRIMTEMP_BOUNDSMAX])
		);
		baseVec.addXYZRef(templatePos);
		baseVec.addXYZRef(&(singleton->volMinReadyInPixels),-1.0f);
		baseVec.addXYZ(1.0f);
		
		FIVector4 curCoord;
		
		FIVector4 newCoordMin;
		FIVector4 newCoordMax;
		newCoordMin.copyFrom(&baseVec);
		newCoordMax.copyFrom(&baseVec);
		
		// centerCoord.averageXYZ(
		// 	&(paramArrGeom[E_PRIMTEMP_BOUNDSMAX]),
		// 	&(paramArrGeom[E_PRIMTEMP_BOUNDSMIN])
		// );
		innerBoxRad.averageNegXYZ(
			&(paramArrGeom[E_PRIMTEMP_BOUNDSMAX]),
			&(paramArrGeom[E_PRIMTEMP_BOUNDSMIN])
		);
		
		innerBoxRad.addXYZ(paramArrGeom[E_PRIMTEMP_CORNERDIS].getFX(),-1.0f);
		
		newCoordMin.addXYZRef(&(paramArrGeom[E_PRIMTEMP_VISMIN]));
		newCoordMax.addXYZRef(&(paramArrGeom[E_PRIMTEMP_VISMAX]));
		
		//newCoordMin.addXYZ(2.0f,2.0f,2.0f);
		//newCoordMax.addXYZ(2.0f,2.0f,2.0f);
		
		int iMin = clamp(newCoordMin.getIX(), vspMin, vspMax);
		int iMax = clamp(newCoordMax.getIX(), vspMin, vspMax);
		int jMin = clamp(newCoordMin.getIY(), vspMin, vspMax);
		int jMax = clamp(newCoordMax.getIY(), vspMin, vspMax);
		int kMin = clamp(newCoordMin.getIZ(), vspMin, vspMax);
		int kMax = clamp(newCoordMax.getIZ(), vspMin, vspMax);
		
		int* bldVal;
		int* terVal;
		int* watVal;
		
		int ind;
		
		bool isInObj;
		bool isInside;
		
		minV.setIXYZ(iMin,jMin,kMin);
		maxV.setIXYZ(iMax,jMax,kMax);
		FIVector4::growBoundary(&dirtyMin,&dirtyMax,&minV,&maxV);
		
		
		
		// cout << "cornerDis " << paramArrGeom[E_PRIMTEMP_CORNERDIS].getFX() << "\n";
		// doTraceVecND("E_PRIMTEMP_BOUNDSMIN ", &(paramArrGeom[E_PRIMTEMP_BOUNDSMIN]));
		// doTraceVecND("E_PRIMTEMP_BOUNDSMAX ", &(paramArrGeom[E_PRIMTEMP_BOUNDSMAX]));
		// doTraceVecND("innerBoxRad ", &innerBoxRad);
		
		for (k = kMin; k < kMax; k++) {
			for (j = jMin; j < jMax; j++) {
				for (i = iMin; i < iMax; i++) {
					
					curCoord.setFXYZ(i,j,k);
					curCoord.addXYZ(0.5f);
					absVecFromCenter.copyFrom(&curCoord);
					absVecFromCenter.addXYZRef(&baseVec,-1.0f);
					absVecFromCenter.absXYZ();
					
					roundBox(
						&absVecFromCenter,
						&innerBoxRad,
						&(paramArrGeom[E_PRIMTEMP_CORNERDIS]),
						isInObj,
						isInside
					);
					
					ind = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf);
					bldVal = &(extraData[ind*4+E_PTT_BLD]);
					terVal = &(fluidData[ind*4+E_PTT_TER]);
					watVal = &(fluidData[ind*4+E_PTT_WAT]);
					
					if (*bldVal == UNIT_INSIDE) {
						
					}
					else {
						
						if (isInObj) {
							if (isInside) {
								*bldVal = UNIT_INSIDE;
							}
							else {
								*bldVal = UNIT_MAX;
								*watVal = UNIT_MIN; // todo: collect water for redist instead of destroying it
							}
						}
						else {
							
						}
					}
					
					// if (*bldVal == UNIT_MAX) {
					// 	*terVal = UNIT_MAX;
					// }
					// else {
					// 	*terVal = UNIT_MIN;
					// }
					
					
				}
			}
		}
		
		
		
	}
	
	void resetDirtyRegion() {
		lastDirtyMax.copyFrom(&dirtyMax);
		lastDirtyMin.copyFrom(&dirtyMin);
		
		dirtyMax.setFXYZ(vspMin);
		dirtyMin.setFXYZ(vspMax);
	}
	void maxDirtyRegion() {
		dirtyMax.setFXYZ(vspMax);
		dirtyMin.setFXYZ(vspMin);
	}
	
	void applyUnitModification(
		FIVector4 *fPixelWorldCoordsBase,
		int brushAction,
		int modType,
		int radius
	) {
		
			
		
			FIVector4 baseVec;
			baseVec.copyFrom(fPixelWorldCoordsBase);
			baseVec.addXYZRef(&(singleton->volMinReadyInPixels),-1.0f);
			
			
			FIVector4 curCoord;
			
			FIVector4 newCoordMin;
			FIVector4 newCoordMax;
			newCoordMin.copyFrom(&baseVec);
			newCoordMax.copyFrom(&baseVec);
			
			
			newCoordMin.addXYZ(-radius + 1);
			newCoordMax.addXYZ( radius + 1);
			
			int iMin = clamp(newCoordMin.getIX(), vspMin, vspMax);
			int iMax = clamp(newCoordMax.getIX(), vspMin, vspMax);
			int jMin = clamp(newCoordMin.getIY(), vspMin, vspMax);
			int jMax = clamp(newCoordMax.getIY(), vspMin, vspMax);
			int kMin = clamp(newCoordMin.getIZ(), vspMin, vspMax);
			int kMax = clamp(newCoordMax.getIZ(), vspMin, vspMax);
			
			minV.setIXYZ(iMin,jMin,kMin);
			maxV.setIXYZ(iMax,jMax,kMax);
			FIVector4::growBoundary(&dirtyMin,&dirtyMax,&minV,&maxV);
			
			
			int i;
			int j;
			int k;
			
			int* empVal;
			int* bldVal;
			int* terVal;
			int* watVal;
			// int* ideVal;
			// int* stbVal;
			
			int ind;
			
			for (k = kMin; k < kMax; k++) {
				for (j = jMin; j < jMax; j++) {
					for (i = iMin; i < iMax; i++) {
						
						curCoord.setFXYZ(i,j,k);
						
						if (baseVec.distance(&curCoord) <= radius) {
							
							
							ind = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf);
							
							bldVal = &(extraData[ind*4+E_PTT_BLD]);
							empVal = &(fluidData[ind*4+E_PTT_EMP]);
							terVal = &(fluidData[ind*4+E_PTT_TER]);
							watVal = &(fluidData[ind*4+E_PTT_WAT]);
							// ideVal = &(extraData[ind*4+E_PTT_IDE]);
							// stbVal = &(extraData[ind*4+E_PTT_STB]);
							
							switch (brushAction) {
								case E_BRUSH_MOVE:
									return;
								break;
								case E_BRUSH_ADD:
								
									if (modType == E_PTT_WAT) {
										if (
											(*terVal == UNIT_MIN) &&
											(*bldVal == UNIT_MIN)	
										) {
											*watVal = UNIT_MAX;
										}
									}
									else {
										*terVal = UNIT_MAX;
										*watVal = UNIT_MIN;
									}
									
									
									
								break;
								case E_BRUSH_SUB:
									*empVal = UNIT_MAX;
									*bldVal = UNIT_MIN;
									*terVal = UNIT_MIN;
									*watVal = UNIT_MIN;
								break;
								case E_BRUSH_REF:
									watchMinX = iMin;
									watchMaxX = iMax;
									
									watchMinY = jMin;
									watchMaxY = jMax;
									
									watchMinZ = kMin;
									watchMaxZ = kMax;
									
								break;
								
							}
							
							
						}
						
						
					}
				}
			}
			
	}
	
	
	
	
	
};

