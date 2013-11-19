// f00350_gamepage.h
//

#include "f00350_gamepage.e"
#define LZZ_INLINE inline
GamePage::GamePage ()
                   {

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

		
		threshVal = 140;
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

		FBOWrapper* fbow = singleton->getFBOWrapper("hmFBO",0);

		int fbowWidth = fbow->width;

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



		bool hasGeom = addGeom(true);



		

		if ( abs(centerHeight-avgHeight) > singleton->visPageSizeInPixels*2.0 ) { //(worldMaxVisInPixels.getFZ() < minHeight) ||  (worldMinVisInPixels.getFZ() > maxHeight)  ) {
			if (hasGeom && (centerHeight > avgHeight)) {
				fillState = E_FILL_STATE_PARTIAL;
				curState = E_STATE_INIT_END;
				
			}
			else {
				fillState = E_FILL_STATE_EMPTY;
				curState = E_STATE_LENGTH;

				
			}

		}
		else {
			fillState = E_FILL_STATE_PARTIAL;
			curState = E_STATE_INIT_END;

			
		}

		//fillState = E_FILL_STATE_PARTIAL;
		//curState = E_STATE_INIT_END;



		popTrace();

		//curState = E_STATE_INIT_END;
	}
void GamePage::copyToTexture ()
                             {


		

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
bool GamePage::addGeom (bool justTesting)
                                       {

		int i;
		int j;
		int k;
		int m;
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

		paramsPerEntry = 21;
		numEntries = 0;

		start.copyFrom( &worldMinBufInPixels );
		end.copyFrom( &worldMaxBufInPixels );

		start.addXYZ(-bufSize);
		end.addXYZ(bufSize);

		start.intDivXYZ(singleton->holderSizeInPixels);
		end.intDivXYZ(singleton->holderSizeInPixels);
		
		start.clampZ( &(singleton->origin), &(singleton->worldSizeInHoldersM1) );
		end.clampZ( &(singleton->origin), &(singleton->worldSizeInHoldersM1) );


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
								true
								/*
								gg->boundsMinInPixels.inBoundsXYZ( &worldMinBufInPixels, &worldMaxBufInPixels ) ||
								gg->boundsMaxInPixels.inBoundsXYZ( &worldMinBufInPixels, &worldMaxBufInPixels ) ||
								worldMinBufInPixels.inBoundsXYZ( &(gg->boundsMinInPixels), &(gg->boundsMaxInPixels) ) ||
								worldMaxBufInPixels.inBoundsXYZ( &(gg->boundsMinInPixels), &(gg->boundsMaxInPixels) )
								*/
							) {
								
								if (justTesting) {
									return true;
								}

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

								singleton->paramArr[baseInd + 15] = gg->squareVals.getFX();
								singleton->paramArr[baseInd + 16] = gg->squareVals.getFY();
								singleton->paramArr[baseInd + 17] = gg->squareVals.getFZ();

								singleton->paramArr[baseInd + 18] = gg->minMaxMat.getFX();
								singleton->paramArr[baseInd + 19] = gg->minMaxMat.getFY();
								singleton->paramArr[baseInd + 20] = gg->minMaxMat.getFZ();

								numEntries++;

							}

						}
					}

					

				}
			}
		}

		if (numEntries > maxEntries) {
			numEntries = maxEntries;
			doTrace("limit exceeded");
		}

		totParams = numEntries*paramsPerEntry;

		return false;


	}
void GamePage::generateVolume ()
                              {


		int i;
		isRendering = true;
		
		curState = E_STATE_GENERATEVOLUME_BEG;
		
		copyToTexture();


		
		if (parentGPH->usingPoolId == -1) {


			parentGPH->usingPoolId = singleton->requestPoolId(parentGPH->thisHolderId);
			parentGPH->gpuRes = singleton->holderPoolItems[parentGPH->usingPoolId];
			
			// clear fbo by binding it with auto flag
			singleton->bindFBODirect(parentGPH->gpuRes->fboSet);
			singleton->unbindFBO();

		}
		




		addGeom(false);

		singleton->bindShader("GenerateVolumeBare");
		singleton->bindFBO("volGenFBO");
		singleton->setShaderTexture3D(0,singleton->volID);
		singleton->setShaderTexture3D(1,singleton->volIDLinear);
		singleton->sampleFBO("hmFBOLinear",2);
		//singleton->setShaderTexture3D(3,singleton->voroID);

		singleton->setShaderfVec4("mapFreqs", &(singleton->mapFreqs) );
		singleton->setShaderfVec4("mapAmps", &(singleton->mapAmps) );

		singleton->setShaderFloat("seaLevel", ((float)(singleton->gw->seaLevel) )/255.0 );
		singleton->setShaderFloat("slicesPerPitch", (float)( singleton->slicesPerPitch ));
		singleton->setShaderFloat("heightmapMax",singleton->heightmapMax);
		singleton->setShaderFloat("bufferedPageSizeInUnits", bufferedPageSizeInUnits);
		singleton->setShaderFloat("threshVal", (float)threshVal);
		singleton->setShaderFloat("bufferMult", (float)(singleton->bufferMult));
		singleton->setShaderFloat("paramsPerEntry", (float)(paramsPerEntry/3) );
		singleton->setShaderFloat("numEntries", (float)numEntries);
		singleton->setShaderfVec3("worldSizeInPixels", &(singleton->maxBoundsInPixels));
		singleton->setShaderfVec3("worldMinVisInPixels", &(worldMinVisInPixels));
		singleton->setShaderfVec3("worldMaxVisInPixels", &(worldMaxVisInPixels));
		singleton->setShaderfVec3("worldMinBufInPixels", &(worldMinBufInPixels));
		singleton->setShaderfVec3("worldMaxBufInPixels", &(worldMaxBufInPixels));
		singleton->setShaderArrayfVec3("paramArr", singleton->paramArr, totParams/3);

		singleton->drawFSQuad(1.0f);


		//singleton->setShaderTexture3D(3,0);
		singleton->unsampleFBO("hmFBOLinear",2);
		singleton->setShaderTexture3D(1, 0);
		singleton->setShaderTexture3D(0, 0);
		singleton->unbindFBO();
		singleton->unbindShader();

		bool renderSlice = false;


		if (parentGPH->gpuRes != NULL) {


			getCoords();

			//ray trace new texture, generate normals, AO, depth, etc
			
			glEnable(GL_DEPTH_TEST);

			if (renderSlice) {
				singleton->bindShader("RenderVolumeSlice");
			}
			else {
				singleton->bindShader("RenderVolume");
			}
			
			singleton->bindFBODirect(parentGPH->gpuRes->fboSet, 0);
			singleton->sampleFBO("volGenFBO");
			//glClearColor(0.0f,0.0f,0.0f,0.0f);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			singleton->setShaderFloat("pageDepth", (float)( pageDepth ));
			singleton->setShaderFloat("slicesPerPitch", (float)( singleton->slicesPerPitch ));

			//singleton->setShaderfVec3("offsetInPagesLocal", &(offsetInPagesLocal));

			singleton->setShaderFloat("slicesPerPitch", (float)( singleton->slicesPerPitch ));
			singleton->setShaderFloat("bufferMult", (float)(singleton->bufferMult));
			singleton->setShaderFloat("visPageSizeInPixels", (float)(singleton->visPageSizeInPixels));
			singleton->setShaderfVec3("worldMinVisInPixels", &(worldMinVisInPixels));
			singleton->setShaderfVec3("worldMaxVisInPixels", &(worldMaxVisInPixels));
			singleton->setShaderfVec3("worldMinBufInPixels", &(worldMinBufInPixels));
			singleton->setShaderfVec3("worldMaxBufInPixels", &(worldMaxBufInPixels));
			singleton->setShaderfVec4("scaleAndOffset", &scaleAndOffset);

			if (renderSlice) {
				glCallList(singleton->sliceTris);
			}
			else {
				glCallList(singleton->volTris);
			}
			//
			


			singleton->unsampleFBO("volGenFBO");
			singleton->unbindFBO();
			singleton->unbindShader();

			glDisable(GL_DEPTH_TEST);
		}

		
		


		isRendering = false;

		curState = E_STATE_GENERATEVOLUME_END;
	}
void GamePage::getCoords ()
                         {



		int dx = offsetInPagesLocal.getIX();
		int dy = offsetInPagesLocal.getIY();
		int dz = offsetInPagesLocal.getIZ();

		float pitchSrc = (float)((singleton->visPageSizeInPixels*2));
		float pitchSrc2 = (float)((singleton->visPageSizeInPixels*2)/2);

		float dxmod = dx*pitchSrc2;
		float dymod = dy*pitchSrc2;
		float dzmod = dz*pitchSrc2;


		float fx1 = (dxmod-dymod) - pitchSrc2;
		float fy1 = (-(dxmod/2.0f) + -(dymod/2.0f) + dzmod) - pitchSrc2;
		float fx2 = fx1 + pitchSrc;
		float fy2 = fy1 + pitchSrc;

		float sx = singleton->holderSizeInPixels;
		float sy = singleton->holderSizeInPixels;


		fx1 = fx1/sx;
		fy1 = fy1/sy;
		fx2 = fx2/sx;
		fy2 = fy2/sy;

		scaleAndOffset.setFXYZW(
			(fx2 - fx1)/2.0,
			(fy2 - fy1)/2.0,
			(fx1 + fx2)/2.0f,
			(fy1 + fy2)/2.0f
			
		);

		

		/*
	    singleton->sampleFBODirect(parentGPH->gpuRes->fboSet);


	    glColor4f(1, 1, 1, 1);
	    glBegin(GL_QUADS);
	    glNormal3f(0, 0, 1);
	    
	    
	    glTexCoord2f(0.0f, 0.0f);
	    glVertex3f(fx1,fy1,0.0f);
	    
	    glTexCoord2f(1.0f, 0.0f);
	    glVertex3f(fx2,fy1,0.0f);
	    
	    glTexCoord2f(1.0f, 1.0f);
	    glVertex3f(fx2,fy2,0.0f);
	    
	    glTexCoord2f(0.0f, 1.0f);
	    glVertex3f(fx1,fy2,0.0f);
	    
	    glEnd();

	    singleton->unsampleFBODirect(gp->gpuRes->fboSet);
	    */



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
				//createSimplexNoise();
			break;

			default:

			break;
		}
	}
#undef LZZ_INLINE
 
