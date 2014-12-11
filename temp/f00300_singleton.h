// f00300_singleton.h
//

#include "f00300_singleton.e"
#define LZZ_INLINE inline
bool Singleton::CompareStruct::operator () (string const & first, string const & second)
                                                                       {
	        return first.compare(second) < 0;//first.size() < second.size();
	    }
Singleton::Singleton ()
        {
		
		fboMap.clear();
		shaderMap.clear();
		soundMap.clear();
		styleSheetMap.clear();
		
		mainGUI = NULL;
		gw = NULL;
		myWS = NULL;
	}
void Singleton::init (int _defaultWinW, int _defaultWinH, int _scaleFactor, WebSocketServer * _myWS)
        {

		pushTrace("Singleton init");
		int i;
		int j;
		
		internalJSON["shaderParams"].jv = NULL;
		
		for (i = 0; i < EML_LENGTH; i++) {
			music[i] = new GameMusic();
			music[i]->init("..\\data\\music\\"+musicStrings[i]+".ogg");
			music[i]->setLoop(true);
		}
		
		
		for (i = 0; i < E_CT_LENGTH; i++) {
			isInteractiveEnt[i] = false;
		}
		
		float tempf;
		
		
		cdMap[0] = E_CD_EMPTY;
		for (i = 1; i < 256; i++) {
			cdMap[i] = E_CD_SOLID;
		}
		cdMap[32] = E_CD_WATER;
		
		isInteractiveEnt[E_CT_WINDOW] = true;
		isInteractiveEnt[E_CT_DOOR] = true;
		isInteractiveEnt[E_CT_LANTERN] = true;
		
		pathfindingOn = false;
		isMacro = false;
		entOn = false;
		autoScroll = false;
		cavesOn = false;
		bakeParamsOn = true;
		dragging = false;
		mouseMoved = false;
		forceRestart = false;
		changingGenVal = false;
		readyForRestart = false;
		hitGUI = false;
		guiLock = false;
		guiDirty = true;
		
		currentActor = NULL;
		mapComp = NULL;
		mainMenu = NULL;
		ddMenu = NULL;
		selectedEnt = NULL;
		highlightedEnt = NULL;
		draggingMap = false;
		
		FOV = 45.0f;
		float focalLength = 1.0f / tan(FOV / 2.0f);

		totalPointCount = 0;
		
		activeNode = NULL;
		selectedNode = NULL;
		lastSelNode = NULL;
		
		curOrgId = 0;
		
		rootObjJS = NULL;
		guiRootJS = NULL;
		rbStack = NULL;
		rbHeightStack = NULL;

		multiLights = false;
		numDynLights = E_OBJ_LENGTH - E_OBJ_LIGHT0;
		updateMultiLights();

		maxLayerOver = -1;

		iNumSteps = 16;
		currentStep = 0;

		srand(time(NULL));

		timeOfDay = 1.0f;
		targetTimeOfDay = 1.0f;
		
		guiSaveLoc = "..\\data\\saves\\save0\\compMap.txt";

		//invItems = loadBMP("..\\data\\invitems.bmp");
		imageHM0 = loadBMP("..\\data\\hm0.bmp");
		imageHM1 = loadBMP("..\\data\\hm1.bmp");
		cloudImage = loadBMP("..\\data\\clouds.bmp");

		//invItems->getTextureId(GL_NEAREST);
		imageHM0->getTextureId(GL_NEAREST);
		imageHM1->getTextureId(GL_NEAREST);
		cloudImage->getTextureId(GL_LINEAR);


		mapSampScale = 0.5f;
		//renderCount = 1.0;
		int newPitch = (imageHM0->width) * mapSampScale; //*2;

		numLights = MAX_LIGHTS;//min(MAX_LIGHTS,E_OBJ_LENGTH-E_OBJ_LIGHT0);


		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////


		initStyleSheet();

		curMoveSpeed = 0.0f;
		curMoveAccel = 0.0f;
		
		masterVolume = 0.0f;
		ambientVolume = 0.0f;
		guiVolume = 0.0f;
		musicVolume = 0.0f;
		fxVolume = 0.0f;

		applyToChildren = false;
		mirrorOn = true;
		//guiOn = false;
		bCtrlOld = false;
		bShiftOld = false;
		bCtrl = false;
		bShift = false;
		emptyVDNotReady = true;
		firstRun = true;
		waterOn = (MAX_LAYERS == 2);
		treesOn = true;
		rotOn = false;
		markerFound = false;
		doPageRender = true;
		
		
		
		/////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////
		
				
		// IMPORTANT: Maximum height must be less than 2^16, max world pitch must be less than 2^32
		
		// todo:
		
		// realtime cutaway based on green cube position
		// character editor load from json
		// floating trees
		// stacked lots?
		// WAS DOING:
		// replace worldspace fbo make xFBO have 3 targets instead of 2
		// edit: pack normal in 16, only use 2 targets
		
		// adjust camera for better loading, ray cast from cam pos to find nearest
		// chunk to load
		// flood fill from there until it hits blank chunk and stop
		
		
		
		
		
		moveTimer.start();
		
		
		
		
		
		// qqqqqq
		
		
		// TODO:
		// render chunks with offset to save precision
		// add in adjacent voxel information into rvvcubemap
		// fix lighting
		// load chunks better
		// cache chunks properly
		// transparency
		// clearing holders
		// move objects
		
		int bufferDiv = 2;
		int curProfile;
		heightOfNearPlane = 1.0f;
		scrollDiv = 2.0;
		
		msPerFrame = 8.0;
		
		
		volGenFBOX = 128; // MAX OF 128, DO NOT CHANGE THIS VALUE
		
		
		
		maxChangesInPages = 4;
		maxChangesInHolders = 1; 
		maxHInPages = 4;
		maxWInPages = 4;
		pixelsPerCell = 128;
		volGenSuperMod = 2;
		cellsPerPage = 4;
		cellsPerHolder = 8;
		volGenSuperRes = (volGenFBOX/volGenSuperMod);
		
		glEnable(GL_POINT_SPRITE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		
		
		//glPointSize(2.0);
		
		
		
		cdBuffer = new int[cellsPerPage*cellsPerPage*cellsPerPage*E_CD_LENGTH];

		
		// This var determines how high the z val can go,
		// but also increases load times for each block
		
		cellsPerLot = 64; // adjust this to make lots bigger
		
		cellsPerNodeXY = 16;
		cellsPerNodeZ = 8;
		blockSizeInLots = 4;
		unitsPerCell = 4; // ONE UNIT == ONE METER
		

		worldSizeInLots.setIXYZ(newPitch, newPitch, blockSizeInLots);
		worldSizeInCells.copyIntMult(&worldSizeInLots, cellsPerLot);
		blockSizeInCells = blockSizeInLots * cellsPerLot;
		holderSizeInPages = cellsPerHolder / cellsPerPage;
		holdersPerLot = cellsPerLot / cellsPerHolder;
		visPageSizeInUnits = cellsPerPage * unitsPerCell;
		bufferMult = 1.25f;
		bufferedPageSizeInUnits = visPageSizeInUnits * bufferMult;
		worldSizeInHolders.copyIntMult(&worldSizeInLots, holdersPerLot);
		blockSizeInHolders = blockSizeInLots * holdersPerLot;
		blockSizeInPages = blockSizeInHolders * holderSizeInPages;
		worldSizeInBlocks.copyIntDiv(&worldSizeInHolders, blockSizeInHolders);
		worldSizeInPages.copyIntMult(&worldSizeInHolders, holderSizeInPages);
		worldSizeInUnits.copyIntMult(&worldSizeInPages, visPageSizeInUnits);
		
		
		

		pixelsPerLot = pixelsPerCell * cellsPerLot;
		unitSizeInPixels = pixelsPerCell / unitsPerCell;
		blockSizeInPixels = pixelsPerCell * blockSizeInCells;
		worldSizeInPixels.copyIntMult(&worldSizeInCells, pixelsPerCell);
		
		visPageSizeInPixels = pixelsPerCell * cellsPerPage;
		holderSizeInPixels = pixelsPerCell * cellsPerHolder;
		
		
		// pageResolution = visPageSizeInPixels;//512; // height of one page in pixels
		
		
		holderResolution = holderSizeInPixels;
		
		
		voxelSizeInWC = ((float)visPageSizeInPixels)/((float)volGenSuperRes);
		
		
		
		
		
		lastMoveTime = 0.0;
		
		terDataTexScale = 1;
		//if (pixelsPerCell >= 128) {
			terDataTexScale = 2;
		//}
		
		terDataVisPitchXY = blockSizeInCells / cellsPerNodeXY;
		iNodeDivsPerLot = terDataVisPitchXY / blockSizeInLots;
		terDataBufAmount = iNodeDivsPerLot; // pad with one extra lot
		
		floorHeightInCells = (float)cellsPerNodeZ;
		roofHeightInCells = ((float)cellsPerNodeXY)/4.0f;
		wallRadInCells = ((float)cellsPerNodeXY)/4.0f;
		
		
		terDataBufPitchXY = terDataVisPitchXY + terDataBufAmount * 2;
		terDataBufPitchScaledXY = terDataBufPitchXY * terDataTexScale;
		
		terDataVisPitchZ = blockSizeInCells / cellsPerNodeZ;
		terDataBufPitchZ = terDataVisPitchZ + terDataBufAmount * 2;
		terDataBufPitchScaledZ = terDataBufPitchZ * terDataTexScale;

		terDataVisSize = terDataVisPitchXY * terDataVisPitchXY * terDataVisPitchZ;
		terDataBufSize = terDataBufPitchXY * terDataBufPitchXY * terDataBufPitchZ;
		terDataBufSizeScaled = terDataBufPitchScaledXY * terDataBufPitchScaledXY * terDataBufPitchScaledZ;


		terDataScaled = new uint[terDataBufSizeScaled];
		rbStack = new int[terDataBufSize];
		rbHeightStack = new int[terDataBufSize];


		worldSizeInTerData.setIXYZ(
			worldSizeInBlocks.getIX()*terDataVisPitchXY,
			worldSizeInBlocks.getIY()*terDataVisPitchXY,
			worldSizeInBlocks.getIZ()*terDataVisPitchZ
		);
		


		


		
		blockShift = intLogB2(blockSizeInHolders * blockSizeInHolders * blockSizeInHolders);
		holderSizeMB = ( ((float)MAX_LAYERS) * 2.0f * 4.0f * (float)(holderResolution * holderResolution)) / (1024.0f * 1024.0f);








		cout << "\n\n\n\n\n\n";



		// cout << "terDataBufPitchXY: " << terDataBufPitchXY << "\n";
		// cout << "terDataBufPitchZ: " << terDataBufPitchXY << "\n";
		// cout << "terDataVisPitchXY: " << terDataBufPitchXY << "\n";
		// cout << "terDataVisPitchZ: " << terDataBufPitchXY << "\n";
		
		cout << "cellsPerNodeXY: " << cellsPerNodeXY << "\n";
		cout << "cellsPerNodeZ: " << cellsPerNodeZ << "\n";
		
		cout << "blockSizeInHolders: " << blockSizeInHolders << "\n";
		cout << "blockSizeInPixels: " << blockSizeInPixels << "\n";
		cout << "terDataBufPitchScaledXY " << terDataBufPitchScaledXY << "\n";
		cout << "maxChangesInHolders: " << maxChangesInHolders << "\n";
		cout << "volGenFBOX: " << volGenFBOX << "\n";
		cout << "volGenFBOY: " << volGenFBOX *volGenFBOX << "\n";
		cout << "visPageSizeInPixels: " << visPageSizeInPixels << "\n";
		cout << "holderSizeInPixels: " << holderSizeInPixels << "\n";
		cout << "bufferMult: " << bufferMult << "\n";
		cout << "cellsPerLot: " << cellsPerLot << "\n";
		cout << "pixelsPerCell: " << pixelsPerCell << "\n";
		cout << "unitsPerCell: " << unitsPerCell << "\n";
		cout << "blockSizeInLots: " << blockSizeInLots << "\n";
		cout << "pixelsPerUnit: " << unitSizeInPixels << "\n";
		cout << "unitsPerLot: " << cellsPerLot*unitsPerCell << "\n";
		cout << "pixelsPerLot: " << pixelsPerLot << "\n";
		cout << "holderSizeInPages: " << holderSizeInPages << "\n";
		cout << "holdersPerLot: " << holdersPerLot << "\n";
		cout << "visPageSizeInUnits: " << visPageSizeInUnits << "\n";
		cout << "unitSizeInPixels: " << unitSizeInPixels << "\n";


		doTraceVecND("worldSizeInBlocks: ", &worldSizeInBlocks);
		doTraceVecND("worldSizeInLots: ", &worldSizeInLots);
		doTraceVecND("worldSizeInHolders: ", &worldSizeInHolders);
		doTraceVecND("worldSizeInPages: ", &worldSizeInPages);
		doTraceVecND("worldSizeInTerData: ", &worldSizeInTerData);

		GLint glQuery;
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &glQuery);
		cout << "MAX_UNIFORMS: " << glQuery << "\n";

		cout << "\n\n\n\n\n\n";


		mapFreqs.setFXYZW(1.0f, 15.0f, 37.0f, 83.0f);
		mapAmps.setFXYZW(8.0f/16.0f, 8.0f/16.0f, 8.0f/16.0f, 8.0f/16.0f); //0.0f, 0.0f, 0.0f);//


		gridSizeInPixels = pixelsPerCell;



		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		radiosityOn = true;
		testOn = false;
		updateLock = false;
		traceOn = false;
		gridOn = 0.0f;
		fogOn = 1.0f;
		cameraZoom = 1.0f;
		targetZoom = cameraZoom;
		



		int ccr = 0;
		int ccg = 0;
		int ccb = 0;

		doTraceVecND("worldSizeInPixels: ", &worldSizeInPixels);






		glGenTextures(1, &volId);
		glGenTextures(1, &volIdLinear);
		glGenTextures(1, &volIdEmpty);
		glGenTextures(1, &volIdEmptyLinear);

		int curFilter;

		for (i = 0; i < MAX_TER_TEX; i++)
		{

			terTextures[i].usedByBlockId = -1;
			terTextures[i].alreadyBound = false;


			// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, 0);

			glGenTextures(1, &(terTextures[i].texId));
			glBindTexture(GL_TEXTURE_3D, terTextures[i].texId);
			glTexImage3D(
				GL_TEXTURE_3D,
				0,
				GL_RGBA,
				terDataBufPitchScaledXY,
				terDataBufPitchScaledXY,
				terDataBufPitchScaledZ,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				0
			);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_3D, 0);

			TOT_GPU_MEM_USAGE += ((float)(terDataBufSizeScaled * 4)) / (1024.0f * 1024.0f);
		}



		for (i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				glBindTexture(GL_TEXTURE_3D, volId);
				break;
			case 1:
				glBindTexture(GL_TEXTURE_3D, volIdEmpty);
				break;
			case 2:
				glBindTexture(GL_TEXTURE_3D, volIdLinear);
				break;
			case 3:
				glBindTexture(GL_TEXTURE_3D, volIdEmptyLinear);
				break;
			}
			if (i < 2)
			{
				curFilter = GL_NEAREST;
			}
			else
			{
				curFilter = GL_LINEAR;
			}

			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, bufferedPageSizeInUnits, bufferedPageSizeInUnits, bufferedPageSizeInUnits, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, curFilter);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, curFilter);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_3D, 0);
		}

		
		paramArr = new float[4096];
		voroArr = new float[27 * 4];
		matCountArr = new float[256];
		paramArrMap = new float[4096];




		showMap = true;



		lastImageBuffer.data = NULL;
		lastImageBuffer.size = 0;

		lastJSONBuffer.data = NULL;
		lastJSONBuffer.size = 0;
		
		lastJSONBufferGUI.data = NULL;
		lastJSONBufferGUI.size = 0;
		

		nullBuffer.data = new char[1];
		nullBuffer.data[0] = '\0';
		nullBuffer.size = 0;



		palWidth = 256;
		palHeight = 256;

		resultImage = new unsigned char[palWidth * palHeight * 4];

		mouseMovingSize = 100;
		mouseMovingLoc = 0;
		mouseCount = 0;
		mouseMovingStepsBack = 20;
		mouseMoving = new FIVector4[mouseMovingSize];

		myWS = _myWS;

		

		isBare = true;




		defaultWinW = _defaultWinW / _scaleFactor;
		defaultWinH = _defaultWinH / _scaleFactor;
		scaleFactor = _scaleFactor;
		aspectRatio = ((float)_defaultWinW)/((float)_defaultWinH);
		
		guiWinW = _defaultWinW / UI_SCALE_FACTOR;
		guiWinH = _defaultWinH / UI_SCALE_FACTOR;

		origWinW = _defaultWinW;
		origWinH = _defaultWinH;

		curBrushRad = 4.0f;

		mouseState = E_MOUSE_STATE_MOVE;








		wsBufferInvalid = true;

		bufferDim.setIXY(defaultWinW, defaultWinH);
		bufferDimTarg.setIXY(defaultWinW, defaultWinH);
		
		bufferDimHalf.setIXY(defaultWinW / 2, defaultWinH / 2);
		bufferModDim.copyIntMult(&bufferDim,1);

		myTimer.start();
		//fpsTimer.start();


		activeObject = E_OBJ_CAMERA;

		extraRad = 0;
		lastTime = 0.0;







		mbDown = false;
		lbDown = false;
		rbDown = false;
		abDown = false;



		isFullScreen = false;
		shadersAreLoaded = 0;
		readyToRecompile = 0;

		programState = E_PS_IN_GAME;


		for (i = 0; i < E_OBJ_LENGTH; i++)
		{
			dynObjects.push_back(new DynObject());
		}

		dynObjects[E_OBJ_CAMERA]->init(0, 0, worldSizeInPixels.getIZ() / 2, 0, 0, 0, false, E_MT_NONE, NULL, 4.0f );

		for (i = E_OBJ_LIGHT0; i < E_OBJ_LENGTH; i++)
		{

			j = i - E_OBJ_LIGHT0;

			switch (j)
			{
			case 0:
				ccr = 255;
				ccg = 255;
				ccb = 255;
				break;

			case 1:
				ccr = 255;
				ccg = 0;
				ccb = 0;

				break;

			case 2:
				ccr = 255;
				ccg = 128;
				ccb = 0;

				break;

			case 3:
				ccr = 0;
				ccg = 255;
				ccb = 0;

				break;

			case 4:
				ccr = 0;
				ccg = 128;
				ccb = 255;

				break;
			case 5:
				ccr = 128;
				ccg = 0;
				ccb = 255;

				break;
			}

			if (i == E_OBJ_LIGHT0) {
				tempf = 4096.0f*pixelsPerCell;
			}
			else {
				tempf = 16.0f*pixelsPerCell;
			}

			dynObjects[i]->init(
				-2048 + i * 256,
				-2048 + i * 256,
				1024/2,
				ccr, ccg, ccb,
				true,
				E_MT_RELATIVE,
				&(dynObjects[E_OBJ_CAMERA]->pos),
				64.0f,
				tempf
			);



		}
		
		dynObjects[E_OBJ_LIGHT0]->moveType = E_MT_TRACKBALL;

		dynObjects[E_OBJ_FOG]->init(-1024*2, -1024*2, -1024/2,   0, 0, 255,     true, E_MT_RELATIVE, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		dynObjects[E_OBJ_CUTAWAY]->init(4096*4 - 256, 4096*4 - 256 + 2048, 4096*4,   0, 255, 0,     true, E_MT_RELATIVE, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		dynObjects[E_OBJ_HUMAN]->init(2048, 2048, -1024,   0, 255, 255,     true, E_MT_TRACKBALL, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );

		// dynObjects[E_OBJ_P0]->init(512-256,1024-256,2048,   128,0,0,    true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		// dynObjects[E_OBJ_P1]->init(512,1024,2048,      255,0,0,  true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		// dynObjects[E_OBJ_P2]->init(1024,512,2048,      0,255,255,  true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );



		mouseX = 0.0f;
		mouseY = 0.0f;
		mouseXUp = 0.0f;
		mouseYUp = 0.0f;






		//// GL WIDGET START ////

		frameCount = 0;
		forceGetPD = false;
		mapInvalid = true;
		notQuit = true;
		timerNotSet = true;
		screenWidth = defaultWinW;
		screenHeight = defaultWinH;
		mouseLeftDown = mouseRightDown = false;
		mouseX = mouseY = 0;
		myDelta = 0.0f;



		
		glDepthFunc(GL_LESS);
		
		
		glDisable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);
		
		
		fsqDL = glGenLists(1);
		glNewList(fsqDL, GL_COMPILE);
		drawFSQuadOffset(0.0f, 0.0f, 1.0f);
		glEndList();
		
		

		shaderStrings.push_back("PointShader");
		shaderStrings.push_back("GUIShader");
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
		//shaderStrings.push_back("WorldSpaceShader");
		shaderStrings.push_back("BlitPointShader");
		
		shaderStrings.push_back("PreLightingShader");
		shaderStrings.push_back("HDRShader");
		shaderStrings.push_back("PostLightingShader");
		shaderStrings.push_back("BlurShader");
		shaderStrings.push_back("DownScaleShader");
		shaderStrings.push_back("RadiosityShader");
		shaderStrings.push_back("RadiosityCombineShader");
		shaderStrings.push_back("FogShader");
		shaderStrings.push_back("GeomShader");
		shaderStrings.push_back("CombineShader");
		shaderStrings.push_back("GenerateVolume");
		shaderStrings.push_back("GenerateVolumeEnt");
		shaderStrings.push_back("RenderVolume");

		std::sort(shaderStrings.begin(), shaderStrings.end(), compareStruct);
		stringBuf.reserve(shaderStrings.size()*256);


		shaderTextureIds.push_back("Texture0");
		shaderTextureIds.push_back("Texture1");
		shaderTextureIds.push_back("Texture2");
		shaderTextureIds.push_back("Texture3");
		shaderTextureIds.push_back("Texture4");
		shaderTextureIds.push_back("Texture5");
		shaderTextureIds.push_back("Texture6");
		shaderTextureIds.push_back("Texture7");
		shaderTextureIds.push_back("Texture8");
		shaderTextureIds.push_back("Texture9");
		shaderTextureIds.push_back("Texture10");
		shaderTextureIds.push_back("Texture11");
		shaderTextureIds.push_back("Texture12");
		shaderTextureIds.push_back("Texture13");
		shaderTextureIds.push_back("Texture14");
		shaderTextureIds.push_back("Texture15");

		//setupLookups();

		for (i = 0; i < shaderStrings.size(); i++)
		{
			shaderMap.insert(  pair<string, Shader*>(shaderStrings[i], new Shader(this))  );
		}
		doShaderRefresh(false);
		
		int faceDim = 256;
		
		
		/*
		void init(
			int _numBufs,
			int _width,
			int _height,
			int _bytesPerChannel,
			bool _hasDepth,
			int filterEnum = GL_NEAREST,
			int clampEnum = GL_CLAMP_TO_EDGE,
			bool isMultisample = false
		) {
		*/
		
		for (i = 0; i < MAX_VG_FBOS; i++) {
			vgFBOArr[i].init(
				1,
				volGenFBOX,
				volGenFBOX * volGenFBOX,
				1,
				false
			);
		}
		
		for (i = 0; i < MAX_VGT_FBOS; i++) {
			vgtFBOArr[i].init(
				2,
				volGenSuperRes,
				volGenSuperRes * volGenSuperRes,
				1,
				false
			);
		}
		
		
		
		
		/////////////////////////
		/////////////////////////
		
		fogRed = 135.0f/255.0f;
		fogGreen = 160.0f/255.0f;
		fogBlue = 1.0f;
		lastx = 0;
		lasty = 0;
		isMoving = false;
		perspectiveOn = false;

		mainCamera = new GameCamera();
		mainCamera->init();

		for (i = 0; i < 256; i++) {
			keysPressed[i] = false;
		}

		keyMap[KEYMAP_UP] = 'a';
		keyMap[KEYMAP_DOWN] = 'z';
		keyMap[KEYMAP_FORWARD] = 'e';
		keyMap[KEYMAP_BACKWARD] = 'd';
		keyMap[KEYMAP_LEFT] = 's';
		keyMap[KEYMAP_RIGHT] = 'f';
		
		/////////////////////////
		/////////////////////////
		
		
		
		
		
		/* (
				int _numBufs,
				int _width,
				int _height,
				int _bytesPerChannel,
				bool _hasDepth,
				int filterEnum = GL_NEAREST,
				int clampEnum = GL_CLAMP_TO_EDGE,
				bool isMultisample = false
		)*/
		
		
		
			
		//TODO: get rid of these
		//GL_LINEAR
		//fboMap["worldSpaceFBO"].init(4, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_NEAREST);
		//fboMap["wavesWorldSpaceFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_NEAREST);
		
		//fboMap["worldSpaceBlurFBO0"].init(1, bufferDim.getIX()/2, bufferDim.getIY()/2, 4, false, GL_LINEAR);
		//fboMap["worldSpaceBlurFBO1"].init(1, bufferDim.getIX()/2, bufferDim.getIY()/2, 4, false, GL_LINEAR);
		
		fboMap["frontFaceFBO"].init(1, faceDim, faceDim, 4, false, GL_NEAREST);
		fboMap["backFaceFBO"].init(1, faceDim, faceDim, 4, false, GL_NEAREST);
		fboMap["frontFaceMapFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_NEAREST);
		fboMap["backFaceMapFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_NEAREST);
		fboMap["palFBO"].init(1, palWidth, palHeight, 1, false, GL_LINEAR);
		
		bool fboHasDepth = true;
		int numChannels = 4;
		int numMaps = 2;
		
		
		//fboMap["prelightFBO0"].init(1, bufferDimTarg.getIX()/4, bufferDimTarg.getIY()/4, 1, false, GL_LINEAR);
		//fboMap["prelightFBO1"].init(1, bufferDimTarg.getIX()/4, bufferDimTarg.getIY()/4, 1, false, GL_LINEAR);
		
		fboMap["prelightFBO"].init(2, bufferDimTarg.getIX(), bufferDimTarg.getIY(), 1, false, GL_LINEAR);
		
		
		fboMap["pagesTargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
		fboMap["waterTargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
		
		if (FILL_POINTS) {
			fboMap["pages2TargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
			fboMap["water2TargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
			
		}
		//fboMap["pages3TargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
		//fboMap["water3TargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
		fboMap["geomTargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, true);
		fboMap["combineWithWaterTargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
		
		fboMap["noiseFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST, GL_REPEAT);
		//fboMap["guiFBO"].init(1, guiWinW, guiWinH, 1, false, GL_LINEAR);
		fboMap["resultFBO0"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST);
		fboMap["resultFBO1"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST);
		//fboMap["volGenFBO0"].init(1, volGenFBOX, volGenFBOX * volGenFBOX, 1, false);
		//fboMap["volGenFBO1"].init(1, volGenFBOX, volGenFBOX * volGenFBOX, 1, false);
		fboMap["waveFBO"].init(1, bufferDim.getIX()/2, bufferDim.getIY()/2, 1, false, GL_LINEAR, GL_MIRRORED_REPEAT);
		fboMap["swapFBOLin0"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);
		fboMap["swapFBOLin1"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);
		fboMap["swapFBOBLin0"].init(1, bufferDim.getIX() / bufferDiv, bufferDim.getIY() / bufferDiv, 1, false, GL_LINEAR);
		fboMap["swapFBOBLin1"].init(1, bufferDim.getIX() / bufferDiv, bufferDim.getIY() / bufferDiv, 1, false, GL_LINEAR);
		fboMap["swapFBOLinHalf0"].init(1, bufferDim.getIX() / bufferDiv, bufferDim.getIY() / bufferDiv, 1, false, GL_LINEAR);
		fboMap["swapFBOLinHalf1"].init(1, bufferDim.getIX() / bufferDiv, bufferDim.getIY() / bufferDiv, 1, false, GL_LINEAR);
		fboMap["cityFBO"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["hmFBO"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["hmFBOLinear"].init(1, newPitch, newPitch, 1, false, GL_LINEAR, GL_REPEAT);
		fboMap["simplexFBO"].init(1, newPitch, newPitch, 1, false, GL_LINEAR, GL_REPEAT);
		fboMap["swapFBO0"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["swapFBO1"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);



		fontWrappers[EFW_ICONS] = new FontWrapper();
		fontWrappers[EFW_ICONS]->init(this, "icons", true, 2.0f, 0.0f);
		
		fontWrappers[EFW_TEXT] = new FontWrapper();
		fontWrappers[EFW_TEXT]->init(this, "arial_regular_24", false, 1.0f);

		mainGUI = new GameGUI();
		mainGUI->init(this);


		loadAllData();

		GamePlant::initAllPlants(this);
		for (i = 0; i < E_PT_LENGTH/2; i++) {
			gamePlants[i] = new GamePlant();
		}


		gw = new GameWorld();
		
		gpuPool = new PoolManager();
		entityPool = new PoolManager();
		//gpuPagePool = new PoolPageManager();
		//entityPagePool = new PoolPageManager();
		
		int cmMod = 1;
		
		
		gpuPool->init(this,false,false,holderResolution,holderResolution*cmMod);
		entityPool->init(this,true,false,holderResolution,holderResolution*cmMod);
		
		
		
		testHuman = new GameOrg();
		testHuman->init(this);
		
		
		
		
		gw->init(this);
		gw->initMap();
			
		
		music[EML_BIRDSONG0]->play();
		music[EML_CRICKETS0]->play();
		music[EML_OCEANWAVES0]->play();
		music[EML_UNDERWATER0]->play();
		
		loadGUI();
		loadGUIValues();
		
		if (bakeParamsOn) {
			doShaderRefresh(true);
		}
		

		doTraceND("GW DONE");


		popTrace();



	}
void Singleton::prepSound (string soundName)
                                         {
		if (soundMap.find( soundName ) == soundMap.end()) {
			soundMap.insert( pair<string, GameSound>(soundName, GameSound()) );
			soundMap[soundName].init("..\\data\\sounds\\"+soundName+".wav");
		}
	}
void Singleton::playSoundPosAndPitch (string soundName, FIVector4 * listenerPos, FIVector4 * soundPos, float variance, float volume)
          {
		
		
		FIVector4 res;
		
		prepSound(soundName);
		
		res.setFXYZRef(soundPos);
		res.addXYZRef(listenerPos,-1.0f);
		
		soundMap[soundName].setPitch(
			(fGenRand()-0.5f)*2.0*variance + 1.0f
		);
		
		soundMap[soundName].setPositionAndMinDis(
			res.getFX(),
			res.getFY(),
			res.getFZ(),
			32.0f*pixelsPerCell
		);
		soundMap[soundName].play(volume*fxVolume*masterVolume);
	}
void Singleton::playSound (string soundName, float volume)
                                                            {
		prepSound(soundName);
		soundMap[soundName].play(volume);
	}
void Singleton::playSoundEvent (char const * eventName, bool suppress)
                                                                          {
		
		if (suppress) {
			return;
		}
		
		string tempString;
		float volume;
		
		if (mainGUI != NULL) {
			if (mainGUI->isReady) {
				tempString = mainGUI->jvSounds->Child(eventName)->Child("name")->string_value;
				volume = mainGUI->jvSounds->Child(eventName)->Child("vol")->number_value;
				
				if (tempString.size() > 0) {
					playSound( tempString, masterVolume*volume*guiVolume );
				}
			}
		}
		
		
		
		
		
		
	}
void Singleton::dispatchEvent (int button, int state, float x, float y, UIComponent * comp, bool automated, bool preventRefresh)
          {
		
		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);
		
		changingGenVal = false;
		
		// if (guiLock) {
		// 	return;
		// }
		
		int i;
		int cbDataCount;
		JSONValue* cbData = NULL;
		
		bool doSPUpdate = false;
		bool mouseUpEvent = false;
		
		float curValue = comp->getValue();
		
		draggingMap = false;
		
		switch (comp->guiClass) {
			case E_GT_SLIDER:
			case E_GT_BUTTON:
			case E_GT_RADIO:
			case E_GT_DRAGPAD:
			
				switch(button) {
					case GLUT_NO_BUTTON:
					
						
						
						switch(state) {
							case GLUT_OVER:
								playSoundEvent("mouseOver",automated);
							break;
							case GLUT_OUT:
								playSoundEvent("mouseOut",automated);
							break;
							case GLUT_CHANGING:
								if (comp->guiClass == E_GT_DRAGPAD) {
									
									if (comp->uid.compare("map.mapHolder") == 0) {
										cameraPos->setFXYZRef(&baseCameraPos);
										//cout << x << " " << y << " " << comp->dragStart.x << " " << comp->dragStart.y << "\n";
										
										cameraPos->addXYZ(
											-(x - comp->dragStart.x)*worldSizeInPixels.getFX()/(cameraZoom*comp->resultDimInPixels.x),
											-(y - comp->dragStart.y)*worldSizeInPixels.getFY()/(cameraZoom*comp->resultDimInPixels.y),
											0.0f
										);
										draggingMap = true;
									}
									
								}
								else {
									doSPUpdate = true;
								}
								
							break;
						}
						
					break;
					case GLUT_LEFT_BUTTON:
						if (state == GLUT_DOWN) {
							playSoundEvent("mouseDown",automated);
							
							if (comp->uid.compare("map.mapHolder") == 0) {
								baseCameraPos.setFXYZRef(cameraPos);
							}
						}
						else {
							playSoundEvent("mouseUp",automated);
							
							if (comp->guiClass == E_GT_DRAGPAD) {
								
							}
							else {
								doSPUpdate = true;
								mouseUpEvent = true;
							}
							
							
						}
					break;
					
				}
			
				
			break;	
		}
		
		
		if (mouseUpEvent) {
			if (comp->uid.compare("placeEntity.actor") == 0) {
				gw->gameActors.push_back(baseEnt);
				currentActor = &(gw->gameActors.back());
				tempVec1.setIXYZ(2,2,3);
				currentActor->initActor(&lastCellPos,&tempVec1,pixelsPerCell);
				
				//cout << "placeActor\n";
			}
		}
		
		if (
			(state == GLUT_UP) &&
			(button == GLUT_LEFT_BUTTON) &&
			(comp->floatingChildren.size() == 0)
		) {
			ddMenu->visible = false;
			markerFound = false;
		}
		
		
		
				
		if (comp->uid.compare("$options.sound.masterVolume") == 0) {
			masterVolume = curValue;
		}
		else if (comp->uid.compare("$options.sound.ambientVolume") == 0) {
			ambientVolume = curValue;
		}
		else if (comp->uid.compare("$options.sound.guiVolume") == 0) {
			guiVolume = curValue;
		}
		else if (comp->uid.compare("$options.sound.musicVolume") == 0) {
			musicVolume = curValue;
		}
		else if (comp->uid.compare("$options.sound.fxVolume") == 0) {
			fxVolume = curValue;
		}
		else if (comp->uid.compare("$charEdit.entOn") == 0) {
			entOn = curValue != 0.0f;
		}
		else if (comp->uid.compare("$charEdit.pathfindingOn") == 0) {
			pathfindingOn = curValue != 0.0f;
		}
		else if (comp->uid.compare("$charEdit.lockPosition") == 0) {
			//pathfindingOn = curValue != 0.0f;
			if (curValue == 0.0f) {
				dynObjects[E_OBJ_HUMAN]->moveType = E_MT_TRACKBALL;
			}
			else {
				dynObjects[E_OBJ_HUMAN]->moveType = E_MT_NONE;
			}
		}
		else if (comp->uid.compare("$charEdit.mirrorOn") == 0) {
			mirrorOn = curValue != 0.0f;
		}
		else if (comp->uid.compare("$charEdit.applyToChildren") == 0) {
			applyToChildren = curValue != 0.0f;
		}
		
		
		if (comp->jvNodeNoTemplate != NULL) {
			if (comp->jvNodeNoTemplate->HasChild("callback")) {
				curCallback = comp->jvNodeNoTemplate->Child("callback")->string_value;
				
				if (comp->jvNodeNoTemplate->HasChild("callbackData")) {
					
					cbData = comp->jvNodeNoTemplate->Child("callbackData");
				
					// cbDataCount = cbData->CountChildren();
					// for (i = 0; i < cbDataCount; i++) {
					// 	cbDataStrings[i] = cbData->Child(i)->string_value;
					// }
				
				}
				
				if (doSPUpdate) {
					if (curCallback.compare("updateShaderParam") == 0) {
						
						if (cbData != NULL) {
							
							// cout <<
							// 	cbData->Child("paramName")->string_value <<
							// 	"\n";
							
							cbDataStrings[0] = cbData->Child("shaderName")->string_value;
							cbDataStrings[1] = cbData->Child("paramName")->string_value;
							
							
							
							shaderMap[cbDataStrings[0]]->paramMap[cbDataStrings[1]] = curValue;
							
							if (
								(cbDataStrings[0].compare("GenerateVolume") == 0)	||
								(cbDataStrings[0].compare("RenderVolume") == 0) ||
								(cbDataStrings[0].compare("RenderVolumeCubeMap") == 0)
							) {
								
								if (preventRefresh) {
									
								}
								else {
									if (mouseUpEvent) {
										forceRestart = true;
										readyForRestart = true;
									}
									else {
										changingGenVal = true;
										if (mouseMoved) {
											
											readyForRestart = true;
										}
									}
								}
								
								
								
							}
							
							
						}
					}
				}
				
				
			}
		}
		
		
		if (mouseMoved) {
			mouseMoved = false;
		}
	}
StyleSheet * Singleton::getNewStyleSheet (string ssName)
                                                    {
		styleSheetMap[ssName].init();
		return &(styleSheetMap[ssName]);
	}
void Singleton::initStyleSheet ()
                              {
		
		
		StyleSheet* mainSS = getNewStyleSheet("defaultSS");
		StyleSheetState* curState = &(mainSS->compStates[E_COMP_UP]);
		
		
		
		curState->setVal(E_SS_BGCOL0_R, 0.2f, 0.15f, 0.1f, 1.0f);
		curState->setVal(E_SS_BGCOL1_R, 0.1f, 0.075f, 0.05f, 0.5f);
		curState->setVal(E_SS_FGCOL0_R, 0.0f, 0.0f, 0.0f, 0.0f);
		curState->setVal(E_SS_FGCOL1_R, 0.0f, 0.0f, 0.0f, 0.0f);
		curState->setVal(E_SS_TGCOL0_R, 1.0f, 0.75f, 0.0f, 1.0f);
		curState->setVal(E_SS_TGCOL1_R, 1.0f, 0.25f, 0.0f, 0.5f);
		curState->setVal(E_SS_BGCOLTEXT0_R, 0.0f, 0.0f, 0.0f, 0.0f);
		curState->setVal(E_SS_BGCOLTEXT1_R, 0.0f, 0.0f, 0.0f, 0.0f);
		curState->setVal(E_SS_FGCOLTEXT0_R, 1.0f, 1.0f, 1.0f, 1.0f);
		curState->setVal(E_SS_FGCOLTEXT1_R, 1.0f, 0.8f, 0.7f, 1.0f);
		curState->setVal(E_SS_BDCOL_R, 1.0f, 1.0f, 1.0f, 0.5f);

		curState->props[E_SS_PADDING] = 4.0f;
		curState->props[E_SS_BORDER] = 1.0f;
		curState->props[E_SS_MARGIN] = 4.0f;
		curState->props[E_SS_CORNERRAD] = 4.0f;
		curState->props[E_SS_ROUNDNESS] = 1.0f;

		mainSS->compStates[E_COMP_OVER].copyFrom(& (mainSS->compStates[E_COMP_UP]) );
		mainSS->compStates[E_COMP_DOWN].copyFrom(& (mainSS->compStates[E_COMP_UP]) );

		
		
		curState = &(mainSS->compStates[E_COMP_OVER]);
		
		
		curState->setVal(E_SS_FGCOLTEXT0_R, 1.0f, 1.0f, 1.0f, 1.0f);
		curState->setVal(E_SS_FGCOLTEXT1_R, 1.0f, 1.0f, 1.0f, 1.0f);
		curState->props[E_SS_BDCOL_A] = 1.0f;
		
		curState->setVal(E_SS_BGCOL0_R, 0.1f, 0.075f, 0.05f, 1.0);
		curState->setVal(E_SS_BGCOL1_R, 0.05f, 0.025f, 0.0125f, 0.5);
		
		
		
		curState = &(mainSS->compStates[E_COMP_DOWN]);
		
		
		curState->setVal(E_SS_FGCOLTEXT0_R, 1.0f, 0.8f, 0.7f, 1.0f);
		curState->setVal(E_SS_FGCOLTEXT1_R, 1.0f, 1.0f, 1.0f, 1.0f);
		curState->props[E_SS_BDCOL_A] = 1.0f;
		curState->props[E_SS_BORDER] = 2.0f;

		curState->setVal(E_SS_BGCOL0_R, 0.05f, 0.025f, 0.0125f, 0.5f);	
		curState->setVal(E_SS_BGCOL1_R, 0.1f, 0.075f, 0.05f, 1.0f);	
		
		
		
		StyleSheet* tooltipSS = getNewStyleSheet("tooltipSS");
		tooltipSS->copyFrom(mainSS);
		
		curState = &(tooltipSS->compStates[E_COMP_UP]);
		curState->setVal(E_SS_BGCOL0_R, 0.0f, 0.0f, 0.0f, 1.0f);
		curState->setVal(E_SS_BGCOL1_R, 0.0f, 0.0f, 0.0f, 1.0f);
		curState->setVal(E_SS_FGCOLTEXT0_R, 1.0f, 0.75f, 0.0f, 1.0f);
		curState->setVal(E_SS_FGCOLTEXT1_R, 1.0f, 0.75f, 0.0f, 1.0f);
		curState->setVal(E_SS_BDCOL_R, 1.0f, 0.75f, 0.0f, 1.0f);
		tooltipSS->compStates[E_COMP_OVER].copyFrom(& (tooltipSS->compStates[E_COMP_UP]) );
		tooltipSS->compStates[E_COMP_DOWN].copyFrom(& (tooltipSS->compStates[E_COMP_UP]) );

		curState = &(tooltipSS->compStates[E_COMP_OVER]);
		curState = &(tooltipSS->compStates[E_COMP_DOWN]);
		
		
		
		
		StyleSheet* headerSS = getNewStyleSheet("headerSS");
		headerSS->copyFrom(mainSS);
		
		curState = &(headerSS->compStates[E_COMP_UP]);
		curState->setVal(E_SS_BGCOL0_R, 0.0f, 0.2f, 0.5f, 1.0f);
		curState->setVal(E_SS_BGCOL1_R, 0.0f, 0.4f, 0.8f, 0.5f);
		curState->setVal(E_SS_FGCOLTEXT0_R, 1.0f, 0.75f, 0.0f, 1.0f);
		curState->setVal(E_SS_FGCOLTEXT1_R, 1.0f, 0.75f, 0.0f, 1.0f);
		curState->setVal(E_SS_BDCOL_R, 1.0f, 0.75f, 0.0f, 1.0f);
		headerSS->compStates[E_COMP_OVER].copyFrom(& (headerSS->compStates[E_COMP_UP]) );
		headerSS->compStates[E_COMP_DOWN].copyFrom(& (headerSS->compStates[E_COMP_UP]) );

		curState = &(headerSS->compStates[E_COMP_OVER]);
		curState = &(headerSS->compStates[E_COMP_DOWN]);
		
		
		
	}
int Singleton::requestTerIndex (int requestingBlockId)
        {


		int i;

		bool foundFree = false;

		float bestDis;
		float testDis;
		int bestInd = 0;

		GameBlock *curBlock;

		// find if blockId already in use
		for (i = 0; i < MAX_TER_TEX; i++)
		{
			if (terTextures[i].usedByBlockId == requestingBlockId)
			{
				return i;
			}
		}

		// find if any blockIds not in use
		for (i = 0; i < MAX_TER_TEX; i++)
		{
			if (terTextures[i].usedByBlockId == -1)
			{
				bestInd = i;
				foundFree = true;
				break;
			}
		}

		if (foundFree)
		{

		}
		else
		{

			// find furthest blockId
			for (i = 0; i < MAX_TER_TEX; i++)
			{

				curBlock = gw->getBlockAtId(terTextures[i].usedByBlockId);
				testDis = curBlock->offsetInBlocks.distance(&(gw->camBlockPos));

				if (i == 0)
				{
					bestInd = 0;
					bestDis = testDis;
				}
				else
				{
					if (testDis > bestDis)
					{
						bestDis = testDis;
						bestInd = i;
					}
				}

			}

		}

		terTextures[bestInd].usedByBlockId = requestingBlockId;
		terTextures[bestInd].alreadyBound = false;
		return bestInd;


	}
void Singleton::qNormalizeAngle (int & angle)
        {
		while (angle < 0)
		{
			angle += 360 * 16;
		}
		while (angle > 360 * 16)
		{
			angle -= 360 * 16;
		}
	}
Singleton::~ Singleton ()
        {
		if (gw)
		{
			delete gw;
		}
	}
void Singleton::setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown)
        {

		if (isDown)
		{
			progActionsDown[ps * 256 + kc] = pa;
		}
		else
		{
			progActionsUp[ps * 256 + kc] = pa;
		}
	}
void Singleton::setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown)
        {
		int i;

		for (i = 0; i < E_PS_SIZE; i++)
		{
			setProgAction((eProgramState)i, kc, pa, isDown);
		}

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
		for (i = 0; i < 3; i++)
		{

			switch (i)
			{
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
				originVec.getFX() - xm,
				originVec.getFY() - ym,
				originVec.getFZ() - zm
			);

			maxV.setFXYZ(
				originVec.getFX() + xm,
				originVec.getFY() + ym,
				originVec.getFZ() + zm
			);

			drawBox(&minV, &maxV);
		}


	}
void Singleton::drawLine (FIVector4 * p0, FIVector4 * p1)
        {
		

		glBegin(GL_LINES);
		glMultiTexCoord3f(GL_TEXTURE0, 0.0f, 0.0f, 0.0f);
		glVertex3f(p0->getFX(), p0->getFY(), p0->getFZ());
		glMultiTexCoord3f(GL_TEXTURE0, 0.0f, 0.0f, 1.0f);
		glVertex3f(p1->getFX(), p1->getFY(), p1->getFZ());
		glEnd();
	}
void Singleton::drawCubeCentered (FIVector4 * originVec, float radius)
        {
		FIVector4 minV;
		FIVector4 maxV;

		minV.setFXYZ(
			originVec->getFX() - radius,
			originVec->getFY() - radius,
			originVec->getFZ() - radius
		);

		maxV.setFXYZ(
			originVec->getFX() + radius,
			originVec->getFY() + radius,
			originVec->getFZ() + radius
		);

		drawBox(&minV, &maxV);
	}
void Singleton::drawBoxUp (FIVector4 originVec, float radiusX, float radiusY, float diamZ)
        {
		FIVector4 minV;
		FIVector4 maxV;

		minV.setFXYZ(
			originVec.getFX() - radiusX,
			originVec.getFY() - radiusY,
			originVec.getFZ()
		);

		maxV.setFXYZ(
			originVec.getFX() + radiusX,
			originVec.getFY() + radiusY,
			originVec.getFZ() + diamZ
		);

		drawBox(&minV, &maxV);
	}
void Singleton::drawBox (FIVector4 * minVec, FIVector4 * maxVec, int faceFlag)
          {


		float minX = minVec->getFX();
		float minY = minVec->getFY();
		float minZ = minVec->getFZ();

		float maxX = maxVec->getFX();
		float maxY = maxVec->getFY();
		float maxZ = maxVec->getFZ();


		bool drawFront = false;
		bool drawBack = false;
		
		switch(faceFlag) {
			case 0:
				drawBack = true;
			break;
			case 1:
				drawFront = true;
			break;
			case 2:
				drawBack = true;
				drawFront = true;
			break;
		}

		glBegin(GL_QUADS);


		if (drawFront) {
			// front
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
			glVertex3f(minX, minY, maxZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
			glVertex3f(maxX, minY, maxZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
			glVertex3f(maxX, maxY, maxZ);
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
			glVertex3f(minX, maxY, maxZ);
			// right
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
			glVertex3f(maxX, minY, maxZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
			glVertex3f(maxX, minY, minZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
			glVertex3f(maxX, maxY, minZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
			glVertex3f(maxX, maxY, maxZ);
			// top
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
			glVertex3f(minX, maxY, maxZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
			glVertex3f(maxX, maxY, maxZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
			glVertex3f(maxX, maxY, minZ);
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
			glVertex3f(minX, maxY, minZ);
		}
		
		
		if (drawBack) {
			// back
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
			glVertex3f(minX, minY, minZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
			glVertex3f(maxX, minY, minZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
			glVertex3f(maxX, maxY, minZ);
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
			glVertex3f(minX, maxY, minZ);
			// left
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
			glVertex3f(minX, minY, maxZ);
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
			glVertex3f(minX, minY, minZ);
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
			glVertex3f(minX, maxY, minZ);
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
			glVertex3f(minX, maxY, maxZ);
			// bottom
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
			glVertex3f(minX, minY, maxZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
			glVertex3f(maxX, minY, maxZ);
			glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
			glVertex3f(maxX, minY, minZ);
			glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
			glVertex3f(minX, minY, minZ);
		}
		
		

		glEnd();
	}
void Singleton::doShaderRefresh (bool doBake)
        {

		LAST_COMPILE_ERROR = false;

		readyToRecompile = 0;

		int i;
		int j;
		
		Shader* curShader;


		for (i = 0; i < shaderStrings.size(); i++) {
			shaderMap[ shaderStrings[i] ]->init( "../src/glsl/" + shaderStrings[i] + ".c", doBake);
		}
		shadersAreLoaded = 1;
		readyToRecompile = 1;
		
		if (LAST_COMPILE_ERROR) {
			
		}
		else {
			
			// load saved data (if exists)
			// merge saved data with existing data (if exists)
			// save merged data to saved data
			
			
			stringBuf = "{\n\t\"params\":[\n";
			
			
			
			for (i = 0; i < shaderStrings.size(); i++) {
				curShader = shaderMap[ shaderStrings[i] ];
				
				std::sort(curShader->paramVec.begin(), curShader->paramVec.end(), compareStruct);
				
				for (j = 0; j < curShader->paramVec.size(); j++) {
					stringBuf.append("\t\t{");
					stringBuf.append("\"shaderName\":\""+shaderStrings[i]+"\",");
					stringBuf.append("\"paramName\":\""+curShader->paramVec[j]+"\",");
					stringBuf.append("\"uid\":\"$shaderParams."+shaderStrings[i]+"."+curShader->paramVec[j]+"\"");
					stringBuf.append("},\n");
				}
			}
			
			stringBuf[stringBuf.size()-2] = ' ';
			
			
			stringBuf.append("\t]\n}\n\n");
			
			// this should automatically clear the key
			// and deallocate existing entries
			//internalJSON.insertValue("shaderParams", JSON::Parse(stringBuf.c_str()));
			
			processJSONFromString(
				&stringBuf,
				&(internalJSON["shaderParams"].jv)
			);
			
			// cout << "stringBuf:\n\n";
			// cout << stringBuf;
			
			
		}
		

	}
void Singleton::setWH (int w, int h)
        {

		baseW = w;
		baseH = h;
	}
void Singleton::sampleFBODirect (FBOSet * fbos, int offset)
        {
		int i;
		if (shadersAreLoaded)
		{
			for (i = 0; i < fbos->numBufs; i++)
			{
				setShaderTexture(i + offset, fbos->fbos[i].color_tex);
			}
		}
	}
void Singleton::unsampleFBODirect (FBOSet * fbos, int offset)
        {
		int i;
		if (shadersAreLoaded)
		{
			for (i = fbos->numBufs - 1; i >= 0; i--)
			{
				setShaderTexture(i + offset, 0);
			}
		}
	}
void Singleton::bindFBODirect (FBOSet * fbos, int doClear)
        {
		setMatrices(fbos->width, fbos->height);
		
		fbos->bind(doClear);
		currentFBOResolutionX = fbos->width;
		currentFBOResolutionY = fbos->height;
	}
FBOSet * Singleton::getFBOByName (string & fboName)
                                              {
		
		if (fboMap.find( fboName ) == fboMap.end()) {
			cout << "invalid key" << fboName << "\n";
			exit(0);
		}
		
		return &(fboMap[fboName]);
	}
void Singleton::sampleFBO (string fboName, int offset, int swapFlag)
        {
		FBOSet *fbos;

		if (swapFlag == -1)
		{
			fbos = getFBOByName(fboName);//&(fboMap[fboName]);
		}
		else
		{

			if (swapFlag == 0)
			{
				fbos = getFBOByName(fboName + "0");
			}
			else
			{
				fbos = getFBOByName(fboName + "1");
			}

		}

		if (fbos)
		{
			sampleFBODirect(fbos, offset);
		}
		else
		{
			doTrace("sampleFBO: Invalid FBO Name");
		}


	}
void Singleton::unsampleFBO (string fboName, int offset, int swapFlag)
        {

		FBOSet *fbos;

		if (swapFlag == -1)
		{
			fbos = getFBOByName(fboName);
		}
		else
		{

			if (swapFlag == 0)
			{
				fbos = getFBOByName(fboName + "0");
			}
			else
			{
				fbos = getFBOByName(fboName + "1");
			}

		}

		if (fbos)
		{
			unsampleFBODirect(fbos, offset);
		}
		else
		{
			doTrace("unsampleFBO: Invalid FBO Name");
		}

	}
FBOSet * Singleton::getFBOSet (string fboName)
        {
		return getFBOByName(fboName);
	}
FBOWrapper * Singleton::getFBOWrapper (string fboName, int offset)
        {
		FBOSet *fbos = getFBOByName(fboName);
		return fbos->getFBOWrapper(offset);
	}
void Singleton::copyFBO (string src, string dest, int num)
        {
		bindShader("CopyShader");
		bindFBO(dest);
		//sampleFBO(src, 0);
		setShaderTexture(0, getFBOWrapper(src,num)->color_tex);
		drawFSQuad();
		unsampleFBO(src, 0);
		unbindFBO();
		unbindShader();
	}
void Singleton::copyFBO2 (string src, string dest)
        {
		bindShader("CopyShader2");
		bindFBO(dest);
		sampleFBO(src, 0);
		drawFSQuad();
		unsampleFBO(src, 0);
		unbindFBO();
		unbindShader();
	}
void Singleton::bindFBO (string fboName, int swapFlag)
        {

		FBOSet *fbos;

		if (swapFlag == -1)
		{
			fbos = getFBOByName(fboName);
		}
		else
		{

			if (swapFlag == 0)
			{
				fbos = getFBOByName(fboName + "1");
			}
			else
			{
				fbos = getFBOByName(fboName + "0");
			}

		}

		if (fbos)
		{
			bindFBODirect(fbos);
		}
		else
		{
			doTrace("bindFBO: Invalid FBO Name");
		}


	}
void Singleton::unbindFBO ()
        {
		
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		setMatrices(baseW, baseH);

	}
void Singleton::bindShader (string shaderName)
        {
		
		int i;
		int totSize;

		if (shadersAreLoaded)
		{
			curShader = shaderName;
			curShaderPtr = shaderMap[curShader];
			curShaderPtr->bind();
			
			totSize = curShaderPtr->paramVec.size();
			
			if (bakeParamsOn) {
				
			}
			else {
				for (i = 0; i < totSize; i++) {
					
					// if (curShaderPtr->paramVec[i].compare("lightColBNight")) {
					// 	cout << curShaderPtr->paramMap[curShaderPtr->paramVec[i]] << "\n";
					// }
					
					setShaderFloat(
						curShaderPtr->paramVec[i],
						curShaderPtr->paramMap[curShaderPtr->paramVec[i]]
					);
				}
			}
			
		}

	}
void Singleton::unbindShader ()
        {
		if (shadersAreLoaded)
		{
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
void Singleton::setShaderMatrix4x4 (string paramName, float * x, int count)
        {
		curShaderPtr->setShaderMatrix4x4(paramName, x, count);
	}
void Singleton::setShaderArray (string paramName, float * x, int count)
        {
		curShaderPtr->setShaderArray(paramName, x, count);
	}
GLint Singleton::getShaderLoc (string paramName)
                                             {
		return curShaderPtr->getShaderLoc(paramName);
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
void Singleton::setShaderFloatUB (string paramName, float x)
        {
		curShaderPtr->setShaderFloatUB(paramName, x);
	}
void Singleton::setShaderfVec4UB (string paramName, FIVector4 * v)
        {
		curShaderPtr->setShaderfVec4UB(paramName, v);
	}
void Singleton::updateUniformBlock (int ubIndex, int ubDataSize)
        {
		curShaderPtr->updateUniformBlock(ubIndex, ubDataSize);
	}
void Singleton::invalidateUniformBlock (int ubIndex)
        {
		curShaderPtr->invalidateUniformBlock(ubIndex);
	}
void Singleton::beginUniformBlock (int ubIndex)
        {
		curShaderPtr->beginUniformBlock(ubIndex);
	}
bool Singleton::wasUpdatedUniformBlock (int ubIndex)
        {

		return curShaderPtr->wasUpdatedUniformBlock(ubIndex);

	}
void Singleton::setShaderTexture (int multitexNumber, uint texId)
        {
		if (shadersAreLoaded)
		{
			glActiveTexture(GL_TEXTURE0 + multitexNumber);
			glBindTexture(GL_TEXTURE_2D, texId);
			curShaderPtr->setShaderInt(shaderTextureIds[multitexNumber] , multitexNumber);
		}
	}
void Singleton::setShaderTexture3D (int multitexNumber, uint texId)
        {
		if (shadersAreLoaded)
		{
			glActiveTexture(GL_TEXTURE0 + multitexNumber);
			glBindTexture(GL_TEXTURE_3D, texId);
			curShaderPtr->setShaderInt(shaderTextureIds[multitexNumber], multitexNumber);
		}
	}
bool Singleton::shiftDown ()
        {
		return (bool)(glutGetModifiers()&GLUT_ACTIVE_SHIFT);
	}
bool Singleton::ctrlDown ()
        {
		return (bool)(glutGetModifiers()&GLUT_ACTIVE_CTRL);
	}
bool Singleton::altDown ()
        {
		return (bool)(glutGetModifiers()&GLUT_ACTIVE_ALT);
	}
void Singleton::drawQuadBounds (float fx1, float fy1, float fx2, float fy2, float fz)
          {
		//glColor4f(1, 1, 1, 1);
		//glNormal3f(0, 0, 1);
		
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(fx1, fy1, fz);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(fx2, fy1, fz);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(fx2, fy2, fz);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(fx1, fy2, fz);

		glEnd();
	}
void Singleton::drawFSQuad ()
        {
		glCallList(fsqDL);
	}
void Singleton::drawFSQuadOffset (float xOff, float yOff, float zm)
        {
		float fx1 = (xOff - 1.0f) * zm;
		float fy1 = (yOff - 1.0f) * zm;
		float fx2 = (xOff + 1.0f) * zm;
		float fy2 = (yOff + 1.0f) * zm;

		glBegin(GL_QUADS);
		//glColor4f(1, 1, 1, 1);
		//glNormal3f(0, 0, 1);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(fx1, fy1, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(fx2, fy1, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(fx2, fy2, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(fx1, fy2, 0.0f);

		glEnd();
	}
void Singleton::drawFBO (string fboName, int ind, float zm, int swapFlag)
        {
		if (swapFlag == -1)
		{
			drawFBOOffset(fboName, ind, 0.0f, 0.0f, zm);
		}
		else
		{
			if (swapFlag == 0)
			{
				drawFBOOffset(fboName + "1", ind, 0.0f, 0.0f, zm);
			}
			else
			{
				drawFBOOffset(fboName + "0", ind, 0.0f, 0.0f, zm);
			}

		}
	}
void Singleton::drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zm)
        {

		glBindTexture(GL_TEXTURE_2D, fbos->fbos[ind].color_tex);
		//glClearColor(0.2,0.2,0.2,0.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawFSQuadOffset(xOff, yOff, zm);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
void Singleton::drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zm)
        {
		FBOSet *fbos = getFBOByName(fboName);

		if (fbos)
		{
			drawFBOOffsetDirect(fbos, ind, xOff, yOff, zm);
		}
		else
		{
			doTrace("drawFBOOffsetDirect: Invalid FBO Name");
		}

	}
float Singleton::getTerHeightScaled (float val)
        {
		return mixf(0.125f,0.875f,val)*worldSizeInPixels.getFZ();
		
	}
float Singleton::getSLNormalized ()
        {
		return (((float)gw->seaLevel) / 255.0f);
	}
float Singleton::getSLInPixels ()
        {
		return getSLNormalized()*worldSizeInPixels.getFZ() + 2.5f*pixelsPerCell;
	}
float Singleton::getHeightAtPixelPos (float x, float y, bool dd)
        {
		FBOWrapper *fbow;
		float xc;
		float yc;

		int channel = 0;

		float testHeight;
		
		float v0;
		float v1;
		float v2;
		float v3;
		
		if (mapInvalid)
		{

			cout << "mapInvalid\n";
			return 0.0f;

		}
		else
		{
			FBOWrapper *fbow = getFBOWrapper("hmFBO", 0);

			xc = (x / worldSizeInPixels.getFX()) * ((float)fbow->width);
			yc = (y / worldSizeInPixels.getFY()) * ((float)fbow->height);

			v0 = fbow->getPixelAtLinear((xc * mapFreqs.getFX()), (yc * mapFreqs.getFX()), channel);
			v1 = fbow->getPixelAtLinear((xc * mapFreqs.getFY()), (yc * mapFreqs.getFY()), channel);
			v2 = fbow->getPixelAtLinear((xc * mapFreqs.getFZ()), (yc * mapFreqs.getFZ()), channel);
			v3 = fbow->getPixelAtLinear((xc * mapFreqs.getFW()), (yc * mapFreqs.getFW()), channel);


			if (dd) {
				//cout << "hmvals: " << v0 << ", " << v1 << ", " << v2 << ", " << v3 << "\n";
			}
			

			testHeight = 
				  v0 * mapAmps.getFX()
				+ v1 * mapAmps.getFY()
				+ v2 * mapAmps.getFZ()
				+ v3 * mapAmps.getFW()
				
				- v1 * mapAmps.getFY()*0.5f
				- v2 * mapAmps.getFZ()*0.5f
				- v3 * mapAmps.getFW()*0.5f
				
			;
			
			if (dd) {
				//cout << "testHeight " << testHeight << "\n";
			}

			testHeight = clampf(testHeight,0.0f,1.0f);

			return getTerHeightScaled(testHeight);



		}



	}
void Singleton::transformEnt (GameOrg * curEnt)
                                           {
		curEnt->baseNode->doTransform(this);
	}
void Singleton::angleToVec (FIVector4 * fv, float xr, float yr)
                                                           {
		fv->setFXYZ(
			 -sin(xr)*sin(yr),
			 -cos(xr)*sin(yr),
			 -cos(yr)
		);
		fv->normalize();
	}
void Singleton::syncObjects (FIVector4 * bp)
                                        {
		int i;
				
		float xrp;
		float yrp;
		
		float xrotrad = (mainCamera->rotation[0] / 180 * M_PI);
		float yrotrad = (mainCamera->rotation[1] / 180 * M_PI);
		
		
		
		
		
		
		for (i = 1; i < E_OBJ_LENGTH; i++)
		{

			if (dynObjects[i]->moveType == E_MT_TRACKBALL) {
				
				xrp = xrotrad +	dynObjects[i]->posTrackball.getFX()/200.0f;
				yrp = yrotrad + dynObjects[i]->posTrackball.getFY()/200.0f;
				
				angleToVec(&tempVec1,xrp,yrp);
				tempVec1.multXYZ(dynObjects[i]->posTrackball.getFZ()*10.0f + 8.0f*pixelsPerCell);
				
				
				dynObjects[i]->pos.copyFrom(bp);
				dynObjects[i]->pos.addXYZRef( &(tempVec1) );
				
			}
			else {
				if (dynObjects[i]->moveType == E_MT_RELATIVE) {
					dynObjects[i]->pos.copyFrom(bp);
					dynObjects[i]->pos.addXYZRef( &(dynObjects[i]->posRel) );
				}
			}

		}
		
		testHuman->basePosition.copyFrom(&(dynObjects[E_OBJ_HUMAN]->pos));
		transformEnt(testHuman);
	}
void Singleton::moveCamera (FIVector4 * pModXYZ)
        {
		
		int i;
		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);
		
		if (
				(pModXYZ->getFX() != 0.0) ||
				(pModXYZ->getFY() != 0.0) ||
				(pModXYZ->getFZ() != 0.0)
		) {
			wsBufferInvalid = true;
		}
		
		

		cameraPos->addXYZRef(pModXYZ);


		pModXYZ->setFZ(0.0f);

		if (cameraPos->getFX() > worldSizeInPixels.getFX() / 2.0)
		{
			cameraPos->setFX( cameraPos->getFX() - worldSizeInPixels.getFX() );
		}
		if (cameraPos->getFX() < -worldSizeInPixels.getFX() / 2.0)
		{
			cameraPos->setFX( cameraPos->getFX() + worldSizeInPixels.getFX() );
		}
		if (cameraPos->getFY() > worldSizeInPixels.getFY() / 2.0)
		{
			cameraPos->setFY( cameraPos->getFY() - worldSizeInPixels.getFY() );
		}
		if (cameraPos->getFY() < -worldSizeInPixels.getFY() / 2.0)
		{
			cameraPos->setFY( cameraPos->getFY() + worldSizeInPixels.getFY() );
		}



		//syncObjects(cameraPos);

		
		mainCamera->unitPos[0] = dynObjects[E_OBJ_CAMERA]->pos.getFX();
		mainCamera->unitPos[1] = dynObjects[E_OBJ_CAMERA]->pos.getFY();
		mainCamera->unitPos[2] = dynObjects[E_OBJ_CAMERA]->pos.getFZ();
		
	}
GameOrgNode * Singleton::getMirroredNode (GameOrgNode * curNode)
                                                           {
		if ((curNode->nodeName < E_BONE_C_BEG)&&mirrorOn) {
			if (curNode->nodeName <= E_BONE_L_END) {
				return testHuman->baseNode->getNode(
					curNode->nodeName+(E_BONE_R_BEG-E_BONE_L_BEG)
				);
			}
			else {
				return testHuman->baseNode->getNode(
					curNode->nodeName-(E_BONE_R_BEG-E_BONE_L_BEG)
				);
			}
		}
		else {
			return NULL;
		}
		
	}
void Singleton::applyNodeChanges (GameOrgNode * _curNode, float dx, float dy)
                                                                         {
		
		GameOrgNode* curNode = _curNode;
		
		int i;
		int j;
		int k;
		
		float xm = 0.0f;
		float ym = 0.0f;
		float zm = 0.0f;
		
		float dirMod = 1.0f;
		
		if (
			(curNode->nodeName < E_BONE_C_BEG) &&
			(mirrorOn)
		) {
			j = 2;
		}
		else {
			j = 1;
		}
		
		for (i = 0; i < j; i++) {
			
			
			if (i == 1) {
				curNode = getMirroredNode(curNode);
				
				dirMod = -1.0f;
			}
			
			
			if (abDown) {
				makeDirty();
			}
			
			xm = dx/50.0f;
			ym = dy/50.0f;
			
			if (shiftDown()) { // || altDown()
								
				if (lbDown) {
					curNode->tbnRadScale0.addXYZ(0.0f,xm,ym);
				}
				if (rbDown) {
					curNode->tbnRadScale1.addXYZ(0.0f,xm,ym);
				}
				if (mbDown) {
					curNode->boneLengthScale += ym;
				}
			}
			else {
				
				if (lbDown) {
					curNode->rotThe += dirMod*ym;
				}
				if (rbDown) {
					curNode->rotRho += dirMod*ym;
				}
				if (mbDown) {
					curNode->rotPhi += dirMod*ym;
				}
				
				
			}
			
			if (applyToChildren) {
				for (k = 0; k < curNode->children.size(); k++) {
					applyNodeChanges(curNode->children[k], dx, dy);
				}
			}
			
			
		}
			
		
	}
void Singleton::moveObject (float dx, float dy)
        {

		int i;
		int j;
		int diffx;
		int diffy;
		
		
		GameOrgNode* curNode;

		
		modXYZ.setFXYZ(0.0f,0.0f,0.0f);

		float dxZM = dx;
		float dyZM = dy;

		bool doDefault = false;
		
		
		
		
		//////////
		
		
		float xmod = 0.0f;
		float ymod = 0.0f;
		float zmod = 0.0f;
		float xrotrad = (mainCamera->rotation[0] / 180 * M_PI);
		float yrotrad = (mainCamera->rotation[1] / 180 * M_PI);
		
		

		
		
		
		
		//////////
		
		
		
		
		
		

		if (abDown)
		{
			
			if (rbDown || (shiftDown() ) )
			{
				zmod += dy;

			}
			else
			{
				
				xmod += float(sin(yrotrad))*dy;
				ymod += float(cos(yrotrad))*dy;
				//zmod -= float(cos(xrotrad))*dy;

				xmod += float(cos(yrotrad))*(-dx);
				ymod -= float(sin(yrotrad))*(-dx);
				
			}
			
			modXYZ.setFXYZ(
				xmod*4.0f,
				ymod*4.0f,
				zmod*4.0f
			);
			

			lastModXYZ.addXYZRef(&modXYZ);




		}
		
		
		if (
			(entOn) && 
			(activeNode != NULL)
			
			// todo: remove this
			//&& false
			
		) {
				
			applyNodeChanges(activeNode, dx, dy);
			
		}
		else {
			if (shiftDown())
			{
				
				
				if ((mouseState == E_MOUSE_STATE_BRUSH)&&lbDown)
				{
					curBrushRad -= modXYZ.getFZ() / 50.0f;

					if (curBrushRad < 0.0f)
					{
						curBrushRad = 0.0f;
					}
				}
				else
				{
					
					
					
				}

			}
			else
			{
				if (mouseState == E_MOUSE_STATE_BRUSH)
				{
					doDefault = true;
				}
				else
				{

					if (activeObject == E_OBJ_CAMERA)
					{
						doDefault = true;
					}
					else
					{
						
						if (dynObjects[activeObject]->moveType == E_MT_TRACKBALL) {
							if (rbDown) {
								dynObjects[activeObject]->posTrackball.addXYZ(0.0f,0.0f,dy);
							}
							else {
								dynObjects[activeObject]->posTrackball.addXYZ(dx,dy,0.0f);
							}
							
						}
						else {
							if (dynObjects[activeObject]->moveType == E_MT_RELATIVE) {
								dynObjects[activeObject]->posRel.addXYZRef(&modXYZ, -1.0f);
							}
							else {
								dynObjects[activeObject]->pos.addXYZRef(&modXYZ, -1.0f);
							}
						}
						
						
						
					}
				}

				if (doDefault)
				{
					
					
					if (lbDown) {
						mainCamera->addRotation(dx*0.25f, dy*0.25f);
					}
					
					//moveCamera(&modXYZ);
				}
				


			}
		}
		


		






	}
void Singleton::updateMultiLights ()
        {
		if (multiLights)
		{
			numDynLights = E_OBJ_LENGTH - E_OBJ_LIGHT0;
		}
		else
		{
			numDynLights = 1;
		}
	}
void Singleton::toggleFullScreen ()
        {
		isFullScreen = !isFullScreen;
		if (isFullScreen)
		{
			glutFullScreen();
		}
		else
		{
			glutPositionWindow(250, 50);
		}
	}
void Singleton::restartGen (bool instantRefresh, bool clearEverything)
                                                                   {
		
		
		gw->actionOnHolders(E_HOLDER_ACTION_RESET, instantRefresh, clearEverything);

		
		
	}
void Singleton::setObjToElevationBase (FIVector4 * obj)
                                                   {
		
		obj->setFZ(
			getHeightAtPixelPos(obj->getFX(), obj->getFY())
		);

	}
void Singleton::setCameraToElevationBase ()
                                        {
		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);

		setObjToElevationBase(cameraPos);
	}
void Singleton::setCameraToElevation ()
        {
		setCameraToElevationBase();
		
		wsBufferInvalid = true;
	}
void Singleton::moveCameraToTown ()
                                {
		
		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);
		FIVector4 townPos;
		townPos.setFXYZ(
			(gw->provinceX[15]/gw->mapWidth),
			(gw->provinceY[15]/gw->mapHeight),
			0.0f
		);
		
		townPos.multXYZRef(&worldSizeInPixels);
		
		townPos.addXYZRef(cameraPos,-1.0f);
		moveCamera(&townPos);
		setCameraToElevation();
	}
void Singleton::processSpecialKeys (int key, int _x, int _y)
        {

		int x = _x / scaleFactor;
		int y = _y / scaleFactor;

		switch (key)
		{
		case GLUT_KEY_F1 :

			break;
		case GLUT_KEY_F2 :

			break;
		case GLUT_KEY_F3 :

			break;
		}
	}
void Singleton::updateCS ()
                        {
		if (
			(bCtrl != bCtrlOld) ||
			(bShift != bShiftOld)
		) {
			forceGetPD = true;
			wsBufferInvalid = true;
		}
		bCtrlOld = bCtrl;
		bShiftOld = bShift;
	}
void Singleton::keyboardUp (unsigned char key, int _x, int _y)
        {
		int x = _x / scaleFactor;
		int y = _y / scaleFactor;
		
		//cout << "key: " << key << "\n";
		
		
		processKey(key,x,y,false);
		switch(key) {
			case 'a':
			case 'z':
			case 'e':
			case 'd':
			case 's':
			case 'f':
				return;
			break;
			
		}
		
		
		
		GameOrgNode* curNode;
		
		


		
		bShift = shiftDown();
		bCtrl = ctrlDown();
		updateCS();


		if (key == 17) {
			glutLeaveMainLoop();
		}

		switch (key) {

		case '1':
			currentActor->moveCell(0,0,1);
		break;
		case '0':
			currentActor->moveCell(0,0,-1);
		break;
		case '8':
			currentActor->moveCellRotated(1);
		break;
		case '5':
			currentActor->moveCellRotated(-1);
		break;
		case '4':
			currentActor->rotate(1,true);
			//currentActor->moveCell(-1,0,0);
		break;
		case '6':
			currentActor->rotate(-1,true);
			//currentActor->moveCell(1,0,0);
		break;
		
		// case 'a':
		// 	// selectedNode->material += 1.0f;
		// 	// curNode = getMirroredNode(selectedNode);
		// 	// if (curNode != NULL) {
		// 	// 	curNode->material += 1.0f;
		// 	// }
		// 	// makeDirty();
		// break;
		// case 'z':
		// 	// selectedNode->material -= 1.0f;
		// 	// curNode = getMirroredNode(selectedNode);
		// 	// if (curNode != NULL) {
		// 	// 	curNode->material -= 1.0f;
		// 	// }
		// 	// makeDirty();
			
		// break;



		case 'i':
				isMacro = !isMacro;
				
				//mirrorOn = !mirrorOn;
				cout << "isMacro: " << isMacro << "\n";
			break;


		// case '9':
		// 	saveAllData();
		// 	cout << "data saved\n";
		// break;
		
		case 19: //ctrl-s
			saveGUIValues();
			//cout << "Use s key in web editor to save\n";
			break;

		case 15: //ctrl-o
			//loadAllData();
			loadGUIValues();

			break;

		case '[':
			iNumSteps /= 2;
			if (iNumSteps < 16)
			{
				iNumSteps = 16;
			}
			doTraceND("iNumSteps: ", i__s(iNumSteps));

			break;
		case ']':
			iNumSteps *= 2;
			if (iNumSteps > 256)
			{
				iNumSteps = 256;
			}
			doTraceND("iNumSteps: ", i__s(iNumSteps));

			break;


		case 'u':
			moveCameraToTown();
			break;
			
		case 'q':
		
			// autoScroll = !autoScroll;
			
			// if (autoScroll) {
			// 	scrollTimer.start();
			// 	baseScrollPos.copyFrom(&(dynObjects[E_OBJ_CAMERA]->pos));
			// }
			
			
			break;

		case 'w':
			resetActiveNode();
		break;
		case 'W':
			maxWInPages++;
			break;
		case 'Q':
			maxWInPages--;
			if (maxWInPages < 1)
			{
				maxWInPages = 1;
			}
			break;

		case 27: // esc
			std::exit(0);
			break;

		case 'b':
		
			bakeParamsOn = !bakeParamsOn;
			cout << "bakeParamsOn: " << bakeParamsOn << "\n";
			doShaderRefresh(bakeParamsOn);
			
			
			
		
			//radiosityOn = !radiosityOn;
			break;


		case 'R':
		
			//loadGUIValues(false);
			doShaderRefresh(bakeParamsOn);
			loadGUI();
			loadGUIValues();
		break;
		case 'r':
			doShaderRefresh(bakeParamsOn);
			

			cout << "Shaders Refreshed\n";
			
			break;
			
		case 'j':
			doShaderRefresh(bakeParamsOn);
		
			mapInvalid = true;
			gw->initMap();
		break;

		case 'G':
			gridOn = 1.0 - gridOn;
			cout << "Grid On: " << gridOn << "\n";

			break;


		case 'g':
		
			mouseState++;

			if (mouseState == E_MOUSE_STATE_LENGTH)
			{
				mouseState = 0;
			}
			
			cout << mouseStateStrings[mouseState] << "\n";


			wsBufferInvalid = true;
			forceGetPD = true;
		
			
		break;
		
		case 'l':

			multiLights = !multiLights;
			updateMultiLights();


			forceGetPD = true;

			break;

		case ';':
			doPageRender = !doPageRender;
			cout << "doPageRender: " << doPageRender << "\n";
		break;
		case 'p':
			toggleFullScreen();
			break;

		case 'o':
			targetTimeOfDay = 1.0f-targetTimeOfDay;
			// targetTimeOfDay += 0.5;
			
			// if (targetTimeOfDay > 1.0) {
			// 	targetTimeOfDay = 0.0;
			// }
			
			break;

		case 'h':
			waterOn = !waterOn;

			if (MAX_LAYERS == 1)
			{
				waterOn = false;
			}



			cout << "waterOn " << waterOn << "\n";
			break;

		case 't':
			testOn = !testOn;
			
			break;
		// case 'o':
		// 	//rotOn = !rotOn;
		// 	break;

		case '\t':
		
			if (mainGUI->isReady) {
				if (mainMenu == NULL) {
					
				}
				else {
					if (mainMenu->visible) {
						playSoundEvent("hideGUI");
					}
					
					mainMenu->visible = !(mainMenu->visible);
					
					if (mainMenu->visible) {
						playSoundEvent("showGUI");
					}
				}
			}
			
		
			
			
			break;

		case ' ':
			selectedEnts.cycleEnts();
			
		break;

		case 'c':
			doShaderRefresh(bakeParamsOn);
			restartGen(false, true);
			break;
		

		case 'x':
			fogOn = 1.0 - fogOn;
			cout << "fog on " << fogOn << "\n";
			break;

		case 'm':

			
			runReport();
			

			break;

		
		case 'A':
			maxHInPages++;
			break;
		case 'Z':
			maxHInPages--;
			if (maxHInPages < 1)
			{
				maxHInPages = 1;
			}
			break;

		

		case 'v':
			gw->toggleVis(selectedEnts.getSelectedEnt());
			break;



		default:

			break;
		}


	}
void Singleton::processKey (unsigned char key, int x, int y, bool isPressed)
                                                                         {

		
		keysPressed[key] = isPressed;
	}
void Singleton::keyboardDown (unsigned char key, int _x, int _y)
        {
		int x = _x / scaleFactor;
		int y = _y / scaleFactor;
		
		processKey(key,x,y,true);
		
		
		
		
		bShift = shiftDown();
		bCtrl = ctrlDown();
		updateCS();

		
	}
void Singleton::runReport ()
                         {
		
		mainGUI->runReport();
		cout << "lightCount: " << gw->lightCount << "\n";
		cout << "TOT GPU MEM USED (MB): " << TOT_GPU_MEM_USAGE << "\n";
		cout << "HolderSize (MB): " << holderSizeMB << "\n";
		cout << "Num GPU Holders: " << gpuPool->holderPoolItems.size() << "\n";
		cout << "GPU Pooled MB Used: " << ((float)gpuPool->holderPoolItems.size())*holderSizeMB << "\n";
		cout << "totalPointCount: " << totalPointCount << "\n";
	}
void Singleton::getPixData (FIVector4 * toVector, int _xv, int _yv, bool forceUpdate, bool isObj)
        {

		FBOWrapper *fbow;
		int newX;
		int newY;
		
		float xv = _xv;
		float yv = _yv;


		if (wsBufferInvalid || forceUpdate || forceGetPD)
		{
			if (isObj) {
				getFBOWrapper("geomTargFBO", 1)->getPixels();
			}
			else {
				getFBOWrapper("pagesTargFBO", 0)->getPixels();
			}
			
			//cout << "getBuf\n";
			
		}

		
		
		newX = clamp(xv, 0, bufferDim.getIX() - 1);
		newY = clamp(yv, 0, bufferDim.getIY() - 1);


		if (isObj) {
			fbow = getFBOWrapper("geomTargFBO", 1);
			fbow->getPixelAtF(toVector, newX, (bufferDim.getIY() - 1) - newY);
		}
		else {
			fbow = getFBOWrapper("pagesTargFBO", 0);
			fbow->getPixelAtF(toVector, newX, (bufferDim.getIY() - 1) - newY);
		}

		wsBufferInvalid = false;
		forceGetPD = false;
		
	}
void Singleton::mouseMove (int _x, int _y)
        {

		mouseMoved = true;

		int x = _x / scaleFactor;
		int y = _y / scaleFactor;

		int dx = x - lastPosX;
		int dy = y - lastPosY;

		guiX = _x/UI_SCALE_FACTOR;
		guiY = _y/UI_SCALE_FACTOR;

		bShift = shiftDown();
		bCtrl = ctrlDown();
		updateCS();

		lastMouseX = x;
		lastMouseY = y;

		mouseXUp = x;
		mouseYUp = y;


		
			

		

		if (abDown)
		{
			moveObject((float)dx, (float)dy);
		}
		else
		{

			if (entOn||pathfindingOn||(mouseState != E_MOUSE_STATE_MOVE)) {
				getPixData(&mouseMovePD, x, y, false, false);
			}

			


			

			// if (
			// 	mouseState != E_MOUSE_STATE_MOVE
			// )	{

				gw->modifyUnit(&mouseMovePD, E_BRUSH_MOVE);
			//}
			
			


			//////////////

			if (entOn) {
				updateNearestOrgNode(false, &mouseMovePD);
			}
			else {
				activeNode = NULL;
				setSelNode(NULL);
			}
			

			gw->findNearestEnt(
				&highlightedEnts,
				E_ET_GEOM,
				2,
				1,
				&mouseMovePD
			);
			highlightedEnt = highlightedEnts.getSelectedEnt();


			//////////////


		}

		lastPosX = x;
		lastPosY = y;

		if ( (x >= 0) && (y >= 0) && (x < baseW) && (y < baseH) )
		{

			
		}
	}
void Singleton::makeDirty ()
                         {
		testHuman->gph->childrenDirty = true;
	}
void Singleton::setSelNode (GameOrgNode * newNode)
                                              {
		
		selectedNode = newNode;
		if (selectedNode != lastSelNode) {
			makeDirty();
		}
		lastSelNode = newNode;
		
	}
void Singleton::worldToScreenBase (FIVector4 * sc, FIVector4 * wc)
                                                             {
		
		
		
		
		
		
		
		Vector4 v;
		v.x = wc->getFX();
		v.y = wc->getFY();
		v.z = wc->getFZ();
		v.w = 1.0;
		

		
		
		
		
		GLdouble winX;
		GLdouble winY;
		GLdouble winZ;
		
		
		gluProject(
			v.x,// GLdouble  	objX,
			v.y,// GLdouble  	objY,
			v.z, // GLdouble  	objZ,
			viewMatrixD, // const GLdouble *  	model,
			projMatrixD, // const GLdouble *  	proj,
			viewport, // const GLint *  	view,
			&winX,
			&winY,
			&winZ
		);
		
		sc->setFXYZ(
			(winX/bufferDim.getFX())/((float)DEF_SCALE_FACTOR),
			(winY/bufferDim.getFY())/((float)DEF_SCALE_FACTOR),
			1.0f - winZ/mainCamera->clipDist[1]
		);
		

	}
void Singleton::handleMovement ()
                                  {

		
		
		
		float xrotrad = (mainCamera->rotation[0] / 180 * M_PI);
		float yrotrad = (mainCamera->rotation[1] / 180 * M_PI);
		
		
		
		angleToVec(&lookAtVec,xrotrad,yrotrad);
		
		
		
		double curMoveTime = moveTimer.getElapsedTimeInMilliSec();
		
		

		float xmod = 0.0f;
		float ymod = 0.0f;
		float zmod = 0.0f;
		

		if (keysPressed[keyMap[KEYMAP_UP]]) {
			zmod += 1.0f;
		}

		if (keysPressed[keyMap[KEYMAP_DOWN]]) {
			zmod -= 1.0f;
		}

		if (keysPressed[keyMap[KEYMAP_FORWARD]]) {

			
			xmod += float(sin(xrotrad));
			ymod += float(cos(xrotrad));
			zmod -= float(cos(yrotrad));
		}

		if (keysPressed[keyMap[KEYMAP_BACKWARD]]) {
			
			xmod -= float(sin(xrotrad));
			ymod -= float(cos(xrotrad));
			zmod += float(cos(yrotrad));
		}

		if (keysPressed[keyMap[KEYMAP_RIGHT]]) {
			
			xmod += float(cos(xrotrad));
			ymod -= float(sin(xrotrad));
		}

		if (keysPressed[keyMap[KEYMAP_LEFT]]) {
			
			xmod -= float(cos(xrotrad));
			ymod += float(sin(xrotrad));
		}

		
		
		
		curMoveAccel = (curMoveTime-lastMoveTime)*0.5;
		curMoveSpeed += curMoveAccel;
		
		if (curMoveSpeed > 128.0f) {
			curMoveSpeed = 128.0f;
		}
		
		lastMoveTime = curMoveTime;
		
		modXYZ.setFXYZ(
			xmod*curMoveSpeed,
			ymod*curMoveSpeed,
			zmod*curMoveSpeed
		);
		moveCamera(&modXYZ);
		
		
		if (
			(xmod != 0.0f) ||
			(ymod != 0.0f) ||
			(zmod != 0.0f)
		) {
			wsBufferInvalid = true;
		
		}
		else {
			
			curMoveAccel = 0.0f;
			curMoveSpeed *= 0.95f;
		}
		
		isMoving = (curMoveSpeed >= 1.0);
		
		
		
		
	}
void Singleton::mouseClick (int button, int state, int _x, int _y)
        {
		
		int tempInt;


		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);

		int x = _x / scaleFactor;
		int y = _y / scaleFactor;

		lastPosX = x;
		lastPosY = y;
		
		guiX = _x/UI_SCALE_FACTOR;
		guiY = _y/UI_SCALE_FACTOR;

		GameBlock *curBlock;

		int res;
		int i;
		int j;
		int k;
		int ii;
		int jj;
		int bestInd;
		float bestDis;
		float curDis;

		float wheelDelta = 0.0f;
		bool mbClicked = false;
		bool rbClicked = false;
		bool lbClicked = false;

		int mouseMovingStart;

		FIVector4 testVec;
		FIVector4 oneVec;
		oneVec.setFXYZ(1.0f, 1.0f, 1.0f);
		oneVec.normalize();

		bool doProc = false;
		bool noTravel = false;
		bool abClicked = false;
		
		hitGUI = false;
		
		if ((mainGUI != NULL)) {
			if (mainGUI->isReady) {
				if (mainMenu != NULL) {
					if (mainMenu->visible){
							doProc = true;
					}
				}
				if (ddMenu != NULL) {
					if (ddMenu->visible){
							doProc = true;
					}
				}
				if (doProc) {
					hitGUI = mainGUI->testHit(button, state, guiX, guiY);
				}
				
			}
		}
		

		if (hitGUI) {
			return;
		}
		
		
		
		

		switch (button)
		{
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
			wheelDelta = 1.0f / 20.0f;
			break;

		case 4: // wheel down
			wheelDelta = -1.0f / 20.0f;
			break;
		}

		abDown = lbDown || rbDown || mbDown;
		abClicked = lbClicked || rbClicked || mbClicked;
		
		wsBufferInvalid = true;
		
		if (abDown) {
			getPixData(&mouseDownPD, x, y, true, false);
		}
		else {
			getPixData(&mouseUpPD, x, y, true, false);
		}
		
		if (abClicked) {
			mouseEnd.setIXY(x, y);
			noTravel = mouseEnd.distance(&mouseStart) < 30.0;
			
		}
		

		if (state == GLUT_DOWN) {
			mouseVel.setFXY(0.0f, 0.0f);
		}
		
		//GamePage* gp;

		if (abClicked) {


			if (ddMenu != NULL) {
				if (rbClicked&&(!bCtrl)&&(mouseState == E_MOUSE_STATE_MOVE)) {
					ddMenu->visible = true;
					
					ddMenu->floatOffset.x = (guiX);
					ddMenu->floatOffset.y = (guiY);
					// ddMenu->dragStart.x = 0.0f;
					// ddMenu->dragStart.y = 0.0f;
					// ddMenu->dragOffset.x = 0.0f;
					// ddMenu->dragOffset.y = 0.0f;
					// ddMenu->lastDrag.x = 0.0f;
					// ddMenu->lastDrag.y = 0.0f;
					// ddMenu->updateValue(guiX,guiY);
					
					worldMarker.copyFrom(&mouseUpPD);
					
					lastCellPos.copyFrom(&(worldMarker));
					lastCellPos.intDivXYZ(pixelsPerCell);
					//lastCellPos.multXYZ(pixelsPerCell);
					
					// tempVec.copyFrom(&lastCellPos);
					// tempVec.addXYZ(pixelsPerCell);
					
					// gp = gw->getPageAtCoords()
					// for (i = 0; i < E_CD_LENGTH; i++) {
					// 	cout << gp->totCol[i] << "\n";
					// }
					// cout << "\n";
					
					
					markerFound = true;
					
				}
				else {
					if (noTravel) {
						ddMenu->visible = false;
						markerFound = false;
					}
					
				}
			}
			


			if (abDown)
			{

			}
			else
			{

				muTime = myTimer.getElapsedTimeInMilliSec();


				if ( (activeObject == E_OBJ_CAMERA) && (muTime - mdTime > 300.0f) )
				{
					mouseMovingStart = (mouseMovingLoc - min(mouseMovingStepsBack, mouseCount) + mouseMovingSize) % mouseMovingSize;


					if (mouseCount > 0)
					{
						mouseVel.copyFrom( &(mouseMoving[mouseMovingLoc]) );
						mouseVel.addXYZRef( &(mouseMoving[mouseMovingStart]), -1.0f);
					}


				}



				lastModXYZ.normalize();

				if (shiftDown())
				{

				}
				else
				{




					activeObject = E_OBJ_CAMERA;
					


					
					
					
					gw->findNearestEnt(
						&selectedEnts,
						E_ET_GEOM,
						2,
						1,
						&mouseUpPD,
						true
					);
					
					selectedEnt = selectedEnts.getSelectedEnt();

					if (
						(selectedEnt == NULL) ||
						(mouseState == E_MOUSE_STATE_PICKING) ||
						(mouseState == E_MOUSE_STATE_BRUSH)
					)	{

					}
					else
					{

						switch (selectedEnt->buildingType)
						{
						case E_CT_DOOR:
						case E_CT_WINDOW:
							
							

						
							gw->getHoldersInEnt(selectedEnt);
							selectedEnt->toggleTransform();
							gw->getHoldersInEnt(selectedEnt);
							gw->refreshHoldersInList(true, true);
							gw->holdersToRefresh.clear();
							
							if (selectedEnt->toggled) {
								// open
								switch (selectedEnt->buildingType)
								{
									case E_CT_DOOR:
										playSoundPosAndPitch(
											"open3",
											cameraPos,
											selectedEnt->getVisMinInPixelsT(),
											0.3f
										);
									break;
									case E_CT_WINDOW:
										playSoundPosAndPitch(
											"open1",
											cameraPos,
											selectedEnt->getVisMinInPixelsT(),
											0.3f
										);
									break;
								}
							}
							else {
								// close
								
								switch (selectedEnt->buildingType)
								{
									case E_CT_DOOR:
										playSoundPosAndPitch(
											"close2",
											cameraPos,
											selectedEnt->getVisMinInPixelsT(),
											0.3f
										);
									break;
									case E_CT_WINDOW:
										playSoundPosAndPitch(
											"close1",
											cameraPos,
											selectedEnt->getVisMinInPixelsT(),
											0.3f
										);
									break;
								}
							}
							

							
							wsBufferInvalid = true;
							break;

						case E_CT_LANTERN:
							selectedEnt->light->toggle();
							playSoundPosAndPitch(
								"castinet0",
								cameraPos,
								selectedEnt->getVisMinInPixelsT(),
								0.3f
							);
							gw->updateLights();
							cout << "final toggle " << selectedEnt->light->toggled << "\n";
							break;

						}

					}






					if ( noTravel ) {
						
						if (pathfindingOn) {
							if (lbClicked) {
								
								if (currentStep == 2) {
									currentStep = 0;
									moveNodes[0].setFXYZ(0.0,0.0,0.0);
									moveNodes[1].setFXYZ(0.0,0.0,0.0);
								}
								else {
									moveNodes[currentStep].copyFrom(&(gw->lastUnitPos));
									currentStep++;
								}
							}
						}
						else {
							switch (mouseState) {
								case E_MOUSE_STATE_BRUSH:
									
									if (lbClicked)
									{
										gw->modifyUnit(&mouseUpPD, E_BRUSH_ADD);
									}
									else
									{
										gw->modifyUnit(&mouseUpPD, E_BRUSH_SUB);
									}

									forceGetPD = true;

								break;
							}
						}
						
						
						
					}

				}


			}


		}
		else
		{
			if (abDown)
			{

				if (rbDown && lbDown)
				{

				}
				else
				{


					lastModXYZ.setFXYZ(0.0f, 0.0f, 0.0f);

					mouseMovingLoc = 0;
					mouseCount = 0;
					mdTime = myTimer.getElapsedTimeInMilliSec();
					mouseStart.setIXY(x, y);

					










					activeObject = E_OBJ_CAMERA;
					
					
					
					
					if (bCtrl) {
						
						getPixData(&mouseObjPD, x, y, true, true);
						
						if (mouseObjPD.getFW() != 0.0) {
							// find nearest dyn object
							
							cout << "OBJECT ID " << mouseObjPD.getFW() << "\n";
							

							bestInd = mouseObjPD.getFW();
							
							if ((bestInd >= E_OBJ_LENGTH)||(bestInd <= 0)) {
								
							}
							else {
								activeObject = (E_OBJ)(bestInd);
							}
							

							
						}
					}
					else {
						
						
						
						if (entOn) {
							updateNearestOrgNode(true, &mouseDownPD);
						}
						
					}
					
					
					
				}


			}
		}



		


		if ( (button == 3) || (button == 4) ) {
			
			myDelta += wheelDelta;
			targetZoom = pow(2.0, myDelta);

		}

		

	}
void Singleton::resetActiveNode ()
                               {
		
		GameOrgNode* curNode = NULL;
		
		if (selectedNode == NULL) {
			curNode = lastSelNode;
		}
		else {
			curNode = selectedNode;
		}
		
		if (curNode != NULL) {
			curNode->rotThe = 0.0f;
			curNode->rotPhi = 0.0f;
			curNode->rotRho = 0.0f;
			
			curNode->tbnRadScale0.setFXYZ(1.0f,1.0f,1.0f);
			curNode->tbnRadScale1.setFXYZ(1.0f,1.0f,1.0f);
			makeDirty();
		}
	}
void Singleton::updateNearestOrgNode (bool setActive, FIVector4 * mousePosWS)
                                                                         {
		// tempVec3.setFXYZRef(mousePosWS);
		// tempVec3.addXYZRef(&(testHuman->basePosition),-1.0f);
		
		//worldToScreenBase(&tempVec1, mousePosWS);
		
		bestNode = NULL;
		bestNodeDis = 99999.0f;
		findNearestOrgNode(
			testHuman->baseNode,
			mousePosWS//&tempVec1
		);
		
		//cout << "bestNodeDis " << bestNodeDis << "\n";
		
		if (bestNodeDis >= 3.0f*pixelsPerCell) {
			bestNode = NULL;
			activeNode = NULL;
			setSelNode(NULL);
		}
		
		if (bestNode != NULL) {
			
			setSelNode(bestNode);
			if (setActive) {
				activeNode = bestNode;				
			}
		}
	}
void Singleton::findNearestOrgNode (GameOrgNode * curNode, FIVector4 * mousePosWS)
          {
		
		tempVec3.setFXYZRef(&(curNode->orgTrans[1]));
		tempVec3.multXYZ(pixelsPerCell);
		tempVec3.addXYZRef(&(testHuman->basePosition));
		
		//worldToScreenBase(&tempVec2, &tempVec3);
		float curDis = mousePosWS->distance(&tempVec3);//&tempVec2);
		
		if (curDis < bestNodeDis) {
			bestNodeDis = curDis;
			bestNode = curNode;
		}
		
		int i;
		
		for (i = 0; i < curNode->children.size(); i++) {
			findNearestOrgNode(curNode->children[i],mousePosWS);
		}
		
	}
void Singleton::processB64 (charArr * sourceBuffer, charArr * saveBuffer)
        {

		char *buf = sourceBuffer->data;
		int len = sourceBuffer->size;

		if (saveBuffer != &nullBuffer)
		{
			if (saveBuffer->data != NULL)
			{
				delete[] saveBuffer->data;
				saveBuffer->data = NULL;
			}
			saveBuffer->data = new char[len];
			strncpy(saveBuffer->data, buf, len);
			saveBuffer->size = len;
		}

		string palString = "palFBO";

		FBOSet *fbos = getFBOByName(palString);


		//unsigned char* resultImage = new unsigned char[256*256*4];

		membuf inBuffer(sourceBuffer->data, sourceBuffer->data + sourceBuffer->size);
		std::istream myIS(&inBuffer);
		Poco::Base64Decoder b64in(myIS);



		std::ostringstream oss;
		oss << b64in.rdbuf();

		std::string strConst = oss.str();
		const char *inString = strConst.c_str();


		lodepng_decode32(&resultImage, &palWidth, &palHeight, (unsigned char *)inString, strConst.size() );

		fbos->copyFromMem(0, resultImage);

	}
bool Singleton::processJSONFromString (string * sourceBuffer, JSONValue * * destObj)
          {
		if (*destObj != NULL)
		{
			delete *destObj;
			*destObj = NULL;
		}
		
		*destObj = JSON::Parse(sourceBuffer->c_str());

		if (*destObj == NULL)
		{
			doTraceND("Invalid JSON\n\n");
			cout << sourceBuffer << "\n\n";
			return false;
		}
		else
		{
			doTraceND("\nValid JSON\n");
			return true;
		}
		
	}
bool Singleton::processJSON (charArr * sourceBuffer, charArr * saveBuffer, JSONValue * * destObj)
          {
		
		if (*destObj != NULL)
		{
			delete *destObj;
			*destObj = NULL;
		}

		//doTraceND("destObj is now NULL");
		

		//doTraceND("processJSON()");

		char *buf = sourceBuffer->data;
		int len = sourceBuffer->size;
		//JSONValue *jsonVal = NULL;

		if (saveBuffer != &nullBuffer)
		{
			if (saveBuffer->data != NULL)
			{
				delete[] saveBuffer->data;
				saveBuffer->data = NULL;
			}
			saveBuffer->data = new char[len];
			strncpy(saveBuffer->data, buf, len);
			saveBuffer->size = len;
		}

		//doTraceND("Begin JSON::Parse()");

		if (buf == NULL)
		{
			//doTraceND("buf is NULL");
			return false;
		}
		else
		{
			//doTraceND("buf is not NULL");
			*destObj = JSON::Parse(buf);
		}


		//doTraceND("End JSON::Parse()");


		if (*destObj == NULL)
		{
			doTraceND("Invalid JSON\n\n");
			return false;
		}
		else
		{
			doTraceND("\nValid JSON\n");
			return true;
		}


	}
bool Singleton::loadJSON (string path, JSONValue * * destObj)
          {
		
		bool res = false;
		
		charArr dest;
		dest.data = NULL;
		dest.size = 0;
		
		if ( loadFile(path, &dest) )
		{
			if (processJSON(&dest, &nullBuffer, destObj)) {
				res = true;
			}
			else {
				res = false;
			}
		}
		else {
			res = false;
		}
		
		
		if (dest.data != NULL)
		{
			delete[] dest.data;
			dest.data = NULL;
		}
		
		return res;
		
	}
void Singleton::setGUIText (string key, string stringValue, float floatValue, bool applyVal, bool applyString)
          {
		UICStruct* curComp;
		if (compMap.find( key ) == compMap.end()) {
			// invalid key
		}
		else {
			curComp = &(compMap[key]);
			
			if (
				(curComp->uic == NULL) ||
				(curComp->nodeId < 0)	
			) {
				// component was deleted
			}
			else {
				if (applyString) {
					curComp->uic->setText(stringValue);
				}
				if (applyVal) {
					curComp->uic->setValue(floatValue);
				}
				
			}
		}
	}
float Singleton::getGUIValue (string key)
                                      {
		UICStruct* curComp;
		if (compMap.find( key ) == compMap.end()) {
			// invalid key
		}
		else {
			curComp = &(compMap[key]);
			
			if (
				(curComp->uic == NULL) ||
				(curComp->nodeId < 0)	
			) {
				// component was deleted
			}
			else {
				return curComp->uic->getValue();
			}
		}
		
		return 0.0;
	}
UIComponent * Singleton::getGUIComp (string key)
                                            {
		UICStruct* curComp;
		if (compMap.find( key ) == compMap.end()) {
			// invalid key
		}
		else {
			curComp = &(compMap[key]);
			
			if (
				(curComp->uic == NULL) ||
				(curComp->nodeId < 0)	
			) {
				// component was deleted
			}
			else {
				return curComp->uic;
			}
		}
		
		return NULL;
	}
void Singleton::setGUIValue (string key, float floatValue, bool dispatchEvent, bool preventRefresh)
          {
		UICStruct* curComp;
		
		if (compMap.find( key ) == compMap.end()) {
			// invalid key
		}
		else {
			curComp = &(compMap[key]);
			
			if (
				(curComp->uic == NULL) ||
				(curComp->nodeId < 0)	
			) {
				// component was deleted
			}
			else {
				curComp->uic->setValue(floatValue,dispatchEvent,preventRefresh);
			}
		}
	}
void Singleton::loadGUIValues (bool applyValues)
                                                     {
		
		cout << "Loading GUI Values\n";
		
		int i;
		
		charArr dest;
		dest.data = NULL;
		dest.size = 0;
		
		UICStruct* curComp;
		
		string loadBuf;
		vector<string> splitStrings;
		if ( loadFile(guiSaveLoc, &dest) )
		{
			loadBuf = string(dest.data);
			splitStrings = split(loadBuf, '^');
			
			for (i = 0; i < splitStrings.size(); i += 2) {
				
				setGUIValue(
					splitStrings[i],
					hexToFloat( &(splitStrings[i+1]) ),
					true,
					true
				);
				
				if (applyValues) {
					
				}
				else {
					
				}
				
			}
			
		}
		else {
			cout << "Unable to load GUI Values\n";
			
		}
		
		if (dest.data != NULL)
		{
			delete[] dest.data;
			dest.data = NULL;
		}
		
		cout << "End Loading GUI Values\n";
	}
void Singleton::saveGUIValues ()
                             {
		
		cout << "Saving GUI Values\n";
		
		stringBuf = "";
		
		for(itUICStruct iterator = compMap.begin(); iterator != compMap.end(); iterator++) {
				
				if (
					(iterator->second.uic == NULL) ||
					(iterator->second.nodeId < 0)
				) {
					
				}
				else {
					if (iterator->first[0] ==  '$') {
						stringBuf.append(
							iterator->first + "^" + floatToHex(iterator->second.uic->getValue()) + "^"
						);
					}
					
					
				}
				
		    // iterator->first = key
		    // iterator->second = value
		}
		
		saveFileString(guiSaveLoc,&stringBuf);
		
		cout << "End Saving GUI Values\n";
	}
void Singleton::updateGUI ()
                         {
		
		float chunkMem;
		float fbMem;
		
		
		
		int mvPerPage = (volGenFBOX*volGenFBOX*volGenFBOX)/(1024*1024);
		
		float voxelsGen = PAGE_COUNT*mvPerPage;
		
		string maxGPUMString = " / " + fi__s(MAX_GPU_MEM);
		
		if (frameCount%120 == 0) {
			
			chunkMem = ((float)gpuPool->holderPoolItems.size())*holderSizeMB;
			fbMem = TOT_GPU_MEM_USAGE - chunkMem;
			
			
			setGUIText("debug.fbMem", "Frame Buffer Mem Used: " + fi__s(fbMem) + maxGPUMString, fbMem/MAX_GPU_MEM, true );
			setGUIText("debug.chunkMem", "Chunk Mem Used: " + fi__s(chunkMem) + maxGPUMString, chunkMem/MAX_GPU_MEM, true );
			setGUIText("debug.totMem", "Total Mem Used: " + fi__s(TOT_GPU_MEM_USAGE) + maxGPUMString, TOT_GPU_MEM_USAGE/MAX_GPU_MEM, true );
			setGUIText("debug.chunksGen", "Voxels Generated (In Millions!): " + fi__s(voxelsGen) );
			 
		}
		
	}
void Singleton::loadGUI ()
                       {
		
		
		for(itUICStruct iterator = compMap.begin(); iterator != compMap.end(); iterator++) {
				iterator->second.uic = NULL;
				iterator->second.nodeId = -1;
		    // iterator->first = key
		    // iterator->second = value
		}
		
		
		if (
			loadJSON("..\\data\\lastJSONBufferGUI.js", &guiRootJS)
		) {
			mainGUI->guiFromJSON(
				guiRootJS
			);
		
			for(itUICStruct iterator = compMap.begin(); iterator != compMap.end(); iterator++) {
					
					
					if (iterator->second.nodeId != -1) {
						iterator->second.uic = mainGUI->findNodeById(iterator->second.nodeId);
					}
			}
		
		}
		
		mapComp = getGUIComp("map.mapHolder");
		mainMenu = getGUIComp("guiHandles.mainMenu");
		ddMenu = getGUIComp("guiHandles.ddMenu");
		
		if (mainMenu != NULL) {
			mainMenu->visible = false;
		}
		if (ddMenu != NULL) {
			ddMenu->visible = false;
		}
		
		
	}
void Singleton::loadAllData ()
        {
		if ( loadFile("..\\data\\lastJSONBuffer.js", &lastJSONBuffer) )
		{
			processJSON(&lastJSONBuffer, &nullBuffer, &rootObjJS);
		}

		if ( loadFile("..\\data\\lastImageBuffer.txt", &lastImageBuffer) )
		{
			processB64(&lastImageBuffer, &nullBuffer);
		}

	}
void Singleton::saveAllData ()
        {
		
		
		saveFile("..\\data\\lastJSONBuffer.js", &lastJSONBuffer);
		saveFile("..\\data\\lastImageBuffer.txt", &lastImageBuffer);
		
		cout << "All Data Saved\n";
	}
bool Singleton::loadFile (string fnString, charArr * dest)
        {
		
		const char* fileName = fnString.c_str();
		
		doTraceND("Loading: ", fileName);

		if (dest == NULL)
		{
			doTraceND("Null Data");
			return false;
		}

		std::ifstream infile (fileName, std::ifstream::in);


		if ( ! infile.is_open() )
		{
			doTraceND("Could Not Open File For Loading");
			return false;
		}

		// get size of file
		infile.seekg (0, infile.end);
		long size = (long)infile.tellg();
		infile.seekg (0, infile.beg);


		dest->size = size;

		if (dest->data != NULL)
		{
			delete[] dest->data;
			dest->data = NULL;
		}

		dest->data = new char[size+1];

		// read content of infile
		infile.read (dest->data, size);

		if ( infile.bad() )
		{
			doTraceND("Could Not Load From File");
			infile.close();
			return false;
		}

		infile.close();
		
		dest->data[size] = '\0';

		doTraceND("Load Successful");

		return true;
	}
bool Singleton::saveFileString (string fileName, string * source)
        {

		if (source == NULL)
		{
			doTraceND("Null Data");
			return false;
		}

		std::ofstream outfile (fileName.c_str(), std::ofstream::out);

		if ( ! outfile.is_open() )
		{
			doTraceND("Could Not Open File For Saving");
			return false;
		}

		outfile.write(source->c_str(), source->size());

		if ( outfile.bad() )
		{
			doTraceND("Could Not Save To File");
			outfile.close();
			return false;
		}

		outfile.close();

		doTraceND("Save Successful");

		return true;
	}
bool Singleton::saveFile (char * fileName, charArr * source)
        {

		if (source->data == NULL)
		{
			doTraceND("Null Data");
			return false;
		}

		std::ofstream outfile (fileName, std::ofstream::out);

		if ( ! outfile.is_open() )
		{
			doTraceND("Could Not Open File For Saving");
			return false;
		}

		outfile.write (source->data, source->size);

		if ( outfile.bad() )
		{
			doTraceND("Could Not Save To File");
			outfile.close();
			return false;
		}

		outfile.close();

		doTraceND("Save Successful");

		return true;
	}
void Singleton::updateAmbientSounds ()
                                   {
		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);
		
		int i;
		int j;
		
		int maxRad = 2.0f;
		
		float avgHeight = 0.0f;
		float tot = 0.0f;
		
		for (i = -maxRad; i <= maxRad; i++) {
			for (j = -maxRad; j <= maxRad; j++) {
				avgHeight += getHeightAtPixelPos(
					cameraPos->getFX() + i*32.0f*pixelsPerCell,
					cameraPos->getFY() + j*32.0f*pixelsPerCell
				);
				tot += 1.0f;
			}
		}
		
		float terHeight = avgHeight/tot;
		
		float seaHeight = getSLInPixels();
		
		float heightDif = clampf((terHeight-seaHeight)/(16.0*pixelsPerCell), 0.0, 1.0);
		
		float isUnderWater = 0.0f;
		
		if (cameraPos->getFZ() < seaHeight) {
			isUnderWater = 1.0f;	
		}
		
		
		
		music[EML_BIRDSONG0]->setVolume(masterVolume*ambientVolume*timeOfDay*heightDif*(1.0-isUnderWater));
		music[EML_CRICKETS0]->setVolume(masterVolume*ambientVolume*(1.0f-timeOfDay)*heightDif*(1.0-isUnderWater));
		music[EML_OCEANWAVES0]->setVolume(masterVolume*ambientVolume*(1.0f-heightDif)*(1.0-isUnderWater));
		music[EML_UNDERWATER0]->setVolume(masterVolume*ambientVolume*(isUnderWater));
		
		
	}
void Singleton::frameUpdate ()
                           {
		
		cameraZoom += (targetZoom - cameraZoom) / (4.0f);
		
		if (cameraZoom < 1.0f) {
			cameraZoom = 1.0f;
		}
		
		float hs = holderSizeInPixels;
		
		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);
		float scrollAmount = 0.0f;
		
		updateAmbientSounds();
		if ((mainGUI != NULL)) {
			if (mainGUI->isReady) {
				//mainGUI->testOver(guiX, guiY);
			}
			
		}
		
		
		if (autoScroll) {
			cameraPos->copyFrom(&baseScrollPos);
			scrollAmount = scrollTimer.getElapsedTimeInMilliSec()/scrollDiv;
			cameraPos->addXYZ(-scrollAmount,-scrollAmount,0.0f);
			
			// cameraPosAdjusted.copyFrom(cameraPos);
			// setObjToElevationBase(&cameraPosAdjusted);
			// cameraPos->addXYZ(16.0*pixelsPerCell,16.0*pixelsPerCell,0.0f);
			//syncObjects(&cameraPosAdjusted);
			
			//syncObjects(cameraPos);
			
			
			
			
		}
		else {
			if (
				(
					readyForRestart &&
					(guiLock == false)
					//&& (gw->curLoadRadius > minWInPages)	
				)
				||
				forceRestart
			) {
				
				if (forceRestart) {
					forceRestart = false;			
				}
				readyForRestart = false;
				guiLock = true;
				restartGen(false, false);
				guiLock = false;
				
			}
			
			updateGUI();
		}
		
		handleMovement();
		
		
		if (
			(gw->mapLockOn) ||
			(mapInvalid)
		) {
			
		}
		else {
			gw->update();
		}
		
		
		syncObjects(cameraPos);
		
		
		TRACE_ON = false;
		
		
		frameCount++;
	}
void Singleton::display ()
        {

		curTime = myTimer.getElapsedTimeInMilliSec();

		float elTime = curTime - lastTime;
		
		float fMouseVel;

		if (myWS == NULL)
		{

		}
		else
		{

			if (myWS->dataReady)
			{

				if (myWS->isJSON)
				{
					if ( processJSON( &(myWS->recBuffer), &lastJSONBuffer, &rootObjJS ) )
					{
						saveAllData();

					}
				}
				else
				{
					processB64(  &(myWS->recBuffer), &lastImageBuffer );

				}

				

				myWS->dataReady = false;
				myWS->isWorking = false;
			}
		}




		if (elTime >= ( msPerFrame  )  ) {

			if (firstRun)
			{
				
			}
			else
			{
				if ( (frameCount % 120) == 0)
				{
					gw->updateLights();
				}
			}




			lastTime = curTime;


			timeOfDay += (targetTimeOfDay - timeOfDay) / 8.0;


			mouseMovingLoc = (mouseMovingLoc + 1) % mouseMovingSize;
			mouseCount++;
			mouseMoving[mouseMovingLoc].setIXY(lastMouseX, lastMouseY);


			lastModXYZ.multXYZ(0.95f);
			mouseVel.multXYZ(0.95f);
			fMouseVel = mouseVel.distance(&origin);



			

			if (shadersAreLoaded)
			{

				if (traceOn)
				{
					traceOn = false;
					TRACE_ON = true;
				}
				if (updateLock)
				{

				}
				else
				{
					frameUpdate();
				}
			}

			
		}

		if (firstRun)
		{
			//toggleFullScreen();
		}

		firstRun = false;

		//doTrace( "POSSIBLE ERROR: " , i__s(glGetError()) , "\n" );

	}
void Singleton::setMatrices (int w, int h)
        {
		int i;
		float* ptr1;
		float* ptr2;
		
		if (perspectiveOn) {
			glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
			glMatrixMode (GL_PROJECTION); //set the matrix to projection
			glLoadIdentity ();
			
			gluPerspective (
				FOV,
				(GLfloat)w / (GLfloat)h,
				mainCamera->clipDist[0],
				mainCamera->clipDist[1]
			); //set the perspective (angle of sight, width, height, , depth)
			glMatrixMode (GL_MODELVIEW); //set the matrix back to model
			
			
			glLoadIdentity();
			glRotatef(mainCamera->rotation[1],1.0,0.0,0.0);
			glRotatef(mainCamera->rotation[0],0.0,0.0,1.0);
			glTranslated(
				-mainCamera->unitPos[0],
				-mainCamera->unitPos[1],
				-mainCamera->unitPos[2]
			);
			
			
			
			glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix.get());
			glGetFloatv(GL_PROJECTION_MATRIX, projMatrix.get());
			
			ptr1 = viewMatrix.get();
			ptr2 = projMatrix.get();
			
			
			for (i = 0; i < 16; i++) {
				viewMatrixD[i] = ptr1[i];
				projMatrixD[i] = ptr2[i];
			}
			
			glGetIntegerv(GL_VIEWPORT, viewport);
			
			
			heightOfNearPlane =
				(
					((float)abs(viewport[3]-viewport[1])) /
			    	(2.0f*tan(0.5f*FOV*M_PI/180.0f))
			    ) *
			    (
			    	2.0f*voxelSizeInWC / ((float)scaleFactor)	
			    );
			    
			
			
			// lastW = -1; 
			// lastH = -1;
			
		}
		else {
			if (
				(lastW == w) && 
				(lastH == h)
			) {
				
			}
			else {
				glViewport(0, 0, w, h);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity ();
				glMatrixMode (GL_PROJECTION);
				glLoadIdentity ();
				
				lastW = w;
				lastH = h;
			}
		}
		
		

	}
void Singleton::reshape (int w, int h)
        {

		setWH(w, h);

		screenWidth = w;
		screenHeight = h;
		
		setMatrices(baseW, baseH);
	}
void Singleton::idleFunc ()
        {

	}
#undef LZZ_INLINE
 
