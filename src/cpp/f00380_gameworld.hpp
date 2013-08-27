
class GameWorld {
public:


	iVector3 iDim;
	int iPageSize;
	int iVolumeSize;
	int loadRad;
	int loadRadZ;
	int renderRad;
	float unitScale;
	int diagrams[E_RENDER_LENGTH][E_STATE_LENGTH];
	int* curDiagram;
	int renderMethod;
	bool doDrawFBO;
	iVector3 minWithinLR;
	iVector3 maxWithinLR;
	Singleton* singleton;
	GamePage** worldData;
	GamePage** texData;
	int iBufferSize;
	iVector3 origin;
	iVector3 cameraPos;
	iVector3 playerPos;
	iVector2 iScreenDim;
	fVector2 fScreenDim;
	Poco::ThreadPool threadpool;


	GameWorld()
	{

		// Notes
		// -----
		// Depth: 16 bits
		// Material: 16 bits
		// AO val: 8 bits, normal: 24 bits

	}

	int checkBounds(int i, int j, int k) {
		
		int res = 1;

		if (i < 0) {res = 0;}
		if (j < 0) {res = 0;}
		if (k < 0) {res = 0;}
		if (i >= iDim.x) {res = 0;}
		if (j >= iDim.y) {res = 0;}
		if (k >= iDim.z) {res = 0;}

		return res;
	}


	void resetToState(E_STATES resState) {
		int i;
		for (i = 0; i < iVolumeSize; i++) {
			if (worldData[i]) {
				if( worldData[i]->curState > resState) {
					worldData[i]->curState = resState;
				}
			}
		}
	}

	void init(iVector3 _iDim, Singleton* _singleton, int _renderMethod) {
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

		renderMethod = _renderMethod;
		singleton = _singleton;
		iDim = _iDim;

	    diagrams[E_RENDER_VOL][E_STATE_INIT_END] = E_STATE_CREATESIMPLEXNOISE_LAUNCH;
		diagrams[E_RENDER_VOL][E_STATE_CREATESIMPLEXNOISE_END] = E_STATE_COPYTOTEXTURE_LAUNCH;
	    diagrams[E_RENDER_VOL][E_STATE_COPYTOTEXTURE_END] = E_STATE_LENGTH;

		curDiagram = diagrams[renderMethod];

		iPageSize = 4;
		unitScale = 0.25f;

		loadRad = 4;
		loadRadZ = 4;
	    renderRad = loadRad-1;
		
		iVolumeSize = iDim.x*iDim.y*iDim.z;
		
	    worldData = new GamePage*[iVolumeSize];
	    
	    
		

	    cameraPos.x = loadRad*iPageSize;
	    cameraPos.y = loadRad*iPageSize;
	    cameraPos.z = loadRadZ*iPageSize;
	    
	    playerPos = cameraPos;


		
		for (i = 0; i < iVolumeSize; i++) {
			worldData[i] = NULL;
		}


		origin.x = 0; origin.y = 0; origin.z = 0;

	    
	    #ifdef DEBUG_MODE
	    popTrace();
	    #endif
	}

	void update(bool changesMade, bool bufferInvalid, int maxH) {

		bool procResult = processPages();
		if (procResult || changesMade) {
			renderPages(singleton->cameraZoom, maxH);
		}

		if (procResult || changesMade || bufferInvalid) {

			glClearColor(0.6,0.6,0.7,0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			postProcess(singleton->cameraZoom);

			glutSwapBuffers();
			glFlush();
			
		}

		
		
	}

	bool processPages() {
		int i, j, k;
		int res;
		int ind;

		int iw = iDim.x;
	    int jw = iDim.y;
	    int kw = iDim.z;

	    int ii;
	    int jj;
	    int kk;

	    bool cmade = false;

	    bool cont = true;


	    iVector3 curPos;
	    iVector3 camPagePos;

	    camPagePos.x = cameraPos.x/iPageSize;
	    camPagePos.y = cameraPos.y/iPageSize;
	    camPagePos.z = cameraPos.z/iPageSize;

		minWithinLR.x = camPagePos.x - loadRad;
		minWithinLR.y = camPagePos.y - loadRad;
		minWithinLR.z = camPagePos.z - loadRadZ;

		maxWithinLR.x = camPagePos.x + loadRad;
		maxWithinLR.y = camPagePos.y + loadRad;
		maxWithinLR.z = camPagePos.z + loadRadZ;

	    int curInd = 0;
		

	    int m;
	    E_STATES nState;


	    if (threadpool.allocated() >= 16) {
	    	threadpool.joinAll();
	    }


		for (k = -loadRadZ; k <= loadRadZ; k++) {
			for (j = -loadRad; j <= loadRad; j++) {
				for (i = -loadRad; i <= loadRad; i++) {

					ii = i+camPagePos.x;
					jj = j+camPagePos.y;
					kk = k+camPagePos.z;


					res = checkBounds(ii,jj,kk);

					if (res > 0) {



						ind = kk*jw*iw + jj*iw + ii;

						curPos.x = ii*iPageSize;
						curPos.y = jj*iPageSize;
						curPos.z = kk*iPageSize;

						


						if (worldData[ind] == NULL) {

							#ifdef DEBUG_MODE
							doTrace("E_STATE_INIT_LAUNCH");
							#endif

							worldData[ind] = new GamePage();
							worldData[ind]->init(singleton, iPageSize, curPos, 512);

							cont=false;
						}
						else if (cont) {

							nState = (E_STATES)curDiagram[worldData[ind]->curState];

							switch(nState) {
	                            case E_STATE_CREATESIMPLEXNOISE_LAUNCH:

	                            	#ifdef DEBUG_MODE
	                            	doTrace("E_STATE_CREATESIMPLEXNOISE_LAUNCH");
	                            	#endif

	                            	worldData[ind]->nextState = nState;
	                            	
	                            	
	                            	if ( threadpool.available() > 0 ) {
	                            		threadpool.start(*worldData[ind]);
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
							
						}

					}

					curInd++;
				}
			}
		}
		
		return cmade;
	}

	void renderPages(float zoom, int maxH) {
		int i, j, k;
		int res;
		int ind;

		int iw = iDim.x;
	    int jw = iDim.y;
	    int kw = iDim.z;

	    int ii;
	    int jj;
	    int kk;

	    iVector3 curPos;
	    iVector3 camPagePos;

	    camPagePos.x = cameraPos.x/iPageSize;
	    camPagePos.y = cameraPos.y/iPageSize;
	    camPagePos.z = cameraPos.z/iPageSize;

	    int curInd = 0;
		

	    int m;


	    singleton->bindShader("ShaderTarg2");
	    singleton->bindFBO("testFBO");

	    

	    maxH = std::max(std::min(loadRadZ,maxH), -loadRadZ);

		for (k = -loadRadZ; k <= maxH; k++) {
			for (j = -loadRad; j <= loadRad; j++) {
				for (i = -loadRad; i <= loadRad; i++) {

					ii = i+camPagePos.x;
					jj = j+camPagePos.y;
					kk = k+camPagePos.z;


					res = checkBounds(ii,jj,kk);

					if (res > 0) {



						ind = kk*jw*iw + jj*iw + ii;

						curPos.x = ii*iPageSize;
						curPos.y = jj*iPageSize;
						curPos.z = kk*iPageSize;

						if (worldData[ind] == NULL) {

						}
						else {

							switch(curDiagram[worldData[ind]->curState]) {
								case E_STATE_LENGTH:
									drawPage(worldData[ind], ii, jj, kk);
								break;
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
	    

		int pitchSrc = gp->iRenderSize;
		int pitchSrc2 = gp->iRenderSize/2;
		int pitchSrc4 = gp->iRenderSize/4;


		int x1 = (dx*pitchSrc2-dy*pitchSrc2) - pitchSrc2;
		int y1 = (-(dx*pitchSrc2/2) + -(dy*pitchSrc2/2) + dz*pitchSrc2) - pitchSrc2;
		int x2 = x1 + pitchSrc;
		int y2 = y1 + pitchSrc;

		float fx1 = x1;
		float fy1 = y1;
		float fx2 = x2;
		float fy2 = y2;


		fx1 = fx1*2.0f*unitScale/fScreenDim.x;
		fy1 = fy1*2.0f*unitScale/fScreenDim.y;
		fx2 = fx2*2.0f*unitScale/fScreenDim.x;
		fy2 = fy2*2.0f*unitScale/fScreenDim.y;



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




	void postProcess(float zoom) {
		

		if (doDrawFBO) {
			singleton->drawFBOOffset("volGenFBO", 0, singleton->cameraPos.x, singleton->cameraPos.y, zoom);
		}
		else {
			singleton->bindShader("ShaderLighting");
			singleton->setShaderVec2("mouseCoords",singleton->mouseX,singleton->mouseY);
			
			singleton->bindFBO("resultFBO");
			singleton->sampleFBO("testFBO");

			//MUST BE CALLED AFTER FBO IS BOUND
			singleton->setShaderVec2("resolution",singleton->currentFBOResolutionX, singleton->currentFBOResolutionY);

			singleton->drawFSQuad(1.0);
			singleton->unsampleFBO("testFBO");
			singleton->unbindFBO();
			singleton->unbindShader();

			//glEnable(GL_BLEND);
			singleton->drawFBOOffset("resultFBO", 0, singleton->cameraPos.x, singleton->cameraPos.y, zoom);
			//glDisable(GL_BLEND);
		}
		
	}

	/*
	void setNeighbors(int x, int y, int z) {
		int rad = 1;
		int i, j, k;
		int ind;

		int iw = iDim.x;
	    int jw = iDim.y;
	    int kw = iDim.z;

		int curX, curY, curZ;

		ind = z*jw*iw + y*iw + x;

		int ind2;
		int ind3;

		int minX = std::max(0,minWithinLR.x);
		int minY = std::max(0,minWithinLR.y);
		int minZ = std::max(0,minWithinLR.z);

		int maxX = std::min(iDim.x,maxWithinLR.x + 1);
		int maxY = std::min(iDim.y,maxWithinLR.y + 1);
		int maxZ = std::min(iDim.z,maxWithinLR.z + 1);


		for (k = -rad; k <= rad; k++) {
			curZ = k + z;
			for (j = -rad; j <= rad; j++) {
				curY = j + y;
				for (i = -rad; i <= rad; i++) {
					curX = i + x;

					ind2 = (k+1)*9 + (j+1)*3 + (i+1);
					ind3 = curZ*jw*iw + curY*iw + curX;
					
					if ((curZ < minZ || curZ >= maxZ)||(curY < minY || curY >= maxY)||(curX < minX || curX >= maxX)) {
						worldData[ind]->neighbors[ind2] = blankPage;
					}
					else {


						worldData[ind]->neighbors[ind2] = worldData[ind3];
					}

				}
			}


		}

	}

	int checkNeighbors(int x, int y, int z, int rad, int minState) {
		int i, j, k;
		int ind;

		int iw = iDim.x;
	    int jw = iDim.y;
	    int kw = iDim.z;

		int curX, curY, curZ;

		int minX = std::max(0,minWithinLR.x);
		int minY = std::max(0,minWithinLR.y);
		int minZ = std::max(0,minWithinLR.z);

		int maxX = std::min(iDim.x,maxWithinLR.x + 1);
		int maxY = std::min(iDim.y,maxWithinLR.y + 1);
		int maxZ = std::min(iDim.z,maxWithinLR.z + 1);



		for (k = -rad; k <= rad; k++) {
			curZ = k + z;

			if (curZ < minZ || curZ >= maxZ) {

			}
			else {
				for (j = -rad; j <= rad; j++) {
					curY = j + y;

					if (curY < minY || curY >= maxY) {

					}
					else {
						for (i = -rad; i <= rad; i++) {
							curX = i + x;

							if (curX < minX || curX >= maxX) {
								
							}
							else {
								ind = curZ*jw*iw + curY*iw + curX;
								


								if (worldData[ind] == NULL) {
									return 0;
								}
								else {
									if (worldData[ind]->curState < minState) {
										return 0;
									}
								}
								
							}

						}
					}

					
				}
			}


		}

		return 1;
	}
	*/

	

	void setWH(int width, int height) {
		iScreenDim.x = width;
		fScreenDim.x = width;
		iScreenDim.y = height;
		fScreenDim.y = height;
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