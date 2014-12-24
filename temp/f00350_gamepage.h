// f00350_gamepage.h
//

#include "f00350_gamepage.e"
#define LZZ_INLINE inline
GamePage::GamePage ()
                   {
		terRes = -1;
		parentBlock = NULL;
		volData = NULL;
		volDataLinear = NULL;
		cellData = NULL;
	}
void GamePage::init (Singleton * _singleton, GamePageHolder * _parentGPH, int _thisPageId, int offsetX, int offsetY, int offsetZ, int oxLoc, int oyLoc, int ozLoc, bool _isEntity)
          {

		

		isEntity = false;
		hasLines = false;
		hasSolids = false;
		hasTrans = false;
		isDirty = true;
		

		isEntity = _isEntity;

		thisPageId = _thisPageId;
		singleton = _singleton;
		parentGPH = _parentGPH;
		gw = singleton->gw;
		
		if (!isEntity) {
			parentBlock = gw->getBlockAtId(parentGPH->blockId);
		}
		
		
		

		maxEntries = 32;
		isRendering = true;

		int i;

		volDataModified = false;
		threadRunning = false;


		//maxHeightInUnits = (singleton->maxHeightInUnits);



		int visPageSizeInUnits = singleton->visPageSizeInUnits;
		bufferedPageSizeInUnits = (visPageSizeInUnits) * (singleton->bufferMult);

		offsetInPages.setIXYZ(offsetX, offsetY, offsetZ);
		offsetInUnits.copyFrom(&offsetInPages);
		offsetInUnits.multXYZ(singleton->visPageSizeInUnits);

		offsetInPagesLocal.setIXYZ(oxLoc, oyLoc, ozLoc);

		float hzp = (float)(singleton->holderSizeInPages);

		pageDepth = ((1.0f - ( (offsetInPagesLocal.getFZ() * hzp * hzp + offsetInPagesLocal.getFY() * hzp + offsetInPagesLocal.getFX()) / (hzp * hzp * hzp) )) * 0.9f + 0.05f) * 0.5f;


		unitSizeInPixels = (float)(singleton->unitSizeInPixels);



		iVolumeSize = bufferedPageSizeInUnits * bufferedPageSizeInUnits * bufferedPageSizeInUnits;




		worldMinVisInPixels.copyFrom(&offsetInUnits);
		worldMaxVisInPixels.copyFrom(&offsetInUnits);
		worldMaxVisInPixels.addXYZ(visPageSizeInUnits);
		worldMinVisInPixels.multXYZ((float)unitSizeInPixels);
		worldMaxVisInPixels.multXYZ((float)unitSizeInPixels);

		worldUnitMin.copyFrom(&offsetInUnits);
		worldUnitMax.copyFrom(&offsetInUnits);
		worldUnitMax.addXYZ(visPageSizeInUnits);

		worldUnitMin.addXYZ( -(bufferedPageSizeInUnits - visPageSizeInUnits) / 2 );
		worldUnitMax.addXYZ( (bufferedPageSizeInUnits - visPageSizeInUnits) / 2 );

		worldMinBufInPixels.copyFrom(&worldUnitMin);
		worldMaxBufInPixels.copyFrom(&worldUnitMax);
		worldMinBufInPixels.multXYZ((float)unitSizeInPixels);
		worldMaxBufInPixels.multXYZ((float)unitSizeInPixels);

		voroSize.setFXYZ(
			singleton->visPageSizeInPixels,
			singleton->visPageSizeInPixels,
			singleton->visPageSizeInPixels
		);


		centerPosition.copyFrom(&worldMinVisInPixels);
		centerPosition.addXYZRef(&worldMaxVisInPixels);
		centerPosition.multXYZ(0.5f);

		
		
		hasWater = ( singleton->getSLInPixels() >= worldMinVisInPixels.getFZ() );
		
		
		if (isEntity) {
			addEntityGeom(true);
		}
		else {			
			addGeom(true);
		}
		
		



	}
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
				volDataLinear[i] = (0 << 24) | (0 << 16) | (0 << 8) | (0);
			}
		}

		return volDataLinear;
	}
void GamePage::copyToTexture (bool isForEmptyVD)
                                              {


		int id1 = singleton->volId;
		int id2 = singleton->volIdLinear;

		if (isForEmptyVD) {
			id1 = singleton->volIdEmpty;
			id2 = singleton->volIdEmptyLinear;
		}

		glBindTexture(GL_TEXTURE_3D, id1);
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

		glBindTexture(GL_TEXTURE_3D, 0);
		glBindTexture(GL_TEXTURE_3D, id2);
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
		glBindTexture(GL_TEXTURE_3D, 0);





		/*
		glTexSubImage2D(
		  GLenum    target,
		  GLint   level,
		  GLint   xoffset,
		  GLint   yoffset,
		  GLsizei   width,
		  GLsizei   height,
		  GLenum    format,
		  GLenum    type,
		  const GLvoid *    data
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


		glBindTexture(GL_TEXTURE_2D, singleton->volId);
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
void GamePage::setFalse ()
                        {
		hasLines = false;
		hasSolids = false;
		hasTrans = false;
		hasTree = false;
		hasWindow = false;
		hasGeom = false;
		hasTerrain = false;
	}
void GamePage::addEntityGeom (bool justTesting)
                                             {
		
		int i;
		int j;
		int k;
		int m;
		int n;
		int p;
		int ind;
		int bufSize = (singleton->visPageSizeInPixels * singleton->bufferMult);
		int geomInPage;
		int baseInd;

		GamePageHolder *gph;
		GameEnt *gg;

		paramsPerEntry = E_GP_LENGTH * 3;
		numEntries = 0;

		bool doProc;

		for (i = 0; i < E_MAT_PARAM_LENGTH; i++) {
			singleton->matCountArr[i] = 0.0f;
		}

		


		if (justTesting) {
			setFalse();
		}

		
		gph = parentGPH;

		if (gph) {
			geomInPage = gph->entityGeom.size();
			

			for (m = 0; m < geomInPage; m++) {
				
				gg = gph->entityGeom[m];

				if (

					FIVector4::intersect( gg->getVisMinInPixelsT(), gg->getVisMaxInPixelsT(), &worldMinBufInPixels, &worldMaxBufInPixels )
					&& (gg->visible)

				) {

					if (justTesting) {
						
						
						if (gg->buildingType == E_CT_LINES) {
							hasLines = true;
						}
						else {
							hasGeom = true;
						}
						
						
					}
					else {
						for (p = 0; p < E_GP_LENGTH; p++) {
							baseInd = numEntries * paramsPerEntry + p * 3;
							singleton->paramArr[baseInd + 0] = gg->geomParams[p].getFX();
							singleton->paramArr[baseInd + 1] = gg->geomParams[p].getFY();
							singleton->paramArr[baseInd + 2] = gg->geomParams[p].getFZ();
						}

						singleton->matCountArr[gg->geomParams[E_GP_MATPARAMS].getIX()] += 1.0f;

						numEntries++;
						
					}
				}
			}
		}
		
		

		if (justTesting) {
			parentGPH->hasTrans = false;
			hasTrans = false;
			if (hasGeom||hasLines) {
				parentGPH->hasSolids = true;
				hasSolids = true;
			}
		}
		else {
			if (numEntries > maxEntries) {
				numEntries = maxEntries;
				doTraceND("limit exceeded");
			}

			totParams = numEntries * paramsPerEntry;
		}


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
		int bufSize = (singleton->visPageSizeInPixels * singleton->bufferMult);
		intPair curId;
		int geomInPage;
		int baseInd;

		FIVector4 start;
		FIVector4 end;

		GamePageHolder *gph;
		GameEnt *gg;

		paramsPerEntry = E_GP_LENGTH * 3;
		numEntries = 0;

		bool doProc;

		start.copyFrom( &worldMinBufInPixels );
		end.copyFrom( &worldMaxBufInPixels );

		start.addXYZ(-bufSize);
		end.addXYZ(bufSize);

		start.intDivXYZ(singleton->holderSizeInPixels);
		end.intDivXYZ(singleton->holderSizeInPixels);

		start.clampZ( singleton->origin.getFZ(), singleton->worldSizeInHolders.getFZ() - 1.0f );
		end.clampZ( singleton->origin.getFZ(), singleton->worldSizeInHolders.getFZ() - 1.0f );

		for (i = 0; i < E_MAT_PARAM_LENGTH; i++) {
			singleton->matCountArr[i] = 0.0f;
		}

		if (justTesting) {
			setFalse();
		}


		for (k = start.getIZ(); k <= end.getIZ(); k++ ) {
			for (j = start.getIY(); j <= end.getIY(); j++ ) {
				for (i = start.getIX(); i <= end.getIX(); i++ ) {
					gph = gw->getHolderAtCoords(i, j, k);

					// TODO critical: make sure holders are ready before pages


					if (gph) {
						geomInPage = gph->containsEntIds[E_ET_GEOM].data.size();

						for (m = 0; m < geomInPage; m++) {
							curId = gph->containsEntIds[E_ET_GEOM].data[m];
							gg = &(gw->blockData[curId.v0]->gameEnts[E_ET_GEOM].data[curId.v1]);


							if (

								FIVector4::intersect( gg->getVisMinInPixelsT(), gg->getVisMaxInPixelsT(), &worldMinBufInPixels, &worldMaxBufInPixels )
								&& (gg->visible)

							) {

								if (justTesting) {
									if ( (gg->buildingType == E_CT_WINDOW) || (gg->buildingType == E_CT_LANTERN)) {
										hasWindow = true;
									}

									if (gg->buildingType == E_CT_TREE) {
										hasTree = true;
									}
									hasGeom = true;
								}
								else {

									doProc = true;

									for (n = 0; n < numEntries; n++) {
										if (singleton->entIdArr[n] == curId) {
											doProc = false;
										}
									}

									if (doProc) {

										singleton->entIdArr[numEntries] = curId;



										for (p = 0; p < E_GP_LENGTH; p++) {

											baseInd = numEntries * paramsPerEntry + p * 3;

											singleton->paramArr[baseInd + 0] = gg->geomParams[p].getFX();
											singleton->paramArr[baseInd + 1] = gg->geomParams[p].getFY();
											singleton->paramArr[baseInd + 2] = gg->geomParams[p].getFZ();
										}

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
			
			terRes = parentBlock->isNearTerrain(&centerPosition);
			
			hasTerrain = (terRes == E_TER_GROUNDLEVEL);
			
			if (terRes == E_TER_UNDERGROUND) {
				
			}
			else {
				if (hasWindow || hasWater) {
					parentGPH->hasTrans = true;
					hasTrans = true;
					if (MAX_LAYERS < 2) {
						parentGPH->hasSolids = true;
						hasSolids = true;
					}
				}

				if (hasTerrain || hasGeom || volDataModified) {
					parentGPH->hasSolids = true;
					hasSolids = true;
				}
			}
			
			
						
			
		}
		else {
			if (numEntries > maxEntries) {
				numEntries = maxEntries;
				cout << "limit exceeded " << numEntries << " / " << maxEntries << "\n";
			}

			totParams = numEntries * paramsPerEntry;
		}




	}
void GamePage::getVoroPoints ()
                             {


		int i, j, k;

		int counter = 0;
		int rad = 2;
		int tempi = 0;
		
		//int iMaxPoints = 27;
		//float fMaxPoints = (float)iMaxPoints;

		float fi, fj, fk;

		
		FIVector4 posFloored;
		FIVector4 posFlooredInPixels;
		
		// posFloored.copyFrom(&worldMaxVisInPixels);
		// posFloored.addXYZRef(&worldMinVisInPixels);
		// posFloored.multXYZ(0.5f);

		posFloored.copyFrom(&offsetInPages);
		posFloored.addXYZ(0.5f);
		
		posFlooredInPixels.copyFrom(&posFloored);
		posFlooredInPixels.multXYZ(&voroSize);

		// posFloored.divXYZ(&voroSize);
		// posFloored.floorXYZ();


		int totVoro = 27;
		
		

		for (i = -rad; i <= rad; i++) {
			fi = (float)i;
			for (j = -rad; j <= rad; j++) {
				fj = (float)j;
				for (k = -rad; k <= rad; k++) {
					fk = (float)k;


					curPos.copyFrom(&posFloored);
					curPos.addXYZ(fi, fj, fk);


					// if (
					//  (curPos.getIX() + curPos.getIY() + curPos.getIZ()) % 3 == 0
					// ) {




					//if (curPos.getIZ()%3 == 0) {

					randNum.setRand(&curPos);
					//randNum.multXYZ(1.0f);

					singleton->voroVecArr[counter].copyFrom(&curPos);
					singleton->voroVecArr[counter].addXYZRef(&randNum);
					singleton->voroVecArr[counter].multXYZ(&voroSize);

					//testNum.setRand(&curPos);

					//if (testNum.getFX() > 1.0f - (fMaxPoints/125.0f) ) { //true) {//
					// singleton->voroArr[counter * 4 + 0] = newPos.getFX();
					// singleton->voroArr[counter * 4 + 1] = newPos.getFY();
					// singleton->voroArr[counter * 4 + 2] = newPos.getFZ();
					//singleton->voroArr[counter * 4 + 3] = (testNum.getFX()) * 0.5 + 0.5;
					counter++;
					//}

					// if (counter >= iMaxPoints) {
					// 	counter = iMaxPoints;
					// 	goto VORO_DONE;
					// }
					//}



				}
			}
		}

// VORO_DONE:
// 		voroCount = counter;


		


		for (i = 0; i < 125; i++){
			singleton->indexArr[i].index1 = i;
			singleton->indexArr[i].value = singleton->voroVecArr[i].distance(&posFlooredInPixels);
		}
		
		bubbleSortF(singleton->indexArr, 125);
		
		int curInd = 0;
		for (i = 0; i < 27; i++) {
			curInd = singleton->indexArr[i].index1;
			singleton->voroArr[i * 4 + 0] = singleton->voroVecArr[curInd].getFX();
			singleton->voroArr[i * 4 + 1] = singleton->voroVecArr[curInd].getFY();
			singleton->voroArr[i * 4 + 2] = singleton->voroVecArr[curInd].getFZ();
		}

		
		voroCount = 27;
	}
void GamePage::addAllGeom ()
                          {
		if (isEntity) {
			
			addEntityGeom(true);
			addEntityGeom(false);
		}
		else {
			
			addGeom(true);
			addGeom(false);
			
		}
	}
void GamePage::generateVolume (bool dd)
                                             {
		PAGE_COUNT++;

		int curVGFBO = CUR_VG_FBO;
		CUR_VG_FBO++;
		if (CUR_VG_FBO >= MAX_VG_FBOS) {
			CUR_VG_FBO = 0;
		}
		
		int curVGTFBO = CUR_VGT_FBO;
		CUR_VGT_FBO++;
		if (CUR_VGT_FBO >= MAX_VGT_FBOS) {
			CUR_VGT_FBO = 0;
		}
		
		// 1 = 0
		// 2 = 1
		// 4 = 2
		// 8 = 2
		
		float curBlendAmount = 0.5f;
		

		// int hspLog = 2;
		
		// switch (singleton->holderSizeInPages) {
		// 	case 1:
		// 		hspLog = 0;
		// 	break;
		// 	case 2:
		// 		hspLog = 1;
		// 	break;
		// }
		
		
		int resIndex = 0;
		int i;
		int mLayers = 1;
		bool didRender = false;
		isRendering = true;
		
		if (volDataModified) {
			copyToTexture(false);
		}
		else {
			if (singleton->emptyVDNotReady) {
				singleton->emptyVDNotReady = false;
				copyToTexture(true);
			}
		}
		
		
		

		parentGPH->clearSet(); //false
		getVoroPoints();


		if (dd) {
			
			hasTerrain = true;
			hasSolids = true;
			parentGPH->hasSolids = true;
			
		}


		/////////////////////////////////////////
		// BEGIN GEN VOLUME
		/////////////////////////////////////////


		if (isEntity) {
			curBlendAmount = 1.0f;
			singleton->bindShader("GenerateVolumeEnt");
		}
		else {
			singleton->bindShader("GenerateVolume");
		}

		

		singleton->bindFBODirect(&(singleton->vgFBOArr[curVGFBO]), 0);

		if (volDataModified) {
			singleton->setShaderTexture3D(0, singleton->volId);
			singleton->setShaderTexture3D(1, singleton->volIdLinear);
		}
		else {
			singleton->setShaderTexture3D(0, singleton->volIdEmpty);
			singleton->setShaderTexture3D(1, singleton->volIdEmptyLinear);
		}

		singleton->sampleFBO("hmFBOLinear", 2);
		
		if (!isEntity) {
			resIndex = parentBlock->copyTerToTexture();
			singleton->setShaderTexture3D(4, singleton->terTextures[resIndex].texId);
		}
		
		
		singleton->setShaderFloat("totLayers", MAX_LAYERS);
		singleton->setShaderFloat("pixelsPerCell", singleton->pixelsPerCell);	
		singleton->setShaderFloat("volumePitch", (float)( singleton->volGenFBOX ));
		singleton->setShaderfVec4("worldMinBufInPixels", &(worldMinBufInPixels));
		singleton->setShaderfVec4("worldMaxBufInPixels", &(worldMaxBufInPixels));

		if (isEntity) {
			
		}
		else {
			singleton->setShaderfVec4("worldSizeInPixels", &(singleton->worldSizeInPixels));	
			singleton->setShaderFloat("seaLevel", singleton->getSLInPixels() );
			singleton->setShaderfVec4("blockMinBufInPixels", &(parentBlock->blockMinBufInPixels));
			singleton->setShaderfVec4("blockMaxBufInPixels", &(parentBlock->blockMaxBufInPixels));
		}
		
		


		singleton->setShaderInt("hasTree", (int)hasTree);
		singleton->setShaderInt("hasGeom", (int)hasGeom);
		singleton->setShaderInt("hasTerrain", (int)hasTerrain);
		singleton->setShaderInt("hasLines", (int)hasLines);

		if (hasGeom||hasTree||hasLines) {
			singleton->setShaderInt("paramsPerEntry", (paramsPerEntry / 3) );
			singleton->setShaderInt("numEntries", numEntries);
			singleton->setShaderArrayfVec3("paramArr", singleton->paramArr, totParams / 3);
			singleton->setShaderArray("matCountArr", singleton->matCountArr, E_MAT_PARAM_LENGTH);
		}

		//singleton->setShaderInt("voroCount", voroCount);
		singleton->setShaderArrayfVec4("voroArr", singleton->voroArr, voroCount);

		singleton->drawFSQuad();

		if (!isEntity) {
			singleton->setShaderTexture3D(4, 0);
		}

		
		//singleton->setShaderTexture(3,0);
		singleton->unsampleFBO("hmFBOLinear", 2);
		singleton->setShaderTexture3D(1, 0);
		singleton->setShaderTexture3D(0, 0);
		singleton->unbindFBO();
		singleton->unbindShader();


		/////////////////////////////////////////
		// END GEN VOLUME
		/////////////////////////////////////////







		/////////////////////////////////////////
		// BEGIN RENDER VOLUME
		/////////////////////////////////////////


		

		if (parentGPH->gpuRes != NULL) {
			singleton->bindShader("RenderVolume");
			

			

			if (
				
				(hasTerrain || hasGeom || hasLines || hasWater || hasWindow)
				
			) {
				
				
				
				didRender = true;
				
				singleton->bindFBODirect(&(singleton->vgtFBOArr[curVGTFBO]), 1);	

				singleton->sampleFBODirect(&(singleton->vgFBOArr[curVGFBO]),0);
				//singleton->sampleFBO("frontFaceFBO", 2);
				//singleton->sampleFBO("backFaceFBO", 3);

				singleton->setShaderFloat("blendAmount",curBlendAmount);
				//singleton->setShaderFloat("curLayer", i);
				singleton->setShaderFloat("volGenSuperMod", singleton->volGenSuperMod);
				singleton->setShaderFloat("volumePitch", (float)( singleton->volGenFBOX ));
				//singleton->setShaderFloat("holderSizeInPagesLog", hspLog);
				singleton->setShaderFloat("bufferMult", (float)(singleton->bufferMult));
				singleton->setShaderFloat("visPageSizeInPixels", (float)(singleton->visPageSizeInPixels));
				singleton->setShaderfVec3("worldMinVisInPixels", &(worldMinVisInPixels));
				singleton->setShaderfVec3("worldMaxVisInPixels", &(worldMaxVisInPixels));
				
				singleton->drawFSQuad();

				
				//singleton->unsampleFBO("backFaceFBO",3);
				//singleton->unsampleFBO("frontFaceFBO",2);
				singleton->unsampleFBODirect(&(singleton->vgFBOArr[curVGFBO]),0);
				singleton->unbindFBO();
			}

			

			singleton->unbindShader();
		}
		
		


		/////////////////////////////////////////
		// END RENDER VOLUME
		/////////////////////////////////////////




		
		
		if (didRender) {
			
			getPoints(curVGTFBO);
			
		}
		
		isDirty = false;
		isRendering = false;

	}
int GamePage::getIndex (int i, int j, int k, int p)
                                                 {
		return (i + j*p + k*p*p);
	}
void GamePage::getPoints (int fboNum)
                                   {
		
		int sres = singleton->volGenSuperRes;
		int sresM1 = sres-1;
		float fres = sres;
		
		
		
		
		int cellsPerPage = singleton->cellsPerPage;
		int cellDataSize = cellsPerPage*cellsPerPage*cellsPerPage;
		int cdBufferSize = cellDataSize*E_CD_LENGTH;
		int ci;
		
		float fVisPageSizeInPixels = singleton->visPageSizeInPixels;
		
		int i, j, k, m, n, p, q, t;
		
		int ni;
		int nj;
		int nk;
		
		int ind = 0;
		int ind2 = 0;
		
		
		bool doMip = MAX_MIP_LEV > 1;
		singleton->vgtFBOArr[fboNum].fbos[0].getPixels(doMip);
		singleton->vgtFBOArr[fboNum].fbos[1].getPixels(doMip);
		
		if (doMip) {
			singleton->vgtFBOArr[fboNum].fbos[0].updateMips3D(sres);
			singleton->vgtFBOArr[fboNum].fbos[1].updateMips3D(sres);
		}
		
		
		int procCount[MAX_LAYERS];
		
		bool edgeK;
		bool edgeJ;
		bool edgeI;
		
		
		//float iv0 = 0.0f;
		//float iv1 = singleton->voxelSizeInWC;
		
		int mval;

		
		int MIN_MIP = 0;
		int MAX_MIP = 1;
		int AVG_MIP = 2;
		
		float fi = 0.0f;
		float fj = 0.0f;
		float fk = 0.0f;
		
		
		float bpX = 0.0f;
		float bpY = 0.0f;
		float bpZ = 0.0f;
		
		//uint dirFlags;
		bool isCand;
		
		uint flagVals[6];
		flagVals[0] = 1;
		flagVals[1] = 2;
		flagVals[2] = 4;
		flagVals[3] = 8;
		flagVals[4] = 16;
		flagVals[5] = 32;
		
		const uint AIR_VAL = 0;
		
		FBOWrapper* fbow0 = &(singleton->vgtFBOArr[fboNum].fbos[0]);
		FBOWrapper* fbow1 = &(singleton->vgtFBOArr[fboNum].fbos[1]);
		
		bool doProc;
		bool hasAir = false;
		bool hasSolid = false;
		bool hasSolidAndAir = false;
		
		
		
		for (i = 0; i < sres*sres*sres; i++) {
			
			if (fbow0->getPixelAtIndex(i,A_CHANNEL) == 0) {
				hasAir = true;
			}
			else {
				hasSolid = true;
			}
			
			if (fbow0->getPixelAtIndex(i,G_CHANNEL) != 0) {
				hasSolidAndAir = true;
			}
		}
		
		
		
		for (p = 0; p < MAX_MIP_LEV*MAX_LAYERS; p++) {
			vertices[p].data.clear();
		}
		
		
		
		
		
		
		
		// determine collision matrix
		if (hasSolid||hasSolidAndAir) {
			if (cellData == NULL) {
				cellData = new int[cellDataSize];
			}
			
			for (i = 0; i < cellDataSize; i++) {
				cellData[i] = E_CD_EMPTY;
			}
			for (i = 0; i < cdBufferSize; i++) {
				singleton->cdBuffer[i] = 0;
			}
			
			
			for (k = 0; k < sres; k++) {
				nk = ((k*cellsPerPage)/sres)*cellsPerPage*cellsPerPage;
				for (j = 0; j < sres; j++) {
					nj = ((j*cellsPerPage)/sres)*cellsPerPage;
					for (i = 0; i < sres; i++) {
						ni = ((i*cellsPerPage)/sres);
						ind = getIndex(i,j,k,sres);
						ind2 = nk + nj + ni;
						
						q = fbow0->getPixelAtIndex(ind,A_CHANNEL);
						p = singleton->cdMap[q];
						
						
						
						singleton->cdBuffer[p*cellDataSize + ind2] += 1;
						
					}
				}
			}
			
			
			// for (j = 0; j < E_CD_LENGTH; j++) {
			// 		totCol[j] = 0;
			// 		for (i = 0; i < cellDataSize; i++) {
			// 			totCol[j] += singleton->cdBuffer[j*cellDataSize + i];
			// 		}
			// }
			
			
			
			for (i = 0; i < cellDataSize; i++) {
				
				m = singleton->cdBuffer[E_CD_EMPTY*cellDataSize + i]/4;
				p = E_CD_EMPTY;
				
				for (j = 1; j < E_CD_LENGTH; j++) {
					q = singleton->cdBuffer[j*cellDataSize + i];
					if (q >= m) {
						m = q;
						p = j;
					}
				}
				
				cellData[i] = p;
				
				
				
			}
			
		}
		
		
		
		
		if ( (hasAir&&hasSolid)||(hasSolidAndAir) ) {
			
		}
		else {
			
			goto DO_CLEANUP;
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		for (p = 0; p < MAX_MIP_LEV; p++) {
			
			for (q = 0; q < MAX_LAYERS; q++) {
				procCount[q] = 0;
			}
			
			
			
			if (p == 0) {
				mval = 3;
			}
			else {
				mval = MAX_MIP;
			}
			
			
			for (n = 0; n < 2; n++) {
				
				
				
				for (k = 0; k < sres; k++) {
					fk = k;
					
					for (j = 0; j < sres; j++) {
						fj = j;
						
						for (i = 0; i < sres; i++) {
							fi = i;
							
							ind = getIndex(i,j,k,sres);
							
							
							
							
							
							if (
								(fbow0->getPixelAtIndex3DMip(ind,A_CHANNEL,mval,p) == 0)
							) {
								// is air
							}
							else {
								
								q = fbow0->getPixelAtIndex3DMip(ind,R_CHANNEL,mval,p);
								
								isCand = fbow0->getPixelAtIndex3DMip(ind,G_CHANNEL,mval,p) != 0;
								
								
								
								
								// front facing: counter clock wise
								
								doProc = false;
								
								
								
								// x + 
								if (i != sresM1) {
									ind2 = getIndex(i+1,j,k,sres);
									
									
									t = fbow0->getPixelAtIndex3DMip(ind2,R_CHANNEL,mval,p);
									doProc = doProc||(PROC_MATRIX[q][t]);
								}
								else {
									doProc = doProc||isCand;
								}
								
								// x - 
								if (i != 0) {
									ind2 = getIndex(i-1,j,k,sres);
									t = fbow0->getPixelAtIndex3DMip(ind2,R_CHANNEL,mval,p);
									doProc = doProc||(PROC_MATRIX[q][t]);
								}
								else {
									doProc = doProc||isCand;
								}
								
								// y + 
								if (j != sresM1) {
									ind2 = getIndex(i,j+1,k,sres);
									t = fbow0->getPixelAtIndex3DMip(ind2,R_CHANNEL,mval,p);
									doProc = doProc||(PROC_MATRIX[q][t]);
								}
								else {
									doProc = doProc||isCand;
								}
								
								// y - 
								if (j != 0) {
									ind2 = getIndex(i,j-1,k,sres);
									t = fbow0->getPixelAtIndex3DMip(ind2,R_CHANNEL,mval,p);
									doProc = doProc||(PROC_MATRIX[q][t]);
								}
								else {
									doProc = doProc||isCand;
								}
								
								// z + 
								if (k != sresM1) {
									ind2 = getIndex(i,j,k+1,sres);
									t = fbow0->getPixelAtIndex3DMip(ind2,R_CHANNEL,mval,p);
									doProc = doProc||(PROC_MATRIX[q][t]);
								}
								else {
									doProc = doProc||isCand;
								}
								
								// z- 
								if (k != 0) {
									ind2 = getIndex(i,j,k-1,sres);
									t = fbow0->getPixelAtIndex3DMip(ind2,R_CHANNEL,mval,p);
									doProc = doProc||(PROC_MATRIX[q][t]);
								}
								else {
									doProc = doProc||isCand;
								}
								
								
								if (doProc) {
									if (n == 0) {
										procCount[q]++;
									}
									else {
										bpX = worldMinVisInPixels.getFX() + ((fi)/fres)*fVisPageSizeInPixels;
										bpY = worldMinVisInPixels.getFY() + ((fj)/fres)*fVisPageSizeInPixels;
										bpZ = worldMinVisInPixels.getFZ() + ((fk)/fres)*fVisPageSizeInPixels;
										
										ci = p*MAX_LAYERS+q;
										
										vertices[ci].data.push_back(fbow1->getPixelAtIndex3DMip(ind,R_CHANNEL,mval,p)*2.0f/255.0f - 1.0f);
										vertices[ci].data.push_back(fbow1->getPixelAtIndex3DMip(ind,G_CHANNEL,mval,p)*2.0f/255.0f - 1.0f);
										vertices[ci].data.push_back(fbow1->getPixelAtIndex3DMip(ind,B_CHANNEL,mval,p)*2.0f/255.0f - 1.0f);
										vertices[ci].data.push_back(
											fbow0->getPixelAtIndex3DMip(ind,B_CHANNEL,mval,p) +
											fbow0->getPixelAtIndex3DMip(ind,A_CHANNEL,mval,p)*256
										);
										
										vertices[ci].data.push_back(bpX);
										vertices[ci].data.push_back(bpY);
										vertices[ci].data.push_back(bpZ);
										vertices[ci].data.push_back(1.0f);
										
										//totalPointCount++;
										
										//getPixVal(fbow0,fbow1,ind, bpX,bpY,bpZ, iv0,iv0,iv0);
										
									}
								}
								
							}
						}	
					}
				}
				
				if (n==0) {
					for (q = 0; q < 2; q++) {
						ci = p*MAX_LAYERS+q;
						if (procCount[q] > 0) {
							vertices[ci].data.reserve((procCount[q]+1)*8);
						}
					}
					
					
				}
				
				
			}
			
			
			
			
			
			
			
			sres = sres/2;
			sresM1 = sres-1;
			fres = sres;
			
		}
		
		
		
		
		
DO_CLEANUP:

	;
	
	
	// if (vertices[0].data.size() > 0) {
	// 	if (isEntity) {
	// 		cout << "RENDER ENT\n";
	// 	}
	// }
	
		
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
#undef LZZ_INLINE
 
