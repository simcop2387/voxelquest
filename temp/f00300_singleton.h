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
		gamePhysics = NULL;
		allInit = false;
		
		fboMap.clear();
		shaderMap.clear();
		soundMap.clear();
		styleSheetMap.clear();
		
		mainGUI = NULL;
		gw = NULL;
		
		
		
		
		// #ifdef USE_POCO
		// 	myWS = NULL;
		// #endif
		
	}
FIVector4 * Singleton::BTV2FIV (btVector3 btv)
                                          {
		btvConv.setFXYZ(btv.getX(),btv.getY(),btv.getZ());
		return &btvConv;
	}
void Singleton::init (int _defaultWinW, int _defaultWinH, int _scaleFactor)
          {

		pushTrace("Singleton init");
		int i;
		int j;
		int k;
		int m;
		int ind;
		float tempf;
		
		RUN_COUNT = 0;
		TEMP_DEBUG = false;
		
		initNetMasks();
		
		
		if (DO_RANDOMIZE) {
			// todo: get rid of this for random seeds, causes desync
			srand(time(NULL));
			RAND_COUNTER = rand();
			RAND_COUNTER2 = rand();
		}
		
		
		// todo: mem leak, should delete?
		
		for (i = 0; i < E_SDT_LENGTH; i++) {
			externalJSON[E_SPECIAL_DATA_TYPE_STRINGS[i]].jv = NULL;
		}
		
		externalJSON["kb.js"].jv = NULL;
		
		
		for (i = 0; i < EML_LENGTH; i++) {
			music[i] = new GameMusic();
			music[i]->init("..\\data\\music\\"+musicStrings[i]+".ogg");
			music[i]->setLoop(true);
		}
		
		for (i = 0; i < 6; i++) {
			dirVecs[i].setFXYZ(
				DIR_VECS[i][0],
				DIR_VECS[i][1],
				DIR_VECS[i][2]
			);
		}
		
		
		
		initAllMatrices();
		
		for (i = 0; i < 16; i++) {
			ZERO_FLOATS[i] = 0.0f;
		}
		
		for (i = 0; i < 32; i++) {
			fsQuad.vertexVec.push_back(vertexDataQuad[i]);
		}
		for (i = 0; i < 6; i++) {
			fsQuad.indexVec.push_back(indexDataQuad[i]);
		}
		fsQuad.init(
			2,
			GL_STATIC_DRAW
		);
		fsQuad.updateNew();
		
		
		colVecs[0].setFXYZ(255,0,0);
		colVecs[1].setFXYZ(0,255,0);
		colVecs[2].setFXYZ(0,0,255);
		colVecs[3].setFXYZ(255,255,0);
		colVecs[4].setFXYZ(255,0,255);
		colVecs[5].setFXYZ(0,255,255);
		colVecs[6].setFXYZ(128,0,0);
		colVecs[7].setFXYZ(0,128,0);
		colVecs[8].setFXYZ(0,0,128);
		colVecs[9].setFXYZ(128,128,0);
		colVecs[10].setFXYZ(128,0,128);
		colVecs[11].setFXYZ(0,128,128);
		colVecs[12].setFXYZ(255,128,128);
		colVecs[13].setFXYZ(128,255,128);
		colVecs[14].setFXYZ(128,128,255);
		colVecs[15].setFXYZ(255,255,128);
		
		
		camRotation[0] = 0.0f;
		camRotation[1] = M_PI/2.0f;
		
		curCamRotation[0] = camRotation[0];
		curCamRotation[1] = camRotation[1];
		
		
		for (i = 0; i < KEYMAP_LENGTH; i++) {
			keyMapCoolDown[i] = 0;
			keyMapMaxCoolDown[i] = 0;
		}
		
		
		for (i = 0; i < E_CT_LENGTH; i++) {
			isInteractiveEnt[i] = false;
		}
		isInteractiveEnt[E_CT_WINDOW] = true;
		isInteractiveEnt[E_CT_DOOR] = true;
		isInteractiveEnt[E_CT_LANTERN] = true;
		
		
		
		lightVec.setFXYZ(0.3f,0.4f,-1.0f);
		lightVec.normalize();
		lightVecOrig.copyFrom(&lightVec);
		
		//totTimePassedGraphics = 0;
		//totTimePassedPhysics = 0;
		
		identMatrix.identity();
		
		
		destructCount = 0;
		
		sphereMapOn = false;
		waitingOnDestruction = false;
		
		physicsOn = true;
		isPressingMove = false;
		fxaaOn = false;
		doPathReport = false;
		refreshPaths = false;
		placingTemplate = true;
		smoothMove = true;
		waterBulletOn = false;
		ignoreFrameLimit = false;
		autoMove = false;
		inputOn = false;
		isMacro = false;
		
		cavesOn = false;
		bakeParamsOn = true;
		dragging = false;
		mouseMoved = false;
		hitGUI = false;
		guiDirty = true;
		
		
		lastMouseOrigX = 0.0f;
		lastMouseOrigY = 0.0f;
		
		threadNetSend.init();
		threadNetRecv.init();
		
		
		for (i = 0; i < E_FM_LENGTH; i++) {
			menuList[i] = NULL;
		}
		
		// for (k = 0; k < VORO_PITCH; k++) {
		// 	for (j = 0; j < VORO_PITCH; j++) {
		// 		for (i = 0; i < VORO_PITCH; i++) {
		// 			voroArr[k*VORO_PITCH*VORO_PITCH + j*VORO_PITCH + i] = 
		// 			Vector3(
		// 				(fGenRand2()-0.5f)*2.0f*VORO_VARIANCE + 0.5f,
		// 				(fGenRand2()-0.5f)*2.0f*VORO_VARIANCE + 0.5f,
		// 				(fGenRand2()-0.5f)*2.0f*VORO_VARIANCE + 0.5f
		// 			);
		// 		}
		// 	}
		// }
		
		getVoroOffsets();
		
		
		mapComp = NULL;
		fieldText = NULL;
		selectedEnt = NULL;
		highlightedEnt = NULL;
		draggingMap = false;
		
		MAX_TRAVEL_DIS = 10.0f;
		FOV = 45.0f;
		focalLength = 1.0f / tan(FOV / 2.0f);

		totalPointCount = 0;
		
		
		
		curOrgId = 0;
		
		rootObjJS = NULL;
		guiRootJS = NULL;
		constRootJS = NULL;
		rbStack = NULL;
		rbHeightStack = NULL;

		multiLights = false;
		numDynLights = E_OBJ_LENGTH - E_OBJ_LIGHT0;
		updateMultiLights();

		maxLayerOver = -1;

		iNumSteps = 16;
		pathFindingStep = 0;

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

		
		limbTBO.init(true, limbTBOData, NULL, MAX_LIMB_DATA_IN_BYTES);
		
		numLights = MAX_LIGHTS;//min(MAX_LIGHTS,E_OBJ_LENGTH-E_OBJ_LIGHT0);


		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////

		

		lastNodeId = 0;
		escCount = 0;

		initStyleSheet();

		curMoveSpeed = 0.0f;
		curMoveAccel = 0.0f;
		
		masterVolume = 0.0f;
		ambientVolume = 0.0f;
		guiVolume = 0.0f;
		musicVolume = 0.0f;
		fxVolume = 0.0f;
		
		curPrimMod = 0.0f;
		curPrimTemplate = 1;
		geomStep = 0;

		drawOrient = false;
		applyToChildren = false;
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
		placingGeom = false;
		
		
		
		
		
		/////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////
		
				
		// IMPORTANT: Maximum height must be less than 2^16, max world pitch must be less than 2^32
		
		medianCount = 0;
		currentTick = 0;
		earthMod = E_PTT_TER;
		
		moveTimer.start();
		
		
		matVolDim.setIXYZ(64,64,256);
		matVolSize = matVolDim.getIX()*matVolDim.getIY()*matVolDim.getIZ();
		matVol = new uint[matVolSize];
		matSlice0 = new materialNode[matVolDim.getIX()*matVolDim.getIY()];
		matSlice1 = new materialNode[matVolDim.getIX()*matVolDim.getIY()];
		matSlice2 = new materialNode[matVolDim.getIX()*matVolDim.getIY()];
		
		for (i = 0; i < matVolSize; i++) {
			matVol[i] = 0;
		}
		
		
		diagCount = 0;
		
		
		
		
		
		// was doing: lerp from start to end postions, worlspace per pixel
		
		// was doing: exmaine why fluidchange 66 33
		
		// was doing: silouhette rendering?
		
		// todo: compress volumes into stacks of lines
		// todo: rasterize voxels with tris for depth cache
		// todo - generate silouhette by expanding position of edges with high differential
		// map right in the center of these edges to inexpensively determine silouhette
		
		// todo: probe area ahead of current ray step to see if near edge
		
		
		
		
		
		curPattern = E_PAT_5X5DIAMOND;
		curPatternRot = 0;
		
		generatePatterns();
		
		bakeTicks = 0;
		tbTicks = 0;
		tempCounter = 0;
		actorCount = 0;
		polyCount = 0;
		fpsCountMax = 500;
		
		fpsTest = false;
		pathfindingOn = false;
		pathfindingGen = false;
		pathfindingTestOn = false;
		updateHolders = false;
		updateFluid = false;
		
		maxHolderDis = 32;
		heightOfNearPlane = 1.0f;
		scrollDiv = 2.0;
		frameSkip = 32;
		frameSkipCount = 0;
		
		int bufferDiv = 1;
		
		if (DO_POINTS) {
			glDisable(GL_POINT_SMOOTH);
			glEnable(GL_POINT_SPRITE);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		}
		
		glLineWidth(4.0f);
		
		// This var determines how high the z val can go,
		// but also increases load times for each block
		
		
		// qqqqqq
		
		
		heightMapMaxInCells = 4096.0f;
		//mapSampScale = 2.0f;
		int newPitch = (imageHM0->width) * 2; //*2;
		mapPitch = (imageHM0->width); //newPitch;// //
		
		
		voxelsPerCell = VOXELS_PER_CELL;
		paddingInCells = 1;
		
		cellsPerHolder = 16;
		holdersPerBlock = 8;
		
		holdersPerWorld = newPitch;
		cellsPerWorld = holdersPerWorld*cellsPerHolder;
		cellsPerBlock = holdersPerBlock * cellsPerHolder;
		blocksPerWorld = holdersPerWorld/holdersPerBlock;
		
		
		
		
		cellsPerHolderPad = cellsPerHolder+paddingInCells*2;
		voxelsPerHolderPad = voxelsPerCell*cellsPerHolderPad;
		voxelsPerHolder = voxelsPerCell*cellsPerHolder;
		
		for (i = 0; i < MAX_PDPOOL_SIZE; i++) {
			pdPool[i].data = new PaddedDataEntry[cellsPerHolderPad*cellsPerHolderPad*cellsPerHolderPad];
			//pdPool[i].voxData = new VoxEntry[voxelsPerCell*voxelsPerCell*voxelsPerCell]
			pdPool[i].isFree = true;
			pdPool[i].voxelBuffer.pitch = voxelsPerHolderPad;
			pdPool[i].voxelBuffer.totSize = voxelsPerHolderPad*voxelsPerHolderPad*voxelsPerHolderPad;
			pdPool[i].voxelBuffer.data = new VoxelBufferEntry[
				pdPool[i].voxelBuffer.totSize
			];
			
			// octPool[i] = new GameOctree();
			// octPool[i]->init(
			// 	this,
			// 	cellsPerHolder*voxelsPerCell*2,
			// 	1024*1024*2
			// );
		}
		
		for (i = 0; i < MAX_TBOPOOL_SIZE; i++) {
			tboPool[i].init(128*1024*1024);
		}
		
		
		if (blocksPerWorld > 256) {
			cout << "Too many blocks in world, change holdersPerBlock\n";
			exit(0);
		}
		
		
		
		
		globWheelDelta = 0.0f;
		amountInvalidMove = 0.0f;
		amountInvalidRotate = 0.0f;
		sphereMapPrec = 0.0f;
		
		cellsPerNodeXY = 16;
		cellsPerNodeZ = 8;
		
		
		
		clipDist[0] = 1.0f;
		clipDist[1] = 512.0f;
		
		terDataTexScale = 1;
		
		terDataVisPitchXY = cellsPerBlock / cellsPerNodeXY;
		iNodeDivsPerHolder = terDataVisPitchXY / holdersPerBlock;
		terDataBufAmount = iNodeDivsPerHolder; // pad with one extra lot
		
		floorHeightInCells = (float)cellsPerNodeZ;
		roofHeightInCells = ((float)cellsPerNodeXY)/4.0f;
		wallRadInCells = ((float)cellsPerNodeXY)/4.0f;
		
		
		terDataBufPitchXY = terDataVisPitchXY + terDataBufAmount * 2;
		terDataBufPitchScaledXY = terDataBufPitchXY * terDataTexScale;
		
		terDataVisPitchZ = cellsPerBlock / cellsPerNodeZ;
		terDataBufPitchZ = terDataVisPitchZ + terDataBufAmount * 2;
		terDataBufPitchScaledZ = terDataBufPitchZ * terDataTexScale;

		terDataVisSize = terDataVisPitchXY * terDataVisPitchXY * terDataVisPitchZ;
		terDataBufSize = terDataBufPitchXY * terDataBufPitchXY * terDataBufPitchZ;
		terDataBufSizeScaled = terDataBufPitchScaledXY * terDataBufPitchScaledXY * terDataBufPitchScaledZ;


		terDataScaled = new uint[terDataBufSizeScaled];
		rbStack = new int[terDataBufSize];
		rbHeightStack = new int[terDataBufSize];


		
		
		timeDelta = 0.0;
		
		mdTimeLR[0] = 0.0f;
		mdTimeLR[1] = 0.0f;
		clickTimeLR[0] = 0.0f;
		clickTimeLR[1] = 0.0f;
		
		
		
		
		
		
		


		
		blockShift = intLogB2(holdersPerBlock * holdersPerBlock * holdersPerBlock);
		holderSizeMB = 0;







		cout << "\n\n\n\n\n\n";


		
		cout << "cellsPerNodeXY: " << cellsPerNodeXY << "\n";
		cout << "cellsPerNodeZ: " << cellsPerNodeZ << "\n";
		
		cout << "holdersPerBlock: " << holdersPerBlock << "\n";
		cout << "cellsPerBlock: " << cellsPerBlock << "\n";
		cout << "terDataBufPitchScaledXY " << terDataBufPitchScaledXY << "\n";
		cout << "cellsPerHolder: " << cellsPerHolder << "\n";

		cout << "cellsPerWorld: " << cellsPerWorld << "\n";
		cout << "blocksPerWorld: " << blocksPerWorld << "\n";


		GLint glQuery;
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &glQuery);
		cout << "MAX_UNIFORMS: " << glQuery << "\n";

		cout << "\n\n\n\n\n\n";


		mapFreqs.setFXYZW(
			1.0f,
			4.0f,
			16.0f,
			32.0f
		);
		mapAmps.setFXYZW(
			16.0f/16.0f,
			2.0f/16.0f,
			1.0f/16.0f,
			0.25f/16.0f
		); //0.0f, 0.0f, 0.0f);//


		


		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		radiosityOn = true;
		testOn = false;
		testOn2 = false;
		testOn3 = false;
		updateLock = false;
		traceOn = false;
		frameMouseMove = false;
		depthInvalidMove = true;
		lastDepthInvalidMove = true;
		depthInvalidRotate = true;
		drawTargPaths = false;
		renderingOct = false;
		renderingOctBounds = false;
		placingPattern = false;
		gridOn = false;
		fogOn = 1.0f;
		cameraZoom = 1.0f;
		targetZoom = cameraZoom;
		

		zoomDelta = 0.0f;
		subjectDelta = 0.0f;
		subjectZoom = 1.0f;
		targetSubjectZoom = 1.0f;

		int ccr = 0;
		int ccg = 0;
		int ccb = 0;
		int curFilter;

		
		
		gameNetwork = new GameNetwork();
		gameNetwork->init(this);
		
		//if (!(gameNetwork->isConnected)) {
		//if (DO_CONNECT) {
			gameNetwork->socketConnect(true);
		//}
		
		
		for (i = 0; i < E_FID_LENGTH; i++) {
			gameFluid[i] = new GameFluid();
			gameFluid[i]->init(this, i);
		}
		
		
		// must be done after all are init
		for (i = 0; i < E_FID_LENGTH; i++) {
			gameFluid[i]->updateTBOData(true, false);
		}
		
		
		
		
		
		gameLogic = new GameLogic();
		gameLogic->init(this);


		// for (i = 0; i < MAX_TER_TEX; i++)
		// {

		// 	terTextures[i].usedByBlockId = -1;
		// 	terTextures[i].alreadyBound = false;


		// 	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, 0);

		// 	glGenTextures(1, &(terTextures[i].texId));
		// 	glBindTexture(GL_TEXTURE_3D, terTextures[i].texId);
		// 	glTexImage3D(
		// 		GL_TEXTURE_3D,
		// 		0,
		// 		GL_RGBA,
		// 		terDataBufPitchScaledXY,
		// 		terDataBufPitchScaledXY,
		// 		terDataBufPitchScaledZ,
		// 		0,
		// 		GL_RGBA,
		// 		GL_UNSIGNED_BYTE,
		// 		0
		// 	);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// 	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// 	glBindTexture(GL_TEXTURE_3D, 0);

		// 	TOT_GPU_MEM_USAGE += ((float)(terDataBufSizeScaled * 4)) / (1024.0f * 1024.0f);
		// }

		


		glGenTextures(1, &volIdMat);
		glBindTexture(GL_TEXTURE_3D, volIdMat);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, matVolDim.getIX(), matVolDim.getIY(), matVolDim.getIZ(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_3D, 0);

		
		paramArr = new float[4096];
		paramArrGeom = new float[128];
		splashArr = new float[MAX_SPLASHES*4];
		explodeArr = new float[MAX_EXPLODES*4];
		voroArr = new float[27 * 4];
		matCountArr = new float[256];
		paramArrMap = new float[4096];

		
		
		


		showMap = true;



		lastImageBuffer.data = NULL;
		lastImageBuffer.size = 0;

		//lastJSONBuffer.data = NULL;
		//lastJSONBuffer.size = 0;
		
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
		
		// #ifdef USE_POCO
		// 	myWS = _myWS;
		// #endif
		

		

		isBare = true;



		defaultWinW = _defaultWinW / _scaleFactor;
		defaultWinH = _defaultWinH / _scaleFactor;
		scaleFactor = _scaleFactor;
		aspectRatio = ((float)_defaultWinW)/((float)_defaultWinH);
		
		guiWinW = _defaultWinW / UI_SCALE_FACTOR;
		guiWinH = _defaultWinH / UI_SCALE_FACTOR;

		origWinW = _defaultWinW;
		origWinH = _defaultWinH;

		curBrushRad = 2.0f;
		mouseState = E_MOUSE_STATE_MOVE;








		wsBufferInvalid = true;

		// x*y must == z*z*z
		

		bufferDim.setIXY(defaultWinW, defaultWinH);
		bufferDimTarg.setIXY(defaultWinW, defaultWinH);
		
		bufferDimHalf.setIXY(defaultWinW / 2, defaultWinH / 2);
		bufferModDim.copyIntMult(&bufferDim,1);
		bufferRenderDim.copyIntDiv(&bufferDimTarg,RENDER_SCALE_FACTOR);


		myVBOGrid.init(bufferRenderDim.getIX(), bufferRenderDim.getIY());



		myTimer.start();


		activeObject = E_OBJ_CAMERA;

		extraRad = 0;
		//lastTime = 0.0;





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

		dynObjects[E_OBJ_CAMERA]->init(0, 0, 0, 0, 0, 0, false, E_MT_NONE, NULL, 4.0f );

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
				tempf = 128.0f;
			}
			else {
				tempf = 16.0f;
			}

			dynObjects[i]->init(
				-16 + i * 2,
				-16 + i * 2,
				4,
				ccr, ccg, ccb,
				true,
				E_MT_RELATIVE,
				&(dynObjects[E_OBJ_CAMERA]->pos),
				0.125f,
				tempf
			);



		}
		
		dynObjects[E_OBJ_LIGHT0]->moveType = E_MT_TRACKBALL;
		
		
		// dynObjects[E_OBJ_LIMBTARG0]->init(
		// 	-16 + 3 * 2,
		// 	-16 + 3 * 2,
		// 	4,
		// 	255, 0, 0,
		// 	true,
		// 	E_MT_TRACKBALL,
		// 	&(dynObjects[E_OBJ_CAMERA]->pos),
		// 	0.5f,
		// 	16.0f
		// );
		
		
		cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);
		
		
		//dynObjects[E_OBJ_FOG]->init(-1024*2, -1024*2, -1024/2,   0, 0, 255,     true, E_MT_RELATIVE, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		//dynObjects[E_OBJ_CUTAWAY]->init(4096*4 - 256, 4096*4 - 256 + 2048, 4096*4,   0, 255, 0,     true, E_MT_RELATIVE, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		//dynObjects[E_OBJ_HUMAN]->init(2048, 2048, -1024,   0, 255, 255,     true, E_MT_TRACKBALL, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );

		// dynObjects[E_OBJ_P0]->init(512-256,1024-256,2048,   128,0,0,    true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		// dynObjects[E_OBJ_P1]->init(512,1024,2048,      255,0,0,  true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		// dynObjects[E_OBJ_P2]->init(1024,512,2048,      0,255,255,  true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );



		mouseX = 0.0f;
		mouseY = 0.0f;
		mouseXUp = 0.0f;
		mouseYUp = 0.0f;

		geomOrigOffset.setFXYZ(0.0f);






		
		frameCount = 0;
		forceGetPD = false;
		mapInvalid = true;
		notQuit = true;
		timerNotSet = true;
		screenWidth = defaultWinW;
		screenHeight = defaultWinH;
		mouseLeftDown = mouseRightDown = false;
		mouseX = mouseY = 0;



		
		glDepthFunc(GL_LESS);
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
		

		
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
		
		
		if (USE_SPHERE_MAP) {
			shaderStrings.push_back("PrimShader_330_DOTER_USESPHEREMAP");
		}
		else {
			//shaderStrings.push_back("PrimShader_330_DOTER_DOPOLY");
			shaderStrings.push_back("PrimShader_330_DOTER");
		}
		shaderStrings.push_back("PrimShader_330_DOPRIM");
		
		
		
		shaderStrings.push_back("SolidCombineShader");
		shaderStrings.push_back("CylBBShader");
		shaderStrings.push_back("FXAAShader");
		shaderStrings.push_back("TerGenShader");
		shaderStrings.push_back("GUIShader");
		shaderStrings.push_back("MedianShader");
		shaderStrings.push_back("MergeShader");
		shaderStrings.push_back("TopoShader");
		shaderStrings.push_back("PointShader");
		shaderStrings.push_back("RoadShader");
		shaderStrings.push_back("SkeletonShader");
		shaderStrings.push_back("DilateShader");
		shaderStrings.push_back("TerrainMix");
		shaderStrings.push_back("Simplex2D");
		shaderStrings.push_back("WaveHeightShader");
		shaderStrings.push_back("WaterShader");
		shaderStrings.push_back("WaterShaderCombine");
		shaderStrings.push_back("CopyShader");
		shaderStrings.push_back("CopyShader2");
		shaderStrings.push_back("NoiseShader");
		shaderStrings.push_back("MapBorderShader");
		shaderStrings.push_back("BillboardShader");
		shaderStrings.push_back("PreLightingShader");
		shaderStrings.push_back("PostLightingShader");
		shaderStrings.push_back("BlurShader");
		shaderStrings.push_back("RadiosityShader");
		shaderStrings.push_back("RadiosityCombineShader");
		shaderStrings.push_back("FogShader");
		shaderStrings.push_back("OctShader");
		shaderStrings.push_back("RasterShader");
		shaderStrings.push_back("HolderShader");
		shaderStrings.push_back("GridShader");
		shaderStrings.push_back("GeomShader");
		shaderStrings.push_back("BoxShader");
		shaderStrings.push_back("PolyShader");
		shaderStrings.push_back("PolyCombineShader");
		//shaderStrings.push_back("SphereShader");
		//shaderStrings.push_back("BlitPointShader");
		//shaderStrings.push_back("HDRShader");
		//shaderStrings.push_back("DownScaleShader");
		//shaderStrings.push_back("CombineShader");
		//shaderStrings.push_back("GenerateVolume");
		//shaderStrings.push_back("GenerateVolumeEnt");
		//shaderStrings.push_back("RenderVolume");

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
		
		
		
		/////////////////////////
		/////////////////////////
		
		smoothTime = 0.0f;
		timeMod = true;
		resultShake = 0.0f;
		cameraShake = 0.0f;
		lastx = 0;
		lasty = 0;
		isMoving = false;
		updateMatFlag = false;
		matVolLock = false;
		perspectiveOn = false;
		lastPersp = false;


		for (i = 0; i < MAX_KEYS; i++) {
			keysPressed[i] = false;
			keyDownTimes[i] = 0.0;
			//keyUpTimes[i] = 0.0;
		}

		keyMap[KEYMAP_UP] = 'a';
		keyMapMaxCoolDown[KEYMAP_UP] = 3;
		keyMap[KEYMAP_DOWN] = 'z';
		keyMapMaxCoolDown[KEYMAP_DOWN] = 3;
		keyMap[KEYMAP_FORWARD] = 'e';
		keyMap[KEYMAP_BACKWARD] = 'd';
		keyMap[KEYMAP_LEFT] = 's';
		keyMap[KEYMAP_RIGHT] = 'f';
		keyMap[KEYMAP_FIRE_PRIMARY] = '6';
		keyMapMaxCoolDown[KEYMAP_FIRE_PRIMARY] = 20;
		keyMap[KEYMAP_GRAB] = 'w';
		keyMapMaxCoolDown[KEYMAP_GRAB] = 200;
		keyMap[KEYMAP_THROW] = 'y';
		keyMapMaxCoolDown[KEYMAP_THROW] = 200;
		
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
		
		// fboMap["frontFaceFBO"].init(1, faceDim, faceDim, 4, false, GL_NEAREST);
		// fboMap["backFaceFBO"].init(1, faceDim, faceDim, 4, false, GL_NEAREST);
		// fboMap["frontFaceMapFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_NEAREST);
		// fboMap["backFaceMapFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_NEAREST);
		//fboMap["palFBO"].init(1, palWidth, palHeight, 1, false, GL_LINEAR);
		
		bool fboHasDepth = true;
		int numChannels = 4;
		int numMaps = 2;
		
		int tx = 0;
		int ty = 0;
		int tz = 0;
		int clampType;
		
		int vwChan = 1;
		bool doProc;
		int filterType;
		
		for (i = 0; i < E_VW_LENGTH; i++) {
			
			filterType = GL_LINEAR;
			vwChan = 1;
			doProc = true;
			
			switch (i) {
				
				// tx = 4096;
				// ty = 4096;
				// tz = 256;
				
				// tx = 2048;
				// ty = 1024;
				// tz = 128;
				case E_VW_HOLDER:
					tz = cellsPerHolder;
					clampType = GL_CLAMP_TO_EDGE; //GL_CLAMP_TO_BORDER
				break;
				case E_VW_WORLD:
					
					// vwChan = 4;
					// filterType = GL_NEAREST,
					// tz = blocksPerWorld;
					// clampType = GL_REPEAT; //GL_CLAMP_TO_BORDER
					// if (!GEN_POLYS_WORLD) {
					 	doProc = false;
					// }
					
				break;
				// case E_VW_TERGEN:
				// 	tz = 128;
				// 	clampType = GL_CLAMP_TO_EDGE;//GL_CLAMP_TO_BORDER
				// break;
				case E_VW_VORO:
					tz = 128;
					clampType = GL_REPEAT;
				break;
			}
			
			if (doProc) {
				volumeWrappers[i] = new VolumeWrapper();
				volumeWrappers[i]->init(tz, clampType, (vwChan==4), filterType ); //volumeWrapperStrings[i]
				//fboMap[volumeWrapperStrings[i]].init(1, tx, ty, vwChan, false);
			}
			
		}
		
		
		
		
		fboMap["prelightFBO"].init(4, bufferDimTarg.getIX(), bufferDimTarg.getIY(), 1, false, GL_LINEAR);
		
		// fboMap["allTargFBO"].init(6, bufferRenderDim.getIX(), bufferRenderDim.getIY(), numChannels, fboHasDepth);
		// fboMap["allDepthFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);
		
		
		fboMap["prmTargFBO"].init(8, bufferRenderDim.getIX(), bufferRenderDim.getIY(), numChannels, fboHasDepth);
		fboMap["prmDepthFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);
		
		
		fboMap["numstepsFBO"].init(1, bufferRenderDim.getIX(), bufferRenderDim.getIY(), numChannels, fboHasDepth);
		
		fboMap["terTargFBO"].init(8, bufferRenderDim.getIX(), bufferRenderDim.getIY(), numChannels, fboHasDepth);
		fboMap["limbFBO"].init(1, bufferRenderDim.getIX(), bufferRenderDim.getIY(), numChannels, fboHasDepth);
		fboMap["terDepthFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);
		
		for (i = 0; i <= NUM_POLY_STRINGS; i++) {
			fboMap[polyFBOStrings[i]].init(1, bufferRenderDim.getIX(), bufferRenderDim.getIY(), 4, true);
		}
		
		
		fboMap["rasterFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 4, true, GL_NEAREST);
		
		fboMap["rasterPosFBO"].init(1, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);//, GL_REPEAT);
		fboMap["rasterSourceFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST);//, GL_REPEAT);
		
		
		
		
		if (USE_SPHERE_MAP) {
			fboMap["sphTargFBO"].init(8, bufferRenderDim.getIX()*SPHEREMAP_SCALE_FACTOR, bufferRenderDim.getIY()*SPHEREMAP_SCALE_FACTOR, numChannels, fboHasDepth);
			fboMap["sphDepthFBO"].init(numMaps, bufferDimTarg.getIX()*SPHEREMAP_SCALE_FACTOR, bufferDimTarg.getIY()*SPHEREMAP_SCALE_FACTOR, numChannels, fboHasDepth, GL_LINEAR, GL_REPEAT);
		}
		
		
		fboMap["solidBaseTargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
		fboMap["solidTargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
		
		
		fboMap["waterTargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
		
		
		
		fboMap["swapTargFBO0"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, false);
		fboMap["swapTargFBO1"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, false);
		
		
		fboMap["geomBaseTargFBO"].init( numMaps+1, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, true);
		fboMap["geomTargFBO"].init(     numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, true);
		fboMap["combineWithWaterTargFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth);
		
		fboMap["debugTargFBO"].init(     numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, true);
		
		
		
		
		
		
		fboMap["noiseFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["noiseFBOLinear"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR, GL_REPEAT);
		//fboMap["guiFBO"].init(1, guiWinW, guiWinH, 1, false, GL_LINEAR);
		fboMap["resultFBO0"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST);
		fboMap["resultFBO1"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST);
		fboMap["lastFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST);
		//fboMap["waveFBO"].init(1, bufferDim.getIX()/2, bufferDim.getIY()/2, 1, false, GL_LINEAR, GL_MIRRORED_REPEAT);
		fboMap["swapFBOLin0"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);
		fboMap["swapFBOLin1"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);
		fboMap["swapFBOBLin0"].init(1, bufferDim.getIX() / bufferDiv, bufferDim.getIY() / bufferDiv, 1, false, GL_LINEAR);
		fboMap["swapFBOBLin1"].init(1, bufferDim.getIX() / bufferDiv, bufferDim.getIY() / bufferDiv, 1, false, GL_LINEAR);
		fboMap["swapFBOLinHalf0"].init(1, bufferDim.getIX() / bufferDiv, bufferDim.getIY() / bufferDiv, 1, false, GL_LINEAR);
		fboMap["swapFBOLinHalf1"].init(1, bufferDim.getIX() / bufferDiv, bufferDim.getIY() / bufferDiv, 1, false, GL_LINEAR);
		fboMap["cityFBO"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["hmFBO"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["hmFBOLinear"].init(1, newPitch, newPitch, 1, false, GL_LINEAR, GL_REPEAT);
		fboMap["hmFBOLinearBig"].init(1, mapPitch, mapPitch, 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["simplexFBO"].init(1, newPitch, newPitch, 1, false, GL_LINEAR, GL_REPEAT);
		fboMap["swapFBO0"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["swapFBO1"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
		
		
		loadConstants();
		
		// LEAVE THIS IN FOR VSYNC
		myDynBuffer = new DynBuffer();


		fontWrappers[EFW_ICONS] = new FontWrapper();
		fontWrappers[EFW_ICONS]->init(this, "icons", true, 1.0f, 0.0f);
		
		fontWrappers[EFW_TEXT] = new FontWrapper();
		fontWrappers[EFW_TEXT]->init(this, "arial_regular_14", false, 1.0f);

		mainGUI = new GameGUI();
		mainGUI->init(this);


		//loadAllData();

		GamePlant::initAllPlants(this);
		for (i = 0; i < E_PT_LENGTH/2; i++) {
			gamePlants[i] = new GamePlant();
		}


		//gameOct = new GameOctree();
		//gameOct->init(this, cellsPerWorld, false, true, false, 32*1024*1024);
		
		
		

		gem = new GameEntManager();
		gem->init(this);
		gw = new GameWorld();
		
		
		
		
		//TODO: fix this for proper angle alignment to model
		//orientRotation();
		
		
		gw->init(this);
		gw->initMap();
		
		
		
		//bulletTest();
		
		gameAI = new GameAI();
		gameAI->init(this);
		
		
		// gem->gameObjects[E_OBJ_CAMERA] = BaseObj();
		// gem->gameObjects[E_OBJ_CAMERA].init(
		// 	E_OBJ_CAMERA,
		// 	0,
		// 	0,
		// 	1,
		// 	&lastCellPos,
		// 	5,5,5
		// );
		
		
		music[EML_BIRDSONG0]->play();
		music[EML_CRICKETS0]->play();
		music[EML_OCEANWAVES0]->play();
		music[EML_UNDERWATER0]->play();
		
		loadGUI();
		loadValuesGUI();
		
		if (bakeParamsOn) {
			doShaderRefresh(true);
		}
		
		
		
		
		
		
		

		allInit = true;
		
		doTraceND("GW DONE");


		popTrace();



	}
void Singleton::applyPat (int patInd, int patShape, int rot, int x, int y, int val, int rad)
          {
		int i;
		int j;
		int ind;
		
		bool doProc = false;
		
		int cenX;
		int cenY;
		
		switch(rot) {
			case 0:
				cenX = PATTERN_CENTER + x;
				cenY = PATTERN_CENTER + y;
			break;
			case 1:
				cenX = PATTERN_CENTER - y;
				cenY = PATTERN_CENTER + x;
			break;
			case 2:
				cenX = PATTERN_CENTER - x;
				cenY = PATTERN_CENTER - y;
			break;
			case 3:
				cenX = PATTERN_CENTER + y;
				cenY = PATTERN_CENTER - x;
			break;
		}
		
		cout << "\n\n";
		
		for (j = 0; j < PATTERN_SIZE; j++) {
			cout << "\n";
			for (i = 0; i < PATTERN_SIZE; i++) {
				ind = i + j*PATTERN_SIZE;
				
				doProc = false;
				switch (patShape) {
					case E_PATSHAPE_SQUARE:
						doProc = (
							(abs(i-cenX) <= rad) &&
							(abs(j-cenY) <= rad)
						);
					break;
					case E_PATSHAPE_DIAMOND:
						doProc = (
							(
								abs(i-cenX) +
								abs(j-cenY)	
							) <= rad
						);
					break;
				}
				
				if (doProc) {
					patterns[patInd*4+rot].patternVals[ind] = val;
					cout << "X";
				}
				else {
					cout << "O";
				}
			}
		}
		
	}
void Singleton::getVoroOffsets ()
                              {
		int i;
		int j;
		int k;
		
		for (k = 0; k <= 2; k++) {
			for (j = 0; j <= 2; j++) {
				for (i = 0; i <= 2; i++) {
					voroOffsets[k*9 + j*3 + i] = vec3(
						i - 1,
						j - 1,
						k - 1	
					);
				}
			}
		}
	}
void Singleton::generatePatterns ()
                                {
		int q;
		
		int i;
		int j;
		int k;
		
		int xb;
		int yb;
		
		for (k = 0; k < 4; k++) {
			for (q = 0; q < E_PAT_LENGTH; q++) {
				switch (q) {
					case E_PAT_1X1SQUARE:
						applyPat(q,E_PATSHAPE_SQUARE,k,0,0,1,0);
					break;
					case E_PAT_3X3SQUARE:
						applyPat(q,E_PATSHAPE_SQUARE,k,0,0,1,1);
					break;
					case E_PAT_5X5SQUARE:
						applyPat(q,E_PATSHAPE_SQUARE,k,0,0,1,2);
					break;
					case E_PAT_1X1DIAMOND:
						applyPat(q,E_PATSHAPE_DIAMOND,k,0,0,1,0);
					break;
					case E_PAT_3X3DIAMOND:
						applyPat(q,E_PATSHAPE_DIAMOND,k,0,0,1,1);
					break;
					case E_PAT_5X5DIAMOND:
						applyPat(q,E_PATSHAPE_DIAMOND,k,0,0,1,2);
					break;
					
				}
			}
		}
		
		
		
		
	}
int Singleton::placeInStack ()
                           {
		int curId;
		int stackSize = emptyStack.size();
		if (stackSize > 0) {
			curId = emptyStack.back();
			emptyStack.pop_back();
			//compStack[curId].data = new UIComponent();
			compStack[curId].isValid = true;
			
			
			return curId;
		}
		else {
			compStack.push_back( CompStruct() );
			compStack.back().data = new UIComponent();
			compStack.back().isValid = true;
			
			
			return compStack.size() - 1;
			
		}
	}
int Singleton::placeInLayer (int nodeId, int layer)
                                                {
		// int curId;
		// if (emptyLayers[layer].size() > 0) {
		// 	curId = emptyLayers[layer].back();
		// 	emptyLayers[layer].pop_back();
		// 	guiLayers[layer][curId] = nodeId;
			
		// 	return curId;
		// }
		// else {
			guiLayers[layer].push_back(nodeId);	
			return guiLayers[layer].size() - 1;
			
		//}
	}
void Singleton::initAllMatrices ()
                               {
		int i;
		int j;
		int k;
		int m;
		int curOffset;
		
		for (k = 0; k < NUM_ORIENTATIONS; k++) {
			for (j = 0; j < NUM_ORIENTATIONS; j++) {
				for (i = 0; i < NUM_ORIENTATIONS; i++) {
					curOffset = (k*NUM_ORIENTATIONS*NUM_ORIENTATIONS + j*NUM_ORIENTATIONS + i)*16;
					
					for (m = 0; m < 16; m++) {
						ALL_ROT[curOffset+m] = 0.0f;
					}
					ALL_ROT[curOffset+15] = 1.0f;
					
					for (m = 0; m < 3; m++) {
						ALL_ROT[curOffset+0+m] = DIR_VECS[i][m];
						ALL_ROT[curOffset+4+m] = DIR_VECS[j][m];
						ALL_ROT[curOffset+8+m] = DIR_VECS[k][m];
					}
					
					
					
				}
			}
		}
		
	}
void Singleton::funcNT2 ()
                       {
		threadNetRecv.setRunningLocked(true);
		gameNetwork->updateRecv();
		threadNetRecv.setRunningLocked(false);
	}
void Singleton::startNT2 ()
                        {
		if (threadNetRecv.threadRunning) {
			
		}
		else {
			threadNetRecv.threadRunning = true;
			threadNetRecv.threadMain = std::thread(&Singleton::funcNT2, this);
		}
	}
bool Singleton::stopNT2 ()
                       {
		bool didStop = false;
		if (threadNetRecv.threadRunning) {
			threadNetRecv.threadMain.join();
			threadNetRecv.threadRunning = false;
			didStop = true;
		}
		return didStop;
	}
void Singleton::funcNT ()
                      {
		threadNetSend.setRunningLocked(true);
		gameNetwork->updateSend();
		threadNetSend.setRunningLocked(false);
	}
void Singleton::startNT ()
                       {
		if (threadNetSend.threadRunning) {
			
		}
		else {
			threadNetSend.threadRunning = true;
			threadNetSend.threadMain = std::thread(&Singleton::funcNT, this);
		}
	}
bool Singleton::stopNT ()
                      {
		bool didStop = false;
		
		if (threadNetSend.threadRunning) {
			threadNetSend.threadMain.join();
			threadNetSend.threadRunning = false;
			didStop = true;
		}
		return didStop;
		
	}
void Singleton::prepSound (string soundName)
                                         {
		if (soundMap.find( soundName ) == soundMap.end()) {
			soundMap.insert( pair<string, GameSound>(soundName, GameSound()) );
			soundMap[soundName].init("..\\data\\sounds\\"+soundName+".wav");
		}
	}
void Singleton::playSoundEnt (string soundName, BaseObj * ge, float variance, float volume, bool doLoop)
          {
		
		if (ge == NULL) {
			playSoundPosAndPitch(soundName,cameraGetPosNoShake(),cameraGetPosNoShake(),variance,volume,doLoop);
		}
		else {
			if (
				(ge->entType == E_ENTTYPE_TRACE) ||
				(ge->isHidden)	
			) {
				
			}
			else {
				playSoundPosAndPitch(soundName,cameraGetPosNoShake(),BTV2FIV(ge->getCenterPoint(E_BDG_CENTER)),variance,volume,doLoop);
			}
		}
		
		
		
		
	}
void Singleton::playSoundPosAndPitch (string soundName, FIVector4 * listenerPos, FIVector4 * soundPos, float variance, float volume, bool doLoop)
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
			32.0f
		);
		
		soundMap[soundName].setLoop(doLoop);
		soundMap[soundName].play(volume*fxVolume*masterVolume);
	}
void Singleton::updateSoundPosAndPitch (string soundName, FIVector4 * listenerPos, FIVector4 * soundPos, float volume, float decay)
          {
		
		
		FIVector4 res;
		
		prepSound(soundName);
		
		res.setFXYZRef(soundPos);
		res.addXYZRef(listenerPos,-1.0f);
		
		
		soundMap[soundName].setPositionAndMinDis(
			res.getFX(),
			res.getFY(),
			res.getFZ(),
			32.0f
		);
		
		soundMap[soundName].setVolumeSmooth(volume,decay);
		
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
void Singleton::updateMatVol ()
                            {
		
		
		
		matVolLock = true;
		
		JSONValue* jv = fetchJSONData("materials.js", false);
		JSONValue* curJV = NULL;
		JSONValue* curK = NULL;
		JSONValue* curJ = NULL;
		JSONValue* curI = NULL;
		JSONValue* curCol = NULL;
		
		int i;
		int j;
		int k;
		int m;
		int n;
		int p;
		int totN;
		
		int curInd0;
		int curInd0Prev;
		int curInd0Next;
		int curInd1;
		
		int mvInd;
		
		uint rv, gv, bv;
		
		joi_type itI;
		joi_type itJ;
		joi_type itK;
		
		float totRatio;
		float curLerp;
		float curLerpNoPower;
		float curPower;
		float curLerpWithPower;
		float curRatio;
		
		int mvx = matVolDim.getIX();
		int mvy = matVolDim.getIY();
		int mvz = matVolDim.getIZ();
		
		if (jv != NULL) {
			
			
			
			
			
			curJV = jv->Child("materials");
			
			k = 0;
			for (itK = curJV->object_value.begin(); itK != curJV->object_value.end(); itK++) {
				curK = itK->second;
				
				j = 0;
				for (itJ = curK->object_value.begin(); itJ != curK->object_value.end(); itJ++) {
					curJ = itJ->second;
					
					i = 0;
					totRatio = 0.0f;
					for (itI = curJ->object_value.begin(); itI != curJ->object_value.end(); itI++) {
						curI = itI->second;
						
						curInd0 = i+j*mvx;
						curCol = curI->Child("i0_color");
						
						
						matSlice0[curInd0].h = curCol->Child(0)->number_value;
						matSlice0[curInd0].s = curCol->Child(1)->number_value;
						matSlice0[curInd0].l = curCol->Child(2)->number_value;
						
						hsv2rgb(&(matSlice0[curInd0]));
						
						matSlice0[curInd0].power = curI->Child("i1_power")->number_value;
						matSlice0[curInd0].ratio = curI->Child("i2_ratio")->number_value;
						
						if (matSlice0[curInd0].ratio <= 0.0f) {
							matSlice0[curInd0].ratio = 1.0f/(mvx-1.0f);
						}
						
						totRatio += matSlice0[curInd0].ratio;
						
						i++;
					}
					
					
					if (i == 0) {
						
					}
					else {
						for (m = 0; m < i; m++) {
							curInd0 = m+j*mvx;
							matSlice0[curInd0].ratio = matSlice0[curInd0].ratio*(matVolDim.getFX()-1.0f)/totRatio;
						}
						
						
						totN = 0;
						for (m = 0; m < i; m++) {
							curInd0 = m+j*mvx;
							
							if (i == 1) {
								curInd0Prev = curInd0;
								curInd0Next = curInd0;
							}
							else {
								curInd0Prev = (m-1)+j*mvx;
								curInd0Next = (m+1)+j*mvx;
							}
							
							
							
							curRatio = matSlice0[curInd0].ratio;
							
							for (n = 0; (n < (curRatio+0.1f) )&&(totN < mvx); n++) {
								curLerp = ((float)n)/curRatio;
								curInd1 = totN + j*mvx;
								
								if (m == 0) {
									
									curLerpNoPower = curLerp*0.5f;
									curPower = mixf(matSlice0[curInd0].power,matSlice0[curInd0Next].power,curLerpNoPower);
									curLerpWithPower = pow(curLerpNoPower,curPower*8.0f);
									
									matSlice1[curInd1].r = mixf(matSlice0[curInd0].r,matSlice0[curInd0Next].r,curLerpWithPower);
									matSlice1[curInd1].g = mixf(matSlice0[curInd0].g,matSlice0[curInd0Next].g,curLerpWithPower);
									matSlice1[curInd1].b = mixf(matSlice0[curInd0].b,matSlice0[curInd0Next].b,curLerpWithPower);
								}
								else if (m == (i-1)) {
									curLerpNoPower = curLerp*0.5f+0.5f;
									curPower = mixf(matSlice0[curInd0Prev].power,matSlice0[curInd0].power,curLerpNoPower);
									curLerpWithPower = pow(curLerpNoPower,curPower*8.0f);
									
									matSlice1[curInd1].r = mixf(matSlice0[curInd0Prev].r,matSlice0[curInd0].r,curLerpWithPower);
									matSlice1[curInd1].g = mixf(matSlice0[curInd0Prev].g,matSlice0[curInd0].g,curLerpWithPower);
									matSlice1[curInd1].b = mixf(matSlice0[curInd0Prev].b,matSlice0[curInd0].b,curLerpWithPower);
								}
								else {
									
									
									if (curLerp < 0.5f) {
										curLerpNoPower = curLerp + 0.5f;
										curPower = mixf(matSlice0[curInd0Prev].power,matSlice0[curInd0].power,curLerpNoPower);
										curLerpWithPower = pow(curLerpNoPower,curPower*8.0f);
										
										matSlice1[curInd1].r = mixf(matSlice0[curInd0Prev].r,matSlice0[curInd0].r,curLerpWithPower);
										matSlice1[curInd1].g = mixf(matSlice0[curInd0Prev].g,matSlice0[curInd0].g,curLerpWithPower);
										matSlice1[curInd1].b = mixf(matSlice0[curInd0Prev].b,matSlice0[curInd0].b,curLerpWithPower);
									}
									else {
										curLerpNoPower = curLerp - 0.5f;
										curPower = mixf(matSlice0[curInd0].power,matSlice0[curInd0Next].power,curLerpNoPower);
										curLerpWithPower = pow(curLerpNoPower,curPower*8.0f);
										
										matSlice1[curInd1].r = mixf(matSlice0[curInd0].r,matSlice0[curInd0Next].r,curLerpWithPower);
										matSlice1[curInd1].g = mixf(matSlice0[curInd0].g,matSlice0[curInd0Next].g,curLerpWithPower);
										matSlice1[curInd1].b = mixf(matSlice0[curInd0].b,matSlice0[curInd0Next].b,curLerpWithPower);											
										
									}
									
									
									
								}
								
								
								
								totN++;
							}
							
							
						}
					}
						
					
					
					
					
					j++;
					
				}
				
				
				
				
				
				////////////////////
				
				
				
				
				
				for (p = 0; p < mvx; p++) {
					
					totN = 0;
					for (m = 0; m < j; m++) {
						curInd0 = p + m*mvx;
						
						if (j == 1) {
							curInd0Prev = curInd0;
							curInd0Next = curInd0;
						}
						else {
							curInd0Prev = p + (m-1)*mvx;
							curInd0Next = p + (m+1)*mvx;
						}
						
						
						
						if (j == 0) {
							curRatio = (float(mvy));
						}
						else {
							curRatio = (float(mvy))/(float(j));
						}
						
						
						for (n = 0; (n < (curRatio+0.1f))&&(totN < mvy); n++) {
							curLerp = ((float)n)/curRatio;
							curInd1 = p + totN*mvx;
							
							if (m == 0) {
								
								curLerpWithPower = curLerp*0.5f;
								
								matSlice2[curInd1].r = mixf(matSlice1[curInd0].r,matSlice1[curInd0Next].r,curLerpWithPower);
								matSlice2[curInd1].g = mixf(matSlice1[curInd0].g,matSlice1[curInd0Next].g,curLerpWithPower);
								matSlice2[curInd1].b = mixf(matSlice1[curInd0].b,matSlice1[curInd0Next].b,curLerpWithPower);
							}
							else if (m == (j-1)) {
								curLerpWithPower = curLerp*0.5f+0.5f;
								
								matSlice2[curInd1].r = mixf(matSlice1[curInd0Prev].r,matSlice1[curInd0].r,curLerpWithPower);
								matSlice2[curInd1].g = mixf(matSlice1[curInd0Prev].g,matSlice1[curInd0].g,curLerpWithPower);
								matSlice2[curInd1].b = mixf(matSlice1[curInd0Prev].b,matSlice1[curInd0].b,curLerpWithPower);
							}
							else {
								
								
								if (curLerp < 0.5f) {
									curLerpWithPower = curLerp + 0.5f;
									
									matSlice2[curInd1].r = mixf(matSlice1[curInd0Prev].r,matSlice1[curInd0].r,curLerpWithPower);
									matSlice2[curInd1].g = mixf(matSlice1[curInd0Prev].g,matSlice1[curInd0].g,curLerpWithPower);
									matSlice2[curInd1].b = mixf(matSlice1[curInd0Prev].b,matSlice1[curInd0].b,curLerpWithPower);
								}
								else {
									curLerpWithPower = curLerp - 0.5f;
									
									matSlice2[curInd1].r = mixf(matSlice1[curInd0].r,matSlice1[curInd0Next].r,curLerpWithPower);
									matSlice2[curInd1].g = mixf(matSlice1[curInd0].g,matSlice1[curInd0Next].g,curLerpWithPower);
									matSlice2[curInd1].b = mixf(matSlice1[curInd0].b,matSlice1[curInd0Next].b,curLerpWithPower);											
									
								}
								
								
								
							}
							
							
							
							mvInd = curInd1 + k*mvx*mvy;
							rv = matSlice2[curInd1].r*255.0f;
							gv = matSlice2[curInd1].g*255.0f;
							bv = matSlice2[curInd1].b*255.0f;
							matVol[mvInd] = (0 << 24) | (bv << 16) | (gv << 8) | (rv);
							
							
							
							
							totN++;
						}
						
						
					}
				}
				
				
				
				
				////////////////////
				
				
				
				
				
				
				
				
				
				
				k++;
			}
			
			
			
			glBindTexture(GL_TEXTURE_3D, 0);
			glBindTexture(GL_TEXTURE_3D, volIdMat);
			glTexSubImage3D(
				GL_TEXTURE_3D,
				0,

				0,
				0,
				0,

				mvx,
				mvy,
				mvz,

				GL_RGBA,
				GL_UNSIGNED_BYTE,

				matVol
			);
			glBindTexture(GL_TEXTURE_3D, 0);
			
			
			
		}
		
		
		
		matVolLock = false;	
		
	}
void Singleton::toggleDDMenu (int x, int y, bool toggled)
                                                      {
		
		if (placingGeom||gem->combatMode()) {
			return;
		}
		
		int ind = 0;
		bool objTargeted = false;
		bool actOnSel = false;
		UIComponent* tempComp;
		
		if (toggled) {
			
			ind = mouseDownOPD.getFW();
			
			objTargeted = ind >= E_OBJ_LENGTH;	
			
			
			
			if (objTargeted) {
				gem->setSelInd(ind);				
			}
			else {
				getMarkerPos(x, y);
				markerFound = true;
				gem->setSelInd(0);
			}
			
			
			actOnSel = 
				objTargeted &&
				(gem->selObjInd != gem->actObjInd) &&
				(gem->actObjInd >= E_OBJ_LENGTH);
			
			tempComp = getGUIComp("ddMenu.placeEntity");
			if (tempComp != NULL) {tempComp->enabled = !objTargeted;}
			tempComp = getGUIComp("ddMenu.removeEntity");
			if (tempComp != NULL) {tempComp->enabled = objTargeted;}
			tempComp = getGUIComp("ddMenu.editEntity");
			if (tempComp != NULL) {tempComp->enabled = objTargeted;}
			tempComp = getGUIComp("ddMenu.changeMaterial");
			if (tempComp != NULL) {tempComp->enabled = objTargeted;}
			
			tempComp = getGUIComp("ddMenu.attack");
			if (tempComp != NULL) {tempComp->enabled = actOnSel;}
			tempComp = getGUIComp("ddMenu.talkTo");
			if (tempComp != NULL) {tempComp->enabled = actOnSel;}
			tempComp = getGUIComp("ddMenu.engageWith");
			if (tempComp != NULL) {tempComp->enabled = actOnSel;}
			tempComp = getGUIComp("ddMenu.pickPocket");
			if (tempComp != NULL) {tempComp->enabled = actOnSel;}
			tempComp = getGUIComp("ddMenu.tradeWith");
			if (tempComp != NULL) {tempComp->enabled = actOnSel;}
			
			
			
			tempComp = getGUIComp("ddMenu.menuBar");
			if (tempComp != NULL) {
				if (objTargeted) {
					tempComp->setText(gem->objStrings[gem->gameObjects[gem->selObjInd].objectType ]);
				}
				else {
					tempComp->setText("Context Menu");
				}
				
			}
			
			menuList[E_FM_DDMENU]->isDirty = true;
			menuList[E_FM_DDMENU]->visible = true;
			ddMenuBar = menuList[E_FM_DDMENU]->getChild(0)->getChild(0);
			ddMenuBar->lastDrag.x = (guiX);
			ddMenuBar->lastDrag.y = min((float)(guiY), (float)( (guiWinH - menuList[E_FM_DDMENU]->getChild(0)->resultDimInPixels.y) ));
			ddMenuBar->forceDragUpdate = true;
		}
		else {
			menuList[E_FM_DDMENU]->visible = false;
			markerFound = false;
		}
	}
void Singleton::dispatchEvent (int button, int state, float x, float y, UIComponent * comp, bool automated, bool preventRefresh)
          {
		
		UIComponent* tempComp;
		
		
		BaseObj* tmpObj = NULL;
		
		StatSheet* curStatSheet;
		
		bool hitPicker = false;
		bool wasDoubleClick = false;
		
		float wheelDelta = 0.0f;
		
		double tempTime = myTimer.getElapsedTimeInMilliSec();
		
		int i;
		int j;
		
		int cbDataCount;
		JSONValue* cbData = NULL;
		
		bool doSPUpdate = false;
		bool mouseUpEvent = false;
		bool mouseDownEvent = false;
		
		float curValue = comp->getValue();
		float curValueY = comp->getValueY();
		
		draggingMap = false;
		
		bool noTravel = false;
		bool isInteractiveComp = false;
		bool suppressSound = false;
		
		float newVal;
		float oldVal;
		float difVal;
		
		
		switch (comp->guiClass) {
			case E_GT_SLIDER:
			case E_GT_BUTTON:
			case E_GT_RADIO:
			case E_GT_CHECK:
			case E_GT_DRAGPAD:
					isInteractiveComp = true;
			break;
			default:
				if (comp->jvNodeNoTemplate->HasChild("interactive")) {
					isInteractiveComp = true;
				}
			break;
		}
		
		suppressSound = automated||(!isInteractiveComp);
		
		
		switch(button) {
			case GLUT_NO_BUTTON:
			
				
				
				switch(state) {
					case GLUT_OVER:
						playSoundEvent("mouseOver",suppressSound);
					break;
					case GLUT_OUT:
						playSoundEvent("mouseOut",suppressSound);
					break;
					case GLUT_CHANGING:
						if (comp->guiClass == E_GT_DRAGPAD) {
							
							if (comp->uid.compare("map.mapHolder") == 0) {
								cameraPos->setFXYZRef(&baseCameraPos);
								
								cameraPos->addXYZ(
									-(x - comp->dragStart.x)*((float)cellsPerWorld)/(cameraZoom*comp->resultDimInPixels.x),
									-(y - comp->dragStart.y)*((float)cellsPerWorld)/(cameraZoom*comp->resultDimInPixels.y),
									0.0f
								);
								
								camLerpPos.copyFrom(cameraPos);
								
								
								
								
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
					playSoundEvent("mouseDown",suppressSound);
					
					if (comp->uid.compare("map.mapHolder") == 0) {
						baseCameraPos.setFXYZRef(cameraPos);
					}
					
					mouseDownEvent = true;
					mdTime = myTimer.getElapsedTimeInMilliSec();
				}
				else {
					playSoundEvent("mouseUp",suppressSound);
					
					if (comp->guiClass == E_GT_DRAGPAD) {
						
					}
					else {
						doSPUpdate = true;
						mouseUpEvent = true;
					}
					
					
				}
			break;
			
			case 3: // wheel up
				frameMouseMove = true;
				wheelDelta = 1.0f / 20.0f;
				break;

			case 4: // wheel down
				frameMouseMove = true;
				wheelDelta = -1.0f / 20.0f;
				break;
			
			
		}
		
		if (mouseUpEvent) {
			if (!automated) {
				noTravel = mouseEnd.distance(&mouseStart) < MAX_TRAVEL_DIS;
				
				if (lastNodeId == comp->nodeId) {
					
					if ( (tempTime-clickTime) < 500 ) {
						wasDoubleClick = true;
					}
					
				}
				
				clickTime = tempTime;
				lastNodeId = comp->nodeId;
			}
		}
		
		
		
		if (comp->uid.compare("map.mapHolder") == 0) {
			
			
			zoomDelta += wheelDelta;
			
			if (zoomDelta < 0.0) {
				zoomDelta = 0.0;
			}
			
			
			targetZoom = pow(2.0, zoomDelta);
			if (!ignoreFrameLimit) {
				cameraZoom = targetZoom;
			}
			
		}
		
		if (gem->handleGUI(
			comp, mouseUpEvent, mouseDownEvent, noTravel, wasDoubleClick	
		)) {
			goto DISPATCH_EVENT_END;
		}
		
		
		
		
		
		
		if (mouseUpEvent) {
			
			
			if (comp->uid.compare("charEdit.savePose") == 0) {
				saveOrg();
			}
			else if (comp->uid.compare("charEdit.loadPose") == 0) {
				loadOrg();
			}
			else if (comp->uid.compare("$charEdit.orgOn") == 0) {
				gem->orgOn = curValue != 0.0f;
			}
			else if (comp->uid.compare("$charEdit.pathfindingOn") == 0) {
				//pathfindingOn = curValue != 0.0f;
			}
			else if (comp->uid.compare("$charEdit.editPose") == 0) {
				gem->editPose = curValue != 0.0f;
				EDIT_POSE = gem->editPose;
			}
			else if (comp->uid.compare("$charEdit.mirrorOn") == 0) {
				gem->mirrorOn = curValue != 0.0f;
			}
			else if (comp->uid.compare("$charEdit.applyToChildren") == 0) {
				applyToChildren = curValue != 0.0f;
			}
			else if (comp->uid.compare("fieldMenu.ok") == 0) {
				endFieldInput(true);
			}
			else if (comp->uid.compare("fieldMenu.cancel") == 0) {
				endFieldInput(false);
			}
			else if (comp->uid.compare("#materialPicker") == 0) {
				gem->updateOrgMat(comp);
			}
			else if (comp->uid.compare("statMenu.apply") == 0) {
				if (gem->getCurActor() != NULL) {	
					curStatSheet = &(gem->getCurActor()->statSheet);
					
					for (i = 0; i < E_CS_LENGTH; i++) {
						curStatSheet->baseStats[i] = curStatSheet->unapplyedStats[i];
					}
					
				}
			}
			else if (comp->uid.compare("statMenu.reset") == 0) {
				if (gem->getCurActor() != NULL) {
					curStatSheet = &(gem->getCurActor()->statSheet);
					
					
					
					for (i = 0; i < E_CS_LENGTH; i++) {
						
						while (curStatSheet->unapplyedStats[i] > curStatSheet->baseStats[i]) {
							curStatSheet->unapplyedStats[i] -= 1;
							curStatSheet->availPoints += 1;
						}
						
						tempComp = getGUIComp("statMenu.statParent")->getChild(i);
						tempComp->setValue(
							curStatSheet->unapplyedStats[i]/tempComp->divisions
						);
						
					}
					
					tempComp = getGUIComp("statMenu.availPoints");
					tempComp->setValue(
						curStatSheet->availPoints/tempComp->divisions
					);
					
					
				}
			}
			
			if (comp->guiClass == E_GT_BUTTON) {
				toggleDDMenu(x,y,false);
			}
			
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
		else if (comp->uid.compare("$options.graphics.clipDist") == 0) {
			
			clipDist[1] = curValue*4096.0f*4.0;
			
		}
		else if (comp->uid.compare("$options.graphics.maxHeight") == 0) {
			
			//heightMapMaxInCells = curValue*8192.0f;
			
		}
		else if (comp->uid.compare("$options.graphics.fov") == 0) {
			FOV = mixf(5.0f,120.0f,curValue);
			focalLength = 1.0f / tan(FOV / 2.0f);
			
		}
		else if (comp->uid.compare("$options.graphics.sphereMapPrec") == 0) {
			sphereMapPrec = mixf(0.0f,200.0f,curValue);
		}
		else if (comp->uid.compare("#statMenu.stat") == 0) {
			if (gem->getCurActor() != NULL) {
				
				curStatSheet = &(gem->getCurActor()->statSheet);
				
				oldVal = curStatSheet->unapplyedStats[comp->index];
				newVal = roundVal(curValue*comp->divisions);
				difVal = newVal-oldVal;
				
				tempComp = getGUIComp("statMenu.availPoints");
				
				curStatSheet->availPoints -= difVal;
				curStatSheet->unapplyedStats[comp->index] = newVal;
				
				while (curStatSheet->availPoints < 0) {
					curStatSheet->availPoints += 1;
					curStatSheet->unapplyedStats[comp->index] -= 1;
				}
				
				while (curStatSheet->unapplyedStats[comp->index] < curStatSheet->baseStats[comp->index]) {
					curStatSheet->availPoints -= 1;
					curStatSheet->unapplyedStats[comp->index] += 1;
				}
				
				
				comp->setValue(
					curStatSheet->unapplyedStats[comp->index]/comp->divisions
				);
				tempComp->setValue(
					curStatSheet->availPoints/tempComp->divisions
				);
				
				
				
				
				
			}
			
		}
		
		
		
		if (
			(button == GLUT_LEFT_BUTTON) ||
			(state == GLUT_CHANGING)	
		) {
			if (comp->uid.compare("#materialVal") == 0) {
				updateMatFlag = true;
			}
			
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
							
							
							cbDataStrings[0] = cbData->Child("shaderName")->string_value;
							cbDataStrings[1] = cbData->Child("paramName")->string_value;
							
							
							
							shaderMap[cbDataStrings[0]]->paramMap[cbDataStrings[1]] = curValue;
							
							
							
						}
					}
				}
				
				
			}
		}
		
DISPATCH_EVENT_END:
		
		if (mouseMoved) {
			mouseMoved = false;
		}
		if (mouseUpEvent) {
			gem->isDraggingObject = false;
		}
	}
StyleSheet * Singleton::getNewStyleSheet (string ssName)
                                                    {
		styleSheetMap[ssName].init();
		return &(styleSheetMap[ssName]);
	}
void Singleton::initStyleSheet ()
                              {
		
		int i;
		int j;
		
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

		curState->props[E_SS_PADDING] = 2.0f;
		curState->props[E_SS_BORDER] = 1.0f;
		curState->props[E_SS_MARGIN] = 2.0f;
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
		curState->setVal(E_SS_BGCOL0_R, 0.2f, 0.1f, 0.0f, 1.0f);
		curState->setVal(E_SS_BGCOL1_R, 0.1f, 0.05f, 0.0f, 1.0f);
		curState->setVal(E_SS_FGCOLTEXT0_R, 1.0f, 0.75f, 0.0f, 1.0f);
		curState->setVal(E_SS_FGCOLTEXT1_R, 1.0f, 0.75f, 0.0f, 1.0f);
		curState->setVal(E_SS_BDCOL_R, 1.0f, 0.75f, 0.0f, 1.0f);
		headerSS->compStates[E_COMP_OVER].copyFrom(& (headerSS->compStates[E_COMP_UP]) );
		headerSS->compStates[E_COMP_DOWN].copyFrom(& (headerSS->compStates[E_COMP_UP]) );

		curState = &(headerSS->compStates[E_COMP_OVER]);
		curState = &(headerSS->compStates[E_COMP_DOWN]);
		
		
		
		StyleSheet* sectionSS = getNewStyleSheet("sectionSS");
		sectionSS->copyFrom(mainSS);
		
		for (i = 0; i < E_COMP_TOTAL; i++) {
			curState = &(sectionSS->compStates[i]);
			curState->setVal(E_SS_FGCOLTEXT0_R, 0.5f, 0.5f, 0.5f, 1.0f);
			curState->setVal(E_SS_FGCOLTEXT1_R, 0.4f, 0.4f, 0.4f, 1.0f);
			curState->setVal(E_SS_BGCOL0_R, 0.2f, 0.2f, 0.2f, 1.0f);
			curState->setVal(E_SS_BGCOL1_R, 0.1f, 0.1f, 0.1f, 0.5f);
		}
		
		StyleSheet* redSS = getNewStyleSheet("redSS");
		redSS->copyFrom(mainSS);
		
		for (i = 0; i < E_COMP_TOTAL; i++) {
			curState = &(redSS->compStates[i]);
			curState->setVal(E_SS_BGCOL0_R, 1.0f, 0.1f, 0.2f, 1.0f);
			curState->setVal(E_SS_BGCOL1_R, 0.8f, 0.05f, 0.1f, 0.75f);
		}
		
		
		
		StyleSheet* greenSS = getNewStyleSheet("greenSS");
		greenSS->copyFrom(mainSS);
		
		for (i = 0; i < E_COMP_TOTAL; i++) {
			curState = &(greenSS->compStates[i]);
			curState->setVal(E_SS_BGCOL0_R, 0.0f, 1.0f, 0.5f, 1.0f);
			curState->setVal(E_SS_BGCOL1_R, 0.0f, 0.8f, 0.8f, 0.5f);
		}
		
		
		StyleSheet* spacerSS = getNewStyleSheet("spacerSS");
		spacerSS->copyFrom(mainSS);
		
		for (i = 0; i < E_COMP_TOTAL; i++) {
			curState = &(spacerSS->compStates[i]);
			
			for (j = 0; j <= E_SS_BDCOL_R; j += 4) {
				curState->setVal(j, 0.0f, 0.0f, 0.0f, 0.0f);
			}
			
		}
		
		
		
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
void Singleton::drawBoxMinMax (btVector3 v0, btVector3 v1)
          {
		tempVec1.setBTV(v0);
		tempVec2.setBTV(v1);
		drawBox(&tempVec1,&tempVec2);
	}
void Singleton::drawBoxRad (btVector3 v0, btVector3 v1)
          {
		tempVec1.setBTV(v0-v1);
		tempVec2.setBTV(v0+v1);
		drawBox(&tempVec1,&tempVec2);
	}
void Singleton::drawBox (FIVector4 * v0, FIVector4 * v1, int faceFlag)
          {


		float minX = min(v0->getFX(),v1->getFX());
		float minY = min(v0->getFY(),v1->getFY());
		float minZ = min(v0->getFZ(),v1->getFZ());

		float maxX = max(v0->getFX(),v1->getFX());
		float maxY = max(v0->getFY(),v1->getFY());
		float maxZ = max(v0->getFZ(),v1->getFZ());


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
void Singleton::getMaterialString ()
                                 {
		string resString = "\n";
		
		JSONValue* jv = fetchJSONData("materials.js", false);
		
		if (jv != NULL) {
			JSONValue* jv2 = jv->Child("materials");
			JSONValue* curChild;
			
			int numChildren = jv2->CountChildren();
			int i;
			
			string lastKey;
			
			for (i = 0; i < numChildren; i++) {
				curChild = jv2->Child(i);
				lastKey = jv2->lastKey;
				
				splitStrings.clear();
				splitStrings = split(lastKey, '_');
				
				resString.append("const float TEX_"+splitStrings[1]+"="+i__s(i)+".0/255.0;\n");
				
			}
			
			resString.append("\n");
			
			includeMap["materials"] = resString;
			
		}
		
	}
bool Singleton::getPrimTemplateString ()
                                     {
		
		primTemplateStack.clear();
		
		JSONValue* jv = fetchJSONData("primTemplates.js",true);		
		JSONValue* jv2 = jv->Child("primTemplates");
		JSONValue* jv3 = NULL;
		JSONValue* jv4 = NULL;
		
		int i;
		int j;
		int k;
		
		int tempInd;
		
		int numTemplates = jv2->CountChildren();
		int numProps = 0;
		int numFields = 0;
		
		int propCount = 0;
		int maxProps = numTemplates*E_PRIMTEMP_LENGTH;
		
		float curNumber;
		
		string resString = "";
		resString.append("const int PRIMS_PER_MACRO = " + i__s(gameFluid[E_FID_BIG]->primsPerMacro) + ";\n");
		resString.append("const int VECS_PER_PRIM = " + i__s(gameFluid[E_FID_BIG]->floatsPerPrimEntry/4) + ";\n");
		resString.append("const float PRIM_DIV = " + i__s(gameFluid[E_FID_BIG]->primDiv) + ".0;\n");
		
		
		for (i = 0; i < numTemplates; i++) {
			jv3 = jv2->Child(i);
			numProps = jv3->CountChildren() - 1; //minus one for comment
			if (numProps != E_PRIMTEMP_LENGTH) {
				cout << "ERROR: invalid number of properties\n";
				return false;
			}
			
			tempInd = propCount;
			
			for (j = 0; j < numProps; j++) {
				jv4 = jv3->Child(j);
				numFields = jv4->CountChildren();
				if (numFields != 4) {
					cout << "ERROR: invalid number of fields\n";
					return false;
				}
				
				primTemplateStack.push_back(FIVector4());
								
				for (k = 0; k < numFields; k++) {
					curNumber = jv4->Child(k)->number_value;
					primTemplateStack.back().setIndex(k, curNumber);
				}
				
				propCount++;
				
			}
			
			primTemplateStack[tempInd + E_PRIMTEMP_VISMIN].multXYZRef(
					&(primTemplateStack[tempInd + E_PRIMTEMP_BOUNDSMIN])
			);
			primTemplateStack[tempInd + E_PRIMTEMP_VISMAX].multXYZRef(
					&(primTemplateStack[tempInd + E_PRIMTEMP_BOUNDSMAX])
			);
			
			
		}
		
		resString.append("const vec4 primTemp[" + i__s(numTemplates*E_PRIMTEMP_LENGTH) + "] = vec4[](\n");
		
		numFields = 4;
		
		for (i = 0; i < primTemplateStack.size(); i++) {
			resString.append("vec4(");
			for (k = 0; k < numFields; k++) {
				curNumber = primTemplateStack[i][k];
				resString.append( f__s(curNumber) );
				if (k < numFields-1) {
					resString.append(",");
				}
			}
			resString.append(")");
			
			if (i < (primTemplateStack.size()-1) ) {
				resString.append(",");
			}
			
			resString.append("\n");
		}
		
		resString.append(");\n");
		
		//cout << resString << "\n";
		
		includeMap["primTemplates"] = resString;
		
		return true;
	}
void Singleton::refreshIncludeMap ()
                                 {
		getMaterialString();
		getPrimTemplateString();
	}
void Singleton::doShaderRefresh (bool doBake)
        {

		loadConstants();

		LAST_COMPILE_ERROR = false;

		readyToRecompile = 0;

		int i;
		int j;
		
		Shader* curShader;
		
		refreshIncludeMap();


		// this is expensive
		for (i = 0; i < shaderStrings.size(); i++) {
			shaderMap[ shaderStrings[i] ]->init(shaderStrings[i] , doBake, &includeMap);
		}
		
		//"../src/glsl/" + shaderStrings[i] + ".c"
		
		if (DO_SHADER_DUMP) {
			cout << "SHADER_DUMP\n";
			saveFileString("..\\data\\temp.txt", &globString);
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
			
			processJSONFromString(
				&stringBuf,
				&(externalJSON["E_SDT_SHADERPARAMS"].jv)
			);
			
			
			
		}
		

	}
void Singleton::setWH (int w, int h)
        {

		baseW = w;
		baseH = h;
	}
void Singleton::sampleFBODirect (FBOSet * fbos, int offset, int _minOff, int _maxOff)
          {
		int i;
		
		int minOff = _minOff;
		int maxOff = _maxOff;
		if (maxOff == -1) {
			maxOff = fbos->numBufs;
		}
		
		if (shadersAreLoaded) {
			for (i = minOff; i < maxOff; i++) {
				setShaderTexture(i - minOff + offset, fbos->fbos[i].color_tex);
			}
		}
	}
void Singleton::unsampleFBODirect (FBOSet * fbos, int offset, int _minOff, int _maxOff)
          {
		int i;
		
		int minOff = _minOff;
		int maxOff = _maxOff;
		if (maxOff == -1) {
			maxOff = fbos->numBufs;
		}
		
		if (shadersAreLoaded) {
			for (i = maxOff - 1; i >= minOff; i--) {
				setShaderTexture(i - minOff + offset, 0);
			}
		}
	}
void Singleton::getMatrixFromFBO (string fboName)
                                              {
		FBOSet *fbos = getFBOByName(fboName);
		setMatrices(fbos->width, fbos->height);
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
			cout << "invalid key " << fboName << "\n";
			exit(0);
		}
		
		return &(fboMap[fboName]);
	}
void Singleton::sampleFBO (string fboName, int offset, int swapFlag, int minOff, int maxOff)
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
			sampleFBODirect(fbos, offset, minOff, maxOff);
		}
		else
		{
			doTrace("sampleFBO: Invalid FBO Name");
		}


	}
void Singleton::unsampleFBO (string fboName, int offset, int swapFlag, int minOff, int maxOff)
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
			unsampleFBODirect(fbos, offset, minOff, maxOff);
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
		setShaderTexture(0, 0);
		unbindFBO();
		unbindShader();
	}
void Singleton::copyFBO2 (string src, string dest, int num1, int num2)
        {
		bindShader("CopyShader2");
		bindFBO(dest);
		setShaderTexture(0, getFBOWrapper(src,num1)->color_tex);
		setShaderTexture(1, getFBOWrapper(src,num2)->color_tex);
		drawFSQuad();
		setShaderTexture(1, 0);
		setShaderTexture(0, 0);
		unbindFBO();
		unbindShader();
	}
void Singleton::bindFBO (string fboName, int swapFlag, int doClear)
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
			bindFBODirect(fbos, doClear);
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


		if (shaderMap.find( shaderName ) == shaderMap.end()) {
			cout << "invalid shader name " << shaderName << "\n";
			exit(0);
		}

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
void Singleton::setShaderMatrix3x3 (string paramName, float * x, int count)
        {
		curShaderPtr->setShaderMatrix3x3(paramName, x, count);
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
void Singleton::setShaderbtVec3 (string paramName, btVector3 v)
        {
		curShaderPtr->setShaderbtVec3(paramName, v);
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
void Singleton::setShaderTBO (int multitexNumber, GLuint tbo_tex, GLuint tbo_buf, bool isFloat)
        {
		if (shadersAreLoaded)
		{
			glActiveTexture(GL_TEXTURE0 + multitexNumber);
			glBindTexture(GL_TEXTURE_2D, tbo_tex);
			if (tbo_tex != 0) {
				if (isFloat) {
					glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, tbo_buf);
				}
				else {
					glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, tbo_buf);
				}
				
			}
			curShaderPtr->setShaderInt(shaderTextureIds[multitexNumber] , multitexNumber);
		}
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
void Singleton::drawQuadWithCoords (FIVector4 * p0, FIVector4 * p1, FIVector4 * p2, FIVector4 * p3, float tx1, float ty1, float tx2, float ty2)
          {
		//glColor4f(1, 1, 1, 1);
		//glNormal3f(0, 0, 1);
		
		glBegin(GL_QUADS);

		glTexCoord2f(tx1, ty1);
		glVertex3f(p0->getFX(), p0->getFY(), p0->getFZ());

		glTexCoord2f(tx2, ty1);
		glVertex3f(p1->getFX(), p1->getFY(), p1->getFZ());

		glTexCoord2f(tx2, ty2);
		glVertex3f(p2->getFX(), p2->getFY(), p2->getFZ());

		glTexCoord2f(tx1, ty2);
		glVertex3f(p3->getFX(), p3->getFY(), p3->getFZ());

		glEnd();
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
		float fx0 = (xOff - 1.0f) * zm;
		float fy0 = (yOff - 1.0f) * zm;
		float fx1 = (xOff + 1.0f) * zm;
		float fy1 = (yOff + 1.0f) * zm;

		glBegin(GL_QUADS);
		//glColor4f(1, 1, 1, 1);
		//glNormal3f(0, 0, 1);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(fx0, fy0, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(fx1, fy0, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(fx1, fy1, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(fx0, fy1, 0.0f);

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
		return val*heightMapMaxInCells;
	}
float Singleton::getSLNormalized ()
        {
		return (((float)gw->seaLevel) / 255.0f);
	}
float Singleton::getSeaHeightScaled ()
        {
		return getSLNormalized()*heightMapMaxInCells;
	}
float Singleton::getHeightAtPixelPos (float x, float y, bool dd)
        {
		FBOWrapper *fbow;
		float xc;
		float yc;

		int channel = 0;

		float testHeight;
		
		float v0 = 0.0f;
		float v1 = 0.0f;
		float v2 = 0.0f;
		float v3 = 0.0f;
		
		
		if (mapInvalid)
		{

			cout << "mapInvalid\n";
			return 0.0f;

		}
		else
		{
			FBOWrapper *fbow = getFBOWrapper("hmFBO", 0);

			xc = (x / ((float)cellsPerWorld)) * ((float)fbow->width);
			yc = (y / ((float)cellsPerWorld)) * ((float)fbow->height);

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
				
				// - v1 * mapAmps.getFY()*0.5f
				// - v2 * mapAmps.getFZ()*0.5f
				// - v3 * mapAmps.getFW()*0.5f
				
			;
			
			if (dd) {
				//cout << "testHeight " << testHeight << "\n";
			}

			testHeight = clampf(testHeight,0.0f,1.0f);

			return getTerHeightScaled(testHeight);



		}



	}
void Singleton::angleToVec (FIVector4 * fv, float xr, float yr)
                                                           {
		fv->setFXYZ(
			cos(xr)*sin(yr),
			sin(xr)*sin(yr),
			cos(yr)
		);
		fv->normalize();
	}
void Singleton::vecToAngle (FIVector4 * fv, FIVector4 * ta)
                                                       {
		
		ta->setFXYZ(
			atan2(fv->getFY(),fv->getFX()),
			acos(fv->getFZ()),
			0.0f	
		);
	}
void Singleton::syncObjects ()
                           {
		int i;
		float xrp;
		float yrp;
		
		float xrotrad = getCamRot(0);
		float yrotrad = getCamRot(1);
		
		
		
		for (i = 1; i < E_OBJ_LENGTH; i++)
		{

			if (dynObjects[i]->moveType == E_MT_TRACKBALL) {
				
				xrp = xrotrad +	dynObjects[i]->posTrackball.getFX()/200.0f;
				yrp = yrotrad + dynObjects[i]->posTrackball.getFY()/200.0f;
				
				angleToVec(&tempVec1,xrp,yrp);
				tempVec1.multXYZ(dynObjects[i]->posTrackball.getFZ()*0.5f + 2.0f);
				
				
				dynObjects[i]->pos.copyFrom(cameraGetPosNoShake());
				dynObjects[i]->pos.addXYZRef( &(tempVec1) );
				
			}
			else {
				if (dynObjects[i]->moveType == E_MT_RELATIVE) {
					dynObjects[i]->pos.copyFrom(cameraGetPosNoShake());
					dynObjects[i]->pos.addXYZRef( &(dynObjects[i]->posRel) );
				}
			}
		}
		
		
		
	}
void Singleton::updateCamVals ()
                             {
		
		
		if (camLerpPos.getFX() > ((float)cellsPerWorld) / 2.0)
		{
			camLerpPos.setFX( camLerpPos.getFX() - ((float)cellsPerWorld) );
		}
		if (camLerpPos.getFX() < -((float)cellsPerWorld) / 2.0)
		{
			camLerpPos.setFX( camLerpPos.getFX() + ((float)cellsPerWorld) );
		}
		if (camLerpPos.getFY() > ((float)cellsPerWorld) / 2.0)
		{
			camLerpPos.setFY( camLerpPos.getFY() - ((float)cellsPerWorld) );
		}
		if (camLerpPos.getFY() < -((float)cellsPerWorld) / 2.0)
		{
			camLerpPos.setFY( camLerpPos.getFY() + ((float)cellsPerWorld) );
		}
		
		
		if (cameraPos->getFX() > ((float)cellsPerWorld) / 2.0)
		{
			cameraPos->setFX( cameraPos->getFX() - ((float)cellsPerWorld) );
		}
		if (cameraPos->getFX() < -((float)cellsPerWorld) / 2.0)
		{
			cameraPos->setFX( cameraPos->getFX() + ((float)cellsPerWorld) );
		}
		if (cameraPos->getFY() > ((float)cellsPerWorld) / 2.0)
		{
			cameraPos->setFY( cameraPos->getFY() - ((float)cellsPerWorld) );
		}
		if (cameraPos->getFY() < -((float)cellsPerWorld) / 2.0)
		{
			cameraPos->setFY( cameraPos->getFY() + ((float)cellsPerWorld) );
		}
		
		if (smoothMove) {
			
			
			// tempLerpPos.copyFrom(cameraPos);
			// tempLerpPos.lerpXYZ(&camLerpPos,0.1f);
			
			//&camLerpPos);
			// tempLerpPos.addXYZRef(cameraPos,-1.0f);
			// tempLerpPos.multXYZ(timeDelta*8.0f);
			
			cameraPos->lerpXYZ(&camLerpPos,conVals[E_CONST_CAM_LERP_AMOUNT]);
			
			amountInvalidMove = camLerpPos.length();
			depthInvalidMove = amountInvalidMove > 0.01f;
			
			//cameraPos->addXYZRef(&tempLerpPos);
			
		}
		else {
			cameraPos->copyFrom(&camLerpPos);
		}
		
		
		lastHolderPos.copyIntDiv(cameraPos,cellsPerHolder);
		
		
		resultShake = -cameraShake*sin(shakeTimer.getElapsedTimeInMilliSec()/20.0f);
		
		resultCameraPos.copyFrom(cameraPos);
		resultCameraPos.addXYZ(0.0f,0.0f,resultShake*0.5f);
		
		cameraShake += (0.0f - cameraShake)*timeDelta*8.0f;
		

	}
void Singleton::moveCamera (FIVector4 * pModXYZ)
        {
		
		int i;
		
		if (
				(pModXYZ->getFX() != 0.0) ||
				(pModXYZ->getFY() != 0.0) ||
				(pModXYZ->getFZ() != 0.0)
		) {
			
			if (!smoothMove) {
				amountInvalidMove = pModXYZ->length();
				depthInvalidMove = amountInvalidMove > 0.01f;
			}
			
			wsBufferInvalid = true;
		}
		
		camLerpPos.addXYZRef(pModXYZ);
		
		//cameraPos->addXYZRef(pModXYZ);

		//pModXYZ->setFZ(0.0f);
		
		
		
	}
GLfloat Singleton::getCamRot (int ind)
                                   {
		
		return curCamRotation[ind];
		
	}
void Singleton::moveObject (float dx, float dy)
        {
		

		int i;
		int j;
		int diffx;
		int diffy;
		
		
		GameOrgNode* curNode;


		float deltaMod = 1.0f/32.0f;
		
		modXYZ.setFXYZ(0.0f,0.0f,0.0f);

		float dxZM = dx;
		float dyZM = dy;

		bool doDefault = false;
		
		
		
		
		//////////
		
		
		float xmod = 0.0f;
		float ymod = 0.0f;
		float zmod = 0.0f;
		float xrotrad = getCamRot(0);
		float yrotrad = getCamRot(1);
		
		

		
		
		
		
		//////////
		
		
		
		
				

		if (abDown) {
			
			if ( shiftDown() ) { //rbDown || 
				zmod += dy;

			}
			else {
				
				xmod += float(sin(yrotrad))*dy;
				ymod += float(cos(yrotrad))*dy;
				//zmod -= float(cos(xrotrad))*dy;

				xmod += float(cos(yrotrad))*(dx); //??
				ymod -= float(sin(yrotrad))*(dx);
			}
			
			modXYZ.setFXYZ(
				xmod*4.0f,
				ymod*4.0f,
				zmod*4.0f
			);
			



		}
		
		
		if (
			(gem->orgOn) && 
			(gem->activeNode != NULL) &&
			gem->editPose
		) {
			gem->applyNodeChanges(gem->activeNode, dx, dy);
		}
		else {
			if (
				shiftDown() && 
				(mouseState == E_MOUSE_STATE_BRUSH) && 
				lbDown
			) {
				
				curBrushRad -= modXYZ.getFZ() / 50.0f;
				if (curBrushRad < 0.0f)
				{
					curBrushRad = 0.0f;
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
						
						if (dynObjects[activeObject]->moveType == E_MT_TRACKBALL) {
							if (lbDown&&rbDown) {
								dynObjects[activeObject]->posTrackball.addXYZ(0.0f,0.0f,dy*deltaMod);
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
					
					
					if (
						rbDown
						//&&( (gem->getCurActor()==NULL) || (gem->editPose)	)
					) {
						
						camRotation[0] -= dx*0.01f;
						camRotation[1] += dy*0.01f;
					}
					
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
void Singleton::setCameraToElevation ()
                                    {

		float newHeight = getHeightAtPixelPos(cameraGetPosNoShake()->getFX(), cameraGetPosNoShake()->getFY());
		
		newHeight = max(newHeight,getSeaHeightScaled()+64.0f);
		
		float curHeight = cameraGetPosNoShake()->getFZ();

		cout << "curHeight " << curHeight << " newHeight " << newHeight << "\n";

		modXYZ.setFXYZ(
			0.0,
			0.0,
			128.0f + newHeight - curHeight
		);
		
		moveCamera(&modXYZ);
		cameraGetPosNoShake()->copyFrom(&camLerpPos);

	}
btVector3 Singleton::screenToWorld (float mx, float my, float camAng)
          {
		
		//Matrix4 matProjection = viewMatrix*projMatrix;
		//Matrix4 matInverse = matProjection.invert();

		// Matrix4 matInverse(viewMatrixDI);

		// Vector4 vIn = Vector4(
		// 	(2.0f*mx)-1.0f,
		// 	1.0f-(2.0f*my),
		// 	0.5, //2.0f*mz-1.0f;
		// 	1.0f
		// );
		
		float baseAng = atan2(1.0f-(2.0f*my),(2.0f*mx)-1.0f);
		baseAng += camAng + M_PI/2.0f;
		
		
		
		
		// Vector4 pos = matInverse * vIn;

		// pos.w = 1.0 / pos.w;
		// pos.x *= pos.w;
		// pos.y *= pos.w;
		// pos.z *= pos.w;
		
		//return btVector3(pos.x, pos.y, pos.z);

		return btVector3(cos(baseAng),sin(baseAng),0.0f);

	}
btVector3 Singleton::getRayTo (float x, float y)
                                             {

		//float top = 1.f;
		//float bottom = -1.f;
		//float nearPlane = 1.f;
		// float tanFov = //(top-bottom)*0.5f / nearPlane;
		// float fov = FOV//btScalar(2.0) * btAtan(tanFov);

		//btVector3 camPos,camTarget;
		

		btVector3 rayFrom = cameraGetPosNoShake()->getBTV();
		btVector3 rayForward = lookAtVec.getBTV();
		rayForward.normalize();
		float farPlane = clipDist[1];// 10000.f;
		rayForward*= farPlane;

		btVector3 rightOffset;
		btVector3 cameraUp=btVector3(0,0,1);

		btVector3 vertical = cameraUp;

		btVector3 hor;
		hor = rayForward.cross(vertical);
		hor.safeNormalize();
		vertical = hor.cross(rayForward);
		vertical.safeNormalize();

		float tanfov = tanf(0.5f*FOV);


		hor *= 2.f * farPlane * tanfov;
		vertical *= 2.f * farPlane * tanfov;

		btScalar aspect;
		float width = origWinW;
		float height = origWinH;

		aspect =  width / height;

		hor*=aspect;


		btVector3 rayToCenter = rayFrom + rayForward;
		btVector3 dHor = hor * 1.f/width;
		btVector3 dVert = vertical * 1.f/height;


		btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
		//rayTo += btScalar(x) * dHor;
		//rayTo -= btScalar(y) * dVert;
		return rayTo;
	}
void Singleton::runReport ()
                         {
		
		//cout << "pathFinalStack.size() " << gameLogic->pathFinalStack.size() << "\n";
		
		//tempCounter++;
		
		
		doPathReport = true;
		
		// if (tempCounter >= gameLogic->pathFinalStack.size()) {
		// 	tempCounter = 0;
		// }
		
		//mainGUI->runReport();
		
		
		// cout << "lastMouseX" << lastMouseX << "\n";
		// cout << "lastMouseY" << lastMouseY << "\n";
		
		// cout << "polyCount " << polyCount << "\n";
		
		// doTraceVecND("cameraPos ", cameraGetPosNoShake());
		// doTraceVecND("lookAtVec ", &lookAtVec);
		// cout << "\n";
		
		// cout << "lightCount: " << gw->lightCount << "\n";
		//cout << "TOT GPU MEM USED (MB): " << TOT_GPU_MEM_USAGE << "\n";
		// cout << "HolderSize (MB): " << holderSizeMB << "\n";
		// cout << "totalPointCount: " << totalPointCount << "\n";
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
void Singleton::getMarkerPos (int x, int y)
                                        {
		getPixData(&spaceUpPD, x, y, true, false);
		worldMarker.copyFrom(&spaceUpPD);
		lastCellPos.copyFrom(&(worldMarker));
	}
void Singleton::resetGeom ()
                         {
		int i;
		geomStep = 0;
		geomOrigOffset.setFXYZ(0.0f);
		geomPoints[0].setFXYZ(0.0f,0.0f,0.0f);
		
		for (i = 0; i < FLOATS_PER_TEMPLATE; i++) {
			paramArrGeom[i] = defaultTemplate[i];
		}
	}
void Singleton::processInput (unsigned char key, bool keyDown, int x, int y)
                                                                         {
		
		int i;
		int tempType = E_ENTTYPE_NPC;
		
		GamePageHolder* curHolder;
		
		if (inputOn) {
			if (keyDown) {
				
			}
			else {
				processFieldInput(key);	
			}
			return;
		}
		
		unsigned char newKey = key;
		
		if (
			(key >= 'A') &&
			(key <= 'Z')
		) {
			newKey += 32;
		}
		
		keysPressed[newKey] = keyDown;
		
		GameOrg* testOrg = gem->getCurOrg();
		
		
		if (keyDown) {
			
		}		
		else {
			switch (key) {

				



			
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


				
				
				case '1':
				case '2':
				case '3':
				
					switch(key) {
						case '1':
							tempType = E_ENTTYPE_NPC;
						break;
						case '2':
							tempType = E_ENTTYPE_WEAPON;
						break;
						case '3':
							tempType = E_ENTTYPE_WEAPON;
							gem->weaponToPlace = E_PG_WPSPEAR;
						break;
					}
				
					if (updateHolders) {
						getMarkerPos(x, y);
						gem->placeNewEnt(gameNetwork->isConnected,tempType,&lastCellPos);
						
						if (key == '2') {
							gem->weaponToPlace++;
							if (gem->weaponToPlace > E_PG_WPSPEAR) {
								gem->weaponToPlace = E_PG_WPSWORD;
							}
						}
						
						gem->refreshTurnList();
						
					}
					else {
						cout << "Turn On Holder Update (u)\n";
						doAlert();
					}
					
				break;
				//case '0':
					
				//break;
				
				
				case '9':
					renderingOctBounds = !renderingOctBounds;
				break;
				case '/':
					//gameOct->captureBuffer(true);
					//gameOct->updateTBO();
					//gameOct->updateVBO();
				break;
				case '*':
					//renderingOct = !renderingOct;
				break;
				case '-':
					//gameOct->modRenderLevel(-1);
				break;
				case '+':
					//gameOct->modRenderLevel(1);
				break;
				
				case '`':
					placingGeom = !placingGeom;
					if (placingGeom) {
						resetGeom();
					}
					else {
						
					}
				break;
				
				
				case 'i':
						
						ignoreFrameLimit = !ignoreFrameLimit;
						cout << "ignoreFrameLimit: " << ignoreFrameLimit << "\n";
					break;
				case 19: //ctrl-s
					saveExternalJSON();
					saveGUIValues();
					gem->saveCurrentPose();
					//cout << "Use s key in web editor to save\n";
					break;

				case 15: //ctrl-o
					//loadAllData();
					loadValuesGUI();

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
					
					// if (updateHolders) {
					// 	if (pathfindingOn) {
					// 		updateHolders = false;
					// 		pathfindingOn = false;
					// 	}
					// 	else {
					// 		pathfindingOn = true;
					// 	}
					// }
					// else {
					// 	updateHolders = true;
					// }
				
					updateHolders = !updateHolders;
					//pathfindingGen = updateHolders;
					
					
					cout << "\n";
					cout << "updateHolders " << updateHolders << "\n";
					
					
					
				break;
				
				
				case 'Q':
					gem->toggleFirstPerson();
				break;
				
				case 'q':
					
					gem->toggleActorSel();
					
					break;

				
				case 'W':
					cout << "start FPS timer\n";
					fpsTest = true;
					fpsCount = 0;
					fpsTimer.start();
				break;
				

				case 27: // esc
					//std::exit(0);
					
					if (menuList[E_FM_DDMENU]->visible || menuList[E_FM_CONTMENU]->visible) {
						toggleDDMenu(x,y,false);
						menuList[E_FM_CONTMENU]->visible = false;
						gem->closeAllContainers();
						escCount = 0;
					}
					else {
						
						escCount++;
						if (escCount >= 3) {
							std::exit(0);
						}
					}
					
					//pickerMenu->visible = false;
					
					
					
					break;

				case 'b':
				
					bakeParamsOn = !bakeParamsOn;
					cout << "bakeParamsOn: " << bakeParamsOn << "\n";
					doShaderRefresh(bakeParamsOn);
					
					
					
				
					//radiosityOn = !radiosityOn;
					break;


				case 'R':
				
					//loadValuesGUI(false);
					gw->noiseGenerated = false;
					gw->blockHolder->wasGenerated = false;
					
					loadGUI();
					loadValuesGUI();
					gem->loadPoseInfo(true);
					
				break;
				case 'r':
					
					
					
					doShaderRefresh(bakeParamsOn);
					cout << "Shaders Refreshed\n";

					
					
					break;
				
				case 'n':
					//gem->nextSwing(gem->getCurActorUID(),RLBN_LEFT);
					//gem->nextSwing(gem->getCurActorUID(),RLBN_RIGT);
					runReport();
					
					
				break;
				
				case 'N':
					gem->saveEveryPose();
				break;
				
				case 'j':
				
					pathfindingTestOn = !pathfindingTestOn;
					cout << "pathfindingTestOn: " << pathfindingTestOn << "\n";
				
					//gem->resetActiveNode();
				
					// 
					// doShaderRefresh(bakeParamsOn);
					// mapInvalid = true;
					// gw->initMap();
				break;

				case 'J':
				
					doShaderRefresh(bakeParamsOn);
					mapInvalid = true;
					gw->initMap();
				
				break;

				case 'G':
				
					
				
					gridOn = !gridOn;
					cout << "Grid On: " << gridOn << "\n";

					break;


				case 'g':
				
					mouseState++;

					
					if (mouseState == E_MOUSE_STATE_PICKING) {
						
						mouseState = E_MOUSE_STATE_BRUSH;
						
						if (earthMod == E_PTT_WAT) {
							mouseState = 0;
							earthMod = E_PTT_TER;
						}
						else {
							earthMod = E_PTT_WAT;
						}
						
					}
					

					
					
					cout << mouseStateStrings[mouseState] << "\n";


					wsBufferInvalid = true;
					forceGetPD = true;
				
					
				break;
				
				case 'k':
					gem->destroyTerrain = !(gem->destroyTerrain);
					cout << "destroyTerrain: " << gem->destroyTerrain << "\n";
					//gameAI->getKB();
				break;
				
				case 'l':

					multiLights = !multiLights;
					updateMultiLights();


					forceGetPD = true;

					break;

				case ';':
					physicsOn = !physicsOn;
					cout << "physicsOn: " << physicsOn << "\n";
				break;
				case 'p':
				
					curPattern++;
					if (curPattern >= E_PAT_LENGTH) {
						curPattern = 0;
					}
				
					
				break;
				case 'P':
				
					placingPattern = !placingPattern;
					cout << "placingPattern: " << placingPattern << "\n";
				
					//toggleFullScreen();
				break;
				case '\\':
					toggleFullScreen();
				break;
				
				case 'o':
					//targetTimeOfDay = 1.0f-targetTimeOfDay;
					sphereMapOn = !sphereMapOn;
					
					break;

				case 'h':
					
					// diagCount++;
					
					// if (diagCount >= 9) {
					// 	diagCount = 0;
					// }
					
					//gameFluid[E_FID_SML]->updateTBOData(false,true);
					//gameFluid[E_FID_BIG]->updateTBOData(false,true);
					
					gem->showHealth = !(gem->showHealth);
					
					break;
					
				case 'y':
					// throw
				break;
				case 'T':
					testOn = !testOn;
					
				break;
				case 't':
				
					
				
					//testOn2 = !testOn2;
					//testOn3 = !testOn3;
					renderingOct = !renderingOct;
					if (renderingOct) {
						gw->updateTBOPool(5);
					}
					
					// if (renderingOct) {
					// 	gameLogic->threadPoolList->stopAll();
					// 	gameLogic->threadPoolPath->stopAll();
					// }
					
					
					//pathfindingTestOn = !pathfindingTestOn;
					
				break;
				// case 'o':
				// 	//rotOn = !rotOn;
				// 	break;
					

				case '\t':
				
					if (menuList[E_FM_DDMENU]->visible || menuList[E_FM_CONTMENU]->visible) {
						toggleDDMenu(x,y,false);
						menuList[E_FM_CONTMENU]->visible = false;
						gem->closeAllContainers();
						
						escCount = 0;
					}
					else {
						if (mainGUI->isReady) {
							if (menuList[E_FM_MAINMENU] == NULL) {
								
							}
							else {
								if (menuList[E_FM_MAINMENU]->visible) {
									playSoundEvent("hideGUI");
								}
								
								menuList[E_FM_MAINMENU]->visible = !(menuList[E_FM_MAINMENU]->visible);
								
								if (menuList[E_FM_MAINMENU]->visible) {
									playSoundEvent("showGUI");
								}
							}
						}
					}
					
					break;

				case ' ':
					
					
					
				
					//timeMod = !timeMod;
					// if (mouseState == E_MOUSE_STATE_PICKING) {
					// 	selectedEnts.cycleEnts();
					// }
					
					showHudMenu( !(menuList[E_FM_HUDMENU]->visible) );
					
					//showStatMenu( !(menuList[E_FM_STATMENU]->visible) );
					
					//cout << makePretty("E_TEST_STRING_VALUE", "E_TEST_") << "\n";
					
					
				break;
				
				
				case 'C':
				
					
				break;
				case 'c':
					
					
					gem->setTurnBased(!(gem->turnBased));
					gem->combatOn = (gem->turnBased);
					gridOn = gem->combatOn;
					//gem->combatOn = !(gem->combatOn);
					//cout << "gem->combatOn " << gem->combatOn << "\n";
					
					
					//setCameraToElevation();
				
					//doShaderRefresh(bakeParamsOn);

					break;
					
				case 'v':
					gem->togglePoseEdit();
					
					
					//waterBulletOn = !waterBulletOn;
					//gw->toggleVis(selectedEnts.getSelectedEnt());
					break;
					
				case 'V':
					gem->applyNonPoseData();
				break;
				

				case 'X':
					//fogOn = 1.0 - fogOn;
					//cout << "fog on " << fogOn << "\n";
					gem->changePose(-1);
					break;
					
				case 'x':
					gem->changePose(1);
					
					//fxaaOn = !fxaaOn;
					//cout << "fxaaOn " << fxaaOn << "\n";
					break;

				case 'm':


					gem->mirrorOn = !gem->mirrorOn;
					cout << "gem->mirrorOn " << gem->mirrorOn << "\n";
					
					// doPathReport = true;

					// medianCount++;					
					// if (medianCount == 4) {
					// 	medianCount = 0;
					// }
					
					//
					
					// refreshPaths = true;
					

					break;
				case 'M':
					// smoothMove = !smoothMove;
					// cout << "smoothMove " << smoothMove << "\n";
					
					medianCount++;					
					if (medianCount == 4) {
						medianCount = 0;
					
					}
					cout << "medianCount " << medianCount << "\n";
					
				break;
				

				

				case 'A':
				case 'Z':
				case 'S':
				case 'F':
				case 'E':
				case 'D':
				case 'a':
				case 'z':
				case 's':
				case 'f':
				case 'e':
				case 'd':
					// reserved for movement
				break;
				
				// 127 del
				// 8 backspace

				default:
					cout << "No code for key: " << ((int)key) << "\n";
					break;
			}
			
			
			if (gem->turnBased&&(gem->getCurActor() != NULL)) {
				switch(key) {
					case 'a':
						
					break;
					case 'z':
						
					break;
					
					case 'w':
						gem->makeGrab(gem->getCurActor()->uid, -1);
					break;
					case 'y':
						gem->makeThrow(gem->getCurActor()->uid,-1);
					break;
					
					case 's':
						gem->makeTurnTB(gem->getCurActor()->uid, 1);
						
					break;
					case 'f':
						gem->makeTurnTB(gem->getCurActor()->uid, -1);
						
					break;
					
					case 'e':
						if (gem->makeMoveTB(gem->getCurActor()->uid, 1)) {
							gem->endHumanTurn();
						}
					break;
					case 'd':
						if (gem->makeMoveTB(gem->getCurActor()->uid, -1)) {
							gem->endHumanTurn();
						}
					break;
				}
			}
			
		}
		
		
	}
void Singleton::getPixData (FIVector4 * toVector, int _xv, int _yv, bool forceUpdate, bool isObj)
        {

		

		if (fpsTest) {
			return;
		}

		FBOWrapper *fbow;
		int newX;
		int newY;
		
		float xv = _xv;
		float yv = _yv;
		
		float bufx;
		float bufy;

		if (isObj) {
			//fbow = getFBOWrapper("geomBaseTargFBO",2);
			fbow = getFBOWrapper("limbFBO", 0);
		}
		else {
			fbow = getFBOWrapper("solidTargFBO", 0);
		}
		
		float srcW = fbow->width;
		float srcH = fbow->height;
		
		bufx = xv/bufferDim.getFX();
		bufy = 1.0f-yv/bufferDim.getFY();

		if (wsBufferInvalid || forceUpdate || forceGetPD) {
			
			// glFlush();
			// glFinish();
			
			fbow->getPixels();
			
			// glFlush();
			// glFinish();
			
		}

		// newX = clamp(xv, 0, bufx - 1);
		// newY = clamp(yv, 0, bufy - 1);

		fbow->getPixelAtF(
			toVector, 
			bufx*srcW,
			bufy*srcH
		);
			//newX, ((bufy - 1) - newY));

		wsBufferInvalid = false;
		forceGetPD = false;
		
	}
float Singleton::getMinGeom (int baseIndex)
                                        {
		int newIndex = baseIndex*4;
		
		return min(
			min(
				paramArrGeom[newIndex + 0],
				paramArrGeom[newIndex + 1]
			),
			paramArrGeom[newIndex + 2]
		);
		
	}
void Singleton::setFXYZWGeom (int baseIndex, FIVector4 * baseVec)
                                                             {
		int newIndex = baseIndex*4;
		int i;
		
		for (i = 0; i < 4; i++) {
			paramArrGeom[newIndex + i] = baseVec->getIndex(i);
		}
		
	}
void Singleton::setFXYGeom (int baseIndex, float xv, float yv)
                                                           {
		int newIndex = baseIndex*4;
		paramArrGeom[newIndex + 0] = xv;
		paramArrGeom[newIndex + 1] = yv;
	}
void Singleton::setFXGeom (int baseIndex, float xv)
                                                {
		int newIndex = baseIndex*4;
		paramArrGeom[newIndex + 0] = xv;
	}
void Singleton::setFYGeom (int baseIndex, float yv)
                                                {
		int newIndex = baseIndex*4;
		paramArrGeom[newIndex + 1] = yv;
	}
void Singleton::setFZGeom (int baseIndex, float zv)
                                                {
		int newIndex = baseIndex*4;
		paramArrGeom[newIndex + 2] = zv;
	}
void Singleton::setFWGeom (int baseIndex, float wv)
                                                {
		int newIndex = baseIndex*4;
		paramArrGeom[newIndex + 3] = wv;
	}
float Singleton::getFXGeom (int baseIndex)
                                       {
		int newIndex = baseIndex*4;
		return paramArrGeom[newIndex + 0];
	}
float Singleton::getFYGeom (int baseIndex)
                                       {
		int newIndex = baseIndex*4;
		return paramArrGeom[newIndex + 1];
	}
float Singleton::getFZGeom (int baseIndex)
                                       {
		int newIndex = baseIndex*4;
		return paramArrGeom[newIndex + 2];
	}
float Singleton::getFWGeom (int baseIndex)
                                       {
		int newIndex = baseIndex*4;
		return paramArrGeom[newIndex + 3];
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
void Singleton::keyboardUp (unsigned char key, int _x, int _y)
        {
		int x = _x / scaleFactor;
		int y = _y / scaleFactor;
		
		frameMouseMove = true;
		
		GameOrgNode* curNode;
		
		
		

		
		bShift = shiftDown();
		bCtrl = ctrlDown();
		updateCS();


		if (key == 17) {
			glutLeaveMainLoop();
		}

		keyStack.push_back(KeyStackEvent());
		keyStack.back().key = key;
		keyStack.back().keyDown = false;
		keyStack.back().x = x;
		keyStack.back().y = y;

		//processInput(key, false, x, y);


	}
void Singleton::keyboardDown (unsigned char key, int _x, int _y)
        {
		int x = _x / scaleFactor;
		int y = _y / scaleFactor;
		
		frameMouseMove = true;
		
		
		
		
		
		bShift = shiftDown();
		bCtrl = ctrlDown();
		updateCS();

		keyStack.push_back(KeyStackEvent());
		keyStack.back().key = key;
		keyStack.back().keyDown = true;
		keyStack.back().x = x;
		keyStack.back().y = y;
		//processInput(key, true, x, y);
	}
void Singleton::updateCurGeom (int x, int y)
                                         {
		
		float maxDis = 30.0;
		
		int i;
		int j;
		
		float xv;
		float yv;
		float xv2;
		float yv2;
		float minCornerDis;
		float maxCornerDis;
		
		int baseInd;
		
		if (geomStep == 0) {
			geomPoints[geomStep].setFXYZRef(&mouseMovePD);
			geomPoints[geomStep].setFW(-1.0f);
			geomPoints[geomStep].floorXYZ();
			geomStep++;
		}
		//else {
			geomPoints[geomStep].setFXYZ(
				((float)x)/bufferDim.getFX(),
				((float)y)/bufferDim.getFY(),
				0.0f
			);
		//}
		
		
		float xrotrad = getCamRot(0);
		float yrotrad = getCamRot(1);
		
		for (i = 0; i <= geomStep; i++) {
			
			xv = (geomPoints[i][0]-0.5f)*2.0f;
			yv = (geomPoints[i][1]-0.5f)*2.0f;
			
			xv2 = xv;
			yv2 = yv;
			
			xv = abs(xv);
			yv = abs(yv);
			
			xv *= maxDis;
			yv *= maxDis;
			yv2 *= maxDis;
			xv2 *= maxDis;
			
			
			if (placingTemplate) {
				switch(i) {
					case E_GEOM_POINTS_TEMP_ORIGIN:
						// wheel (no longer) changes template
						
						
					
					break;
					case E_GEOM_POINTS_TEMP_OFFSET:
					
					//rrrrr
					//xmod += float(cos(xrotrad));
					//ymod -= float(sin(xrotrad));
					
					
					
						geomOrigOffset.setFXYZ(
							-signedFloor(
								cos(xrotrad)*yv2 + -sin(xrotrad)*xv2
							),
							-signedFloor(
								sin(xrotrad)*yv2 + cos(xrotrad)*xv2
							),
							signedFloor(curPrimMod)
						);
					break;
				}
				
				baseInd = curPrimTemplate*E_PRIMTEMP_LENGTH;
				
				for (j = 0; j < E_PRIMTEMP_LENGTH; j++) {
					setFXYZWGeom(j, &(primTemplateStack[baseInd+j]) );
				}
			}
			else {
				switch (i) {
					case E_GEOM_POINTS_ORIGIN:
						
					break;
					case E_GEOM_POINTS_RAD_XYZ:
						setFXYGeom(E_PRIMTEMP_BOUNDSMIN, signedFloor(-xv), signedFloor(-yv));
						setFXYGeom(E_PRIMTEMP_BOUNDSMAX, signedFloor( xv), signedFloor( yv));
						
						setFXYGeom(E_PRIMTEMP_VISMIN, signedFloor(-xv), signedFloor(-yv));
						setFXYGeom(E_PRIMTEMP_VISMAX, signedFloor( xv), signedFloor( yv));
						
						setFZGeom(E_PRIMTEMP_BOUNDSMIN, signedFloor(-curPrimMod));
						setFZGeom(E_PRIMTEMP_BOUNDSMAX, signedFloor( curPrimMod));
						
						setFZGeom(E_PRIMTEMP_VISMIN, signedFloor(-curPrimMod));
						setFZGeom(E_PRIMTEMP_VISMAX, signedFloor( curPrimMod));
						
					break;
					case E_GEOM_POINTS_OFFSET:
						geomOrigOffset.setFXYZ(
							signedFloor(xv2),
							signedFloor(yv2),
							signedFloor(curPrimMod)
						);
					break;
					case E_GEOM_POINTS_CORNER:
					
						minCornerDis = 0.1;
						maxCornerDis = getMinGeom(E_PRIMTEMP_BOUNDSMAX);
						
						setFXGeom(E_PRIMTEMP_CORNERDIS, 
							signedFloor(mixf(
								minCornerDis,
								maxCornerDis,
								clampfZO(yv/(maxDis*0.5f))	
							))
						);
					break;
					case E_GEOM_POINTS_POWER_VALS:
						setFZGeom(E_PRIMTEMP_CORNERDIS, xv + 1.0f);
						setFWGeom(E_PRIMTEMP_CORNERDIS, yv + 1.0f);
					break;
					case E_GEOM_POINTS_NEG_RAD_XY:
						setFXYGeom(E_PRIMTEMP_VISMIN, 
							signedFloor(max(xv2,getFXGeom(E_PRIMTEMP_BOUNDSMIN))),
							signedFloor(max(yv2,getFYGeom(E_PRIMTEMP_BOUNDSMIN)))
						);
					break;
					case E_GEOM_POINTS_POS_RAD_XY:						
						setFXYGeom(E_PRIMTEMP_VISMAX, 
							signedFloor(min(xv2,getFXGeom(E_PRIMTEMP_BOUNDSMAX))),
							signedFloor(min(yv2,getFYGeom(E_PRIMTEMP_BOUNDSMAX)))
						);
					
					break;
					case E_GEOM_POINTS_NEG_RAD_Z:
						setFZGeom(E_PRIMTEMP_VISMIN, signedFloor(max(yv2,getFZGeom(E_PRIMTEMP_BOUNDSMIN))) );
					break;
					case E_GEOM_POINTS_POS_RAD_Z:
						setFZGeom(E_PRIMTEMP_VISMAX, signedFloor(min(yv2,getFZGeom(E_PRIMTEMP_BOUNDSMAX))) );
					break;
					case E_GEOM_POINTS_THICKNESS:
						setFYGeom(E_PRIMTEMP_CORNERDIS, signedFloor(yv));
					break;
				}
			}
			
			
		}
		
		
	}
void Singleton::mouseMove (int _x, int _y)
        {

		int i;
		int j;

		

		frameMouseMove = true;

		mouseMoved = true;

		lastMouseOrigX = _x;
		lastMouseOrigY = _y;

		int x = _x / scaleFactor;
		int y = _y / scaleFactor;

		int dx = _x - lastPosX;
		int dy = _y - lastPosY;
		
		guiX = _x/UI_SCALE_FACTOR;
		guiY = _y/UI_SCALE_FACTOR;

		bShift = shiftDown();
		bCtrl = ctrlDown();
		updateCS();

		lastMouseX = x;
		lastMouseY = y;

		mouseXUp = x;
		mouseYUp = y;

		bool ddVis = false;
		bool noTravel = false;
		
		if (menuList[E_FM_DDMENU] != NULL) {
			ddVis = menuList[E_FM_DDMENU]->visible;
		}
		
		float fx = ((float)x)*M_PI*2.0f / bufferDim[0];
		float fy = ((float)y)*M_PI / bufferDim[1];
		
		
		if (hitGUI) {
			return;
		}
		
		
		if ((gem->highlightedLimb==-1)&&mbDown) {
			angleToVec(&lightVec, fx*2.0, fy*2.0);
			lightVecOrig.copyFrom(&lightVec);
			lightVec.setFZ(-abs(lightVec.getFZ()));
		}
		
		
		if (abDown)
		{
			
			// if (bCtrl&&(mouseState == E_MOUSE_STATE_BRUSH)) {
				
			// }
			// else {
				
			// }
			
			moveObject((float)dx, (float)dy);
			
		}
		else
		{
			
			// if (
			// 	(gem->getCurActor() != NULL) && (!gem->editPose)	
			// ) {
			// 	camRotation[0] -= dx*0.02f;
			// 	camRotation[1] += dy*0.02f;
			// }
			
			
			

			if ( placingPattern||placingGeom||RT_TRANSFORM||gem->editPose||pathfindingTestOn||(mouseState != E_MOUSE_STATE_MOVE)) {
			//if (true) {
				getPixData(&mouseMovePD, x, y, false, false);
				getPixData(&mouseMoveOPD, x, y, true, true);
			}
			

			gw->updateMouseCoords(&mouseMovePD);
			
			if (pathfindingTestOn) {
				
				if (gameLogic->getClosestPathRad(mouseMovePD.getBTV(), closestHolder) > -1) {
					
					if (pathFindingStep < 2) {
						gameLogic->testPath.points[pathFindingStep] = mouseMovePD.getBTV();
					}
				}
				
				
			}
			

			//////////////

			

			if (
				gem->orgOn &&
				gem->editPose
				&& (!ddVis)
			) {
				gem->updateNearestOrgNode(false, &mouseMovePD);
			}
			else {
				if (!ddVis) {
					gem->activeNode = NULL;
					gem->setSelNode(NULL);
				}
				
			}
			
			if (placingGeom) {
				updateCurGeom(x, y);
			}
			else {
				
				gw->findNearestEnt(
					&highlightedEnts,
					E_ET_GEOM,
					2,
					1,
					&mouseMovePD
				);
				highlightedEnt = highlightedEnts.getSelectedEnt();


			}
			

			

			//////////////


		}

		lastPosX = _x;
		lastPosY = _y;

		
	}
void Singleton::mouseClick (int button, int state, int _x, int _y)
                                                               {
		
		int tempInt;

		FIVector4 newPos;

		int x = _x / scaleFactor;
		int y = _y / scaleFactor;

		float mx = ((float)_x)/origWinW;
		float my = ((float)_y)/origWinH;

		lastPosX = _x;
		lastPosY = _y;
		
		guiX = _x/UI_SCALE_FACTOR;
		guiY = _y/UI_SCALE_FACTOR;

		GameBlock *curBlock;

		int buttonInt = 0;

		int res;
		int i;
		int j;
		int k;
		int ii;
		int jj;
		int bestInd;
		int upInd;
		float bestDis;
		float curDis;
		bool findObject = false;
		bool hitObject = false;
		bool wasDoubleClick[2];
		wasDoubleClick[0] = false;
		wasDoubleClick[1] = false;
		
		bool ddVis = false;
		if (menuList[E_FM_DDMENU] != NULL) {
			ddVis = menuList[E_FM_DDMENU]->visible;
		}

		float wheelDelta = 0.0f;
		int wheelDeltaInt = 0;
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
		bool longClick = false;
		
		
		
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

		
		}

		abDown = lbDown || rbDown || mbDown;
		abClicked = lbClicked || rbClicked || mbClicked;
		
		
		if (mbClicked) {
			gem->checkActorRefresh();
		}
		
		if (abDown) {
			mdTime = myTimer.getElapsedTimeInMilliSec();
			
			
		}
		else {
			muTime = myTimer.getElapsedTimeInMilliSec();
			longClick = (muTime - mdTime) > 300.0f;
		}
		
		int curHand = -1;
		
		
		
		
		if (lbClicked) {
			curHand = RLBN_LEFT;
			
		}
		if (rbClicked) {
			curHand = RLBN_RIGT;	
		}
		
		if (state == GLUT_UP) {
			if (curHand > -1) {
				mdTimeLR[curHand] = myTimer.getElapsedTimeInMilliSec();
				
				if ( (mdTimeLR[curHand]-clickTimeLR[curHand]) < 500 ) {
					wasDoubleClick[curHand] = true;
				}
				
				
				clickTimeLR[curHand] = mdTimeLR[curHand];
			}
		}
		
		
		
		
		
		
		if (abDown) {
			mouseStart.setIXY(x, y);
		}
		if (abClicked) {
			mouseEnd.setIXY(x, y);
			noTravel = mouseEnd.distance(&mouseStart) < MAX_TRAVEL_DIS;	
		}
		
		if (anyMenuVisible()) {
			hitGUI = mainGUI->testHit(button, state, guiX, guiY);
		}
		

		if (hitGUI) {
			return;
		}
		
		
		if (gem->combatMode()) {
			
			if (lbClicked) {
				gem->setSwing(
					mx*2.0f - 1.0f,
					my*2.0f - 1.0f,
					gem->getCurActor()->uid,
					RLBN_LEFT,
					bCtrl
				);
				gem->makeSwing(gem->getCurActor()->uid, RLBN_LEFT);
				if (gem->turnBased) {gem->endHumanTurn();}
				return;
			}
			if (rbClicked) {
				gem->setSwing(
					mx*2.0f - 1.0f,
					my*2.0f - 1.0f,
					gem->getCurActor()->uid,
					RLBN_RIGT,
					bCtrl
				);
				gem->makeSwing(gem->getCurActor()->uid, RLBN_RIGT);
				if (gem->turnBased) {gem->endHumanTurn();}
				return;
			}
			
		}
		
		
		
		//cout << "NO GUI HIT\n";
		
		
		wsBufferInvalid = true;
		
		if (abDown) {
			getPixData(&mouseDownPD, x, y, true, false);
			getPixData(&mouseDownOPD, x, y, true, true);
		}
		else {
			getPixData(&mouseUpPD, x, y, true, false);
			getPixData(&mouseUpOPD, x, y, true, true);
		}
		
		// if (lbDown) {
		// 	if (gamePhysics != NULL) {
		// 		gamePhysics->pickBody(mouseDownPD.getBTV(),mouseDownOPD.getBTV());
		// 	}
		// }
		
		
		
		
		
		
		if (lbClicked) {
			gamePhysics->lastBodyPick = NULL;
			gamePhysics->lastBodyUID = -1;
		}
		
		

		if (abClicked) {

			if (menuList[E_FM_DDMENU] != NULL) {
				if (noTravel) {
					toggleDDMenu(x,y,false);
				}
				if (
					rbClicked &&
					(!lbDown) &&
					noTravel &&
					(!longClick) &&
					(mouseState != E_MOUSE_STATE_BRUSH)
				) {
					toggleDDMenu(x,y,true);
				}
			}
			
			
			if (abDown) {

			}
			else {
				if (lbClicked) {
					
					if (placingGeom) {
						geomStep++;
						curPrimMod = 0.0f;
						
						cout << "cur step: " << E_GEOM_POINTS_STR[geomStep] << "\n";
						
						if (
							(
								(geomStep == E_GEOM_POINTS_LENGTH) && (!placingTemplate)
							)
							||
							(
								(geomStep == E_GEOM_POINTS_TEMP_LENGTH) && (placingTemplate)
							)
						) {
							
							geomStep--;
							
							if (placingTemplate) {
								
								
								newPos.setFXYZRef(&(geomPoints[0]));
								newPos.addXYZRef(&geomOrigOffset);
								
								//dont add geom twice
								//gameFluid[E_FID_SML]->pushPlaceTemplate(true, &newPos, curPrimTemplate);
								gameFluid[E_FID_BIG]->pushPlaceTemplate(true, &newPos, curPrimTemplate);
								resetGeom();
							}
							
							
							
							placingGeom = false;
							
							
							
						}
						
						
						
					}
					else {
						
						doProc = true;
						
						
						
						if (doProc) {
							if (noTravel) {
								
								
								if (pathfindingTestOn) {
									
									pathFindingStep++;
									
									if (pathFindingStep == 3) {
										pathFindingStep = 0;
										gameLogic->testPath.points[0] = btVector3(0.0f,0.0f,0.0f);
										gameLogic->testPath.points[1] = btVector3(0.0f,0.0f,0.0f);
										gameLogic->testPath.searchedForPath = false;
										gameLogic->testPath.didFindPath = false;
										gameLogic->testPath.nextInd = -1;
										gameLogic->testPath.finalPoints.clear();
									}
								}
								else {
									
								}
								
								
							}
							else {
								gem->endDrag(mouseUpOPD.getFW());
							}
						}
						
						
						
						
						
						

						if ( (activeObject == E_OBJ_CAMERA) && longClick ) {
							mouseMovingStart = (mouseMovingLoc - min(mouseMovingStepsBack, mouseCount) + mouseMovingSize) % mouseMovingSize;
						}


						if (!(shiftDown())) {


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
							else {

								switch (selectedEnt->buildingType)
								{
								case E_CT_DOOR:
								case E_CT_WINDOW:
									
									
									
									if (selectedEnt->toggled) {
										// open
										switch (selectedEnt->buildingType)
										{
											case E_CT_DOOR:
												playSoundPosAndPitch(
													"open3",
													cameraGetPosNoShake(),
													selectedEnt->getVisMinInPixelsT(),
													0.3f
												);
											break;
											case E_CT_WINDOW:
												playSoundPosAndPitch(
													"open1",
													cameraGetPosNoShake(),
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
													cameraGetPosNoShake(),
													selectedEnt->getVisMinInPixelsT(),
													0.3f
												);
											break;
											case E_CT_WINDOW:
												playSoundPosAndPitch(
													"close1",
													cameraGetPosNoShake(),
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
										cameraGetPosNoShake(),
										selectedEnt->getVisMinInPixelsT(),
										0.3f
									);
									gw->updateLights();
									cout << "final toggle " << selectedEnt->light->toggled << "\n";
									break;

								}

							}



						}

						
					}
					
					
				}
			}


		}
		else {
			if (abDown) {


				mouseMovingLoc = 0;
				mouseCount = 0;
				
				if (rbDown&&(!lbDown)) {
					activeObject = E_OBJ_CAMERA;
				}
			}
		}
		
		
		if (mouseState == E_MOUSE_STATE_MOVE) {
			
			if (
				gem->orgOn &&
				gem->editPose
				&& (!ddVis)
				
			) {
				findObject = !(gem->updateNearestOrgNode(true, &mouseDownPD));
			}
			else {
				findObject = true;
			}
			
			if (findObject) {
				
				gem->updateDragInfo(mouseDownOPD.getFW(), lbDown, wasDoubleClick[RLBN_LEFT]);
				
			}
		}
		
		
		
		

		
		if (!bShift) {
			if (noTravel) {
				switch (mouseState) {
					case E_MOUSE_STATE_BRUSH:
						
						
						
						if (lbClicked) {
							buttonInt = 0;
						}
						else if (rbClicked) {
							buttonInt = 1;
						}
						else if (mbClicked) {
							buttonInt = 2;
						}
						
						if (abClicked) {
							//gameFluid[E_FID_SML]->pushModifyUnit(true, &mouseUpPD, buttonInt, earthMod, curBrushRad);
							gameFluid[E_FID_BIG]->pushModifyUnit(true, &mouseUpPD, buttonInt, earthMod, curBrushRad);
							forceGetPD = true;
						}
						
						

					break;
				}
			}
		}
		
		
		//cout << "button " << button << "\n";
		//cout << "state " << state << "\n\n";
		
		
		switch (button) {
			case 3: // wheel up
			case 7: // rbDown
			case 11: // shift down
				wheelDelta = 1.0f / 20.0f;
				wheelDeltaInt = 1;
				break;

			case 4: // wheel down
			case 8: // rbDown
			case 12: // shift down
				wheelDelta = -1.0f / 20.0f;
				wheelDeltaInt = -1;
				break;
		}
		
		globWheelDelta += wheelDelta;
		
		
		if ((abs(globWheelDelta) > 0.01f)) {
			frameMouseMove = true;
		}
		
		if ((state==1)) {
			
			
			
			if (placingGeom) {
				
				if ((wheelDeltaInt != 0)) {
					if ((button == 7) || (button == 8)) {
						curPrimTemplate -= wheelDeltaInt;
						
						if (curPrimTemplate <= 1) {
							curPrimTemplate = 1;
						}
						if (curPrimTemplate >= (primTemplateStack.size()/E_PRIMTEMP_LENGTH)) {
							curPrimTemplate = (primTemplateStack.size()/E_PRIMTEMP_LENGTH) - 1;
						}
						
						cout << "curPrimTemplate " << curPrimTemplate << "\n";
					}
					else {
						curPrimMod += wheelDeltaInt;
					}
					
					updateCurGeom(x,y);
				}
				
				
				
			}
			 

		}
		
		

	}
bool Singleton::feetContact (BaseObj * ge)
                                      {
		BodyStruct* curBody;
		
		curBody = ge->getBodyByBoneId(E_BONE_L_LOWERLEG);
		if (curBody != NULL) {
			if (curBody->hasContact) {
				return true;
			}
		}
		curBody = ge->getBodyByBoneId(E_BONE_R_LOWERLEG);
		if (curBody != NULL) {
			if (curBody->hasContact) {
				return true;
			}
		}
		curBody = ge->getBodyByBoneId(E_BONE_L_TALUS);
		if (curBody != NULL) {
			if (curBody->hasContact) {
				return true;
			}
		}
		curBody = ge->getBodyByBoneId(E_BONE_R_TALUS);
		if (curBody != NULL) {
			if (curBody->hasContact) {
				return true;
			}
		}
		
		return false;
	}
void Singleton::flushKeyStack ()
                             {
		while (keyStack.size() > 0) {
			processInput(
				keyStack.front().key,
				keyStack.front().keyDown,
				keyStack.front().x,
				keyStack.front().y
			);
			keyStack.pop_front();
		}
	}
void Singleton::applyKeyAction (bool isReq, int actorId, uint keyFlags, float camRotX, float camRotY)
                                                                                                  {
		
		
		
		int i;
		BaseObj* ca;
		
		btVector3 mouseWP;
		
		bool charMoved;
		float deltaAng;
		
		if (isReq) {
			naUintData[0] = keyFlags;
			naIntData[0] = actorId;
			naFloatData[0] = camRotX;
			naFloatData[1] = camRotY;
			gameNetwork->addNetworkAction(E_NO_KEY_ACTION,naUintData,naIntData,naFloatData);
			return;
		}
		
		if (gem->turnBased) {
			return;
		}
		
		if (actorId < 0) {
			
		}
		else {
			
			ca = &(gem->gameObjects[actorId]);
			
			unzipBits(keyFlags,keyMapResultUnzipped,KEYMAP_LENGTH);
			
			if (ca->isAlive()) {
				
				if (keyMapResultUnzipped[KEYMAP_RIGHT]) {
					
					if (bShift) {
						gem->makeMove( actorId, btVector3( 1.0f,0.0f,0.0f), true, true );
					}
					else {
						gem->makeTurn(actorId, -conVals[E_CONST_TURN_AMOUNT]);
					}
					
					//
				}
				
				if (keyMapResultUnzipped[KEYMAP_LEFT]) {
					if (bShift) {
						gem->makeMove( actorId, btVector3(-1.0f,0.0f,0.0f), true, true );
					}
					else {
						gem->makeTurn(actorId, conVals[E_CONST_TURN_AMOUNT]);
					}
					
					//
				}
				
				if (keyMapResultUnzipped[KEYMAP_FIRE_PRIMARY]) {
					gem->makeShoot(actorId, E_ENTTYPE_BULLET);
				}
				
				if (keyMapResultUnzipped[KEYMAP_GRAB]) {
					gem->makeGrab(actorId,-1);
				}
				
				if (keyMapResultUnzipped[KEYMAP_THROW]) {
					gem->makeThrow(actorId,-1);
				}
				
				if (keyMapResultUnzipped[KEYMAP_UP]) {
					gem->makeJump(actorId, 1, 1.0f);
				}
				
				if (keyMapResultUnzipped[KEYMAP_DOWN]) {
					gem->makeJump(actorId, 0, 1.0f);
				}
				
				if (keyMapResultUnzipped[KEYMAP_FORWARD]) {
					gem->makeMove( actorId, btVector3(0.0f, 1.0f,0.0f), true, true );
				}
				
				if (keyMapResultUnzipped[KEYMAP_BACKWARD]) {
					gem->makeMove( actorId, btVector3(0.0f,-1.0f,0.0f), true, true );
				}
				
				// mouseWP = screenToWorld(
				// 	((float)lastPosX)/origWinW,
				// 	((float)lastPosY)/origWinH,
				// 	camRotX
				// );
				
				//getPixData(&mouseMovePD, lastMouseX, lastMouseY, false, false);
				
				if (bShift) {
					deltaAng = ca->turnTowardsPointDelta(
						// mouseMovePD.getBTV()
						
						ca->getCenterPoint(E_BDG_CENTER) +
						lookAtVec.getBTV()
						
						// ca->getCenterPoint(E_BDG_CENTER) - mouseWP
						
					);
					
					if (!gem->editPose) {
						gem->makeTurn(actorId, deltaAng*16.0f);
					}
				}
				
				
				
				
				
			}
		}
		
		
		
		
	}
void Singleton::gatherKeyActions ()
                                {
		int i;
		
		bool res;
		
		for (i = 0; i < KEYMAP_LENGTH; i++) {
			res = keysPressed[keyMap[i]];
			
			if (res) {
				if (keyMapCoolDown[i] == 0) {
					keyMapCoolDown[i] = keyMapMaxCoolDown[i];
				}
				else {
					res = false;
				}
			}
			
			keyMapCoolDown[i] -= 1;
			if (keyMapCoolDown[i] < 0) {
				keyMapCoolDown[i] = 0;
			}
			
			keyMapResultZipped[i] = res;
		}
		
		applyKeyAction(
			gameNetwork->isConnected,
			gem->getCurActorUID(),
			zipBits(keyMapResultZipped, KEYMAP_LENGTH),
			getCamRot(0),
			getCamRot(1)
		);
	}
void Singleton::handleMovement ()
                              {
		
		
		
		
		int i;
		
		
		btVector3 skullPos;
		
		
		
		float xrotrad = getCamRot(0);
		float yrotrad = getCamRot(1);
		
		angleToVec(&lookAtVec,xrotrad,yrotrad);
		
		lookAtVec2D.copyFrom(&lookAtVec);
		lookAtVec2D.normalizeXY();
		
		
		
		float xmod = 0.0f;
		float ymod = 0.0f;
		float zmod = 0.0f;
		
		
		float tempMoveSpeed;
		
		//unsigned char curKey;
		
		isPressingMove = false;
		
		btVector3 tempBTV;
		
		amountInvalidRotate = (abs(curCamRotation[0]-camRotation[0]) + abs(curCamRotation[1]-camRotation[1]));
		depthInvalidRotate = (amountInvalidRotate > 0.001f);
		
		if (smoothMove||(
			(!gem->firstPerson) &&
			(gem->getCurActor() != NULL)	
		)) {
			curCamRotation[0] += (camRotation[0]-curCamRotation[0])*timeDelta*8.0f;
			curCamRotation[1] += (camRotation[1]-curCamRotation[1])*timeDelta*8.0f;
		}
		else {
			curCamRotation[0] = camRotation[0];
			curCamRotation[1] = camRotation[1];
		}
		
		
		
		if (gem->getCurActor() == NULL) {
			if (keysPressed[keyMap[KEYMAP_UP]]) {
				zmod += 1.0f;
				isPressingMove = true;
			}

			if (keysPressed[keyMap[KEYMAP_DOWN]]) {
				zmod -= 1.0f;
				isPressingMove = true;
			}

			if (keysPressed[keyMap[KEYMAP_FORWARD]]) { // || mbDown

				xmod += lookAtVec[0];//float(cos(xrotrad));
				ymod += lookAtVec[1]; //float(sin(xrotrad));
				zmod += lookAtVec[2];//float(cos(yrotrad));
				
				isPressingMove = true;
			}

			if (keysPressed[keyMap[KEYMAP_BACKWARD]]) {
				
				
				xmod -= lookAtVec[0];
				ymod -= lookAtVec[1];
				zmod -= lookAtVec[2];
				
				// xmod += float(cos(xrotrad+M_PI));
				// ymod += float(sin(xrotrad+M_PI));
				// zmod += float(cos(yrotrad));
				
				isPressingMove = true;
			}

			if (keysPressed[keyMap[KEYMAP_RIGHT]]) {
				
				xmod += lookAtVec2D[1];//float(cos(xrotrad+M_PI*0.5f));
				ymod += -lookAtVec2D[0];//float(sin(xrotrad+M_PI*0.5f));
				
				isPressingMove = true;
			}

			if (keysPressed[keyMap[KEYMAP_LEFT]]) {
				
				xmod += -lookAtVec2D[1];//float(cos(xrotrad-M_PI*0.5f));
				ymod += lookAtVec2D[0];//float(sin(xrotrad-M_PI*0.5f));
				
				isPressingMove = true;
			}

			
			
			
			
			if (isPressingMove) {
				
				curMoveAccel += 0.5f*timeDelta;
				curMoveSpeed += curMoveAccel*timeDelta;
				
				// if (curMoveSpeed > 32.0*timeDelta) {
				// 	curMoveSpeed = 32.0*timeDelta;
				// }
				
				wsBufferInvalid = true;
			}
			else {
				curMoveAccel = 0.0f;
				curMoveSpeed += (0.0f-curMoveSpeed)*timeDelta;
			}
			
			
			
			
			tempMoveSpeed = curMoveSpeed;
			
			if (bShift) {
				tempMoveSpeed *= 1.0f/32.0f;
			}
			
			modXYZ.setFXYZ(
				xmod*tempMoveSpeed,
				ymod*tempMoveSpeed,
				zmod*tempMoveSpeed
			);
			
			moveCamera(&modXYZ);
			
			
			isMoving = (curMoveSpeed >= 1.0);
		}
		
		
		
		if (gem->getCurActor() != NULL) {
			if (gem->getCurActor()->entType == E_ENTTYPE_NPC) {
				skullPos = gem->getCurActor()->getBodyByBoneId(E_BONE_C_SKULL)->body->getCenterOfMassPosition();
			}
			else {
				skullPos = gem->getCurActor()->getCenterPoint(E_BDG_CENTER);
			}
			
			
			if (gem->firstPerson) {
				targetCameraPos.setBTV(
					skullPos
				);
				//targetCameraPos.addXYZ(0.0f,0.0f,2.0f);
			}
			else {
				targetCameraPos.copyFrom(&lookAtVec);
				targetCameraPos.multXYZ( -(gem->subjectDistance)*subjectZoom );
				
				targetCameraPos.addXYZRef(BTV2FIV(
					skullPos
				));
				
			}
			
			
			if (!gem->firstPerson) {
				if (
					keysPressed[keyMap[KEYMAP_FORWARD]] ||
					keysPressed[keyMap[KEYMAP_BACKWARD]]
				) {
					isPressingMove = true;
					if (!rbDown) {
						
						tempBTV = multByOtherRot(
							btVector3(0.0f,1.0f,0.0f),
							gem->getCurActor()->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis()
						);
						
						camRotation[0] += 
							getShortestAngle(
								camRotation[0],
								atan2(tempBTV.getY(), tempBTV.getX()),
								timeDelta*1.0
							);
					}
				}
			}
			
			
			
			modXYZ.copyFrom(&targetCameraPos);
			modXYZ.addXYZRef(cameraGetPosNoShake(),-1.0f);
			//modXYZ.multXYZ(0.25f);
			
			moveCamera(&modXYZ);
			
		}
		
	}
bool Singleton::anyMenuVisible ()
                              {
		bool doProc = false;
		int i;
		
		
		
		
		if ((mainGUI != NULL)) {
			if (mainGUI->isReady) {
				
				for (i = 0; i < E_FM_LENGTH; i++) {
					if (menuList[i] != NULL) {
						if (menuList[i]->visible){
							doProc = true;
						}
					}
				}
				
				
			}
		}
		
		return doProc;
		
	}
void Singleton::performCamShake (BaseObj * ge, float fp)
                                                    {
		float lastCamShake = cameraShake;
		
		cameraShake = max(
			cameraShake,
			fp
		);
		
		if (cameraShake > lastCamShake) {
			
			
			shakeTimer.stop();
			shakeTimer.start();
		}
		
		
	}
void Singleton::explodeBullet (BaseObj * ge)
                                        {
		
		FIVector4 newPos;
		
		
		
		newPos.setBTV(ge->getCenterPoint(E_BDG_CENTER));
		newPos.addXYZ(0.0f,0.0f,-2.0f);
		
		if (waterBulletOn) {
			playSoundEnt(
				"bubble0",
				ge,
				0.2f
			);
		}
		else {
			playSoundEnt(
				"explosion0",
				ge,
				0.2f,
				4.0
			);
			
			performCamShake(ge,1.0f);
		}
		
		sphereStack.push_back(SphereStruct());
		sphereStack.back().position.setBTV(ge->getCenterPoint(E_BDG_CENTER));
		sphereStack.back().curRad = 1.0f;
		sphereStack.back().maxRad = explodeRad;
		sphereStack.back().radVel = 40.0f;
		sphereStack.back().radAcc = -5.0f;
		
		
		//gameFluid[E_FID_SML]->pushExplodeBullet(true,&newPos,boolToInt(waterBulletOn));
		gameFluid[E_FID_BIG]->pushExplodeBullet(true,&newPos,boolToInt(waterBulletOn),explodeRad);
		
		explodeStack.push_back(ExplodeStruct());
		
		if (ge->hasBodies()) {
			explodeStack.back().pos = ge->bodies[E_BDG_CENTER].body->getCenterOfMassPosition();
			explodeStack.back().radius = 20.0f;
			explodeStack.back().power = 200.0f;
		}
		
		
		gem->removeVisObject(ge->uid, true);
		
	}
void Singleton::getJVNodeByString (JSONValue * rootNode, JSONValue * * resultNode, string stringToSplit)
          {
		//if (TEMP_DEBUG) cout << "getJVNodeByString(" << stringToSplit <<  ")\n";
		
		int i;
		*resultNode = rootNode;
		
		splitStrings.clear();
		splitStrings = split(stringToSplit, '.');
		
		for (i = 0; i < splitStrings.size(); i++) {
			//if (dd) cout << splitStrings[i] << "\n";
			
			if (
				(splitStrings[i][0] >= '0')	&&
				(splitStrings[i][0] <= '9')
			) {
				*resultNode = (*resultNode)->Child(
					stoi(splitStrings[i])
				);
			}
			else {
				if ( (*resultNode)->HasChild(splitStrings[i]) ) {
					*resultNode = (*resultNode)->Child(splitStrings[i]);
				}
				else {
					cout << "NULL RESULT NODE\n";
					*resultNode = NULL;
					return;
				}
			}
			
			
			
		}
		
	}
string Singleton::makePretty (string sourceString, string remString)
                                                                 {
		string newString = sourceString.substr(remString.size());
		
		
		std::string::size_type i;
		
		for (i = 0; i < newString.length(); i++) {
			if (i == 0) {
				
			}
			else {
				if (newString[i-1] == '_') {
					newString[i] = toupper(newString[i]);
				}
				else {
					newString[i] = tolower(newString[i]);
				}
			}
			
		}
		
		for (i = 0; i < newString.length(); i++) {
			if (newString[i] == '_') {
				newString[i] = ' ';
			}
		}
		
		return newString;
		
	}
void Singleton::cleanJVPointer (JSONValue * * jv)
                                            {
		
		if (*jv != NULL) {
			delete *jv;
		}
		
		*jv = NULL;
		
	}
void Singleton::getSpecialData (int datEnum, string datString)
                                                           {
		int i;
		int objectType;
		int childId;
		
		JSONValue* tempVal0;
		JSONValue* tempVal1;
		BaseObj* curCont;
		
		
		cleanJVPointer(&(externalJSON[datString].jv));
				
		externalJSON[datString].jv = new JSONValue(JSONObject());
		
		
		
		StatSheet* curSS;
		
		
		switch (datEnum) {
			case E_SDT_OBJECTDATA:
				externalJSON[datString].jv->object_value["objects"] = new JSONValue(JSONArray());
				tempVal0 = externalJSON[datString].jv->object_value["objects"];
				
				for (itBaseObj iterator = gem->gameObjects.begin(); iterator != gem->gameObjects.end(); iterator++) {
					// iterator->first = key
					// iterator->second = value
					
					curCont = &(gem->gameObjects[iterator->first]);
					
					if (curCont->isOpen) {
						
						tempVal0->array_value.push_back( new JSONValue(JSONObject()) );
						
						tempVal0->array_value.back()->object_value["children"] = new JSONValue(JSONArray());
						tempVal0->array_value.back()->object_value["objectId"] = new JSONValue( ((double)(iterator->first)) );
						tempVal1 = tempVal0->array_value.back()->Child("children");
						
						
						for (i = 0; i < curCont->children.size(); i++) {
							tempVal1->array_value.push_back(new JSONValue(JSONObject()));
							childId = curCont->children[i];
							objectType = gem->gameObjects[childId].objectType;
							tempVal1->array_value.back()->object_value["objectType"] = new JSONValue( ((double)(objectType)) );
							tempVal1->array_value.back()->object_value["objectId"] = new JSONValue( ((double)(childId)) );
						}
					}
				}
			break;
			case E_SDT_STATDATA:
				
				externalJSON[datString].jv->object_value["stats"] = new JSONValue(JSONArray());
				tempVal0 = externalJSON[datString].jv->object_value["stats"];
				
				if (gem->getCurActor() == NULL) {
					cout << "NULL STATS\n";
					return;
				}
				else {
					
					curSS = &(gem->getCurActor()->statSheet);
					
					for (i = 0; i < E_CS_LENGTH; i++) {
						tempVal0->array_value.push_back( new JSONValue(JSONObject()) );
						
						tempVal0->array_value.back()->object_value["label"] = new JSONValue( makePretty(E_CHAR_STAT_STRINGS[i],"E_CS_") );
						tempVal0->array_value.back()->object_value["value"] = new JSONValue( ((double)(curSS->unapplyedStats[i]))/((double)(MAX_STAT_VALUE)) );
						tempVal0->array_value.back()->object_value["divisions"] = new JSONValue( ((double)(MAX_STAT_VALUE)) );
						
						
					}
				}
			break;
			case E_SDT_STATUSDATA:
				
				externalJSON[datString].jv->object_value["status"] = new JSONValue(JSONArray());
				tempVal0 = externalJSON[datString].jv->object_value["status"];
				
				if (gem->getCurActor() == NULL) {
					cout << "NULL STATUS\n";
					return;
				}
				else {
					
					curSS = &(gem->getCurActor()->statSheet);
					
					for (i = 0; i < E_STATUS_LENGTH; i++) {
						tempVal0->array_value.push_back( new JSONValue(JSONObject()) );
						
						tempVal0->array_value.back()->object_value["label"] = new JSONValue( makePretty(E_CHAR_STATUS_STRINGS[i],"E_STATUS_") );
						tempVal0->array_value.back()->object_value["value"] = new JSONValue( ((double)(curSS->curStatus[i]))/((double)(curSS->maxStatus[i])) );
						tempVal0->array_value.back()->object_value["divisions"] = new JSONValue( ((double)(curSS->maxStatus[i])) );
						
						
					}
				}
			break;
			default:
				cout << "ERROR: unexpected type in getSpecialData(): " << datEnum << "\n";
			break;
		}
		
		
		
		
		
	}
void Singleton::updateStatGUI ()
                             {
		UIComponent* tempComp;
		
		
		if (gem->getCurActor() == NULL) {
			return;
		}
		
		StatSheet* curSS = &(gem->getCurActor()->statSheet);
		
		tempComp = getGUIComp("statMenu.availPoints");
		tempComp->setValue(
			((float)curSS->availPoints)/((float)(tempComp->divisions))
		);
		
		
		
	}
void Singleton::updateStatusHUD ()
                               {
		
		
		int i;
		
		if (gem->getCurActor() == NULL) {
			return;
		}
		if (menuList[E_FM_HUDMENU] == NULL) {
			return;
		}
		if (menuList[E_FM_HUDMENU]->visible) {
			
		}
		else {
			return;
		}
		
		StatSheet* curStatSheet = &(gem->getCurActor()->statSheet);
		
		UIComponent* tempComp = getGUIComp("hudMenu.statContainer");
		UIComponent* childComp;
		
		if (tempComp == NULL) {
			return;
		}
		
		float v1;
		float v2;
		
		for (i = 0; i < E_STATUS_LENGTH; i++) {
			childComp = tempComp->getChild(i);
			
			v1 = curStatSheet->curStatus[i];
			v2 = curStatSheet->maxStatus[i];
			
			childComp->setValue(v1/v2);
		}
		
		
	}
void Singleton::showHudMenu (bool visible)
                                       {
		if (menuList[E_FM_HUDMENU] != NULL) {
			menuList[E_FM_HUDMENU]->visible = visible;
			
			externalJSON.erase("E_SDT_STATUSDATA"); // mem leak?
			
			mainGUI->refreshNode(
				mainGUI->findNodeByString("hudMenu.hudContainer")	
			);
						
			if (visible) {
				
				updateStatGUI();
				
			}
			
			
		}
	}
void Singleton::showStatMenu (bool visible)
                                        {
		
		
		cout << "refreshStats\n";
		
		if (menuList[E_FM_STATMENU] != NULL) {
			menuList[E_FM_STATMENU]->visible = visible;
			externalJSON.erase("E_SDT_STATDATA"); // mem leak?
			externalJSON.erase("E_SDT_STATUSDATA"); // mem leak?
			
			mainGUI->refreshNode(
				mainGUI->findNodeByString("statMenu.statContainer")	
			);
						
			if (visible) {
				
				updateStatGUI();
				
			}
			
			
		}
		
	}
void Singleton::refreshContainers (bool onMousePos)
                                                {
		UIComponent* objCont = NULL;
		
		bool oldVis = false;
		
		if (menuList[E_FM_CONTMENU] != NULL) {
			
			cout << "refreshContainers\n";
			
			externalJSON.erase("E_SDT_OBJECTDATA"); // mem leak?
			
			
			
			oldVis = menuList[E_FM_CONTMENU]->visible;
			menuList[E_FM_CONTMENU]->visible = gem->anyContainerOpen();

			objCont = mainGUI->findNodeByString("objectContainer");
			//objCont->jvNodeNoTemplate->Child("dataParams")->number_value = contIndex;
			
			mainGUI->refreshNode(objCont);
			
			if (onMousePos&&(oldVis == false)) {
				
				// menuList[E_FM_CONTMENU]->dragOffset.x = 0.0f;
				// menuList[E_FM_CONTMENU]->dragOffset.y = 0.0f;
				contMenuBar = menuList[E_FM_CONTMENU]->getChild(0)->getChild(0);
				
				contMenuBar->lastDrag.x = (guiX);
				contMenuBar->lastDrag.y = min((float)(guiY), (float)( (guiWinH - menuList[E_FM_CONTMENU]->getChild(0)->resultDimInPixels.y) ));
				contMenuBar->forceDragUpdate = true;
			}
			
		}
	}
JSONValue * Singleton::fetchJSONData (string dataFile, bool doClean, JSONValue * params)
                                                                                          {
		
		
		bool doLoad = false;
		bool loadRes = false;
		int dataFileEnum;
		
		if (externalJSON.find( dataFile ) == externalJSON.end()) {
			doLoad = true;
		}
		else {
			if (externalJSON[dataFile].jv == NULL) {
				doLoad = true;
			}
			else {
				if (doClean) {
					cleanJVPointer( &(externalJSON[dataFile].jv) );
					doLoad = true;
				}
			}
		}
			
		if (doLoad) {
			
			dataFileEnum = stringToEnum(
				E_SPECIAL_DATA_TYPE_STRINGS,
				E_SDT_LENGTH,
				dataFile
			);
			
			switch (dataFileEnum) {
				case E_SDT_SHADERPARAMS:
					cout << "attempted to load shaderParams\n";
				break;
				case -1:
					cout << "load jv data "  + dataFile << "\n";
					loadRes = loadJSON(
						"..\\data\\" + dataFile,
						&((externalJSON[dataFile]).jv)
					);
					
					if (loadRes == false) {
						return NULL;	
					}
				break;
				default:
					getSpecialData(dataFileEnum, dataFile);
				break;
			}
			
			
			
			
		}
		
		
		
		return (externalJSON[dataFile]).jv;
	}
bool Singleton::processJSONFromString (string * sourceBuffer, JSONValue * * destObj)
          {
		cleanJVPointer(destObj);
		
		*destObj = JSON::Parse(sourceBuffer->c_str());

		if (*destObj == NULL)
		{
			doTraceND("Invalid JSON\n\n");
			doAlert();
			//cout << sourceBuffer << "\n\n";
			return false;
		}
		else
		{
			doTraceND("\nValid JSON\n");
			return true;
		}
		
	}
void Singleton::specialReplace (string & allTextString, string preDelim, string pstDelim)
          {
		
		string paramName;
		
		jsonPostStack.clear();
		
		std::size_t found;
		std::size_t found2;
		std::size_t found3;
		
		int baseIndex = 0;
		bool doCont = true;
		
		int preLen = preDelim.size();
		int pstLen = pstDelim.size();
		
		while (doCont) {
			found = allTextString.find(preDelim, baseIndex);
			if (found != std::string::npos) {
				
				
				baseIndex = found+preLen;
				//allTextString[found] = ' ';
				
				found3 = allTextString.find(' ', baseIndex);
				found2 = allTextString.find(pstDelim, baseIndex);
				
				if (found2 != std::string::npos) {
					
					if ( 
						((found2-found) > 32) || // max var length of 32
						(found3 < found2) // found a space between the delimitters
					) { 
						
					}
					else {
						baseIndex = found2+pstLen;
						//allTextString[found2] = ' ';
						
						paramName = allTextString.substr(found + preLen, (found2-found) - pstLen);
						
						jsonPostStack.push_back(paramName);
						
						cout << "PARAM NAME " << paramName << "\n";
						
					}
					
					doCont = true;
				}
				else {
					doCont = false;
				}
				
				
			}
			else {
				doCont = false;
			}
		}
		
	}
void Singleton::jsonPostProc ()
                            {
		
		std::size_t found = jsonPostString.find("@@", 0);
		if (found == std::string::npos) {
			return;	
		}
		
		string preStr = "\"@@";
		string pstStr = "@@\"";
		
		cout << "jsonPostProc\n";
		
		specialReplace(jsonPostString,preStr,pstStr);
		
		int i;
		int enumVal;
		bool doProc;
		
		string newString = "";
		
		for (i = 0; i < jsonPostStack.size(); i++) {
			enumVal = stringToEnum(E_JSON_PARAM_STRINGS,E_JP_LENGTH,jsonPostStack[i]);
			
			doProc = true;
			
			switch(enumVal) {
				case E_JP_STATMENUAVAILDIV:
					newString = i__s(E_CS_LENGTH*MAX_STAT_VALUE);
				break;
				default:
					doProc = false;
					cout << "invalid JSON Post Process Enum " << jsonPostStack[i] << "\n";
				break;
			}
			
			if (doProc) {
				replaceStr(jsonPostString, preStr+jsonPostStack[i]+pstStr, newString);
			}
			
			
			
			
		}
		
		jsonPostStack.clear();
		
	}
bool Singleton::processJSON (charArr * sourceBuffer, charArr * saveBuffer, JSONValue * * destObj)
          {
		
		cleanJVPointer(destObj);

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
			
			jsonPostString = string(buf);
			jsonPostProc();
			
			
			*destObj = JSON::Parse(jsonPostString.c_str());
		}


		//doTraceND("End JSON::Parse()");


		if (*destObj == NULL)
		{
			doTraceND("Invalid JSON\n\n");
			doAlert();
			return false;
		}
		else
		{
			doTraceND("\nValid JSON\n");
			
			
			
			return true;
		}


	}
void Singleton::doAlert ()
                       {
		playSound("xylo0", 1.0f);
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
			
			if (curComp->nodeId < 0) {
				// component was deleted
			}
			else {
				if (applyString) {
					compStack[curComp->nodeId].data->setText(stringValue);
				}
				if (applyVal) {
					compStack[curComp->nodeId].data->setValue(floatValue);
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
			
			if (curComp->nodeId < 0) {
				// component was deleted
			}
			else {
				return compStack[curComp->nodeId].data->getValue();
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
			
			if (curComp->nodeId < 0) {
				// component was deleted
			}
			else {
				return compStack[curComp->nodeId].data;
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
			
			if (curComp->nodeId < 0) {
				// component was deleted
			}
			else {
				compStack[curComp->nodeId].data->setValue(floatValue,dispatchEvent,preventRefresh);
			}
		}
	}
void Singleton::loadValuesGUI (bool applyValues)
                                                     {
		
		cout << "Loading GUI Values\n";
		
		int i;
		
		charArr dest;
		dest.data = NULL;
		dest.size = 0;
		
		UICStruct* curComp;
		
		string loadBuf;
		//vector<string> splitStrings;
		
		
		if ( loadFile(guiSaveLoc, &dest) )
		{
			loadBuf = string(dest.data);
			
			splitStrings.clear();
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
void Singleton::saveExternalJSON ()
                                {
		
		cout << "Saving External JSON Values\n";
		
		
		
		
		for(itJSStruct iterator = externalJSON.begin(); iterator != externalJSON.end(); iterator++) {
			
			if (iterator->second.jv != NULL) {
				saveFileString(
					"..\\data\\" + iterator->first,
					&(iterator->second.jv->Stringify())
				);
			}
			
			// iterator->first = key
			// iterator->second = value
		}
		
		
		
		cout << "End Saving External JSON Values\n";
	}
void Singleton::saveGUIValues ()
                             {
		
		cout << "Saving GUI Values\n";
		
		stringBuf = "";
		
		for(itUICStruct iterator = compMap.begin(); iterator != compMap.end(); iterator++) {
				
				if (iterator->second.nodeId < 0) {
					
				}
				else {
					if (iterator->first[0] ==  '$') { // values with $ are saved
						stringBuf.append(
							iterator->first + "^" + floatToHex(compStack[iterator->second.nodeId].data->getValue()) + "^"
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
		
		
		
		int mvPerPage = 1;
		
		float voxelsGen = PAGE_COUNT*mvPerPage;
		
		string maxGPUMString = " / " + fi__s(MAX_GPU_MEM);
		
		// if (frameCount%120 == 0) {
			
		// 	chunkMem = 0;
		// 	fbMem = TOT_GPU_MEM_USAGE - chunkMem;
			
			
		// 	setGUIText("debug.fbMem", "Frame Buffer Mem Used: " + fi__s(fbMem) + maxGPUMString, fbMem/MAX_GPU_MEM, true );
		// 	setGUIText("debug.chunkMem", "Chunk Mem Used: " + fi__s(chunkMem) + maxGPUMString, chunkMem/MAX_GPU_MEM, true );
		// 	setGUIText("debug.totMem", "Total Mem Used: " + fi__s(TOT_GPU_MEM_USAGE) + maxGPUMString, TOT_GPU_MEM_USAGE/MAX_GPU_MEM, true );
		// 	setGUIText("debug.chunksGen", "Voxels Generated (In Millions!): " + fi__s(voxelsGen) );
			 
		// }
		
	}
void Singleton::beginFieldInput (string defString, int cb)
                                                       {
		currentFieldString = defString;
		
		fieldCallback = cb;
		
		inputOn = true;
		menuList[E_FM_FIELDMENU]->visible = true;
		
		if (fieldText != NULL) {
			if (currentFieldString.compare("") == 0) {
				fieldText->setText(" ");
			}
			else {
				fieldText->setText(currentFieldString);
			}
			
		}
	}
void Singleton::processFieldInput (unsigned char key)
                                                  {
		
		bool doRef = false;
		
		switch (key) {
			case 13: // enter
				endFieldInput(true);
			break;
			case 27: // esc
				endFieldInput(false);
			break;
			case 8: // backspace
				currentFieldString = currentFieldString.substr(0, currentFieldString.size()-1);
				doRef = true;
			break;
			default:
				currentFieldString += key;
				doRef = true;
			break;
		}
		
		if (doRef) {
			if (fieldText != NULL) {
				fieldText->setText(currentFieldString);
			}
		}
	}
void Singleton::endFieldInput (bool success)
                                         {
		
		inputOn = false;
		menuList[E_FM_FIELDMENU]->visible = false;
		
		if (success) {
			switch (fieldCallback) {
				case E_FC_SAVEORG:
					gem->saveOrgFromMenu(currentFieldString);
				break;
				case E_FC_LOADORG:
					gem->loadOrgFromMenu(currentFieldString);					
				break;
				
			}
		}
		
	}
void Singleton::saveOrg ()
                       {
		beginFieldInput("",E_FC_SAVEORG);
	}
void Singleton::loadOrg ()
                       {
		beginFieldInput("",E_FC_LOADORG);
	}
float Singleton::getConst (string conName)
                                       {
		if (constRootJS == NULL) {
			
		}
		else {
			if (constRootJS->HasChild(conName)) {
				return constRootJS->Child(conName)->number_value;
			}
			else {
				doAlert();
				cout << "Missing: " << conName << "\n";
			}
		}
		return 0.0f;
	}
int Singleton::iGetConst (int ev)
                              {
		return conVals[ev];
	}
void Singleton::loadConstants ()
                             {
		int i;
		if (loadJSON("..\\data\\constants.js", &constRootJS)) {
			
			for (i = 0; i < E_CONST_LENGTH; i++) {
				conVals[i] = getConst(E_CONST_STRINGS[i]);
			}
			
		}
		
		mapAmps.setFXYZW(
			conVals[E_CONST_MAPAMP0],
			conVals[E_CONST_MAPAMP1],
			conVals[E_CONST_MAPAMP2],
			conVals[E_CONST_MAPAMP3]
		);
		mapFreqs.setFXYZW(
			conVals[E_CONST_MAPFREQ0],
			conVals[E_CONST_MAPFREQ1],
			conVals[E_CONST_MAPFREQ2],
			conVals[E_CONST_MAPFREQ3]
		);
		
		STEP_TIME_IN_SEC = conVals[E_CONST_STEP_TIME_IN_MICRO_SEC]/1000000.0;
		
		
	}
void Singleton::loadGUI ()
                       {
		int i;
		
		externalJSON.clear();
		doShaderRefresh(bakeParamsOn);
		
		for(itUICStruct iterator = compMap.begin(); iterator != compMap.end(); iterator++) {
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
		
			// for(itUICStruct iterator = compMap.begin(); iterator != compMap.end(); iterator++) {
			// 	if (iterator->second.nodeId != -1) {
			// 		//iterator->second.uic = (compStack[iterator->second.nodeId].data);
			// 	}
			// }
		
		}
		
		for (i = 0; i < E_FM_LENGTH; i++) {
			menuList[i] = getGUIComp("guiHandles." + E_FLOATING_MENU_STRINGS[i]);
			if (menuList[i] != NULL) {
				menuList[i]->visible = false;
			}
		}
		
		
		mapComp = getGUIComp("map.mapHolder");
		fieldText = getGUIComp("fieldMenu.field");
		
		
		updateMatFlag = true;
		
		
	}
string Singleton::loadFileString (string fnString)
                                               {
		// charArr tempFile;
		// tempFile.data = NULL;
		// tempFile.size = 0;
		// string tempFileString;
		
		
		// if ( loadFile(fnString,&tempFile) ) {
		// 	tempFileString = string(tempFile.data);
			
		// 	if (tempFile.data != NULL)
		// 	{
		// 		delete[] tempFile.data;
		// 		tempFile.data = NULL;
		// 		tempFile.size = 0;
		// 	}
			
		// 	cout << "::::::::::: \n\n\n\n";
			
		// 	cout << tempFileString << "\n";
			
		// 	cout << "::::::::::: \n\n\n\n";
			
		// 	return tempFileString;
			
		// }
		// else {
		// 	return "";
		// }
		
		
		std::ifstream t(fnString);
		std::stringstream buffer;
		buffer << t.rdbuf();
		
		return buffer.str();
		
	}
std::ifstream::pos_type Singleton::filesize (char const * filename)
        {
	    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	    return in.tellg(); 
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

		// // get size of file
		// infile.seekg (0, infile.end);
		// long size = (long)infile.tellg();
		// infile.seekg (0, infile.beg);

		long mySize = filesize(fnString.c_str());

		dest->size = mySize;

		if (dest->data != NULL)
		{
			delete[] dest->data;
			dest->data = NULL;
		}

		dest->data = new char[mySize+8];

		// read content of infile
		infile.read (dest->data, mySize);

		if ( infile.bad() )
		{
			doTraceND("Could Not Load From File");
			infile.close();
			return false;
		}

		infile.close();
		
		dest->data[mySize] = '\0';

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
float Singleton::getUnderWater ()
                              {
		if (
			(gw->getCellAtCoords(
				cameraGetPosNoShake()->getFX(),
				cameraGetPosNoShake()->getFY(),
				cameraGetPosNoShake()->getFZ() - 1.0f
			) == E_CD_WATER) ||
			(cameraGetPosNoShake()->getFZ() < (getSeaHeightScaled()-32.0f))	
		) {
			return 1.0;
		}
		else {
			return 0.0;
		}
		
	}
void Singleton::updateAmbientSounds ()
                                   {
		
		int i;
		int j;
		
		int maxRad = 2;
		
		float avgHeight = 0.0f;
		float tot = 0.0f;
		
		for (i = -maxRad; i <= maxRad; i++) {
			for (j = -maxRad; j <= maxRad; j++) {
				avgHeight += getHeightAtPixelPos(
					cameraGetPosNoShake()->getFX() + i*256.0f,
					cameraGetPosNoShake()->getFY() + j*256.0f
				);
				tot += 1.0f;
			}
		}
		
		float terHeight = avgHeight/tot;
		
		float seaHeight = getSeaHeightScaled();
		
		float heightDif = clampf((terHeight-seaHeight)/(1024.0f), 0.0f, 1.0f);
		
		float isUnderWater = getUnderWater();
		
		// if (cameraGetPosNoShake()->getFZ() < seaHeight) {
		// 	isUnderWater = 1.0f;	
		// }
		
		
		
		music[EML_BIRDSONG0]->setVolume(masterVolume*ambientVolume*timeOfDay*heightDif*(1.0-isUnderWater));
		music[EML_CRICKETS0]->setVolume(masterVolume*ambientVolume*(1.0f-timeOfDay)*heightDif*(1.0-isUnderWater));
		music[EML_OCEANWAVES0]->setVolume(masterVolume*ambientVolume*(1.0f-heightDif)*(1.0-isUnderWater));
		music[EML_UNDERWATER0]->setVolume(masterVolume*ambientVolume*(isUnderWater));
		
		
	}
void Singleton::frameUpdate ()
                           {
		
		float temp;
		float temp2;
		
		//int currentTickMod = 0;
		
		if (firstRun) {
			
		}
		else {
			//curMoveTime = moveTimer.getElapsedTimeInMicroSec();
			
			// if (lastMoveTime == 0.0) {
			// 	//timeDelta = 0.0f;
			// }
			// else {
				
			// 	if (ignoreFrameLimit) {
			// 		timeDelta = 
			// 			timeDelta = bulletTimer.getTimeMicroseconds()/1000000.0;//*0.999 + ((curMoveTime-lastMoveTime)/1000000.0)*0.001;//TIME_DELTA;
			// 			bulletTimer.reset();
			// 			//1.0/45.0;
			// 	}
			// 	else {
			// 		timeDelta = 1.0/120.0;
			// 	}
				
				
				
				
			// 	// if (smoothMove) {
			// 	// 	timeDelta = 1.0f/90.0f;
			// 	// }
			// 	// else {
			// 	// 	timeDelta = 1.0f/90.0f;
			// 	// }
				
			// 	//60.0f;//(curMoveTime-lastMoveTime)/1000000.0;
			// }
			
			//lastMoveTime = curMoveTime;
			
			
			
			
			// #######
			
			
			////////////////////////////////
			
			
			// WAS DOING: PASS KEY STATE (BITFLAG) TO NETWORK FOR EACH TICK AND MOUSE STATE
			
			
			////////////////////////////////
			
			
			
			// #######
			
			
			
			
		}
		
		
		
		if (updateMatFlag&&(!matVolLock)) {
			updateMatFlag = false;
			updateMatVol();
		}
		
		
		if (ignoreFrameLimit) {
			subjectZoom += (targetSubjectZoom - subjectZoom) / 32.0f;
			cameraZoom += (targetZoom - cameraZoom) / 4.0f;
		}
		
		
		
		if (!placingGeom) {
			if (abs(globWheelDelta) > 0.001f) {
				if (gem->getCurActor() != NULL) {
					subjectDelta -= globWheelDelta;
					targetSubjectZoom = pow(2.0, subjectDelta);
					if (!ignoreFrameLimit) {
						subjectZoom = targetSubjectZoom;
					}
				}
			}
		}

		globWheelDelta *= 0.5f;
		
		
		
		if (cameraZoom < 1.0f) {
			cameraZoom = 1.0f;
		}
		
		float hs = cellsPerHolder;
		
		float scrollAmount = 0.0f;
		
		updateAmbientSounds();
		if ((mainGUI != NULL)) {
			if (mainGUI->isReady) {
				//mainGUI->testOver(guiX, guiY);
			}
		}
		syncObjects();
		updateGUI();
		
		
		if (
			(gw->mapLockOn) ||
			(mapInvalid)
		) {
			
		}
		else {
			
			
			
			if (mainGUI != NULL) {
				if (mainGUI->isReady) {
					currentTick++;
					
					
					
					if (gem->getCurActor() != NULL) {
						if ((currentTick%10) == 0) {
							
							// if (rbDown) {
							// 	makeShoot(gem->getCurActor()->uid, E_ENTTYPE_TRACE);
							// }
							
							
						}
					}
					
					if (currentTick > 2) {
						
						
						updateCamVals();
						
						
						
						if (currentTick < 4) {
							cameraGetPosNoShake()->setFXYZ(2048.0,2048.0,0.0);
							camLerpPos.copyFrom(cameraGetPosNoShake());
						}
						
						if (currentTick == 4) {
							setCameraToElevation();
							
							gamePhysics = new GamePhysics();
							gamePhysics->init(this);
							
							GLint maxTBO;
							glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxTBO);
							cout << "GL_MAX_TEXTURE_BUFFER_SIZE " << maxTBO << "\n";
							
							
							
						}
						
						if (
							(gem->getCurActor() != NULL) &&
							gem->getCurActor()->hasBodies()	
						) {
							
							if (gem->getCurActor()->bodies[E_BDG_CENTER].inWater) {
								temp = clampfZO(
									gem->getCurActor()->getVel(0)->length()
								)*0.25f;
								temp2 = 0.0f;
							}
							else {
								
								if (gem->getCurActor()->allFalling()) {
									temp2 = 0.0f;
								}
								else {
									temp2 = clampfZO(
										gem->getCurActor()->getVel(0)->length()
									);
								}
								
								temp = 0.0f;
							}
							
							
							
							updateSoundPosAndPitch(
								"swimming0",
								cameraGetPosNoShake(),
								BTV2FIV(gem->getCurActor()->getCenterPoint(E_BDG_CENTER)),
								temp*0.2,
								0.01
							);
							updateSoundPosAndPitch(
								"walkinggravel0",
								cameraGetPosNoShake(),
								BTV2FIV(gem->getCurActor()->getCenterPoint(E_BDG_CENTER)),
								temp2*0.2,
								0.1
							);
						}
						
						
						// if (
						// 	gameFluid[E_FID_SML]->fluidReading ||
						// 	gameFluid[E_FID_BIG]->fluidReading
						// ) {
						// 	if (gameFluid[E_FID_BIG]->fluidReading) {
						// 		if (gameFluid[E_FID_SML]->cycleTerminated) {
						// 			gameFluid[E_FID_BIG]->updateAll();
						// 		}
						// 		else {
						// 			// wait for small fluid to finish its cycle
						// 			gameFluid[E_FID_SML]->updateAll();
						// 		}
						// 	}
						// 	else {
						// 		if (gameFluid[E_FID_BIG]->cycleTerminated) {
						// 			gameFluid[E_FID_SML]->updateAll();
						// 		}
						// 		else {
						// 			// wait for big fluid to finish its cycle
						// 			gameFluid[E_FID_BIG]->updateAll();
						// 		}
						// 	}
						// }
						// else {
							
							
						// 	gameFluid[E_FID_SML]->updateAll();
						// 	gameFluid[E_FID_BIG]->updateAll();
							
						// 	if (
						// 		gameFluid[E_FID_SML]->fluidReading ||
						// 		gameFluid[E_FID_BIG]->fluidReading
						// 	) {
								
						// 	}
						// 	else {
								
						// 		if (gameFluid[E_FID_SML]->cycleTerminated) {
						// 			gameFluid[E_FID_SML]->cycleTerminated = false;
									
						// 			//tempVec1.copyFrom(&(gameFluid[E_FID_SML]->volMinReadyInPixels));
						// 			//tempVec1.addXYZRef(&(gameFluid[E_FID_SML]->volMinReadyInPixels),-1.0f);
									
						// 			// gameFluid[E_FID_BIG]->copyPrimTexture(
						// 			// 	tempVec1[0],
						// 			// 	tempVec1[1],
						// 			// 	tempVec1[2],
						// 			// 	gameFluid[E_FID_SML]->volSizePrim,
						// 			// 	&(gameFluid[E_FID_SML]->volDataPrim[0])
						// 			// );
									
						// 			gameFluid[E_FID_SML]->startFT();
						// 		}
								
								
						// 		if (gameFluid[E_FID_BIG]->cycleTerminated) {
						// 			gameFluid[E_FID_BIG]->cycleTerminated = false;
						// 			gameFluid[E_FID_BIG]->startFT();
						// 		}
						// 	}
						// }
						
						if ((!draggingMap)&&(!fpsTest)&&updateHolders) {
							gameFluid[E_FID_BIG]->updateAll();
							
							if (gameFluid[E_FID_BIG]->fluidReading) {
								if (gameFluid[E_FID_BIG]->proceedingToRead) {
									gameFluid[E_FID_BIG]->tryToEndRead();
								}
								else {
									gameFluid[E_FID_BIG]->proceedWithRead();
								}
							}
							
							
							if (gameFluid[E_FID_BIG]->readyForTermination) {
								
								
								if (
								 gameFluid[E_FID_BIG]->anyThreadsRunning()
								) {
									
								}
								else {
									gameFluid[E_FID_BIG]->readyForTermination = false;
									gameFluid[E_FID_BIG]->cycleTerminated = true;
								}
								
								
							}
							
							if (gameFluid[E_FID_BIG]->cycleTerminated) {
								
								gameLogic->loadNearestHolders();
								
								gameFluid[E_FID_BIG]->cycleTerminated = false;
								gameFluid[E_FID_BIG]->startFT();
							}
						}
						
						
					}
					
					
					
					if ((currentTick > 4)&&allInit) {
						
						
						//gw->drawPrim();
						
						if (renderingOct) {
							//gw->renderOct(gameOct);
							//gw->rasterOct(gameOct,true);
							
							// if ((bakeTicks % iGetConst(E_CONST_BAKE_TICKS)) == 0) {
							// 	gw->update(false);
							// }
							// gw->rasterGrid(&myVBOGrid,true);
							// bakeTicks++;
							gw->update(false,false);
							gw->rasterHolders(true,true);
							
						}
						else {
							//gw->rasterOct(gameOct,false);
							//gw->rasterGrid(&myVBOGrid,false);
							gw->update(true,true);
						}
						
						
						// if (GEN_POLYS_WORLD) {
						// 	gw->generateBlockHolder();
						// }
						
						if (gem->turnBased) {
							if (
								((tbTicks%iGetConst(E_CONST_TURNBASED_TICKS)) == 0)
								// || (gem->getCurActor() != NULL)
							) {
								gem->cycleTurn();
							}
							tbTicks++;
						}
						
					}
					
					
					
				}
			}
			
		}
		
		
		
		
		
		
		
		
		
		
		TRACE_ON = false;
		
		
		frameCount++;
	}
FIVector4 * Singleton::cameraGetPos ()
                                  {
		return &resultCameraPos;
	}
FIVector4 * Singleton::cameraGetPosNoShake ()
                                         {
		return cameraPos;
	}
float Singleton::getTargetTimeOfDay ()
                                   {
		return 1.0f-(lightVecOrig.getFZ() + 1.0f)*0.5f;
	}
void Singleton::updateBullets ()
                             {
		int i;
		SphereStruct* ss;
		
		for (i = 0; i < sphereStack.size(); i++) {
			ss = &(sphereStack[i]);
			
			ss->radVel += ss->radAcc*timeDelta;
			ss->curRad += ss->radVel*timeDelta;
			
			if (ss->curRad >= ss->maxRad) {
				ss->curRad = ss->maxRad;
				ss->radVel = 0.0f;
			}
		}
		
		for (i = 0; i < sphereStack.size(); i++) {
			ss = &(sphereStack[i]);
			if (ss->curRad <= 0.0) {
				sphereStack.erase(sphereStack.begin() + i);
			}
		}
	}
void Singleton::display (bool doFrameRender)
        {
		
		bool noTravel = false;
		
		float maxPhysTime = 
			conVals[E_CONST_STEP_TIME_IN_MICRO_SEC]*
			conVals[E_CONST_PHYS_STEPS_PER_FRAME];
		
		
		frameSkipCount++;
		
		
		curTime = myTimer.getElapsedTimeInMilliSec();
		smoothTime = (sin(curTime/200.0)+1.0f)*0.5f;
		
		if (timeMod) {
			pauseTime = curTime;
		}
		

		//float elTime = curTime - lastTime;
		
		// #ifdef USE_POCO
		// 	if (myWS == NULL)
		// 	{

		// 	}
		// 	else
		// 	{

		// 		if (myWS->dataReady)
		// 		{

		// 			if (myWS->isJSON)
		// 			{
		// 				// if ( processJSON( &(myWS->recBuffer), &lastJSONBuffer, &rootObjJS ) )
		// 				// {
		// 				// 	saveAllData();

		// 				// }
		// 			}
		// 			else
		// 			{
		// 				//processB64(  &(myWS->recBuffer), &lastImageBuffer );

		// 			}

		// 			myWS->dataReady = false;
		// 			myWS->isWorking = false;
		// 		}
		// 	}
		// #endif

		
		int i;
		
		for (i = 0; i < MAX_KEYS; i++) {
			if (keysPressed[i]) {
				break;
			}
		}
		
		if (i != MAX_KEYS) {
			frameMouseMove = true;
		}
		
		if (firstRun) {
			bulletTimer.reset();
		}
		
		unsigned long int curTimePassed = bulletTimer.getTimeMicroseconds();
		timeDelta = 1.0/60.0;
		bulletTimer.reset();
		
		//totTimePassedGraphics += curTimePassed;
		// totTimePassedPhysics += curTimePassed*conVals[E_CONST_SPEEDUP_FACTOR];
		
		// if (totTimePassedPhysics > maxPhysTime) {
		// 	totTimePassedPhysics = maxPhysTime;
		// }
		
		
		if (currentTick > 4) {
			if (gamePhysics != NULL) {
				
				
				if (threadNetRecv.isReady()) {
					stopNT2();
					
					gameNetwork->applyNetworkActions();
					
					if (gameNetwork->isConnected) {
						startNT2();
					}
					
				}
				
				
				if (threadNetSend.isReady()) {
					stopNT();
					
					gameNetwork->flushNetworkActions();
					
					if (gameNetwork->isConnected) {
						startNT();
					}
					
				}
				
				gameLogic->applyBehavior();
				
				if (gem->turnBased) {
					
				}
				else {
								
				}
								
				
				flushKeyStack();
				gatherKeyActions();
				handleMovement();
				
				
				// if (destructCount > 50000) {
				// 	waitingOnDestruction = false;
				// }
				
				// if (waitingOnDestruction) {
				// 	destructCount++;
				// }
				// else {
					
				// }
				
				if (physicsOn) {
					gamePhysics->updateAll();
				}
				
				
			}
		}
		
		updateBullets();
		

		if (
			//true  
			// ( 
			// 	((frameSkipCount%frameSkip) == 0) &&
			// 	(frameMouseMove||ignoreFrameLimit)
			// ) || fpsTest
			//totTimePassedGraphics > 8000
			
			true
		) {
			//cout << "totTimePassedGraphics " << totTimePassedGraphics << "\n";
			//totTimePassedGraphics -= 8000;
			
			frameMouseMove = false;

			if (firstRun)
			{
				
			}
			else
			{
				// if ( (frameCount % 3000) == 0)
				// {
				// 	gw->updateLights();
				// }
			}

			


			//lastTime = curTime;
			timeOfDay += (getTargetTimeOfDay() - timeOfDay) / 8.0;

			if (
				lbDown &&
				gem->isDraggingObject && 
				(gem->draggingFromType != E_DT_NOTHING) &&
				((curTime-mdTime) > 300) &&
				(!gem->editPose)
			) {
				glutSetCursor(GLUT_CURSOR_CROSSHAIR);
			
				mouseMoveVec.setFXYZ(lastMouseX,lastMouseY,0.0);
				noTravel = mouseMoveVec.distance(&mouseStart) < MAX_TRAVEL_DIS;	
				markerFound = !noTravel;
				getPixData(&worldMarker, lastMouseX, lastMouseY, true, false);
			
			}
			else {
				markerFound = (menuList[E_FM_DDMENU]->visible)&&(gem->selObjInd < E_OBJ_LENGTH);
				glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
			}


			mouseMovingLoc = (mouseMovingLoc + 1) % mouseMovingSize;
			mouseCount++;
			mouseMoving[mouseMovingLoc].setIXY(lastMouseX, lastMouseY);



			

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
					
					
					
					//if (doFrameRender) {
						frameUpdate();
						lastDepthInvalidMove = depthInvalidMove;
						depthInvalidMove = false;
						depthInvalidRotate = false;
					//}
					
				}
			}
			
			


			if (fpsTest) {
				fpsCount++;
				
				if (fpsCount == fpsCountMax) {
					
					fpsTest = false;
					
					cout << "Average Frame Time: " << (fpsTimer.getElapsedTimeInMilliSec()*1000.0/((double)(fpsCountMax))) << "\n";
					cout << "FPS: " << 1.0/(fpsTimer.getElapsedTimeInSec()/((double)(fpsCountMax))) << "\n";
					fpsTimer.stop();
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
bool Singleton::gluInvertMatrix (double const (m) [16], float (invOut) [16])
        {
	    float inv[16], det;
	    int i;

	    inv[0] = m[5]  * m[10] * m[15] - 
	             m[5]  * m[11] * m[14] - 
	             m[9]  * m[6]  * m[15] + 
	             m[9]  * m[7]  * m[14] +
	             m[13] * m[6]  * m[11] - 
	             m[13] * m[7]  * m[10];

	    inv[4] = -m[4]  * m[10] * m[15] + 
	              m[4]  * m[11] * m[14] + 
	              m[8]  * m[6]  * m[15] - 
	              m[8]  * m[7]  * m[14] - 
	              m[12] * m[6]  * m[11] + 
	              m[12] * m[7]  * m[10];

	    inv[8] = m[4]  * m[9] * m[15] - 
	             m[4]  * m[11] * m[13] - 
	             m[8]  * m[5] * m[15] + 
	             m[8]  * m[7] * m[13] + 
	             m[12] * m[5] * m[11] - 
	             m[12] * m[7] * m[9];

	    inv[12] = -m[4]  * m[9] * m[14] + 
	               m[4]  * m[10] * m[13] +
	               m[8]  * m[5] * m[14] - 
	               m[8]  * m[6] * m[13] - 
	               m[12] * m[5] * m[10] + 
	               m[12] * m[6] * m[9];

	    inv[1] = -m[1]  * m[10] * m[15] + 
	              m[1]  * m[11] * m[14] + 
	              m[9]  * m[2] * m[15] - 
	              m[9]  * m[3] * m[14] - 
	              m[13] * m[2] * m[11] + 
	              m[13] * m[3] * m[10];

	    inv[5] = m[0]  * m[10] * m[15] - 
	             m[0]  * m[11] * m[14] - 
	             m[8]  * m[2] * m[15] + 
	             m[8]  * m[3] * m[14] + 
	             m[12] * m[2] * m[11] - 
	             m[12] * m[3] * m[10];

	    inv[9] = -m[0]  * m[9] * m[15] + 
	              m[0]  * m[11] * m[13] + 
	              m[8]  * m[1] * m[15] - 
	              m[8]  * m[3] * m[13] - 
	              m[12] * m[1] * m[11] + 
	              m[12] * m[3] * m[9];

	    inv[13] = m[0]  * m[9] * m[14] - 
	              m[0]  * m[10] * m[13] - 
	              m[8]  * m[1] * m[14] + 
	              m[8]  * m[2] * m[13] + 
	              m[12] * m[1] * m[10] - 
	              m[12] * m[2] * m[9];

	    inv[2] = m[1]  * m[6] * m[15] - 
	             m[1]  * m[7] * m[14] - 
	             m[5]  * m[2] * m[15] + 
	             m[5]  * m[3] * m[14] + 
	             m[13] * m[2] * m[7] - 
	             m[13] * m[3] * m[6];

	    inv[6] = -m[0]  * m[6] * m[15] + 
	              m[0]  * m[7] * m[14] + 
	              m[4]  * m[2] * m[15] - 
	              m[4]  * m[3] * m[14] - 
	              m[12] * m[2] * m[7] + 
	              m[12] * m[3] * m[6];

	    inv[10] = m[0]  * m[5] * m[15] - 
	              m[0]  * m[7] * m[13] - 
	              m[4]  * m[1] * m[15] + 
	              m[4]  * m[3] * m[13] + 
	              m[12] * m[1] * m[7] - 
	              m[12] * m[3] * m[5];

	    inv[14] = -m[0]  * m[5] * m[14] + 
	               m[0]  * m[6] * m[13] + 
	               m[4]  * m[1] * m[14] - 
	               m[4]  * m[2] * m[13] - 
	               m[12] * m[1] * m[6] + 
	               m[12] * m[2] * m[5];

	    inv[3] = -m[1] * m[6] * m[11] + 
	              m[1] * m[7] * m[10] + 
	              m[5] * m[2] * m[11] - 
	              m[5] * m[3] * m[10] - 
	              m[9] * m[2] * m[7] + 
	              m[9] * m[3] * m[6];

	    inv[7] = m[0] * m[6] * m[11] - 
	             m[0] * m[7] * m[10] - 
	             m[4] * m[2] * m[11] + 
	             m[4] * m[3] * m[10] + 
	             m[8] * m[2] * m[7] - 
	             m[8] * m[3] * m[6];

	    inv[11] = -m[0] * m[5] * m[11] + 
	               m[0] * m[7] * m[9] + 
	               m[4] * m[1] * m[11] - 
	               m[4] * m[3] * m[9] - 
	               m[8] * m[1] * m[7] + 
	               m[8] * m[3] * m[5];

	    inv[15] = m[0] * m[5] * m[10] - 
	              m[0] * m[6] * m[9] - 
	              m[4] * m[1] * m[10] + 
	              m[4] * m[2] * m[9] + 
	              m[8] * m[1] * m[6] - 
	              m[8] * m[2] * m[5];

	    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	    if (det == 0)
	        return false;

	    det = 1.0 / det;

	    for (i = 0; i < 16; i++)
	        invOut[i] = inv[i] * det;

	    return true;
	}
int Singleton::getMatrixInd (int col, int row)
                                           {
		return col*4 + row;
	}
void Singleton::ComputeFOVProjection (float * result, float fov, float aspect, float nearDist, float farDist, bool leftHanded)
          {
	    //
	    // General form of the Projection Matrix
	    //
	    // uh = Cot( fov/2 ) == 1/Tan(fov/2)
	    // uw / uh = 1/aspect
	    // 
	    //   uw         0       0       0    	0  1  2  3
	    //    0        uh       0       0    	4  5  6  7
	    //    0         0      f/(f-n)  1    	8  9  10 11
	    //    0         0    -fn/(f-n)  0   	12 13 14 15
	    //
	    // Make result to be identity first

		int i;
		int j;
		
		float val;
		
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				
				if (i == j) {
					val = 1.0f;
				}
				else {
					val = 0.0f;
				}
				
				result[getMatrixInd(i,j)] = val;
			}
		}

		
	    float frustumDepth = farDist - nearDist;
	    float oneOverDepth = 1 / frustumDepth;

	    result[getMatrixInd(1,1)] = 1 / tan(0.5f * fov);
	    result[getMatrixInd(0,0)] = (leftHanded ? 1 : -1 ) * result[getMatrixInd(1,1)] / aspect;
	    result[getMatrixInd(2,2)] = farDist * oneOverDepth;
	    result[getMatrixInd(3,2)] = (-farDist * nearDist) * oneOverDepth;
	    result[getMatrixInd(2,3)] = 1;
	    result[getMatrixInd(3,3)] = 0;
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
				clipDist[0],
				clipDist[1]
			); //set the perspective (angle of sight, width, height, , depth)
			
			
			// ComputeFOVProjection(
			// 	projMatrix.get(),
			// 	FOV,
			// 	w/h,
			// 	clipDist[0],
			// 	clipDist[1],
			// 	false
			// );
			
			glGetFloatv(GL_PROJECTION_MATRIX, projMatrix.get());
			
			glMatrixMode (GL_MODELVIEW); //set the matrix back to model
			
			//*180.0f/M_PI / 180 * M_PI
			
			glLoadIdentity();
			
			gluLookAt(
				cameraGetPos()->getFX(),
				cameraGetPos()->getFY(),
				cameraGetPos()->getFZ(),
				cameraGetPos()->getFX()+lookAtVec[0],
				cameraGetPos()->getFY()+lookAtVec[1],
				cameraGetPos()->getFZ()+lookAtVec[2],
				0.0f,
				0.0f,
				1.0f
			);
			
			// glRotatef(getCamRot(1)*180.0f/M_PI,0.0f,1.0f,0.0f);
			// glRotatef(getCamRot(0)*180.0f/M_PI,0.0f,0.0f,1.0f);
			// glTranslated(
			// 	-cameraGetPos()->getFX(),
			// 	-cameraGetPos()->getFY(),
			// 	-cameraGetPos()->getFZ()
			// );
			
			
			
			glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix.get());
			
			
			ptr1 = viewMatrix.get();
			ptr2 = projMatrix.get();
			
			
			for (i = 0; i < 16; i++) {
				viewMatrixD[i] = ptr1[i];
				projMatrixD[i] = ptr2[i];
			}
			
			gluInvertMatrix(viewMatrixD, viewMatrixDI);
			
			glGetIntegerv(GL_VIEWPORT, viewport);
			
			
			heightOfNearPlane =
				(
					((float)abs(viewport[3]-viewport[1])) /
			    (2.0f*tan(0.5f*FOV*M_PI/180.0f))
			  ) *
			  (
			  	2.0f / ((float)scaleFactor)	
			  );
			
			// lastW = -1; 
			// lastH = -1;
			
		}
		else {
			if (
				(lastW == w) && 
				(lastH == h) &&
				(lastPersp == perspectiveOn)
			) {
				
			}
			else {
				glViewport(0, 0, w, h);
				
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity ();
				
				glMatrixMode (GL_PROJECTION);
				glLoadIdentity ();
				
				// glMatrixMode(GL_PROJECTION);
				// glLoadIdentity();
				// glOrtho(-0.5, +0.5, -0.5, +0.5, clipDist[0], clipDist[1]);
				
				lastW = w;
				lastH = h;
			}
		}
		
		lastPersp = perspectiveOn;

	}
void Singleton::reshape (int w, int h)
        {

		setWH(w, h);

		screenWidth = w;
		screenHeight = h;
		
		setMatrices(baseW, baseH);
	}
#undef LZZ_INLINE
 
