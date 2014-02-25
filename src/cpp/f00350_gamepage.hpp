

class GamePage: public Poco::Runnable {
public:

	Singleton* singleton;

	int thisPageId;
	//int usingPoolId;
	//PooledResource* gpuRes;

	int bufferedPageSizeInUnits;
	FIVector4 offsetInUnits;
	FIVector4 offsetInPages;
	FIVector4 offsetInPagesLocal;
	float pageDepth;
	float seaHeightInPixels;
	int iVolumeSize;
	
	bool hasGeom;
	bool hasTerrain;
	bool hasWater;
	bool hasWindow;

	//FBOWrapper* fbow;

	uint* volData;
	uint* volDataLinear;
	

	bool isRendering;

	int paramsPerEntry;
	int numEntries;
	int totParams;
	int maxEntries;
	

	int maxHeightInUnits;

	FIVector4 worldSeed;

	//bool isDirty;

	bool threadRunning;

	float unitSizeInPixels;

	E_STATES curState;
	E_STATES nextState;

	FIVector4 worldMinVisInPixels;
	FIVector4 worldMaxVisInPixels;
	FIVector4 worldMinBufInPixels;
	FIVector4 worldMaxBufInPixels;

	FIVector4 scaleAndOffset;


	FIVector4 worldUnitMin;
	FIVector4 worldUnitMax;


	E_FILL_STATE fillState;

	GamePageHolder* parentGPH;

	GamePage() {

	}



	void init(
		Singleton* _singleton,
		GamePageHolder* _parentGPH,
		int _thisPageId,
		int offsetX,
		int offsetY,
		int offsetZ,
		int oxLoc,
		int oyLoc,
		int ozLoc
	) {

		pushTrace("GamePage init()");

		thisPageId = _thisPageId;
		singleton = _singleton;
		parentGPH = _parentGPH;
		//usingPoolId = -1;

		maxEntries = 32;
		isRendering = true;

		int i;

		//fbow = singleton->getFBOWrapper("volGenFBO",0);

		
		threadRunning = false;


		maxHeightInUnits = (singleton->maxHeightInUnits);

		//isDirty = false;

		fillState = E_FILL_STATE_PARTIAL;
		curState = E_STATE_INIT_BEG;
		nextState = E_STATE_WAIT;

		
		int visPageSizeInUnits = singleton->visPageSizeInUnits;
		bufferedPageSizeInUnits = (visPageSizeInUnits) * (singleton->bufferMult);

		offsetInPages.setIXYZ(offsetX,offsetY,offsetZ);
		offsetInUnits.copyFrom(&offsetInPages);
		offsetInUnits.multXYZ(singleton->visPageSizeInUnits);

		offsetInPagesLocal.setIXYZ(oxLoc,oyLoc,ozLoc);

		float hzp = (float)(singleton->holderSizeInPages);

		pageDepth = ((1.0f - ( (offsetInPagesLocal.getFZ()*hzp*hzp + offsetInPagesLocal.getFY()*hzp + offsetInPagesLocal.getFX())/(hzp*hzp*hzp) ))*0.9f + 0.05f)*0.5f;


		unitSizeInPixels = (float)(singleton->unitSizeInPixels);


		worldSeed.copyFrom(&(singleton->worldSeed));



		iVolumeSize = bufferedPageSizeInUnits*bufferedPageSizeInUnits*bufferedPageSizeInUnits;
		volData = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volData[i] = 0;
		}

		volDataLinear = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volDataLinear[i] = (0<<24)|(0<<16)|(0<<8)|(0);
		}



		worldMinVisInPixels.copyFrom(&offsetInUnits);
		worldMaxVisInPixels.copyFrom(&offsetInUnits);
		worldMaxVisInPixels.addXYZ(visPageSizeInUnits);
		worldMinVisInPixels.multXYZ((float)unitSizeInPixels);
		worldMaxVisInPixels.multXYZ((float)unitSizeInPixels);

		worldUnitMin.copyFrom(&offsetInUnits);
		worldUnitMax.copyFrom(&offsetInUnits);
		worldUnitMax.addXYZ(visPageSizeInUnits);

		worldUnitMin.addXYZ( -(bufferedPageSizeInUnits-visPageSizeInUnits)/2 );
		worldUnitMax.addXYZ( (bufferedPageSizeInUnits-visPageSizeInUnits)/2 );

		worldMinBufInPixels.copyFrom(&worldUnitMin);
		worldMaxBufInPixels.copyFrom(&worldUnitMax);
		worldMinBufInPixels.multXYZ((float)unitSizeInPixels);
		worldMaxBufInPixels.multXYZ((float)unitSizeInPixels);



		float minHeight = 99999.0f;
		float maxHeight = 0.0f;
		float avgHeight = 0.0f;
		float centerHeight = (worldMinVisInPixels.getFZ() + worldMaxVisInPixels.getFZ())/2.0f;
		GameWorld* gw = singleton->gw;
		seaHeightInPixels = ( ( ((float)gw->seaLevel)*singleton->heightmapMax )/255.0 );
		

		float testHeight[4];

		testHeight[0] = singleton->getHeightAtPixelPos(worldMinVisInPixels.getIX(),worldMinVisInPixels.getIY());
		testHeight[1] = singleton->getHeightAtPixelPos(worldMaxVisInPixels.getIX(),worldMinVisInPixels.getIY());
		testHeight[2] = singleton->getHeightAtPixelPos(worldMinVisInPixels.getIX(),worldMaxVisInPixels.getIY());
		testHeight[3] = singleton->getHeightAtPixelPos(worldMaxVisInPixels.getIX(),worldMaxVisInPixels.getIY());


		for (i = 0; i < 4; i++) {
			avgHeight += testHeight[i];
			
			if (testHeight[i] < minHeight) {
				minHeight = testHeight[i];
			}
			if (testHeight[i] > maxHeight) {
				maxHeight = testHeight[i];
			}
			

		}

		avgHeight /= 4.0;

		maxHeight += singleton->visPageSizeInPixels*2;
		minHeight -= singleton->visPageSizeInPixels*2;



		
		hasTerrain = ( avgHeight + singleton->visPageSizeInPixels*4.0f >= worldMinVisInPixels.getFZ() );//(abs(centerHeight-avgHeight) <= singleton->visPageSizeInPixels*4.0);
		hasWater =  ( seaHeightInPixels >= worldMinVisInPixels.getFZ() );

		addGeom(true);

		


		if (hasGeom || hasTerrain || hasWater) {
			fillState = E_FILL_STATE_PARTIAL;
			curState = E_STATE_INIT_END;
		}
		else {
			fillState = E_FILL_STATE_EMPTY;
			curState = E_STATE_LENGTH;
		}



		popTrace();

	}

	void copyToTexture() {


		/*
		glTexSubImage2D(
			GLenum  	target,
		 	GLint  	level,
		 	GLint  	xoffset,
		 	GLint  	yoffset,
		 	GLsizei  	width,
		 	GLsizei  	height,
		 	GLenum  	format,
		 	GLenum  	type,
		 	const GLvoid *  	data
		 );


		void glTexSubImage3D(

			GLenum target,
		 	GLint level,
		 	GLint xoffset,
		 	GLint yoffset,
		 	GLint zoffset,
		 	GLsizei width,
		 	GLsizei height,
		 	GLsizei depth,
		 	GLenum format,
		 	GLenum type,
		 

		glBindTexture(GL_TEXTURE_2D, singleton->volID);
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0,
				
				xoff,
				yoff,

				width,
				height,

				GL_RGBA,
				GL_UNSIGNED_SHORT,

				cpuArrPtr
			);
		glBindTexture(GL_TEXTURE_2D, 0);


		*/
		

		glBindTexture(GL_TEXTURE_3D,singleton->volID);
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
		glBindTexture(GL_TEXTURE_3D,singleton->volIDLinear);
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

		

	}


	void addGeom(bool justTesting) {

		int i;
		int j;
		int k;
		int m;
		int n;
		int ind;
		int bufSize = (singleton->visPageSizeInPixels*singleton->bufferMult);
		intPair curId;
		int geomInPage;
		int baseInd;

		GameWorld* gw = singleton->gw;

		FIVector4 start;
		FIVector4 end;

		GamePageHolder* gph;
		GameGeom* gg;

		paramsPerEntry = GameGeom::paramsPerEntry;
		numEntries = 0;

		bool doProc;

		start.copyFrom( &worldMinBufInPixels );
		end.copyFrom( &worldMaxBufInPixels );

		start.addXYZ(-bufSize);
		end.addXYZ(bufSize);

		start.intDivXYZ(singleton->holderSizeInPixels);
		end.intDivXYZ(singleton->holderSizeInPixels);
		
		start.clampZ( singleton->origin.getFZ(), singleton->worldSizeInHolders.getFZ()-1.0f );
		end.clampZ( singleton->origin.getFZ(), singleton->worldSizeInHolders.getFZ()-1.0f );

		for (i = 0; i < E_MAT_PARAM_LENGTH; i++) {
			singleton->matCountArr[i] = 0.0f;
		}

		if (justTesting) {
			hasWindow = false;
			hasGeom = false;
		}
		

		for (k = start.getIZ(); k <= end.getIZ(); k++ ) {
			for (j = start.getIY(); j <= end.getIY(); j++ ) {
				for (i = start.getIX(); i <= end.getIX(); i++ ) {
					gph = gw->getHolderAtCoords(i,j,k);

					// TODO critical: make sure holders are ready before pages 


					if (gph) {
						geomInPage = gph->containsGeomIds.size();

						for (m = 0; m < geomInPage; m++) {
							curId = gph->containsGeomIds[m];
							gg = gw->blockData[curId.v0]->gameGeom[curId.v1];


							if (

								FIVector4::intersect( gg->getVisMinInPixelsT(), gg->getVisMaxInPixelsT(), &worldMinBufInPixels, &worldMaxBufInPixels ) 
								&& (gg->visible)
								
							) {
								
								if (justTesting) {
									if (gg->buildingType == E_BT_WINDOW) {
										hasWindow = true;
									}
									hasGeom = true;
								}
								else {

									doProc = true;

									for (n = 0; n < numEntries; n++) {
										if (singleton->geomIDArr[n] == gg->globalID) {
											doProc = false;
										}
									}

									if (doProc) {

										singleton->geomIDArr[numEntries] = gg->globalID;

										baseInd = numEntries*paramsPerEntry;

										

										singleton->paramArr[baseInd + 0] = gg->getBoundsMinInPixelsT()->getFX();
										singleton->paramArr[baseInd + 1] = gg->getBoundsMinInPixelsT()->getFY();
										singleton->paramArr[baseInd + 2] = gg->getBoundsMinInPixelsT()->getFZ();

										singleton->paramArr[baseInd + 3] = gg->getBoundsMaxInPixelsT()->getFX();
										singleton->paramArr[baseInd + 4] = gg->getBoundsMaxInPixelsT()->getFY();
										singleton->paramArr[baseInd + 5] = gg->getBoundsMaxInPixelsT()->getFZ();

										singleton->paramArr[baseInd + 6] = gg->getVisMinInPixelsT()->getFX();
										singleton->paramArr[baseInd + 7] = gg->getVisMinInPixelsT()->getFY();
										singleton->paramArr[baseInd + 8] = gg->getVisMinInPixelsT()->getFZ();

										singleton->paramArr[baseInd + 9] = gg->getVisMaxInPixelsT()->getFX();
										singleton->paramArr[baseInd + 10] = gg->getVisMaxInPixelsT()->getFY();
										singleton->paramArr[baseInd + 11] = gg->getVisMaxInPixelsT()->getFZ();

										singleton->paramArr[baseInd + 12] = gg->cornerDisInPixels.getFX();
										singleton->paramArr[baseInd + 13] = gg->cornerDisInPixels.getFY();
										singleton->paramArr[baseInd + 14] = gg->cornerDisInPixels.getFZ();

										singleton->paramArr[baseInd + 15] = gg->powerVals.getFX();
										singleton->paramArr[baseInd + 16] = gg->powerVals.getFY();
										singleton->paramArr[baseInd + 17] = gg->powerVals.getFZ();

										singleton->paramArr[baseInd + 18] = gg->thickVals.getFX();
										singleton->paramArr[baseInd + 19] = gg->thickVals.getFY();
										singleton->paramArr[baseInd + 20] = gg->thickVals.getFZ();

										singleton->paramArr[baseInd + 21] = gg->matParams.getFX();
										singleton->paramArr[baseInd + 22] = gg->matParams.getFY();
										singleton->paramArr[baseInd + 23] = gg->matParams.getFZ();

										singleton->matCountArr[gg->matParams.getIX()] += 1.0f;

										numEntries++;
									}
								}
							}
						}
					}
				}
			}
		}

		if (justTesting) {
			if (hasWindow || hasWater) {
				parentGPH->hasTrans = true;
			}

			if (hasTerrain || hasGeom) {
				parentGPH->hasSolids = true;
			}
		}
		else {
			if (numEntries > maxEntries) {
				numEntries = maxEntries;
				doTrace("limit exceeded");
			}

			totParams = numEntries*paramsPerEntry;
		}

		


	}


	void generateVolume() {


		int i;
		isRendering = true;
		
		curState = E_STATE_GENERATEVOLUME_BEG;
		
		copyToTexture();
		
		parentGPH->clearSet(false);

		addGeom(true);
		addGeom(false);


		PAGE_COUNT++;

		singleton->bindShader("GenerateVolume");
		
		

		singleton->bindFBO(singleton->curVGString);
		singleton->setShaderTexture3D(0,singleton->volID);
		singleton->setShaderTexture3D(1,singleton->volIDLinear);
		singleton->sampleFBO("hmFBOLinear",2);
		singleton->setShaderTexture(3,singleton->terrainID);
		//singleton->setShaderTexture(3,singleton->uvPattern->tid);
		//singleton->setShaderTexture3D(3,singleton->voroID);

		singleton->setShaderInt("hasGeom", (int)hasGeom);
		singleton->setShaderInt("hasTerrain", (int)hasTerrain);

		singleton->setShaderfVec4("mapFreqs", &(singleton->mapFreqs) );
		singleton->setShaderfVec4("mapAmps", &(singleton->mapAmps) );

		singleton->setShaderFloat("totLayers", MAX_LAYERS);
		singleton->setShaderFloat("pixelsPerMeter", singleton->pixelsPerMeter);
		//singleton->setShaderFloat("directPass", singleton->directPass);
		singleton->setShaderFloat("seaLevel", seaHeightInPixels );
		singleton->setShaderFloat("heightmapMax",singleton->heightmapMax);
		//singleton->setShaderFloat("slicesPerPitch", (float)( singleton->slicesPerPitch ));
		
		singleton->setShaderFloat("volumePitch", (float)( singleton->volGenFBOX ));

		singleton->setShaderFloat("maxFloors", singleton->maxFloors);
		singleton->setShaderFloat("bufferedPageSizeInUnits", bufferedPageSizeInUnits);
		singleton->setShaderFloat("bufferMult", (float)(singleton->bufferMult));
		singleton->setShaderfVec3("worldSizeInPixels", &(singleton->maxBoundsInPixels));
		singleton->setShaderfVec3("worldMinVisInPixels", &(worldMinVisInPixels));
		singleton->setShaderfVec3("worldMaxVisInPixels", &(worldMaxVisInPixels));
		singleton->setShaderfVec3("worldMinBufInPixels", &(worldMinBufInPixels));
		singleton->setShaderfVec3("worldMaxBufInPixels", &(worldMaxBufInPixels));

		

		if (hasGeom) {
			singleton->setShaderFloat("paramsPerEntry", (float)(paramsPerEntry/3) );
			singleton->setShaderFloat("numEntries", (float)numEntries);
			singleton->setShaderArrayfVec3("paramArr", singleton->paramArr, totParams/3);
			singleton->setShaderArray("matCountArr", singleton->matCountArr, E_MAT_PARAM_LENGTH);
		}
		

		singleton->drawFSQuad(1.0f);

		singleton->setShaderTexture(3,0);
		singleton->unsampleFBO("hmFBOLinear",2);
		singleton->setShaderTexture3D(1, 0);
		singleton->setShaderTexture3D(0, 0);
		singleton->unbindFBO();
		singleton->unbindShader();

		

		

		






		// if (singleton->useVolumeTex) {
		// 	fbow->getPixelsFast();
		// 	glBindTexture(GL_TEXTURE_3D,singleton->volGenID);
		// 		glTexSubImage3D(
		// 			GL_TEXTURE_3D,
		// 			0,
					
		// 			0,
		// 			0,
		// 			0,

		// 			singleton->volGenFBOX,
		// 			singleton->volGenFBOX,
		// 			singleton->volGenFBOX,

		// 			GL_RGBA,
		// 			GL_UNSIGNED_BYTE,

		// 			fbow->pixelsUINT
		// 		);

		// 	glBindTexture(GL_TEXTURE_3D,0);
		// }
		


		

		if (parentGPH->gpuRes != NULL) {


			getCoords();

			//ray trace new texture, generate normals, AO, depth, etc
			
			glEnable(GL_DEPTH_TEST);

			
			singleton->bindShader("RenderVolume");






			for (i = 0; i < MAX_LAYERS; i++) {

				if (
					((i == 0)&&(hasTerrain||hasGeom)) ||
					((i == 1)&&(hasWater||hasWindow))
				) {
					singleton->bindFBODirect(parentGPH->gpuRes->getFBOS(i), 0);
					
					// if (singleton->useVolumeTex) {
					// 	singleton->setShaderTexture3D(0,singleton->volGenID);
					// }
					// else {
						singleton->sampleFBO(singleton->curVGString);
					//}


					//
					//glClearColor(0.0f,0.0f,0.0f,0.0f);
					//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					singleton->setShaderFloat("curLayer", i);
					//singleton->setShaderFloat("directPass", singleton->directPass);
					singleton->setShaderFloat("pageDepth", (float)( pageDepth ));
					singleton->setShaderFloat("volumePitch", (float)( singleton->volGenFBOX ));

					//singleton->setShaderfVec3("offsetInPagesLocal", &(offsetInPagesLocal));
					singleton->setShaderFloat("bufferMult", (float)(singleton->bufferMult));
					singleton->setShaderFloat("visPageSizeInPixels", (float)(singleton->visPageSizeInPixels));
					singleton->setShaderfVec3("worldMinVisInPixels", &(worldMinVisInPixels));
					singleton->setShaderfVec3("worldMaxVisInPixels", &(worldMaxVisInPixels));
					singleton->setShaderfVec3("worldMinBufInPixels", &(worldMinBufInPixels));
					singleton->setShaderfVec3("worldMaxBufInPixels", &(worldMaxBufInPixels));
					singleton->setShaderfVec4("scaleAndOffset", &scaleAndOffset);

					glCallList(singleton->volTris);
					

					// if (singleton->useVolumeTex) {
					// 	singleton->setShaderTexture3D(0,0);
					// }
					// else {
						singleton->unsampleFBO(singleton->curVGString);
					//}

					singleton->unbindFBO();
				}
				
			}

			singleton->unbindShader();
			




			

			glDisable(GL_DEPTH_TEST);
		}

		
		


		isRendering = false;

		curState = E_STATE_GENERATEVOLUME_END;
	}


	
	void getCoords() {



		float dx = offsetInPagesLocal.getFX();
		float dy = offsetInPagesLocal.getFY();
		float dz = offsetInPagesLocal.getFZ();

		float pitchSrc = (float)((singleton->visPageSizeInPixels*2.0f));
		float pitchSrc2 = (pitchSrc)/2.0f;

		float dxmod = dx*pitchSrc2;
		float dymod = dy*pitchSrc2;
		float dzmod = dz*pitchSrc2;


		float fx1 = (dxmod-dymod) - pitchSrc2;
		float fy1 = (-(dxmod/2.0f) + -(dymod/2.0f) + dzmod) - pitchSrc2;
		float fx2 = fx1 + pitchSrc;
		float fy2 = fy1 + pitchSrc + 2.0; // TODO: THIS "+ 2.0" is a hack used to cover cracks between pages, it should not be used

		float sx = singleton->holderSizeInPixels;
		float sy = singleton->holderSizeInPixels;


		fx1 = fx1/sx;
		fy1 = fy1/sy;
		fx2 = fx2/sx;
		fy2 = fy2/sy;

		scaleAndOffset.setFXYZW(
			(fx2 - fx1)/2.0f,
			(fy2 - fy1)/2.0f,
			(fx1 + fx2)/2.0f,
			(fy1 + fy2)/2.0f
			
		);


	}
	


	~GamePage() {

		if (volData) {
			delete[] volData;
		}
		if (volDataLinear) {
			delete[] volDataLinear;
		}
	}




	void run() {
		switch (nextState) {
			case E_STATE_CREATESIMPLEXNOISE_LAUNCH:
				//createSimplexNoise();
			break;

			default:

			break;
		}
	}





	// uint NumberOfSetBits(uint i)
	// {
	//     i = i - ((i >> 1) & 0x55555555);
	//     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	//     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
	// }

	// uint clamp(uint val) {
	// 	if (val > 255) {
	// 		val = 255;
	// 	}
	// 	if (val < 0) {
	// 		val = 0;
	// 	}
	// 	return val;
	// }
	




	// void createSimplexNoise() {

	// 	threadRunning = true;

	// 	// REMINDER: DO NOT ACCESS EXTERNAL POINTERS INSIDE THREAD

	// 	bool isBlank = false;
	// 	bool isFull = false;
		

	// 	curState = E_STATE_CREATESIMPLEXNOISE_BEG;

	// 	int i, j, k, m;

	// 	int totLen = bufferedPageSizeInUnits;

	// 	float fTotLen = (float)totLen;

	// 	int ind = 0;

	// 	uint tmp;

	// 	float fx, fy, fz;

	// 	uint randOff[3];
	// 	float ijkVals[3];

	// 	const float RAND_MOD[9] = {
	// 		3456.0f, 5965.0f, 45684.0f,
	// 		4564.0f, 1234.0f, 6789.0f,
	// 		4567.0f, 67893.0f, 13245.0f
	// 	};

	// 	float totLenO4 = totLen/4;
	// 	float totLen3O4 = (totLen*3)/4;
	// 	float fSimp;
	// 	float heightThresh;
	// 	float testVal;

	// 	bool mustNotBeFull = false;



	// 	for (j = 0; j < totLen; j++) {

	// 		ijkVals[1] = (float)j;

	// 		fy = (j) + offsetInUnits.getFY();

	// 		for (i = 0; i < totLen; i++) {

	// 			ijkVals[0] = (float)i;

	// 			fx = (i) + offsetInUnits.getFX();
				
	// 			for (k = 0; k < totLen; k++) {

	// 				ijkVals[2] = (float)k;

	// 				fz = (k) + offsetInUnits.getFZ();

	// 				ind = k*totLen*totLen + j*totLen + i;

					
					
	// 				if (fx < 0.0f || fy < 0.0f || fz < 0.0f ) {
	// 					volData[ind] = 0;
	// 					volDataLinear[ind] = (255<<16)|(255<<8)|255;
	// 				}
	// 				else {

	// 					heightThresh = (fz/ ((float)maxHeightInUnits) );
	// 					if (heightThresh > 1.0f) {
	// 						heightThresh = 1.0f;
	// 					}
	// 					if (heightThresh < 0.0f) {
	// 						heightThresh = 0.0f;
	// 					}



	// 					if (k + offsetInUnits.getIZ() >= maxHeightInUnits) {
	// 						tmp = 0;
	// 						mustNotBeFull = true;
	// 					}
	// 					else {
	// 						testVal = simplexScaledNoise(
	// 											4.0f, //octaves
	// 											0.5f, //persistence (amount added in each successive generation)
	// 											1.0f/32.0f, //scale (frequency)
	// 											0.0f,
	// 											1.0f,
	// 											fx+worldSeed.getFX(),
	// 											fy+worldSeed.getFY(),
	// 											fz+worldSeed.getFZ()
	// 										);
							

							
	// 						tmp = clamp(testVal*255.0f*(1.0f-heightThresh*heightThresh*heightThresh));
	// 					}

						

	// 					if ( i >= totLenO4 && i <= totLen3O4 ) {
	// 						if ( j >= totLenO4 && j <= totLen3O4 ) {
	// 							if ( k >= totLenO4 && k <= totLen3O4 ) {
	// 								if (tmp > threshVal) {
	// 									isBlank = false;
	// 								}
	// 								else {
	// 									isFull = false;
	// 								}
	// 							}
	// 						}
	// 					}

						
						


	// 					for (m = 0; m < 3; m++) {
	// 						fSimp = simplexScaledNoise(
	// 																	1.0f, //octaves
	// 																	1.0f, //persistence (amount added in each successive generation)
	// 																	1.0f/4.0, //scale (frequency)
	// 																	0.0f,
	// 																	1.0f,
	// 																	fx+RAND_MOD[m*3+0],
	// 																	fy+RAND_MOD[m*3+1],
	// 																	fz+RAND_MOD[m*3+2]
	// 																);
	// 						randOff[m] = clamp( ( fSimp + ijkVals[m])*255.0f/fTotLen);
							


	// 					}

	// 					if ( (tmp%16 > 5) && ( (i+j+k)%2 == 0) ) {

	// 						/*if (randOff[0] == 0 && randOff[1] == 0 && randOff[2] == 0) {
	// 							randOff[1] = 1;
	// 						} */

	// 						volData[ind] = (0)|(randOff[2]<<16)|(randOff[1]<<8)|randOff[0];
	// 						volDataLinear[ind] = (tmp<<24)|(255<<16)|(255<<8)|255;
	// 					}
	// 					else {
	// 						volData[ind] = (0);
	// 						volDataLinear[ind] = (tmp<<24)|(255<<16)|(255<<8)|255;;
	// 					}

						
	// 				}
					
					

					
	// 			}
	// 		}
	// 	}





	// 	/*
	// 	if (mustNotBeFull) {
	// 		isFull = false;
	// 	}

	// 	if (isBlank || isFull ) {

	// 		if (isBlank) {
	// 			fillState = E_FILL_STATE_EMPTY;
	// 		}
	// 		if (isFull) {
	// 			fillState = E_FILL_STATE_FULL;
	// 		}

	// 		curState = E_STATE_LENGTH;
	// 	}
	// 	else {*/

	// 		//fillState = E_FILL_STATE_PARTIAL;
	// 		//curState = E_STATE_CREATESIMPLEXNOISE_END;
	// 	//}


		

	// 	fillState = E_FILL_STATE_PARTIAL;
	// 	curState = E_STATE_CREATESIMPLEXNOISE_END;


	// 	threadRunning = false;

	// }

};