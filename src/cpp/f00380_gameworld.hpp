
class GameWorld {
public:


	int numProvinces;
	int seaLevel;
	int pageCount;
	int mapSwapFlag;
	int visPageSizeInUnits;
	int iHolderSize;
	int iBlockSize;
	int diagrams[E_RENDER_LENGTH][E_STATE_LENGTH];
	int renderMethod;
	int iBufferSize;
	int maxThreads;
	int availThreads;
	int visPageSizeInPixels;
	int holderSizeInPages;

	int MIN_MIP;
	int MAX_MIP;
	int AVG_MIP;

	int stChannel;
	int btChannel;
	int pathChannel;
	int houseChannel;
	
	int hmChannel;
	int idChannel;
	int densityChannel;
	int blockChannel;

	int* curDiagram;
	int* provinceGrid;
	int* provinceX;
	int* provinceY;

	bool doDrawFBO;
	bool lastProcResult;
	bool mapLockOn;

	float mapStep;
	float mapTrans;

	std::vector<coordAndIndex> roadCoords;

	

	vector<int> ocThreads;
	
	FIVector4 lScreenCoords;
	FIVector4 aoScreenCoords;
	FIVector4 worldSizeInPages;
	FIVector4 worldSizeInHolders;
	FIVector4 worldSizeInBlocks;

	FIVector4 camPagePos;
	FIVector4 camHolderPos;
	FIVector4 camBlockPos;
	FIVector4 iPixelWorldCoords;
	FIVector4 pagePos;
	FIVector4 unitPos;
	FIVector4 lastUnitPos;
	FIVector4 lastPagePos;

	FIVector4 minLRInPixels;
	FIVector4 maxLRInPixels;
	


	FIVector4 tempVec;
	FIVector4 unitPosMin;
	FIVector4 unitPosMax;
	FIVector4 unitPosMinIS;
	FIVector4 unitPosMaxIS;

	FIVector4 startBounds;
	FIVector4 endBounds;

	Singleton* singleton;
	GamePageHolder** holderData;
	GameBlock** blockData;

	FBOWrapper* curFBO;
	FBOWrapper* curFBO2;
	
	
	Poco::ThreadPool threadpool;

	


	GameWorld()
	{

		// Notes
		// -----
		// Depth: 16 bits
		// Material: 16 bits
		// AO val: 8 bits, normal: 24 bits

	}


	void init(Singleton* _singleton) {

		pushTrace("GameWorld init()");


		int i;
		int j;

		mapLockOn = false;

		numProvinces = 32;
		provinceGrid = new int[numProvinces*numProvinces];
		provinceX = new int[numProvinces];
		provinceY = new int[numProvinces];
		seaLevel;

		mapSwapFlag = 0;
		mapStep = 0.0f;

		pageCount = 0;
		lastProcResult = true;
		maxThreads = 7;
		availThreads = maxThreads;

		for (i = 0; i < maxThreads; i++) {
			ocThreads.push_back(-1);
		}

		hmChannel = 0;
		idChannel = 1;
		densityChannel = 2;
		blockChannel = 3;

		stChannel = 0;
		btChannel = 1;
		pathChannel = 2;
		houseChannel = 3;
		



		MIN_MIP = 0;
		MAX_MIP = 1;
		AVG_MIP = 2;
		

		doDrawFBO = false;

		for (j = 0; j < E_RENDER_LENGTH; j++) {
			for (i = 0; i < E_STATE_LENGTH; i++) {
				diagrams[j][i] = E_STATE_WAIT;
			}
		}

		renderMethod = (int)E_RENDER_VOL;
		singleton = _singleton;
		worldSizeInPages.copyFrom( &(singleton->worldSizeInPages) );
		worldSizeInHolders.copyFrom( &(singleton->worldSizeInHolders) );
		worldSizeInBlocks.copyFrom( &(singleton->worldSizeInBlocks) );

		visPageSizeInPixels = singleton->visPageSizeInPixels;

	    diagrams[E_RENDER_VOL][E_STATE_INIT_END] = E_STATE_GENERATEVOLUME_LAUNCH;//E_STATE_CREATESIMPLEXNOISE_LAUNCH;
		diagrams[E_RENDER_VOL][E_STATE_CREATESIMPLEXNOISE_END] = E_STATE_GENERATEVOLUME_LAUNCH;//E_STATE_COPYTOTEXTURE_LAUNCH;
	    //diagrams[E_RENDER_VOL][E_STATE_COPYTOTEXTURE_END] = E_STATE_GENERATEVOLUME_LAUNCH;
	    diagrams[E_RENDER_VOL][E_STATE_GENERATEVOLUME_END] = E_STATE_LENGTH;

		curDiagram = diagrams[renderMethod];

		holderSizeInPages = singleton->holderSizeInPages;
		visPageSizeInUnits = singleton->visPageSizeInUnits;

		iHolderSize = worldSizeInHolders.getIX()*worldSizeInHolders.getIY()*worldSizeInHolders.getIZ();
		iBlockSize = worldSizeInBlocks.getIX()*worldSizeInBlocks.getIY();

		holderData = new GamePageHolder*[iHolderSize];
		for (i = 0; i < iHolderSize; i++) {
			holderData[i] = NULL;
		}

		blockData = new GameBlock*[iBlockSize];
		for (i = 0; i < iBlockSize; i++) {
			blockData[i] = NULL;
		}
	    


	    popTrace();
	}



	int wrapCoord(int val, int mv) {
		while (val < 0) {
			val += mv;
		}
		while (val >= mv) {
			val -= mv;
		}

		//val = val % max;

		return val;
	}


	GameBlock* getBlockAtCoords(int xInBlocks, int yInBlocks, bool createOnNull = false) {
		

		int newX = wrapCoord(xInBlocks,worldSizeInBlocks.getIX());
		int newY = wrapCoord(yInBlocks,worldSizeInBlocks.getIY());

		int ind =
			newY*worldSizeInBlocks.getIX() +
			newX;

		if (blockData[ind]) {

		}
		else {
			if (createOnNull) {
				blockData[ind] = new GameBlock();
				blockData[ind]->init(singleton, ind, xInBlocks, yInBlocks);
			}
		}

		return blockData[ind];
		
	}


	GamePageHolder* getHolderAtCoords(int x, int y, int z, bool createOnNull = false) {
		

		int newX = wrapCoord(x,worldSizeInHolders.getIX());
		int newY = wrapCoord(y,worldSizeInHolders.getIY());
		int newZ = z;

		int ind =
			newZ*worldSizeInHolders.getIX()*worldSizeInHolders.getIY() +
			newY*worldSizeInHolders.getIX() +
			newX;

		if (holderData[ind]) {

		}
		else {
			if (createOnNull) {
				holderData[ind] = new GamePageHolder();
				holderData[ind]->init(singleton, ind, x, y, z, x, y, z);
			}
		}

		return holderData[ind];
		
	}

	// assumed that holder exists when calling this
	GamePageHolder* getHolderAtIndex(int ind) {
		

		return holderData[ind];

	}


	// assumed that page exists when calling this
	GamePage* getPageAtIndex(int ind) {
		//pushTrace("getPageAtIndex()");

		int newInd = ind;
		int x, y, z;
		GamePage* gp;

		z = newInd / (worldSizeInPages.getIX()*worldSizeInPages.getIY());
		newInd -= z*(worldSizeInPages.getIX()*worldSizeInPages.getIY());

		y = newInd / (worldSizeInPages.getIX());
		newInd -= y*(worldSizeInPages.getIX());

		x = newInd;

		gp = getPageAtCoords(x,y,z,false);

		//popTrace();

		return gp;
		
	}
	

	GamePage* getPageAtCoords(int x, int y, int z, bool createOnNull = false) {
		//pushTrace("getPageAtCoords()");
		int hx, hy, hz;
		int px, py, pz;
		int gpInd;
		//int xmod = 0;
		//int ymod = 0;
		int newX = wrapCoord(x,worldSizeInPages.getIX());
		int newY = wrapCoord(y,worldSizeInPages.getIY());
		int newZ = z;
		int ind = 
			newZ*worldSizeInPages.getIX()*worldSizeInPages.getIY() +
			newY*worldSizeInPages.getIX() +
			newX;

		GamePage* gp = NULL;

		px = newX%holderSizeInPages;
		py = newY%holderSizeInPages;
		pz = newZ%holderSizeInPages;

		


		// if (x < 0) {
		// 	if (x%holderSizeInPages == 0) {

		// 	}
		// 	else {
		// 		xmod = -1;
		// 	}
			
		// }
		// if (y < 0) {
		// 	if (y%holderSizeInPages == 0) {

		// 	}
		// 	else {
		// 		ymod = -1;
		// 	}
		// }
		


		GamePageHolder* gph = getHolderAtCoords(
			intDiv(x,holderSizeInPages),
			intDiv(y,holderSizeInPages),
			intDiv(z,holderSizeInPages),
			createOnNull
		);
		

		if ( gph ) {


			
			gpInd = pz*holderSizeInPages*holderSizeInPages + py*holderSizeInPages + px;
			//gph->getPageIndex(px,py,pz);

			if (gpInd == -1) {
				// prob
				cout << "Invalid holder index\n";
			}
			else {
				gp = gph->pageData[gpInd];

				if (gp) {

				}
				else {
					if (createOnNull) {


						//cout << "x: " << x << " y: " << y << " newX: " << newX << " newY: " << newY << " px: " << px << " py: " << py << "\n";

						gph->pageData[gpInd] = new GamePage();
						gp = gph->pageData[gpInd];
						gp->init(
							singleton,
							gph,
							ind,
							x, y, z,//newX, newY, newZ,
							px, py, pz
						);
					}
				}
			}

		}
		else {
			
		}

		//popTrace();
		return gp;

	}



	bool checkBounds(int k) { //int i, int j, int k) {
		//pushTrace("checkBounds()");
		
		bool res = true;

		//if (i < 0) {res = false;}
		//if (j < 0) {res = false;}
		if (k < 0) {res = false;}
		//if (i >= worldSizeInPages.getIX()) {res = false;}
		//if (j >= worldSizeInPages.getIY()) {res = false;}
		if (k >= worldSizeInPages.getIZ()) {res = false;}

		//popTrace();
		return res;
		
	}


	void resetToState(E_STATES resState) {
		pushTrace("resetToState()");

		int i;
		int j;

		threadpool.joinAll();

		GamePage* curPage;

		for (i = 0; i < iHolderSize; i++) {
			if (holderData[i]) {

				for (j = 0; j < singleton->holderSizeInPages; j++) {
					curPage = holderData[i]->pageData[j];

					if (curPage) {
						if (curPage->curState > resState) {

							if (curPage->fillState == E_FILL_STATE_PARTIAL) {
								curPage->curState = resState;
							}

						}
					}
				}

				
			}
		}
		popTrace();
	}



	void update() {

		pushTrace("update()");

		int i;

		float x;
		float y;
		float z;

		mapTrans = 1.0f-singleton->cameraZoom/0.1f;
		if (mapTrans > 0.91) {
			mapTrans = 1.0;
		}
		if (mapTrans < 0.1) {
			mapTrans = 0.0;
		}

		if (mapLockOn) {
			return;
		}

		if (singleton->mapInvalid) {
			initMap();

			//popTrace();
			//return;
		}


		bool changesMade = singleton->changesMade;
		bool bufferInvalid = singleton->bufferInvalid;

		bool procResult;
		bool doRenderGeom = true;


		if (mapTrans < 1.0f) {

			if (singleton->isZooming || singleton->isPanning ) { //(false) { //
				
			}
			else {
				procResult = processPages();
				
				if ( (lastProcResult != procResult) && (procResult == false)  ) {
					singleton->wsBufferInvalid = true;

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
		popTrace();
	}
		
		

	bool processPages() {

		pushTrace("processPages()");

		
		int counter;
		int i, j, k;
		int res;
		

	    int ii;
	    int jj;
	    int kk;

	    bool cmade = false;

	    float heightAtPoint = singleton->getHeightAtPixelPos(singleton->cameraPos.getFX(), singleton->cameraPos.getFY());


	    camPagePos.copyFrom( &(singleton->cameraPos) );
	    //camPagePos.setFZ(heightAtPoint);
		camPagePos.intDivXYZ(visPageSizeInPixels);

		camHolderPos.copyFrom(&camPagePos);
		camHolderPos.intDivXYZ(singleton->holderSizeInPages);

		camBlockPos.copyFrom( &(singleton->cameraPos) );
		camBlockPos.intDivXYZ(singleton->blockSizeInPixels);

		
	    GamePage* curPage;
	    GameBlock* curBlock;

	    int m;
	    E_STATES nState;

	    int loadRad = singleton->maxW;
	    int loadRad2 = singleton->maxH;

	    int changeCount = 0;
	    int maxChanges = 32;

	    if (singleton->lbDown || singleton->rbDown) {
	    	//loadRad = 1;
	    	//return false;
	    }
	    


	    // check for threads to free
	    if (availThreads < maxThreads) {
	    	for (i = 0; i < ocThreads.size(); i++) {
	    		if ( ocThreads[i] == -1) {
	    			// already freed
	    		}
	    		else{
	    			if ( getPageAtIndex(ocThreads[i]) == NULL ) {
	    				// page was destroyed, free thread

	    				ocThreads[i] = -1;
	    				availThreads++;
	    			}
	    			else {
	    				if (getPageAtIndex(ocThreads[i])->threadRunning) {

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
	    	popTrace();
	    	return false;
	    }

	    minLRInPixels.copyFrom(&camPagePos);
	    maxLRInPixels.copyFrom(&camPagePos);
	    minLRInPixels.addXYZ(-loadRad,-loadRad,-loadRad2);
	    maxLRInPixels.addXYZ(loadRad,loadRad,loadRad2);
	    minLRInPixels.multXYZ(singleton->visPageSizeInPixels);
	    maxLRInPixels.multXYZ(singleton->visPageSizeInPixels);

	    int blockRad = 2;
	    for (j = -blockRad; j <= blockRad; j++) {
	    	for (i = -blockRad; i <= blockRad; i++) {
	    		ii = i + camBlockPos.getIX();
	    		jj = j + camBlockPos.getIY();

	    		curBlock = getBlockAtCoords(ii, jj, true);

	    	}
	    }


		//for (k = 0; k < singleton->maxH; k++) {
	    for (k = -loadRad2; k <= loadRad2; k++) {
	    	kk = k+camPagePos.getIZ();

			for (j = -(loadRad); j <= (loadRad); j++) {
				jj = j+camPagePos.getIY();

				for (i = -(loadRad); i <= (loadRad); i++) {
					ii = i+camPagePos.getIX();

					
					

					if ( checkBounds(kk) ) {

						
						curPage = getPageAtCoords(ii, jj, kk);

						if (curPage == NULL) {

							
							doTrace("E_STATE_INIT_LAUNCH");
							

							curPage = getPageAtCoords(ii, jj, kk, true);

							doTrace("E_STATE_INIT_LAUNCH_END");

							pageCount++;
							changeCount++;


						}
						else {



							nState = (E_STATES)curDiagram[curPage->curState];


							switch(nState) {
	                            case E_STATE_CREATESIMPLEXNOISE_LAUNCH:

	                            	
	                            	doTrace("E_STATE_CREATESIMPLEXNOISE_LAUNCH");
	                            	

	                            	

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


	                            			ocThreads[counter] = curPage->thisPageId;
	                            			
	                            			availThreads--;

	                            			curPage->nextState = nState;

	                            			try {
	                            				threadpool.start(*curPage);
	                            			}
	                            			catch (SystemException & exc) {
	                            				doTrace("MEM EXCEPTION");
	                            			}

	                            			changeCount++;

	                            		}


	                            		
	                            	}
	                            	else {

	                            		popTrace();
	                            		return cmade;
	                            	}
	                            	

	                            	//curPage->run();
									
									
								break;


								case E_STATE_GENERATEVOLUME_LAUNCH:

									
									doTrace("E_STATE_GENERATEVOLUME_LAUNCH");
									

									
									curPage->nextState = nState;
									curPage->generateVolume();
									cmade = true;

									changeCount++;
									
									//popTrace();
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
								popTrace();
								return cmade;
							}
							
						}

					}

					
				}
			}
		}



		popTrace();
		
		return cmade;
	}


	void renderPages() {

		pushTrace("renderPages()");

		int i, j, k, m;
		int res;
		int drawnPageCount = 0;
		int skippedPages = 0;
		int cid;
		int ppSize = singleton->orderedIds.size();

		singleton->bindShader("BlitShader");
		singleton->bindFBO("pagesFBO");

	    GamePageHolder* gp;

	    for (i = 0; i < ppSize; i++) {
	    	cid = singleton->orderedIds[i];

	    	if (cid == -1) {

	    	}
	    	else {
	    		gp = getHolderAtIndex(cid);

	    		if (gp == NULL) {

	    		}
	    		else {

	    			//if (gp->offsetInHolders.manhattanDis(&camHolderPos) <= 1.0 ) {
	    				drawHolder(gp);
	    			//}

	    		}
	    	}
	    	
	    }


		singleton->unbindShader();
		singleton->unbindFBO();
  

		//doTrace( "POSSIBLE ERROR: " , i__s(glGetError()) , "\n" );

		popTrace();
	}



	void drawHolder(GamePageHolder* gp) {
		pushTrace("drawHolder()");

		int dx = gp->trueOffsetInHolders.getIX();
		int dy = gp->trueOffsetInHolders.getIY();
		int dz = gp->trueOffsetInHolders.getIZ();

		float pitchSrc = (float)((singleton->holderSizeInPixels*2));
		float pitchSrc2 = (float)((singleton->holderSizeInPixels*2)/2);

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


		if (gp->gpuRes != NULL) {
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

	    


	    popTrace();

	}



	void combineBuffers() {
		pushTrace("combineBuffers()");

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
		popTrace();
	}


	void renderGeom() {
		pushTrace("renderGeom()");


		
		//glEnable(GL_DEPTH_TEST);

		singleton->bindShader("GeomShader");
		
		singleton->setShaderFloat("curTime", singleton->curTime);
		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf));
		singleton->setShaderFloat("isWire", 0.0);
		
		singleton->bindFBO("geomFBO");
		//singleton->sampleFBO("pagesFBO");


		glEnable(GL_DEPTH_TEST);
		//remember 2x radius


		switch (singleton->mouseState) {

			case E_MOUSE_STATE_MOVE:

				if (singleton->gridOn == 1.0f) {
					singleton->setShaderFloat("matVal", 8.0f);
					
					//singleton->drawCubeCentered(singleton->cameraPos,32.0);
					
					singleton->setShaderFloat("isWire", 1.0);
					singleton->drawBox(minLRInPixels, maxLRInPixels);
				}
				



			break;

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

		// singleton->setShaderFloat("matVal", 9.0f);
		// singleton->setShaderFloat("isWire", 0.0);
		// singleton->drawBox(minLRInPixels, maxLRInPixels);
		

		glDisable(GL_DEPTH_TEST);

		

		//singleton->unsampleFBO("pagesFBO");
		singleton->unbindFBO();
		singleton->unbindShader();

		//glDisable(GL_DEPTH_TEST);

		popTrace();
		
	}


	void modifyUnit(FIVector4 *fPixelWorldCoordsBase, E_BRUSH brushAction) {

		pushTrace("modifyUnit()");

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

		

		GamePage* curPage;


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
			popTrace();
			return;
		}



		
		for (m = 0; m < 2; m++) {
			for (i = -newRad; i <= newRad; i++) {
				for (j = -newRad; j <= newRad; j++) {
					for (k = -newRad; k <= newRad; k++) {




						ii = i+pagePos.getIX();
						jj = j+pagePos.getIY();
						kk = k+pagePos.getIZ();

						if (checkBounds(kk)) {

							//

							curPage = getPageAtCoords(ii,jj,kk);

							if (curPage == NULL) {

								curPage = getPageAtCoords(ii,jj,kk, true);
								
								//curPage->createSimplexNoise();

								//doTrace("created new page");
								//curPage->curState = E_STATE_LENGTH;
							}

							if (
								//(curPage->curState != E_STATE_CREATESIMPLEXNOISE_BEG) // ||
								//(curPage->curState == E_STATE_LENGTH)
								true
							) {
								

								startBounds.maxXYZ(&unitPosMin,&(curPage->worldUnitMin));
								endBounds.minXYZ(&unitPosMax,&(curPage->worldUnitMax));

								

								

								



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
												
												//TODO: fix this to wrap
												if (
													tempVec.inBoundsXYZ(
														&(curPage->worldUnitMin),
														&(curPage->worldUnitMax)
													)
												) {
													tempVec.addXYZRef( &(curPage->worldUnitMin), -1.0);

													ind2 = tempVec.getIZ()*pageSizeMult*pageSizeMult + tempVec.getIY()*pageSizeMult + tempVec.getIX();
													
													if (ind2 < 0 || ind2 >= pageSizeMult*pageSizeMult*pageSizeMult) {
														//doTrace("ind2 out of range ", i__s(ind2), " of ", i__s(pageSizeMult*pageSizeMult*pageSizeMult));
													}
													else {

														if (m == 0) {

															linV = curPage->volDataLinear[ind2];
															nearV = curPage->volData[ind2];

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

															curPage->volData[ind2] = (nearA<<24)|(nearB<<16)|(nearG<<8)|(nearR);
															curPage->volDataLinear[ind2] = (linA<<24)|(linB<<16)|(linG<<8)|(linR);
															

															curPage->parentGPH->isDirty = true;
															changes = true;
														}
														else {
															if (curPage->parentGPH->isDirty) {
																curPage->parentGPH->isDirty = false;
																
																//curPage->generateVolume();
																curPage->parentGPH->refreshChildren();
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

		
		
		popTrace();

	}

	void renderWorldSpace() {


		pushTrace("renderWorldSpace()");

		doTraceND("renderWorldSpace() TOT GPU MEM USED (MB): ", f__s(TOT_GPU_MEM_USAGE));

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


		popTrace();
	}


	void renderGrass() {


		pushTrace("renderGrass()");

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

		
		popTrace();
	}

	float weighPath(float x1, float y1, float x2, float y2, float rad, bool doSet, bool isOcean) {
		
		int i;
		int iMax = (int)min(64.0f, rad*4.0f);

		float fi;
		float fMax = (float)(iMax-1);
		float lerp;

		float curX;
		float curY;


		float startVal = curFBO->getPixelAtWrapped((int)x1, (int)y1, hmChannel);
		float lastRes = startVal;
		float curRes;
		float curRes2;
		float tempRes;
		float tot = 0.0f;

		bool startsInWater = startVal <= seaLevel;

		for (i = 1; i < iMax; i++) {
			fi = (float)i;
			lerp = fi/fMax;
			curX = (1.0f-lerp)*x1 + (lerp)*x2;
			curY = (1.0f-lerp)*y1 + (lerp)*y2;
			curRes = curFBO->getPixelAtWrapped((int)curX, (int)curY, hmChannel);
			
			if (isOcean) {
				if (doSet) {

					if (curRes > seaLevel) {

						tempRes = abs(curRes-lastRes);
						tempRes = tempRes*tempRes*tempRes;
						//tempRes = 255-curRes;
					}
					else {
						tempRes = curRes;
					}

					
					//tempRes = abs(curRes-lastRes);
					//tempRes = tempRes*tempRes*tempRes;
					if ((curRes > seaLevel) != (lastRes > seaLevel)) {
						tempRes += 100000.0f;
					}

					// if (startsInWater) {
					// 	if (curRes > seaLevel) {
					// 		tempRes += 1000000.0f;
					// 	}
					// }

				}
				else {
					tempRes = 0.0;
					if (curRes > seaLevel) {
						tempRes = 1.0f;
					}
					else {
						tempRes = -1.0f;
					}
				}
			}
			else {
				tempRes = abs(curRes-lastRes)*10.0 + curRes;
				//tempRes = tempRes;
				if (curRes <= seaLevel-20.0f) {
					tempRes += 100000.0f;
				}
				if ((curRes > seaLevel) != (lastRes > seaLevel)) {
					tempRes += 100000.0f;
				}
			}
			

			tot += tempRes;
			lastRes = curRes;

		}

		return tot;
	}

	float findBestPath(float x1, float y1, float x2, float y2, int generation, int roadIndex, bool doSet, bool isOcean) {
		int i;
		int j;

		coordAndIndex baseCoord;
		coordAndIndex bestCoord;
		coordAndIndex testCoord;

		float mpx = (x1+x2)/2.0;
		float mpy = (y1+y2)/2.0;
		float dis = quickDis(x1,y1,x2,y2);
		float rad = dis/2.0;
		float mpxTemp;
		float mpyTemp;
		float delta;
		float bestDis;
		float curDis;
		float bestDelta = FLT_MAX;
		float bestX;
		float bestY;
		float genMod;

		int q;
		int p;

		int ibx;
		int iby;
		int ix2;
		int iy2;
		int tot1 = 0;
		int tot2 = 0;
		int iRad;
		int numTries = max((int)(rad), 20);

		if ( (rad < 2.0f) || (generation > 1024) ) {
			// do manhattan distance

			if (doSet) {
				ibx = x1;
				iby = y1;
				ix2 = x2;
				iy2 = y2;


				while (ibx != ix2) {
					curFBO2->setPixelAtWrapped(ibx, iby, pathChannel, 255);
					if (ibx < ix2) {
						ibx++;
					}
					else {
						ibx--;
					}
				}
				while (iby != iy2) {
					curFBO2->setPixelAtWrapped(ibx, iby, pathChannel, 255);
					if (iby < iy2) {
						iby++;
					}
					else {
						iby--;
					}
				}
				
				curFBO2->setPixelAtWrapped(ibx, iby, pathChannel, 255);
			}
			else {

			}

			return 0.0f;
			
		}



		genMod = 2.0f;

		// if (generation > 32) {
		// 	return 0.0f;
		// }
		

		

		for (i = 0; i < numTries; i++) {
			mpxTemp = mpx + (fGenRand()*dis-rad)/genMod;
			mpyTemp = mpy + (fGenRand()*dis-rad)/genMod;

			delta = weighPath(x1,y1,mpxTemp,mpyTemp,rad/2.0f, doSet, isOcean);
			delta += weighPath(mpxTemp,mpyTemp,x2,y2,rad/2.0f, doSet, isOcean);

			if (delta < bestDelta) {
				bestDelta = delta;
				bestX = mpxTemp;
				bestY = mpyTemp;
			}

		}



		if (doSet) {
			

			
			if (generation < 8) {

				baseCoord.x = bestX;
				baseCoord.y = bestY;
				baseCoord.index = roadIndex;
				//roadCoords.push_back(baseCoord);

				bestDis = FLT_MAX;
				for (i = 0; i < roadCoords.size(); i++) {
					testCoord = roadCoords[i];


					if (baseCoord.index == testCoord.index) {

					}
					else {
						curDis = coordDis(&baseCoord,&testCoord);

						if (curDis < bestDis) {
							bestDis = curDis;
							bestCoord = roadCoords[i];
						}
					}
					
					
				}

				if (bestDis != FLT_MAX) {
					curDis = coordDis(&bestCoord,&baseCoord);

					if (curDis < min(400.0f, rad) ) { //
						baseCoord = bestCoord;
						baseCoord.index = roadIndex;
					}

					bestX = baseCoord.x;
					bestY = baseCoord.y;
				}

				roadCoords.push_back(baseCoord);
			}
			

			


			


			//curFBO2->setPixelAtWrapped((int)bestX, (int)bestY, pathChannel, 255);
			//curFBO2->setPixelAtWrapped((int)bestX, (int)bestY, breadCrumbChannel, 255);
			findBestPath(x1,y1,bestX,bestY,generation+1,roadIndex, doSet, isOcean);
			findBestPath(bestX,bestY,x2,y2,generation+1,roadIndex, doSet, isOcean);
		}

		return bestDelta;

	}

	void initMap() {
		mapLockOn = true;

		pushTrace("initMap()");

		mapSwapFlag = 0;
		
		mapStep = 0.0f;

		

		FBOWrapper* fbow = singleton->getFBOWrapper("hmFBO",0);
		FBOWrapper* fbow2 = singleton->getFBOWrapper("cityFBO",0);

		curFBO = fbow;
		curFBO2 = fbow2;

		FIVector4 tempVec1;
		FIVector4 tempVec2;
		FIVector4 tempVec3;

		FIVector4 startVec;
		FIVector4 endVec;
		FIVector4 midPointVec;

		int w = fbow2->width;
		int h = fbow2->height;
		

		//0:r
		//1:g
		//2:b
		//3:a

		


		int i;
		int j;
		int k;
		int m;
		int totSize = w*h;
		int* btStack = new int[totSize];
		int btStackInd = 0;
		int curInd;
		int curX;
		int curY;
		int destX;
		int destY;
		int lastDir;
		int curXRight;
		int curXLeft;
		int curYUp;
		int curYDown;
		int testX;
		int testY;
		int testInd;
		int basePix;
		int basePix2;
		int testPix;
		int testPix1;
		int testPix2;
		int testPix3;
		int testPix4;
		int count;
		int p1;
		int p2;

		int dirFlags[4];
		int dirFlagsOp[4];
		int dirFlagsO[4];
		int dirFlagsOpO[4];
		int dirModX[4];
		int dirModY[4];
		int opDir[4];
		int dirFlagClear;
		int visFlag = 16;
		int visFlagO = ~16;
		
		// int alleyFlag = 32;
		// int streetFlag = 64;
		// int highwayFlag = 128;
		
		int startDir;
		int curDir;
		int blockMip = intLogB2(singleton->blockSizeInHolders);

		int cx1;
		int cy1;
		int cx2;
		int cy2;

		int histogram[256];

		float delta;
		float bestDelta;
		float nextBestDelta;

		int nextBestInd;
		int bestDir;
		int nextBestDir;

		int bestInd;
		int tempVal;

		int xind;
		int yind;
		int curHeight;
		
		bool isValid;
		bool notFound;
		bool touchesWater;
		bool doBreak;

		float mult;
		float tempDis;

		dirFlagsO[0] = 1;
		dirFlagsO[1] = 2;
		dirFlagsO[2] = 4;
		dirFlagsO[3] = 8;

		dirFlagsOpO[0] = 2;
		dirFlagsOpO[1] = 1;
		dirFlagsOpO[2] = 8;
		dirFlagsOpO[3] = 4;
		
		dirFlags[0] = ~1;
		dirFlags[1] = ~2;
		dirFlags[2] = ~4;
		dirFlags[3] = ~8;

		dirFlagsOp[0] = ~2;
		dirFlagsOp[1] = ~1;
		dirFlagsOp[2] = ~8;
		dirFlagsOp[3] = ~4;

		dirFlagClear = ~15;

		dirModX[0] = 1;
		dirModX[1] = -1;
		dirModX[2] = 0;
		dirModX[3] = 0;

		dirModY[0] = 0;
		dirModY[1] = 0;
		dirModY[2] = 1;
		dirModY[3] = -1;

		opDir[0] = 1;
		opDir[1] = 0;
		opDir[2] = 3;
		opDir[3] = 2;




		for (i = 0; i < 16; i++) {
			singleton->paramArrMap[i*3+0] = fGenRand();
			singleton->paramArrMap[i*3+1] = fGenRand();
			singleton->paramArrMap[i*3+2] = fGenRand();
		}

		for (i = 0; i < 6; i++) {
			singleton->paramArrMap[i*3+0] = i;
		}

		for (i = 0; i < 30; i++) {
			p1 = iGenRand(6);
			p2 = iGenRand(6);

			if (p1 != p2) {
				tempVal = singleton->paramArrMap[p1];
				singleton->paramArrMap[p1] = singleton->paramArrMap[p2];
				singleton->paramArrMap[p2] = tempVal;
			}
		}



		singleton->bindShader("Simplex2D");
		singleton->bindFBO("simplexFBO");
		singleton->setShaderFloat("curTime", fGenRand()*100.0f);//singleton->curTime);
		singleton->drawFSQuad(1.0f);
		singleton->unbindFBO();
		singleton->unbindShader();
		
		singleton->bindShader("TerrainMix");
		singleton->bindFBO("hmFBOLinear");
		singleton->sampleFBO("simplexFBO", 0);
		singleton->setShaderTexture(1,singleton->imageHM0->tid);
		singleton->setShaderTexture(2,singleton->imageHM1->tid);
		singleton->setShaderArrayfVec3("paramArrMap", singleton->paramArrMap, 16 );
		singleton->setShaderFloat("mapSampScale", singleton->mapSampScale);
		singleton->drawFSQuad(1.0f);
		singleton->setShaderTexture(2,0);
		singleton->setShaderTexture(1,0);
		singleton->unsampleFBO("simplexFBO", 0);
		singleton->unbindFBO();
		singleton->unbindShader();

		singleton->copyFBO("hmFBOLinear","hmFBO");

		
		fbow->getPixels(true);
		fbow->setAllPixels(densityChannel,255);
		fbow->setAllPixels(idChannel,0);
		fbow->setAllPixels(blockChannel,0);


		// determine sea level

		for (i = 0; i < 256; i++) {
			histogram[i] = 0;
		}
		for (i = 0; i < totSize; i++) {
			histogram[fbow->getPixelAtIndex(i,hmChannel)]++;
		}

		int totFilled = 0;
		i = 0;
		while (totFilled < (totSize*40)/100 ) {
			totFilled += histogram[i];
			i++;
		}

		seaLevel = i;
		cout << "Sea Level: " << seaLevel << "\n";



		// place cities

		for (i = 1; i < numProvinces; i++) {

			isValid = false;

			do {

				xind = (int)(fGenRand()*fbow->width);
				yind = (int)(fGenRand()*fbow->height);

				tempVec1.setFXYZ(xind,yind,0.0f);

				notFound = false;
				for (j = 1; j < i; j++) {
					tempVec2.setFXYZ(provinceX[j], provinceY[j], 0.0f);
					tempDis = tempVec2.wrapDistance(&tempVec1, w, false);

					if (tempDis < 200.0f) {
						notFound = true;
					}

				}

				if (notFound) {

				}
				else {
					if (fbow->getPixelAtC(xind,yind,idChannel) == 0) {
						curHeight = fbow->getPixelAtC(xind,yind,hmChannel);

						if (
							(curHeight > seaLevel + 10)
						) {
							
							provinceX[i] = xind;
							provinceY[i] = yind;
							fbow->setPixelAtC(xind,yind,idChannel,i);
							fbow->setPixelAtC(xind,yind,densityChannel,0);
							isValid = true;
							
						}
					}
				}


				
			}
			while (!isValid);
			
		}

		// grow provinces

		fbow->cpuToGPU();

		singleton->copyFBO("hmFBO","swapFBO0");
		singleton->bindShader("MapBorderShader");
		mapStep = 0.0f;
		for (i = 0; i < 1024; i++) {
			
			singleton->bindFBO("swapFBO",mapSwapFlag);
			singleton->sampleFBO("swapFBO",0,mapSwapFlag);
			singleton->setShaderFloat("seaLevel", ((float)seaLevel)/255.0 );
			singleton->setShaderFloat("mapStep", mapStep);
			singleton->setShaderFloat("texPitch", w);
			singleton->drawFSQuad(1.0f);
			singleton->unsampleFBO("swapFBO",0,mapSwapFlag);
			singleton->unbindFBO();
			

			mapSwapFlag = 1-mapSwapFlag;
			mapStep += 1.0f;
		}
		singleton->unbindShader();


		singleton->bindShader("MapBorderShader");
		mapStep = 1.0f;
		for (i = 0; i < 256; i++) {
			
			singleton->bindFBO("swapFBO",mapSwapFlag);
			singleton->sampleFBO("swapFBO",0,mapSwapFlag);
			singleton->setShaderFloat("seaLevel", ((float)seaLevel)/255.0 );
			singleton->setShaderFloat("mapStep", -mapStep);
			singleton->setShaderFloat("texPitch", w);
			singleton->drawFSQuad(1.0f);
			singleton->unsampleFBO("swapFBO",0,mapSwapFlag);
			singleton->unbindFBO();
			

			mapSwapFlag = 1-mapSwapFlag;
			mapStep += 1.0f;
		}
		singleton->unbindShader();


		singleton->copyFBO("swapFBO0","hmFBO");
		

		fbow->getPixels();
		fbow->updateMips();


		// find neighboring cities 
		for (i = 0; i < numProvinces*numProvinces; i++) {
			provinceGrid[i] = 0;
		}

		for (k = 0; k < totSize; k++) {
			curInd = k;
			curY = curInd/w;
			curX = curInd-curY*w;

			basePix = fbow->getPixelAtIndex(curInd,idChannel);

			testPix = fbow->getPixelAtIndex(fbow->getIndex(curX+1,curY), idChannel);
			testPix2 = fbow->getPixelAtIndex(fbow->getIndex(curX,curY+1), idChannel);


			if (basePix != 0) {
				if (testPix != 0) {
					if (basePix != testPix) {

						provinceGrid[basePix*numProvinces + testPix] = 1;
						provinceGrid[basePix + testPix*numProvinces] = 1;

					}
				}
				if (testPix2 != 0) {
					if (basePix != testPix2) {
						provinceGrid[basePix*numProvinces + testPix2] = 1;
						provinceGrid[basePix + testPix2*numProvinces] = 1;
					}
				}
			}


		}



		// 1 - x+
		// 2 - x-
		// 4 - y+
		// 8 - y-


		fbow2->getPixels(true);
		fbow2->setAllPixels(btChannel,15);
		fbow2->setAllPixels(stChannel,0);
		fbow2->setAllPixels(pathChannel,0);
		fbow2->setAllPixels(houseChannel,0);

		
		int blockMod = singleton->blockSizeInHolders;
		for (k = 0; k < totSize; k++) {
			curInd = k;
			curY = curInd/w;
			curX = curInd-curY*w;

			basePix = fbow->getMipVal(curX,curY,blockMip,idChannel,MAX_MIP);
			testPix = fbow->getMipVal(curX,curY,blockMip,densityChannel,AVG_MIP);

			testPix1 = fbow->getMipVal(curX,curY,blockMip,idChannel,MAX_MIP,-1,-1,0);
			testPix2 = fbow->getMipVal(curX,curY,blockMip,idChannel,MAX_MIP,-1,1,0);
			testPix3 = fbow->getMipVal(curX,curY,blockMip,idChannel,MAX_MIP,-1,0,-1);
			testPix4 = fbow->getMipVal(curX,curY,blockMip,idChannel,MAX_MIP,-1,0,1);

			

			if (testPix1 != testPix2 || testPix3 != testPix4 || testPix > 120 ) {
				fbow->setPixelAtIndex(curInd,blockChannel,0);
			}
			else {
				fbow->setPixelAtIndex(curInd,blockChannel,basePix);
			}

		}

		fbow->cpuToGPU();
		singleton->copyFBO("hmFBO","hmFBOLinear");
		

		cout << "start add in city roads\n";
		//add in city roads

		for (i = 0; i < numProvinces; i++) {
			// recursive backtrack
			btStack[0] = fbow2->getIndex(provinceX[i],provinceY[i]);
			btStackInd = 0;
			
			while (btStackInd > -1) {

				curInd = btStack[btStackInd];
				curY = curInd/w;
				curX = curInd-curY*w;

				fbow2->orPixelAtIndex(curInd, btChannel, visFlag);

				startDir = 0;//iGenRand(4);
				count = 0;
				notFound = true;
				bestDelta = FLT_MAX;


				testPix2 = fbow->getPixelAtIndex(curInd,blockChannel);

				//testPix2 = fbow->getMipVal(curX,curY,blockMip,densityChannel,AVG_MIP);
				//testPix3 = fbow->getMipVal(curX,curY,blockMip,idChannel,MIN_MIP);
				//testPix4 = fbow->getMipVal(curX,curY,blockMip,idChannel,MAX_MIP);


				if ( testPix2 != 0 ) {
					do {
						curDir = (startDir + count)%4;

						testX = curX + dirModX[curDir];
						testY = curY + dirModY[curDir];
						testInd = fbow2->getIndex(testX,testY);
						testPix = fbow2->getPixelAtIndex(testInd, btChannel);
						testPix3 = fbow->getPixelAtIndex(testInd, blockChannel);

						if ( (testPix & visFlag) == 0 && (testPix3 != 0)) {
							//not visited, proceed
							notFound = false;


							delta = abs(
								fbow->getPixelAtIndex(curInd,hmChannel) -
								fbow->getPixelAtIndex(testInd,hmChannel)
							);

							if (delta < bestDelta) {
								bestDelta = delta;
								bestDir = curDir;
								bestInd = testInd;
							}

						}

						count++;
					}
					while (count < 4); //notFound && 
				}

				if (notFound) {
					btStackInd--;
				}
				else {

					// join the two and remove walls
					fbow2->andPixelAtIndex(curInd, btChannel, dirFlags[bestDir]);
					fbow2->andPixelAtIndex(bestInd, btChannel, dirFlagsOp[bestDir]);
					
					btStackInd++;
					btStack[btStackInd] = bestInd;
				}

			}
		}

		cout << "end add in city roads\n";

		

		// clear visited
		for (k = 0; k < totSize; k++) {
			testPix = fbow2->getPixelAtIndex(k, btChannel);
			if ( (testPix & visFlag) == 0) {
				//not visited
				for (i = 0; i < 4; i++) {
					fbow2->andPixelAtIndex(k, btChannel, dirFlags[i]);
				}
			}
			else {
				//visited
			}

			fbow2->andPixelAtIndex(k, btChannel, visFlagO );
		}
	


		cout << "start link close cities\n";

		// link close cities

		for (i = 1; i < numProvinces-1; i++) {
			for (j = i + 1; j < numProvinces; j++) {

				curInd = i + j*numProvinces;

				if (provinceGrid[curInd] == 1) {
					p1 = i;
					p2 = j;



					tempVec1.setIXYZ(provinceX[p1],provinceY[p1],0);
					tempVec2.setIXYZ(provinceX[p2],provinceY[p2],0);

					tempVec2.wrapDistance(&tempVec1,w);
					tempVec3.copyFrom(&tempVec1);

					findBestPath(
						tempVec2.getFX(),
						tempVec2.getFY(),
						tempVec3.getFX(),
						tempVec3.getFY(),
						0,
						curInd,
						true,
						false
					);

				}

			}
		}



		cout << "end link close cities\n";



		floatAndIndex* oceanRes = new floatAndIndex[numProvinces*numProvinces];

		for (i = 0; i < numProvinces*numProvinces; i++) {
			oceanRes[i].value = FLT_MAX;
			oceanRes[i].index1 = 0;
			oceanRes[i].index2 = 0;
		}

		cout << "start find biggest ocean gaps\n";


		for (k = 0; k < 2; k++) {

			cout << "iteration: " << k << "\n";

			count = 0;

			if (k == 0) {
				for (i = 1; i < numProvinces-1; i++) {
					for (j = i + 1; j < numProvinces; j++) {
						curInd = i + j*numProvinces;
						if (provinceGrid[curInd] != 1) {
							p1 = i;
							p2 = j;



							tempVec1.setIXYZ(provinceX[p1],provinceY[p1],0);
							tempVec2.setIXYZ(provinceX[p2],provinceY[p2],0);

							tempVec2.wrapDistance(&tempVec1,w);
							tempVec3.copyFrom(&tempVec1);


							oceanRes[count].value = findBestPath(
								tempVec2.getFX(),
								tempVec2.getFY(),
								tempVec3.getFX(),
								tempVec3.getFY(),
								0,
								curInd,
								false,
								true
							);
							oceanRes[count].index1 = i;
							oceanRes[count].index2 = j;

							count++;

						}

					}
				}
			}
			else {
				bubbleSortF(oceanRes,numProvinces*numProvinces);


				for (i = 0; i < 30; i++) {
					
					p1 = oceanRes[i].index1;
					p2 = oceanRes[i].index2;

					curInd = p1 + p2*numProvinces;

					tempVec1.setIXYZ(provinceX[p1],provinceY[p1],0);
					tempVec2.setIXYZ(provinceX[p2],provinceY[p2],0);

					tempVec2.wrapDistance(&tempVec1,w);
					tempVec3.copyFrom(&tempVec1);

					findBestPath(
						tempVec2.getFX(),
						tempVec2.getFY(),
						tempVec3.getFX(),
						tempVec3.getFY(),
						0,
						curInd,
						true,
						true
					);
				}

			}

			
		}

		

		cout << "end find biggest ocean gaps\n";


		

		



		mapSwapFlag = 0;
		mapStep = 0.0f;

		fbow2->cpuToGPU();
		singleton->copyFBO("cityFBO","swapFBO0");
		singleton->bindShader("DilateShader");
		for (i = 0; i < 2; i++) {
			
			singleton->bindFBO("swapFBO",mapSwapFlag);
			singleton->sampleFBO("swapFBO",0,mapSwapFlag);
			singleton->sampleFBO("hmFBO",1);
			singleton->setShaderFloat("seaLevel", ((float)seaLevel)/255.0 );
			singleton->setShaderFloat("mapStep", 1.0);
			singleton->setShaderFloat("doDilate", 1.0);
			singleton->setShaderFloat("texPitch", w);
			singleton->drawFSQuad(1.0f);
			singleton->unsampleFBO("hmFBO",1);
			singleton->unsampleFBO("swapFBO",0,mapSwapFlag);
			singleton->unbindFBO();

			mapSwapFlag = 1-mapSwapFlag;
			mapStep += 1.0f;
		}
		singleton->unbindShader();
		singleton->copyFBO("swapFBO0","cityFBO");
		fbow2->getPixels();
		//fbow2->updateMips();






		//bool notCovered = true;
		int id = 1;
		int totCount;
		int fillColor;
		bool incId;

		cout << "start road regions\n";

		for (i = 0; i < totSize; i++) {

			if (fbow2->getPixelAtIndex(i,pathChannel) == 0) {



				for (j = 0; j < 2; j++) {
					btStack[0] = i;
					btStackInd = 0;
					totCount = 0;

					if (j == 0) {
						fillColor = id;
					}
					else {
						fillColor = 255;
					}


					while (btStackInd > -1) {

						curInd = btStack[btStackInd];
						curY = curInd/w;
						curX = curInd-curY*w;

						if (j == 0) {
							fbow2->orPixelAtIndex(curInd, btChannel, visFlag);
						}
						else {
							fbow2->andPixelAtIndex(curInd, btChannel, visFlagO );
						}
						

						fbow2->setPixelAtIndex(curInd, pathChannel, fillColor );
						

						count = 0;
						notFound = true;


						
						do {
							curDir = count;

							testX = curX + dirModX[curDir];
							testY = curY + dirModY[curDir];
							testInd = fbow2->getIndex(testX,testY);
							testPix = fbow2->getPixelAtIndex(testInd, btChannel);
							testPix2 = fbow2->getPixelAtIndex(testInd, pathChannel);


							if (j == 0) {
								if ( ( (testPix & visFlag) == 0 ) && (testPix2 == 0)) {
									notFound = false;
									totCount++;
									
								}
							}
							else {
								if ( ( (testPix & visFlag) > 0) && (testPix2 == id)) {
									notFound = false;
									totCount++;
								}
							}


							

							count++;
						}
						while (notFound && count < 4);
						
						if (notFound) {
							btStackInd--;
						}
						else {

							// join the two and remove walls
							//fbow2->andPixelAtIndex(curInd, btChannel, dirFlags[bestDir]);
							//fbow2->andPixelAtIndex(bestInd, btChannel, dirFlagsOp[bestDir]);
							

							btStackInd++;
							btStack[btStackInd] = testInd;
						}

					}

					incId = false;

					if (j == 0) {
						if (totCount < 1000) {
							//cout << "Too Small\n";
						}
						else {
							incId = true;
							j++;
						}
					}
					else {
						incId = true;
					}

					if (incId) {
						//cout << "ID: " << id << "\n";
						id++;
						if (id > 254) {
							id = 1;
						}
					}
					

					
				}
				



			}

		}


		cout << "end road regions\n";

		// clear visited
		for (k = 0; k < totSize; k++) {
			fbow2->andPixelAtIndex(k, btChannel, visFlagO );
		}

		fbow2->cpuToGPU();


		mapSwapFlag = 0;
		mapStep = 0.0f;
		singleton->copyFBO("cityFBO","swapFBO0");
		


		singleton->bindShader("SkeletonShader");
		for (k = 0; k < 19; k++) {
			
			singleton->bindFBO("swapFBO",mapSwapFlag);
			singleton->sampleFBO("swapFBO",0,mapSwapFlag);
			//singleton->sampleFBO("hmFBO",1);
			singleton->setShaderFloat("seaLevel", ((float)seaLevel)/255.0 );
			singleton->setShaderFloat("mapStep", 0.0);
			singleton->setShaderFloat("texPitch", w);
			singleton->drawFSQuad(1.0f);
			//singleton->unsampleFBO("hmFBO",1);
			singleton->unsampleFBO("swapFBO",0,mapSwapFlag);
			singleton->unbindFBO();
			mapSwapFlag = 1-mapSwapFlag;
			mapStep += 1.0f;
			
		}
		singleton->unbindShader();


		singleton->bindShader("RoadShader");
		for (k = 0; k < 1; k++) {
			
			singleton->bindFBO("swapFBO",mapSwapFlag);
			singleton->sampleFBO("swapFBO",0,mapSwapFlag);
			//singleton->sampleFBO("hmFBO",1);
			singleton->setShaderFloat("seaLevel", ((float)seaLevel)/255.0 );
			singleton->setShaderFloat("mapStep", 0.0);
			singleton->setShaderFloat("texPitch", w);
			singleton->drawFSQuad(1.0f);
			//singleton->unsampleFBO("hmFBO",1);
			singleton->unsampleFBO("swapFBO",0,mapSwapFlag);
			singleton->unbindFBO();
			mapSwapFlag = 1-mapSwapFlag;
			mapStep += 1.0f;
			
		}
		singleton->unbindShader();


		singleton->copyFBO("swapFBO0","cityFBO");
		fbow2->getPixels();







		// generate streets

		int* streetFlagsV = new int[w]; //runs vertical
		int* streetFlagsH = new int[h]; //runs horizontal

		for (i = 0; i < w; i++) {
			streetFlagsV[i] = 0;
		}
		for (i = 0; i < h; i++) {
			streetFlagsH[i] = 0;
		}

		// 1 - x+
		// 2 - x-
		// 4 - y+
		// 8 - y-

		for (i = 0; i < w; i+= singleton->blockSizeInHolders ) {

			curInd = i-1;

			while (curInd<0) {
				curInd+=w;
			}

			streetFlagsV[curInd] |= 1;
			streetFlagsV[i] |= 2;
		}
		for (i = 0; i < h; i+= singleton->blockSizeInHolders ) {

			curInd = i-1;

			while (curInd<0) {
				curInd+=h;
			}

			streetFlagsH[curInd] |= 4;
			streetFlagsH[i] |= 8;
		}


		


		



		

		
		//add in main streets
		for (k = 0; k < totSize; k++) {
			curInd = k;
			curY = curInd/w;
			curX = curInd-curY*w;

			testPix = fbow->getPixelAtIndex(curInd,blockChannel);

			if (testPix != 0) {
				fbow2->orPixelAtIndex(curInd, stChannel, streetFlagsH[curY]|streetFlagsV[curX]);
			}


			

		}

		/*
		for (i = 0; i < numProvinces-1; i++) {
			for (j = i + 1; j < numProvinces; j++) {
				if (provinceGrid[i + j*numProvinces] == 1) {

					k = fbow->numMips-1;

					cx1 = provinceX[i];
					cy1 = provinceY[i];
					cx2 = provinceX[j];
					cy2 = provinceY[j];

					while (getMipInd(cx1,cy1,k) == getMipInd(cx2,cy2,k)) {
						k--;
					}

				}
			}
		}



		// remove any road that touches water or is out of town

		int cityLevel = 0;
		for (k = 0; k < totSize; k++) {
			curInd = k;
			curY = curInd/w;
			curX = curInd-curY*w;

			touchesWater = false;

			for (j = -1; j <= 1; j++) {				
				for (i = -1; i <= 1; i++) {
					testX = curX + i;
					testY = curY + j;
					testInd = fbow->getIndex(testX,testY);
					testPix = fbow->getPixelAtIndex(testInd, hmChannel);
					

					if (testPix < seaLevel + 10) {
						touchesWater = true;
						break;
					}

				}
				if (touchesWater) {
					break;
				}
			}

			testPix2 = fbow->getPixelAtIndex(curInd, densityChannel);


			if (touchesWater) { // || (testPix2 > 120) 
				fbow2->andPixelAtIndex(curInd, btChannel, dirFlagClear);
				fbow2->andPixelAtIndex(curInd, stChannel, dirFlagClear);
				
				//TODO: repair all broken (half) edges
			}
			// if (testPix2 > 80) {
			// 	fbow2->andPixelAtIndex(curInd, stChannel, dirFlagClear);
			// }
			

		}



		// fix half streets

		for (k = 0; k < totSize; k++) {
			curInd = k;
			curY = curInd/w;
			curX = curInd-curY*w;

			basePix = fbow2->getPixelAtIndex(curInd, btChannel);
			basePix2 = fbow2->getPixelAtIndex(curInd, stChannel);

			for (i = 0; i < 4; i++) {
				
				
				
				curDir = i;

				testX = curX + dirModX[curDir];
				testY = curY + dirModY[curDir];
				testInd = fbow2->getIndex(testX,testY);
				testPix = fbow2->getPixelAtIndex(testInd, btChannel);
				testPix2 = fbow2->getPixelAtIndex(testInd, stChannel);


				if ( (basePix & dirFlagsO[curDir]) != (testPix & dirFlagsOpO[curDir]) ) {
					fbow2->orPixelAtIndex(curInd, btChannel, dirFlagsO[curDir]);
					fbow2->orPixelAtIndex(testInd, btChannel, dirFlagsOpO[curDir]);
				}

				if ( (basePix2 & dirFlagsO[curDir]) != (testPix2 & dirFlagsOpO[curDir]) ) {
					fbow2->orPixelAtIndex(curInd, stChannel, dirFlagsO[curDir]);
					fbow2->orPixelAtIndex(testInd, stChannel, dirFlagsOpO[curDir]);
				}

			}

		}

*/
		

















		// ?????


		

		cout << "start filling houses\n";

		fbow->updateMips();

		int wb = worldSizeInBlocks.getIX();
		int hb = worldSizeInBlocks.getIY();
		int blockInd;

		for (i = 0; i < wb; i++) {
			for (j = 0; j < hb; j++) {
				blockInd = i + j*wb;

				testPix = fbow->getMipAtIndex(blockInd, blockMip, blockChannel, MIN_MIP);

				if (testPix != 0) {



					id = 1;

					for (k = i*blockMod; k < (i+1)*blockMod; k++) {
						for (m = j*blockMod; m < (j+1)*blockMod; m++) {
							curInd = k + m*w;
							curX = k;
							curY = m;

							
							testPix = fbow2->getPixelAtIndex(curInd, btChannel);

							if (testPix & visFlag) {

							}
							else {
								btStack[0] = curInd;
								btStackInd = 0;
								totCount = 0;


								while ( (btStackInd > -1) && (totCount < 4) ) {

									curInd = btStack[btStackInd];
									curY = curInd/w;
									curX = curInd-curY*w;

									fbow2->orPixelAtIndex(curInd, btChannel, visFlag);
									fbow2->setPixelAtIndex(curInd, houseChannel, id );
									

									count = 0;
									notFound = true;

									testPix2 = fbow2->getPixelAtIndex(curInd, btChannel);
									testPix3 = fbow2->getPixelAtIndex(curInd, stChannel);
									testPix4 = fbow2->getPixelAtIndex(curInd, pathChannel);
									
									do {
										curDir = count;

										testX = curX + dirModX[curDir];
										testY = curY + dirModY[curDir];

										testInd = fbow2->getIndex(testX,testY);

										testPix = fbow2->getPixelAtIndex(testInd, btChannel);

										if (
											( (testPix & visFlag) == 0 ) &&
											( (testPix2 & dirFlagsO[curDir]) == 0) &&
											( (testPix3 & dirFlagsO[curDir]) == 0) &&
											( (testPix4 & dirFlagsO[curDir]) == 0)
										) {
											notFound = false;
											totCount++;
											
										}
										

										count++;
									}
									while (notFound && count < 4);
									
									if (notFound) {
										btStackInd--;
									}
									else {

										// join the two and remove walls
										//fbow2->andPixelAtIndex(curInd, btChannel, dirFlags[bestDir]);
										//fbow2->andPixelAtIndex(bestInd, btChannel, dirFlagsOp[bestDir]);
										

										btStackInd++;
										btStack[btStackInd] = testInd;
									}

								}

								id++;
								if (id > 254) {
									id = 1;
								}




							}

							
						}
					}

				}

			}
		}


		cout << "end filling houses\n";

		// clear visited
		for (k = 0; k < totSize; k++) {
			fbow2->andPixelAtIndex(k, btChannel, visFlagO );
		}



		// ?????










		fbow2->cpuToGPU();

		delete[] btStack;
		delete[] streetFlagsH;
		delete[] streetFlagsV;
		delete[] oceanRes;

		//////////

		
		
		singleton->mapInvalid = false;

		singleton->setCameraToElevation();

		cout << "DONE WITH MAP\n";

		mapLockOn = false;

		popTrace();
	}

	void drawMap() {


		pushTrace("drawMap()");

		FBOWrapper* fbow = singleton->getFBOWrapper("hmFBOLinear", 0);

		


		singleton->bindShader("TopoShader");
		singleton->sampleFBO("palFBO", 0);
		singleton->sampleFBO("hmFBO",1); //Linear
		singleton->sampleFBO("cityFBO",2);

		singleton->setShaderFloat("mapTrans", mapTrans);
		singleton->setShaderFloat("seaLevel", ((float)seaLevel)/255.0 );
		singleton->setShaderFloat("curTime", singleton->curTime);
		singleton->setShaderFloat("cameraZoom", singleton->cameraZoom);
		singleton->setShaderfVec3("cameraPos", &(singleton->cameraPos));
		singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf));
		singleton->setShaderVec2("mapDimInPixels", fbow->width, fbow->height);
		singleton->setShaderfVec3("maxBoundsInPixels", &(singleton->maxBoundsInPixels) );

		singleton->drawQuadBounds(
			-singleton->maxBoundsInPixels.getFX()/2.0f,
			-singleton->maxBoundsInPixels.getFY()/2.0f,
			singleton->maxBoundsInPixels.getFX()/2.0f,
			singleton->maxBoundsInPixels.getFY()/2.0f
		);
		
		singleton->unsampleFBO("hmFBO",2);
		singleton->unsampleFBO("hmFBOLinear",1);
		singleton->unsampleFBO("palFBO",0);
		singleton->unbindShader();



		popTrace();
	}



	void postProcess() {


		pushTrace("postProcess()");

		float newZoom;

		// NOTE: ALWAYS UNSAMPLE IN REVERSE ORDER!!!

		singleton->worldToScreen(&lScreenCoords, &(singleton->lightPos));
		singleton->worldToScreen(&aoScreenCoords, &(singleton->activeObjectPos));

		//singleton->drawFBO("palFBO", 0, 1.0 );

		

		if ( mapTrans < 1.0 ) {
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

			singleton->setShaderFloat("visPageSizeInPixels", (float)(singleton->visPageSizeInPixels));
			singleton->setShaderFloat("holderSizeInPixels", (float)(singleton->holderSizeInPixels));
			singleton->setShaderFloat("unitSizeInPixels", (float)(singleton->unitSizeInPixels));

			singleton->setShaderFloat("gridOn",singleton->gridOn);
			singleton->setShaderFloat("heightmapMax",singleton->heightmapMax);
			singleton->setShaderFloat("cameraZoom",singleton->cameraZoom);
			singleton->setShaderfVec2("bufferDim", &(singleton->bufferDimHalf));
			singleton->setShaderFloat("diskOn",singleton->diskOn);
			singleton->setShaderFloat("curTime", singleton->curTime);

			
			singleton->bindFBO("resultFBO");
			singleton->sampleFBO("combineFBO",0);
			singleton->sampleFBO("geomFBO", 2);
			singleton->sampleFBO("palFBO", 4);

			//MUST BE CALLED AFTER FBO IS BOUND
			singleton->setShaderVec2("resolution",singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);

			singleton->drawFSQuad(1.0f);
			singleton->unsampleFBO("palFBO", 4);
			singleton->unsampleFBO("geomFBO", 2);
			singleton->unsampleFBO("combineFBO",0);
			singleton->unbindFBO();
			singleton->unbindShader();

			newZoom = std::max(1.0f,singleton->cameraZoom);
			
			singleton->drawFBO("resultFBO", 0, newZoom );
		}

		
		

		
		if ( mapTrans > 0.0 ) {
			glEnable(GL_BLEND);
			drawMap();
			glDisable(GL_BLEND);
		}
		
		

		popTrace();
		
		
	}

	~GameWorld() {


		threadpool.stopAll();

		int i;
		int j;

		GamePage* curPage;

		for (i = 0; i < iHolderSize; i++) {
			if (holderData[i]) {

				for (j = 0; j < singleton->holderSizeInPages; j++) {
					curPage = holderData[i]->pageData[j];

					if (curPage) {

						delete curPage;

						
					}
				}

				
			}
		}
		

		
	}


};