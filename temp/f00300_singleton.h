// f00300_singleton.h
//

#include "f00300_singleton.e"
#define LZZ_INLINE inline
Singleton::Singleton ()
                    {
		volTris = NULL;
		sliceTris = NULL;
		gw = NULL;
		myWS = NULL;
	}
void Singleton::init (int _defaultWinW, int _defaultWinH, int _scaleFactor, WebSocketServer * _myWS)
                                                                                                {

		pushTrace("Singleton init");
		int i;
		rootObj = NULL;
		highlightedGeom = NULL;
		selectedGeom = NULL;

		srand(time(NULL));
		
		//uvPattern = loadBMP("..\\data\\uvpat.bmp");
		//uvPattern->getTextureId(GL_LINEAR);
		imageHM0 = loadBMP("..\\data\\hm0.bmp");
		imageHM1 = loadBMP("..\\data\\hm1.bmp");
		cloudImage = loadBMP("..\\data\\clouds.bmp");
		imageHM0->getTextureId(GL_NEAREST);
		imageHM1->getTextureId(GL_NEAREST);
		cloudImage->getTextureId(GL_LINEAR);
		mapSampScale = 0.5f;
		int newPitch = (imageHM0->width)*mapSampScale;//*2;

		numLights = min(MAX_LIGHTS,E_OBJ_LENGTH-E_OBJ_LIGHT0);


		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////

		firstRun = true;
		//useVolumeTex = false;
		waterOn = false;

		// 256 meters to a block
		// 32 meters to a lot (10,000 square feet)
		// 8 lots per block side, 64 lots in a block
		// 2 units to a meter

		maxLayers = MAX_LAYERS;

		// Resolution Dependent
		maxChanges = 16; // this number is defined in processPages()
		volGenFBOX = 128; // MAX OF 128, DO NOT CHANGE THIS VALUE
		curVGString = "volGenFBO128";
		//volGenFBOY = volGenFBOX*volGenFBOX;
		visPageSizeInPixels = 256; // height of one page in pixels
		holderSizeInPixels = 512; // height of holder in pixels
		bufferMultRec = 4;
		bufferMult = 1.0f + 1.0f/((float)bufferMultRec);


		// IMPORTANT: Maximum height must be less than 2^16, max world pitch must be less than 2^32

		// World Scale Dependent

		iNodeDivsPerLot = 4;

		metersPerLot = 32; // adjust this to make lots bigger
		pixelsPerMeter = 256; // when you make pixels per meter larger, you must do the same for units per meter
		unitsPerMeter = pixelsPerMeter/32;//16;
		blockSizeInLots = 8;

		maxFloors = MAX_FLOORS;

		pixelsPerUnit = pixelsPerMeter/unitsPerMeter;
		pixelsPerLot = pixelsPerMeter*metersPerLot;
		unitsPerLot = unitsPerMeter*metersPerLot;
		worldSizeInLots.setIXYZ(newPitch,newPitch,blockSizeInLots);
		holderSizeInPages = holderSizeInPixels/visPageSizeInPixels;
		holdersPerLot = pixelsPerLot/holderSizeInPixels;
		visPageSizeInUnits = visPageSizeInPixels/pixelsPerUnit;

		

		worldSizeInHolders.copyIntMult(&worldSizeInLots,holdersPerLot);
		bufferedPageSizeInUnits = visPageSizeInUnits*bufferMult;
		blockSizeInHolders = blockSizeInLots*holdersPerLot; // 8*2 = 16
		blockSizeInPages = blockSizeInHolders*holderSizeInPages;
		blockSizeInPixels = blockSizeInHolders*holderSizeInPixels;
		worldSizeInBlocks.copyIntDiv(&worldSizeInHolders,blockSizeInHolders);
		worldSizeInPages.copyIntMult(&worldSizeInHolders,holderSizeInPages);
		unitSizeInPixels = visPageSizeInPixels/visPageSizeInUnits;
		
		blockShift = intLogB2(blockSizeInHolders*blockSizeInHolders*blockSizeInHolders);

		holderSizeMB = (2.0f*4.0f*(float)(holderSizeInPixels*holderSizeInPixels))/(1024.0f*1024.0f);

		cout << "\n\n\n\n\n\n";

		cout << "maxChanges: " << maxChanges << "\n";
		cout << "volGenFBOX: " << volGenFBOX << "\n";
		cout << "volGenFBOY: " << volGenFBOX*volGenFBOX << "\n";
		cout << "visPageSizeInPixels: " << visPageSizeInPixels << "\n";
		cout << "holderSizeInPixels: " << holderSizeInPixels << "\n";
		cout << "bufferMult: " << bufferMult << "\n";
		cout << "metersPerLot: " << metersPerLot << "\n";
		cout << "pixelsPerMeter: " << pixelsPerMeter << "\n";
		cout << "unitsPerMeter: " << unitsPerMeter << "\n";
		cout << "blockSizeInLots: " << blockSizeInLots << "\n";
		cout << "pixelsPerUnit: " << pixelsPerUnit << "\n";
		cout << "unitsPerLot: " << unitsPerLot << "\n";
		cout << "pixelsPerLot: " << pixelsPerLot << "\n";
		cout << "holderSizeInPages: " << holderSizeInPages << "\n";
		cout << "holdersPerLot: " << holdersPerLot << "\n";
		cout << "visPageSizeInUnits: " << visPageSizeInUnits << "\n";
		cout << "unitSizeInPixels: " << unitSizeInPixels << "\n";


		doTraceVecND("worldSizeInBlocks: ", &worldSizeInBlocks);
		doTraceVecND("worldSizeInLots: ", &worldSizeInLots);
		doTraceVecND("worldSizeInHolders: ", &worldSizeInHolders);
		doTraceVecND("worldSizeInPages: ", &worldSizeInPages);

		cout << "\n\n\n\n\n\n";


		voroSize = 32;
		mapFreqs.setFXYZW(1.0f, 32.0f, 128.0f, 256.0f);
		mapAmps.setFXYZW(0.9f, 0.05f, 0.03f, 0.02f);
		
		maxPooledRes = 512;

		maxH = 4;
		maxW = 4;
		


		//cout << "holderSizeInPixels X2 " << holderSizeInPixels*2 << "\n";

		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		radiosityOn = false;
		updateLock = false;
		traceOn = false;
		gridOn = 0.0f;
		fogOn = 0.0f;
		geomCounter = 0;

		grassSpacing = 1;//8/DEF_SCALE_FACTOR;// *2.0;
		directPass = 0.0f;


		// TODO: examine if this variable is necessary
		maxHeightInUnits = (worldSizeInPages.getIZ()-bufferMult)*(visPageSizeInUnits);

		minBoundsInPixels.setIXYZ(0,0,0);
		maxBoundsInPixels.setIXYZ(
			(worldSizeInPages.getIX()-1)*unitSizeInPixels*visPageSizeInUnits,
			(worldSizeInPages.getIY()-1)*unitSizeInPixels*visPageSizeInUnits,
			(worldSizeInPages.getIZ()-1)*unitSizeInPixels*visPageSizeInUnits
		);

		heightmapMax = maxBoundsInPixels.getFZ()/2.0f;

		doTraceVecND("worldSizeInPixels: ", &maxBoundsInPixels);


		


		
		glGenTextures(1,&volID);
		glGenTextures(1,&volIDLinear);
		glGenTextures(1,&voroID);
		glGenTextures(1,&voroIDLinear);


		// if (useVolumeTex) {
		// 	glGenTextures(1,&volGenID);
		// 	glBindTexture(GL_TEXTURE_3D,volGenID);
		// 	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, volGenFBOX, volGenFBOX, volGenFBOX, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// 	glBindTexture(GL_TEXTURE_3D,0);
		// }
		


		glBindTexture(GL_TEXTURE_3D,volID);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, bufferedPageSizeInUnits, bufferedPageSizeInUnits, bufferedPageSizeInUnits, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_3D,0);

		glBindTexture(GL_TEXTURE_3D,volIDLinear);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, bufferedPageSizeInUnits, bufferedPageSizeInUnits, bufferedPageSizeInUnits, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_3D,0);





		glBindTexture(GL_TEXTURE_3D,voroID);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, voroSize, voroSize, voroSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glBindTexture(GL_TEXTURE_3D,0);

		glBindTexture(GL_TEXTURE_3D,voroIDLinear);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, voroSize, voroSize, voroSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glBindTexture(GL_TEXTURE_3D,0);


		createVoroVolume();


		geomIDArr = new int[1024];
		paramArr = new float[4096];
		matCountArr = new float[256];
		paramArrMap = new float[4096];

		
		
		
		showMap = true;

		

		lastImageBuffer.data = NULL;
		lastImageBuffer.size = 0;
		
		lastJSONBuffer.data = NULL;
		lastJSONBuffer.size = 0;
		
		nullBuffer.data = new char[1];
		nullBuffer.data[0] = '\0';
		nullBuffer.size = 0;

		

		palWidth = 256;
		palHeight = 256;

		resultImage = new unsigned char[256*256*4];

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
		isBare = true;
		grassWH.setFXYZ(1.0/128.0, 1.0/512.0, 0.0);
		diskOn = 0.0f;
		


		


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





		

		wsBufferInvalid = true;

		bufferDim.setIXY(defaultWinW,defaultWinH);
		bufferDimHalf.setIXY(defaultWinW/2,defaultWinH/2);
		

		myTimer.start();

		grassState = E_GRASS_STATE_OFF;

		activeObject = E_OBJ_CAMERA;

		extraRad = 0;
		lastTime = 0.0;

		

		


		
		mbDown=false;
		lbDown=false;
		rbDown=false;

		

		isFullScreen = false;
	    shadersAreLoaded = 0;
	    readyToRecompile = 0;

		programState = E_PS_IN_GAME;


		for (i = 0; i < E_OBJ_LENGTH; i++) {
			dynObjects.push_back(new DynObject());
		}

		dynObjects[E_OBJ_CAMERA]->init(0, 0, maxBoundsInPixels.getIZ()/2, 0, 0, 0, false, false, NULL );

		for (i = E_OBJ_LIGHT0; i < E_OBJ_LENGTH; i++) {
			dynObjects[i]->init(1024+i*256,1024+i*256,2048, 255,255,255,     true, true, &(dynObjects[E_OBJ_CAMERA]->pos) );
		}
		
		dynObjects[E_OBJ_FOG]->init(-512,-512,-512,   0,0,255,     true, true, &(dynObjects[E_OBJ_CAMERA]->pos) );
		dynObjects[E_OBJ_CUTAWAY]->init(1024-256,1024-256,2048,   0,255,0,     true, true, &(dynObjects[E_OBJ_CAMERA]->pos) );


	    cameraZoom = 1.0f;
	    targetZoom = 1.0f;

	    mouseX = 0.0f;
	    mouseY = 0.0f;
	    mouseXUp = 0.0f;
	    mouseYUp = 0.0f;




	    

	    //createSliceList(visPageSizeInPixels);
		createVTList();
		createGrassList(grassSpacing);


		//// GL WIDGET START ////
		
		frameCount = 0;
		changesMade = false;
		forceGetPD = false;
		bufferInvalid = false;
		mapInvalid = true;
		notQuit = true;
		timerNotSet = true;
		screenWidth = defaultWinW;
		screenHeight = defaultWinH;
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


	    

	    fboStrings.push_back("palFBO");
	    fboStrings.push_back("worldSpaceFBO");

	    fboStrings.push_back("pagesFBO");
	    fboStrings.push_back("waterFBO");

	    fboStrings.push_back("grassFBO");
	    fboStrings.push_back("geomFBO");
	    fboStrings.push_back("combineFBO");
	    fboStrings.push_back("combineFBOWithWater");
	    fboStrings.push_back("combineFBOSpare");
	    fboStrings.push_back("noiseFBO");
	    fboStrings.push_back("resultFBO0");
	    fboStrings.push_back("resultFBO1");

	    fboStrings.push_back("volGenFBO128");
	    fboStrings.push_back("volGenFBO64");
	    fboStrings.push_back("volGenFBO32");
	    fboStrings.push_back("volGenFBO16");

	    fboStrings.push_back("waveFBO");

	    fboStrings.push_back("swapFBO0");
	    fboStrings.push_back("swapFBO1");
	    fboStrings.push_back("swapFBOLin0");
	    fboStrings.push_back("swapFBOLin1");

	    fboStrings.push_back("swapFBOBLin0");
	    fboStrings.push_back("swapFBOBLin1");

	    fboStrings.push_back("swapFBOLinHalf0");
	    fboStrings.push_back("swapFBOLinHalf1");

	    //fboStrings.push_back("swapFBO32Lin0");
	    //fboStrings.push_back("swapFBO32Lin1");

	    //fboStrings.push_back("terrainFBO");

	    fboStrings.push_back("cityFBO");
	    fboStrings.push_back("hmFBO");
	    fboStrings.push_back("hmFBOLinear");
	    fboStrings.push_back("simplexFBO");
	    


	    shaderStrings.push_back("RoadShader");
	    shaderStrings.push_back("SkeletonShader");
	    shaderStrings.push_back("DilateShader");
	    shaderStrings.push_back("TerrainMix");
	    shaderStrings.push_back("Simplex2D");
	    shaderStrings.push_back("TopoShader");
	    shaderStrings.push_back("WaveHeightShader");
	    shaderStrings.push_back("WaterShader");
	    shaderStrings.push_back("WaterShaderCombine");
	    shaderStrings.push_back("CopyShader");
	    shaderStrings.push_back("CopyShader2");
	    shaderStrings.push_back("NoiseShader");
	    shaderStrings.push_back("MapBorderShader");
	    shaderStrings.push_back("WorldSpaceShader");
	    shaderStrings.push_back("BlitShader");
	    shaderStrings.push_back("PreLightingShader");
	    shaderStrings.push_back("HBlurShader");
	    shaderStrings.push_back("VBlurShader");
	    shaderStrings.push_back("DownScaleShader");
	    shaderStrings.push_back("RadiosityShader");
	    shaderStrings.push_back("RadiosityCombineShader");
	    shaderStrings.push_back("FogShader");
	    shaderStrings.push_back("GeomShader");
	    shaderStrings.push_back("PreGrassShader");
	    shaderStrings.push_back("GrassShader");
	    shaderStrings.push_back("CombineShader");
	    shaderStrings.push_back("GenerateVolume");
	    shaderStrings.push_back("RenderVolume");
	    //shaderStrings.push_back("RenderVolumeSlice");
	    //shaderStrings.push_back("LightingShader");
	    //shaderStrings.push_back("OutlineShader");
	    //shaderStrings.push_back("WaterShaderNormals");

	    

	    shaderTextureIDs.push_back("Texture0");
	    shaderTextureIDs.push_back("Texture1");
	    shaderTextureIDs.push_back("Texture2");
	    shaderTextureIDs.push_back("Texture3");
	    shaderTextureIDs.push_back("Texture4");
	    shaderTextureIDs.push_back("Texture5");
	    shaderTextureIDs.push_back("Texture6");
	    shaderTextureIDs.push_back("Texture7");
	    shaderTextureIDs.push_back("Texture8");
	    shaderTextureIDs.push_back("Texture9");

	    //setupLookups();

	    for (i = 0; i < shaderStrings.size(); i++) {
	        shaderMap.insert(  pair<string,Shader*>(shaderStrings[i], NULL)  );
	    }
	    doShaderRefresh();

	    //fboSize = 512;
	    //bufsPerFBO = 2;

	    for (i = 0; i < fboStrings.size(); i++) {
	        fboMap.insert(  pair<string, FBOSet*>(fboStrings[i], new FBOSet())  );
	    }

	    //init(int _numBufs, int _width, int _height, int _bytesPerChannel, bool hasDepth, int filterEnum, int clampEnum);

	    fboMap["worldSpaceFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false);
	    fboMap["palFBO"]->init(1, palWidth, palHeight, 1, false, GL_LINEAR);

	    fboMap["pagesFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["waterFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["grassFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["geomFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, true);
	    fboMap["combineFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["combineFBOWithWater"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["combineFBOSpare"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["noiseFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST, GL_REPEAT);
	    fboMap["resultFBO0"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    fboMap["resultFBO1"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    //fboMap["resultFBO2"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false);
	    
	    

	    fboMap["volGenFBO16"]->init(1, 16, 16*16, 1, false);
	    fboMap["volGenFBO32"]->init(1, 32, 32*32, 1, false);
	    fboMap["volGenFBO64"]->init(1, 64, 64*64, 1, false);
	    fboMap["volGenFBO128"]->init(1, 128, 128*128, 1, false);

	    fboMap["waveFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);

	    fboMap["swapFBOLin0"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);
	   	fboMap["swapFBOLin1"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);

	   	fboMap["swapFBOBLin0"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);
	   	fboMap["swapFBOBLin1"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);

	   	fboMap["swapFBOLinHalf0"]->init(1, bufferDim.getIX()/2, bufferDim.getIY()/2, 1, false, GL_LINEAR);
	   	fboMap["swapFBOLinHalf1"]->init(1, bufferDim.getIX()/2, bufferDim.getIY()/2, 1, false, GL_LINEAR);

	   	//fboMap["swapFBO32Lin0"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_LINEAR);
	   	//fboMap["swapFBO32Lin1"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_LINEAR);

	    
	   	//fboMap["terrainFBO"]->init(1, newPitch*iNodeDivsPerLot, newPitch*iNodeDivsPerLot, 2, false, GL_NEAREST, GL_REPEAT);

	    fboMap["cityFBO"]->init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
	    fboMap["hmFBO"]->init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
	    fboMap["hmFBOLinear"]->init(1, newPitch, newPitch, 1, false, GL_LINEAR, GL_REPEAT);
	    fboMap["simplexFBO"]->init(1, newPitch, newPitch, 1, false, GL_LINEAR, GL_REPEAT);
	    fboMap["swapFBO0"]->init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
	    fboMap["swapFBO1"]->init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);



	    
	    //fboMap["terrainFBO"]->init(1, newPitch*iNodeDivsPerLot, newPitch*iNodeDivsPerLot, 2, false, GL_NEAREST, GL_REPEAT);
	    /*
		(	GLenum target,
		 	GLint level,
		 	GLint internalFormat,
		 	GLsizei width,
		 	GLsizei height,
		 	GLint border,
		 	GLenum format,
		 	GLenum type,
		 	const GLvoid * data);

	    */
	    glBindTexture(GL_TEXTURE_2D,terrainID);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newPitch*iNodeDivsPerLot, newPitch*iNodeDivsPerLot, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, 0);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    // glClearColor(0.0,0.0,0.0,0.0);
	    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glBindTexture(GL_TEXTURE_2D,0);

	    




	    loadAllData();


	    gw = new GameWorld();
	    gw->init(this);


	    
		
	    
	    
	    popTrace();



	}
void Singleton::createVoroVolume ()
                                {


		

		int i, j, k, m;
		int totLen = voroSize;
		float fTotLen = (float)totLen;
		int ind = 0;
		uint tmp;
		float fx, fy, fz;
		uint randOff[3];
		float ijkVals[3];

		const float RAND_MOD[9] = {
			3456.0f, 5965.0f, 45684.0f,
			4564.0f, 1234.0f, 6789.0f,
			4567.0f, 67893.0f, 13245.0f
		};

		float totLenO4 = totLen/4;
		float totLen3O4 = (totLen*3)/4;
		float fSimp;
		float heightThresh;
		float testVal;


		int iVolumeSize = voroSize*voroSize*voroSize;

		volData = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volData[i] = 0;
		}

		volDataLinear = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volDataLinear[i] = (255<<24)|(255<<16)|(255<<8)|(0);
		}


		for (j = 0; j < totLen; j++) {

			ijkVals[1] = (float)j;

			fy = (j);

			for (i = 0; i < totLen; i++) {

				ijkVals[0] = (float)i;

				fx = (i);
				
				for (k = 0; k < totLen; k++) {

					ijkVals[2] = (float)k;
					fz = (k);
					ind = k*totLen*totLen + j*totLen + i;


					for (m = 0; m < 3; m++) {
						fSimp = simplexScaledNoise(
																	1.0f, //octaves
																	1.0f, //persistence (amount added in each successive generation)
																	1.0f/4.0, //scale (frequency)
																	0.0f,
																	1.0f,
																	fx+RAND_MOD[m*3+0],
																	fy+RAND_MOD[m*3+1],
																	fz+RAND_MOD[m*3+2]
																);

						fSimp = clampfZO(fSimp)*255.0;
						randOff[m] = fSimp;

					}

					volData[ind] = (0)|(randOff[2]<<16)|(randOff[1]<<8)|randOff[0];
					volDataLinear[ind] = volData[ind];

					
					
					
					

					
				}
			}
		}


		glBindTexture(GL_TEXTURE_3D,voroID);
			glTexSubImage3D(
				GL_TEXTURE_3D,
				0,
				
				0,
				0,
				0,

				voroSize,
				voroSize,
				voroSize,

				GL_RGBA,
				GL_UNSIGNED_BYTE,

				volData
			);

		glBindTexture(GL_TEXTURE_3D,0);
		glBindTexture(GL_TEXTURE_3D,voroIDLinear);
			glTexSubImage3D(
				GL_TEXTURE_3D,
				0,
				
				0,
				0,
				0,

				voroSize,
				voroSize,
				voroSize,

				GL_RGBA,
				GL_UNSIGNED_BYTE,

				volDataLinear
			);
		glBindTexture(GL_TEXTURE_3D,0);

	}
void Singleton::reorderIds ()
                          {
		int i;
		int j;

		int oidSize =  orderedIds.size();
		int oidSizeM1 =  oidSize-1;

		intPair id0;
		intPair id1;

		int tempId;

		int tot0;
		int tot1;

		bool doSwap;

		GamePageHolder* gp0;
		GamePageHolder* gp1;

		for (i = 0; i < oidSizeM1; i++) {
			for (j = i + 1; j < oidSize; j++ ) {
				
				id0 = orderedIds[i];
				id1 = orderedIds[j];

				if ( pairIsNeg(id0) || pairIsNeg(id1)) {

				}
				else {

					gp0 = gw->getHolderAtID(id0);
					gp1 = gw->getHolderAtID(id1);

					if (gp0 == NULL || gp1 == NULL) {

					}
					else {
						tot0 = gp0->offsetInHolders.getFZ();
						tot1 = gp1->offsetInHolders.getFZ();

						if (tot0 == tot1) {
							tot0 = gp0->offsetInHolders.getFY();
							tot1 = gp1->offsetInHolders.getFY();

							if (tot0 == tot1) {
								tot1 = gp1->offsetInHolders.getFX();
								tot0 = gp0->offsetInHolders.getFX();

								if (tot0 == tot1) {
									doSwap = false;
								}
								else {
									doSwap = tot0 > tot1;
								}
							}
							else {
								doSwap = tot0 > tot1;
							}
						}
						else {
							doSwap = tot0 > tot1;
						}

						if (doSwap) {
							orderedIds[i] = id1;
							orderedIds[j] = id0;
						}
						
						
					}
				}

				
			}
		}
	}
int Singleton::findFurthestHolderId ()
                                   {
		

		int longestInd = 0;
		int i;

		float longestDis = 0.0f;
		float testDis;

		FIVector4 tempVec;

		GamePageHolder* gp;
		GamePageHolder* bestGP = NULL;

		for (i = 0; i < holderPoolItems.size(); i++) {
			gp = gw->getHolderAtID(holderPoolItems[i]->usedByHolderId);

			if (gp == NULL) {

			}
			else {

				/*
				tempVec.copyFrom(&(gw->camHolderPos));
				tempVec.addXYZ( worldSizeInHolders.getIX(), worldSizeInHolders.getIY(), 0.0 );

				tempVec2.copyFrom(&(gw->camHolderPos));
				tempVec2.addXYZ( -worldSizeInHolders.getIX(), -worldSizeInHolders.getIY(), 0.0 );

				testDis1 = gp->offsetInHolders.distance( &(gw->camHolderPos) );
				testDis2 = gp->offsetInHolders.distance( &(tempVec) );
				testDis3 = gp->offsetInHolders.distance( &(tempVec2) );

				testDis = min(min(testDis1,testDis2),testDis3);

				*/

				tempVec.copyFrom(&(gw->camHolderPos));
				gp->offsetInHolders.wrapDistance(&tempVec,worldSizeInHolders.getIX());
				testDis = gp->offsetInHolders.distance( &(tempVec) );

				if (testDis > longestDis) {
					longestDis = testDis;
					longestInd = i;
					bestGP = gp;
				}
			}
		}

		//doTraceVecND("bestGP->offsetInHolders", &(bestGP->offsetInHolders) );
		//doTraceVecND("gw->camHolderPos", &(gw->camHolderPos) );

		return longestInd;
	}
int Singleton::requestPoolId (int blockID, int holderID)
                                                     { //int requestingHolderId

		
		int holderToFreeId;
		intPair usedByHolderId;
		
		int i;

		
		if (TOT_GPU_MEM_USAGE < MAX_GPU_MEM) { // && poolItemsCreated <= maxPooledRes) {
			holderPoolItems.push_back( new PooledResource() );
			holderPoolItems.back()->init(this);

			holderToFreeId = poolItemsCreated;
			holderPoolIds.push_front(holderToFreeId);
			orderedIds.push_back(intPair());
			orderedIds.back().v0 = blockID;
			orderedIds.back().v1 = holderID;
			poolItemsCreated++;

		}
		else {
			holderToFreeId = findFurthestHolderId();//holderPoolIds.back();
			usedByHolderId = holderPoolItems[holderToFreeId]->usedByHolderId;

			GamePageHolder* consumingHolder;

			if ( pairIsNeg(usedByHolderId) ) {
				// this pooledItem is already free 

			}
			else {
				// free this pooledItem from the holder that is consuming it and give it to the requesting holder

				consumingHolder = gw->getHolderAtID(usedByHolderId);

				if (consumingHolder == NULL) {
					// holder was deleted already
				}
				else {
					consumingHolder->unbindGPUResources();
				}
				

			}



			for (i = 0; i < orderedIds.size(); i++) {
				if ( pairIsEqual(orderedIds[i],usedByHolderId) ) {
					orderedIds[i].v0 = blockID;
					orderedIds[i].v1 = holderID;
					break;
				}
			}


			holderPoolIds.remove(holderToFreeId);
			holderPoolIds.push_front(holderToFreeId);
		}


		holderPoolItems[holderToFreeId]->usedByHolderId.v0 = blockID;
		holderPoolItems[holderToFreeId]->usedByHolderId.v1 = holderID;

		reorderIds();

		return holderToFreeId;

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
void Singleton::createGrassList (int spacing)
                                          {

		int i;
		int j;

		float fi;
		float fj;

		float tcx;
		float tcy;


		int iMax = bufferDim.getIX()/spacing;
		int jMax = bufferDim.getIY()/spacing;

		float fiMax = (float)iMax;
		float fjMax = (float)jMax;
		float heightMod;

		grassTris = glGenLists(1);
		

		glNewList(grassTris, GL_COMPILE);

		//glBegin(GL_TRIANGLES);
		glBegin(GL_QUADS);

		//glNormal3f(0, 0, 1);

		

		for (j = jMax-1; j >= 0; j -= 1) {
			fj = ((float)(j*2-jMax) + 1.0f)*8.0f/fjMax;
			tcy = fj;//(fj + 1.0f)/2.0f;
			for (i = 0; i < iMax; i += 1) {
				fi = ((float)(i*2-iMax) + 1.0f)*8.0f/fiMax;
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
void Singleton::drawCrossHairs (FIVector4 originVec, float radius)
                                                               {
		FIVector4 minV;
		FIVector4 maxV;

		float xm;
		float ym;
		float zm;

		float maxRad = 4096.0f;

		int i;
		for (i = 0; i < 3; i++) {

			switch (i) {
				case 0:
					xm = maxRad;
					ym = radius;
					zm = radius;
				break;
				case 1:
					xm = radius;
					ym = maxRad;
					zm = radius;
				break;
				case 2:
					xm = radius;
					ym = radius;
					zm = maxRad;
				break;
			}

			minV.setFXYZ(
				originVec.getFX()-xm,
				originVec.getFY()-ym,
				originVec.getFZ()-zm
			);

			maxV.setFXYZ(
				originVec.getFX()+xm,
				originVec.getFY()+ym,
				originVec.getFZ()+zm
			);

			drawBox(&minV,&maxV);
		}

		
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

		drawBox(&minV,&maxV);
	}
void Singleton::drawBoxUp (FIVector4 originVec, float radiusX, float radiusY, float diamZ)
                                                                                       {
		FIVector4 minV;
		FIVector4 maxV;

		minV.setFXYZ(
			originVec.getFX()-radiusX,
			originVec.getFY()-radiusY,
			originVec.getFZ()
		);

		maxV.setFXYZ(
			originVec.getFX()+radiusX,
			originVec.getFY()+radiusY,
			originVec.getFZ()+diamZ
		);

		drawBox(&minV,&maxV);
	}
void Singleton::drawBox (FIVector4 * minVec, FIVector4 * maxVec)
                                                       {


		float minX = minVec->getFX();
		float minY = minVec->getFY();
		float minZ = minVec->getFZ();

		float maxX = maxVec->getFX();
		float maxY = maxVec->getFY();
		float maxZ = maxVec->getFZ();


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

			//glColor4f(backfaceX[i], backfaceY[i], backfaceZ[i], 1.0f);

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
	            shaderMap[ shaderStrings[i] ] = NULL;
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
	            setShaderTexture(i+offset,fbos->fbos[i].color_tex);
	        }
	    }
	}
void Singleton::unsampleFBODirect (FBOSet * fbos, int offset)
                                                           {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = fbos->numBufs - 1; i >= 0; i--) {
	            setShaderTexture(i+offset,0);
	        }
	    }
	}
void Singleton::bindFBODirect (FBOSet * fbos, int doClear)
                                                          {
	    setMatrices(fbos->width,fbos->height);
	    fbos->bind(doClear);
	    currentFBOResolutionX = fbos->width;
	    currentFBOResolutionY = fbos->height;
	}
void Singleton::sampleFBO (string fboName, int offset, int swapFlag)
                                                                      {
	    FBOSet* fbos;
	    
		if (swapFlag == -1) {
			fbos = fboMap[fboName];
		}
		else {

			if (swapFlag == 0) {
				fbos = fboMap[fboName + "0"];
			}
			else {
				fbos = fboMap[fboName + "1"];
			}
			
		}
	    
	    if (fbos) {
	    	sampleFBODirect(fbos,offset);
	    }
	    else {
	    	doTrace("sampleFBO: Invalid FBO Name");
	    }

	    
	}
void Singleton::unsampleFBO (string fboName, int offset, int swapFlag)
                                                                        {
	    
		FBOSet* fbos;

		if (swapFlag == -1) {
			fbos = fboMap[fboName];
		}
		else {

			if (swapFlag == 0) {
				fbos = fboMap[fboName + "0"];
			}
			else {
				fbos = fboMap[fboName + "1"];
			}
			
		}

		if (fbos) {
			unsampleFBODirect(fbos,offset);
		}
		else {
			doTrace("unsampleFBO: Invalid FBO Name");
		}
	    
	}
FBOSet * Singleton::getFBOSet (string fboName)
                                           {
		return fboMap[fboName];
	}
FBOWrapper * Singleton::getFBOWrapper (string fboName, int offset)
                                                               {
		FBOSet* fbos = fboMap[fboName];
		return fbos->getFBOWrapper(offset);
	}
void Singleton::copyFBO (string src, string dest)
                                              {
		bindShader("CopyShader");
		bindFBO(dest);
		sampleFBO(src, 0);
		drawFSQuad(1.0f);
		unsampleFBO(src, 0);
		unbindFBO();
		unbindShader();
	}
void Singleton::copyFBO2 (string src, string dest)
                                               {
		bindShader("CopyShader2");
		bindFBO(dest);
		sampleFBO(src, 0);
		drawFSQuad(1.0f);
		unsampleFBO(src, 0);
		unbindFBO();
		unbindShader();
	}
void Singleton::bindFBO (string fboName, int swapFlag)
                                                      {
	    
		FBOSet* fbos;

		if (swapFlag == -1) {
			fbos = fboMap[fboName];
		}
		else {

			if (swapFlag == 0) {
				fbos = fboMap[fboName + "1"];
			}
			else {
				fbos = fboMap[fboName + "0"];
			}
			
		}

		if (fbos) {
			bindFBODirect(fbos);
		}
		else {
			doTrace("bindFBO: Invalid FBO Name");
		}

	   
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
	        curShaderPtr = shaderMap[curShader];
	        curShaderPtr->bind();
	    }
	    
	}
void Singleton::unbindShader ()
                            {
	    if (shadersAreLoaded) {
	        curShaderPtr->unbind();
	    }
	}
void Singleton::setShaderArrayfVec3 (string paramName, float * x, int count)
                                                                        {
		curShaderPtr->setShaderArrayfVec3(paramName, x, count);
	}
void Singleton::setShaderArrayfVec4 (string paramName, float * x, int count)
                                                                        {
		curShaderPtr->setShaderArrayfVec4(paramName, x, count);
	}
void Singleton::setShaderArray (string paramName, float * x, int count)
                                                                   {
		curShaderPtr->setShaderArray(paramName, x, count);
	}
void Singleton::setShaderFloat (string paramName, float x)
                                                       {
		curShaderPtr->setShaderFloat(paramName, x);
	}
void Singleton::setShaderInt (string paramName, int x)
                                                   {
		curShaderPtr->setShaderInt(paramName, x);
	}
void Singleton::setShaderfVec2 (string paramName, FIVector4 * v)
                                                            {
		curShaderPtr->setShaderfVec2(paramName, v);
	}
void Singleton::setShaderVec2 (string paramName, float x, float y)
                                                               {
	    curShaderPtr->setShaderVec2(paramName, x, y);
	}
void Singleton::setShaderVec3 (string paramName, float x, float y, float z)
                                                                        {
	    curShaderPtr->setShaderVec3(paramName, x, y, z);
	}
void Singleton::setShaderfVec3 (string paramName, FIVector4 * v)
                                                            {
	    curShaderPtr->setShaderfVec3(paramName, v);
	}
void Singleton::setShaderVec4 (string paramName, float x, float y, float z, float w)
                                                                                 {
	    curShaderPtr->setShaderVec4(paramName, x, y, z, w);
	}
void Singleton::setShaderfVec4 (string paramName, FIVector4 * v)
                                                            {
	    curShaderPtr->setShaderfVec4(paramName, v);
	}
void Singleton::setShaderTexture (int multitexNumber, uint texID)
                                                              {
	    if (shadersAreLoaded) {
	        glActiveTexture(GL_TEXTURE0 + multitexNumber);
	        glBindTexture(GL_TEXTURE_2D, texID);
	        curShaderPtr->setShaderInt(shaderTextureIDs[multitexNumber] ,multitexNumber);
	    }
	}
void Singleton::setShaderTexture3D (int multitexNumber, uint texID)
                                                               {
	    if (shadersAreLoaded) {
	        glActiveTexture(GL_TEXTURE0 + multitexNumber);
	        glBindTexture(GL_TEXTURE_3D, texID);
	        curShaderPtr->setShaderInt(shaderTextureIDs[multitexNumber],multitexNumber);
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
void Singleton::drawQuadBounds (float fx1, float fy1, float fx2, float fy2)
                                                                        {
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
void Singleton::drawFBO (string fboName, int ind, float zoom, int swapFlag)
                                                                           {
	    if (swapFlag == -1) {
	    	drawFBOOffset(fboName, ind, 0.0f, 0.0f, zoom);
	    }
	    else {
	    	if (swapFlag == 0) {
	    		drawFBOOffset(fboName+"1", ind, 0.0f, 0.0f, zoom);
	    	}
	    	else {
	    		drawFBOOffset(fboName+"0", ind, 0.0f, 0.0f, zoom);
	    	}
	    	
	    }
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

	    if (fbos) {
	    	drawFBOOffsetDirect(fbos, ind, xOff, yOff, zoom);
	    }
	    else {
	    	doTrace("drawFBOOffsetDirect: Invalid FBO Name");
	    }
	    
	}
float Singleton::getSeaLevelInPixels ()
                                    {
		return ( ((float)gw->seaLevel)/255.0)*heightmapMax;
	}
float Singleton::getCityHeight ()
                              {
		return ( ((float)gw->seaLevel + 20.0)/255.0)*heightmapMax;
	}
float Singleton::getHeightAtPixelPos (float x, float y, bool ignoreCity)
                                                                             {
		FBOWrapper* fbow;
		float xc;
		float yc;

		int channel = 0;

		float testHeight;
		float testHeight0;
		float testHeight1;

		if (mapInvalid) {

			return 0.0f;

		}
		else {
			FBOWrapper* fbow = getFBOWrapper("hmFBO",0);

			xc = (x / maxBoundsInPixels.getFX()) * ((float)fbow->width);
			yc = (y / maxBoundsInPixels.getFY()) * ((float)fbow->height);

			testHeight0 =
				fbow->getPixelAtLinear((xc*mapFreqs.getFX()), (yc*mapFreqs.getFX()), channel)*mapAmps.getFX() +
				fbow->getPixelAtLinear((xc*mapFreqs.getFY()), (yc*mapFreqs.getFY()), channel)*mapAmps.getFY() +
				fbow->getPixelAtLinear((xc*mapFreqs.getFZ()), (yc*mapFreqs.getFZ()), channel)*mapAmps.getFZ() +
				fbow->getPixelAtLinear((xc*mapFreqs.getFW()), (yc*mapFreqs.getFW()), channel)*mapAmps.getFW();


			return testHeight0 * heightmapMax;


			// if (ignoreCity) {
			// 	return testHeight0 * heightmapMax;
			// }

			// testHeight1 = ((float)gw->seaLevel + 20.0)/255.0;



			// if ( fbow->getPixelAtLinear((xc*mapFreqs.getFX()), (yc*mapFreqs.getFX()), gw->blockChannel) > 0.0f) {
			// 	testHeight = testHeight1;
			// }
			// else {
			// 	testHeight = testHeight0;
			// }
			
			
			// return testHeight * heightmapMax;

		}

		

	}
void Singleton::moveCamera (FIVector4 * modXYZ)
                                           {
		wsBufferInvalid = true;

		int i;

		FIVector4* cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);

		cameraPos->addXYZRef(modXYZ);


		modXYZ->setFZ(0.0f);

		if (cameraPos->getFX() > maxBoundsInPixels.getFX()/2.0) {
			cameraPos->setFX( cameraPos->getFX() - maxBoundsInPixels.getFX() );
		}
		if (cameraPos->getFX() < -maxBoundsInPixels.getFX()/2.0) {
			cameraPos->setFX( cameraPos->getFX() + maxBoundsInPixels.getFX() );
		}
		if (cameraPos->getFY() > maxBoundsInPixels.getFY()/2.0) {
			cameraPos->setFY( cameraPos->getFY() - maxBoundsInPixels.getFY() );
		}
		if (cameraPos->getFY() < -maxBoundsInPixels.getFY()/2.0) {
			cameraPos->setFY( cameraPos->getFY() + maxBoundsInPixels.getFY() );
		}
		

		
		for (i = 1; i < E_OBJ_LENGTH; i++) {

			if (dynObjects[i]->isRelative) {
				dynObjects[i]->pos.copyFrom(cameraPos);
				dynObjects[i]->pos.addXYZRef( &(dynObjects[i]->posRel) );
			}
			
		}

		isPanning = true;
	}
void Singleton::moveObject (float dx, float dy, float zoom)
                                                        {


		

		float dxZoom = dx*2.0/zoom;
		float dyZoom = dy*2.0/zoom;

		float grassWidth;
		float grassHeight;

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
				grassWidth = grassWH.getFX();
				grassHeight = grassWH.getFY();
				

				grassHeight -= modXYZ.getFZ()/10000.0f;
				grassWidth -= modXYZ.getFX()/10000.0f;


				if (grassHeight < 0.0f) {
					grassHeight = 0.0f;
				}
				if (grassWidth < 0.0f) {
					grassWidth = 0.0f;
				}

				grassWH.setFXYZ(grassWidth,grassHeight,0.0);

			}

			
		}
		else {


			if (mouseState == E_MOUSE_STATE_BRUSH) {
				doDefault = true;
			}
			else {

				if (activeObject == E_OBJ_CAMERA) {
					doDefault = true;
				}
				else {
					dynObjects[activeObject]->posRel.addXYZRef(&modXYZ, -1.0f);
					dynObjects[activeObject]->pos.addXYZRef(&modXYZ, -1.0f);
					//activeObjectPos.setFXYZRef( &(dynObjects[activeObject]->pos) );
				}
			}

			if (doDefault) {
				moveCamera(&modXYZ);
			}


			
		}


		

		

	}
void Singleton::toggleFullScreen ()
                                {
		isFullScreen = !isFullScreen;
		if (isFullScreen) {
			glutFullScreen();
		}
		else {
			glutPositionWindow(250,50);
		}
	}
void Singleton::updateVG ()
                        {
		if (volGenFBOX < 16) {
			volGenFBOX = 16;
		}
		if (volGenFBOX > 128) {
			volGenFBOX = 128;
		}

		doTraceND("volGenFBOX: ", i__s(volGenFBOX));

		switch(volGenFBOX) {
			case 16:
				curVGString = "volGenFBO16";
				maxChanges = 32;
			break;
			case 32:
				curVGString = "volGenFBO32";
				maxChanges = 32;
			break;
			case 64:
				curVGString = "volGenFBO64";
				maxChanges = 16;
			break;
			case 128:
				curVGString = "volGenFBO128";
				maxChanges = 16;
			break;

		}
	}
void Singleton::setCameraToElevation ()
                                    {

		FIVector4* cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);

		cameraPos->setFZ( getHeightAtPixelPos(cameraPos->getFX(), cameraPos->getFY()) + maxW*visPageSizeInPixels/2.0f );
		bufferInvalid = true;
		changesMade = true;
		wsBufferInvalid = true;
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

		int holderCount;

		changesMade = false;


		int enCounter;

		bool restartGen = false;

		doTrace(i__s(key) );


		if (key == 17) {
			glutLeaveMainLoop();
		}

		switch(key) {

			
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

			case 19: //ctrl-s
				//saveAllData();
				cout << "Use s key in web editor to save\n";					
			break;

			case 15: //ctrl-o
				loadAllData();		
				
			break;

			case '[':
				volGenFBOX /= 2;

				updateVG();				

			break;
			case ']':
				volGenFBOX *= 2;

				updateVG();
			break;


			case 'e':
				setCameraToElevation();
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

			case 27: // esc
				std::exit(0);
			break;

			case 'b':
				radiosityOn = !radiosityOn;
			break;

			case 'r':
				doShaderRefresh();
				bufferInvalid = true;

				cout << "Shaders Refreshed\n";
				//mapInvalid = true;
			break;

			case 'G':
				gridOn = 1.0-gridOn;
				cout << "Grid On: " << gridOn << "\n";
				bufferInvalid = true;
				changesMade = true;
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


			case 'p':
				//cout << "curZoom " << cameraZoom << "\n";
				toggleFullScreen();
			break;

			case 'd':
				directPass = 1.0f-directPass;
			break;

			case 'h':
				waterOn = !waterOn;

				if (MAX_LAYERS == 1) {
					waterOn = false;
				}

				cout << "waterOn " << waterOn << "\n";
			break;

			case 't':
				traceOn = true;
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
				//isBare = !isBare;
				restartGen = true;
			break;

			case 'f':
				fogOn = 1.0-fogOn;
				cout << "fog on " << fogOn << "\n";
			break;

			case 'm':
				
				/*
				mapInvalid = true;
				bufferInvalid = true;
				changesMade = true;
				wsBufferInvalid = true;
				forceGetPD = true;
				*/

				//showMap = !showMap;
				
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


			case 'v':
				if (selectedGeom == NULL) {

				}
				else {
					selectedGeom->visible = !(selectedGeom->visible);
					
					holderCount = gw->getHoldersInGeom(selectedGeom);
					gw->refreshHoldersInList(true); //holderCount <= 12
					gw->holdersToRefresh.clear();

					bufferInvalid = true;
					changesMade = true;



				}
			break;


			
			default:
				
			break;
		}

		if (restartGen) {
			doShaderRefresh();
			gw->actionOnHolders(E_HOLDER_ACTION_RESET);
			bufferInvalid = true;
			changesMade = true;
		}

	}
void Singleton::keyboardDown (unsigned char key, int _x, int _y)
                                                             {


		int x = _x/scaleFactor;
		int y = _y/scaleFactor;
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
GameGeom * Singleton::findNearestGeom (FIVector4 * testPoint)
                                                        {
		GameBlock* curBlock;

		int i;
		int j;
		int k;
		int ii;
		int jj;
		float bestDis;
		float curDis;

		bestDis = 99999.0f;
		GameGeom* resGeom = NULL;
		
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				ii = i + gw->camBlockPos.getIX();
				jj = j + gw->camBlockPos.getIY();

				curBlock = gw->getBlockAtCoords(ii, jj, false);

				if (curBlock == NULL) {

				}
				else {
					for (k = 0; k < curBlock->gameGeom.size(); k++) {
						if (
							testPoint->inBoundsXYZ(
								curBlock->gameGeom[k]->getVisMinInPixelsT(),
								curBlock->gameGeom[k]->getVisMaxInPixelsT()
							)
						) {
							curDis =
								curBlock->gameGeom[k]->getVisMinInPixelsT()->distance(testPoint) +
								curBlock->gameGeom[k]->getVisMaxInPixelsT()->distance(testPoint);

							if (curDis < bestDis) {
								bestDis = curDis;
								resGeom = curBlock->gameGeom[k];
							}

						}
					}
				}	
			}
		}

		return resGeom;
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

			getPixData(&mouseMovePD, x, y);

			if (mouseState == E_MOUSE_STATE_BRUSH || mouseState == E_MOUSE_STATE_MEASURE  ) {
				
				gw->modifyUnit(&mouseMovePD, E_BRUSH_MOVE);
			}


			//////////////


			
			highlightedGeom = findNearestGeom(&mouseMovePD);


			//////////////


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

		FIVector4* cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);

		dMod.copyFrom(wc);
		dMod.addXYZRef(cameraPos, -1.0);

		float x1 = (dMod.getFX()-dMod.getFY());
		float y1 = (-(dMod.getFX()/2.0f) + -(dMod.getFY()/2.0f) + dMod.getFZ());
		float myZoom = std::min(1.0f, cameraZoom);

		x1 = x1*(myZoom)/bufferDim.getFX();
		y1 = y1*(myZoom)/bufferDim.getFY();

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

		GameBlock* curBlock;

		int res;
		int i;
		int j;
		int k;
		int ii;
		int jj;
		int bestInd;
		float bestDis;
		float curDis;

		float wheelDelta = 0.0;
		bool mbClicked = false;
		bool rbClicked = false;
		bool lbClicked = false;

		int mouseMovingStart;
		
		FIVector4 testVec;
		FIVector4 oneVec;
		oneVec.setFXYZ(1.0f,1.0f,1.0f);
		oneVec.normalize();


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



		if (rbClicked) {
			if (lbDown) {

			}
			else {
				isPanning = false;
			}
			
		}
		if (lbClicked) {
			if (rbDown) {

			}
			else {
				isPanning = false;
			}
			
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

				muTime = myTimer.getElapsedTimeInMilliSec();

				mouseEnd.setIXY(x,y);
				//mouseVel.copyFrom(&mouseEnd);
				//mouseVel.addXYZRef(&mouseMoving, -1.0f);

				if ( (activeObject == E_OBJ_CAMERA) && (muTime-mdTime > 300.0f) ) {
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


				//mouseVel.multXYZ( clampf(1.0f-(muTime-mdTime)/1000.0f, 0.1f, 1.0f)/cameraZoom );

				if (shiftDown()) {

				}
				else {
					




					activeObject = E_OBJ_CAMERA;
					wsBufferInvalid = true;
					getPixData(&mouseUpPD, x, y);

					
					selectedGeom = findNearestGeom(&mouseUpPD);

					if (selectedGeom == NULL) {

					}
					else {
						if (
							(selectedGeom->buildingType == E_BT_DOOR) ||
							(selectedGeom->buildingType == E_BT_WINDOW) 

						) {
							gw->getHoldersInGeom(selectedGeom);
							selectedGeom->applyTransform(selectedGeom->rotDir,false);
							gw->getHoldersInGeom(selectedGeom);
							gw->refreshHoldersInList(true); //holderCount <= 12
							gw->holdersToRefresh.clear();

							bufferInvalid = true;
							changesMade = true;
							wsBufferInvalid = true;
						}
						
					}



					


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
					mdTime = myTimer.getElapsedTimeInMilliSec();
					mouseStart.setIXY(x,y);

					getPixData(&mouseDownPD, x, y);






					






					if (mouseDownPD.getIW() == 1) {

						

						bestInd = 0;
						bestDis = -99999.0f;
						for (i = 1; i < dynObjects.size(); i++) {
							

							//curDis = dynObjects[i]->pos.distance(&mouseDownPD);
							
							testVec.copyFrom( &(dynObjects[i]->pos) );
							testVec.addXYZRef(&mouseDownPD,-1.0f);

							if (
								(testVec.getFX() < 0.0f) &&
								(testVec.getFY() < 0.0f) &&
								(testVec.getFZ() < 0.0f)

							) {
								testVec.multXYZ(-1.0f);
							}

							testVec.normalize();

							curDis = testVec.dot(&oneVec);


							if (curDis > bestDis) {
								bestDis = curDis;
								bestInd = i;
							}
						}

						activeObject = (E_OBJ)(bestInd);



						diskOn = 1.0f;
						//activeObjectPos.setFXYZRef(&(dynObjects[activeObject]->pos));

					}
					else {
						activeObject = E_OBJ_CAMERA;
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
void Singleton::processB64 (charArr * sourceBuffer, charArr * saveBuffer)
                                                                    {
		
		char* buf = sourceBuffer->data;
		int len = sourceBuffer->size;

		if (saveBuffer != &nullBuffer) {
			if (saveBuffer->data != NULL) {
				delete[] saveBuffer->data;
				saveBuffer->data = NULL;
			}
			saveBuffer->data = new char[len];
			strncpy(saveBuffer->data, buf, len);
			saveBuffer->size = len;
		}


		FBOSet* fbos = fboMap["palFBO"];


		//unsigned char* resultImage = new unsigned char[256*256*4];

		membuf inBuffer(sourceBuffer->data, sourceBuffer->data + sourceBuffer->size);
		std::istream myIS(&inBuffer);
		Poco::Base64Decoder b64in(myIS);



		std::ostringstream oss;
		oss << b64in.rdbuf();

		std::string strConst = oss.str();
		const char* inString = strConst.c_str();


		lodepng_decode32(&resultImage, &palWidth, &palHeight, (unsigned char*)inString, strConst.size() );

		fbos->copyFromMem(0, resultImage);

	}
bool Singleton::processJSON (charArr * sourceBuffer, charArr * saveBuffer)
                                                                     {
		
		doTraceND("processJSON()");

		char* buf = sourceBuffer->data;
		int len = sourceBuffer->size;
		JSONValue *jsonVal = NULL;

		if (saveBuffer != &nullBuffer) {
			if (saveBuffer->data != NULL) {
				delete[] saveBuffer->data;
				saveBuffer->data = NULL;
			}
			saveBuffer->data = new char[len];
			strncpy(saveBuffer->data,buf,len);
			saveBuffer->size = len;
		}
		
		doTraceND("Begin JSON::Parse()");

		if (buf == NULL) {
			doTraceND("buf is NULL");
			return false;
		}
		else {
			doTraceND("buf is not NULL");
			jsonVal = JSON::Parse(buf);
		}


		doTraceND("End JSON::Parse()");


		if (jsonVal == NULL) {
			doTraceND("Invalid JSON\n\n");
			return false;	
		}
		else {
			doTraceND("");
			doTraceND("Valid JSON");
			doTraceND("");


			if (rootObj != NULL) {
				delete rootObj;
				rootObj = NULL;
			}

			doTraceND("rootObj is now NULL");


			rootObj = jsonVal;
			jsonVal = NULL;

			//doTrace( "JSON VAL", f__s(jsonVal->Child(L"x")->number_value) , "\n\n" );
			return true;
		}
		
		
	}
void Singleton::loadAllData ()
                           {
		if ( loadFile("..\\data\\lastJSONBuffer.js", &lastJSONBuffer) ) {
			processJSON(&lastJSONBuffer,&nullBuffer);
		}

		if ( loadFile("..\\data\\lastImageBuffer.txt", &lastImageBuffer) ) {
			processB64(&lastImageBuffer,&nullBuffer);
		}

		bufferInvalid = true;
	}
void Singleton::saveAllData ()
                           {
		saveFile("..\\data\\lastJSONBuffer.js", &lastJSONBuffer);
		saveFile("..\\data\\lastImageBuffer.txt", &lastImageBuffer);
	}
bool Singleton::loadFile (char * fileName, charArr * dest)
                                                     {
		doTraceND("Loading: ", fileName);
		
		if (dest == NULL) {
			doTraceND("Null Data");
			return false;
		}

		std::ifstream infile (fileName, std::ifstream::in);
		

		if ( ! infile.is_open() ){
			doTraceND("Could Not Open File For Loading");
			return false;
		}

		// get size of file
		infile.seekg (0,infile.end);
		long size = infile.tellg();
		infile.seekg (0, infile.beg);


		dest->size = size;

		if (dest->data != NULL) {
			delete[] dest->data;
			dest->data = NULL;
		}

		dest->data = new char[size];

		// read content of infile
		infile.read (dest->data,size);

		if ( infile.bad() ){
			doTraceND("Could Not Load From File");
			infile.close();
			return false;
		}

		infile.close();

		doTraceND("Load Successful");

		return true;
	}
bool Singleton::saveFile (char * fileName, charArr * source)
                                                       {

		if (source->data == NULL) {
			doTraceND("Null Data");
			return false;
		}

		std::ofstream outfile (fileName, std::ofstream::out);
		
		if ( ! outfile.is_open() ){
			doTraceND("Could Not Open File For Saving");
			return false;
		}

		outfile.write (source->data,source->size);

		if ( outfile.bad() ){
			doTraceND("Could Not Save To File");
			outfile.close();
			return false;
		}

		outfile.close();

		doTraceND("Save Successful");

		return true;
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
					if ( processJSON( &(myWS->recBuffer), &lastJSONBuffer  ) ) {
						saveAllData();

					}
				}
				else {
					processB64(  &(myWS->recBuffer), &lastImageBuffer  );
					
				}

				bufferInvalid = true;

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

			// if ( fMouseVel < 2.0f ) {
			// 	mouseVel.setFXY(0.0f,0.0f);
			// 	isPanning = false;
			// }
			// else {
			// 	isPanning = true;


			// 	panMod.copyFrom(&lastModXYZ);
			// 	panMod.multXYZ(fMouseVel/16.0f);
			// 	moveCamera(&panMod);
			// }


			dz = (targetZoom-cameraZoom)/(16.0f);

			/*
			if (abs(dz) < 0.0001) {
				dz = 0.0f;
			}
			*/

			/*
			if (cameraZoom > 8.0f) {
				cameraZoom = 8.0f;
			}
			if (cameraZoom < 1.0f/8.0f) {
				cameraZoom = 1.0f/8.0f;
			}
			*/
			
			cameraZoom += dz;

			if ( ( abs(dz)/cameraZoom < 0.0001 ) && (isZooming)) {
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

				if (traceOn) {
					traceOn = false;
					TRACE_ON = true;
				}
				if (updateLock) {

				}
				else {
					gw->update();
				}
				

				TRACE_ON = false;

				changesMade = false;
				bufferInvalid = false;
			}
		}

		if (firstRun) {
			toggleFullScreen();
		}

		firstRun = false;		

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
 
