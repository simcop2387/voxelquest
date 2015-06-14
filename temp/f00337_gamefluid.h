// f00337_gamefluid.h
//

#include "f00337_gamefluid.e"
#define LZZ_INLINE inline
GameFluid::GameFluid ()
                    {
		UNIT_MIN = FLUID_UNIT_MIN;
		UNIT_MAX = FLUID_UNIT_MAX;
		
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
void GameFluid::init (Singleton * _singleton)
                                         {
		singleton = _singleton;
		volSizePrim = singleton->volSizePrim;
		bufAmount = 1;
		volSizePrimBuf = volSizePrim+bufAmount*2;
		
		curTick = 0;
		maxTicks = 1;//volSizePrim/8;
		
		vspMin = bufAmount;
		vspMax = volSizePrimBuf-bufAmount;
		
		fluidData = new int[volSizePrimBuf*volSizePrimBuf*volSizePrimBuf*4];
		lastFluidData = new int[volSizePrimBuf*volSizePrimBuf*volSizePrimBuf*4];
		
	}
void GameFluid::updateFluidData (bool firstTime)
                                             {
		
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
		
		int totSize = volSizePrimBuf*volSizePrimBuf*volSizePrimBuf;
		
		bool doProc;
		
		float fi;
		float fj;
		float fk;
		
		int ox;
		int oy;
		int oz;
		
		int curId;
		int tempi;
		
		int* terVal;
		int* watVal;
		int* watVal2;
		int* watVal3;
		int* terVal2;
		int* ideVal;
		int* edgVal;
		
		int curCollectedWater;
		
		int fluidBodyCount;
		
		
		// k is largest at the top
		
		bool bTouchesAir;
		
		float fSimp[4];
		int iSimp[4];
		
		float disFromTop;
		float disFromBot;
		float spanZ;
		float minZ;
		
		float maxDif = 1.0f/4.0f;// /8.0f;
		
		bool isAir;
		bool isEmptyWater;
		bool didPrint = false;
		
		bool isInBounds;
		
		int minV0 = min( ((curTick)*totSize)/maxTicks, totSize-1 );
		int minV1 = min( ((curTick+1)*totSize)/maxTicks, totSize-1 );
		
		
		if (firstTime) {
			for (i = 0; i < 4; i++) {
				fSimp[i] = 0.0f;
			}
			
			for (i = 0; i < totSize; i++) {
				ind = i*4;
				fluidData[ind+E_PTT_TER] = UNIT_MAX;
				fluidData[ind+E_PTT_WAT] = UNIT_MIN;
			}
			
			
			for (k = vspMin; k < vspMax; k++) {
				fk = k;
				for (j = vspMin; j < vspMax; j++) {
					fj = j;
					for (i = vspMin; i < vspMax; i++) {
						fi = i;
				
						
						fSimp[0] = clampfZO(simplexScaledNoise(
							4.0f, //octaves
							0.5f, //persistence (amount added in each successive generation)
							1.0f/16.0f, //scale (frequency)
							0.0f, // lo bound
							1.0f, // hi bound
							fi,
							fj,
							fk
						));
						
						fSimp[1] = clampfZO(simplexScaledNoise(
							4.0f, //octaves
							0.5f, //persistence (amount added in each successive generation)
							1.0f/16.0f, //scale (frequency)
							0.0f, // lo bound
							1.0f, // hi bound
							fi+124.0f,
							fj+23.0f,
							fk+53.0f
						));
						
						if (fSimp[0] > 0.5) {
							iSimp[0] = UNIT_MAX;
						}
						else {
							iSimp[0] = UNIT_MIN;
						}
						
						if (fSimp[1] > 0.75) {
							iSimp[1] = UNIT_MAX;
						}
						else {
							iSimp[1] = UNIT_MIN;
						}
						
						ind = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf)*4;
						fluidData[ind+0] = iSimp[0];
						fluidData[ind+1] = iSimp[1];
						fluidData[ind+2] = 0;
						fluidData[ind+3] = 0;
						
						
					}	
				}
			}
		}
		
		
		//if (curTick == 0) {
			// clear water ids and edges
			for (i = 0; i < totSize; i++) {
				ind = i;
				fluidData[ind*4+E_PTT_IDE] = 0;
				fluidData[ind*4+E_PTT_STB] = 0;
				lastFluidData[ind*4+E_PTT_WAT] = fluidData[ind*4+E_PTT_WAT];
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
			
			
			while (modStack.size() > 0) {
				applyUnitModification(
					&(modStack.back().basePos),
					modStack.back().brushAction,
					modStack.back().modType,
					modStack.back().radius
				);
				modStack.pop_back();
			}
		//}
		
		
		
		
		
		// find stable regions
		fluidBodyCount = 1;
		for (i = minV0; i <= minV1; i++) {
			ind = i;
			
			
			watVal = &(fluidData[ind*4+E_PTT_WAT]);
			ideVal = &(fluidData[ind*4+E_PTT_STB]);
			
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
			ideVal = &(fluidData[ind*4+E_PTT_IDE]);
			
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
				
				
				
				terVal = &(fluidData[ind*4+E_PTT_TER]);
				watVal = &(fluidData[ind*4+E_PTT_WAT]);
				
				isAir = 
					(*watVal == UNIT_MIN) &&
					(*terVal == UNIT_MIN);
				
				isEmptyWater = (*watVal == 0);
				
				#ifdef DEBUG_BOUNDS
				if (isInBounds) {
					if (isAir) {cout << "isAir\n";}
					cout << "watVal " << *watVal << "\n";
				}
				#endif
				
				
				if (isAir) {
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
							
							//terVal2 = &(fluidData[testInd2*4+E_PTT_TER]);
							watVal3 = &(fluidData[testInd2*4+E_PTT_WAT]);
							
							// has (partially) full water cell to side
							if (*watVal3 >= 0) {
								
								// if unit below that one is earth
								testInd3 = testInd2 - volSizePrimBuf*volSizePrimBuf;
								if (
									(fluidData[testInd3*4+E_PTT_TER] > UNIT_MIN)
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
		
		
		
		
	
		// remove water from top of fluid body
		for (n = 0; n < fsVec.size(); n++) {
			
			
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
				//ideVal = &(fluidData[ind*4+E_PTT_IDE]);
				
				totWat += max(*watVal,0);
				
				// make sure no water is above or that there is earth above
				testInd = ind + (volSizePrimBuf*volSizePrimBuf);
				watVal2 = &(fluidData[testInd*4+E_PTT_WAT]);
				terVal2 = &(fluidData[testInd*4+E_PTT_TER]);
				
				if (
					(*watVal > 0) &&
					(
						(*watVal2 <= 0) ||
						(*terVal2 != UNIT_MIN)
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
		
		
		// add water to bottom of fluid body
		for (n = 0; n < fsVec.size(); n++) {
			
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
	
		
		if (didPrint) {
			watchMinX = -1;
		}
		
		
		curTick++;
		if (curTick == maxTicks) {
			curTick = 0;
		}
		
		
		
	}
bool GameFluid::findStableRegions (int startInd, int newId)
                                                        {
		indexStack.clear();
		indexStack.push_back(startInd);
		
		int dir;
		
		int ind;
		int testInd;
		int testInd2;
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
			fluidData[ind*4+E_PTT_STB] = newId;
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
				ideVal = &(fluidData[testInd*4+E_PTT_STB]);
				
				
				
				isAir = 
					(*watVal == UNIT_MIN) &&
					(*terVal == UNIT_MIN);
					
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
bool GameFluid::floodFillId (int startInd, int newId)
                                                  {
		
		indexStack.clear();
		indexStack.push_back(startInd);
		
		int dir;
		
		int ind;
		int testInd;
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
			fluidData[ind*4+E_PTT_IDE] = newId;
			
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
				
				if (fluidData[testInd*4+E_PTT_TER] != UNIT_MIN) {
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
				
				terVal = &(fluidData[testInd*4+E_PTT_TER]);
				watVal = &(fluidData[testInd*4+E_PTT_WAT]);
				ideVal = &(fluidData[testInd*4+E_PTT_IDE]);
				
				isAir = 
					(*watVal == UNIT_MIN) &&
					(*terVal == UNIT_MIN);
				
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
			
			
			terVal = &(fluidData[ind*4+E_PTT_TER]);
			watVal = &(fluidData[ind*4+E_PTT_WAT]);
			
			// remove ids from any air values so they can be reused
			if (
				(*watVal == UNIT_MIN) &&
				(*terVal == UNIT_MIN)	
			) {
				fluidData[ind*4+E_PTT_IDE] = 0;
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
bool GameFluid::inBounds (int i, int j, int k)
                                           {
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
void GameFluid::modifyUnit (FIVector4 * fPixelWorldCoordsBase, int brushAction, int modType, int radius)
          {
		modStack.push_back(ModUnitStruct());
		modStack.back().basePos.copyFrom(fPixelWorldCoordsBase);
		modStack.back().brushAction = brushAction;
		modStack.back().modType = modType;
		modStack.back().radius = radius;
	}
void GameFluid::applyUnitModification (FIVector4 * fPixelWorldCoordsBase, int brushAction, int modType, int radius)
          {
			
			FIVector4 newCoordMin;
			FIVector4 newCoordMax;
			newCoordMin.copyFrom(fPixelWorldCoordsBase);
			newCoordMax.copyFrom(fPixelWorldCoordsBase);
			
			newCoordMin.addXYZ(-radius + 1);
			newCoordMax.addXYZ( radius + 1);
			
			int iMin = clamp(newCoordMin.getIX(), vspMin, vspMax);
			int iMax = clamp(newCoordMax.getIX(), vspMin, vspMax);
			int jMin = clamp(newCoordMin.getIY(), vspMin, vspMax);
			int jMax = clamp(newCoordMax.getIY(), vspMin, vspMax);
			int kMin = clamp(newCoordMin.getIZ(), vspMin, vspMax);
			int kMax = clamp(newCoordMax.getIZ(), vspMin, vspMax);
			
			
			
			int i;
			int j;
			int k;
			
			int* terVal;
			int* watVal;
			// int* ideVal;
			// int* stbVal;
			
			int ind;
			
			for (k = kMin; k < kMax; k++) {
				for (j = jMin; j < jMax; j++) {
					for (i = iMin; i < iMax; i++) {
						
						ind = (i + j*volSizePrimBuf + k*volSizePrimBuf*volSizePrimBuf);
						
						terVal = &(fluidData[ind*4+E_PTT_TER]);
						watVal = &(fluidData[ind*4+E_PTT_WAT]);
						// ideVal = &(fluidData[ind*4+E_PTT_IDE]);
						// stbVal = &(fluidData[ind*4+E_PTT_STB]);
						
						switch (brushAction) {
							case E_BRUSH_MOVE:
								return;
							break;
							case E_BRUSH_ADD:
							
								if (modType == E_PTT_WAT) {
									if (*terVal == UNIT_MIN) {
										*watVal = UNIT_MAX;
									}
								}
								else {
									*terVal = UNIT_MAX;
									*watVal = UNIT_MIN;
								}
								
								
								
							break;
							case E_BRUSH_SUB:
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
#undef LZZ_INLINE
 
