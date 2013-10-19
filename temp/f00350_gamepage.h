// f00350_gamepage.h
//

#include "f00350_gamepage.e"
#define LZZ_INLINE inline
GamePage::GamePage ()
                   {

	}
void GamePage::init (Singleton * _singleton, int _thisPageId, FIVector4 * _offsetInUnits)
                                                                                     {
		thisPageId = _thisPageId;
		singleton = _singleton;
		usingPoolId = -1;

		maxEntries = 32;


		int i;

		
		threshVal = 140;
		threadRunning = false;


		maxHeightInUnits = (singleton->maxHeightInUnits);

		isDirty = false;

		fillState = E_FILL_STATE_PARTIAL;
		curState = E_STATE_INIT_BEG;
		nextState = E_STATE_WAIT;





		bufferedPageSizeInUnits = (singleton->visPageSizeInUnits) * (singleton->bufferMult);
		offsetInUnits.copyFrom(_offsetInUnits);

		unitSizeInPixels = (float)(singleton->unitSizeInPixels);


		worldSeed.copyFrom(&(singleton->worldSeed));



		iVolumeSize = bufferedPageSizeInUnits*bufferedPageSizeInUnits*bufferedPageSizeInUnits;
		volData = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volData[i] = 0;
		}

		volDataLinear = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volDataLinear[i] = (255<<24)|(255<<16)|(255<<8)|(0);
		}


		totLenO2 = bufferedPageSizeInUnits/2;
		totLenVisO2 = bufferedPageSizeInUnits/(2*(singleton->bufferMult));



		worldMinVisInPixels.copyFrom(&offsetInUnits);
		worldMaxVisInPixels.copyFrom(&offsetInUnits);
		worldMinVisInPixels.addXYZ( -totLenVisO2 );
		worldMaxVisInPixels.addXYZ(  totLenVisO2 );
		worldMinVisInPixels.multXYZ((float)unitSizeInPixels);
		worldMaxVisInPixels.multXYZ((float)unitSizeInPixels);

		worldMinBufInPixels.copyFrom(&offsetInUnits);
		worldMaxBufInPixels.copyFrom(&offsetInUnits);
		worldMinBufInPixels.addXYZ( -totLenO2 );
		worldMaxBufInPixels.addXYZ(  totLenO2 );
		worldMinBufInPixels.multXYZ((float)unitSizeInPixels);
		worldMaxBufInPixels.multXYZ((float)unitSizeInPixels);



		worldUnitMin.copyFrom(&offsetInUnits);
		worldUnitMax.copyFrom(&offsetInUnits);
		worldUnitMin.addXYZ( -totLenO2 );
		worldUnitMax.addXYZ(  totLenO2 );

		

		curState = E_STATE_INIT_END;
	}
uint GamePage::NumberOfSetBits (uint i)
        {
	    i = i - ((i >> 1) & 0x55555555);
	    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
	}
uint GamePage::clamp (uint val)
                             {
		if (val > 255) {
			val = 255;
		}
		if (val < 0) {
			val = 0;
		}
		return val;
	}
void GamePage::createSimplexNoise ()
                                  {

		threadRunning = true;

		// REMINDER: DO NOT ACCESS EXTERNAL POINTERS INSIDE THREAD

		bool isBlank = false;
		bool isFull = false;
		

		curState = E_STATE_CREATESIMPLEXNOISE_BEG;

		int i, j, k, m;

		int totLen = bufferedPageSizeInUnits;

		float fTotLen = (float)totLen;

		int ind = 0;

		uint tmp;

		float fx, fy, fz;

		uint randOff[3];
		float ijkVals[3];

		const float RAND_MOD[9] = {
			3456.0f, 5965.0f, 45684.0f,
			4564.0f, 1234.0f, 6789.0f,
			4567.0f, 67893.0f, 13245.0f
		};

		float totLenO4 = totLen/4;
		float totLen3O4 = (totLen*3)/4;
		float fSimp;
		float heightThresh;
		float testVal;

		bool mustNotBeFull = false;

		for (j = 0; j < totLen; j++) {

			ijkVals[1] = (float)j;

			fy = (j - totLenO2) + offsetInUnits.getFY();

			for (i = 0; i < totLen; i++) {

				ijkVals[0] = (float)i;

				fx = (i - totLenO2) + offsetInUnits.getFX();
				
				for (k = 0; k < totLen; k++) {

					ijkVals[2] = (float)k;

					fz = (k - totLenO2) + offsetInUnits.getFZ();

					ind = k*totLen*totLen + j*totLen + i;

					
					
					if (fx < 0.0f || fy < 0.0f || fz < 0.0f ) {
						volData[ind] = 0;
						volDataLinear[ind] = (255<<16)|(255<<8)|255;
					}
					else {

						heightThresh = (fz/ ((float)maxHeightInUnits) );
						if (heightThresh > 1.0f) {
							heightThresh = 1.0f;
						}
						if (heightThresh < 0.0f) {
							heightThresh = 0.0f;
						}



						if (k + offsetInUnits.getIZ() >= maxHeightInUnits) {
							tmp = 0;
							mustNotBeFull = true;
						}
						else {
							testVal = simplexScaledNoise(
												4.0f, //octaves
												0.5f, //persistence (amount added in each successive generation)
												1.0f/32.0f, //scale (frequency)
												0.0f,
												1.0f,
												fx+worldSeed.getFX(),
												fy+worldSeed.getFY(),
												fz+worldSeed.getFZ()
											);
							

							
							tmp = clamp(testVal*255.0f*(1.0f-heightThresh*heightThresh*heightThresh));
						}

						

						if ( i >= totLenO4 && i <= totLen3O4 ) {
							if ( j >= totLenO4 && j <= totLen3O4 ) {
								if ( k >= totLenO4 && k <= totLen3O4 ) {
									if (tmp > threshVal) {
										isBlank = false;
									}
									else {
										isFull = false;
									}
								}
							}
						}

						
						


						for (m = 0; m < 3; m++) {
							fSimp = simplexScaledNoise(
																		1.0f, //octaves
																		1.0f, //persistence (amount added in each successive generation)
																		1.0f/4.0, //scale (frequency)
																		0.0f,
																		1.0f,
																		fx+RAND_MOD[m*3+0],
																		fy+RAND_MOD[m*3+1],
																		fz+RAND_MOD[m*3+2]
																	);
							randOff[m] = clamp( ( fSimp + ijkVals[m])*255.0f/fTotLen);
							


						}

						if ( (tmp%16 > 5) && ( (i+j+k)%2 == 0) ) {

							/*if (randOff[0] == 0 && randOff[1] == 0 && randOff[2] == 0) {
								randOff[1] = 1;
							} */

							volData[ind] = (0)|(randOff[2]<<16)|(randOff[1]<<8)|randOff[0];
							volDataLinear[ind] = (tmp<<24)|(255<<16)|(255<<8)|255;
						}
						else {
							volData[ind] = (0);
							volDataLinear[ind] = (tmp<<24)|(255<<16)|(255<<8)|255;;
						}

						
					}
					
					

					
				}
			}
		}



		/*
		if (mustNotBeFull) {
			isFull = false;
		}

		if (isBlank || isFull ) {

			if (isBlank) {
				fillState = E_FILL_STATE_EMPTY;
			}
			if (isFull) {
				fillState = E_FILL_STATE_FULL;
			}

			curState = E_STATE_LENGTH;
		}
		else {*/

			fillState = E_FILL_STATE_PARTIAL;
			curState = E_STATE_CREATESIMPLEXNOISE_END;
		//}

		threadRunning = false;

	}
void GamePage::unbindGPUResources ()
                                  {
		usingPoolId = -1;
		curState = E_STATE_CREATESIMPLEXNOISE_END;
	}
void GamePage::copyToTexture ()
                             {

		curState = E_STATE_COPYTOTEXTURE_BEG;


		if (usingPoolId == -1) {
			usingPoolId = singleton->requestPoolId(thisPageId);
			gpuRes = singleton->pagePoolItems[usingPoolId];
		}

		

		glBindTexture(GL_TEXTURE_3D,gpuRes->volID);
			glTexSubImage3D(
				GL_TEXTURE_3D,
				0,
				
				0,
				0,
				0,

				bufferedPageSizeInUnits,
				bufferedPageSizeInUnits,
				bufferedPageSizeInUnits,

				GL_RGBA,
				GL_UNSIGNED_BYTE,

				volData
			);

		glBindTexture(GL_TEXTURE_3D,0);
		glBindTexture(GL_TEXTURE_3D,gpuRes->volIDLinear);
			glTexSubImage3D(
				GL_TEXTURE_3D,
				0,
				
				0,
				0,
				0,

				bufferedPageSizeInUnits,
				bufferedPageSizeInUnits,
				bufferedPageSizeInUnits,

				GL_RGBA,
				GL_UNSIGNED_BYTE,

				volDataLinear
			);
		glBindTexture(GL_TEXTURE_3D,0);

		

		curState = E_STATE_COPYTOTEXTURE_END;

	}
void GamePage::addGeom ()
                       {
		
		int i;
		int j;
		int k;
		int ind;

		int startX;
		int endX;
		int startY;
		int endY;

		int curId;
		int geomInPage;
		int baseInd;

		GameGeom* gg;

		paramsPerEntry = 18;
		numEntries = 0;//singleton->gameGeom.size();

		startX = ( worldMinBufInPixels.getIX() ) / singleton->geomPageSizeInPixels;
		startY = ( worldMinBufInPixels.getIY() ) / singleton->geomPageSizeInPixels;

		endX = ( worldMaxBufInPixels.getIX() ) / singleton->geomPageSizeInPixels;
		endY = ( worldMaxBufInPixels.getIY() ) / singleton->geomPageSizeInPixels;

		
		for (j = startY; j <= endY; j++) {
			for (i = startX; i <= endX; i++) {
				ind = j*(singleton->worldSizeInGeomPages.getIX()) + i;



				geomInPage = singleton->gw->geomData[ind].containsGeomIds.size();

				for (k = 0; k < geomInPage; k++) {
					curId = singleton->gw->geomData[ind].containsGeomIds[k];
					gg = singleton->gameGeom[curId];

					if (
						gg->boundsMinInPixels.inBoundsXYZ( &worldMinBufInPixels, &worldMaxBufInPixels ) ||
						gg->boundsMaxInPixels.inBoundsXYZ( &worldMinBufInPixels, &worldMaxBufInPixels ) ||
						worldMinBufInPixels.inBoundsXYZ( &(gg->boundsMinInPixels), &(gg->boundsMaxInPixels) ) ||
						worldMaxBufInPixels.inBoundsXYZ( &(gg->boundsMinInPixels), &(gg->boundsMaxInPixels) )
					) {
						

						baseInd = numEntries*paramsPerEntry;

						singleton->paramArr[baseInd + 0] = gg->boundsMinInPixels.getFX();
						singleton->paramArr[baseInd + 1] = gg->boundsMinInPixels.getFY();
						singleton->paramArr[baseInd + 2] = gg->boundsMinInPixels.getFZ();

						singleton->paramArr[baseInd + 3] = gg->boundsMaxInPixels.getFX();
						singleton->paramArr[baseInd + 4] = gg->boundsMaxInPixels.getFY();
						singleton->paramArr[baseInd + 5] = gg->boundsMaxInPixels.getFZ();

						singleton->paramArr[baseInd + 6] = gg->originInPixels.getFX();
						singleton->paramArr[baseInd + 7] = gg->originInPixels.getFY();
						singleton->paramArr[baseInd + 8] = gg->originInPixels.getFZ();

						singleton->paramArr[baseInd + 9] = gg->powerVals.getFX();
						singleton->paramArr[baseInd + 10] = gg->powerVals.getFY();
						singleton->paramArr[baseInd + 11] = gg->powerVals.getFZ();

						singleton->paramArr[baseInd + 12] = gg->coefficients.getFX();
						singleton->paramArr[baseInd + 13] = gg->coefficients.getFY();
						singleton->paramArr[baseInd + 14] = gg->coefficients.getFZ();

						singleton->paramArr[baseInd + 15] = gg->minMaxMat.getFX();
						singleton->paramArr[baseInd + 16] = gg->minMaxMat.getFY();
						singleton->paramArr[baseInd + 17] = gg->minMaxMat.getFZ();

						numEntries++;

					}

				}
			}
		}


		if (numEntries > maxEntries) {
			numEntries = maxEntries;
			doTrace("limit exceeded");
		}



		totParams = numEntries*paramsPerEntry;



	}
void GamePage::generateVolume ()
                              {

		int i;
		
		
		curState = E_STATE_GENERATEVOLUME_BEG;
		
		
		if (usingPoolId == -1) {

		}
		else {


			addGeom();

			// TODO: one shader, set flag

			if (singleton->isBare) {
				singleton->bindShader("GenerateVolumeBare");
			}
			else {
				singleton->bindShader("GenerateVolume");
			}



			


			singleton->bindFBO("volGenFBO");
			singleton->setShaderTexture3D(0,gpuRes->volID);
			singleton->setShaderTexture3D(1,gpuRes->volIDLinear);
			singleton->setShaderTexture(2,singleton->lookup2to3ID);
			//singleton->setShaderTexture(3,singleton->imageTerrainHM->tid);
			singleton->sampleFBO("hmFBO",3);

			singleton->setShaderFloat("bufferedPageSizeInUnits", bufferedPageSizeInUnits);
			singleton->setShaderFloat("threshVal", (float)threshVal);

			singleton->setShaderFloat("bufferMult", (float)(singleton->bufferMult));
			singleton->setShaderfVec3("worldMinVisInPixels", &(worldMinVisInPixels));
			singleton->setShaderfVec3("worldMaxVisInPixels", &(worldMaxVisInPixels));
			singleton->setShaderfVec3("worldMinBufInPixels", &(worldMinBufInPixels));
			singleton->setShaderfVec3("worldMaxBufInPixels", &(worldMaxBufInPixels));

			singleton->setShaderFloat("paramsPerEntry", (float)(paramsPerEntry/3) );
			singleton->setShaderFloat("numEntries", (float)numEntries);
			singleton->setShaderArrayfVec3("paramArr", singleton->paramArr, totParams/3);

			singleton->drawFSQuad(1.0f);

			singleton->unsampleFBO("hmFBO",3);
			//singleton->setShaderTexture(3, 0);
			singleton->setShaderTexture(2, 0);
			singleton->setShaderTexture3D(1, 0);
			singleton->setShaderTexture3D(0, 0);
			
			
			

			singleton->unbindFBO();
			singleton->unbindShader();

			//ray trace new texture, generate normals, AO, depth, etc
			singleton->bindShader("RenderVolume");
			singleton->bindFBODirect(gpuRes->fboSet);
			singleton->sampleFBO("volGenFBO");
			glClearColor(0.0f,0.0f,0.0f,0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			singleton->setShaderFloat("bufferMult", (float)(singleton->bufferMult));
			singleton->setShaderfVec3("worldMinVisInPixels", &(worldMinVisInPixels));
			singleton->setShaderfVec3("worldMaxVisInPixels", &(worldMaxVisInPixels));
			singleton->setShaderfVec3("worldMinBufInPixels", &(worldMinBufInPixels));
			singleton->setShaderfVec3("worldMaxBufInPixels", &(worldMaxBufInPixels));


			
			

			glCallList(singleton->volTris);
			singleton->unsampleFBO("volGenFBO");
			singleton->unbindFBO();
			singleton->unbindShader();
		}

		curState = E_STATE_GENERATEVOLUME_END;
	}
GamePage::~ GamePage ()
                    {

		if (volData) {
			delete[] volData;
		}
		if (volDataLinear) {
			delete[] volDataLinear;
		}
	}
void GamePage::run ()
                   {
		switch (nextState) {
			case E_STATE_CREATESIMPLEXNOISE_LAUNCH:
				createSimplexNoise();
			break;

			default:

			break;
		}
	}
#undef LZZ_INLINE
 
