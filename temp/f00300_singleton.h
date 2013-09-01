// f00300_singleton.h
//

#include "f00300_singleton.e"
#define LZZ_INLINE inline
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
Singleton::Singleton ()
                    {
		volTris = NULL;
		gw = NULL;
    	//gm = NULL;
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


		int iMax = 512;
		int jMax = 512;

		float fiMax = (float)iMax;
		float fjMax = (float)jMax;
		float baseRad = 1.0f/fiMax;
		float grassHeight = 0.0;//(4.0f)/fjMax;
		float heightMod;

		grassTris = glGenLists(1);
		

		glNewList(grassTris, GL_COMPILE);

		//glBegin(GL_TRIANGLES);
		glBegin(GL_QUADS);

		//glNormal3f(0, 0, 1);

		

		for (j = jMax-1; j >= 0; j--) {
			fj = ((float)(j*2-jMax))/fjMax;
			tcy = (fj + 1.0f)/2.0f;
			for (i = 0; i < iMax; i++) {
				fi = ((float)(i*2-iMax))/fiMax;
				tcx = (fi + 1.0f)/2.0f;
			

				heightMod = 0.0;//genRand(0.0f,4.0f)/fjMax;

				//glColor4f(backfaceX[i], backfaceY[i], backfaceZ[i], 1.0f);

				//
				
				
				glMultiTexCoord3f( GL_TEXTURE0, tcx, tcy, 0.0f);
				glVertex3f(fi-baseRad,fj,0.0f);
				glMultiTexCoord3f( GL_TEXTURE0, tcx, tcy, 0.0f);
				glVertex3f(fi+baseRad,fj,0.0f);

				glMultiTexCoord3f( GL_TEXTURE0, tcx, tcy, 1.0f);
				glVertex3f(fi-baseRad,fj+grassHeight+heightMod,1.0f);
				glMultiTexCoord3f( GL_TEXTURE0, tcx, tcy, 1.0f);
				glVertex3f(fi+baseRad,fj+grassHeight+heightMod,1.0f);
				
			}


			
		}

		glEnd();
		
		glEndList();
		
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
void Singleton::init (int _defaultWinW, int _defaultWinH)
                                                      {

		pushTrace("Singleton init");

		myTimer.start();

		extraRad = 0;
		lastTime = 0.0;

		srand(time(0));
		seedX = genRand(5000.0f,500000.0f);
		seedY = genRand(5000.0f,500000.0f);
		seedZ = genRand(5000.0f,500000.0f);

		int i;
		mbDown=false;
		lbDown=false;
		rbDown=false;

		for (i = 0; i < MAX_KEYS; i++) {
			keyDownArr[i] = false;
		}
		keySetup();

		defaultWinW = _defaultWinW;
		defaultWinH = _defaultWinH;

		isFullScreen = false;
	    shadersAreLoaded = 0;
	    readyToRecompile = 0;

		programState = E_PS_IN_GAME;

	    cameraPos.x = 0.0f;
	    cameraPos.y = 0.0f;
	    cameraPos.z = 0.0f;

	    iCameraPos.x = 0;
	    iCameraPos.y = 0;
	    iCameraPos.z = 0;

	    cameraZoom = 1.0f;

	    mouseX = 0.0f;
	    mouseY = 0.0f;
	    mouseXUp = 0.0f;
	    mouseYUp = 0.0f;

	    shiftPressed = false;
		ctrlPressed = false;
		altPressed = false;

		createVTList();
		createGrassList();


		//// GL WIDGET START ////
		
		frameCount = 0;
		changesMade = false;
		bufferInvalid = false;
		notQuit = true;
		timerNotSet = true;
		screenWidth = 400;
		screenHeight = 300;
		mouseLeftDown = mouseRightDown = false;
		mouseX = mouseY = 0;
		myDelta = 0.0f;
		iVector3 igwSize; igwSize.x = 64; igwSize.y = 64; igwSize.z = 64;
		gw = new GameWorld();



		gw->init(igwSize, this, E_RENDER_VOL);
		maxH = gw->loadRadZ;

		//gm = new GameMap();
		//gm->init(this, 1024, 512);
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


	    



	    fboStrings.push_back("testFBO");
	    fboStrings.push_back("resultFBO");
	    fboStrings.push_back("volGenFBO");

	    shaderStrings.push_back("shaderWater");
	    shaderStrings.push_back("ShaderTarg2");
	    shaderStrings.push_back("ShaderLighting");
	    shaderStrings.push_back("GrassShader");
	    shaderStrings.push_back("GenerateVolume");
	    shaderStrings.push_back("RenderVolume");
	    shaderStrings.push_back("Simplex2D");
	    shaderStrings.push_back("CalcFlow");
	    shaderStrings.push_back("Erode");
	    shaderStrings.push_back("DLA");
	    shaderStrings.push_back("MapShader");
	    

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
	    fboMap["testFBO"]->init(2, 1024, 1024, 1);
	    fboMap["resultFBO"]->init(1, 1024, 1024, 1);
	    fboMap["volGenFBO"]->init(1, 4096, 4096, 1);
	    
	    popTrace();




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
void Singleton::sampleFBODirect (FBOSet * fbos)
                                           {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = 0; i < fbos->numBufs; i++) {
	            setShaderTexture(fbos->fbos[i].color_tex, i);
	        }
	    }
	}
void Singleton::unsampleFBODirect (FBOSet * fbos)
                                             {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = fbos->numBufs - 1; i >= 0; i--) {
	            setShaderTexture(0, i);
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
void Singleton::sampleFBO (string fboName)
                                       {
	    FBOSet* fbos = fboMap[fboName];
	    sampleFBODirect(fbos);
	}
void Singleton::unsampleFBO (string fboName)
                                         {
	    
	    FBOSet* fbos = fboMap[fboName];
	    unsampleFBODirect(fbos);
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
void Singleton::setShaderfVec3 (string paramName, fVector3 * v)
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
void Singleton::moveCamera (float dx, float dy, float zoom)
                                                        {


		/*
		int dxmod = dx*pitchSrc2 - singleton->iCameraPos.x;
		int dymod = dy*pitchSrc2 - singleton->iCameraPos.y;
		int dzmod = dz*pitchSrc2 - singleton->iCameraPos.z;


		int x1 = (dxmod-dymod) - pitchSrc2;
		int y1 = (-(dxmod/2) + -(dymod/2) + dzmod) - pitchSrc2;
		*/

		float dxZoom = dx/zoom;
		float dyZoom = dy/zoom;

		if (lbDown) {
			cameraPos.x -= dyZoom + dxZoom/2.0;
			cameraPos.y -= dyZoom - dxZoom/2.0;//
			
			//cameraPos.x += dx/zoom;
			//cameraPos.y -= dy/zoom;
		}
		if (rbDown) {
			cameraPos.z += dyZoom;
		}

		iCameraPos.x = (int)cameraPos.x;
		iCameraPos.y = (int)cameraPos.y;
		iCameraPos.z = (int)cameraPos.z;

		//cameraPos.x += dx/(zoom/2.0);
		//cameraPos.y -= dy/(zoom/2.0);
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
void Singleton::processSpecialKeys (int key, int x, int y)
                                                       {

		switch(key) {
			case GLUT_KEY_F1 :

			break;
			case GLUT_KEY_F2 :

			break;
			case GLUT_KEY_F3 :

			break;
		}
	}
void Singleton::processKey (unsigned char key, int x, int y, bool isPressed)
                                                                         {
		
	}
void Singleton::keyboardUp (unsigned char key, int x, int y)
                                                         {

		changesMade = false;

		int keymod = glutGetModifiers();

		if (keymod&GLUT_ACTIVE_SHIFT) {} else {
			shiftPressed = false;
		}
		if (keymod&GLUT_ACTIVE_CTRL) {} else {
			ctrlPressed = false;
		}
		if (keymod&GLUT_ACTIVE_ALT) {} else {
			altPressed = false;
		}

		// TODO: map special keys like ctrl, alt, shift


		//doAction(progActionsUp[((int)programState)*256 + key]);


		switch(key) {
			case 'q':
				doAction(E_PA_QUIT);
			break;
			case 'r':
				//gm->baseRendered = false;

				doShaderRefresh();
				bufferInvalid = true;


				

			break;

			case '[':
				extraRad--;
				if (extraRad < 0) {
					extraRad = 0;
				}

				if (shiftPressed) {
					extraRad = 0;
				}

				doTrace("Extra Radius: ", i__s(extraRad));
			break;

			case ']':
				extraRad++;
				doTrace("Extra Radius: ", i__s(extraRad));
			break;

			case 't':
				doShaderRefresh();
			    gw->resetToState(E_STATE_COPYTOTEXTURE_END);
			    bufferInvalid = true;
			    changesMade = true;
			break;

			case 'a':
				changesMade = true;
				maxH++;
			break;
			case 'z':
				changesMade = true;
				maxH--;
			break;
			case 'f':
				gw->doDrawFBO = !(gw->doDrawFBO);
			break;
			default:
				
			break;
		}

	}
void Singleton::keyboardDown (unsigned char key, int x, int y)
                                                           {

		int keymod = glutGetModifiers();

		if (keymod&GLUT_ACTIVE_SHIFT) {
			shiftPressed = true;
		}
		if (keymod&GLUT_ACTIVE_CTRL) {
			ctrlPressed = true;
		}
		if (keymod&GLUT_ACTIVE_ALT) {
			altPressed = true;
		}

		//doAction(progActionsDown[((int)programState)*256 + key]);
	}
void Singleton::mouseMove (int x, int y)
                                     {
		int dx = x - lastPosX;
		int dy = y - lastPosY;

		mouseXUp = x;
		mouseYUp = y;

		if (lbDown || rbDown) {
		    moveCamera((float)dx, (float)dy, cameraZoom);
		}
		if (mbDown) {
		    mouseX = x;
		    mouseY = y;		    
		}
		lastPosX = x;
		lastPosY = y;

		if ( (x >= 0) && (y >= 0) && (x < baseW) && (y < baseH) && (rbDown||lbDown||mbDown) ) {
			bufferInvalid = true;
			
			if (rbDown||lbDown) {
				changesMade = true;
			}
			
			
		}
	}
void Singleton::mouseClick (int button, int state, int x, int y)
                                                             {
		
		lastPosX = x;
		lastPosY = y;

		float wheelDelta = 0.0;

		switch (button) {
			case GLUT_LEFT_BUTTON:
				lbDown = (state == GLUT_DOWN);
				changesMade = true;
			break;
			case GLUT_RIGHT_BUTTON:
				rbDown = (state == GLUT_DOWN);
			break;
			case GLUT_MIDDLE_BUTTON:
				mbDown = (state == GLUT_DOWN);
			break;

			case 3: // wheel up
				wheelDelta = 1.0/20.0f;
				changesMade = true;
			break;

			case 4: // wheel down
				wheelDelta = -1.0/20.0f;
				changesMade = true;
			break;
		}

		myDelta += wheelDelta;
		cameraZoom = pow(2.0, myDelta);

		if (x >= 0 && y >= 0 && x < baseW && y < baseH) {
			bufferInvalid = true;
		}

	}
void Singleton::display ()
                           {

		curTime = myTimer.getElapsedTimeInMilliSec();

		float elTime = curTime - lastTime;

		if (elTime >= 16.0) {
			lastTime = curTime;

			if (shadersAreLoaded) {
				gw->update(changesMade, bufferInvalid, maxH);

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
 
