// f00300_singleton.h
//

#include "f00300_singleton.e"
#define LZZ_INLINE inline
Singleton::Singleton ()
                    {
		volTris = NULL;
		gw = NULL;
		myWS = NULL;
	}
void Singleton::init (int _defaultWinW, int _defaultWinH, int _scaleFactor, WebSocketServer * _myWS)
                                                                                                {

		pushTrace("Singleton init");



		mouseMovingSize = 100;
		mouseMovingLoc = 0;
		mouseCount = 0;
		mouseMovingStepsBack = 20;
		mouseMoving = new FIVector4[mouseMovingSize];

		myWS = _myWS;

		poolItemsCreated = 0;
		activeMode = 1;

		isZooming = false;
		isPanning = false;
		softMode = false;
		reportPagesDrawn = false;
		isBare = false;
		grassHeight = 1.0/128.0;


		defaultWinW = _defaultWinW/_scaleFactor;
		defaultWinH = _defaultWinH/_scaleFactor;
		scaleFactor = _scaleFactor;

		curBrushRad = 1.0f;

		mouseState = E_MOUSE_STATE_MOVE;

		worldSeed.setFXYZ(

			genRand(5000.0f,500000.0f),
			genRand(5000.0f,500000.0f),
			genRand(5000.0f,500000.0f)
		);

		
		bufferMult = 2;
		diskOn = 0.0f;
		visPageSizeInPixels = 128; // height of one page in pixels
		visPageSizeInUnits = 8;
		worldSizeInPages.setIXYZ(128,128,12);
		unitSizeInPixels = (visPageSizeInPixels)/visPageSizeInUnits;
		

		maxH = worldSizeInPages.getIZ();
		maxW = 4;

		maxHeightInUnits = (worldSizeInPages.getIZ()-bufferMult)*(visPageSizeInUnits);

		minBoundsInPixels.setIXYZ(0,0,0);
		maxBoundsInPixels.setIXYZ(
			(worldSizeInPages.getIX()-1)*unitSizeInPixels*visPageSizeInUnits,
			(worldSizeInPages.getIY()-1)*unitSizeInPixels*visPageSizeInUnits,
			(worldSizeInPages.getIZ()-1)*unitSizeInPixels*visPageSizeInUnits
		);


		wsBufferInvalid = true;

		bufferDim.setIXY(defaultWinW,defaultWinH);
		bufferDimHalf.setIXY(defaultWinW/2,defaultWinH/2);
		

		myTimer.start();

		grassState = E_GRASS_STATE_OFF;

		activeObject = E_OBJ_NONE;

		extraRad = 0;
		lastTime = 0.0;

		srand(time(0));

		


		int i;
		mbDown=false;
		lbDown=false;
		rbDown=false;

		for (i = 0; i < MAX_KEYS; i++) {
			keyDownArr[i] = false;
		}
		keySetup();

		

		isFullScreen = false;
	    shadersAreLoaded = 0;
	    readyToRecompile = 0;

		programState = E_PS_IN_GAME;


		cameraPos.setFXYZ(2048.0, 2048.0, maxBoundsInPixels.getFZ()/2.0);

		lightPos.copyFrom(&cameraPos);
		fogPos.copyFrom(&cameraPos);

		lightPos.setFZ(maxBoundsInPixels.getFZ()*0.6f);
		//lightPos.addXYZ(0.0f, 0.0f, 0.0f);
		fogPos.addXYZ(-256.0f);

	    cameraZoom = 1.0f;
	    targetZoom = 1.0f;

	    mouseX = 0.0f;
	    mouseY = 0.0f;
	    mouseXUp = 0.0f;
	    mouseYUp = 0.0f;

		createVTList();
		createGrassList();


		//// GL WIDGET START ////
		
		frameCount = 0;
		changesMade = false;
		forceGetPD = false;
		bufferInvalid = false;
		notQuit = true;
		timerNotSet = true;
		screenWidth = 400;
		screenHeight = 300;
		mouseLeftDown = mouseRightDown = false;
		mouseX = mouseY = 0;
		myDelta = 0.0f;
		
		
		

		

	


		//gm = new GameMap();
		orthographicProjection();
		//// GL WIDGET END ////



	    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // 4-byte pixel alignment
	    glEnable(GL_TEXTURE_2D);
	    glDisable(GL_DEPTH_TEST);
	    glDisable(GL_CULL_FACE);
	    glDisable(GL_LIGHTING);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glDisable(GL_BLEND);
	    //glEnable(GL_BLEND);


	    


	    fboStrings.push_back("worldSpaceFBO");

	    fboStrings.push_back("pagesFBO");
	    fboStrings.push_back("grassFBO");
	    fboStrings.push_back("geomFBO");
	    fboStrings.push_back("combineFBO");
	    fboStrings.push_back("resultFBO");
	    fboStrings.push_back("volGenFBO");


	    /*
	    shaderStrings.push_back("Simplex2D");
	    shaderStrings.push_back("CalcFlow");
	    shaderStrings.push_back("Erode");
	    shaderStrings.push_back("DLA");
	    shaderStrings.push_back("MapShader");
	    shaderStrings.push_back("shaderWater");
	    */


	    shaderStrings.push_back("WorldSpaceShader");
	    shaderStrings.push_back("BlitShader");
	    shaderStrings.push_back("LightingShader");
	    shaderStrings.push_back("GeomShader");
	    shaderStrings.push_back("GrassShader");
	    shaderStrings.push_back("CombineShader");
	    shaderStrings.push_back("GenerateVolume");
	    shaderStrings.push_back("GenerateVolumeBare");
	    shaderStrings.push_back("RenderVolume");
	    

	    

	    shaderTextureIDs.push_back("Texture0");
	    shaderTextureIDs.push_back("Texture1");
	    shaderTextureIDs.push_back("Texture2");
	    shaderTextureIDs.push_back("Texture3");
	    shaderTextureIDs.push_back("Texture4");
	    shaderTextureIDs.push_back("Texture5");
	    shaderTextureIDs.push_back("Texture6");
	    shaderTextureIDs.push_back("Texture7");


	    setupLookups();

	    for (i = 0; i < shaderStrings.size(); i++) {
	        shaderMap.insert(  pair<string,Shader*>(shaderStrings[i], NULL)  );
	    }
	    doShaderRefresh();

	    //fboSize = 512;
	    //bufsPerFBO = 2;

	    for (i = 0; i < fboStrings.size(); i++) {
	        fboMap.insert(  pair<string, FBOSet*>(fboStrings[i], new FBOSet())  );
	    }

	    //init(int _numBufs, int _width, int _height, int _bytesPerChannel);

	    fboMap["worldSpaceFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false);

	    fboMap["pagesFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["grassFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["geomFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, true);
	    fboMap["combineFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["resultFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["volGenFBO"]->init(1, 4096, 4096, 1, false);


	    gw = new GameWorld();
	    gw->init(this);


	    
	    popTrace();




	}
void Singleton::sendPoolIdToFront (int id)
                                       {
		int i = 0;

		pagePoolIds.remove(id);
		pagePoolIds.push_front(id);

	}
int Singleton::requestPoolId (int requestingPageId)
                                                {

		
		int pageToFreeId;
		int usedByPageId;

		
		if (TOT_GPU_MEM_USAGE < MAX_GPU_MEM) {
			pagePoolItems.push_back( new PooledResource() );
			pagePoolItems.back()->init(this);

			pageToFreeId = poolItemsCreated;
			pagePoolIds.push_front(pageToFreeId);
			poolItemsCreated++;

		}
		else {
			pageToFreeId = pagePoolIds.back();
			usedByPageId = pagePoolItems[pageToFreeId]->usedByPageId;

			GamePage* consumingPage;

			if (usedByPageId == -1) {
				// this pooledItem is already free 

			}
			else {
				// free this pooledItem from the page that is consuming it and give it to the requesting page

				consumingPage = gw->worldData[usedByPageId];

				if (consumingPage == NULL) {
					// page was deleted already
				}
				else {
					consumingPage->unbindGPUResources();
				}
				

			}

			pagePoolIds.pop_back();
			pagePoolIds.push_front(pageToFreeId);
		}


		pagePoolItems[pageToFreeId]->usedByPageId = requestingPageId;

		return pageToFreeId;

	}
void Singleton::qNormalizeAngle (int & angle)
        {
	    while (angle < 0) {
	        angle += 360 * 16;
	    }
	    while (angle > 360 * 16) {
	        angle -= 360 * 16;
	    }
	}
void Singleton::setupLookups ()
                            {
		pushTrace("setupLookups");

	    uint i, j, k, m;
	    uint side = 256;
	    uint totalSize = side*side*side;

	    lookup2to3 = new uint[totalSize];
	    //lookup3to2 = new uint[totalSize];

	    uint xpos;
	    uint ypos;
	    uint zpos;

	    uint ind;

	    ind = 0;
	    for (j = 0; j < 4096; j++) {
	        for (i = 0; i < 4096; i++) {
	            ind = i+j*4096;
	            xpos = i%256;
	            ypos = j%256;
	            zpos = i/256 + (j/256)*16;////(ind)/(256*256);

	            //lookup2to3[ind] = ind;
	            //ind++;
	            lookup2to3[ind] = (255<<24)|(zpos<<16)|(ypos<<8)|xpos;   
	        }
	    }
	    
	    /*
	    ind = 0;
	    for (k = 0; k < side; k++) {
	        for (j = 0; j < side; j++) {
	            for (i = 0; i < side; i++) {

	                lookup3to2[ind] = ind;
	                ind++;
	            }
	        }
	    }
	    */
	    

	    /*
	    glGenTextures(1,&lookup3to2ID);
	    glBindTexture(GL_TEXTURE_3D,lookup3to2ID);
	    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, side, side, side, 0, GL_RGBA, GL_UNSIGNED_BYTE, lookup3to2);
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
	    
	    
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	    
	    glBindTexture(GL_TEXTURE_3D,0);
	    */
	    /*
	    glBindTexture(GL_TEXTURE_2D,texID);
	        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iRenderSize, iRenderSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, uTexMap);
	        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	        glBindTexture(GL_TEXTURE_2D,0);
	    */

	    glGenTextures(1,&lookup2to3ID);
	    glBindTexture(GL_TEXTURE_2D,lookup2to3ID);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4096, 4096, 0, GL_RGBA, GL_UNSIGNED_BYTE, lookup2to3);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, 0);
	    /*
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	    */
	    glBindTexture(GL_TEXTURE_2D,0);

	    //delete[] lookup2to3;
	    //delete[] lookup3to2;

	    //lookup2to3 = NULL;
	    //lookup3to2 = NULL;


	    popTrace();

	}
void Singleton::perspectiveProjection ()
        {
	    float aspect = 1.0;

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    glFrustum(-aspect, +aspect, -1.0, +1.0, 4.0, 15.0);
	    glMatrixMode(GL_MODELVIEW);
	}
void Singleton::orthographicProjection ()
        {

	    glMatrixMode (GL_MODELVIEW);
	    glLoadIdentity ();
	    glMatrixMode (GL_PROJECTION);
	    glLoadIdentity ();

	}
Singleton::~ Singleton ()
                     {
		if (gw) {
			delete gw;
		}
	}
float Singleton::genRand (float LO, float HI)
                                          {

		return LO + (float)rand()/((float)RAND_MAX/(HI-LO));
	}
void Singleton::setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown)
                                                                                               {

		if (isDown) {
			progActionsDown[ps*256 + kc] = pa;
		}
		else {
			progActionsUp[ps*256 + kc] = pa;
		}
	}
void Singleton::setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown)
                                                                                {
		int i;

		for (i = 0; i < E_PS_SIZE; i++) {
			setProgAction((eProgramState)i, kc, pa, isDown);
		}

	}
void Singleton::keySetup ()
                        {

		setProgActionAll(27,  E_PA_QUIT, false);
		setProgActionAll('p', E_PA_TOGGLE_FULLSCREEN, false);
		setProgActionAll('r', E_PA_REFRESH, false);

	}
void Singleton::createGrassList ()
                               {

		int i;
		int j;

		float fi;
		float fj;

		float tcx;
		float tcy;

		int spacing = 1;

		int iMax = 512;
		int jMax = 512;

		float fiMax = (float)iMax;
		float fjMax = (float)jMax;
		float heightMod;

		grassTris = glGenLists(1);
		

		glNewList(grassTris, GL_COMPILE);

		//glBegin(GL_TRIANGLES);
		glBegin(GL_QUADS);

		//glNormal3f(0, 0, 1);

		

		for (j = jMax-1; j >= 0; j -= spacing) {
			fj = ((float)(j*2-jMax) + 1.0f)/fjMax;
			tcy = fj;//(fj + 1.0f)/2.0f;
			for (i = 0; i < iMax; i += spacing) {
				fi = ((float)(i*2-iMax) + 1.0f)/fiMax;
				tcx = fi;//(fi + 1.0f)/2.0f;
			

				heightMod = 0.0;//genRand(0.0f,4.0f)/fjMax;

				//glColor4f(backfaceX[i], backfaceY[i], backfaceZ[i], 1.0f);

				//
				
				
				glMultiTexCoord4f( GL_TEXTURE0, tcx, tcy, 0.2f, -1.0);
				glVertex3f(fi,fj,0.0);
				

				glMultiTexCoord4f( GL_TEXTURE0, tcx, tcy, 0.0f, 0.0);
				glVertex3f(fi,fj,0.0f);


				glMultiTexCoord4f( GL_TEXTURE0, tcx, tcy, 0.2f, 1.0);
				glVertex3f(fi,fj,0.0f);


				glMultiTexCoord4f( GL_TEXTURE0, tcx, tcy, 1.0f, 0.0);
				glVertex3f(fi,fj,0.0f);

				
			}


			
		}

		glEnd();
		
		glEndList();
		
	}
void Singleton::drawCubeCentered (FIVector4 originVec, float radius)
                                                                 {
		FIVector4 minV;
		FIVector4 maxV;

		minV.setFXYZ(
			originVec.getFX()-radius,
			originVec.getFY()-radius,
			originVec.getFZ()-radius
		);

		maxV.setFXYZ(
			originVec.getFX()+radius,
			originVec.getFY()+radius,
			originVec.getFZ()+radius
		);

		drawBox(minV,maxV);
	}
void Singleton::drawBox (FIVector4 minVec, FIVector4 maxVec)
                                                     {


		float minX = minVec.getFX();
		float minY = minVec.getFY();
		float minZ = minVec.getFZ();

		float maxX = maxVec.getFX();
		float maxY = maxVec.getFY();
		float maxZ = maxVec.getFZ();


    	glBegin(GL_QUADS);

    	    // front
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
    	    glVertex3f(minX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
    	    glVertex3f(maxX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
    	    glVertex3f(maxX, maxY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
    	    glVertex3f(minX, maxY, maxZ);
    	    // back
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
    	    glVertex3f(minX, minY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
    	    glVertex3f(maxX, minY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
    	    glVertex3f(maxX, maxY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
    	    glVertex3f(minX, maxY, minZ);
    	    // right
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
    	    glVertex3f(maxX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
    	    glVertex3f(maxX, minY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
    	    glVertex3f(maxX, maxY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
    	    glVertex3f(maxX, maxY, maxZ);
    	    // left
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
    	    glVertex3f(minX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
    	    glVertex3f(minX, minY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
    	    glVertex3f(minX, maxY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
    	    glVertex3f(minX, maxY, maxZ);
    	    // top
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
    	    glVertex3f(minX, maxY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
    	    glVertex3f(maxX, maxY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
    	    glVertex3f(maxX, maxY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
    	    glVertex3f(minX, maxY, minZ);
    	    // bottom
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
    	    glVertex3f(minX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
    	    glVertex3f(maxX, minY, maxZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
    	    glVertex3f(maxX, minY, minZ);
    	    glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
    	    glVertex3f(minX, minY, minZ);



    	glEnd();
    }
void Singleton::createVTList ()
                            {

		volTris = glGenLists(1);
		

		float texMin = 0.0f;
		float texMax = 1.0f;

		float fx1 = -1.0f;
		float fy2 = -1.0f;
		float fx2 = 1.0f;
		float fy1 = 1.0f;

		float centerX = (fx1 + fx2)/2.0f;
		float centerY = (fy1 + fy2)/2.0f;

		float fy25 = fy1*0.75f + fy2*0.25f;
		float fy75 = fy1*0.25f + fy2*0.75f;

		float coordsX[8];
		float coordsY[8];

		coordsX[0] = centerX;
		coordsY[0] = centerY;

		coordsX[1] = centerX;
		coordsY[1] = fy1;

		coordsX[2] = fx2;
		coordsY[2] = fy25;

		coordsX[3] = fx2;
		coordsY[3] = fy75;

		coordsX[4] = centerX;
		coordsY[4] = fy2;

		coordsX[5] = fx1;
		coordsY[5] = fy75;

		coordsX[6] = fx1;
		coordsY[6] = fy25;

		coordsX[7] = coordsX[1];
		coordsY[7] = coordsY[1];

		float backfaceX[8];
		float backfaceY[8];
		float backfaceZ[8];



		backfaceX[0] = texMin;
		backfaceY[0] = texMin;
		backfaceZ[0] = texMin;
		backfaceX[1] = texMin;
		backfaceY[1] = texMin;
		backfaceZ[1] = texMax;
		backfaceX[2] = texMax;
		backfaceY[2] = texMin;
		backfaceZ[2] = texMax;
		backfaceX[3] = texMax;
		backfaceY[3] = texMin;
		backfaceZ[3] = texMin;
		backfaceX[4] = texMax;
		backfaceY[4] = texMax;
		backfaceZ[4] = texMin;
		backfaceX[5] = texMin;
		backfaceY[5] = texMax;
		backfaceZ[5] = texMin;
		backfaceX[6] = texMin;
		backfaceY[6] = texMax;
		backfaceZ[6] = texMax;
		backfaceX[7] = backfaceX[1];
		backfaceY[7] = backfaceY[1];
		backfaceZ[7] = backfaceZ[1];


		glNewList(volTris, GL_COMPILE);

		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, 0, 1);

		int i;

		for (i = 0; i < 8; i++) {

			glColor4f(backfaceX[i], backfaceY[i], backfaceZ[i], 1.0f);

			glMultiTexCoord3f( GL_TEXTURE0, backfaceX[i], backfaceY[i], backfaceZ[i]);
			
			if (i == 0) {
				//glColor4f((backfaceX[i]+1.0f)/2.0f, (backfaceY[i]+1.0f)/2.0f, (backfaceZ[i]+1.0f)/2.0f, 1.0f);
				glMultiTexCoord3f( GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
			}
			else {
				glMultiTexCoord3f( GL_TEXTURE1, backfaceX[i], backfaceY[i], backfaceZ[i]);
			}

			glVertex3f(coordsX[i],coordsY[i],0.0f);
		}

		glEnd();
		

		glEndList();
	}
void Singleton::doShaderRefresh ()
                               {
		pushTrace( "doShaderRefresh" );

	    readyToRecompile = 0;

	    int i;
	    int j;


	    for (i = 0; i < shaderStrings.size(); i++) {
	        if (shaderMap[ shaderStrings[i] ]) {
	            delete shaderMap[ shaderStrings[i] ];
	        }
	        shaderMap[ shaderStrings[i] ] = new Shader( ("../src/glsl/"+shaderStrings[i]+".c").c_str() );
	    }
	    shadersAreLoaded=1;
	    readyToRecompile = 1;

	    popTrace();
	    
	}
void Singleton::setMatrices (int w, int h)
                                       {

		glViewport(0,0,w,h);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity ();
	    glMatrixMode (GL_PROJECTION);
	    glLoadIdentity ();

	}
void Singleton::setWH (int w, int h)
                                 {

	    baseW = w;
	    baseH = h;
	}
void Singleton::sampleFBODirect (FBOSet * fbos, int offset)
                                                         {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = 0; i < fbos->numBufs; i++) {
	            setShaderTexture(fbos->fbos[i].color_tex, i+offset);
	        }
	    }
	}
void Singleton::unsampleFBODirect (FBOSet * fbos, int offset)
                                                           {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = fbos->numBufs - 1; i >= 0; i--) {
	            setShaderTexture(0, i+offset);
	        }
	    }
	}
void Singleton::bindFBODirect (FBOSet * fbos)
                                         {
	    setMatrices(fbos->width,fbos->height);
	    fbos->bind(1);
	    currentFBOResolutionX = fbos->width;
	    currentFBOResolutionY = fbos->height;
	}
void Singleton::sampleFBO (string fboName, int offset)
                                                     {
	    FBOSet* fbos = fboMap[fboName];
	    sampleFBODirect(fbos,offset);
	}
void Singleton::unsampleFBO (string fboName, int offset)
                                                       {
	    
	    FBOSet* fbos = fboMap[fboName];
	    unsampleFBODirect(fbos,offset);
	}
FBOWrapper * Singleton::getFBOWrapper (string fboName, int offset)
                                                               {
		FBOSet* fbos = fboMap[fboName];
		return fbos->getFBOWrapper(offset);
	}
void Singleton::bindFBO (string fboName)
                                     {
	    
	    FBOSet* fbos = fboMap[fboName];
	    bindFBODirect(fbos);
	}
void Singleton::unbindFBO ()
                         {
	    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	    setMatrices(baseW,baseH);
	    
	}
void Singleton::bindShader (string shaderName)
                                           {

	    if (shadersAreLoaded) {
	        curShader = shaderName;
	        shaderMap[curShader]->bind();
	    }
	    
	}
void Singleton::unbindShader ()
                            {
	    if (shadersAreLoaded) {
	        shaderMap[curShader]->unbind();
	    }
	    
	}
void Singleton::setShaderFloat (string paramName, float x)
                                                       {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x);

	        shaderMap[curShader]->setShaderFloat(paramName, x);
	    }
	}
void Singleton::setShaderfVec2 (string paramName, FIVector4 * v)
                                                            {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderfVec2(paramName, v);
	    }
	}
void Singleton::setShaderVec2 (string paramName, float x, float y)
                                                               {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderVec2(paramName, x, y);
	    }
	}
void Singleton::setShaderVec3 (string paramName, float x, float y, float z)
                                                                        {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderVec3(paramName, x, y, z);
	    }
	}
void Singleton::setShaderfVec3 (string paramName, FIVector4 * v)
                                                            {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderfVec3(paramName, v);
	    }
	}
void Singleton::setShaderVec4 (string paramName, float x, float y, float z, float w)
                                                                                 {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y,z,w);
	        shaderMap[curShader]->setShaderVec4(paramName, x, y, z, w);
	    }
	}
void Singleton::setShaderfVec4 (string paramName, FIVector4 * v)
                                                            {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderfVec4(paramName, v);
	    }
	}
void Singleton::setShaderTexture (uint texID, int multitexNumber)
                                                              {
	    if (shadersAreLoaded) {
	        glActiveTexture(GL_TEXTURE0 + multitexNumber);
	        glBindTexture(GL_TEXTURE_2D, texID);
	        shaderMap[curShader]->setShaderInt(shaderTextureIDs[multitexNumber] ,multitexNumber);
	    }
	}
void Singleton::setShaderTexture3D (uint texID, int multitexNumber)
                                                                {
	    if (shadersAreLoaded) {
	        glActiveTexture(GL_TEXTURE0 + multitexNumber);
	        glBindTexture(GL_TEXTURE_3D, texID);
	        shaderMap[curShader]->setShaderInt(shaderTextureIDs[multitexNumber],multitexNumber);
	    }
	}
bool Singleton::shiftDown ()
                         {
		return glutGetModifiers()&GLUT_ACTIVE_SHIFT;
	}
bool Singleton::ctrlDown ()
                        {
		return glutGetModifiers()&GLUT_ACTIVE_CTRL;
	}
bool Singleton::altDown ()
                       {
		return glutGetModifiers()&GLUT_ACTIVE_ALT;
	}
void Singleton::drawFSQuad (float zoom)
                                    {
	    drawFSQuadOffset(0.0f,0.0f,zoom);
	}
void Singleton::drawFSQuadOffset (float xOff, float yOff, float zoom)
                                                                  {
	    float fx1 = (xOff-1.0f)*zoom;
	    float fy1 = (yOff-1.0f)*zoom;
	    float fx2 = (xOff+1.0f)*zoom;
	    float fy2 = (yOff+1.0f)*zoom;

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
	}
void Singleton::drawFBO (string fboName, int ind, float zoom)
                                                          {
	    drawFBOOffset(fboName, ind, 0.0f, 0.0f, zoom);
	}
void Singleton::drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zoom)
                                                                                            {

	    glBindTexture(GL_TEXTURE_2D, fbos->fbos[ind].color_tex);
	    //glClearColor(0.2,0.2,0.2,0.0);
	    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    drawFSQuadOffset(xOff,yOff,zoom);
	    glBindTexture(GL_TEXTURE_2D, 0);
	}
void Singleton::drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zoom)
                                                                                        {
	    FBOSet* fbos = fboMap[fboName];
	    drawFBOOffsetDirect(fbos, ind, xOff, yOff, zoom);
	}
void Singleton::moveCamera (FIVector4 * modXYZ)
                                           {
		wsBufferInvalid = true;
		cameraPos.addXYZRef(modXYZ);
		modXYZ->setFZ(0.0f);
		lightPos.addXYZRef(modXYZ, 1.0f);
		fogPos.addXYZRef(modXYZ, 1.0f);
		isPanning = true;
	}
void Singleton::moveObject (float dx, float dy, float zoom)
                                                        {


		

		float dxZoom = dx/zoom;
		float dyZoom = dy/zoom;

		bool doDefault = false;

		FIVector4 modXYZ;

		if (lbDown||rbDown) {
			if (rbDown || (shiftDown() ) ) {
				modXYZ.setFZ(  dyZoom );
				modXYZ.setFX( -(0.0f + dxZoom/2.0f) );
				modXYZ.setFY( -(0.0f - dxZoom/2.0f) );



			}
			else {
				modXYZ.setFX( -(dyZoom + dxZoom/2.0f) );
				modXYZ.setFY( -(dyZoom - dxZoom/2.0f) );
			}

			//modXYZTemp.copyFrom(&modXYZ);
			//modXYZTemp.normalize();
			lastModXYZ.addXYZRef(&modXYZ);


			
			
		}
		else {
			
		}

		

		if (shiftDown()) {

			if (mouseState == E_MOUSE_STATE_BRUSH) {
				curBrushRad -= modXYZ.getFZ()/50.0f;

				if (curBrushRad < 0.0f) {
					curBrushRad = 0.0f;
				}
			}
			else {
				grassHeight -= modXYZ.getFZ()/10000.0f;

				if (grassHeight < 0.0f) {
					grassHeight = 0.0f;
				}
			}

			
		}
		else {


			if (mouseState == E_MOUSE_STATE_BRUSH) {
				doDefault = true;
			}
			else {
				switch (activeObject) {

					case E_OBJ_LIGHT:
						lightPos.addXYZRef(&modXYZ, -1.0f);
						//lightPos.clampXYZ(&minBoundsInPixels,&maxBoundsInPixels);
						activeObjectPos.setFXYZRef(&lightPos);
					break;

					case E_OBJ_FOG:
						fogPos.addXYZRef(&modXYZ, -1.0f);
						//lightPos.clampXYZ(&minBoundsInPixels,&maxBoundsInPixels);
						activeObjectPos.setFXYZRef(&fogPos);
					break;
					default:
						doDefault = true;
					break;

				}
			}

			if (doDefault) {
				moveCamera(&modXYZ);
			}


			
		}


		

		

	}
void Singleton::doAction (eProgramAction pa)
                                         {
		switch (pa) {
			case E_PA_QUIT:
				if (isFullScreen) {
					//glutLeaveGameMode();
				}
				PROG_ACTIVE = false;
				glutLeaveMainLoop();
			break;
			case E_PA_TOGGLE_FULLSCREEN:
				isFullScreen = !isFullScreen;

				if (isFullScreen) {
					//glutGameModeString( "1280x800:32@75" ); //the settings for fullscreen mode
					//glutEnterGameMode();
					//glutFullScreen();
				}
				else {
					//glutReshapeWindow(defaultWinW, defaultWinH);
					//glutLeaveGameMode();
				}

			break;
			case E_PA_REFRESH:
            	doShaderRefresh();
			break;
			case E_PA_SIZE:
				//unused
			break;

		}
	}
void Singleton::processSpecialKeys (int key, int _x, int _y)
                                                         {

		int x = _x/scaleFactor;
		int y = _y/scaleFactor;

		switch(key) {
			case GLUT_KEY_F1 :

			break;
			case GLUT_KEY_F2 :

			break;
			case GLUT_KEY_F3 :

			break;
		}
	}
void Singleton::processKey (unsigned char key, int _x, int _y, bool isPressed)
                                                                           {
		
		int x = _x/scaleFactor;
		int y = _y/scaleFactor;
	}
void Singleton::keyboardUp (unsigned char key, int _x, int _y)
                                                           {

		int x = _x/scaleFactor;
		int y = _y/scaleFactor;

		changesMade = false;


		//doAction(progActionsUp[((int)programState)*256 + key]);

		int enCounter;

		bool restartGen = false;

		//doTrace(i__s(key) );


		if (key == 17) {
			glutLeaveMainLoop();
		}

		switch(key ) {

			
			case '0':
				activeMode = 0;

			break;
			case '1':
				activeMode = 1;
				doTrace("Dirt and Grass");
			break;
			case '2':
				activeMode = 2;
				doTrace("Rock");
			break;
			case '3':
				activeMode = 3;
				doTrace("Brick");
			break;
			case '4':
				activeMode = 4;
				doTrace("Flat Top Stone");
			break;
			case '5':
				activeMode = 5;
				doTrace("Flat Top Dirt");
			break;
			case '6':
				activeMode = 6;
			break;
			case '7':
				activeMode = 7;
			break;
			case '8':
				activeMode = 8;
			break;
			case '9':
				activeMode = 9;
			break;

			case 's':
				softMode = !softMode;
				
			break;

			case 'w':
				changesMade = true;
				maxW++;
			break;
			case 'q':
				changesMade = true;
				maxW--;
				if (maxW < 1) {
					maxW = 1;
				}
			break;

			case 'r':
				doShaderRefresh();
				bufferInvalid = true;

			break;

			case 'g':

				enCounter = (int)grassState;
				enCounter++;
				grassState = (E_GRASS_STATE)enCounter;

				if (grassState == E_GRASS_STATE_LENGTH) {
					grassState = (E_GRASS_STATE)0;
				}

				bufferInvalid = true;
				changesMade = true;
			break;



			case 't':
				restartGen = true;
			break;

			case '\t':


				enCounter = (int)mouseState;
				
				

				if (ctrlDown()) {
					enCounter--;
				} 
				else {
					enCounter++;
				}
				if (enCounter < 0) {
					enCounter = ((int)E_MOUSE_STATE_LENGTH)-1;
				}

				mouseState = (E_MOUSE_STATE)enCounter;

				if (mouseState == E_MOUSE_STATE_LENGTH) {
					mouseState = (E_MOUSE_STATE)0;
				}

				bufferInvalid = true;
				changesMade = true;
				wsBufferInvalid = true;
				forceGetPD = true;


			break;

			case 'c':
				isBare = !isBare;
				restartGen = true;
			break;

			case 'm':
				reportPagesDrawn = true;
				//doTrace("Avail threads: ", i__s(gw->availThreads));
			break;

			case 'a':
				changesMade = true;
				maxH++;
			break;
			case 'z':
				changesMade = true;
				maxH--;
				if (maxH < 0) {
					maxH = 0;
				}
			break;
			
			default:
				
			break;
		}

		if (restartGen) {
			doShaderRefresh();
			gw->resetToState(E_STATE_CREATESIMPLEXNOISE_END);
			bufferInvalid = true;
			changesMade = true;
		}

	}
void Singleton::keyboardDown (unsigned char key, int _x, int _y)
                                                             {


		int x = _x/scaleFactor;
		int y = _y/scaleFactor;

		//doAction(progActionsDown[((int)programState)*256 + key]);
	}
int Singleton::clamp (int val, int min, int max)
                                             {
		if (val > max) {
			val = max;
		}
		if (val < min) {
			val = min;
		}
		return val;
	}
float Singleton::clampf (float val, float min, float max)
                                                      {
		if (val > max) {
			val = max;
		}
		if (val < min) {
			val = min;
		}
		return val;
	}
void Singleton::getPixData (FIVector4 * toVector, int xv, int yv)
                                                             {

		FBOWrapper* fbow;
		int newX;
		int newY;

		float newFX;
		float newFY;

		if (wsBufferInvalid) {
			gw->renderWorldSpace();
		}

		


		if (cameraZoom > 1.0) {
			newX = (int)((  (((float)xv)-bufferDimHalf.getFX()) / cameraZoom)+bufferDimHalf.getFX());
			newY = (int)((  (((float)yv)-bufferDimHalf.getFY()) / cameraZoom)+bufferDimHalf.getFY());
		}
		else {
			newX = clamp(xv,0,bufferDim.getIX()-1);
			newY = clamp(yv,0,bufferDim.getIY()-1);
		}

		

		fbow = getFBOWrapper("worldSpaceFBO",0);
		fbow->getPixelAtF(toVector, newX, (bufferDim.getIY()-1)-newY);
	}
void Singleton::mouseMove (int _x, int _y)
                                       {

		int x = _x/scaleFactor;
		int y = _y/scaleFactor;

		int dx = x - lastPosX;
		int dy = y - lastPosY;


		lastMouseX = x;
		lastMouseY = y;


		

		mouseXUp = x;
		mouseYUp = y;

		

		if (lbDown || rbDown) {
		    moveObject((float)dx, (float)dy, cameraZoom);
		}
		else {
			if (mouseState == E_MOUSE_STATE_BRUSH  ) {
				getPixData(&mouseMovePD, x, y);
				gw->modifyUnit(&mouseMovePD, E_BRUSH_MOVE);
			}
		}


		if (mbDown) {

		}
		lastPosX = x;
		lastPosY = y;

		if ( (x >= 0) && (y >= 0) && (x < baseW) && (y < baseH) ) { // && (rbDown||lbDown||mbDown)
			bufferInvalid = true;
			
			if (rbDown||lbDown) {
				changesMade = true;
			}
			
			
		}
	}
void Singleton::worldToScreen (FIVector4 * sc, FIVector4 * wc)
                                                         {

		FIVector4 dMod;

		dMod.copyFrom(wc);
		dMod.addXYZRef(&cameraPos, -1.0);

		float x1 = (dMod.getFX()-dMod.getFY());
		float y1 = (-(dMod.getFX()/2.0f) + -(dMod.getFY()/2.0f) + dMod.getFZ());
		float myZoom = std::min(1.0f, cameraZoom);

		x1 = x1*(myZoom)/bufferDimHalf.getFX();
		y1 = y1*(myZoom)/bufferDimHalf.getFY();

		x1 = (x1 + 1.0)/2.0;
		y1 = (y1 + 1.0)/2.0;

		sc->setFX(x1);
		sc->setFY(y1);
	}
void Singleton::screenToWorld (FIVector4 * tc, FIVector4 * wc)
                                                         {


	}
void Singleton::mouseClick (int button, int state, int _x, int _y)
                                                               {
		

		int x = _x/scaleFactor;
		int y = _y/scaleFactor;

		lastPosX = x;
		lastPosY = y;

		int res;

		float wheelDelta = 0.0;
		bool mbClicked = false;
		bool rbClicked = false;
		bool lbClicked = false;

		int mouseMovingStart;
		


		switch (button) {
			case GLUT_LEFT_BUTTON:
				lbDown = (state == GLUT_DOWN);
				lbClicked = (state == GLUT_UP);
				
			break;
			case GLUT_RIGHT_BUTTON:
				rbDown = (state == GLUT_DOWN);
				rbClicked = (state == GLUT_UP);

			break;
			case GLUT_MIDDLE_BUTTON:
				mbDown = (state == GLUT_DOWN);
				mbClicked = (state == GLUT_UP);

			break;

			case 3: // wheel up
				wheelDelta = 1.0/20.0f;
				//changesMade = true;
			break;

			case 4: // wheel down
				wheelDelta = -1.0/20.0f;
				//changesMade = true;
			break;
		}








		if (state == GLUT_DOWN) {
			mouseVel.setFXY(0.0f,0.0f);
		}

		if (mbClicked) {

		}
		

		if (rbDown || lbDown) {
			
		}
		else {

		}

		if (rbClicked || lbClicked) {

			


			if (rbDown || lbDown) {

			}
			else {

				doTrace("MOUSE UP");
				muTime = myTimer.getElapsedTimeInMilliSec();

				mouseEnd.setIXY(x,y);
				//mouseVel.copyFrom(&mouseEnd);
				//mouseVel.addXYZRef(&mouseMoving, -1.0f);

				if (activeObject != E_OBJ_LIGHT && activeObject != E_OBJ_FOG && (muTime-mdTime > 300.0f) ) {
					mouseMovingStart = (mouseMovingLoc - min(mouseMovingStepsBack, mouseCount) + mouseMovingSize)%mouseMovingSize;
					

					if (mouseCount > 0) {
						mouseVel.copyFrom( &(mouseMoving[mouseMovingLoc]) );
						mouseVel.addXYZRef( &(mouseMoving[mouseMovingStart]), -1.0f);
					}

					
				}

				

				
				/*
				mouseMovingSize = 100;
				mouseMovingLoc = 0;
				mouseCount = 0;
				mouseMovingStepsBack = 10;
				*/

				lastModXYZ.normalize();

				//if (activeObject == E_OBJ_LIGHT || activeObject == E_OBJ_CAMERA) {
					//mouseVel
				//}


				//mouseVel.multXYZ( clampf(1.0f-(muTime-mdTime)/1000.0f, 0.1f, 1.0f)/cameraZoom );

				if (shiftDown()) {

				}
				else {
					




					activeObject = E_OBJ_NONE;
					wsBufferInvalid = true;
					getPixData(&mouseUpPD, x, y);

					
					



					if ( mouseEnd.distance(&mouseStart) > 30.0 ) {
						
					} 
					else {
						if (mouseState == E_MOUSE_STATE_BRUSH) {


							if (lbClicked) {
								gw->modifyUnit(&mouseUpPD, E_BRUSH_ADD);
							}
							else {
								gw->modifyUnit(&mouseUpPD, E_BRUSH_SUB);
							}

							forceGetPD = true;

						}
					}


					diskOn = 0.0f;
				}

				
			}

			
		}
		else {
			if (rbDown || lbDown) {

				if (rbDown && lbDown) {

				}
				else {


					lastModXYZ.setFXYZ(0.0f, 0.0f, 0.0f);

					mouseMovingLoc = 0;
					mouseCount = 0;
					doTrace("MOUSE DOWN");
					mdTime = myTimer.getElapsedTimeInMilliSec();
					mouseStart.setIXY(x,y);

					getPixData(&mouseDownPD, x, y);
					activeObject = (E_OBJ)((int) mouseDownPD.getFW());

					switch (activeObject) {
						case E_OBJ_LIGHT:
							activeObjectPos.setFXYZRef(&lightPos);
							diskOn = 1.0f;
						break;

						case E_OBJ_FOG:
							activeObjectPos.setFXYZRef(&fogPos);
							diskOn = 1.0f;
						break;
						default:
							
						break;
					}

					if (mouseState == E_MOUSE_STATE_BRUSH) {
						diskOn = 0.0;
					}


				}

				
			}
		}


		









		
		

		if (button == 3 || button == 4) {

			myDelta += wheelDelta;
			targetZoom = pow(2.0, myDelta);
			isZooming = true;

		}

		if (x >= 0 && y >= 0 && x < baseW && y < baseH) {
			bufferInvalid = true;
		}

	}
void Singleton::processB64 ()
                          {
		doTrace("processData() BASE64");

		membuf sbuf(myWS->recBuffer, myWS->recBuffer + myWS->recBufferLength);
		Poco::Base64Decoder b64in(sbuf);

		//ofBuffer buffer;
		//b64in >> buffer;



	}
void Singleton::processJSON ()
                           {
		doTrace("processData() JSON");

		JSONValue *jsonVal = myWS->recMessage;

		if (jsonVal == NULL) {

		}
		else {
			//doTrace( "JSON VAL", f__s(jsonVal->Child(L"x")->number_value) , "\n\n" );
		}
		
		
	}
void Singleton::display ()
                           {





		curTime = myTimer.getElapsedTimeInMilliSec();

		float elTime = curTime - lastTime;
		float dz;
		float fMouseVel;

		if (myWS == NULL) {

		}
		else {

			if (myWS->dataReady) {

				if (myWS->isJSON) {
					processJSON();
				}
				else {
					processB64();
				}

				myWS->dataReady = false;
				myWS->isWorking = false;
			}
		}
		

		


		if (elTime >= 16.0f) {
			lastTime = curTime;



			mouseMovingLoc = (mouseMovingLoc+1)%mouseMovingSize;
			mouseCount++;
			mouseMoving[mouseMovingLoc].setIXY(lastMouseX,lastMouseY);



			lastModXYZ.multXYZ(0.95f);

			mouseVel.multXYZ(0.95f);
			
			fMouseVel = mouseVel.distance(&origin);

			if ( fMouseVel < 1.0f ) {
				mouseVel.setFXY(0.0f,0.0f);
				isPanning = false;
			}
			else {
				isPanning = true;


				panMod.copyFrom(&lastModXYZ);
				panMod.multXYZ(fMouseVel/16.0f);
				moveCamera(&panMod);
			}


			dz = (targetZoom-cameraZoom)/(16.0f);

			if (abs(dz) < 0.0001) {
				dz = 0.0f;
			}

			if (cameraZoom > 8.0f) {
				cameraZoom = 8.0f;
			}
			if (cameraZoom < 1.0f/8.0f) {
				cameraZoom = 1.0f/8.0f;
			}
			
			cameraZoom += dz;

			if ((dz == 0.0f) && (isZooming)) {
				isZooming = false;
				wsBufferInvalid = true;
				bufferInvalid = true;
				changesMade = true;
			}
			else {
				if (isZooming) {
					bufferInvalid = true;
					changesMade = true;
				}
				
			}

			if (shadersAreLoaded) {
				gw->update();

				changesMade = false;
				bufferInvalid = false;
			}
		}

		

		

		//doTrace( "POSSIBLE ERROR: " , i__s(glGetError()) , "\n" );

	}
void Singleton::reshape (int w, int h)
                                   {
		
		setWH(w,h);

		screenWidth = w;
		screenHeight = h;
		
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
		glMatrixMode(GL_MODELVIEW);
		orthographicProjection();

		
		setMatrices(baseW,baseH);
	}
void Singleton::idleFunc ()
                            {
		
	}
#undef LZZ_INLINE
 
