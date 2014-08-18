
class Singleton
{
public:

	E_OBJ activeObject;
	E_OBJ tempObj;
	E_MOUSE_STATE mouseState;
	//E_GRASS_STATE grassState;
	eProgramState programState;
	eProgramAction progActionsDown[E_PS_SIZE * 256];
	eProgramAction progActionsUp[E_PS_SIZE * 256];

	bool bShift;
	bool tiltChanged;
	bool testOn;
	bool emptyVDNotReady;
	bool radiosityOn;
	bool updateLock;
	bool isFullScreen;
	bool changesMade;
	bool mapInvalid;
	bool bufferInvalid;
	bool wsBufferInvalid;
	bool forceGetPD;
	bool mouseLeftDown;
	bool mouseRightDown;
	bool pboSupported;
	bool notQuit;
	bool timerNotSet;
	bool lbDown;
	bool rbDown;
	bool mbDown;
	bool isZooming;
	bool isPanning;
	bool isBare;
	bool reportPagesDrawn;
	bool showMap;
	bool traceOn;
	bool waterOn;
	bool treesOn;
	bool firstRun;
	bool rotOn;
	

	int currentStep;
	int maxLayers;
	int maxChangesInHolders;
	int baseW;
	int baseH;
	int scaleFactor;
	int activeMode;
	int visPageSizeInPixels;
	int visPageSizeInUnits;
	int unitSizeInPixels;
	int maxHeightInUnits;
	int holdersPerLot;
	int pixelsPerLot;

	int numDynLights;
	int iNumSteps;

	int pixelsPerMeter;
	int pixelsPerUnit;
	int metersPerLot;
	int unitsPerMeter;
	int unitsPerLot;
	int extraRad;
	int defaultWinW;
	int defaultWinH;
	int shadersAreLoaded;
	int readyToRecompile;
	int lastPosX;
	int lastPosY;
	int frameCount;
	int maxHInPages;
	int maxWInPages;
	int screenWidth;
	int screenHeight;
	int mouseMovingSize;
	int mouseMovingLoc;
	int mouseMovingStepsBack;
	int mouseCount;
	int lastMouseX;
	int lastMouseY;
	int bufferedPageSizeInUnits;
	int voroSize;
	int geomCounter;
	int lightCounter;

	
	
	int metersPerNodeXY;
	int terDataVisPitchXY;
	int terDataBufPitchXY;
	int terDataBufPitchScaledXY;
	
	int metersPerNodeZ;
	int terDataVisPitchZ;
	int terDataBufPitchZ;
	int terDataBufPitchScaledZ;
	
	
	int terDataBufAmount;
	int terDataVisSize;
	int terDataBufSize;

	int terDataTexScale;
	int terDataBufSizeScaled;
	uint *terDataScaled;

	int iNodeDivsPerLot;

	int selGeomListInd;

	int holderSizeInPages;
	int holderSizeInPixels;
	//int grassSpacing;

	uint volGenFBOX;
	//uint volGenFBOY;
	//uint slicesPerPitch;
	uint palWidth;
	uint palHeight;
	uint blockShift;

	float directPass;
	float fogOn;
	float gridOn;
	float mapSampScale;
	float curBrushRad;
	float diskOn;

	float timeOfDay;
	float targetTimeOfDay;

	float gridSizeInPixels;

	float origWinW;
	float origWinH;


	double curMS;
	//double renderCount;
	
	float tiltAmount;
	
	//float maxSeaDepth;
	float currentFBOResolutionX;
	float currentFBOResolutionY;
	float mouseX;
	float mouseY;
	float mouseXUp;
	float mouseYUp;
	float cameraZoom;
	float targetZoom;
	float curTime;
	float lastTime;
	float myDelta;
	float mdTime;
	float muTime;
	//float heightmapMax;
	//float heightmapMin;
	float bufferMult;
	float holderSizeMB;

	float *paramArr;
	float *voroArr;
	float *matCountArr;
	int *geomIdArr;
	float *paramArrMap;

	//FIVector4 activeObjectPos;
	//FIVector4 minBoundsInPixels;
	//FIVector4 maxBoundsInPixels;
	FIVector4 worldSizeInPixels;
	FIVector4 mouseUpPD;
	FIVector4 mouseDownPD;
	FIVector4 mouseMovePD;
	
	FIVector4 tempVec1;
	FIVector4 tempVec2;

	//FIVector4 grassWH;

	FIVector4 worldSizeInPages;
	FIVector4 worldSizeInLots;
	FIVector4 worldSizeInHolders;
	FIVector4 worldSizeInBlocks;
	
	FIVector4 moveNodes[2];
	
	FIVector4 mouseStart;
	FIVector4 mouseEnd;

	FIVector4 mapFreqs;
	FIVector4 mapAmps;


	FIVector4 *mouseMoving;
	FIVector4 mouseVel;
	FIVector4 worldSeed;

	FIVector4 bufferDim;
	FIVector4 bufferDimHalf;
	FIVector4 origin;
	FIVector4 lastModXYZ;
	FIVector4 panMod;
	FIVector4 dMod;
	FIVector4 modXYZ;
	

	std::vector<DynObject *> dynObjects;
	std::vector<GameGeom *> selGeomList;
	PathHolder charPathHolder;
	PathHolder splitPathHolder;


	float floorHeightInMeters;
	float roofHeightInMeters;
	float wallRadInMeters;


	//int iNodeDivsPerLot;
	// int iSegmentsPerBlockSide;
	// float fSegmentsPerBlockSide;
	// int iNodesPerBlockSide;
	// int iNodesPerBlock;
	//int segmentSizeInMeters;
	//int floorsPerBlock;

	int blockSizeInLots;
	int blockSizeInMeters;
	int blockSizeInHolders;
	int blockSizeInPages;
	int blockSizeInPixels;

	//int maxFloors;

	

	//Image* uvPattern;
	Image *basicFont;
	//Image* testPat;
	Image *imageHM0;
	Image *imageHM1;
	Image *cloudImage;

	GameGeom *highlightedGeom;
	GameGeom *selectedGeom;

	Shader *curShaderPtr;
	string curShader;
	string allText;


	PoolManager* gpuPool;
	PoolManager* entityPool;
	
	
	vector<string> shaderStrings;
	vector<string> fboStrings;
	vector<string> shaderTextureIds;
	map<string, Shader *> shaderMap;
	map<string, FBOSet *> fboMap;

	//string curVGString;
	//string smallVGString;

	GLuint volId;
	GLuint volIdLinear;
	GLuint volIdEmpty;
	GLuint volIdEmptyLinear;

	GLuint volGenId;
	//GLuint terrainId;
	GLuint volTris;
	GLuint sliceTris;
	//GLuint grassTris[MAX_GRASS_LEV];
	uint *lookup2to3;
	//uint* volData;
	//uint* volDataLinear;

	unsigned char *resultImage;

	charArr nullBuffer;
	charArr lastImageBuffer;
	charArr lastJSONBuffer;


	JSONValue *rootObj;

	WebSocketServer *myWS;
	Timer myTimer;
	//Timer fpsTimer;
	GameWorld *gw;

	float lightArr[MAX_LIGHTS * 16];
	int numLights;


	bool multiLights;

	//int *rbStack;
	int *rbHeightStack;


	TerTexture terTextures[MAX_TER_TEX];

	//FTGLPixmapFont* myFont;


	GameGUI *mainGUI;

	Singleton()
	{
		volTris = NULL;
		sliceTris = NULL;
		gw = NULL;
		myWS = NULL;
	}


	void init(int _defaultWinW, int _defaultWinH, int _scaleFactor, WebSocketServer *_myWS)
	{

		pushTrace("Singleton init");
		int i;
		int j;
		rootObj = NULL;
		highlightedGeom = NULL;
		selectedGeom = NULL;
		//rbStack = NULL;
		rbHeightStack = NULL;

		multiLights = false;
		numDynLights = E_OBJ_LENGTH - E_OBJ_LIGHT0;
		updateMultiLights();

		iNumSteps = 64;
		currentStep = 0;

		srand(time(NULL));

		timeOfDay = 1.0f;
		targetTimeOfDay = 1.0f;

		//uvPattern = loadBMP("..\\data\\uvpat.bmp");
		//uvPattern->getTextureId(GL_LINEAR);
		//testPat = loadBMP("..\\data\\testpat.bmp");
		basicFont = loadBMP("..\\data\\basicfont.bmp");
		imageHM0 = loadBMP("..\\data\\hm0.bmp");
		imageHM1 = loadBMP("..\\data\\hm1.bmp");
		cloudImage = loadBMP("..\\data\\clouds.bmp");

		//testPat->getTextureId(GL_LINEAR);
		basicFont->getTextureId(GL_NEAREST);
		imageHM0->getTextureId(GL_NEAREST);
		imageHM1->getTextureId(GL_NEAREST);
		cloudImage->getTextureId(GL_LINEAR);

		selGeomListInd = 0;

		mapSampScale = 0.5f;
		curMS = 0.0;
		//renderCount = 1.0;
		int newPitch = (imageHM0->width) * mapSampScale; //*2;

		numLights = MAX_LIGHTS;//min(MAX_LIGHTS,E_OBJ_LENGTH-E_OBJ_LIGHT0);

		// myFont = new FTGLPixmapFont("..\\data\\arial.ttf");
		// if(myFont->Error()) {
		//  doTraceND("Error Loading Font");
		// }
		// else {
		//  myFont->FaceSize(72);
		// }



		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////

		bShift = false;
		tiltChanged = false;
		emptyVDNotReady = true;
		firstRun = true;
		waterOn = false;
		treesOn = true;
		rotOn = false;

		maxLayers = MAX_LAYERS;

		// Resolution Dependent

		volGenFBOX = 128; // MAX OF 128, DO NOT CHANGE THIS VALUE



		maxChangesInHolders = 4; // this number is defined here // not in processPages()
		maxHInPages = 2;
		maxWInPages = 2;


		visPageSizeInPixels = 512; // height of one page in pixels
		holderSizeInPixels = 512; // height of holder in pixels
		pixelsPerMeter = PIXELS_PER_METER; // when you make pixels per meter larger, you must do the same for units per meter
		unitsPerMeter = 4;
		bufferMult = 1.25f;

		tiltAmount = 0.5f;


		// IMPORTANT: Maximum height must be less than 2^16, max world pitch must be less than 2^32

		// World Scale Dependent

		metersPerNodeXY = 16;
		metersPerNodeZ = 8;
		
		metersPerLot = 64; // adjust this to make lots bigger
		blockSizeInLots = 8;
		blockSizeInMeters = blockSizeInLots * metersPerLot;
		
		terDataTexScale = 1;
		if (pixelsPerMeter >= 128) {
			terDataTexScale = 2;
		}
		
		terDataVisPitchXY = blockSizeInMeters / metersPerNodeXY;
		
		iNodeDivsPerLot = terDataVisPitchXY / blockSizeInLots;
		
		
		terDataBufAmount = iNodeDivsPerLot; // pad with one extra lot
		
		floorHeightInMeters = (float)metersPerNodeZ;
		roofHeightInMeters = ((float)metersPerNodeXY)/4.0f;
		wallRadInMeters = ((float)metersPerNodeXY)/4.0f;
		
		


		
	
		
		
		
		
		terDataBufPitchXY = terDataVisPitchXY + terDataBufAmount * 2;
		terDataBufPitchScaledXY = terDataBufPitchXY * terDataTexScale;
		
		terDataVisPitchZ = blockSizeInMeters / metersPerNodeZ;
		terDataBufPitchZ = terDataVisPitchZ + terDataBufAmount * 2;
		terDataBufPitchScaledZ = terDataBufPitchZ * terDataTexScale;

		terDataVisSize = terDataVisPitchXY * terDataVisPitchXY * terDataVisPitchZ;
		terDataBufSize = terDataBufPitchXY * terDataBufPitchXY * terDataBufPitchZ;
		terDataBufSizeScaled = terDataBufPitchScaledXY * terDataBufPitchScaledXY * terDataBufPitchScaledZ;


		terDataScaled = new uint[terDataBufSizeScaled];
		//rbStack = new PathNode[terDataBufSize* MAX_BLOCK_STACK];
		rbHeightStack = new int[terDataBufSize];

		//maxFloors = MAX_FLOORS;

		pixelsPerUnit = pixelsPerMeter / unitsPerMeter;
		pixelsPerLot = pixelsPerMeter * metersPerLot;
		unitsPerLot = unitsPerMeter * metersPerLot;
		worldSizeInLots.setIXYZ(newPitch, newPitch, blockSizeInLots);
		holderSizeInPages = holderSizeInPixels / visPageSizeInPixels;
		holdersPerLot = pixelsPerLot / holderSizeInPixels;
		visPageSizeInUnits = visPageSizeInPixels / pixelsPerUnit;



		worldSizeInHolders.copyIntMult(&worldSizeInLots, holdersPerLot);
		bufferedPageSizeInUnits = visPageSizeInUnits * bufferMult;
		blockSizeInHolders = blockSizeInLots * holdersPerLot; // 8*2 = 16
		blockSizeInPages = blockSizeInHolders * holderSizeInPages;
		blockSizeInPixels = blockSizeInHolders * holderSizeInPixels;
		worldSizeInBlocks.copyIntDiv(&worldSizeInHolders, blockSizeInHolders);
		worldSizeInPages.copyIntMult(&worldSizeInHolders, holderSizeInPages);
		unitSizeInPixels = visPageSizeInPixels / visPageSizeInUnits;

		worldSizeInPixels.copyIntMult(&worldSizeInHolders, holderSizeInPixels);

		blockShift = intLogB2(blockSizeInHolders * blockSizeInHolders * blockSizeInHolders);

		holderSizeMB = ( ((float)MAX_LAYERS) * 2.0f * 4.0f * (float)(holderSizeInPixels * holderSizeInPixels)) / (1024.0f * 1024.0f);








		cout << "\n\n\n\n\n\n";

		cout << "blockSizeInHolders: " << blockSizeInHolders << "\n";
		cout << "blockSizeInPixels: " << blockSizeInPixels << "\n";
		cout << "terDataBufPitchScaledXY " << terDataBufPitchScaledXY << "\n";
		cout << "maxChangesInHolders: " << maxChangesInHolders << "\n";
		cout << "volGenFBOX: " << volGenFBOX << "\n";
		cout << "volGenFBOY: " << volGenFBOX *volGenFBOX << "\n";
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
		mapAmps.setFXYZW(1.0f, 4.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f); //0.0f, 0.0f, 0.0f);//


		gridSizeInPixels = pixelsPerMeter;


		//cout << "holderSizeInPixels X2 " << holderSizeInPixels*2 << "\n";

		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		radiosityOn = true;
		testOn = true;
		updateLock = false;
		traceOn = false;
		gridOn = 0.0f;
		fogOn = 0.0f;
		geomCounter = 0;
		lightCounter = 0;

		//grassSpacing = 1;//8/DEF_SCALE_FACTOR;// *2.0;
		directPass = 0.0f;

		int ccr = 0;
		int ccg = 0;
		int ccb = 0;

		// TODO: examine if this variable is necessary
		maxHeightInUnits = (worldSizeInPages.getIZ() - bufferMult) * (visPageSizeInUnits);

		// minBoundsInPixels.setIXYZ(0, 0, 0);
		// maxBoundsInPixels.setIXYZ(
		// 	(worldSizeInPages.getIX() - 1)*unitSizeInPixels * visPageSizeInUnits,
		// 	(worldSizeInPages.getIY() - 1)*unitSizeInPixels * visPageSizeInUnits,
		// 	(worldSizeInPages.getIZ() - 1)*unitSizeInPixels * visPageSizeInUnits
		// );

		//heightmapMax = maxBoundsInPixels.getFZ() * 0.75f;
		//heightmapMin = 0.0f;
		//maxSeaDepth = 8.0f * pixelsPerMeter;
		

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


		geomIdArr = new int[1024];
		paramArr = new float[4096];
		voroArr = new float[27 * 4];
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

		resultImage = new unsigned char[256 * 256 * 4];

		mouseMovingSize = 100;
		mouseMovingLoc = 0;
		mouseCount = 0;
		mouseMovingStepsBack = 20;
		mouseMoving = new FIVector4[mouseMovingSize];

		myWS = _myWS;

		
		activeMode = 1;

		isZooming = false;
		isPanning = false;
		reportPagesDrawn = false;
		isBare = true;
		//grassWH.setFXYZ(1.0/128.0, 1.0/512.0, 0.0);
		diskOn = 0.0f;






		defaultWinW = _defaultWinW / _scaleFactor;
		defaultWinH = _defaultWinH / _scaleFactor;
		scaleFactor = _scaleFactor;

		origWinW = _defaultWinW;
		origWinH = _defaultWinH;

		curBrushRad = 1.0f;

		mouseState = E_MOUSE_STATE_MOVE;

		worldSeed.setFXYZ(
			genRand(5000.0f, 500000.0f),
			genRand(5000.0f, 500000.0f),
			genRand(5000.0f, 500000.0f)
		);







		wsBufferInvalid = true;

		bufferDim.setIXY(defaultWinW, defaultWinH);
		bufferDimHalf.setIXY(defaultWinW / 2, defaultWinH / 2);


		myTimer.start();
		//fpsTimer.start();

		//grassState = E_GRASS_STATE_OFF;

		activeObject = E_OBJ_CAMERA;

		extraRad = 0;
		lastTime = 0.0;







		mbDown = false;
		lbDown = false;
		rbDown = false;



		isFullScreen = false;
		shadersAreLoaded = 0;
		readyToRecompile = 0;

		programState = E_PS_IN_GAME;


		for (i = 0; i < E_OBJ_LENGTH; i++)
		{
			dynObjects.push_back(new DynObject());
		}

		dynObjects[E_OBJ_CAMERA]->init(0, 0, worldSizeInPixels.getIZ() / 2, 0, 0, 0, false, false, NULL, 4.0f );

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

			dynObjects[i]->init(1024 + i * 256, 1024 + i * 256, 2048, ccr, ccg, ccb,     true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 32.0f );



		}

		dynObjects[E_OBJ_FOG]->init(-1024, -1024, -512,   0, 0, 255,     true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		dynObjects[E_OBJ_CUTAWAY]->init(1024 - 256, 1024 - 256, 2048,   0, 255, 0,     true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );

		// dynObjects[E_OBJ_P0]->init(512-256,1024-256,2048,   128,0,0,    true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		// dynObjects[E_OBJ_P1]->init(512,1024,2048,      255,0,0,  true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );
		// dynObjects[E_OBJ_P2]->init(1024,512,2048,      0,255,255,  true, true, &(dynObjects[E_OBJ_CAMERA]->pos), 64.0f );


		cameraZoom = 1.0f;
		targetZoom = 1.0f;

		mouseX = 0.0f;
		mouseY = 0.0f;
		mouseXUp = 0.0f;
		mouseYUp = 0.0f;






		//createSliceList(visPageSizeInPixels);
		//createVTList();

		// for (i = 0; i < MAX_GRASS_LEV; i++) {
		//  createGrassList(i); //grassSpacing
		// }



		//// GL WIDGET START ////

		frameCount = -1;
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
		
		fboStrings.push_back("frontFaceFBO");
		fboStrings.push_back("backFaceFBO");
		
		fboStrings.push_back("frontFaceMapFBO");
		fboStrings.push_back("backFaceMapFBO");

		fboStrings.push_back("pagesFBO");
		fboStrings.push_back("waterFBO");

		//fboStrings.push_back("grassFBO");
		fboStrings.push_back("geomFBO");
		fboStrings.push_back("combineFBO");
		fboStrings.push_back("combineFBOWithWater");
		fboStrings.push_back("combineFBOSpare");
		fboStrings.push_back("noiseFBO");
		fboStrings.push_back("resultFBO0");
		fboStrings.push_back("resultFBO1");
		fboStrings.push_back("volGenFBO0");
		fboStrings.push_back("volGenFBO1");

		// for (i = 0; i < VOLGEN_NAMES_LEN; i++) {
		//  fboStrings.push_back(VOLGEN_NAMES[i]);
		// }


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



		shaderStrings.push_back("GUIShader");
		shaderStrings.push_back("RoadShader");
		shaderStrings.push_back("SkeletonShader");
		shaderStrings.push_back("DilateShader");
		shaderStrings.push_back("TerrainMix");
		shaderStrings.push_back("Simplex2D");
		shaderStrings.push_back("TopoShader");
		shaderStrings.push_back("TCShader");
		shaderStrings.push_back("TCShader2");
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
		//shaderStrings.push_back("DownSampleShader");
		shaderStrings.push_back("DownScaleShader");
		shaderStrings.push_back("RadiosityShader");
		shaderStrings.push_back("RadiosityCombineShader");
		shaderStrings.push_back("FogShader");
		shaderStrings.push_back("GeomShader");
		//shaderStrings.push_back("PreGrassShader");
		//shaderStrings.push_back("GrassShader");
		shaderStrings.push_back("CombineShader");
		//shaderStrings.push_back("DepositShader");
		shaderStrings.push_back("GenerateVolume");
		shaderStrings.push_back("GenerateVolumeEnt");
		shaderStrings.push_back("RenderVolume");
		//shaderStrings.push_back("RenderVolumeSlice");
		//shaderStrings.push_back("LightingShader");
		//shaderStrings.push_back("OutlineShader");
		//shaderStrings.push_back("WaterShaderNormals");



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
			shaderMap.insert(  pair<string, Shader *>(shaderStrings[i], NULL)  );
		}
		doShaderRefresh();

		//fboSize = 512;
		//bufsPerFBO = 2;

		for (i = 0; i < fboStrings.size(); i++)
		{
			fboMap.insert(  pair<string, FBOSet *>(fboStrings[i], new FBOSet())  );
		}

		//init(int _numBufs, int _width, int _height, int _bytesPerChannel, bool hasDepth, int filterEnum, int clampEnum);

		fboMap["worldSpaceFBO"]->init(4, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_LINEAR);
		
		int faceDim = 256;
		
		fboMap["frontFaceFBO"]->init(1, faceDim, faceDim, 4, false, GL_NEAREST);
		fboMap["backFaceFBO"]->init(1, faceDim, faceDim, 4, false, GL_NEAREST);
		fboMap["frontFaceMapFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_NEAREST);
		fboMap["backFaceMapFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_NEAREST);
		
		fboMap["palFBO"]->init(1, palWidth, palHeight, 1, false, GL_LINEAR);

		fboMap["pagesFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
		fboMap["waterFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
		//fboMap["grassFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
		fboMap["geomFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, true);
		fboMap["combineFBO"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
		fboMap["combineFBOWithWater"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
		fboMap["combineFBOSpare"]->init(2, bufferDim.getIX(), bufferDim.getIY(), 1, false);
		fboMap["noiseFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["resultFBO0"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false);
		fboMap["resultFBO1"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false);
		//fboMap["resultFBO2"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false);


		// j = 1;
		// for (i = 0; i < VOLGEN_NAMES_LEN; i++) {
		//  fboMap[VOLGEN_NAMES[i]]->init(2, j, j*j, 1, false);
		//  j *= 2;
		// }

		fboMap["volGenFBO0"]->init(2, volGenFBOX, volGenFBOX * volGenFBOX, 1, false);
		fboMap["volGenFBO1"]->init(2, volGenFBOX, volGenFBOX * volGenFBOX, 1, false);



		fboMap["waveFBO"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);

		fboMap["swapFBOLin0"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);
		fboMap["swapFBOLin1"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_LINEAR);

		fboMap["swapFBOBLin0"]->init(1, bufferDim.getIX() / 2, bufferDim.getIY() / 2, 1, false, GL_LINEAR);
		fboMap["swapFBOBLin1"]->init(1, bufferDim.getIX() / 2, bufferDim.getIY() / 2, 1, false, GL_LINEAR);

		fboMap["swapFBOLinHalf0"]->init(1, bufferDim.getIX() / 2, bufferDim.getIY() / 2, 1, false, GL_LINEAR);
		fboMap["swapFBOLinHalf1"]->init(1, bufferDim.getIX() / 2, bufferDim.getIY() / 2, 1, false, GL_LINEAR);

		//fboMap["swapFBO32Lin0"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_LINEAR);
		//fboMap["swapFBO32Lin1"]->init(1, bufferDim.getIX(), bufferDim.getIY(), 4, false, GL_LINEAR);


		//fboMap["terrainFBO"]->init(1, newPitch*iNodeDivsPerLot, newPitch*iNodeDivsPerLot, 2, false, GL_NEAREST, GL_REPEAT);

		fboMap["cityFBO"]->init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["hmFBO"]->init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["hmFBOLinear"]->init(1, newPitch, newPitch, 1, false, GL_LINEAR, GL_REPEAT);
		fboMap["simplexFBO"]->init(1, newPitch, newPitch, 1, false, GL_LINEAR, GL_REPEAT);
		fboMap["swapFBO0"]->init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
		fboMap["swapFBO1"]->init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);


		
		createVTListTilt();


		//fboMap["terrainFBO"]->init(1, newPitch*iNodeDivsPerLot, newPitch*iNodeDivsPerLot, 2, false, GL_NEAREST, GL_REPEAT);
		/*
		(  GLenum target,
		GLint level,
		GLint internalFormat,
		GLsizei width,
		GLsizei height,
		GLint border,
		GLenum format,
		GLenum type,
		const GLvoid * data);

		*/
		// glBindTexture(GL_TEXTURE_2D,terrainId);
		// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newPitch*iNodeDivsPerLot, newPitch*iNodeDivsPerLot, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, 0);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// // glClearColor(0.0,0.0,0.0,0.0);
		// // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glBindTexture(GL_TEXTURE_2D,0);



		mainGUI = new GameGUI();
		mainGUI->init(this);


		loadAllData();

		GamePlant::initAllPlants(this);



		gw = new GameWorld();
		
		gpuPool = new PoolManager();
		gpuPool->init(this,false);
		
		entityPool = new PoolManager();
		entityPool->init(this,false);
		
		
		
		gw->init(this);
		gw->initMap();

		doTraceND("GW DONE");


		popTrace();



	}


	int requestTerIndex(int requestingBlockId)
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


	static void qNormalizeAngle(int &angle)
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


	void perspectiveProjection()
	{
		float aspect = 1.0;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-aspect, +aspect, -1.0, +1.0, 4.0, 15.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void orthographicProjection()
	{

		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();

	}





	////// END GLWIDGET ////


















	~Singleton()
	{
		if (gw)
		{
			delete gw;
		}
	}



	float genRand(float LO, float HI)
	{

		return LO + (float)rand() / ((float)RAND_MAX / (HI - LO));
	}


	void setProgAction(eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown)
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

	void setProgActionAll(unsigned char kc, eProgramAction pa, bool isDown)
	{
		int i;

		for (i = 0; i < E_PS_SIZE; i++)
		{
			setProgAction((eProgramState)i, kc, pa, isDown);
		}

	}


	// void createGrassList(int index) {


	//  int i;
	//  int j;

	//  float fi;
	//  float fj;


	//  int spacing = 1;
	//  float multiplier = 1.0f;

	//  switch (index) {
	//    case 0:
	//      spacing = 8;
	//      multiplier = 1.0f;
	//    break;

	//    case 1:
	//      spacing = 4;
	//      multiplier = 2.0f;
	//    break;

	//    case 2:
	//      spacing = 2;
	//      multiplier = 4.0f;
	//    break;

	//    case 3:
	//      spacing = 2;
	//      multiplier = 8.0f;
	//    break;
	//  }


	//  int iMax = DEF_WIN_W/spacing;//bufferDim.getIX()/spacing;
	//  int jMax = DEF_WIN_H/spacing;//bufferDim.getIY()/spacing;

	//  float fiMax = (float)iMax;
	//  float fjMax = (float)jMax;

	//  grassTris[index] = glGenLists(1);


	//  glNewList(grassTris[index], GL_COMPILE);

	//  //glBegin(GL_TRIANGLES);
	//  glBegin(GL_QUADS);

	//  //glNormal3f(0, 0, 1);



	//  for (j = jMax-1; j >= 0; j -= 1) {
	//    fj = ((float)(j*2-jMax) + 1.0f)*multiplier/fjMax;
	//    for (i = 0; i < iMax; i += 1) {
	//      fi = ((float)(i*2-iMax) + 1.0f)*multiplier/fiMax;

	//      glMultiTexCoord4f( GL_TEXTURE0, fi, fj, 0.2f, -1.0);
	//      glVertex3f(fi,fj,0.0);


	//      glMultiTexCoord4f( GL_TEXTURE0, fi, fj, 0.0f, 0.0);
	//      glVertex3f(fi,fj,0.0f);


	//      glMultiTexCoord4f( GL_TEXTURE0, fi, fj, 0.2f, 1.0);
	//      glVertex3f(fi,fj,0.0f);


	//      glMultiTexCoord4f( GL_TEXTURE0, fi, fj, 1.0f, 0.0);
	//      glVertex3f(fi,fj,0.0f);


	//    }



	//  }

	//  glEnd();

	//  glEndList();

	// }

	void drawCrossHairs(FIVector4 originVec, float radius)
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

	void drawLine(FIVector4 *p0, FIVector4 *p1)
	{
		

		glBegin(GL_LINES);
		glMultiTexCoord3f(GL_TEXTURE0, 0.0f, 0.0f, 0.0f);
		glVertex3f(p0->getFX(), p0->getFY(), p0->getFZ());
		glMultiTexCoord3f(GL_TEXTURE0, 0.0f, 0.0f, 1.0f);
		glVertex3f(p1->getFX(), p1->getFY(), p1->getFZ());
		glEnd();
	}


	void drawCubeCentered(FIVector4 *originVec, float radius)
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

	void drawBoxUp(FIVector4 originVec, float radiusX, float radiusY, float diamZ)
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


	void drawBox(FIVector4 *minVec, FIVector4 *maxVec, int faceFlag=2)
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

	inline float glslMod(float x, float y)
	{
		return x - y * floor(x / y);
	}



	// inline void sampleAtPoint(FIVector4* point, FIVector4* texc) {

	//  //vec3 point;
	//     //vec2 texc;

	//  int bmWidth = slicesPerPitch*slicesPerPitch*slicesPerPitch - 1;
	//     int squareWidth = slicesPerPitch*slicesPerPitch;
	//     int squareWidthM1 = squareWidth-1;

	//     FIVector4 newPoint;
	//     newPoint.copyFrom(point);
	//     newPoint.multXYZ(1.0f/bufferMult);
	//     newPoint.addXYZ( (1.0f-1.0f/bufferMult)/2.0f );



	//     FIVector4 curFace;
	//     curFace.copyFrom(&newPoint);
	//     curFace.multXYZ( squareWidthM1 );


	//     int bval = curFace.getIZ();
	//     int xval = bval%slicesPerPitch;
	//     int yval = bval/slicesPerPitch;

	//     texc->setIX( (curFace.getIX() + xval*squareWidth ) );
	//     texc->setIY( (curFace.getIY() + yval*squareWidth ) );

	//     texc->multXYZ(1.0f / ((float)bmWidth) );


	//     //return texc.xy

	// }

	// inline void sampleAtPoint(FIVector4* point, FIVector4* texc) {

	//  //vec3 point;
	//     //vec2 texc;

	//     float pitch = slicesPerPitch*slicesPerPitch;
	//     float pitchM1 = pitch-1.0;

	//     //vec3 newPoint = point/bufferMult + (1.0-1.0/bufferMult)/2.0;
	//     FIVector4 newPoint;
	//     newPoint.copyFrom(point);
	//     newPoint.multXYZ(1.0f/bufferMult);
	//     newPoint.addXYZ( (1.0f-1.0f/bufferMult)/2.0f );



	//     //vec3 curFace = (newPoint.rgb*pitchM1+0.5)/pitch;
	//     FIVector4 curFace;
	//     curFace.copyFrom(&newPoint);
	//     curFace.multXYZ( (pitchM1+0.5f)/pitch );



	//     float bval = curFace.getFZ()*pitchM1;
	//     float xval = floor(glslMod(bval, slicesPerPitch))/slicesPerPitch;
	//     float yval = floor(bval/slicesPerPitch)/slicesPerPitch;

	//     texc->setFX( curFace.getFX()/(slicesPerPitch) + xval );
	//     texc->setFY( curFace.getFY()/(slicesPerPitch) + yval );



	//     //return texc.xy

	// }

	//  void createSliceList(int numSlices) {

	//   int i;

	//   float fx1 = -1.0f;
	//   float fy1 = -1.0f;
	//   float fx2 = 1.0f;
	//   float fy2 = 1.0f;

	//   FIVector4 vec3In;
	//   FIVector4 vec2Out;


	//   float centerX = (fx1 + fx2)/2.0f;
	//   float centerY = (fy1 + fy2)/2.0f;

	//   float fy25 = fy1*0.75f + fy2*0.25f;
	//   float fy75 = fy1*0.25f + fy2*0.75f;

	//   float x[4];
	//   float y[4];


	//   sliceTris = glGenLists(1);

	//   glNewList(sliceTris, GL_COMPILE);
	//   glBegin(GL_QUADS);

	//   float zval;
	//   float lerp;
	//   float lerpi;
	//   float fNumSlices = ((float)numSlices) - 1.0f;

	//   for (i = 0; i < numSlices; i++) {
	//     lerp = ((float)i)/fNumSlices;
	//     lerpi = 1.0f-lerp;

	//     zval = lerp*0.5 + 0.25;

	// x[0] = fx1;
	// y[0] = fy75*lerpi + fy25*lerp;

	// x[1] = centerX;
	// y[1] = fy2*lerpi + centerY*lerp;

	// x[2] = fx2;
	// y[2] = fy75*lerpi + fy25*lerp;

	// x[3] = centerX;
	// y[3] = centerY*lerpi + fy1*lerp;

	// vec3In.setFXYZ(0.0f,1.0f,lerpi);
	// sampleAtPoint(&vec3In,&vec2Out);
	// glMultiTexCoord3f(GL_TEXTURE0, vec3In.getFX(), vec3In.getFY(), vec3In.getFZ());
	//     glMultiTexCoord3f(GL_TEXTURE1, vec2Out.getFX(), vec2Out.getFY(), 0.0f);
	//     glVertex3f(x[0], y[0], zval);

	//     vec3In.setFXYZ(0.0f,0.0f,lerpi);
	//     sampleAtPoint(&vec3In,&vec2Out);
	// glMultiTexCoord3f(GL_TEXTURE0, vec3In.getFX(), vec3In.getFY(), vec3In.getFZ());
	//     glMultiTexCoord3f(GL_TEXTURE1, vec2Out.getFX(), vec2Out.getFY(), 0.0f);
	//     glVertex3f(x[1], y[1], zval);

	//     vec3In.setFXYZ(1.0f,0.0f,lerpi);
	//     sampleAtPoint(&vec3In,&vec2Out);
	// glMultiTexCoord3f(GL_TEXTURE0, vec3In.getFX(), vec3In.getFY(), vec3In.getFZ());
	//     glMultiTexCoord3f(GL_TEXTURE1, vec2Out.getFX(), vec2Out.getFY(), 0.0f);
	//     glVertex3f(x[2], y[2], zval);

	//     vec3In.setFXYZ(1.0f,1.0f,lerpi);
	//     sampleAtPoint(&vec3In,&vec2Out);
	// glMultiTexCoord3f(GL_TEXTURE0, vec3In.getFX(), vec3In.getFY(), vec3In.getFZ());
	//     glMultiTexCoord3f(GL_TEXTURE1, vec2Out.getFX(), vec2Out.getFY(), 0.0f);
	//     glVertex3f(x[3], y[3], zval);
	//   }



	//   glEnd();
	//   glEndList();

	//  }


	void createVTListTilt()
	{

		int i;
		int j;

		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);

		//volTris = glGenLists(1);

		float halfTilt = tiltAmount/2.0f;
		
		float halfTiltUp = 0.5f + halfTilt;
		float halfTiltDn = 0.5f - halfTilt;

		float texMin = 0.0f;
		float texMax = 1.0f;

		float fx1 = -1.0f;
		float fy2 = -1.0f;
		float fx2 = 1.0f;
		float fy1 = 1.0f;

		float centerX = (fx1 + fx2) / 2.0f;
		//float centerY = (fy1 + fy2) / 2.0f;

		float fy25 = fy1 * halfTiltUp + fy2 * halfTiltDn;
		float fy75 = fy1 * halfTiltDn + fy2 * halfTiltUp;

		float coordsX[8];
		float coordsY[8];

		coordsX[0] = centerX;
		coordsY[0] = fy1 * tiltAmount + fy2 * (1.0-tiltAmount);//centerY; // back face center

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


		//glNewList(volTris, GL_COMPILE);
		
		

		for (j = 0; j < 2; j++) {
			
			bindShader("TCShader");
			
			if (j == 0) {
				bindFBO("frontFaceFBO");
			}
			else {
				coordsY[0] = fy2 * tiltAmount + fy1 * (1.0-tiltAmount);
				bindFBO("backFaceFBO");
			}
			
			
			
			glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0, 0, 1);
			for (i = 0; i < 8; i++)
			{

				if (j == 0) { // front faces
					if (i == 0)
					{
						glMultiTexCoord3f( GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
					}
					else
					{
						glMultiTexCoord3f( GL_TEXTURE0, backfaceX[i], backfaceY[i], backfaceZ[i]);
					}
				}
				else { // back faces
					glMultiTexCoord3f( GL_TEXTURE0, backfaceX[i], backfaceY[i], backfaceZ[i]);
				}
				

				glVertex3f(coordsX[i], coordsY[i], 0.0f);
			}
			glEnd();
			
			
			
			unbindFBO();
			unbindShader();
			
		}
		
		
		
		tempVec1.setFXYZ(
			cameraPos->getFX() - worldSizeInPixels.getFX() / 2.0f,
			cameraPos->getFY() - worldSizeInPixels.getFY() / 2.0f,
			0.0f
		);
		tempVec2.setFXYZ(
			cameraPos->getFX() + worldSizeInPixels.getFX() / 2.0f,
			cameraPos->getFY() + worldSizeInPixels.getFY() / 2.0f,
			worldSizeInPixels.getFZ()*4.0f // TODO: SHOULD BE NO MULTIPLER HERE
		);
		
		for (j = 0; j < 2; j++) {
			
			bindShader("TCShader2");
			
			if (j == 0) {
				bindFBO("backFaceMapFBO");
			}
			else {
				bindFBO("frontFaceMapFBO");
			}
			
			setShaderFloat("tiltAmount", tiltAmount);
			setShaderFloat("cameraZoom", cameraZoom);
			setShaderfVec2("bufferDim", &(bufferDim));
			setShaderfVec3("cameraPos", cameraPos);
			
			drawBox(&tempVec1,&tempVec2,j);

			unbindFBO();
			unbindShader();
			
		}
		
		

		


		//glEndList();
	}

	void createVTList()
	{

		volTris = glGenLists(1);


		float texMin = 0.0f;
		float texMax = 1.0f;

		float fx1 = -1.0f;
		float fy2 = -1.0f;
		float fx2 = 1.0f;
		float fy1 = 1.0f;

		float centerX = (fx1 + fx2) / 2.0f;
		float centerY = (fy1 + fy2) / 2.0f;

		float fy25 = fy1 * 0.75f + fy2 * 0.25f;
		float fy75 = fy1 * 0.25f + fy2 * 0.75f;

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

		for (i = 0; i < 8; i++)
		{

			//glColor4f(backfaceX[i], backfaceY[i], backfaceZ[i], 1.0f);

			glMultiTexCoord3f( GL_TEXTURE0, backfaceX[i], backfaceY[i], backfaceZ[i]);

			if (i == 0)
			{
				//glColor4f((backfaceX[i]+1.0f)/2.0f, (backfaceY[i]+1.0f)/2.0f, (backfaceZ[i]+1.0f)/2.0f, 1.0f);
				glMultiTexCoord3f( GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				glMultiTexCoord3f( GL_TEXTURE1, backfaceX[i], backfaceY[i], backfaceZ[i]);
			}

			glVertex3f(coordsX[i], coordsY[i], 0.0f);
		}

		glEnd();


		glEndList();
	}



	void doShaderRefresh()
	{

		LAST_COMPILE_ERROR = false;

		pushTrace( "doShaderRefresh" );

		readyToRecompile = 0;

		int i;
		int j;


		for (i = 0; i < shaderStrings.size(); i++)
		{
			if (shaderMap[ shaderStrings[i] ])
			{

				// TODO: Memory leak?

				delete shaderMap[ shaderStrings[i] ];
				shaderMap[ shaderStrings[i] ] = NULL;
			}
			shaderMap[ shaderStrings[i] ] = new Shader( ("../src/glsl/" + shaderStrings[i] + ".c").c_str() );



		}
		shadersAreLoaded = 1;
		readyToRecompile = 1;

		popTrace();

	}





	void setMatrices(int w, int h)
	{

		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity ();
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();

	}

	void setWH(int w, int h)
	{

		baseW = w;
		baseH = h;
	}

	void sampleFBODirect(FBOSet *fbos, int offset = 0)
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

	void unsampleFBODirect(FBOSet *fbos, int offset = 0)
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



	void bindFBODirect(FBOSet *fbos, int doClear = 1)
	{
		setMatrices(fbos->width, fbos->height);
		fbos->bind(doClear);
		currentFBOResolutionX = fbos->width;
		currentFBOResolutionY = fbos->height;
	}

	////




	void sampleFBO(string fboName, int offset = 0, int swapFlag = -1)
	{
		FBOSet *fbos;

		if (swapFlag == -1)
		{
			fbos = fboMap[fboName];
		}
		else
		{

			if (swapFlag == 0)
			{
				fbos = fboMap[fboName + "0"];
			}
			else
			{
				fbos = fboMap[fboName + "1"];
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

	void unsampleFBO(string fboName, int offset = 0, int swapFlag = -1)
	{

		FBOSet *fbos;

		if (swapFlag == -1)
		{
			fbos = fboMap[fboName];
		}
		else
		{

			if (swapFlag == 0)
			{
				fbos = fboMap[fboName + "0"];
			}
			else
			{
				fbos = fboMap[fboName + "1"];
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

	FBOSet *getFBOSet (string fboName)
	{
		return fboMap[fboName];
	}

	FBOWrapper *getFBOWrapper (string fboName, int offset)
	{
		FBOSet *fbos = fboMap[fboName];
		return fbos->getFBOWrapper(offset);
	}


	void copyFBO(string src, string dest)
	{
		bindShader("CopyShader");
		bindFBO(dest);
		sampleFBO(src, 0);
		drawFSQuad(1.0f);
		unsampleFBO(src, 0);
		unbindFBO();
		unbindShader();
	}

	void copyFBO2(string src, string dest)
	{
		bindShader("CopyShader2");
		bindFBO(dest);
		sampleFBO(src, 0);
		drawFSQuad(1.0f);
		unsampleFBO(src, 0);
		unbindFBO();
		unbindShader();
	}

	void bindFBO(string fboName, int swapFlag = -1)
	{

		FBOSet *fbos;

		if (swapFlag == -1)
		{
			fbos = fboMap[fboName];
		}
		else
		{

			if (swapFlag == 0)
			{
				fbos = fboMap[fboName + "1"];
			}
			else
			{
				fbos = fboMap[fboName + "0"];
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

	void unbindFBO()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		setMatrices(baseW, baseH);

	}



	void bindShader(string shaderName)
	{

		if (shadersAreLoaded)
		{
			curShader = shaderName;
			curShaderPtr = shaderMap[curShader];
			curShaderPtr->bind();
		}

	}

	void unbindShader()
	{
		if (shadersAreLoaded)
		{
			curShaderPtr->unbind();
		}
	}

	void setShaderArrayfVec3(string paramName, float *x, int count)
	{
		curShaderPtr->setShaderArrayfVec3(paramName, x, count);
	}
	void setShaderArrayfVec4(string paramName, float *x, int count)
	{
		curShaderPtr->setShaderArrayfVec4(paramName, x, count);
	}

	void setShaderArray(string paramName, float *x, int count)
	{
		curShaderPtr->setShaderArray(paramName, x, count);
	}
	void setShaderFloat(string paramName, float x)
	{
		curShaderPtr->setShaderFloat(paramName, x);
	}
	void setShaderInt(string paramName, int x)
	{
		curShaderPtr->setShaderInt(paramName, x);
	}
	void setShaderfVec2(string paramName, FIVector4 *v)
	{
		curShaderPtr->setShaderfVec2(paramName, v);
	}
	void setShaderVec2(string paramName, float x, float y)
	{
		curShaderPtr->setShaderVec2(paramName, x, y);
	}
	void setShaderVec3(string paramName, float x, float y, float z)
	{
		curShaderPtr->setShaderVec3(paramName, x, y, z);
	}
	void setShaderfVec3(string paramName, FIVector4 *v)
	{
		curShaderPtr->setShaderfVec3(paramName, v);
	}

	void setShaderVec4(string paramName, float x, float y, float z, float w)
	{
		curShaderPtr->setShaderVec4(paramName, x, y, z, w);
	}
	void setShaderfVec4(string paramName, FIVector4 *v)
	{
		curShaderPtr->setShaderfVec4(paramName, v);
	}



	void setShaderFloatUB(string paramName, float x)
	{
		curShaderPtr->setShaderFloatUB(paramName, x);
	}
	void setShaderfVec4UB(string paramName, FIVector4 *v)
	{
		curShaderPtr->setShaderfVec4UB(paramName, v);
	}



	void updateUniformBlock(int ubIndex, int ubDataSize = -1)
	{
		curShaderPtr->updateUniformBlock(ubIndex, ubDataSize);
	}
	void invalidateUniformBlock(int ubIndex)
	{
		curShaderPtr->invalidateUniformBlock(ubIndex);
	}
	void beginUniformBlock(int ubIndex)
	{
		curShaderPtr->beginUniformBlock(ubIndex);
	}
	bool wasUpdatedUniformBlock(int ubIndex)
	{

		return curShaderPtr->wasUpdatedUniformBlock(ubIndex);

	}

	void setShaderTexture(int multitexNumber, uint texId)
	{
		if (shadersAreLoaded)
		{
			glActiveTexture(GL_TEXTURE0 + multitexNumber);
			glBindTexture(GL_TEXTURE_2D, texId);
			curShaderPtr->setShaderInt(shaderTextureIds[multitexNumber] , multitexNumber);
		}
	}

	void setShaderTexture3D(int multitexNumber, uint texId)
	{
		if (shadersAreLoaded)
		{
			glActiveTexture(GL_TEXTURE0 + multitexNumber);
			glBindTexture(GL_TEXTURE_3D, texId);
			curShaderPtr->setShaderInt(shaderTextureIds[multitexNumber], multitexNumber);
		}
	}


	bool shiftDown()
	{
		return (bool)(glutGetModifiers()&GLUT_ACTIVE_SHIFT);
	}

	bool ctrlDown()
	{
		return (bool)(glutGetModifiers()&GLUT_ACTIVE_CTRL);
	}

	bool altDown()
	{
		return (bool)(glutGetModifiers()&GLUT_ACTIVE_ALT);
	}


	void drawQuadBounds(float fx1, float fy1, float fx2, float fy2, float fz)
	{
		glColor4f(1, 1, 1, 1);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);

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

	void drawFSQuad(float zoom)
	{
		drawFSQuadOffset(0.0f, 0.0f, zoom);
	}

	void drawFSQuadOffset(float xOff, float yOff, float zoom)
	{
		float fx1 = (xOff - 1.0f) * zoom;
		float fy1 = (yOff - 1.0f) * zoom;
		float fx2 = (xOff + 1.0f) * zoom;
		float fy2 = (yOff + 1.0f) * zoom;

		glColor4f(1, 1, 1, 1);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);

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

	void drawFBO(string fboName, int ind, float zoom, int swapFlag = -1)
	{
		if (swapFlag == -1)
		{
			drawFBOOffset(fboName, ind, 0.0f, 0.0f, zoom);
		}
		else
		{
			if (swapFlag == 0)
			{
				drawFBOOffset(fboName + "1", ind, 0.0f, 0.0f, zoom);
			}
			else
			{
				drawFBOOffset(fboName + "0", ind, 0.0f, 0.0f, zoom);
			}

		}
	}

	void drawFBOOffsetDirect(FBOSet *fbos, int ind, float xOff, float yOff, float zoom)
	{

		glBindTexture(GL_TEXTURE_2D, fbos->fbos[ind].color_tex);
		//glClearColor(0.2,0.2,0.2,0.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawFSQuadOffset(xOff, yOff, zoom);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void drawFBOOffset(string fboName, int ind, float xOff, float yOff, float zoom)
	{
		FBOSet *fbos = fboMap[fboName];

		if (fbos)
		{
			drawFBOOffsetDirect(fbos, ind, xOff, yOff, zoom);
		}
		else
		{
			doTrace("drawFBOOffsetDirect: Invalid FBO Name");
		}

	}
	
	
	float getTerHeightScaled(float val)
	{
		return mixf(0.125f,0.875f,val)*worldSizeInPixels.getFZ();
		
	}


	float getSLNormalized()
	{
		return (((float)gw->seaLevel) / 255.0f);
	}
	float getSLInPixels()
	{
		return getSLNormalized()*worldSizeInPixels.getFZ();
	}

	float getHeightAtPixelPos(float x, float y, bool dd = false)
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

	void moveCamera(FIVector4 *pModXYZ)
	{
		
		int i;
		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);
		
		// cout << "curHeight: " <<
		// 	getHeightAtPixelPos(cameraPos->getFX(), cameraPos->getFY(), true)/worldSizeInPixels.getFZ() << "\n";
		
		
		wsBufferInvalid = true;

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



		for (i = 1; i < E_OBJ_LENGTH; i++)
		{

			if (dynObjects[i]->isRelative)
			{
				dynObjects[i]->pos.copyFrom(cameraPos);
				dynObjects[i]->pos.addXYZRef( &(dynObjects[i]->posRel) );
			}

		}

		isPanning = true;
	}

	void moveObject(float dx, float dy, float zoom)
	{


		float tilt = tiltAmount;
		float itilt = (1.0-tiltAmount);

		modXYZ.setFXYZ(0.0f,0.0f,0.0f);

		float dxZoom = dx * 1.0 / zoom;
		float dyZoom = dy * 1.0 / zoom;

		//float grassWidth;
		//float grassHeight;

		bool doDefault = false;
		
		
		
		
		float tileWidth = 1.0;
		float tileHeight = itilt;

		



		if (lbDown || rbDown)
		{
			// if (rbDown || (shiftDown() ) )
			// {
			// 	modXYZ.setFZ(  dyZoom );
			// 	modXYZ.setFX( -(0.0f + dxZoom / 2.0f) );
			// 	modXYZ.setFY( -(0.0f - dxZoom / 2.0f) );

			// }
			// else
			// {
			// 	modXYZ.setFX( -(dyZoom + dxZoom / 2.0f) );
			// 	modXYZ.setFY( -(dyZoom - dxZoom / 2.0f) );
			// }
			
			
			// screen to world
			
			if (rbDown || (shiftDown() ) )
			{
				modXYZ.setFZ(  dyZoom*1.0/tilt );
				modXYZ.setFX( -(0.0f + dxZoom) );
				modXYZ.setFY( -(0.0f - dxZoom) );

			}
			else
			{
				
				//iX = (((ScreenX) / tileWidth) - ((ScreenY) / tileHeight));
				//iY = (((ScreenX) / tileWidth) + ((ScreenY) / tileHeight));
				
				
				modXYZ.setFX( -(dyZoom*1.0/itilt + 1.0*dxZoom) );
				modXYZ.setFY( -(dyZoom*1.0/itilt - 1.0*dxZoom) );
				
			}
			

			lastModXYZ.addXYZRef(&modXYZ);




		}
		else
		{

		}



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
				
				if (rbDown) {
					tiltAmount -=  modXYZ.getFZ()*zoom / 500.0f;
					
					tiltAmount = clampf(tiltAmount, 0.125f, 0.75f);
					
					tiltChanged = true;
					changesMade = true;
				}
				
				
				
				// grassWidth = grassWH.getFX();
				// grassHeight = grassWH.getFY();


				// grassHeight -= modXYZ.getFZ()/10000.0f;
				// grassWidth -= modXYZ.getFX()/10000.0f;


				// if (grassHeight < 0.0f) {
				//  grassHeight = 0.0f;
				// }
				// if (grassWidth < 0.0f) {
				//  grassWidth = 0.0f;
				// }

				// grassWH.setFXYZ(grassWidth,grassHeight,0.0);

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
					dynObjects[activeObject]->posRel.addXYZRef(&modXYZ, -1.0f);
					dynObjects[activeObject]->pos.addXYZRef(&modXYZ, -1.0f);
					//activeObjectPos.setFXYZRef( &(dynObjects[activeObject]->pos) );
				}
			}

			if (doDefault)
			{
				moveCamera(&modXYZ);
			}



		}






	}

	void updateMultiLights()
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

	void toggleFullScreen()
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


	// void doAction(eProgramAction pa) {
	//  switch (pa) {
	//    case E_PA_QUIT:
	//      if (isFullScreen) {
	//        //glutLeaveGameMode();
	//      }
	//      PROG_ACTIVE = false;
	//      glutLeaveMainLoop();
	//    break;
	//    case E_PA_TOGGLE_FULLSCREEN:
	//      isFullScreen = !isFullScreen;

	//      if (isFullScreen) {
	//        //glutGameModeString( "1280x800:32@75" ); //the settings for fullscreen mode
	//        //glutEnterGameMode();
	//        //glutFullScreen();
	//      }
	//      else {

	//        //glutReshapeWindow(defaultWinW, defaultWinH);
	//        //glutLeaveGameMode();
	//      }

	//    break;
	//    case E_PA_REFRESH:
	//             doShaderRefresh();
	//    break;
	//    case E_PA_SIZE:
	//      //unused
	//    break;

	//  }
	// }






	/*
	GLUT_KEY_F1   F1 function key
	GLUT_KEY_F2   F2 function key
	GLUT_KEY_F3   F3 function key
	GLUT_KEY_F4   F4 function key
	GLUT_KEY_F5   F5 function key
	GLUT_KEY_F6   F6 function key
	GLUT_KEY_F7   F7 function key
	GLUT_KEY_F8   F8 function key
	GLUT_KEY_F9   F9 function key
	GLUT_KEY_F10    F10 function key
	GLUT_KEY_F11    F11 function key
	GLUT_KEY_F12    F12 function key
	GLUT_KEY_LEFT   Left function key
	GLUT_KEY_RIGHT    Right function key
	GLUT_KEY_UP   Up function key
	GLUT_KEY_DOWN   Down function key
	GLUT_KEY_PAGE_UP  Page Up function key
	GLUT_KEY_PAGE_DOWN  Page Down function key
	GLUT_KEY_HOME   Home function key
	GLUT_KEY_END    End function key
	GLUT_KEY_INSERT   Insert function key
	*/


	// void updateVG() {
	//  if (volGenFBOX < 4) {
	//    volGenFBOX = 4;
	//  }
	//  if (volGenFBOX > 128) {
	//    volGenFBOX = 128;
	//  }

	//  doTraceND("volGenFBOX: ", i__s(volGenFBOX));
	// }


	void setCameraToElevationBase() {
		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);

		cameraPos->setFZ(
			getHeightAtPixelPos(cameraPos->getFX(), cameraPos->getFY())
			// + maxWInPages * visPageSizeInPixels
			// + floorHeightInMeters*1.0f*pixelsPerMeter
		);

	}

	void setCameraToElevation()
	{
		setCameraToElevationBase();
		
		bufferInvalid = true;
		changesMade = true;
		wsBufferInvalid = true;
	}


	void processSpecialKeys(int key, int _x, int _y)
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
	void processKey(unsigned char key, int _x, int _y, bool isPressed)
	{

		int x = _x / scaleFactor;
		int y = _y / scaleFactor;
	}
	void keyboardUp(unsigned char key, int _x, int _y)
	{

		int x = _x / scaleFactor;
		int y = _y / scaleFactor;

		int holderCount;

		changesMade = false;


		int enCounter;

		bool restartGen = false;

		doTrace(i__s(key) );


		if (key == 17)
		{
			glutLeaveMainLoop();
		}

		switch (key)
		{


		case '0':
			activeMode = 0;

			break;
		case '1':
			activeMode = 1;
			//doTrace("Dirt and Grass");
			break;
		case '2':
			activeMode = 2;
			//doTrace("Rock");
			break;
		case '3':
			activeMode = 3;
			//doTrace("Brick");
			break;
		case '4':
			activeMode = 4;
			//doTrace("Flat Top Stone");
			break;
		case '5':
			activeMode = 5;
			//doTrace("Flat Top Dirt");
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

			break;

		case 19: //ctrl-s
			//saveAllData();
			cout << "Use s key in web editor to save\n";
			break;

		case 15: //ctrl-o
			loadAllData();

			break;

		case '[':
			iNumSteps /= 2;
			if (iNumSteps < 16)
			{
				iNumSteps = 16;
			}
			doTraceND("iNumSteps: ", i__s(iNumSteps));
			bufferInvalid = true;
			break;
		case ']':
			iNumSteps *= 2;
			if (iNumSteps > 256)
			{
				iNumSteps = 256;
			}
			doTraceND("iNumSteps: ", i__s(iNumSteps));
			bufferInvalid = true;
			break;


		case 'e':
			setCameraToElevation();
			break;

		case 'w':
			changesMade = true;
			maxWInPages++;
			break;
		case 'q':
			changesMade = true;
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
			radiosityOn = !radiosityOn;
			break;

		case 'r':
			doShaderRefresh();
			bufferInvalid = true;

			cout << "Shaders Refreshed\n";
			
			break;
			
		case 'j':
			doShaderRefresh();
			bufferInvalid = true;			
			mapInvalid = true;
			gw->initMap();
		break;

		case 'G':
			gridOn = 1.0 - gridOn;
			cout << "Grid On: " << gridOn << "\n";
			bufferInvalid = true;
			changesMade = true;
			break;

		case 'g':

			multiLights = !multiLights;
			updateMultiLights();


			bufferInvalid = true;
			changesMade = true;
			forceGetPD = true;

			// enCounter = (int)grassState;
			// enCounter++;
			// grassState = (E_GRASS_STATE)enCounter;
			// if (grassState == E_GRASS_STATE_LENGTH) {
			//  grassState = (E_GRASS_STATE)0;
			// }
			// bufferInvalid = true;
			// changesMade = true;
			break;


		case 'p':
			//cout << "curZoom " << cameraZoom << "\n";
			toggleFullScreen();
			break;

		case 'd':
			targetTimeOfDay += 0.5;
			
			if (targetTimeOfDay > 1.0) {
				targetTimeOfDay = 0.0;
			}
			
			break;

		case 'h':
			waterOn = !waterOn;

			if (MAX_LAYERS == 1)
			{
				waterOn = false;
			}


			bufferInvalid = true;
			changesMade = true;

			cout << "waterOn " << waterOn << "\n";
			break;

		case 'l':

			break;

		case 't':

			tiltAmount += 0.25f;
			
			if (tiltAmount > 0.75f) {
				tiltAmount = 0.25f;
			}
			
			createVTListTilt();
			
			cout << "tiltAmount: " << tiltAmount << "\n";
			
			restartGen = true;


			//testOn = !testOn;
			//cout << "testOn " << testOn << "\n";

			//treesOn = !treesOn;
			//traceOn = true;
			break;
		case 'o':
			rotOn = !rotOn;
			break;

		case '\t':


			enCounter = (int)mouseState;



			if (ctrlDown())
			{
				enCounter--;
			}
			else
			{
				enCounter++;
			}
			if (enCounter < 0)
			{
				enCounter = ((int)E_MOUSE_STATE_LENGTH) - 1;
			}

			mouseState = (E_MOUSE_STATE)enCounter;

			if (mouseState == E_MOUSE_STATE_LENGTH)
			{
				mouseState = (E_MOUSE_STATE)0;
			}

			bufferInvalid = true;
			changesMade = true;
			wsBufferInvalid = true;
			forceGetPD = true;


			break;

		case ' ':
			selGeomListInd++;
			if (selGeomListInd >= selGeomList.size()) {
				selGeomListInd = 0;
			}
			if (selGeomList.size() > 0) {
				selectedGeom = selGeomList[selGeomListInd];
			}
			
		break;

		case 'c':
			//isBare = !isBare;
			restartGen = true;
			break;

		case 'f':
			fogOn = 1.0 - fogOn;
			bufferInvalid = true;
			changesMade = true;
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

			cout << "zoom " << cameraZoom << "\n";

			reportPagesDrawn = true;
			//doTrace("Avail threads: ", i__s(gw->availThreads));
			break;

		case 'a':
			changesMade = true;
			maxHInPages++;
			break;
		case 'z':
			changesMade = true;
			maxHInPages--;
			if (maxHInPages < 0)
			{
				maxHInPages = 0;
			}
			break;


		case 'v':
			if (selectedGeom == NULL)
			{

			}
			else
			{
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

		if (restartGen)
		{
			doShaderRefresh();
			gw->actionOnHolders(E_HOLDER_ACTION_RESET);
			bufferInvalid = true;
			changesMade = true;
		}

	}
	void keyboardDown(unsigned char key, int _x, int _y)
	{


		int x = _x / scaleFactor;
		int y = _y / scaleFactor;
	}


	void getPixData(FIVector4 *toVector, int xv, int yv)
	{

		FBOWrapper *fbow;
		int newX;
		int newY;

		float newFX;
		float newFY;

		if (wsBufferInvalid)
		{
			gw->getWorldSpaceBuffer();
		}




		if (cameraZoom > 1.0)
		{
			newX = (int)(
				(  (((float)xv) - bufferDimHalf.getFX()) / cameraZoom) + bufferDimHalf.getFX()
			);
			newY = (int)(
				(  (((float)yv) - bufferDimHalf.getFY()) / cameraZoom) + bufferDimHalf.getFY()
				// * tiltAmount*2.0f
			);
		}
		else
		{
			newX = clamp(xv, 0, bufferDim.getIX() - 1);
			newY = clamp(yv, 0, bufferDim.getIY() - 1);
		}



		fbow = getFBOWrapper("worldSpaceFBO", 0);
		fbow->getPixelAtF(toVector, newX, (bufferDim.getIY() - 1) - newY);
	}

	GameGeom *findNearestGeom(FIVector4 *testPoint, bool createList = false)
	{
		GameBlock *curBlock;

		int i;
		int j;
		int k;
		int ii;
		int jj;
		
		int curInd = 0;
		float bestDis;
		float curDis;
		
		if (createList) {
			selGeomList.clear();
			selGeomListInd = 0;
		}

		bestDis = 99999.0f;
		GameGeom *resGeom = NULL;

		for (j = -1; j <= 1; j++)
		{
			for (i = -1; i <= 1; i++)
			{
				ii = i + gw->camBlockPos.getIX();
				jj = j + gw->camBlockPos.getIY();

				curBlock = gw->getBlockAtCoords(ii, jj, false);

				if (curBlock == NULL)
				{

				}
				else
				{
					for (k = 0; k < curBlock->gameGeom.size(); k++)
					{
						if (
							testPoint->inBoundsXYZSlack(
								curBlock->gameGeom[k]->getVisMinInPixelsT(),
								curBlock->gameGeom[k]->getVisMaxInPixelsT(),
								0.0625*pixelsPerMeter
							)
						)
						{
							
							if (createList) {
								selGeomList.push_back(curBlock->gameGeom[k]);
								curInd++;
							}
							
							curDis =
								curBlock->gameGeom[k]->getVisMinInPixelsT()->distance(testPoint) +
								curBlock->gameGeom[k]->getVisMaxInPixelsT()->distance(testPoint);

							if (curBlock->gameGeom[k]->visible) {
								
							}
							else {
								curDis *= 16.0f;
							}

							if (curDis < bestDis)
							{
								bestDis = curDis;
								resGeom = curBlock->gameGeom[k];
								selGeomListInd = curInd-1;
							}

						}
					}
				}
			}
		}

		return resGeom;
	}


	void mouseMove(int _x, int _y)
	{

		int x = _x / scaleFactor;
		int y = _y / scaleFactor;

		int dx = x - lastPosX;
		int dy = y - lastPosY;


		bShift = shiftDown();


		lastMouseX = x;
		lastMouseY = y;




		mouseXUp = x;
		mouseYUp = y;


		//mainGUI->testOver(_x, _y);



		if (lbDown || rbDown)
		{
			moveObject((float)dx, (float)dy, cameraZoom);
		}
		else
		{

			getPixData(&mouseMovePD, x, y);

			if (
				mouseState == E_MOUSE_STATE_BRUSH ||
				mouseState == E_MOUSE_STATE_PICKING ||
				mouseState == E_MOUSE_STATE_ENTS  )
			{

				gw->modifyUnit(&mouseMovePD, E_BRUSH_MOVE);
			}
			
			


			//////////////



			highlightedGeom = findNearestGeom(&mouseMovePD);


			//////////////


		}


		if (mbDown)
		{

		}
		lastPosX = x;
		lastPosY = y;

		if ( (x >= 0) && (y >= 0) && (x < baseW) && (y < baseH) )   // && (rbDown||lbDown||mbDown)
		{
			bufferInvalid = true;

			if (rbDown || lbDown)
			{
				changesMade = true;
			}
		}
	}


	void screenToWorldBase(FIVector4 *tc, FIVector4 *wc)
	{
		
	}


	void worldToScreenBase(FIVector4 *sc, FIVector4 *wc) {
		/*
		Screen -> Isometric space equations
		iX = (((ScreenX) / tileWidth) - ((ScreenY) / tileHeight));
		iY = (((ScreenX) / tileWidth) + ((ScreenY) / tileHeight));

		Isometric -> Screen space equations
		ScreenX = (iX + iY) * tileWidth / 2;
		ScreenY = (iY - iX) * tileHeight / 2;

		Assuming the isometric coordinating system goes as this: http://i.imgur.com/aUM4g.png

		*/
		
		float tilt = tiltAmount;
		float itilt = 1.0-tiltAmount;
		
		// float x1 = (wc->getFX() - wc->getFY())/2.0;
		// float y1 = (-wc->getFX() + -wc->getFY() + wc->getFZ());
		
				
		float x1 = (wc->getFX() - wc->getFY());
		//float y1 = (-(wc->getFX() / 2.0f) + -(wc->getFY() / 2.0f) + wc->getFZ());
		float y1 = (-itilt*wc->getFX() + -itilt*wc->getFY() + (tilt)*2.0f*wc->getFZ());
		
		sc->setFX(x1);
		sc->setFY(y1);
	}



	void worldToScreen(FIVector4 *sc, FIVector4 *wc, bool centerIsOrigin = false)
	{
		
		FIVector4 *cameraPos = &(dynObjects[E_OBJ_CAMERA]->pos);
		dMod.copyFrom(wc);
		dMod.addXYZRef(cameraPos, -1.0);
		
		worldToScreenBase(sc,&dMod);
		float x1 = sc->getFX();
		float y1 = sc->getFY();
		
		
		float myZoom = std::min(1.0f, cameraZoom);
		x1 = x1 * (myZoom) / bufferDim.getFX();
		y1 = y1 * (myZoom) / bufferDim.getFY();

		if (centerIsOrigin)
		{

		}
		else
		{
			x1 = (x1 + 1.0) / 2.0;
			y1 = (y1 + 1.0) / 2.0;
		}


		sc->setFX(x1);
		sc->setFY(y1);
	}

	

	void mouseClick(int button, int state, int _x, int _y)
	{


		int x = _x / scaleFactor;
		int y = _y / scaleFactor;

		lastPosX = x;
		lastPosY = y;

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

		bool hitGUI = false;//mainGUI->testHit(button, state, _x, _y);

		if (hitGUI)
		{
			bufferInvalid = true;
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
			//changesMade = true;
			break;

		case 4: // wheel down
			wheelDelta = -1.0f / 20.0f;
			//changesMade = true;
			break;
		}



		if (rbClicked)
		{
			
			if (tiltChanged) {
				tiltChanged = false;
				createVTListTilt();
				gw->actionOnHolders(E_HOLDER_ACTION_RESET);
				bufferInvalid = true;
				changesMade = true;
			}
			
			if (lbDown)
			{

			}
			else
			{
				isPanning = false;
			}

		}
		if (lbClicked)
		{
			
			
			
			if (rbDown)
			{

			}
			else
			{
				isPanning = false;
			}

		}





		if (state == GLUT_DOWN)
		{
			mouseVel.setFXY(0.0f, 0.0f);
		}

		if (mbClicked)
		{

		}


		if (rbDown || lbDown)
		{

		}
		else
		{

		}

		if (rbClicked || lbClicked)
		{




			if (rbDown || lbDown)
			{

			}
			else
			{

				muTime = myTimer.getElapsedTimeInMilliSec();

				mouseEnd.setIXY(x, y);
				//mouseVel.copyFrom(&mouseEnd);
				//mouseVel.addXYZRef(&mouseMoving, -1.0f);

				if ( (activeObject == E_OBJ_CAMERA) && (muTime - mdTime > 300.0f) )
				{
					mouseMovingStart = (mouseMovingLoc - min(mouseMovingStepsBack, mouseCount) + mouseMovingSize) % mouseMovingSize;


					if (mouseCount > 0)
					{
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

				if (shiftDown())
				{

				}
				else
				{




					activeObject = E_OBJ_CAMERA;
					wsBufferInvalid = true;
					getPixData(&mouseUpPD, x, y);


					selectedGeom = findNearestGeom(&mouseUpPD, true);

					if (
						(selectedGeom == NULL) ||
						(mouseState == E_MOUSE_STATE_PICKING)
					)
					{

					}
					else
					{

						switch (selectedGeom->buildingType)
						{
						case E_CT_DOOR:
						case E_CT_WINDOW:
						
						
							gw->getHoldersInGeom(selectedGeom);
							selectedGeom->applyTransform(selectedGeom->rotDir, false);
							gw->getHoldersInGeom(selectedGeom);
							gw->refreshHoldersInList(true); //holderCount <= 12
							gw->holdersToRefresh.clear();

							bufferInvalid = true;
							changesMade = true;
							wsBufferInvalid = true;
							break;

						case E_CT_LANTERN:
							selectedGeom->light->isOn = !(selectedGeom->light->isOn);
							doTraceND("");
							doTraceND("CUR LIGHT DIS ", f__s(selectedGeom->light->screenDistance));
							doTraceND("");

							for (i = 0; i < gw->lightCount; i++)
							{

								doTraceND("LIGHT DIS ", f__s(gw->activeLights[i]->screenDistance));

							}
							doTraceND("");
							doTraceND("");

							gw->updateLights();
							break;

						}

					}






					if ( mouseEnd.distance(&mouseStart) > 30.0 )
					{

					}
					else
					{
						
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
							case E_MOUSE_STATE_ENTS:
								if (lbClicked)
								{
									
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
								else
								{
									
								}
							break;
						}
						
						if (mouseState == E_MOUSE_STATE_BRUSH)
						{}
					}


					diskOn = 0.0f;
				}


			}


		}
		else
		{
			if (rbDown || lbDown)
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

					getPixData(&mouseDownPD, x, y);













					if (mouseDownPD.getIW() == 1)
					{

						

						// find nearest dyn object

						bestInd = 0;
						bestDis = 99999.0f;
						
						worldToScreenBase(&tempVec1, &mouseDownPD);
						
						for (i = 1; i < dynObjects.size(); i++)
						{


							worldToScreenBase(&tempVec2, &(dynObjects[i]->pos));

							curDis = tempVec1.distanceXY(&tempVec2);


							if (curDis < bestDis)
							{
								bestDis = curDis;
								bestInd = i;
							}
						}

						activeObject = (E_OBJ)(bestInd);


						diskOn = 1.0f;
						//activeObjectPos.setFXYZRef(&(dynObjects[activeObject]->pos));

					}
					else
					{
						
						activeObject = E_OBJ_CAMERA;
					}

					if (mouseState == E_MOUSE_STATE_BRUSH)
					{
						diskOn = 0.0;
					}


				}


			}
		}






		if (button == 3 || button == 4)
		{

			myDelta += wheelDelta;
			targetZoom = pow(2.0, myDelta);
			isZooming = true;

		}

		if (x >= 0 && y >= 0 && x < baseW && y < baseH)
		{
			bufferInvalid = true;
		}

	}


	void processB64(charArr *sourceBuffer, charArr *saveBuffer)
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


		FBOSet *fbos = fboMap["palFBO"];


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

	bool processJSON(charArr *sourceBuffer, charArr *saveBuffer)
	{

		doTraceND("processJSON()");

		char *buf = sourceBuffer->data;
		int len = sourceBuffer->size;
		JSONValue *jsonVal = NULL;

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

		doTraceND("Begin JSON::Parse()");

		if (buf == NULL)
		{
			doTraceND("buf is NULL");
			return false;
		}
		else
		{
			doTraceND("buf is not NULL");
			jsonVal = JSON::Parse(buf);
		}


		doTraceND("End JSON::Parse()");


		if (jsonVal == NULL)
		{
			doTraceND("Invalid JSON\n\n");
			return false;
		}
		else
		{
			doTraceND("");
			doTraceND("Valid JSON");
			doTraceND("");


			if (rootObj != NULL)
			{
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


	void loadAllData()
	{
		if ( loadFile("..\\data\\lastJSONBuffer.js", &lastJSONBuffer) )
		{
			processJSON(&lastJSONBuffer, &nullBuffer);
		}

		if ( loadFile("..\\data\\lastImageBuffer.txt", &lastImageBuffer) )
		{
			processB64(&lastImageBuffer, &nullBuffer);
		}

		bufferInvalid = true;
	}

	void saveAllData()
	{
		saveFile("..\\data\\lastJSONBuffer.js", &lastJSONBuffer);
		saveFile("..\\data\\lastImageBuffer.txt", &lastImageBuffer);
	}


	bool loadFile(char *fileName, charArr *dest)
	{
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

		dest->data = new char[size];

		// read content of infile
		infile.read (dest->data, size);

		if ( infile.bad() )
		{
			doTraceND("Could Not Load From File");
			infile.close();
			return false;
		}

		infile.close();

		doTraceND("Load Successful");

		return true;
	}

	bool saveFile(char *fileName, charArr *source)
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





	void display(void)
	{





		curTime = myTimer.getElapsedTimeInMilliSec();

		float elTime = curTime - lastTime;
		float dz;
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
					if ( processJSON( &(myWS->recBuffer), &lastJSONBuffer  ) )
					{
						saveAllData();

					}
				}
				else
				{
					processB64(  &(myWS->recBuffer), &lastImageBuffer  );

				}

				bufferInvalid = true;

				myWS->dataReady = false;
				myWS->isWorking = false;
			}
		}



		if (abs(timeOfDay - targetTimeOfDay) > 0.001)
		{
			bufferInvalid = true;
			changesMade = true;
		}


		if (elTime >= 8.0f) {

			if (firstRun)
			{

			}
			else
			{
				if ( (frameCount % 60) == 0)
				{
					gw->updateLights();
					bufferInvalid = true;
					changesMade = true;
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

			// if ( fMouseVel < 2.0f ) {
			//  mouseVel.setFXY(0.0f,0.0f);
			//  isPanning = false;
			// }
			// else {
			//  isPanning = true;


			//  panMod.copyFrom(&lastModXYZ);
			//  panMod.multXYZ(fMouseVel/16.0f);
			//  moveCamera(&panMod);
			// }


			dz = (targetZoom - cameraZoom) / (16.0f);

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

			//curMS = fpsTimer.getElapsedTimeInMilliSec() / renderCount;

			cameraZoom += dz;
			

			if ( ( abs(dz) / cameraZoom < 0.0001 ) && (isZooming))
			{
				isZooming = false;
				wsBufferInvalid = true;
				bufferInvalid = true;
				changesMade = true;
			}
			else
			{
				if (isZooming)
				{
					bufferInvalid = true;
					changesMade = true;
				}

			}
			

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
					gw->update();
					//renderCount += 1.0;
				}


				TRACE_ON = false;

				changesMade = false;
				bufferInvalid = false;
			}

			frameCount++;
		}

		if (firstRun)
		{
			//toggleFullScreen();
		}

		firstRun = false;

		//doTrace( "POSSIBLE ERROR: " , i__s(glGetError()) , "\n" );

	}
	void reshape(int w, int h)
	{

		setWH(w, h);

		screenWidth = w;
		screenHeight = h;

		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
		glMatrixMode(GL_MODELVIEW);
		orthographicProjection();


		setMatrices(baseW, baseH);
	}
	void idleFunc(void)
	{

	}

};
