// f00350_gamepage.h
//

#include "f00350_gamepage.e"
#define LZZ_INLINE inline
uint * GamePage::getVolData ()
                           {
		int i;

		if (volData == NULL) {
			volData = new uint[iVolumeSize];
			for (i = 0; i < iVolumeSize; i++) {
				volData[i] = 0;
			}
		}

		return volData;
		
	}
uint * GamePage::getVolDataLinear ()
                                 {
		int i;

		if (volDataLinear == NULL) {
			volDataLinear = new uint[iVolumeSize];
			for (i = 0; i < iVolumeSize; i++) {
				volDataLinear[i] = (0<<24)|(0<<16)|(0<<8)|(0);
			}
		}

		return volDataLinear;
	}
GamePage::GamePage ()
                   {
		volData = NULL;
		volDataLinear = NULL;
	}
void GamePage::init (Singleton * _singleton, GamePageHolder * _parentGPH, int _thisPageId, int offsetX, int offsetY, int offsetZ, int oxLoc, int oyLoc, int ozLoc)
          {

		pushTrace("GamePage init()");

		thisPageId = _thisPageId;
		singleton = _singleton;
		parentGPH = _parentGPH;
		//usingPoolId = -1;

		maxEntries = 32;
		isRendering = true;

		int i;

		//fbow = singleton->getFBOWrapper("volGenFBO",0);

		volDataModified = false;
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
void GamePage::copyToTexture (bool isForEmptyVD)
                                              {


		int id1 = singleton->volID;
		int id2 = singleton->volIDLinear;

		if (isForEmptyVD) {
			id1 = singleton->volIDEmpty;
			id2 = singleton->volIDEmptyLinear;
		}

		glBindTexture(GL_TEXTURE_3D,id1);
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

				getVolData()
			);

		glBindTexture(GL_TEXTURE_3D,0);
		glBindTexture(GL_TEXTURE_3D,id2);
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

				getVolDataLinear()
			);
		glBindTexture(GL_TEXTURE_3D,0);

		



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


	}
void GamePage::addGeom (bool justTesting)
                                       {

		int i;
		int j;
		int k;
		int m;
		int n;
		int p;
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

		//paramsPerEntry = GameGeom::paramsPerEntry;
		paramsPerEntry = E_GP_LENGTH*3;
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
			hasTree = false;
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
									if (gg->buildingType == E_BT_TREE) {
										hasTree = true;
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

										

										for (p = 0; p < E_GP_LENGTH; p++) {

											baseInd = numEntries*paramsPerEntry + p*3;

											singleton->paramArr[baseInd + 0] = gg->geomParams[p].getFX();
											singleton->paramArr[baseInd + 1] = gg->geomParams[p].getFY();
											singleton->paramArr[baseInd + 2] = gg->geomParams[p].getFZ();
										}
										

										// singleton->paramArr[baseInd + 0] = gg->getBoundsMinInPixelsT()->getFX();
										// singleton->paramArr[baseInd + 1] = gg->getBoundsMinInPixelsT()->getFY();
										// singleton->paramArr[baseInd + 2] = gg->getBoundsMinInPixelsT()->getFZ();

										// singleton->paramArr[baseInd + 3] = gg->getBoundsMaxInPixelsT()->getFX();
										// singleton->paramArr[baseInd + 4] = gg->getBoundsMaxInPixelsT()->getFY();
										// singleton->paramArr[baseInd + 5] = gg->getBoundsMaxInPixelsT()->getFZ();

										// singleton->paramArr[baseInd + 6] = gg->getVisMinInPixelsT()->getFX();
										// singleton->paramArr[baseInd + 7] = gg->getVisMinInPixelsT()->getFY();
										// singleton->paramArr[baseInd + 8] = gg->getVisMinInPixelsT()->getFZ();

										// singleton->paramArr[baseInd + 9] = gg->getVisMaxInPixelsT()->getFX();
										// singleton->paramArr[baseInd + 10] = gg->getVisMaxInPixelsT()->getFY();
										// singleton->paramArr[baseInd + 11] = gg->getVisMaxInPixelsT()->getFZ();

										// singleton->paramArr[baseInd + 12] = gg->cornerDisInPixels.getFX();
										// singleton->paramArr[baseInd + 13] = gg->cornerDisInPixels.getFY();
										// singleton->paramArr[baseInd + 14] = gg->cornerDisInPixels.getFZ();

										// singleton->paramArr[baseInd + 15] = gg->powerVals.getFX();
										// singleton->paramArr[baseInd + 16] = gg->powerVals.getFY();
										// singleton->paramArr[baseInd + 17] = gg->powerVals.getFZ();

										// singleton->paramArr[baseInd + 15] = gg->powerVals2.getFX();
										// singleton->paramArr[baseInd + 16] = gg->powerVals2.getFY();
										// singleton->paramArr[baseInd + 17] = gg->powerVals2.getFZ();

										// singleton->paramArr[baseInd + 18] = gg->thickVals.getFX();
										// singleton->paramArr[baseInd + 19] = gg->thickVals.getFY();
										// singleton->paramArr[baseInd + 20] = gg->thickVals.getFZ();

										// singleton->paramArr[baseInd + 21] = gg->matParams.getFX();
										// singleton->paramArr[baseInd + 22] = gg->matParams.getFY();
										// singleton->paramArr[baseInd + 23] = gg->matParams.getFZ();

										singleton->matCountArr[gg->geomParams[E_GP_MATPARAMS].getIX()] += 1.0f;

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
void GamePage::generateVolume ()
                              {


		int i;
		isRendering = true;
		
		curState = E_STATE_GENERATEVOLUME_BEG;
		


		if (volDataModified) {
			copyToTexture(false);
		}
		else {
			if (singleton->emptyVDNotReady) {
				singleton->emptyVDNotReady = false;
				copyToTexture(true);
			}
		}
		
		
		parentGPH->clearSet(false);

		addGeom(true);
		addGeom(false);


		PAGE_COUNT++;

		singleton->bindShader("GenerateVolume");
		
		

		singleton->bindFBO(singleton->curVGString);

		if (volDataModified) {
			singleton->setShaderTexture3D(0,singleton->volID);
			singleton->setShaderTexture3D(1,singleton->volIDLinear);
		}
		else {
			singleton->setShaderTexture3D(0,singleton->volIDEmpty);
			singleton->setShaderTexture3D(1,singleton->volIDEmptyLinear);
		}

		
		singleton->sampleFBO("hmFBOLinear",2);
		singleton->setShaderTexture(3,singleton->terrainID);
		//singleton->setShaderTexture(3,singleton->uvPattern->tid);
		//singleton->setShaderTexture3D(3,singleton->voroID);

		singleton->setShaderInt("hasTree", (int)hasTree);
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
			singleton->setShaderInt("paramsPerEntry", (paramsPerEntry/3) );
			singleton->setShaderInt("numEntries", numEntries);
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
			
			//glEnable(GL_DEPTH_TEST);

			
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
			




			

			//glDisable(GL_DEPTH_TEST);
		}

		
		


		isRendering = false;

		curState = E_STATE_GENERATEVOLUME_END;
	}
void GamePage::getCoords ()
                         {



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
GamePage::~ GamePage ()
                    {

		if (volData != NULL) {
			delete[] volData;
		}
		if (volDataLinear != NULL) {
			delete[] volDataLinear;
		}
	}
void GamePage::run ()
                   {
		switch (nextState) {
			case E_STATE_CREATESIMPLEXNOISE_LAUNCH:
				//createSimplexNoise();
			break;

			default:

			break;
		}
	}
#undef LZZ_INLINE
 
