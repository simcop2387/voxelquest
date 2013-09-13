
class GameWorld {
public:


	
	int pageCount;

	
	int visPageSizeInUnits;
	int iVolumeSize;
	int diagrams[E_RENDER_LENGTH][E_STATE_LENGTH];
	int* curDiagram;
	int renderMethod;
	bool doDrawFBO;

	bool lastProcResult;

	bool updatePoolOrder;

	vector<int> ocThreads;
	
	FIVector4 lScreenCoords;
	FIVector4 aoScreenCoords;
	FIVector4 worldSizeInPages;
	FIVector4 curPos;
	FIVector4 camPagePos;
	FIVector4 iPixelWorldCoords;
	FIVector4 pagePos;
	FIVector4 unitPos;
	FIVector4 lastUnitPos;
	FIVector4 lastPagePos;

	FIVector4 tempVec;
	FIVector4 unitPosMin;
	FIVector4 unitPosMax;
	FIVector4 unitPosMinIS;
	FIVector4 unitPosMaxIS;

	FIVector4 startBounds;
	FIVector4 endBounds;

	Singleton* singleton;
	GamePage** worldData;
	int iBufferSize;
	
	Poco::ThreadPool threadpool;

	int maxThreads;
	int availThreads;

	int visPageSizeInPixels;


	GameWorld()
	{

		// Notes
		// -----
		// Depth: 16 bits
		// Material: 16 bits
		// AO val: 8 bits, normal: 24 bits

	}

	bool checkBounds(int i, int j, int k) {
		
		bool res = true;

		if (i < 0) {res = false;}
		if (j < 0) {res = false;}
		if (k < 0) {res = false;}
		if (i >= worldSizeInPages.getIX()) {res = false;}
		if (j >= worldSizeInPages.getIY()) {res = false;}
		if (k >= worldSizeInPages.getIZ()) {res = false;}

		return res;
	}


	void resetToState(E_STATES resState) {
		int i;

		threadpool.joinAll();

		for (i = 0; i < iVolumeSize; i++) {
			if (worldData[i]) {
				if( worldData[i]->curState > resState) {

					if (worldData[i]->fillState == E_FILL_STATE_PARTIAL) {
						worldData[i]->curState = resState;
					}

					
				}
			}
		}
	}


	


	void init(Singleton* _singleton) {

		#ifdef DEBUG_MODE
		pushTrace("GameWorld init");
		#endif

		int i;
		int j;

		updatePoolOrder = false;

		pageCount = 0;
		lastProcResult = true;
		maxThreads = 7;
		availThreads = maxThreads;

		for (i = 0; i < maxThreads; i++) {
			ocThreads.push_back(-1);
		}

		

		

		doDrawFBO = false;

		for (j = 0; j < E_RENDER_LENGTH; j++) {
			for (i = 0; i < E_STATE_LENGTH; i++) {
				diagrams[j][i] = E_STATE_WAIT;
			}
		}

		renderMethod = (int)E_RENDER_VOL;
		singleton = _singleton;
		worldSizeInPages.copyFrom( &(singleton->worldSizeInPages) );

		visPageSizeInPixels = singleton->visPageSizeInPixels;

	    diagrams[E_RENDER_VOL][E_STATE_INIT_END] = E_STATE_CREATESIMPLEXNOISE_LAUNCH;
		diagrams[E_RENDER_VOL][E_STATE_CREATESIMPLEXNOISE_END] = E_STATE_COPYTOTEXTURE_LAUNCH;
	    diagrams[E_RENDER_VOL][E_STATE_COPYTOTEXTURE_END] = E_STATE_GENERATEVOLUME_LAUNCH;
	    diagrams[E_RENDER_VOL][E_STATE_GENERATEVOLUME_END] = E_STATE_LENGTH;

		curDiagram = diagrams[renderMethod];

		visPageSizeInUnits = singleton->visPageSizeInUnits;

		iVolumeSize = worldSizeInPages.getIX()*worldSizeInPages.getIY()*worldSizeInPages.getIZ();
		
	    worldData = new GamePage*[iVolumeSize];
	    
	      

		
		for (i = 0; i < iVolumeSize; i++) {
			worldData[i] = NULL;
		}

	    
	    #ifdef DEBUG_MODE
	    popTrace();
	    #endif
	}

	void update() {

		bool changesMade = singleton->changesMade;
		bool bufferInvalid = singleton->bufferInvalid;

		bool procResult;
		bool doRenderGeom = false;


		if (singleton->isZooming || singleton->isPanning ) {
			
		}
		else {
			procResult = processPages();
			
			if ( (lastProcResult != procResult) && (procResult == false)  ) {
				singleton->wsBufferInvalid = true;

				updatePoolOrder = true;

			}
		}

			if (procResult || changesMade) {
				renderPages();

				
				
				if ( (singleton->grassState != E_GRASS_STATE_ANIM) ) {
					renderGrass();
					doRenderGeom = true;

				}

				
			}

			if (singleton->grassState == E_GRASS_STATE_ANIM) {
				renderGrass();
				doRenderGeom = true;
				bufferInvalid = true;
			}
		

			


		if (procResult || changesMade || bufferInvalid || singleton->rbDown || singleton->lbDown) {


			if (singleton->mouseState == E_MOUSE_STATE_BRUSH) {
				doRenderGeom = true;
			}

			if (doRenderGeom) {
				renderGeom();
				combineBuffers();
			}
			

			glClearColor(0.6,0.6,0.7,0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			postProcess();
			
			glutSwapBuffers();
			glFlush();
			
		}

		if (singleton->forceGetPD) {
			singleton->forceGetPD = false;
			renderWorldSpace();
		}
	


	


		lastProcResult = procResult;
	}
		
		

	bool processPages() {


		
		int counter;
		int i, j, k;
		int res;
		int ind;

		int iw = worldSizeInPages.getIX();
	    int jw = worldSizeInPages.getIY();
	    int kw = worldSizeInPages.getIZ();

	    int ii;
	    int jj;
	    int kk;

	    bool cmade = false;




	    camPagePos.copyFrom( &(singleton->cameraPos) );
		camPagePos.intDivXYZ(visPageSizeInPixels);

	    int curInd = 0;
		

	    int m;
	    E_STATES nState;

	    int loadRad = singleton->maxW;

	    int changeCount = 0;
	    int maxChanges = 32;

	    if (singleton->lbDown || singleton->rbDown) {
	    	loadRad = 1;
	    	//return false;
	    }
	    


	    // check for threads to free
	    if (availThreads < maxThreads) {
	    	for (i = 0; i < ocThreads.size(); i++) {
	    		if ( ocThreads[i] == -1) {
	    			// already freed
	    		}
	    		else{
	    			if ( worldData[ocThreads[i]] == NULL ) {
	    				// page was destroyed, free thread

	    				ocThreads[i] = -1;
	    				availThreads++;
	    			}
	    			else {
	    				if (worldData[ocThreads[i]]->threadRunning) {

	    				}
	    				else {
	    					ocThreads[i] = -1;
	    					availThreads++;
	    				}
	    			}
	    		}
	    	}
	    }

	    if (availThreads == 0) {
	    	return false;
	    }


		for (k = 0; k < singleton->maxH; k++) {
	    //for (k = -loadRad; k <= loadRad; k++) {
	    	kk = k;//+camPagePos.getIZ();

			for (j = -(loadRad); j <= (loadRad); j++) {
				jj = j+camPagePos.getIY();

				for (i = -(loadRad); i <= (loadRad); i++) {
					ii = i+camPagePos.getIX();

					
					

					if ( checkBounds(ii,jj,kk) ) {



						ind = kk*jw*iw + jj*iw + ii;

						curPos.setIXYZ(ii*visPageSizeInUnits,jj*visPageSizeInUnits,kk*visPageSizeInUnits);

						


						if (worldData[ind] == NULL) {

							#ifdef DEBUG_MODE
							doTrace("E_STATE_INIT_LAUNCH");
							#endif

							worldData[ind] = new GamePage();
							worldData[ind]->init(singleton, ind, &curPos);

							pageCount++;
							changeCount++;


						}
						else {

							
							/*

							if (worldData[ind]->curState == E_STATE_CREATESIMPLEXNOISE_END_NOT_PARTIAL) {
								availThreads++;
								if (availThreads > maxThreads) {
									availThreads = maxThreads;
								}
								worldData[ind]->curState = E_STATE_LENGTH;
							}

							if (worldData[ind]->curState == E_STATE_CREATESIMPLEXNOISE_END_PARTIAL) {
								availThreads++;
								if (availThreads > maxThreads) {
									availThreads = maxThreads;
								}
								worldData[ind]->curState = E_STATE_CREATESIMPLEXNOISE_END;
								
							}

							*/

							nState = (E_STATES)curDiagram[worldData[ind]->curState];


							switch(nState) {
	                            case E_STATE_CREATESIMPLEXNOISE_LAUNCH:

	                            	#ifdef DEBUG_MODE
	                            	doTrace("E_STATE_CREATESIMPLEXNOISE_LAUNCH");
	                            	#endif

	                            	

	                            	if ( availThreads > 0 ) {

	                            		

	                            		counter = 0;
	                            		while (ocThreads[counter] != -1) {
	                            			counter++;
	                            			if (counter >= maxThreads) {
	                            				break;
	                            			}
	                            		}
	                            		if (counter >= maxThreads) {
	                            			
	                            		}
	                            		else {


	                            			ocThreads[counter] = ind;
	                            			
	                            			availThreads--;

	                            			worldData[ind]->nextState = nState;

	                            			try {
	                            				threadpool.start(*worldData[ind]);
	                            			}
	                            			catch (SystemException & exc) {
	                            				doTrace("MEM EXCEPTION");
	                            			}

	                            			changeCount++;

	                            		}


	                            		
	                            	}
	                            	else {
	                            		return cmade;
	                            	}
	                            	

	                            	//worldData[ind]->run();
									
									
								break;
								case E_STATE_COPYTOTEXTURE_LAUNCH:

									

									#ifdef DEBUG_MODE
									doTrace("E_STATE_COPYTOTEXTURE_LAUNCH");
									#endif

									
									worldData[ind]->nextState = nState;
									worldData[ind]->copyToTexture();
									cmade = true;
									
									changeCount++;

									//return cmade;
								break;


								case E_STATE_GENERATEVOLUME_LAUNCH:

									#ifdef DEBUG_MODE
									doTrace("E_STATE_GENERATE_VOLUME_LAUNCH");
									#endif

									
									worldData[ind]->nextState = nState;
									worldData[ind]->generateVolume();
									cmade = true;

									changeCount++;
									
									//return cmade;
								break;

								
								case E_STATE_WAIT:

									//do nothing
								break;
								case E_STATE_LENGTH:
	                            	
								break;
								default:
	                                
								break;

							}

							if (changeCount >= maxChanges) {
								return cmade;
							}
							
						}

					}

					curInd++;
				}
			}
		}



		
		return cmade;
	}

	void renderPages() {

		int i, j, k;
		int res;
		int ind;

		int iw = worldSizeInPages.getIX();
	    int jw = worldSizeInPages.getIY();
	    int kw = worldSizeInPages.getIZ();

	    int ii;
	    int jj;
	    int kk;

	    int loadRad = 8;

	    camPagePos.copyFrom( &(singleton->cameraPos) );
	    camPagePos.intDivXYZ(visPageSizeInPixels);

	    int curInd = 0;
		

	    int m;

	    int drawnPageCount = 0;
	    int skippedPages = 0;


	    singleton->bindShader("BlitShader");
	    singleton->bindFBO("pagesFBO");



		for (k = 0; k < singleton->maxH; k++) {
			kk = k;//+camPagePos.getIZ();
		//for (k = -loadRad; k <= loadRad; k++) {
			//kk = k+camPagePos.getIZ();


			for (j = -loadRad; j <= loadRad; j++) {
				jj = j+camPagePos.getIY();

				for (i = -loadRad; i <= loadRad; i++) {
					ii = i+camPagePos.getIX();

					
					

					

					

					if ( checkBounds(ii,jj,kk) ) {



						ind = kk*jw*iw + jj*iw + ii;

						if (worldData[ind] == NULL) {

						}
						else {

							if (
								(worldData[ind]->fillState == E_FILL_STATE_PARTIAL)
								
							) {
								switch(curDiagram[worldData[ind]->curState]) {
									case E_STATE_LENGTH:

										if (worldData[ind]->usingPoolId == -1) {
											skippedPages++;
										}
										else {
											if (updatePoolOrder) {
												singleton->sendPoolIdToFront(worldData[ind]->usingPoolId);
											}
											drawPage(worldData[ind], ii, jj, kk);
											drawnPageCount++;

											if (drawnPageCount >= singleton->pagePoolIds.size()-2) {

												//doTrace("BROKE EARLY!!! ", i__s(drawnPageCount), " / ", i__s(singleton->pagePoolIds.size()-2));
												//singleton->maxW--;
												goto drawPageExit;
											}

										}

										
									break;
								}
							}

							
							
						}

					}

					curInd++;
				}
			}

			
		}

		drawPageExit:

		singleton->unbindShader();
		singleton->unbindFBO();

		updatePoolOrder = false;

		if (singleton->reportPagesDrawn) {
			singleton->reportPagesDrawn = false;

			doTrace("Pages Drawn: ",i__s(drawnPageCount), " Pages Skipped: ",i__s(skippedPages), " AvailPages: ",i__s(singleton->pagePoolIds.size()-2));
		}
	    

		//doTrace( "POSSIBLE ERROR: " , i__s(glGetError()) , "\n" );

	}




	void drawPage(GamePage* gp, int dx, int dy, int dz) {

		float pitchSrc = (float)((singleton->visPageSizeInPixels*2));
		float pitchSrc2 = (float)((singleton->visPageSizeInPixels*2)/2);

		float dxmod = dx*pitchSrc2 - singleton->cameraPos.getFX();
		float dymod = dy*pitchSrc2 - singleton->cameraPos.getFY();
		float dzmod = dz*pitchSrc2 - singleton->cameraPos.getFZ();


		float fx1 = (dxmod-dymod) - pitchSrc2;
		float fy1 = (-(dxmod/2.0f) + -(dymod/2.0f) + dzmod) - pitchSrc2;
		float fx2 = fx1 + pitchSrc;
		float fy2 = fy1 + pitchSrc;



		
		// TODO: should be baseW/H?

		float sx = singleton->bufferDimHalf.getFX();
		float sy = singleton->bufferDimHalf.getFY();

		float myZoom = std::min(1.0f,singleton->cameraZoom);


		fx1 = fx1*(myZoom)/sx;
		fy1 = fy1*(myZoom)/sy;
		fx2 = fx2*(myZoom)/sx;
		fy2 = fy2*(myZoom)/sy;



	    singleton->sampleFBODirect(gp->gpuRes->fboSet);


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



	}

	void combineBuffers() {

		singleton->bindShader("CombineShader");

		singleton->bindFBO("combineFBO");
		singleton->sampleFBO("pagesFBO",0);
		singleton->sampleFBO("grassFBO",2);
		//singleton->sampleFBO("geomFBO",4);


		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		
		singleton->drawFSQuad(1.0f);

		//singleton->unsampleFBO("geomFBO",4);
		singleton->unsampleFBO("grassFBO",2);
		singleton->unsampleFBO("pagesFBO",0);
		
		singleton->unbindFBO();
		singleton->unbindShader();
	}


	void renderGeom() {


		
		//glEnable(GL_DEPTH_TEST);

		singleton->bindShader("GeomShader");
		
		singleton->setShaderFloat("curTime", singleton->curTime);
		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf));
		
		singleton->bindFBO("geomFBO");
		//singleton->sampleFBO("pagesFBO");


		glEnable(GL_DEPTH_TEST);
		//remember 2x radius


		switch (singleton->mouseState) {
			case E_MOUSE_STATE_BRUSH:
				singleton->setShaderFloat("matVal", 6.0f);
				singleton->drawCubeCentered(lastUnitPos, ((int)singleton->curBrushRad)*(singleton->unitSizeInPixels)  );
				glClear(GL_DEPTH_BUFFER_BIT);
			break;
			case E_MOUSE_STATE_OBJECTS:
				singleton->setShaderFloat("matVal", 4.0f);
				singleton->drawCubeCentered(singleton->lightPos,32.0);

				singleton->setShaderFloat("matVal", 5.0f);
				singleton->drawCubeCentered(singleton->fogPos,32.0);
			break;
			
		}
		

		glDisable(GL_DEPTH_TEST);

		

		//singleton->unsampleFBO("pagesFBO");
		singleton->unbindFBO();
		singleton->unbindShader();

		//glDisable(GL_DEPTH_TEST);

		
	}


	void modifyUnit(FIVector4 *fPixelWorldCoordsBase, E_BRUSH brushAction) {

		int radius = ((int)singleton->curBrushRad);

		FIVector4 fPixelWorldCoords;
		fPixelWorldCoords.copyFrom(fPixelWorldCoordsBase);

		/*
		if (brushAction == E_BRUSH_ADD) {
			fPixelWorldCoords.addXYZ(0.0,0.0,radius*singleton->unitSizeInPixels);
		}
		if (brushAction == E_BRUSH_SUB) {
			fPixelWorldCoords.addXYZ(0.0,0.0,-radius*singleton->unitSizeInPixels);
		}
		*/

		

		


		int iw = worldSizeInPages.getIX();
		int jw = worldSizeInPages.getIY();
		int kw = worldSizeInPages.getIZ();
		int ind;
		int ind2;
		int pageSizeMult = visPageSizeInUnits*(singleton->bufferMult);


		int i, j, k, m, n, o, p;

		int ii;
		int jj;
		int kk;

		int pixelPS = (singleton->unitSizeInPixels*singleton->visPageSizeInUnits);



		lastUnitPos.copyFrom(&fPixelWorldCoords);
		lastUnitPos.intDivXYZ(singleton->unitSizeInPixels);
		lastUnitPos.multXYZ(singleton->unitSizeInPixels);
		lastUnitPos.setFW(singleton->unitSizeInPixels);

		lastPagePos.copyFrom(&fPixelWorldCoords);
		lastPagePos.intDivXYZ(pixelPS);
		lastPagePos.multXYZ(pixelPS);
		lastPagePos.setFW(pixelPS);

		bool changes = false;

		int newRad = 2 + radius/visPageSizeInUnits;


		uint linV;
		uint nearV;

		uint linR;
		uint linG;
		uint linB;
		uint linA;

		uint nearR;
		uint nearG;
		uint nearB;
		uint nearA;

		bool isInside;


		pagePos.copyFrom(&fPixelWorldCoords);
		unitPos.copyFrom(&fPixelWorldCoords);

		pagePos.intDivXYZ( pixelPS );
		unitPos.intDivXYZ( singleton->unitSizeInPixels );


		unitPosMin.copyFrom(&unitPos);
		unitPosMax.copyFrom(&unitPos);

		unitPosMin.addXYZ((float)radius-1, -1.0f);
		unitPosMax.addXYZ((float)radius+1, 1.0f);

		unitPosMinIS.copyFrom(&unitPos);
		unitPosMaxIS.copyFrom(&unitPos);
		unitPosMinIS.addXYZ((float)radius+1, -1.0f);
		unitPosMaxIS.addXYZ((float)radius-1, 1.0f);


		if (brushAction == E_BRUSH_MOVE) {
			return;
		}



		
		for (m = 0; m < 2; m++) {
			for (i = -newRad; i <= newRad; i++) {
				for (j = -newRad; j <= newRad; j++) {
					for (k = -newRad; k <= newRad; k++) {




						ii = i+pagePos.getIX();
						jj = j+pagePos.getIY();
						kk = k+pagePos.getIZ();

						if (checkBounds(ii,jj,kk)) {
							ind = kk*jw*iw + jj*iw + ii;

							if (worldData[ind] == NULL) {

								curPos.setIXYZ(ii*visPageSizeInUnits,jj*visPageSizeInUnits,kk*visPageSizeInUnits);
								worldData[ind] = new GamePage();
								worldData[ind]->init(singleton, ind, &curPos);
								worldData[ind]->createSimplexNoise();

								//doTrace("created new page");
								//worldData[ind]->curState = E_STATE_LENGTH;
							}

							if (
								//(worldData[ind]->curState != E_STATE_CREATESIMPLEXNOISE_BEG) // ||
								//(worldData[ind]->curState == E_STATE_LENGTH)
								true
							) {
								

								startBounds.maxXYZ(&unitPosMin,&(worldData[ind]->worldUnitMin));
								endBounds.minXYZ(&unitPosMax,&(worldData[ind]->worldUnitMax));

								

								

								



								if (
									(startBounds.getFX() > endBounds.getFX()) || 
									(startBounds.getFY() > endBounds.getFY()) ||
									(startBounds.getFZ() > endBounds.getFZ())
								) {
									
								}
								else {

									


									for (n = startBounds.getIX(); n < endBounds.getIX(); n++) {
										for (o = startBounds.getIY(); o < endBounds.getIY(); o++) {
											for (p = startBounds.getIZ(); p < endBounds.getIZ(); p++) {

												tempVec.setIXYZ(n,o,p);
												isInside = tempVec.inBoundsXYZ(&unitPosMinIS,&unitPosMaxIS);
												

												if (
													tempVec.inBoundsXYZ(
														&(worldData[ind]->worldUnitMin),
														&(worldData[ind]->worldUnitMax)
													)
												) {
													tempVec.addXYZRef( &(worldData[ind]->worldUnitMin), -1.0);

													ind2 = tempVec.getIZ()*pageSizeMult*pageSizeMult + tempVec.getIY()*pageSizeMult + tempVec.getIX();
													
													if (ind2 < 0 || ind2 >= pageSizeMult*pageSizeMult*pageSizeMult) {
														//doTrace("ind2 out of range ", i__s(ind2), " of ", i__s(pageSizeMult*pageSizeMult*pageSizeMult));
													}
													else {

														if (m == 0) {

															linV = worldData[ind]->volDataLinear[ind2];
															nearV = worldData[ind]->volData[ind2];

															linR = (linV)&255;
															linG = (linV>>8)&255;
															linB = (linV>>16)&255;
															linA = (linV>>24)&255;

															nearR = (nearV)&255;
															nearG = (nearV>>8)&255;
															nearB = (nearV>>16)&255;
															nearA = (nearV>>24)&255;


															if (p >= singleton->maxHeightInUnits) {

																linA = 0;
															}
															else {

																if (brushAction == E_BRUSH_SUB) {


																	if (isInside) {
																		if (singleton->softMode) {
																			linA = min(linA, linA-8);
																		}
																		else {
																			linA = 0;
																		}
																	}
																	
																}
																else {

																	if (isInside) {
																		if (singleton->softMode) {
																			linA += 8;
																			if (linA > 255) {
																				linA = 255;
																			}
																		}
																		else {
																			linA = 255;
																		}
																	}

																	

																	switch(singleton->activeMode) {
																		//
																		case 0:

																		break;

																		// dirt and grass
																		case 1:
																			linR = 255;
																			linG = 255;
																			linB = 255;

																			nearA = 0;
																		break;

																		// rock
																		case 2:
																			linR = 255;
																			linG = 255;
																			linB = 255;

																			nearA = 255;
																		break;
																		
																		// brick
																		case 3:
																			linR = 16;
																			linG = 255;
																			linB = 16;

																			nearA = 255;
																		break;
																			
																		// flat top
																		case 4:
																			linB = 0;
																			nearA = 255;
																		break;
																		
																		//
																		case 5:
																			linB = 0;
																			nearA = 0;
																		break;
																		
																		//
																		case 6:

																		break;
																		
																		//
																		case 7:

																		break;

																		//
																		case 8:

																		break;
																		
																		//
																		case 9:

																		break;
																		
																	}
																}
															}

															worldData[ind]->volData[ind2] = (nearA<<24)|(nearB<<16)|(nearG<<8)|(nearR);
															worldData[ind]->volDataLinear[ind2] = (linA<<24)|(linB<<16)|(linG<<8)|(linR);
															

															worldData[ind]->isDirty = true;
															changes = true;
														}
														else {
															if (worldData[ind]->isDirty) {
																worldData[ind]->isDirty = false;
																worldData[ind]->copyToTexture();
																worldData[ind]->generateVolume();
															}
														}

													}

												}


											}
										}
									}
								}

								


								

							}
							
						}


						




						

						

					}
				}
			}
		}

		if (changes) {
			singleton->changesMade=true;
			singleton->wsBufferInvalid=true;
		}

		
		

	}

	void renderWorldSpace() {
		//doTrace("renderWorldSpace() TOT GPU MEM USED (MB): ", f__s(TOT_GPU_MEM_USAGE));

		singleton->wsBufferInvalid = false;

		singleton->bindShader("WorldSpaceShader");
		

		singleton->bindFBO("worldSpaceFBO");
		singleton->sampleFBO("pagesFBO",0);
		singleton->sampleFBO("geomFBO",2);

		
		//MUST BE CALLED AFTER FBO IS BOUND
		singleton->setShaderVec2("resolution",singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);
		singleton->setShaderVec2("mouseCoords",singleton->mouseX,singleton->mouseY);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec4("fogPos", &(singleton->fogPos));
		singleton->setShaderfVec3("lightPosWS", &(singleton->lightPos));
		singleton->setShaderFloat("cameraZoom",singleton->cameraZoom);
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf) );


		singleton->drawFSQuad(1.0f);

		
		singleton->unsampleFBO("geomFBO",2);
		singleton->unsampleFBO("pagesFBO",0);
		

		singleton->unbindFBO();
		singleton->unbindShader();

		float newZoom = std::max(1.0f,singleton->cameraZoom);
		singleton->drawFBO("resultFBO", 0, newZoom );

		FBOWrapper* fbow = singleton->getFBOWrapper("worldSpaceFBO",0);

		fbow->getPixels();

	}


	void renderGrass() {


		float curTime = 0.0;

		if (singleton->grassState == E_GRASS_STATE_ANIM) {
			curTime = singleton->curTime;
		}

		
		//glEnable(GL_DEPTH_TEST);

		singleton->bindShader("GrassShader");
		
		singleton->setShaderFloat("curTime", curTime);
		singleton->setShaderFloat("grassHeight", singleton->grassHeight);
		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf));

		
		singleton->bindFBO("grassFBO");
		singleton->sampleFBO("pagesFBO");

		if (singleton->grassState == E_GRASS_STATE_ANIM || singleton->grassState == E_GRASS_STATE_ON) {
			glCallList(singleton->grassTris);
		}
		

		singleton->unsampleFBO("pagesFBO");
		singleton->unbindFBO();
		singleton->unbindShader();

		//glDisable(GL_DEPTH_TEST);

		
	}

	void postProcess() {

		// NOTE: ALWAYS UNSAMPLE IN REVERSE ORDER!!!


		//singleton->drawFBO("worldSpaceFBO", 0, 1.0 );

		float newZoom;

		singleton->worldToScreen(&lScreenCoords, &(singleton->lightPos));
		singleton->worldToScreen(&aoScreenCoords, &(singleton->activeObjectPos));

		singleton->bindShader("LightingShader");
		singleton->setShaderVec2("mouseCoords",singleton->mouseX,singleton->mouseY);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec3("lightPosWS", &(singleton->lightPos));
		singleton->setShaderfVec2("lightPosSS", &lScreenCoords);
		singleton->setShaderfVec2("aoPosSS", &aoScreenCoords);
		singleton->setShaderfVec4("fogPos", &(singleton->fogPos));
		singleton->setShaderfVec4("activeObjectPos", &(singleton->activeObjectPos));
		
		singleton->setShaderfVec4("lastUnitPos", &(lastUnitPos) );
		singleton->setShaderfVec4("lastPagePos", &(lastPagePos) );




		singleton->setShaderFloat("cameraZoom",singleton->cameraZoom);
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf));
		singleton->setShaderFloat("diskOn",singleton->diskOn);
		singleton->setShaderFloat("curTime", singleton->curTime);

		
		singleton->bindFBO("resultFBO");
		singleton->sampleFBO("combineFBO",0);
		singleton->sampleFBO("geomFBO", 2);

		//MUST BE CALLED AFTER FBO IS BOUND
		singleton->setShaderVec2("resolution",singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);

		singleton->drawFSQuad(1.0f);
		singleton->unsampleFBO("geomFBO", 2);
		singleton->unsampleFBO("combineFBO",0);
		singleton->unbindFBO();
		singleton->unbindShader();

		

		newZoom = std::max(1.0f,singleton->cameraZoom);
		singleton->drawFBO("resultFBO", 0, newZoom );

		
		
	}

	~GameWorld() {


		threadpool.stopAll();

		int i;

		for (i = 0; i < iVolumeSize; i++) {
			if (worldData[i]) {
				delete worldData[i];
			}
			
		}
		
	}


};