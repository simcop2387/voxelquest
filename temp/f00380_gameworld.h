// f00380_gameworld.h
//

#include "f00380_gameworld.e"
#define LZZ_INLINE inline
GameWorld::GameWorld ()
        {

		// Notes
		// -----
		// Depth: 16 bits
		// Material: 16 bits
		// AO val: 8 bits, normal: 24 bits

	}
void GameWorld::init (Singleton * _singleton)
                                         {

		pushTrace("GameWorld init()");


		int i;
		int j;

		mapSwapFlag = 0;
		mapStep = 0.0f;

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
		worldSizeInHolders.copyFrom( &(singleton->worldSizeInHolders) );

		visPageSizeInPixels = singleton->visPageSizeInPixels;

	    diagrams[E_RENDER_VOL][E_STATE_INIT_END] = E_STATE_CREATESIMPLEXNOISE_LAUNCH;
		diagrams[E_RENDER_VOL][E_STATE_CREATESIMPLEXNOISE_END] = E_STATE_GENERATEVOLUME_LAUNCH;//E_STATE_COPYTOTEXTURE_LAUNCH;
	    //diagrams[E_RENDER_VOL][E_STATE_COPYTOTEXTURE_END] = E_STATE_GENERATEVOLUME_LAUNCH;
	    diagrams[E_RENDER_VOL][E_STATE_GENERATEVOLUME_END] = E_STATE_LENGTH;

		curDiagram = diagrams[renderMethod];

		holderSizeInPages = singleton->holderSizeInPages;
		visPageSizeInUnits = singleton->visPageSizeInUnits;

		iVolumeSize = worldSizeInPages.getIX()*worldSizeInPages.getIY()*worldSizeInPages.getIZ();
		iHolderSize = worldSizeInHolders.getIX()*worldSizeInHolders.getIY()*worldSizeInHolders.getIZ();


		holderData = new GamePageHolder*[iHolderSize];
		for (i = 0; i < iHolderSize; i++) {
			holderData[i] = NULL;
		}
	    


	    popTrace();
	}
int GameWorld::wrapCoord (int val, int mv)
                                       {
		while (val < 0) {
			val += mv;
		}
		while (val >= mv) {
			val -= mv;
		}

		//val = val % max;

		return val;
	}
GamePageHolder * GameWorld::getHolderAtCoords (int x, int y, int z, bool createOnNull)
                                                                                          {
		

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
GamePageHolder * GameWorld::getHolderAtIndex (int ind)
                                                  {
		

		return holderData[ind];

	}
GamePage * GameWorld::getPageAtIndex (int ind)
                                          {
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
GamePage * GameWorld::getPageAtCoords (int x, int y, int z, bool createOnNull)
                                                                                  {
		//pushTrace("getPageAtCoords()");
		int hx, hy, hz;
		int px, py, pz;
		int gpInd;
		int xmod = 0;
		int ymod = 0;
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

		


		if (x < 0) {
			if (x%holderSizeInPages == 0) {

			}
			else {
				xmod = -1;
			}
			
		}
		if (y < 0) {
			if (y%holderSizeInPages == 0) {

			}
			else {
				ymod = -1;
			}
		}
		


		GamePageHolder* gph = getHolderAtCoords(
			(x)/holderSizeInPages + xmod,
			(y)/holderSizeInPages + ymod,
			z/holderSizeInPages,
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
bool GameWorld::checkBounds (int k)
                                { //int i, int j, int k) {
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
void GameWorld::resetToState (E_STATES resState)
                                             {
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
void GameWorld::update2 ()
                       {
		pushTrace("update2()");

		glClearColor(0.6,0.6,0.7,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		if (singleton->mapInvalid) {
			initMap();
		}

		
		if (singleton->showMap) {
			drawMap();
		}
		*/
		

		//singleton->drawFBO("simplexFBO", 0, 1.0);

		/*
		glBindTexture(GL_TEXTURE_2D, singleton->lookup2to3ID);
		singleton->drawFSQuadOffset(0.0,0.0,1.0);
		glBindTexture(GL_TEXTURE_2D, 0);
		*/
		glutSwapBuffers();
		glFlush();


		popTrace();
	}
void GameWorld::addGeom (GameGeom * geom)
                                     {
		
		int i;
		int j;
		int k;
		int ind;
		int bufSize = (visPageSizeInPixels*singleton->bufferMult);
		GamePageHolder* gph;
		FIVector4 start;
		FIVector4 end;

		start.copyFrom( &(geom->boundsMinInPixels) );
		end.copyFrom( &(geom->boundsMaxInPixels) );

		start.addXYZ(-bufSize);
		end.addXYZ(bufSize);

		start.intDivXYZ(singleton->holderSizeInPixels);
		end.intDivXYZ(singleton->holderSizeInPixels);

		start.clampZ(&(singleton->origin),&(singleton->worldSizeInHoldersM1));
		end.clampZ(&(singleton->origin),&(singleton->worldSizeInHoldersM1));


		for (k = start.getIZ(); k <= end.getIZ(); k++ ) {
			for (j = start.getIY(); j <= end.getIY(); j++ ) {
				for (i = start.getIX(); i <= end.getIX(); i++ ) {
					gph = getHolderAtCoords(i,j,k,true);
					gph->containsGeomIds.push_back(geom->id);
				}
			}
		}

		

	}
void GameWorld::update ()
                      {

		pushTrace("update()");

		int i;

		float x;
		float y;
		float z;

		doTraceVecND("MAX BOUNDS IN PIX ",&singleton->maxBoundsInPixels);


		if (singleton->mapInvalid) {
			initMap();

			for (i = 0; i < 512; i++) {
				gameGeom.push_back(new GameGeom());
				
				x = fGenRand()*8192.0f*8.0f;
				y = fGenRand()*8192.0f*8.0f;
				z = singleton->getHeightAtPixelPos(x,y);

				cout << "x: " << x << " y: " << y << " z: " << z << "\n";

				gameGeom.back()->initRand(i,x,y,z);


				//addGeom(gameGeom.back());
			}


			popTrace();
			return;
		}


		doTraceND("yo");

		bool changesMade = singleton->changesMade;
		bool bufferInvalid = singleton->bufferInvalid;

		bool procResult;
		bool doRenderGeom = true;


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
bool GameWorld::processPages ()
                            {

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

		
	    GamePage* curPage;

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
void GameWorld::renderPages ()
                           {

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

	    			//if (gp->isRendering) {

	    			//}
	    			//else {
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
void GameWorld::drawHolder (GamePageHolder * gp)
                                            {
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
void GameWorld::combineBuffers ()
                              {
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
void GameWorld::renderGeom ()
                          {
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
		

		glDisable(GL_DEPTH_TEST);

		

		//singleton->unsampleFBO("pagesFBO");
		singleton->unbindFBO();
		singleton->unbindShader();

		//glDisable(GL_DEPTH_TEST);

		popTrace();
		
	}
void GameWorld::modifyUnit (FIVector4 * fPixelWorldCoordsBase, E_BRUSH brushAction)
                                                                               {

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
								
								curPage->createSimplexNoise();

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
															

															curPage->isDirty = true;
															changes = true;
														}
														else {
															if (curPage->isDirty) {
																curPage->isDirty = false;
																//curPage->copyToTexture();
																curPage->generateVolume();
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
void GameWorld::renderWorldSpace ()
                                {


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
void GameWorld::renderGrass ()
                           {


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
void GameWorld::initMap ()
                       {


		pushTrace("initMap()");

		mapSwapFlag = 0;
		
		mapStep = 0.0f;

		int i;
		int j;
		int k;

		for (i = 0; i < 16; i++) {
			singleton->paramArrMap[i*3+0] = fGenRand();
			singleton->paramArrMap[i*3+1] = fGenRand();
			singleton->paramArrMap[i*3+2] = fGenRand();
		}



		singleton->bindShader("Simplex2D");
		singleton->bindFBO("simplexFBO");
		singleton->setShaderFloat("curTime", singleton->paramArrMap[0]*100.0f);//singleton->curTime);
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

		//should be
		//0:r
		//1:g
		//2:b
		//3:a

		//not this
		//0 - b
		//1 - g
		//2 - r
		//3 - a

		
		int stChannel = 0;
		int btChannel = 1;
		
		int hmChannel = 0;
		int idChannel = 1;
		int densityChannel = 2;

		FBOWrapper* fbow = singleton->getFBOWrapper("hmFBO",0);
		fbow->getPixels();
		fbow->setAllPixels(densityChannel,255);
		fbow->setAllPixels(idChannel,0);


		


		//////////
		
		bool isValid;
		int xind;
		int yind;



		for (i = 0; i < singleton->numProvinces; i++) {

			isValid = false;

			do {

				xind = (int)(fGenRand()*fbow->width);
				yind = (int)(fGenRand()*fbow->height);

				if (fbow->getPixelAtC(xind,yind,hmChannel) > singleton->seaLevel ) {
					if (fbow->getPixelAtC(xind,yind,idChannel) == 0) {
						fbow->setPixelAtC(xind,yind,idChannel,i+1);
						fbow->setPixelAtC(xind,yind,densityChannel,0);
						isValid = true;
					}
					else {
						isValid = false;
					}
				}
				else {
					isValid = false;
				}
			}
			while (!isValid);
			
		}



		// 1 - x+
		// 2 - x-
		// 4 - y+
		// 8 - y-

		FBOWrapper* fbow2 = singleton->getFBOWrapper("cityFBO",0);
		fbow2->getPixels();
		fbow2->setAllPixels(btChannel,15);
		fbow2->setAllPixels(stChannel,0);


		int totSize = fbow2->width*fbow2->height;
		int* btStack = new int[totSize];
		int btStackInd = 0;
		int curInd;
		int curX;
		int curY;
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
		int testPix2;
		int count;

		int dirFlags[4];
		int dirFlagsOp[4];
		int dirFlagsO[4];
		int dirFlagsOpO[4];
		int dirModX[4];
		int dirModY[4];
		int dirFlagClear;
		int visFlag = 16;
		int startDir;
		int curDir;

		
		int w = fbow2->width;
		int h = fbow2->height;
		
		bool notFound;
		bool touchesWater;

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
		

		btStack[0] = fbow2->getIndex(iGenRand(w), iGenRand(h) );

		
		while (btStackInd > -1) {

			curInd = btStack[btStackInd];
			curY = curInd/w;
			curX = curInd-curY*w;

			fbow2->orPixelAtIndex(curInd, btChannel, visFlag);

			startDir = iGenRand(4);
			count = 0;
			notFound = true;

			do {
				curDir = (startDir + count)%4;

				testX = curX + dirModX[curDir];
				testY = curY + dirModY[curDir];
				testInd = fbow2->getIndex(testX,testY);

				testPix = fbow2->getPixelAtIndex(testInd, btChannel);


				if ( (testPix & visFlag) == 0) {
					//not visited, proceed
					notFound = false;
				}

				count++;
			}
			while (notFound && count < 4);

			if (notFound) {
				btStackInd--;
			}
			else {

				// join the two and remove walls
				fbow2->andPixelAtIndex(curInd, btChannel, dirFlags[curDir]);
				fbow2->andPixelAtIndex(testInd, btChannel, dirFlagsOp[curDir]);
				
				btStackInd++;
				btStack[btStackInd] = testInd;
			}

		}

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

		for (i = 0; i < w; i+= (iGenRand(6)+4) ) {
			streetFlagsV[i] |= 1;
			streetFlagsV[ (i+1)%w ] |= 2;
		}
		for (i = 0; i < h; i+= (iGenRand(6)+4) ) {
			streetFlagsH[i] |= 4;
			streetFlagsH[ (i+1)%h ] |= 8;
		}


		


		fbow->cpuToGPU();




		singleton->copyFBO("hmFBO","swapFBO0");
		singleton->bindShader("MapBorderShader");
		for (i = 0; i < 1024; i++) {
			
			singleton->bindFBO("swapFBO",mapSwapFlag);
			//singleton->sampleFBO("palFBO", 0);
			singleton->sampleFBO("swapFBO",0,mapSwapFlag);
			//singleton->setShaderFloat("curTime", mapStep);//singleton->curTime);
			singleton->setShaderFloat("mapStep", mapStep);
			singleton->drawFSQuad(1.0f);
			singleton->unsampleFBO("swapFBO",0,mapSwapFlag);
			//singleton->unsampleFBO("palFBO",0);
			singleton->unbindFBO();
			

			mapSwapFlag = 1-mapSwapFlag;
			mapStep += 1.0f;
		}
		singleton->unbindShader();
		singleton->copyFBO("swapFBO0","hmFBO");
		singleton->copyFBO("hmFBO","hmFBOLinear");

		fbow->getPixels();


		for (k = 0; k < totSize; k++) {
			curInd = k;
			curY = curInd/w;
			curX = curInd-curY*w;

			fbow2->orPixelAtIndex(curInd, stChannel, streetFlagsH[curY]|streetFlagsV[curX]);

		}





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
					

					if (testPix < singleton->seaLevel + 10) {
						touchesWater = true;
						break;
					}

				}
				if (touchesWater) {
					break;
				}
			}

			testPix2 = fbow->getPixelAtIndex(curInd, densityChannel);


			if (touchesWater || (testPix2 > 120) ) {
				fbow2->andPixelAtIndex(curInd, btChannel, dirFlagClear);
				fbow2->andPixelAtIndex(curInd, stChannel, dirFlagClear);
				
				//TODO: repair all broken (half) edges
			}
			if (testPix2 > 80) {
				fbow2->andPixelAtIndex(curInd, stChannel, dirFlagClear);
			}
			

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


		




		fbow2->cpuToGPU();

		delete[] btStack;
		delete[] streetFlagsH;
		delete[] streetFlagsV;
		

		//////////

		
		
		singleton->mapInvalid = false;

		singleton->setCameraToElevation();

		popTrace();
	}
void GameWorld::drawMap ()
                       {


		pushTrace("drawMap()");

		FBOWrapper* fbow = singleton->getFBOWrapper("hmFBOLinear", 0);


		singleton->bindShader("TopoShader");
		singleton->sampleFBO("palFBO", 0);
		singleton->sampleFBO("hmFBOLinear",1);
		singleton->sampleFBO("cityFBO",2);

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
void GameWorld::postProcess ()
                           {


		pushTrace("postProcess()");

		float newZoom;

		// NOTE: ALWAYS UNSAMPLE IN REVERSE ORDER!!!

		singleton->worldToScreen(&lScreenCoords, &(singleton->lightPos));
		singleton->worldToScreen(&aoScreenCoords, &(singleton->activeObjectPos));

		//singleton->drawFBO("palFBO", 0, 1.0 );

		



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
		

		
		if (singleton->showMap) {
			glEnable(GL_BLEND);
			drawMap();
			glDisable(GL_BLEND);
		}
		
		

		popTrace();
		
		
	}
GameWorld::~ GameWorld ()
                     {


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
#undef LZZ_INLINE
 
