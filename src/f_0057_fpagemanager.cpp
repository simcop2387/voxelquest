
struct PageThreadParam {
	fPage3D* threadPage;
	int opNum;
	int threadNum;
	float fParams[giNUM_THREAD_PARAMS];
};

PageThreadParam ptParams[giNUM_THREADS];


void *pageThreadStart( void *ptr )
{
	
	PageThreadParam *ptParam;
	ptParam = (PageThreadParam *) ptr;
	
	switch (ptParam->opNum) {
		case S_OP_SEAMLESS_SIMPLEX_NOISE:
			ptParam->threadPage->opSeamlessSimplexNoise(ptParam->fParams[0],ptParam->fParams[1],ptParam->fParams[2]);
		break;
		case S_OP_SIMPLEX_NOISE:
			ptParam->threadPage->opSimplexNoise(ptParam->fParams[0],ptParam->fParams[1],ptParam->fParams[2]);
		break;
		case S_OP_VORONOI:
			ptParam->threadPage->opVoronoi(ptParam->fParams[0],ptParam->fParams[1]);
		break;
		case S_OP_APPLY_TEXTURE:
			ptParam->threadPage->opApplyTexture();
		break;
		case S_RENDER_VOXELS:
			ptParam->threadPage->renderVoxels();
		break;
		default:
			cout << "WARNING: undefined operation in pageThreadStart: " << ptParam->opNum << "\n";
		break;
	}
	
	
	ptBusy[ptParam->threadNum] = false;
	//pthread_exit(0);

	return NULL;
}



class fGroup3D {
private:

public:
	iVector3 iPagesPerGroup;
	iVector3 iUnitsPerGroup;
	int iTotalPages;
	int iPageSize;

	fVector3 fPagesPerGroup;
	fVector3 fUnitsPerGroup;
	float fTotalPages;
	float fPageSize;

	fPage3D** pageArray;
	fPage3D* blankPage;
	fPage3D** textures;


	fGroup3D() {
		
	}

	void init(int _iPagesPerGroupX, int _iPagesPerGroupY, int _iPagesPerGroupZ, int _iPageSize, fPage3D** _textures=NULL) {
		int i;

		textures = _textures;

		iPagesPerGroup.x = _iPagesPerGroupX;
		iPagesPerGroup.y = _iPagesPerGroupY;
		iPagesPerGroup.z = _iPagesPerGroupZ;
		iPageSize = _iPageSize;


		iUnitsPerGroup.x = iPagesPerGroup.x*iPageSize;
		iUnitsPerGroup.y = iPagesPerGroup.y*iPageSize;
		iUnitsPerGroup.z = iPagesPerGroup.z*iPageSize;

		iTotalPages = iPagesPerGroup.x*iPagesPerGroup.y*iPagesPerGroup.z;

		
		fPageSize = (float)iPageSize;

		fPagesPerGroup.x = (float)iPagesPerGroup.x;
		fPagesPerGroup.y = (float)iPagesPerGroup.y;
		fPagesPerGroup.z = (float)iPagesPerGroup.z;

		fUnitsPerGroup.x = (float)iUnitsPerGroup.x;
		fUnitsPerGroup.y = (float)iUnitsPerGroup.y;
		fUnitsPerGroup.z = (float)iUnitsPerGroup.z;

		fTotalPages = (float)iTotalPages;

		pageArray = new fPage3D*[iTotalPages];
		blankPage = new fPage3D();
		blankPage->init(_iPageSize, 0, 0, 0, textures);
		blankPage->opClear();


		for (i = 0; i < iTotalPages; i++) {
			pageArray[i] = NULL;
		}

	}



	void opPagesAt(iVector3 curPos, int loadRad, int maxLoads, int ruleSet) {
		int i;
		int j;


		int tx;
		int ty;
		int tz;

		int tx2;
		int ty2;
		int tz2;

		int ind;
		int ind2;
		int ind3;
		int curOp = OP_NULL;
		int testOp;

		int threadOp;
		int threadCount = 0;

		int createdCount = 0;

		int loadCount = 0;

		int testID;

		bool stillGoing;

		for (i = 0; (i < oGrid->offsetGridCumCounters[loadRad]) && (loadCount < maxLoads) && (createdCount < 200) && (threadCount < giNUM_THREADS); i++) {
			tx = oGrid->offsetGridCum[i].x + (curPos.x/iPageSize);
			ty = oGrid->offsetGridCum[i].y + (curPos.y/iPageSize);
			tz = oGrid->offsetGridCum[i].z + (curPos.z/iPageSize);

			curOp = OP_NULL;
			testOp = OP_NULL;

			if (tx < 0 || tx >= iPagesPerGroup.x || ty < 0 || ty >= iPagesPerGroup.y || tz < 0 || tz >= iPagesPerGroup.z) {
				//out of bounds
			}
			else {


				ind = tx + ty*(iPagesPerGroup.x) + tz*(iPagesPerGroup.x*iPagesPerGroup.y);
				
				createdCount = 0;

				if (pageArray[ind] == NULL) {
					pageArray[ind] = new fPage3D();
					pageArray[ind]->init(iPageSize, tx, ty, tz, textures);
					createdCount++;
				}

				

				for (j = 0; j < 27; j++) {
					tx2 = tx + static_nxmods[j];
					ty2 = ty + static_nymods[j];
					tz2 = tz + static_nzmods[j];

					ind2 = (static_nxmods[j]+1) + (static_nymods[j]+1)*3 + (static_nzmods[j]+1)*9;


					if (tx2 < 0 || tx2 >= iPagesPerGroup.x || ty2 < 0 || ty2 >= iPagesPerGroup.y || tz2 < 0 || tz2 >= iPagesPerGroup.z) {
						pageArray[ind]->neighbors[ind2] = blankPage;
					}
					else {

						ind3 = tx2 + ty2*(iPagesPerGroup.x) + tz2*(iPagesPerGroup.x*iPagesPerGroup.y);

						if (pageArray[ind]->neighbors[ind2] == NULL) {
							if (pageArray[ind3] == NULL) {
								pageArray[ind3] = new fPage3D();
								pageArray[ind3]->init(iPageSize, tx2, ty2, tz2, textures);
								createdCount++;
							}
							pageArray[ind]->neighbors[ind2] = pageArray[ind3];
						}
					}
				}

				testOp = pageArray[ind]->currentState;

				


				switch (ruleSet) {
					case RS_TERRAIN:
						
						switch (testOp) {
							case E_INIT:
								curOp = S_OP_APPLY_TEXTURE;
							break;
							case E_OP_APPLY_TEXTURE:
								if (pageArray[ind]->testStates(E_OP_APPLY_TEXTURE)) {
									curOp = S_RENDER_VOXELS;
								}
							break;
							case E_RENDER_VOXELS:
								curOp = S_CREATE_DL;
							break;
							default:
								curOp = OP_NULL;
							break;
						}

					break;
				}


				threadOp = OP_NULL;


				switch (curOp) {
					
					case S_OP_CLEAR:
						pageArray[ind]->opClear();
						loadCount += 1;
					break;
					case S_OP_SIMPLEX_NOISE:
						threadOp = S_OP_SIMPLEX_NOISE;
						loadCount += 1;
					break;
					case S_OP_APPLY_TEXTURE:
						threadOp = S_OP_APPLY_TEXTURE;
						loadCount += 1;
					break;
					case S_RENDER_VOXELS:
						threadOp = S_RENDER_VOXELS;
						loadCount += 1;
					break;
					case S_CREATE_DL:
						pageArray[ind]->createDL();
						loadCount += giNUM_THREADS/2;
					break;
					default:
						loadCount += 0;
					break;
				}

				stillGoing = true;

				if (threadOp != OP_NULL) {
					while (stillGoing && (threadCount < giNUM_THREADS) ) {
						if (ptBusy[threadCount]) {
							threadCount++;
						}
						else {
							stillGoing = false;
						}
					}
					if (threadCount < giNUM_THREADS) {
						ptBusy[threadCount] = true;
						ptParams[threadCount].threadPage = pageArray[ind];
						ptParams[threadCount].opNum = threadOp;
						ptParams[threadCount].threadNum = threadCount;
						testID = pthread_create( &threadStore[threadCount], NULL, pageThreadStart, (void*) &(ptParams[threadCount]));
						threadCount++;
					}
					
				}

				/*
				
				*/

				loadCount += createdCount;
				
				
			}
		}
	}

	void doRender(iVector3 curPos, int loadRad, int renderState) {
		int i;

		int tx;
		int ty;
		int tz;

		int ox;
		int oy;
		int oz;

		int ind;

		for (i = 0; i < oGrid->offsetGridCumCounters[loadRad]; i++) {
			ox = oGrid->offsetGridCum[i].x;
			oy = oGrid->offsetGridCum[i].y;
			oz = oGrid->offsetGridCum[i].z;

			tx = ox + (curPos.x/iPageSize);
			ty = oy + (curPos.y/iPageSize);
			tz = oz + (curPos.z/iPageSize);

			if (tx < 0 || tx >= iPagesPerGroup.x || ty < 0 || ty >= iPagesPerGroup.y || tz < 0 || tz >= iPagesPerGroup.z) {
				//out of bounds
			}
			else {


				ind = tx + ty*(iPagesPerGroup.x) + tz*(iPagesPerGroup.x*iPagesPerGroup.y);
				

				if (pageArray[ind] == NULL) {
					
				}
				else {
					pageArray[ind]->renderDL( ((int)sqrt(ox*ox+oy*oy+oz*oz))/giMIP_SCALE, renderState );
				}

				
				
			}
		}
	}

};






class PageManager {
private:
	
public:
	
	fGroup3D* fgTerrain;

	fPage3D* fpTextures[E_TEX_TOTAL];

	iVector3 curPos;

	PageManager() {
		
	}

	void init() {

		int i;
		int testID;
		bool doRun;

		for (i = 0; i < E_TEX_TOTAL; i++) {
			fpTextures[i] = new fPage3D();
			fpTextures[i]->init(giTEXTURE_SIZE, 0, 0, 0);
			fpTextures[i]->setAllNeighbors(fpTextures[i]);
		}

		for (i = 0; i < E_TEX_TOTAL; i++) {

			doRun = true;
			ptParams[i].threadPage = fpTextures[i];
			ptParams[i].threadNum = i;

			switch (i) {
				case E_TEX_NOISE:
					ptParams[i].opNum = S_OP_SIMPLEX_NOISE;
					ptParams[i].fParams[0] = 1.0f;
					ptParams[i].fParams[1] = 1.0f;
					ptParams[i].fParams[2] = 0.5f;
					//fpTextures[i]->opSimplexNoise(1.0f,1.0f,0.5f);
				break;
				case E_TEX_SIMPLEX:
					ptParams[i].opNum = S_OP_SEAMLESS_SIMPLEX_NOISE;
					ptParams[i].fParams[0] = 3.0f;
					ptParams[i].fParams[1] = 0.5f;
					ptParams[i].fParams[2] = 1.0f/64.0f;
					//fpTextures[i]->opSimplexNoise(4.0f,0.5,1.0f/32.0f);
				break;
				case E_TEX_VORONOI:
					ptParams[i].opNum = S_OP_VORONOI;
					ptParams[i].fParams[0] = 16.0f;
					ptParams[i].fParams[1] = 0.5f;
					//fpTextures[i]->opVoronoi(16.0f,0.5f);
				break;
				default:
					doRun = false;
					cout << "INVALID OP IN PAGEMANAGER INIT: " << i << "\n";
				break;
			}

			testID = pthread_create( &threadStore[i], NULL, pageThreadStart, (void*) &(ptParams[i]));

		}


		
		
		
		
				
		for (i = 0; i < E_TEX_TOTAL; i++) {
			pthread_join( threadStore[i], NULL);
		}
		

		fgTerrain = new fGroup3D();
		fgTerrain->init(giPAGES_PER_GROUP[0], giPAGES_PER_GROUP[1], giPAGES_PER_GROUP[2], giUNITS_PER_PAGE, fpTextures);

		curPos.x = 0;
		curPos.y = 0;
		curPos.z = 0;

	}

	void setPosition(float x, float y, float z) {
		curPos.x = (int)x;
		curPos.y = (int)y;
		curPos.z = (int)z;
	}

	void update() {
		fgTerrain->opPagesAt(curPos, giLOAD_RAD, giNUM_THREADS, RS_TERRAIN);
	}

	void doRender(int renderState) {
		fgTerrain->doRender(curPos, giRENDER_RAD, renderState);
	}

};