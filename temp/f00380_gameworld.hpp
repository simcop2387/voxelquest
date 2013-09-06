
class GameWorld {
public:


	
	int pageCount;

	
	int iPageSize;
	int iVolumeSize;
	int renderRad;
	int loadRad;
	int diagrams[E_RENDER_LENGTH][E_STATE_LENGTH];
	int* curDiagram;
	int renderMethod;
	bool doDrawFBO;
	
	FIVector4 screenCoords;
	FIVector4 gwSize;
	FIVector4 minWithinLR;
	FIVector4 maxWithinLR;
	FIVector4 curPos;
	FIVector4 camPagePos;
	FIVector4 iPixelWorldCoords;
	FIVector4 pagePos;
	FIVector4 unitPos;
	FIVector4 lastUnitPos;
	FIVector4 lastPagePos;

	Singleton* singleton;
	GamePage** worldData;
	int iBufferSize;
	
	Poco::ThreadPool threadpool;

	int iRSize;


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
		if (i >= gwSize.getIX()) {res = false;}
		if (j >= gwSize.getIY()) {res = false;}
		if (k >= gwSize.getIZ()) {res = false;}

		return res;
	}


	void resetToState(E_STATES resState) {
		int i;
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

		pageCount = 0;


		#ifdef DEBUG_MODE
		pushTrace("GameWorld init");
		#endif

		int i;
		int j;

		doDrawFBO = false;

		for (j = 0; j < E_RENDER_LENGTH; j++) {
			for (i = 0; i < E_STATE_LENGTH; i++) {
				diagrams[j][i] = E_STATE_WAIT;
			}
		}

		renderMethod = (int)E_RENDER_VOL;
		singleton = _singleton;
		gwSize.copyFrom( &(singleton->gwSize) );

		iRSize = singleton->iRSize;

	    diagrams[E_RENDER_VOL][E_STATE_INIT_END] = E_STATE_CREATESIMPLEXNOISE_LAUNCH;
		diagrams[E_RENDER_VOL][E_STATE_CREATESIMPLEXNOISE_END] = E_STATE_COPYTOTEXTURE_LAUNCH;
	    diagrams[E_RENDER_VOL][E_STATE_COPYTOTEXTURE_END] = E_STATE_GENERATEVOLUME_LAUNCH;
	    diagrams[E_RENDER_VOL][E_STATE_GENERATEVOLUME_END] = E_STATE_LENGTH;

		curDiagram = diagrams[renderMethod];

		iPageSize = singleton->iPageSize;
		loadRad = 4;
		
		renderRad = 12;

		iVolumeSize = gwSize.getIX()*gwSize.getIY()*gwSize.getIZ();
		
	    worldData = new GamePage*[iVolumeSize];
	    
	    
		singleton->cameraPos.setIXY(loadRad*iRSize,loadRad*iRSize);	    

		
		for (i = 0; i < iVolumeSize; i++) {
			worldData[i] = NULL;
		}

	    
	    #ifdef DEBUG_MODE
	    popTrace();
	    #endif
	}

	void update(bool changesMade, bool bufferInvalid, int maxH) {


		bool procResult = processPages();

		if (procResult) {
			singleton->wsBufferInvalid = true;
		}

		if (procResult || changesMade) {
			renderPages(maxH);

			
			
			if ( (singleton->grassState != E_GRASS_STATE_ANIM) ) {
				renderGrass();
				renderGeom();
				combineBuffers();


			}

			
		}

		if (singleton->grassState == E_GRASS_STATE_ANIM) {
			renderGrass();
			renderGeom();
			combineBuffers();
			bufferInvalid = true;
		}


		if (procResult || changesMade || bufferInvalid || singleton->rbDown || singleton->lbDown) {

			glClearColor(0.6,0.6,0.7,0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			postProcess();
			
			glutSwapBuffers();
			glFlush();
			
		}


		
		
		
	}

	bool processPages() {


		

		int i, j, k;
		int res;
		int ind;

		int iw = gwSize.getIX();
	    int jw = gwSize.getIY();
	    int kw = gwSize.getIZ();

	    int ii;
	    int jj;
	    int kk;

	    bool cmade = false;




	    camPagePos.copyFrom( &(singleton->cameraPos) );
		camPagePos.intDivXYZ(iRSize);

	    minWithinLR.copyFrom( &(camPagePos) );
	    maxWithinLR.copyFrom( &(camPagePos) );
	    minWithinLR.addXYZ((float)loadRad, -1.0f);
	    maxWithinLR.addXYZ((float)loadRad, 1.0f);

	    int curInd = 0;
		

	    int m;
	    E_STATES nState;


	    int changeCount = 0;
	    int maxChanges = 32;

	    int extraRad = singleton->extraRad;

	    if (singleton->lbDown) {
	    	extraRad = 0;
	    	maxChanges = 1;
	    }

	    if ( singleton->rbDown) {

	    }
	    

		for (k = -(loadRad+extraRad); k <= (loadRad+extraRad); k++) {
			for (j = -(loadRad+extraRad); j <= (loadRad+extraRad); j++) {
				for (i = -(loadRad+extraRad); i <= (loadRad+extraRad); i++) {

					
					/*
					if (threadpool.allocated() >= 16) {
						threadpool.joinAll();
					}
					*/

					ii = i+camPagePos.getIX();
					jj = j+camPagePos.getIY();
					kk = k+camPagePos.getIZ();


					if ( checkBounds(ii,jj,kk) ) {



						ind = kk*jw*iw + jj*iw + ii;

						curPos.setIXYZ(ii*iPageSize,jj*iPageSize,kk*iPageSize);

						


						if (worldData[ind] == NULL) {

							#ifdef DEBUG_MODE
							doTrace("E_STATE_INIT_LAUNCH");
							#endif

							worldData[ind] = new GamePage();
							worldData[ind]->init(singleton, iPageSize, curPos, iRSize*2);

							pageCount++;
							changeCount++;


						}
						else {

							nState = (E_STATES)curDiagram[worldData[ind]->curState];

							switch(nState) {
	                            case E_STATE_CREATESIMPLEXNOISE_LAUNCH:

	                            	#ifdef DEBUG_MODE
	                            	doTrace("E_STATE_CREATESIMPLEXNOISE_LAUNCH");
	                            	#endif

	                            	worldData[ind]->nextState = nState;

	                            	if ( threadpool.available() > 1 ) {

	                            		try {
	                            			threadpool.start(*worldData[ind]);
	                            		}
	                            		catch (SystemException & exc) {
	                            			doTrace("MEM EXCEPTION");
	                            		}

	                            		

	                            		changeCount++;
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

	void renderPages(int maxH) {

		int i, j, k;
		int res;
		int ind;

		int iw = gwSize.getIX();
	    int jw = gwSize.getIY();
	    int kw = gwSize.getIZ();

	    int ii;
	    int jj;
	    int kk;

	    

	    camPagePos.copyFrom( &(singleton->cameraPos) );
	    camPagePos.intDivXYZ(iRSize);

	    int curInd = 0;
		

	    int m;


	    singleton->bindShader("BlitShader");
	    singleton->bindFBO("pagesFBO");

	    

	    maxH = std::max(std::min(renderRad,maxH), -renderRad);

		for (k = -renderRad; k <= renderRad; k++) {
			for (j = -renderRad; j <= renderRad; j++) {
				for (i = -renderRad; i <= renderRad; i++) {

					
					ii = i+camPagePos.getIX();
					jj = j+camPagePos.getIY();
					kk = k+camPagePos.getIZ();

					if ( checkBounds(ii,jj,kk) ) {



						ind = kk*jw*iw + jj*iw + ii;

						if (worldData[ind] == NULL) {

						}
						else {

							if (worldData[ind]->fillState = E_FILL_STATE_PARTIAL) {
								switch(curDiagram[worldData[ind]->curState]) {
									case E_STATE_LENGTH:
										drawPage(worldData[ind], ii, jj, kk);
									break;
								}
							}

							
							
						}

					}

					curInd++;
				}
			}
		}

		singleton->unbindShader();
		singleton->unbindFBO();
	    

		//doTrace( "POSSIBLE ERROR: " , i__s(glGetError()) , "\n" );

	}




	void drawPage(GamePage* gp, int dx, int dy, int dz) {

		int pitchSrc = gp->iRSize2*(singleton->unitScale);
		int pitchSrc2 = gp->iRSize2;

		int dxmod = dx*pitchSrc2 - singleton->cameraPos.getIX();
		int dymod = dy*pitchSrc2 - singleton->cameraPos.getIY();
		int dzmod = dz*pitchSrc2 - singleton->cameraPos.getIZ();


		int x1 = (dxmod-dymod) - pitchSrc2;
		int y1 = (-(dxmod/2) + -(dymod/2) + dzmod) - pitchSrc2;
		int x2 = x1 + pitchSrc;
		int y2 = y1 + pitchSrc;

		float fx1 = x1;
		float fy1 = y1;
		float fx2 = x2;
		float fy2 = y2;

		
		// TODO: should be baseW/H?

		float sx = singleton->bufferDim.getFX();
		float sy = singleton->bufferDim.getFY();

		float myZoom = std::min(1.0f,singleton->cameraZoom);


		fx1 = fx1*(myZoom)/sx;
		fy1 = fy1*(myZoom)/sy;
		fx2 = fx2*(myZoom)/sx;
		fy2 = fy2*(myZoom)/sy;



	    singleton->sampleFBODirect(gp->fboSet);


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

	    singleton->unsampleFBODirect(gp->fboSet);



	}

	void combineBuffers() {

		singleton->bindShader("CombineShader");

		singleton->bindFBO("combineFBO");
		singleton->sampleFBO("pagesFBO",0);
		singleton->sampleFBO("grassFBO",2);
		singleton->sampleFBO("geomFBO",4);


		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		
		singleton->drawFSQuad(1.0f);


		singleton->unsampleFBO("pagesFBO",0);
		singleton->unsampleFBO("grassFBO",2);
		singleton->unsampleFBO("geomFBO",4);
		singleton->unbindFBO();
		singleton->unbindShader();
	}


	void renderGeom() {


		
		//glEnable(GL_DEPTH_TEST);

		singleton->bindShader("GeomShader");
		
		singleton->setShaderFloat("curTime", singleton->curTime);
		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDim));
		
		singleton->bindFBO("geomFBO");
		//singleton->sampleFBO("pagesFBO");


		glEnable(GL_DEPTH_TEST);
		//remember 2x radius
		singleton->drawCubeCentered(singleton->lightPos,32.0);
		glDisable(GL_DEPTH_TEST);

		

		//singleton->unsampleFBO("pagesFBO");
		singleton->unbindFBO();
		singleton->unbindShader();

		//glDisable(GL_DEPTH_TEST);

		
	}


	void modifyUnit(FIVector4 *fPixelWorldCoordsBase, bool doAdd) {

		FIVector4 fPixelWorldCoords;
		fPixelWorldCoords.copyFrom(fPixelWorldCoordsBase);

		float selMod;

		if (doAdd) {
			selMod = 3.0f;
		}
		else {
			selMod = -3.0f;
		}

		fPixelWorldCoords.addXYZ(selMod);


		int iw = gwSize.getIX();
		int jw = gwSize.getIY();
		int kw = gwSize.getIZ();
		int ind;
		int ind2;
		int pageSizeX2 = iPageSize*2;


		int i;
		int j;
		int k;
		int m;

		int ii;
		int jj;
		int kk;

		int newIRS = iRSize*singleton->unitScale;

		FIVector4 tempVec;

		lastUnitPos.copyFrom(&fPixelWorldCoords);
		lastUnitPos.intDivXYZ(singleton->unitSize);
		lastUnitPos.multXYZ(singleton->unitSize);
		lastUnitPos.setFW(singleton->unitSize);

		lastPagePos.copyFrom(&fPixelWorldCoords);
		lastPagePos.intDivXYZ(newIRS);
		lastPagePos.multXYZ(newIRS);
		lastPagePos.setFW(newIRS);

		bool changes = false;

		
		for (m = 0; m < 2; m++) {
			for (i = -1; i <= 1; i++) {
				for (j = -1; j <= 1; j++) {
					for (k = -1; k <= 1; k++) {
						pagePos.copyFrom(&fPixelWorldCoords);
						unitPos.copyFrom(&fPixelWorldCoords);

						pagePos.intDivXYZ( newIRS );
						unitPos.intDivXYZ( singleton->unitSize );


						ii = i+pagePos.getIX();
						jj = j+pagePos.getIY();
						kk = k+pagePos.getIZ();

						if (checkBounds(ii,jj,kk)) {
							ind = kk*jw*iw + jj*iw + ii;

							if (worldData[ind] != NULL) {

								if (
									(worldData[ind]->curState == E_STATE_GENERATEVOLUME_END) ||
									(worldData[ind]->curState == E_STATE_LENGTH)

									) {
									
									// && worldData[ind]->fillState != E_FILL_STATE_EMPTY

									if (
										unitPos.inBoundsXYZ(
											&(worldData[ind]->worldUnitMin),
											&(worldData[ind]->worldUnitMax)
										)
									) {
										tempVec.copyFrom(&unitPos);
										tempVec.addXYZRef( &(worldData[ind]->worldUnitMin), -1.0);

										ind2 = tempVec.getIZ()*pageSizeX2*pageSizeX2 + tempVec.getIY()*pageSizeX2 + tempVec.getIX();
										
										if (ind2 < 0 || ind2 > pageSizeX2*pageSizeX2*pageSizeX2) {
											//doTrace("ind2 out of range ", i__s(ind2), " of ", i__s(pageSizeX2*pageSizeX2*pageSizeX2));
										}
										else {

											if (m == 0) {
												if (doAdd) {
													worldData[ind]->volData[ind2] |= (255<<24);
												}
												else {
													worldData[ind]->volData[ind2] &= (255<<16)|(255<<8)|(255);
												}

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
									else {
										
										// doTraceVec("min: ", &(worldData[ind]->worldUnitMin) );
										// doTraceVec("cur: ", &(unitPos) );
										// doTraceVec("max: ", &(worldData[ind]->worldUnitMax) );
										// doTrace(" ");

									}

								}
								else {
									//doTrace("curState: ", i__s( (int)(worldData[ind]->curState) ));
								}
							}
							else {
								//doTrace("WD is null at", i__s(ii), " ", i__s(jj), " ", i__s(kk) );
							}
						}

					}
				}
			}
		}

		if (changes) {
			singleton->changesMade=true;
		}

		
		

	}

	void renderWorldSpace() {
		doTrace("renderWorldSpace()");

		singleton->wsBufferInvalid = false;

		
		singleton->worldToScreen(&screenCoords, &(singleton->lightPos));

		singleton->bindShader("WorldSpaceShader");
		singleton->setShaderVec2("mouseCoords",singleton->mouseX,singleton->mouseY);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec3("lightPosWS", &(singleton->lightPos));

		singleton->setShaderfVec2("lightPosSS", &screenCoords);
		singleton->setShaderFloat("cameraZoom",singleton->cameraZoom);
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDim) );


		
		singleton->bindFBO("worldSpaceFBO");
		singleton->sampleFBO("pagesFBO",0);
		singleton->sampleFBO("geomFBO",2);

		


		//MUST BE CALLED AFTER FBO IS BOUND
		singleton->setShaderVec2("resolution",singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);

		singleton->drawFSQuad(1.0f);
		
		singleton->unsampleFBO("pagesFBO",0);
		singleton->unsampleFBO("geomFBO",2);

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
		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDim));
		
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




		//singleton->drawFBO("worldSpaceFBO", 0, 1.0 );

		

		singleton->worldToScreen(&screenCoords, &(singleton->lightPos));

		singleton->bindShader("LightingShader");
		singleton->setShaderVec2("mouseCoords",singleton->mouseX,singleton->mouseY);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec3("lightPosWS", &(singleton->lightPos));
		singleton->setShaderfVec2("lightPosSS", &screenCoords);
		singleton->setShaderfVec4("curWorldPos", &(singleton->activeObjPos) );
		singleton->setShaderfVec4("lastUnitPos", &(lastUnitPos) );
		singleton->setShaderfVec4("lastPagePos", &(lastPagePos) );

		singleton->setShaderFloat("cameraZoom",singleton->cameraZoom);
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDim));
		singleton->setShaderFloat("diskOn",singleton->diskOn);
		singleton->setShaderFloat("curTime", singleton->curTime);

		
		singleton->bindFBO("resultFBO");
		singleton->sampleFBO("combineFBO");

		//MUST BE CALLED AFTER FBO IS BOUND
		singleton->setShaderVec2("resolution",singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);

		singleton->drawFSQuad(1.0f);
		singleton->unsampleFBO("combineFBO");
		singleton->unbindFBO();
		singleton->unbindShader();

		float newZoom = std::max(1.0f,singleton->cameraZoom);
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