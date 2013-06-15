class fPage3D {
private:

public:
	


	int imods[6];
	int jmods[6];
	int kmods[6];

	int octantsX[8];
	int octantsY[8];
	int octantsZ[8];

	int nxmods[27];
	int nymods[27];
	int nzmods[27];

	int faceOffsets[72];



	int iPageSize;
	int iTotalUnits;

	iVector3 iOffsets;

	float fPageSize;
	float fTotalUnits;

	fVector3 fOffsets;

	float* fDataArray;

	int currentState;
	int fillState;

	fPage3D* neighbors[27];
	fPage3D** textures;

	
	GLuint unitDL;
	GLuint waterDL;

	float* mipMapsR[giMIP_LEVELS];
	float* mipMapsG[giMIP_LEVELS];
	float* mipMapsB[giMIP_LEVELS];
	float* mipMapsC[giMIP_LEVELS];
	unsigned int* mipMapsF[giMIP_LEVELS];

	bool noSolidFaces;
	bool noWaterFaces;


	

	fPage3D() {
		
	}

	inline bool testStates(int testState) {
		int i;
		bool res = true;

		for (i = 0; i < 27; i++) {
			if (neighbors[i] == NULL) {
				res = false;
			}
			else {
				if (neighbors[i]->currentState < testState) {
					if (neighbors[i]->currentState != E_OP_CLEAR) {
						res = false;
					}
				}
			}
		}
		return res;
	}

	void setAllNeighbors(fPage3D* fp) {
		int i;

		for (i = 0; i < 27; i++) {
			neighbors[i] = fp;
		}
	}

	inline float getIndex(int xval, int yval, int zval) {

		
		int xtest = xval + iPageSize;
		int ytest = yval + iPageSize;
		int ztest = zval + iPageSize;

		int nx = xtest/iPageSize;
		int ny = ytest/iPageSize;
		int nz = ztest/iPageSize;

		int lx = xtest%iPageSize;
		int ly = ytest%iPageSize;
		int lz = ztest%iPageSize;

		int newIndex = lx + ly*iPageSize + lz*iPageSize*iPageSize;

		//pushTrace(intToString(nx),intToString(ny),intToString(nz));
		//popTrace();

		
		fPage3D* curPage = neighbors[nx + ny*3 + nz*9];

		if (curPage == NULL) {
			return AIR_VAL;
		}
		else {
			switch (curPage->fillState) {
				case FILL_STATE_MIXED:
					return (curPage->fDataArray[newIndex]);
				break;
				case FILL_STATE_AIR:
					return AIR_VAL;
				break;
				case FILL_STATE_WATER:
					return WATER_VAL;
				break;
				case FILL_STATE_SOLID:
					return SOLID_THRESH;
				break;
				default:
					return AIR_VAL;
				break;

			}
		}

		
	}





	void init(int _iPageSize, int _iOffsetX, int _iOffsetY, int _iOffsetZ, fPage3D** _textures=NULL) {
		int i;
		int j;

		////


		noSolidFaces = true;
		noWaterFaces = true;


		for (i = 0; i < 6; i++) {
			imods[i] = static_imods[i];
			jmods[i] = static_jmods[i];
			kmods[i] = static_kmods[i];
		}
		for (i = 0; i < 8; i++) {
			octantsX[i] = static_octantsX[i];
			octantsY[i] = static_octantsY[i];
			octantsZ[i] = static_octantsZ[i];
		}

		for (i = 0; i < 27; i++) {
			nxmods[i] = static_nxmods[i];
			nymods[i] = static_nymods[i];
			nzmods[i] = static_nzmods[i];
		}

		for (i = 0; i < 72; i++) {
			faceOffsets[i] = static_faceOffsets[i];
		}


		////



		textures = _textures;

		currentState = S_INIT;
		fillState = FILL_STATE_MIXED;

		iPageSize = _iPageSize;
		fPageSize = (float)iPageSize;

		iOffsets.x = _iOffsetX*iPageSize;
		iOffsets.y = _iOffsetY*iPageSize;
		iOffsets.z = _iOffsetZ*iPageSize;

		fOffsets.x = (float)iOffsets.x;
		fOffsets.y = (float)iOffsets.y;
		fOffsets.z = (float)iOffsets.z;

		iTotalUnits = iPageSize*iPageSize*iPageSize;
		fTotalUnits = (float)iTotalUnits;

		fDataArray = new float[iTotalUnits];


		j = iTotalUnits;
		for (i = 0; i < giMIP_LEVELS; i++) {
			mipMapsR[i] = new float[j];
			mipMapsG[i] = new float[j];
			mipMapsB[i] = new float[j];
			mipMapsC[i] = new float[j];
			mipMapsF[i] = new unsigned int[j];
			
			j = j/8;
		}

		for (i = 0; i < 27; i++) {
			neighbors[i] = NULL;
		}

		

		currentState = E_INIT;
	}

	inline float sampleTexture(int texNum, int x, int y, int z) {
		int ind;

		ind = (x%giTEXTURE_SIZE) + (y%giTEXTURE_SIZE)*giTEXTURE_SIZE + (z%giTEXTURE_SIZE)*giTEXTURE_SIZE*giTEXTURE_SIZE;

		return textures[texNum]->fDataArray[ind];
	}

	void opApplyTexture() {
		currentState = S_OP_APPLY_TEXTURE;

		int i;

		int ix;
		int iy;
		int iz;

		float fx;
		float fy;
		float fz;

		float testVal1;
		float testVal2;
		float testVal3;
		float testVal4;
		float testVal5;

		bool empty;

		for (i = 0; i < iTotalUnits; i++) {
			ix = i%(iPageSize) + iOffsets.x;
			iy = (i%(iPageSize*iPageSize))/iPageSize + iOffsets.y;
			iz = i/(iPageSize*iPageSize) + iOffsets.z;

			fx = ((float)ix);
			fy = ((float)iy);
			fz = ((float)iz);

			
			
			testVal4 = simplexScaledNoise(
											8.0f, //octaves
											0.5f, //persistence (amount added in each successive generation)
											1.f/512.0f, //scale (frequency)
											0.0f,
											1.0f,
											fx,
											fy,
											fz
										);


			//sampleTexture(E_TEX_SIMPLEX, ix, 0, iz)*sampleTexture(E_TEX_SIMPLEX, (ix+7)/3, 0, (iz+5)/5);

			/*
			if (testVal1*testVal2 > 0.5f) {
				
				if (testVal3 > 3.0f) {
					fDataArray[i] = 1.0f;
				}
				else {
					fDataArray[i] = 0.0f;
				}
				
			}*/
			
			//else

			testVal5 = fy/gfUNITS_PER_GROUP[1];

			empty = false;

			if (testVal5 < testVal4) {
				
				/*
				//testVal1 = sampleTexture(E_TEX_SIMPLEX, ix, iy, iz);
				//testVal2 = sampleTexture(E_TEX_SIMPLEX, (ix+7)/3, (iy+3)/7, (iz+5)/5);
				testVal3 = sampleTexture(E_TEX_VORONOI, ix, iy, iz);

				if (testVal3 > testVal4*3.0) {
					fDataArray[i] = ROCK_VAL;
				}
				else {
					empty = true;
				}*/

				fDataArray[i] = ROCK_VAL;
			}
			else {
				empty = true;
			}

			if (empty) {
				if (fy < gfSEA_LEVEL) {  //(fy + testVal4*32.0)
					fDataArray[i] = WATER_VAL;
				}
				else {
					fDataArray[i] = AIR_VAL;
				}
			}
			
		}

		currentState = E_OP_APPLY_TEXTURE;

	}

	void opSeamlessSimplexNoise(float oct, float pers, float freq) {
		
		currentState = S_OP_SEAMLESS_SIMPLEX_NOISE;

		int i;

		int ix;
		int iy;
		int iz;

		float fx;
		float fy;
		float fz;

		float testVal;

		//float oct = 4.0;
		//float pers = 0.5;
		//float freq =- 1.0/gfTEXTURE_SIZE;

		float ts = gfTEXTURE_SIZE;


		for (i = 0; i < iTotalUnits; i++) {
			ix = i%(iPageSize);
			iy = (i%(iPageSize*iPageSize))/iPageSize;
			iz = i/(iPageSize*iPageSize);

			fx = ((float)ix) + fOffsets.x;
			fy = ((float)iy) + fOffsets.y;
			fz = ((float)iz) + fOffsets.z;

			//float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z ) {
			testVal = 
				(
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy, 	fz) * 		(ts-fx) *		(ts-fy) *	(ts-fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy, 	fz) * 		(fx) *			(ts-fy) *	(ts-fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy-ts, 	fz) * 		(ts-fx) *		(fy) *		(ts-fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy-ts, 	fz) * 		(fx) *			(fy) *		(ts-fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy, 	fz-ts) * 	(ts-fx) *		(ts-fy) *	(fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy, 	fz-ts) * 	(fx) *			(ts-fy) *	(fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy-ts, 	fz-ts) * 	(ts-fx) *		(fy) *		(fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy-ts, 	fz-ts) * 	(fx) *			(fy) *		(fz)
				)/(ts*ts*ts);

			fDataArray[i] = testVal;

			
		}

		currentState = E_OP_SEAMLESS_SIMPLEX_NOISE;
	}

	void opVoronoi(float _fPointScale, float _fPointDensity) {
		
		currentState = S_OP_VORONOI;

		int iVoroTexSize;
		float fVoroTexSize;

		int iPointScale;
		int iPointGridSize;
		float fPointScale;
		float fPointGridSize;
		float fPointDensity;


		

		fPointDensity = _fPointDensity;
		fPointScale = _fPointScale;
		iPointScale = (int)fPointScale;
		
		
		iVoroTexSize = iPageSize;
		iPointGridSize = iVoroTexSize/iPointScale;
		
		fVoroTexSize = (float)iVoroTexSize;
		fPointGridSize = (float)iPointGridSize;
		


		//Dynamic Allocs
		int* idArray = new int[iVoroTexSize*iVoroTexSize*iVoroTexSize];
		float* pointGridBlanks = new float[iPointGridSize*iPointGridSize*iPointGridSize];
		Vector3* pointGrid = new Vector3[iPointGridSize*iPointGridSize*iPointGridSize];
		
		
		float pointBuf = 0.1f;
		float pointBufInv = 1.0-(pointBuf*2.0);
		
		
		int i;
		int j;
		int k;
		
		int i2;
		int j2;
		int k2;
		
		int m;
		int n;
		
		int voroRad = 2;
		
		float disMax = 1000000.0f;
		
		float testDis;
		float dis1;
		float dis2;
		
		
		
		int pointCount;
		
		int curInd;
		int curInd2;
		int idVal;
		
		float x;
		float y;
		float z;
		
		float xo;
		float yo;
		float zo;
		
		
		float imod;
		float jmod;
		float kmod;
		
		
		int ioffset;
		int joffset;
		int koffset;
		
		bool stillGoing;
		
		Vector3 startPoint;
		Vector3 endPoint;
		Vector3 bestPoint;
		
		////////////////////////
		//Generate Random Points
		////////////////////////
		
		for (i = 0; i < iPointGridSize; i++) {
			x = ((float)i);
			
			for (j = 0; j < iPointGridSize; j++) {
				y = ((float)j);
				
				for (k = 0; k < iPointGridSize; k++) {
					z = (float(k));
					
					
					if (fGenRand() >= fPointDensity) {
						xo = -1.0f;
						yo = -1.0f;
						zo = -1.0f;
					}
					else {
						xo = (fGenRand()*pointBufInv + pointBuf + x)*fPointScale;
						yo = (fGenRand()*pointBufInv + pointBuf + y)*fPointScale;
						zo = (fGenRand()*pointBufInv + pointBuf + z)*fPointScale;
					}
					
					
					
					curInd = i + j*iPointGridSize + k*iPointGridSize*iPointGridSize;
					
					pointGrid[curInd].setXYZ(xo, yo, zo);
					pointGridBlanks[curInd] = fGenRand();
					
				}
			}
		}
		
		
		
		////////////////////////////////////////////////////////////////////
		//Make sure every space has at least 2 points within 1 block radius
		////////////////////////////////////////////////////////////////////
		
		
		
		for (i = 0; i < iPointGridSize; i++) {
			x = ((float)i);
			
			for (j = 0; j < iPointGridSize; j++) {
				y = ((float)j);
				
				for (k = 0; k < iPointGridSize; k++) {
					z = (float(k));
					
					
					
					
					pointCount = 0;
					stillGoing = true;
					
					for (m = 0; (m <= voroRad)&&(stillGoing); m++) {
						for (n = 0; (n < oGrid->offsetGridCounters[m])&&(stillGoing); n++) {
							
							
							ioffset = i + oGrid->offsetGrid[m][n].x;
							joffset = j + oGrid->offsetGrid[m][n].y;
							koffset = k + oGrid->offsetGrid[m][n].z;

							//wrap offsets

							if (ioffset < 0) {
								ioffset = iPointGridSize + ioffset;
							}
							if (ioffset >= iPointGridSize) {
								ioffset = ioffset-iPointGridSize;
							}

							if (joffset < 0) {
								joffset = iPointGridSize + joffset;
							}
							if (joffset >= iPointGridSize) {
								joffset = joffset-iPointGridSize;
							}

							if (koffset < 0) {
								koffset = iPointGridSize + koffset;
							}
							if (koffset >= iPointGridSize) {
								koffset = koffset-iPointGridSize;
							}
							
							
							curInd = ioffset + joffset*iPointGridSize + koffset*iPointGridSize*iPointGridSize;
							
							if (pointGrid[curInd].x() >= 0.0f) {
								pointCount++;
							}
							
							if (pointCount >= 2) {
								stillGoing = false;
							}
							
						}
					}
					
					
					if (stillGoing) {
						//less than 2 points found, generate 1
						
						xo = (fGenRand()*pointBufInv + pointBuf + x)*fPointScale;
						yo = (fGenRand()*pointBufInv + pointBuf + y)*fPointScale;
						zo = (fGenRand()*pointBufInv + pointBuf + z)*fPointScale;
						curInd = i + j*iPointGridSize + k*iPointGridSize*iPointGridSize;
						pointGrid[curInd].setXYZ(xo, yo, zo);
						
					}
					else {
						//2 or more points found
					}	
					
				}
			}
		}
		
		
		
		///////////////////////////////
		//Generate the Voronoi Diagram
		///////////////////////////////
		
		
		for (i2 = 0; i2 < iVoroTexSize; i2++) {
			x = ((float)i2);
			i = i2/iPointScale;

			for (j2 = 0; j2 < iVoroTexSize; j2++) {
				y = ((float)j2);
				j = j2/iPointScale;

				for (k2 = 0; k2 < iVoroTexSize; k2++) {
					z = (float(k2));
					k = k2/iPointScale;
					
					
					dis1 = disMax;
					dis2 = disMax;
					
					
					startPoint.setXYZ(x,y,z);
					
					for (m = 0; m <= voroRad; m++) {
						for (n = 0; n < oGrid->offsetGridCounters[m]; n++) {

							
							
							
							ioffset = i + oGrid->offsetGrid[m][n].x;
							joffset = j + oGrid->offsetGrid[m][n].y;
							koffset = k + oGrid->offsetGrid[m][n].z;

							//wrap offsets
							
							imod = 0.0f;
							jmod = 0.0f;
							kmod = 0.0f;
							
							if (ioffset < 0) {
								ioffset = iPointGridSize + ioffset;
								imod = -fVoroTexSize;
							}
							if (ioffset >= iPointGridSize) {
								ioffset = ioffset-iPointGridSize;
								imod = fVoroTexSize;
							}

							if (joffset < 0) {
								joffset = iPointGridSize + joffset;
								jmod = -fVoroTexSize;
							}
							if (joffset >= iPointGridSize) {
								joffset = joffset-iPointGridSize;
								jmod = fVoroTexSize;
							}

							if (koffset < 0) {
								koffset = iPointGridSize + koffset;
								kmod = -fVoroTexSize;
							}
							if (koffset >= iPointGridSize) {
								koffset = koffset-iPointGridSize;
								kmod = fVoroTexSize;
							}
							
							
							
							
							curInd = ioffset + joffset*iPointGridSize + koffset*iPointGridSize*iPointGridSize;
							
							endPoint.setXYZ(pointGrid[curInd].e[0] + imod, pointGrid[curInd].e[1] + jmod, pointGrid[curInd].e[2] + kmod);
							
							if (pointGrid[curInd].x() >= 0.0f) {
								testDis = distance(&startPoint, &endPoint);
								
								if (testDis < dis1) {
									
									idVal = curInd;
									
									bestPoint.setXYZ(endPoint.e[0],endPoint.e[1],endPoint.e[2]);
									
									dis2 = dis1;
									dis1 = testDis;
									
								}
								else {
									if (testDis < dis2) {
										dis2 = testDis;
									}
								}
								
							}
							
						}
					}
					
					curInd2 = i2 + j2*iVoroTexSize + k2*iVoroTexSize*iVoroTexSize;
					
					
					
					fDataArray[curInd2] = fabs(dis2-dis1);///max(dis1,dis2);
					idArray[curInd2] = idVal;
					//vecArray[curInd2].setXYZ(bestPoint.e[0],bestPoint.e[1],bestPoint.e[2]);
					
				}
			}
		}

		currentState = E_OP_VORONOI;
		
	}

	/*
	void opVoronoi(float fBlockSize, float fDensity) {
		

		int i;
		int j;
		int k;

		float buf = 0.1f;
		float buf2 = buf*2.0f;
		float ibuf = 1.0f-buf2;

		int iBlockSize = (int)fBlockSize;
		int iBlockSize2 = iBlockSize*2;
		int distBlockSize = iBlockSize2*iBlockSize2*iBlockSize2;
		int iCellsSize = iPageSize/iBlockSize;
		int iTotalCells = iCellsSize*iCellsSize*iCellsSize;


		float* distBlock = new float[distBlockSize];

		for (i = 0; i < distBlockSize; i++) {
			
		}

		for (i = 0; i < iTotalCells; i++) {
			ix = i%(iCellsSize);
			iy = (i%(iCellsSize*iCellsSize))/iCellsSize;
			iz = i/(iCellsSize*iCellsSize);

			fx = ((float)ix)+ (genRand()*ibuf)+buf;
			fy = ((float)iy)+ (genRand()*ibuf)+buf;
			fz = ((float)iz)+ (genRand()*ibuf)+buf;

			if (fGenRand() < fDensity) {
				
			}

		}



		int* closestIndex1 = new int[iTotalUnits];
		int* closestIndex2 = new int[iTotalUnits];
		float* distFromCenter1 = new float[iTotalUnits];
		float* distFromCenter2 = new float[iTotalUnits];




		for (i = 0; i < iTotalUnits; i++) {
			ix = i%(iPageSize);
			iy = (i%(iPageSize*iPageSize))/iPageSize;
			iz = i/(iPageSize*iPageSize);

			fx = ((float)ix) + fOffsets.x;
			fy = ((float)iy) + fOffsets.y;
			fz = ((float)iz) + fOffsets.z;

		}
	}
	*/

	void opSimplexNoise(float oct, float pers, float freq) {
		
		currentState = S_OP_SIMPLEX_NOISE;

		int i;

		int ix;
		int iy;
		int iz;

		float fx;
		float fy;
		float fz;

		float testVal;


		for (i = 0; i < iTotalUnits; i++) {
			ix = i%(iPageSize);
			iy = (i%(iPageSize*iPageSize))/iPageSize;
			iz = i/(iPageSize*iPageSize);

			fx = ((float)ix) + fOffsets.x;
			fy = ((float)iy) + fOffsets.y;
			fz = ((float)iz) + fOffsets.z;

			//float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z ) {
			testVal = simplexScaledNoise(
											oct, //octaves
											pers, //persistence (amount added in each successive generation)
											freq, //scale (frequency)
											0.0f,
											1.0f,
											fx,
											fy,
											fz
										);

			fDataArray[i] = testVal;

			
		}

		currentState = E_OP_SIMPLEX_NOISE;
	}

	void opClear() {
		int i;

		currentState = S_OP_CLEAR;
		for (i = 0; i < iTotalUnits; i++) {
			fDataArray[i] = 0.0f;
		}
		currentState = E_OP_CLEAR;

		fillState = FILL_STATE_AIR;
	}

	

	void renderVoxels() {
		
		if (fDataArray == NULL) {
			cout << "WARNING RENDERVOXELS called with fDataArray == NULL\n";
			return;
		}

		currentState = S_RENDER_VOXELS;

		int i;
		int j;
		int k;
		int m;
		int n;
		
		Vector3 normVec;
		
		float curDis;
		float totalDis;
		
		int curInd;
		
		//bool touchingWater;
		//bool touchingAir;

		float shadowVal;
		float waterVal;
		float iWaterVal;
		float aoVal;
		
		float rval;
		float gval;
		float bval;
		
		float vecx;
		float vecy;
		float vecz;
		
		
		float fLightVal;
		int iLightVal;
		
		
		int aoRad = 2;
		int normRad = 3;
		
		float val1;
		float val2;
		
		
		

		int ix;
		int iy;
		int iz;

		float fx;
		float fy;
		float fz;

		int ishad;
		int jshad;
		int kshad;
		
		int mat;
		
		float fishad;
		float fjshad;
		float fkshad;

		float lv1;

		noSolidFaces = true;
		noWaterFaces = true;

		unsigned int faceBits;
		unsigned int bitMask[6] = {1,2,4,8,16,32};
		unsigned int solidBit = 64;
		unsigned int waterBit = 128;

		float shadFactor = gfUNITS_PER_PAGE/(gfRAY_STEPS*1.2);
		float shadFactor2 = gfUNITS_PER_PAGE/(gfRAY_STEPS*3.0);
		
		
		j = iTotalUnits;
		for (i = 0; i < giMIP_LEVELS; i++) {
			for (k = 0; k < j; k++) {
				mipMapsR[i][k] = 0.0f;
				mipMapsG[i][k] = 0.0f;
				mipMapsB[i][k] = 0.0f;
				mipMapsC[i][k] = 0.0f;
				mipMapsF[i][k] = 0;
			}
			j = j/8;
		}
		
		
		

		for (i = 0; i < iTotalUnits; i++) {
			ix = i%(iPageSize);
			iy = (i%(iPageSize*iPageSize))/iPageSize;
			iz = i/(iPageSize*iPageSize);

			fx = ((float)ix) + fOffsets.x;
			fy = ((float)iy) + fOffsets.y;
			fz = ((float)iz) + fOffsets.z;
			
			faceBits = 0;
			
			val1 = fDataArray[i];

			//touchingWater = false;
			//touchingAir = false;
			
			if (val1 >= SOLID_THRESH) {
				
				for (m = 0; m < 6; m++) {
					val2 = getIndex(ix+imods[m], iy+jmods[m], iz+kmods[m]);
					
					if (val2 < SOLID_THRESH) {
						faceBits = faceBits|bitMask[m];

						/*
						if (val2 == WATER_VAL) {
							touchingWater = true;
						}
						else {
							//touchingAir = true;
						}
						*/
					}

				}
				
				if (faceBits > 0) {
					noSolidFaces = false;


					//################


					//find shadow value
					
					shadowVal = 0.0f;
					aoVal = 0.0f;
					totalDis = 0.0f;
					vecx = 0.0f;
					vecy = 0.0f;
					vecz = 0.0f;
					
					for (m = 1; m <= giRAY_STEPS; m++) {
						
						rval = ((float)m)*shadFactor+1.0f;//+fGenRand()*3.0f;
						
						ishad = (int)(-gvLightDir.e[0]*rval);
						jshad = (int)(-gvLightDir.e[1]*rval);
						kshad = (int)(-gvLightDir.e[2]*rval);
						
						val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);
						
						if (val2 >= SOLID_THRESH) {
							shadowVal += 1.0f;
						}
					}
					
					for (n = 1; n < normRad; n++) {
						
						
						
						for (m = 0; m < oGrid->offsetGridCounters[n]; m++) {

						
							ishad = oGrid->offsetGrid[n][m].x;
							jshad = oGrid->offsetGrid[n][m].y;
							kshad = oGrid->offsetGrid[n][m].z;

							fishad = (float)ishad;
							fjshad = (float)jshad;
							fkshad = (float)kshad;

							curDis = sqrt(fishad*fishad + fjshad*fjshad + fkshad*fkshad);

							val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);

							if (val2 < SOLID_THRESH) {
								vecx += fishad/curDis;
								vecy += fjshad/curDis;
								vecz += fkshad/curDis;
							}
						}
					}
					
					
					for (m = 0; m < oGrid->offsetGridCounters[aoRad]; m++) {

						ishad = oGrid->offsetGrid[aoRad][m].x*4.0f;
						jshad = oGrid->offsetGrid[aoRad][m].y*4.0f;
						kshad = oGrid->offsetGrid[aoRad][m].z*4.0f;

						fishad = (float)ishad;
						fjshad = (float)jshad;
						fkshad = (float)kshad;

						curDis = sqrt(fishad*fishad + fjshad*fjshad + fkshad*fkshad);

						val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);

						if (val2 < SOLID_THRESH) {
							aoVal += 1.0f/curDis;
						}
						totalDis += 1.0f/curDis;
					}
					
					
					
					normVec.setXYZ(vecx, vecy, vecz);
					normVec.makeUnitVector();
					
					aoVal = clamp(aoVal/totalDis);
					
					
					//waterVal = clamp(waterVal*2.0f/gfRAY_STEPS);
					//iWaterVal = 1.0f-waterVal;
					shadowVal = 1.0f-shadowVal/gfRAY_STEPS;
					shadowVal = shadowVal*shadowVal;

					lv1 = clamp(dot(-gvLightDir, normVec));
					fLightVal = clamp( (shadowVal*lv1 + (1.0-shadowVal)*aoVal*0.2)*0.8 + aoVal*0.2 );
					
					
					
					mat = val1;
					
					/*
					if (mat == 1) {
						if (normVec.e[1]+(genRand()-0.5)/3.0f < 0.1f) {
							mat = 3;
						}
					}
					
					if (mat == 1) {
						fLightVal = clamp(fLightVal + ((((float)((int)(x*z) % 237))/237.0f)-0.5f)/4.0f );
					}
					*/
					
					iLightVal = (int) (fLightVal*(gfMATERIAL_SLOTS-1.0f));
					
					rval = mainMaterials[mat].r[iLightVal];
					gval = mainMaterials[mat].g[iLightVal];
					bval = mainMaterials[mat].b[iLightVal];

					/*
					rval += (fLightVal*fLightVal*fLightVal)/3.0f;
					gval += (fLightVal*fLightVal*fLightVal)/3.0f;
					bval += (fLightVal*fLightVal*fLightVal)/3.0f;
					*/

					/*
					if (touchingWater && !touchingAir) {
						rval = rval*0.5f + 0.0f*0.5f;
						gval = gval*0.5f + 0.5*0.5f;
						bval = bval*0.5f + 1.0*0.5f;
					}
					*/
					

					//rval = ((float)ix)/fPageSize;
					//gval = ((float)iy)/fPageSize;
					//bval = ((float)iz)/fPageSize;
					
					//fLightVal = val1 - 1.0f;

					//fLightVal = floor(fLightVal*12.0f)/12.0f;

					//rval = shadowVal;
					//gval = shadowVal;
					//bval = shadowVal;

					//rval = fLightVal;
					//gval = fLightVal;
					//bval = fLightVal;

					faceBits |= solidBit;

					
					mipMapsR[0][i] = rval;
					mipMapsG[0][i] = gval;
					mipMapsB[0][i] = bval;
					mipMapsC[0][i] = 0.0f;
					mipMapsF[0][i] = faceBits;


					//################
					

					//Add a face if its on the edge to smooth between mipmaps
					if (ix == 0) {
						faceBits |= bitMask[0];
					}
					if (ix == iPageSize-1) {
						faceBits |= bitMask[1];
					}
					if (iy == 0) {
						faceBits |= bitMask[2];
					}
					if (iy == iPageSize-1) {
						faceBits |= bitMask[3];
					}
					if (iz == 0) {
						faceBits |= bitMask[4];
					}
					if (iz == iPageSize-1) {
						faceBits |= bitMask[5];
					}



					n = 2;
					for (m = 1; m < giMIP_LEVELS; m++) {
						
						curInd = ( ix/n) + (iy/n)*(iPageSize/n) + (iz/n)*(iPageSize/n)*(iPageSize/n);
						
						mipMapsR[m][ curInd ] += rval;
						mipMapsG[m][ curInd ] += gval;
						mipMapsB[m][ curInd ] += bval;
						mipMapsC[m][ curInd ] += 1.0f;
						mipMapsF[m][ curInd ] |= faceBits;
						//mipMapsF[m][ curInd ] &= (~waterBit);
						
						n = n*2;
					}
				}
				
				
			}
			else if (val1 == WATER_VAL) {





				for (m = 0; m < 6; m++) {
					val2 = getIndex(ix+imods[m], iy+jmods[m], iz+kmods[m]);
					
					if (val2 == AIR_VAL) {
						faceBits = faceBits|bitMask[m];
					}

				}
				
				if (faceBits > 0) {
					noWaterFaces = false;


					//################


					//find shadow value
					
					shadowVal = 0.0f;
					aoVal = 0.0f;
					totalDis = 0.0f;
					vecx = 0.0f;
					vecy = 0.0f;
					vecz = 0.0f;


					for (m = 1; m <= giRAY_STEPS; m++) {
						
						rval = ((float)m)*shadFactor2+1.0f;//+fGenRand()*3.0f;
						
						ishad = (int)(-gvLightDir.e[0]*rval);
						jshad = (int)(-gvLightDir.e[1]*rval);
						kshad = (int)(-gvLightDir.e[2]*rval);
						
						val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);
						
						if (val2 >= SOLID_THRESH) {
							shadowVal += 1.0f;
						}
					}
					
					for (n = 1; n < normRad; n++) {
						
						for (m = 0; m < oGrid->offsetGridCounters[n]; m++) {

						
							ishad = oGrid->offsetGrid[n][m].x;
							jshad = oGrid->offsetGrid[n][m].y;
							kshad = oGrid->offsetGrid[n][m].z;

							fishad = (float)ishad;
							fjshad = (float)jshad;
							fkshad = (float)kshad;

							curDis = sqrt(fishad*fishad + fjshad*fjshad + fkshad*fkshad);

							val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);

							if (val2 == AIR_VAL) {
								vecx += fishad/curDis;
								vecy += fjshad/curDis;
								vecz += fkshad/curDis;
							}
						}
					}



					for (m = 0; m < oGrid->offsetGridCounters[aoRad]; m++) {

						ishad = oGrid->offsetGrid[aoRad][m].x*4.0f;
						jshad = oGrid->offsetGrid[aoRad][m].y*4.0f;
						kshad = oGrid->offsetGrid[aoRad][m].z*4.0f;

						fishad = (float)ishad;
						fjshad = (float)jshad;
						fkshad = (float)kshad;

						curDis = sqrt(fishad*fishad + fjshad*fjshad + fkshad*fkshad);

						val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);

						if (val2 >= SOLID_THRESH) {
							aoVal += 1.0f/curDis;
						}
						totalDis += 1.0f/curDis;
					}
					
					shadowVal = 1.0f-shadowVal/gfRAY_STEPS;
					shadowVal = shadowVal*shadowVal;
					aoVal = clamp(aoVal/totalDis);
					normVec.setXYZ(vecx, vecy, vecz);
					normVec.makeUnitVector();

					lv1 = clamp(dot(-gvLightDir, normVec));
					fLightVal = clamp((lv1+aoVal)*shadowVal);//clamp(lv1+aoVal);//clamp( (shadowVal*lv1 + (1.0-shadowVal)*aoVal*0.2)*0.8 + aoVal*0.2 );
					
					
					
					mat = val1;
					
					
					iLightVal = (int) (fLightVal*(gfMATERIAL_SLOTS-1.0f));
					
					rval = mainMaterials[mat].r[iLightVal];
					gval = mainMaterials[mat].g[iLightVal];
					bval = mainMaterials[mat].b[iLightVal];

					
					rval = rval + aoVal;
					gval = gval + aoVal;
					bval = bval + aoVal;
					

					//rval = ((float)ix)/fPageSize;
					//gval = ((float)iy)/fPageSize;
					//bval = ((float)iz)/fPageSize;
					
					//fLightVal = val1 - 1.0f;

					//fLightVal = floor(fLightVal*12.0f)/12.0f;

					//rval = shadowVal;
					//gval = shadowVal;
					//bval = shadowVal;

					//rval = fLightVal;
					//gval = fLightVal;
					//bval = fLightVal;

					faceBits |= waterBit;

					
					mipMapsR[0][i] = rval;
					mipMapsG[0][i] = gval;
					mipMapsB[0][i] = bval;
					mipMapsC[0][i] = 0.0f;
					mipMapsF[0][i] = faceBits;


					//################
					
					/*

					//Add a face if its on the edge to smooth between mipmaps
					if (ix == 0) {
						faceBits |= bitMask[0];
					}
					if (ix == iPageSize-1) {
						faceBits |= bitMask[1];
					}
					if (iy == 0) {
						faceBits |= bitMask[2];
					}
					if (iy == iPageSize-1) {
						faceBits |= bitMask[3];
					}
					if (iz == 0) {
						faceBits |= bitMask[4];
					}
					if (iz == iPageSize-1) {
						faceBits |= bitMask[5];
					}
					*/



					n = 2;
					for (m = 1; m < giMIP_LEVELS; m++) {
						
						curInd = ( ix/n) + (iy/n)*(iPageSize/n) + (iz/n)*(iPageSize/n)*(iPageSize/n);
						
						mipMapsR[m][ curInd ] += rval;
						mipMapsG[m][ curInd ] += gval;
						mipMapsB[m][ curInd ] += bval;
						mipMapsC[m][ curInd ] += 1.0f;
						mipMapsF[m][ curInd ] |= faceBits;
						/*
						if ( (mipMapsF[m][ curInd ] & solidBit) == 0) {
							mipMapsF[m][ curInd ] |= waterBit;
						}
						*/
						
						
						n = n*2;
					}
				}

			}
		}
				
		
		
		
		j = (iTotalUnits)/8;
		for (i = 1; i < giMIP_LEVELS; i++) {
			for (k = 0; k < j; k++) {
				mipMapsR[i][k] = mipMapsR[i][k]/mipMapsC[i][k];
				mipMapsG[i][k] = mipMapsG[i][k]/mipMapsC[i][k];
				mipMapsB[i][k] = mipMapsB[i][k]/mipMapsC[i][k];
			}
			j = j/8;
		}

		//cout << noSolidFaces << " " << noWaterFaces << "\n" ;

		if (noSolidFaces && noWaterFaces) {
			if (fDataArray) {
				if (fDataArray[0] == AIR_VAL) {
					fillState = FILL_STATE_AIR;
				}
				else if (fDataArray[0] == WATER_VAL) {
					fillState = FILL_STATE_WATER;
				}
				else {
					fillState = FILL_STATE_SOLID;
				}
			}
			else {
				fillState = FILL_STATE_AIR;
			}
			
		}
		else {
			
			fillState = FILL_STATE_MIXED;
		}
		
		currentState = E_RENDER_VOXELS;
	}


	void createDL() {

		

		if (fDataArray == NULL) {
			cout << "WARNING CREATEDL called with fDataArray == NULL\n";
			return;
		}

		currentState = S_CREATE_DL;

		int i;
		int j;
		int k;
		int m;
		int n;

		int ix;
		int iy;
		int iz;

		float fx;
		float fy;
		float fz;

		
		int mult;
		float fmult;

		float val2;
		
		int curSize;
		int curTot;

		int colCounter = 0;
		int curMipLevel = 0;
		
		int curInd;
		int curInd2;
		int cvSize;
		int mipCounter;

		unsigned int waterInd;

		unsigned int faceBits;
		unsigned int bitVal;
		unsigned int bitMask[6] = {1,2,4,8,16,32};
		unsigned int solidBit = 64;
		unsigned int waterBit = 128;

		int minInd;
		int maxInd;

		bool procFlag;

		
		if (fillState != FILL_STATE_MIXED) {

			if (fDataArray != NULL) {
				delete[] fDataArray;
				fDataArray = NULL;
			}
		}
		else {

			if (noSolidFaces) {
				minInd = 1;
			}
			else {
				unitDL = glGenLists(giMIP_LEVELS);
				minInd = 0;
			}

			if (noWaterFaces) {
				maxInd = 1;
			}
			else {
				waterDL = glGenLists(giMIP_LEVELS);
				maxInd = 2;
				
			}

			for (waterInd = minInd; waterInd < maxInd; waterInd++) {
				

				for (curMipLevel = 0; curMipLevel < giMIP_LEVELS; curMipLevel++) {
					
					if (waterInd == 0) {
						glNewList(unitDL+curMipLevel,GL_COMPILE);
					}
					else {
						glNewList(waterDL+curMipLevel,GL_COMPILE);
					}



					
					glBegin(GL_QUADS);


					curSize = iPageSize;
					curTot = iTotalUnits;
					mult = 1;
					for (i = 0; i < curMipLevel; i++) {
						curSize = curSize / 2;
						curTot = curTot / 8;
						mult = mult*2;
					}
					fmult = (float)mult;


					for (i = 0; i < curTot; i++) {
						ix = i%(curSize);
						iy = (i%(curSize*curSize))/curSize;
						iz = i/(curSize*curSize);

						fx = ((float)ix*mult) + fOffsets.x;
						fy = ((float)iy*mult) + fOffsets.y;
						fz = ((float)iz*mult) + fOffsets.z;

						faceBits = mipMapsF[curMipLevel][i];

						
						if (waterInd == 0) {
							if (faceBits >= waterBit) {
								procFlag = false;
							}
							else {
								procFlag = true;
							}
						}
						else {
							if (faceBits >= waterBit) {
								procFlag = true;
							}
							else {
								procFlag = false;
							}
						}

						
						
						
						if ( procFlag && (faceBits > 0) ) { //
							

							glColor3f(mipMapsR[curMipLevel][i], mipMapsG[curMipLevel][i], mipMapsB[curMipLevel][i]);

							
							for (m = 0; m < 6; m++) {
								bitVal = faceBits & (bitMask[m]);
								if (bitVal > 0) {
									glVertex3f(faceOffsets[m*12+0]*fmult+fx, faceOffsets[m*12+1]*fmult+fy, faceOffsets[m*12+2]*fmult+fz);
									glVertex3f(faceOffsets[m*12+3]*fmult+fx, faceOffsets[m*12+4]*fmult+fy, faceOffsets[m*12+5]*fmult+fz);
									glVertex3f(faceOffsets[m*12+6]*fmult+fx, faceOffsets[m*12+7]*fmult+fy, faceOffsets[m*12+8]*fmult+fz);
									glVertex3f(faceOffsets[m*12+9]*fmult+fx, faceOffsets[m*12+10]*fmult+fy, faceOffsets[m*12+11]*fmult+fz);		
									
								}
							}
						}
					}
					

					glEnd();
					glEndList();
					
				}
			}
			
			
			
			for (curMipLevel = 0; curMipLevel < giMIP_LEVELS; curMipLevel++) {
				delete[] mipMapsR[curMipLevel];
				delete[] mipMapsG[curMipLevel];
				delete[] mipMapsB[curMipLevel];
				delete[] mipMapsC[curMipLevel];
				delete[] mipMapsF[curMipLevel];

				mipMapsR[curMipLevel] = NULL;
				mipMapsG[curMipLevel] = NULL;
				mipMapsB[curMipLevel] = NULL;
				mipMapsC[curMipLevel] = NULL;
				mipMapsF[curMipLevel] = NULL;
			}
			

			
			
		}
		
		currentState = E_CREATE_DL;
	}

	void drawCube(float cr, float cg, float cb) {
		glBegin(GL_QUADS);		// Draw The Cube Using quads
	    glColor3f(cr,cg,cb);	// Color Blue

	    float sx = fOffsets.x;
	    float sy = fOffsets.y;
	    float sz = fOffsets.z;
	    float ex = fOffsets.x + fPageSize;
	    float ey = fOffsets.y + fPageSize;
	    float ez = fOffsets.z + fPageSize;
	    
	    glVertex3f(ex,ey,sz);	// Top Right Of The Quad (Top)
	    glVertex3f(sx,ey,sz);	// Top Left Of The Quad (Top)
	    glVertex3f(sx,ey,ez);	// Bottom Left Of The Quad (Top)
	    glVertex3f(ex,ey,ez);	// Bottom Right Of The Quad (Top)

	    glVertex3f(ex,sy,ez);	// Top Right Of The Quad (Bottom)
	    glVertex3f(sx,sy,ez);	// Top Left Of The Quad (Bottom)
	    glVertex3f(sx,sy,sz);	// Bottom Left Of The Quad (Bottom)
	    glVertex3f(ex,sy,sz);	// Bottom Right Of The Quad (Bottom)

	    glVertex3f(ex,ey,ez);	// Top Right Of The Quad (Front)
	    glVertex3f(sx,ey,ez);	// Top Left Of The Quad (Front)
	    glVertex3f(sx,sy,ez);	// Bottom Left Of The Quad (Front)
	    glVertex3f(ex,sy,ez);	// Bottom Right Of The Quad (Front)

	    glVertex3f(ex,sy,sz);	// Top Right Of The Quad (Back)
	    glVertex3f(sx,sy,sz);	// Top Left Of The Quad (Back)
	    glVertex3f(sx,ey,sz);	// Bottom Left Of The Quad (Back)
	    glVertex3f(ex,ey,sz);	// Bottom Right Of The Quad (Back)

	    glVertex3f(sx,ey,ez);	// Top Right Of The Quad (Left)
	    glVertex3f(sx,ey,sz);	// Top Left Of The Quad (Left)
	    glVertex3f(sx,sy,sz);	// Bottom Left Of The Quad (Left)
	    glVertex3f(sx,sy,ez);	// Bottom Right Of The Quad (Left)

	    glVertex3f(ex,ey,sz);	// Top Right Of The Quad (Right)
	    glVertex3f(ex,ey,ez);	// Top Left Of The Quad (Right)
	    glVertex3f(ex,sy,ez);	// Bottom Left Of The Quad (Right)
	    glVertex3f(ex,sy,sz);	// Bottom Right Of The Quad (Right)
	  	glEnd();
	}

	inline void renderDL(int mipLev, int renderState) {
		if (fDataArray == NULL) {
			//cout << "WARNING RENDERDL called with fDataArray == NULL\n";
			return;
		}

		if (currentState == E_CREATE_DL) {
			if (fillState == FILL_STATE_MIXED) {

				switch (renderState) {
					case E_RENDER_SOLID:
						if (noSolidFaces) {

						}
						else {
							glCallList(unitDL + max(
											min(
												mipLev,
												(giMIP_LEVELS-1)
											),
											giMIN_MIP
										)
							);
						}
					break;
					case E_RENDER_WATER:
						if (noWaterFaces) {

						}
						else {
							glCallList(waterDL + max(
											min(
												mipLev,
												(giMIP_LEVELS-1)
											),
											giMIN_MIP
										)
							);
						}
						
					break;
					default:
						cout << "ERROR INVALID RENDER STATE\n";
						return;
				}

				
				//drawCube(fOffsets.x/100.0f,fOffsets.y/100.0f,fOffsets.z/100.0f);
			}
		}
	}
	
	
};