
class Singleton
{
	
private:
	FIVector4* cameraPos;
	FIVector4 tempLerpPos;
	FIVector4 camLerpPos;
	FIVector4 resultCameraPos;
	FIVector4 targetCameraPos;
	FIVector4 baseCameraPos;
	
public:
	
	struct UIQuad
	{
		fBoundingBox hitBounds;
		CharStruct* cs;
		int fontId;
	};
	
	struct UICont {
		//UIComponent* uiComp;
		UIQuad bg;
		std::vector<UIQuad> charVec;
		//bool locked;
	};
	
	struct JSONStruct {
		JSONValue* jv;
	};
	struct UICStruct {
		int nodeId;
	};
	struct CompStruct {
		bool isValid;
		UIComponent* data;
	};
	
	DynBuffer* myDynBuffer;
	
	struct CompareStruct {
	    bool operator()(const string& first, const string& second) {
	        return first.compare(second) < 0;//first.size() < second.size();
	    }
	};

	CompareStruct compareStruct;
	
	
	typedef map<string, UICStruct>::iterator itUICStruct;
	typedef map<string, JSONStruct>::iterator itJSStruct;
	
	
	
	//unsigned long int totTimePassedGraphics;
	unsigned long int totTimePassedPhysics;
	
	
	
	bool keysPressed[MAX_KEYS];
	double keyDownTimes[MAX_KEYS];
	unsigned char keyMap[KEYMAP_LENGTH];
	bool keyMapResultZipped[KEYMAP_LENGTH];
	bool keyMapResultUnzipped[KEYMAP_LENGTH];
	int keyMapCoolDown[KEYMAP_LENGTH];
	int keyMapMaxCoolDown[KEYMAP_LENGTH];
	
	GLdouble viewMatrixD[16];
	float viewMatrixDI[16];
	GLdouble projMatrixD[16];
	
	Matrix4 identMatrix;
	Matrix4 viewMatrix;
	Matrix4 projMatrix;
	std::vector<Matrix4> objMatrixStack;
	Matrix4 curObjMatrix;
	Matrix4 curMVP;
	Matrix3 curObjMatrix3;
	Matrix4 tempObjMatrix;
	
	GLint viewport[4];

	E_OBJ activeObject;
	E_OBJ tempObj;
	
	eProgramState programState;
	eProgramAction progActionsDown[E_PS_SIZE * 256];
	eProgramAction progActionsUp[E_PS_SIZE * 256];

	EntSelection nearestLights;
	EntSelection highlightedEnts;
	EntSelection selectedEnts;
	GameEnt* selectedEnt;
	GameEnt* highlightedEnt;
	
	TBOWrapper limbTBO;
	float limbTBOData[MAX_LIMB_DATA_IN_BYTES];
	
	bool isWalking;
	bool isPressingMove;
	bool fxaaOn;
	bool doPathReport;
	bool refreshPaths;
	bool placingTemplate;
	bool smoothMove;
	bool waterBulletOn;
	bool ignoreFrameLimit;
	bool autoMove;
	bool allInit;
	bool noBounce;
	bool firstPerson;
	bool updateMatFlag;
	bool matVolLock;
	bool isMoving;
	bool perspectiveOn;
	bool lastPersp;
	bool isInteractiveEnt[E_CT_LENGTH];
	bool inputOn;
	bool pathfindingOn;
	bool placingGeom;
	bool isMacro;
	bool orgOn;
	bool cavesOn;
	bool bakeParamsOn;
	bool dragging;
	bool mouseMoved;
	bool hitGUI;
	bool draggingMap;
	bool guiDirty;
	bool mirrorOn;
	bool applyToChildren;
	bool bShiftOld;
	bool bCtrlOld;
	bool bCtrl;
	bool bShift;
	bool testOn;
	bool testOn2;
	bool emptyVDNotReady;
	bool radiosityOn;
	bool updateLock;
	bool isFullScreen;
	bool mapInvalid;
	bool wsBufferInvalid;
	bool forceGetPD;
	bool mouseLeftDown;
	bool mouseRightDown;
	bool pboSupported;
	bool notQuit;
	bool timerNotSet;
	bool lbDown;
	bool abDown; // lb, rb, or mb is down
	bool rbDown;
	bool mbDown;
	bool isBare;
	bool showMap;
	bool traceOn;
	bool waterOn;
	bool treesOn;
	bool firstRun;
	bool rotOn;
	bool doPageRender;
	bool markerFound;
	bool editPose;
	bool isDraggingObject;
	bool updateHolders;
	bool fpsTest;
	bool frameMouseMove;
	bool depthInvalidRotate;
	bool depthInvalidMove;
	bool lastDepthInvalidMove;
	bool drawOrient;
	
	
	int entIdToIcon[MAX_OBJ_TYPES];
	int iconToEntId[MAX_ICON_ID];
	bool isContainer[MAX_OBJ_TYPES];
	string objStrings[MAX_OBJ_TYPES];
	
	int highlightedLimb;
	int highlightedLimb2;
	
	int curPrimTemplate;
	int geomStep;
	int earthMod;
	int currentTick;
	
	int draggingFromInd;
	int draggingToInd;
	int draggingFromType;
	int draggingToType;
	
	int currentPose;
	int actorCount;
	int limbDataDebug;
	int polyCount;
	int fdWritePos;
	int fdReadPos;
	int fpsCountMax;
	int fpsCount;
	int medianCount;
	int maxHolderDis;
	int gameObjCounter;
	int lastObjectCount;
	int lastObjInd;
	int selObjInd;
	int actObjInd;
	int fieldCallback;
	int mouseState;
	int lastW;
	int lastH;
	int maxLayerOver;
	int pathFindingStep;
	int baseW;
	int baseH;
	int scaleFactor;
	int numDynLights;
	int iNumSteps;
	int curOrgId;
	int extraRad;
	int defaultWinW;
	int defaultWinH;
	int guiWinW;
	int guiWinH;
	int shadersAreLoaded;
	int readyToRecompile;
	int lastPosX;
	int lastPosY;
	int frameCount;
	int screenWidth;
	int screenHeight;
	int mouseMovingSize;
	int mouseMovingLoc;
	int mouseMovingStepsBack;
	int mouseCount;
	int lastMouseX;
	int lastMouseY;
	int cellsPerNodeXY;
	int terDataVisPitchXY;
	int terDataBufPitchXY;
	int terDataBufPitchScaledXY;
	int cellsPerNodeZ;
	int terDataVisPitchZ;
	int terDataBufPitchZ;
	int terDataBufPitchScaledZ;
	int terDataBufAmount;
	int terDataVisSize;
	int terDataBufSize;
	int terDataTexScale;
	int terDataBufSizeScaled;
	int iNodeDivsPerHolder;
	int matVolSize;
	int escCount;
	int mapPitch;
	int lastNodeId;
	int diagCount;
	int frameSkip;
	int frameSkipCount;
	
	
	
	
	
	int cellsPerHolder;
	int cellsPerBlock;
	int holdersPerBlock;
	
	int cellsPerWorld;
	int holdersPerWorld;
	int blocksPerWorld;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	intPair entIdArr[1024];
	
	
	
	
	
	uint palWidth;
	uint palHeight;
	uint blockShift;
	uint *terDataScaled;


	GLfloat camRotation[2];
	GLfloat curCamRotation[2];

	bool timeMod;
	
	uint naUintData[8];
	int naIntData[8];
	float naFloatData[8];
	float conVals[E_CONST_LENGTH];
	
	float lastMouseOrigX;
	float lastMouseOrigY;
	float globWheelDelta;
	float amountInvalidMove;
	float amountInvalidRotate;
	float sphereMapPrec;
	float heightMapMaxInCells;
	float resultShake;
	float cameraShake;
	float subjectDistance;
	float lastSubjectDistance;
	float lastx;
	float lasty;
	float FOV;
	float focalLength;
	float zoomDelta;
	float subjectDelta;
	float subjectZoom;
	float targetSubjectZoom;
	float cameraZoom;
	float targetZoom;
	float fogOn;
	float gridOn;
	float mapSampScale;
	float curBrushRad;
	float timeOfDay;
	float targetTimeOfDay;
	float origWinW;
	float origWinH;
	float guiX;
	float guiY;
	float aspectRatio;
	float currentFBOResolutionX;
	float currentFBOResolutionY;
	float curPrimMod;
	float mouseX;
	float mouseY;
	float mouseXUp;
	float mouseYUp;
	float holderSizeMB;
	float bestNodeDis;
	float heightOfNearPlane;
	float scrollDiv;
	float curMoveSpeed;
	float curMoveAccel;
	float masterVolume;
	float ambientVolume;
	float guiVolume;
	float musicVolume;
	float fxVolume;
	float *paramArr;
	float *paramArrGeom;
	float *splashArr;
	float *explodeArr;
	float *voroArr;
	float *matCountArr;
	float *paramArrMap;
	float clipDist[2];
	
	float MAX_TRAVEL_DIS;
	
	//double curMoveTime;
	//double lastMoveTime;
	double timeDelta;
	double curTime;
	float smoothTime;
	double pauseTime;
	double clickTime;
	//double lastTime;
	double mdTime;
	double muTime;
	
	
	
	GameOrgNode* bestNode;
	GameOrgNode* selectedNode;
	GameOrgNode* lastSelNode;
	GameOrgNode* activeNode;

	FIVector4 geomPoints[E_GEOM_POINTS_LENGTH];

	

	FIVector4 colVecs[16];
		
	FIVector4 geomOrigOffset;
	FIVector4 lastSend;
	
	
	
	
	
	
	
	
	FIVector4 lastHolderPos;
	FIVector4 lightVec;
	FIVector4 lightVecOrig;
	FIVector4 dirVecs[6];
	FIVector4 lastCellPos;
	FIVector4 worldMarker;
	FIVector4 lookAtVec;
	FIVector4 lookAtVec2D;
	FIVector4 baseScrollPos;
	FIVector4 mouseUpPD;
	FIVector4 mouseUpOPD;
	FIVector4 spaceUpPD;
	FIVector4 mouseDownPD;
	FIVector4 mouseDownOPD;
	FIVector4 mouseMovePD;
	FIVector4 mouseMoveOPD;
	FIVector4 tempVec1;
	FIVector4 tempVec2;
	FIVector4 tempVec3;
	
	
	
	
	
	FIVector4 moveNodes[2];
	FIVector4 voroVecArr[125];
	floatAndIndex indexArr[125];
	FIVector4 mouseStart;
	FIVector4 mouseEnd;
	FIVector4 mouseMoveVec;
	FIVector4 mapFreqs;
	FIVector4 mapAmps;
	FIVector4 *mouseMoving;
	
	FIVector4 bufferDim;
	FIVector4 bufferDimTarg;
	FIVector4 bufferDimHalf;
	FIVector4 bufferModDim;
	FIVector4 bufferRenderDim;
	FIVector4 origin;
	FIVector4 panMod;
	FIVector4 dMod;
	FIVector4 modXYZ;
	FIVector4 matVolDim;
	
	uint* matVol;
	
	
	
	ThreadWrapper threadNetSend;
	ThreadWrapper threadNetRecv;
	
	std::list<KeyStackEvent> keyStack;
	
	EntPool entPoolStack[E_ENTTYPE_LENGTH];
	
	std::vector<GameActor*> gameActors;
	std::vector<GameOrg*> gameOrgs;
	std::vector<GameOrg*> gamePoses;
	
	std::vector<ExplodeStruct> explodeStack;
	std::vector<DebrisStruct> debrisStack;
	//std::vector<btRigidBody*> debrisBodies;
	
	std::vector<FIVector4> primTemplateStack;
	std::vector<SphereStruct> sphereStack;
		
	std::vector<int> guiLayers[MAX_UI_LAYERS];
	//std::vector<int> emptyLayers[MAX_UI_LAYERS];
	
	std::vector<RotationInfo> rotMatStack;
	std::vector<DynObject *> dynObjects;
	// PathHolder charPathHolder;
	// PathHolder splitPathHolder;

	VBOWrapper fsQuad;
	
	

	float floorHeightInCells;
	float roofHeightInCells;
	float wallRadInCells;

	
	
	

	Image *imageHM0;
	Image *imageHM1;
	Image *cloudImage;

	BaseObj* currentActor;
	
	GamePageHolder* closestHolder;
	
	GamePlant* gamePlants[E_PT_LENGTH/2];

	Shader *curShaderPtr;
	string currentFieldString;
	string curShader;
	string allText;
	string stringBuf;
	string curCallback;
	string cbDataStrings[10];
	
	string guiSaveLoc;

	
	VolumeWrapper* volumeWrappers[E_VW_LENGTH];
	
	vector<CompStruct> compStack;
	vector<int> emptyStack;
	
	
	
	vector<string> splitStrings;
	vector<string> shaderStrings;
	vector<string> shaderTextureIds;
	map<string, Shader*> shaderMap;
	map<string, UICStruct> compMap;
	map<string, string> includeMap;
	map<string, FBOSet> fboMap;
	
	
	
	// FBOSet vgFBOArr[MAX_VG_FBOS];
	// FBOSet vgtFBOArr[MAX_VGT_FBOS];

	
	
	
	
	
	GLuint fsqDL;

	// GLuint volId;
	// GLuint volIdLinear;
	// GLuint volIdEmpty;
	// GLuint volIdEmptyLinear;
	
	GLuint volIdMat;
	
	
	

	GLuint volGenId;
	uint *lookup2to3;
	unsigned char *resultImage;
	
	materialNode* matSlice0;
	materialNode* matSlice1;
	materialNode* matSlice2;

	charArr nullBuffer;
	charArr lastImageBuffer;
	//charArr lastJSONBuffer;
	charArr lastJSONBufferGUI;

	
	JSONValue *rootObjJS;
	JSONValue *guiRootJS;
	JSONValue *constRootJS;

	// #ifdef USE_POCO
	// 	WebSocketServer *myWS;
	// #endif
	
	HPClock bulletTimer;
	Timer fpsTimer;
	Timer shakeTimer;
	Timer myTimer;
	Timer scrollTimer;
	Timer moveTimer;
	GameWorld* gw;
	GamePhysics* gamePhysics;
	GameFluid* gameFluid[E_FID_LENGTH];
	GameLogic* gameLogic;
	GameNetwork* gameNetwork;
	GameAI* gameAI;

	float lightArr[MAX_LIGHTS * 16];
	int numLights;


	bool multiLights;

	int *rbStack;
	int *rbHeightStack;


	TerTexture terTextures[MAX_TER_TEX];



	
	GameGUI* mainGUI;
	
	UIComponent* mapComp;
	UIComponent* mainMenu;
	UIComponent* contMenu;
	UIComponent* contMenuBar;
	UIComponent* ddMenu;
	UIComponent* ddMenuBar;
	UIComponent* fieldMenu;
	UIComponent* fieldText;
	
	FontWrapper* fontWrappers[EFW_LENGTH];
	
	GameMusic* music[EML_LENGTH];
	map<string, GameSound> soundMap;
	
	map<string, StyleSheet> styleSheetMap;
	map<string, JSONStruct> externalJSON;

	Singleton()
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

	FIVector4 btvConv;
	FIVector4* BTV2FIV(btVector3 btv) {
		btvConv.setFXYZ(btv.getX(),btv.getY(),btv.getZ());
		return &btvConv;
	}
	
	void setSelInd(int ind) {
		
		selObjInd = ind;
	}

	void init(int _defaultWinW, int _defaultWinH, int _scaleFactor
	
		// #ifdef USE_POCO
		// 	, WebSocketServer *_myWS
		// #endif	
		
		
	) {

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
		externalJSON["shaderParams"].jv = NULL;
		externalJSON["objectData"].jv = NULL;
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
		
		
		initAllObjects();
		initAllMatrices();
		
		fsQuad.init(vertexDataQuad, 32, indexDataQuad, 6);
		
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
		totTimePassedPhysics = 0;
		
		identMatrix.identity();
		
		isWalking = false;
		isPressingMove = false;
		fxaaOn = false;
		doPathReport = false;
		refreshPaths = false;
		placingTemplate = true;
		smoothMove = false;
		waterBulletOn = false;
		ignoreFrameLimit = false;
		autoMove = false;
		inputOn = false;
		isMacro = false;
		orgOn = false;
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
		
		currentActor = NULL;
		mapComp = NULL;
		mainMenu = NULL;
		ddMenu = NULL;
		contMenu = NULL;
		fieldMenu = NULL;
		fieldText = NULL;
		selectedEnt = NULL;
		highlightedEnt = NULL;
		draggingMap = false;
		
		MAX_TRAVEL_DIS = 10.0f;
		FOV = 45.0f;
		focalLength = 1.0f / tan(FOV / 2.0f);

		totalPointCount = 0;
		
		activeNode = NULL;
		selectedNode = NULL;
		lastSelNode = NULL;
		
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

		
		limbTBO.init(limbTBOData,MAX_LIMB_DATA_IN_BYTES);
		
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
		highlightedLimb = -1;

		drawOrient = false;
		noBounce = true;
		firstPerson = false;
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
		isDraggingObject = false;
		editPose = false;
		doPageRender = true;
		placingGeom = false;
		
		
		
		
		
		/////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////
		
				
		// IMPORTANT: Maximum height must be less than 2^16, max world pitch must be less than 2^32
		
		medianCount = 0;
		lastObjectCount = 0;
		lastObjInd = 0;
		setSelInd(0);
		actObjInd = 0;
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
		
		
		
		// qqqqqq
		
		// was doing: lerp from start to end postions, worlspace per pixel
		
		// was doing: exmaine why fluidchange 66 33
		
		// was doing: silouhette rendering?
		
		// todo: compress volumes into stacks of lines
		// todo: rasterize voxels with tris for depth cache
		// todo - generate silouhette by expanding position of edges with high differential
		// map right in the center of these edges to inexpensively determine silouhette
		
		// todo: probe area ahead of current ray step to see if near edge
		
		currentPose = E_PK_T_POSE;
		limbDataDebug = 0;
		actorCount = 0;
		polyCount = 0;
		fpsCountMax = 500;
		
		fpsTest = false;
		pathfindingOn = false;
		updateHolders = true;
		
		
		maxHolderDis = 32;
		heightOfNearPlane = 1.0f;
		scrollDiv = 2.0;
		frameSkip = 32;
		frameSkipCount = 0;
		
		int bufferDiv = 1;
		
		if (DO_POINTS) {
			glEnable(GL_POINT_SPRITE);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		}
		
		glLineWidth(4.0f);
		
		// This var determines how high the z val can go,
		// but also increases load times for each block
		
		
		
		heightMapMaxInCells = 8192.0f;
		mapSampScale = 2.0f;
		int newPitch = (imageHM0->width) * mapSampScale; //*2;
		mapPitch = (imageHM0->width)*0.5; //newPitch;// //
		
		cellsPerHolder = 32;
		holdersPerBlock = 8;
		
		holdersPerWorld = newPitch;
		cellsPerWorld = holdersPerWorld*cellsPerHolder;
		cellsPerBlock = holdersPerBlock * cellsPerHolder;
		blocksPerWorld = holdersPerWorld/holdersPerBlock;
		
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


		
		draggingFromInd = 0;
		draggingToInd = 0;
		gameObjCounter = E_OBJ_LENGTH;
		//curMoveTime = 0.0;
		//lastMoveTime = 0.0;
		timeDelta = 0.0;

		
		
		
		
		
		


		
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
			12.0f/16.0f,
			2.0f/16.0f,
			0.5f/16.0f,
			0.125f/16.0f
		); //0.0f, 0.0f, 0.0f);//


		


		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		radiosityOn = true;
		testOn = false;
		testOn2 = false;
		updateLock = false;
		traceOn = false;
		frameMouseMove = false;
		depthInvalidMove = true;
		lastDepthInvalidMove = true;
		depthInvalidRotate = true;
		gridOn = 0.0f;
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
		lastSubjectDistance = 0.0f;
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
		keyMap[KEYMAP_FIRE_PRIMARY] = ' ';
		keyMapMaxCoolDown[KEYMAP_FIRE_PRIMARY] = 20;
		keyMap[KEYMAP_GRAB_THROW] = 'w';
		keyMapMaxCoolDown[KEYMAP_GRAB_THROW] = 200;
		
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
		
		for (i = 0; i < E_VW_LENGTH; i++) {
			
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
					tz = blocksPerWorld;
					clampType = GL_REPEAT; //GL_CLAMP_TO_BORDER
					if (!GEN_POLYS_WORLD) {
						doProc = false;
					}
				break;
				// case E_VW_TERGEN:
				// 	tz = 128;
				// 	clampType = GL_CLAMP_TO_EDGE;//GL_CLAMP_TO_BORDER
				// break;
				case E_VW_VORO:
					tz = 256;
					clampType = GL_REPEAT;
				break;
			}
			
			if (doProc) {
				volumeWrappers[i] = new VolumeWrapper();
				volumeWrappers[i]->init(tz, clampType, (vwChan==4) ); //volumeWrapperStrings[i]
				//fboMap[volumeWrapperStrings[i]].init(1, tx, ty, vwChan, false);
			}
			
		}
		
		
		
		
		fboMap["prelightFBO"].init(4, bufferDimTarg.getIX(), bufferDimTarg.getIY(), 1, false, GL_LINEAR);
		
		// fboMap["allTargFBO"].init(6, bufferRenderDim.getIX(), bufferRenderDim.getIY(), numChannels, fboHasDepth);
		// fboMap["allDepthFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);
		
		
		fboMap["prmTargFBO"].init(8, bufferRenderDim.getIX(), bufferRenderDim.getIY(), numChannels, fboHasDepth);
		fboMap["prmDepthFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);
		
		fboMap["terTargFBO"].init(8, bufferRenderDim.getIX(), bufferRenderDim.getIY(), numChannels, fboHasDepth);
		fboMap["limbFBO"].init(1, bufferRenderDim.getIX(), bufferRenderDim.getIY(), numChannels, fboHasDepth);
		fboMap["terDepthFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);
		
		for (i = 0; i <= NUM_POLY_STRINGS; i++) {
			fboMap[polyFBOStrings[i]].init(1, bufferRenderDim.getIX(), bufferRenderDim.getIY(), 4, true);
		}
		
		
		
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
		
		
		
		
		// fboMap["noiseFBO"].init(1, 1024, 1024, 1, false, GL_NEAREST, GL_REPEAT);
		// fboMap["noiseFBOLinear"].init(1, 1024, 1024, 1, false, GL_LINEAR, GL_REPEAT);
		
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


		gw = new GameWorld();
		
		
		
		
		//TODO: fix this for proper angle alignment to model
		//orientRotation();
		
		
		gw->init(this);
		gw->initMap();
		
		
		
		//bulletTest();
		
		gameAI = new GameAI();
		gameAI->init(this);
		
		
		// gw->gameObjects[E_OBJ_CAMERA] = BaseObj();
		// gw->gameObjects[E_OBJ_CAMERA].init(
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
		
		
		for (i = 0; i < E_ENTTYPE_LENGTH; i++) {
			entPoolStack[i].curIndex = 0;
			
			switch (i) {
				case E_ENTTYPE_BULLET:
					k = 20;
				break;
				case E_ENTTYPE_TRACE:
					k = 0;
				break;
				case E_ENTTYPE_DEBRIS:
					k = 400;
				break;
				default:
					k = 0;
				break;
				
			}

			entPoolStack[i].maxCount = k;
			
			for (j = 0; j < entPoolStack[i].maxCount; j++) {
				placeNewEnt(false, i, &origin, true);
			}
		}
		
		
		for (i = 0; i < E_PK_LENGTH; i++) {
			gamePoses.push_back(new GameOrg());
			gamePoses.back()->init(this,-1,E_ORGTYPE_HUMAN);
			gamePoses.back()->loadFromFile(poseStrings[i], false);
			transformOrg(gamePoses.back());
		}
		

		allInit = true;
		
		doTraceND("GW DONE");


		popTrace();



	}
	
	
	
	
	
	
	
	// void orientRotation() {
	// 	//testHuman->baseNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(0.0f,0.0f,3.0f*M_PI/2.0f);
	// }
	
	
	
	int placeInStack() {
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
	
	int placeInLayer(int nodeId, int layer) {
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
	
	
	void initAllMatrices() {
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
	
	int numberIcons(int pCurCount, int x1, int y1, int x2, int y2) {
	  int i;
	  int j;
	  int curCount = pCurCount;
	  
	  for (j = y1; j <= y2; j++) {
	    for (i = x1; i <= x2; i++) {
				if (curCount >= MAX_OBJ_TYPES) {
					cout << "error curCount " << curCount << "\n";
				}
				else {
					entIdToIcon[curCount] = i + j * ITEMS_PER_ROW;
				}
	    	
	    	if ((i + j * ITEMS_PER_ROW) >= MAX_ICON_ID) {
	    		cout << "error i + j * ITEMS_PER_ROW "  << (i + j * ITEMS_PER_ROW) << "\n";
	    	}
	    	else {
	    		iconToEntId[i + j * ITEMS_PER_ROW] = curCount;
	    	}
	      
	      
	      curCount++;
	    }
	  }
	  
	  
	  return curCount;
	  
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	void funcNT2() {
		threadNetRecv.setRunningLocked(true);
		gameNetwork->updateRecv();
		threadNetRecv.setRunningLocked(false);
	}
	void startNT2() {
		if (threadNetRecv.threadRunning) {
			
		}
		else {
			threadNetRecv.threadRunning = true;
			threadNetRecv.threadMain = std::thread(&Singleton::funcNT2, this);
		}
	}	
	bool stopNT2() {
		bool didStop = false;
		if (threadNetRecv.threadRunning) {
			threadNetRecv.threadMain.join();
			threadNetRecv.threadRunning = false;
			didStop = true;
		}
		return didStop;
	}
	
	
	void funcNT() {
		threadNetSend.setRunningLocked(true);
		gameNetwork->updateSend();
		threadNetSend.setRunningLocked(false);
	}
	void startNT() {
		if (threadNetSend.threadRunning) {
			
		}
		else {
			threadNetSend.threadRunning = true;
			threadNetSend.threadMain = std::thread(&Singleton::funcNT, this);
		}
	}	
	bool stopNT() {
		bool didStop = false;
		
		if (threadNetSend.threadRunning) {
			threadNetSend.threadMain.join();
			threadNetSend.threadRunning = false;
			didStop = true;
		}
		return didStop;
		
	}
	
	
	
	
	
	
	
	void prepSound(string soundName) {
		if (soundMap.find( soundName ) == soundMap.end()) {
			soundMap.insert( pair<string, GameSound>(soundName, GameSound()) );
			soundMap[soundName].init("..\\data\\sounds\\"+soundName+".wav");
		}
	}
	
	void playSoundEnt(
		string soundName,
		BaseObj* ge = NULL,
		float variance=0.0f,
		float volume=1.0f,
		bool doLoop=false
	) {
		
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
				playSoundPosAndPitch(soundName,cameraGetPosNoShake(),BTV2FIV(ge->getCenterPoint(0)),variance,volume,doLoop);
			}
		}
		
		
		
		
	}
	
	void playSoundPosAndPitch(
		string soundName,
		FIVector4* listenerPos,
		FIVector4* soundPos,
		float variance=0.0f,
		float volume=1.0f,
		bool doLoop=false
	) {
		
		
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
	
	void updateSoundPosAndPitch(
		string soundName,
		FIVector4* listenerPos,
		FIVector4* soundPos,
		float volume=1.0f,
		float decay=0.01f
	) {
		
		
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
	
	

	void playSound(string soundName, float volume=1.0f) {
		prepSound(soundName);
		soundMap[soundName].play(volume);
	}

	void playSoundEvent(const char* eventName, bool suppress = false) {
		
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
	
	void setCurrentActor(BaseObj* ge) {
		
		
		currentActor = ge;
		if (currentActor == NULL) {
			actObjInd = 0;
			setFirstPerson(false);
		}
		else {
			
			
			actObjInd = ge->uid;
			
			cout << "actObjInd " << actObjInd << "\n";
			
			subjectDistance = BTV2FIV(currentActor->getCenterPoint(0))->distance(cameraGetPosNoShake());
			
			cout << "subjectDistance " << subjectDistance << "\n"; 
		}
		
	}
	
	void updateMatVol() {
		
		
		
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
	
	int getRandomContId() {
		return iGenRand2(360,419);
	}
	int getRandomNPCId() {
		return (iGenRand2(1432,1671)/4)*4;
	}
	int getRandomMonsterId() {
		return (iGenRand2(1240,1431)/2)*2;
	}
	int getRandomObjId() {
		return iGenRand2(0,907);
		
	}
	
	void fillWithRandomObjects(int parentUID, int gen) {
		
		int i;
		int maxObj = iGenRand2(2,16);
		BaseObj* tmpObj = NULL;
		
		int curId;
		
		
		for (i = 0; i < maxObj; i++) {
			gw->gameObjects[gameObjCounter] = BaseObj();
			tmpObj = &(gw->gameObjects[gameObjCounter]);
			
			curId = getRandomObjId();
			
			
			// if (
			// 	(curId%5 == 0) &&
			// 	(gen <= 1)
			// ) {
				
			// 	curId = getRandomContId();
			// }
			
			tmpObj->init(
				gameObjCounter,
				parentUID,
				curId,
				E_ENTTYPE_OBJ,
				&lastCellPos
			);
			
			gw->gameObjects[parentUID].children.push_back(gameObjCounter);
			
			gameObjCounter++;
			
			if (isContainer[curId] && (gen < 1)) {
				fillWithRandomObjects(gameObjCounter-1, gen + 1);
			}
			
		}
		
	}
	
	
	
	void toggleDDMenu(int x, int y, bool toggled) {
		
		if (placingGeom) {
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
				setSelInd(ind);				
			}
			else {
				getMarkerPos(x, y);
				markerFound = true;
				setSelInd(0);
			}
			
			
			actOnSel = 
				objTargeted &&
				(selObjInd != actObjInd) &&
				(actObjInd >= E_OBJ_LENGTH);
			
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
					tempComp->setText(objStrings[gw->gameObjects[selObjInd].objectType ]);
				}
				else {
					tempComp->setText("Context Menu");
				}
				
			}
			
			ddMenu->isDirty = true;
			ddMenu->visible = true;
			ddMenuBar = ddMenu->getChild(0)->getChild(0);
			ddMenuBar->lastDrag.x = (guiX);
			ddMenuBar->lastDrag.y = min((float)(guiY), (float)( (guiWinH - ddMenu->getChild(0)->resultDimInPixels.y) ));
			ddMenuBar->forceDragUpdate = true;
		}
		else {
			ddMenu->visible = false;
			markerFound = false;
		}
	}
	
	BaseObj* getEquipped(BaseObj* parentObj) {
		int i;
		
		int curChild;
		
		for (i = 0; i < parentObj->children.size();i++) {
			curChild = parentObj->children[i];
			if (gw->gameObjects[curChild].isEquipped) {
				return &(gw->gameObjects[curChild]);
			}
		}
		
		return NULL;
	}
	
	void performDrag(
		bool isReq,
		
		int _draggingFromInd,
		int _draggingFromType,
		int _draggingToInd,
		int _draggingToType,
		
		FIVector4* _worldMarker
	) {
		
		int i;
		int bestPos;
		bool moveCont = false;
		bool draggedIntoWorldObj = false;
		BaseObj* sourceObj = NULL;
		BaseObj* destObj = NULL;
		
		vector<BaseObjType>* myVec;
		
		if (isReq) {
			naFloatData[0] = _worldMarker->getFX();
			naFloatData[1] = _worldMarker->getFY();
			naFloatData[2] = _worldMarker->getFZ();
			naIntData[0] = _draggingFromInd;
			naIntData[1] = _draggingFromType;
			naIntData[2] = _draggingToInd;
			naIntData[3] = _draggingToType;
			gameNetwork->addNetworkAction(E_NO_DRAG_ENT,naUintData,naIntData,naFloatData);
			return;
		}
		
		
		cout << "from " << dragStrings[_draggingFromType] << " to " << dragStrings[_draggingToType] << "\n";
		
		switch (_draggingFromType) {
			case E_DT_NOTHING:
				
			break;
			case E_DT_WORLD_OBJECT:
			
				sourceObj = &(gw->gameObjects[_draggingFromInd]);
			
				switch (_draggingToType) {
					case E_DT_NOTHING:
						
						// lastCellPos.copyFrom(_worldMarker);
						// lastCellPos.addXYZ(0,0,5);
						// sourceObj->setCenterPoint(&lastCellPos);
						
						
					break;
					case E_DT_WORLD_OBJECT:
					case E_DT_INV_OBJECT:
					case E_DT_INV_OBJECT_PARENT:
						destObj = &(gw->gameObjects[_draggingToInd]);
						moveCont = true;
					break;
				}
			break;
			
			case E_DT_INV_OBJECT:
			
				sourceObj = &(gw->gameObjects[_draggingFromInd]);
			
				switch (_draggingToType) {
					case E_DT_NOTHING:
						
						
						lastCellPos.copyFrom(_worldMarker);
						lastCellPos.addXYZ(0,0,5);
						sourceObj->startPoint = lastCellPos.getBTV();
						
						
						gw->gameObjects[sourceObj->parentUID].removeChild(sourceObj->uid);
						gw->addVisObject(sourceObj->uid, false);
						sourceObj->parentUID = 0;
						
						
					break;
					case E_DT_WORLD_OBJECT:
					case E_DT_INV_OBJECT:
					case E_DT_INV_OBJECT_PARENT:
						destObj = &(gw->gameObjects[_draggingToInd]);
						moveCont = true;
					break;
				}
			break;
			case E_DT_INV_OBJECT_PARENT:
				
			break;
		}
		
		
		if (moveCont) {
			
			if (isContainer[destObj->objectType]) {	
				if (_draggingFromInd == _draggingToInd) {
					goto PERFORM_DRAG_END;
				}
			}
			else {
				if (_draggingFromInd == destObj->parentUID) {
					goto PERFORM_DRAG_END;
				}
			}
			
			
			gw->gameObjects[sourceObj->parentUID].removeChild(sourceObj->uid);
			
			if (isContainer[destObj->objectType]) {
				destObj->children.push_back(sourceObj->uid);
				sourceObj->parentUID = destObj->uid;
				draggedIntoWorldObj = true;
			}
			else {
				
				myVec = &(gw->gameObjects[destObj->parentUID].children);
				
				bestPos = 0;
				for (i = 0; i < myVec->size(); i++) {
					if ((*myVec)[i] == destObj->uid) {
						bestPos = i+1;
					}
				}
				myVec->insert(
					myVec->begin()+bestPos,
					sourceObj->uid
				);
				
				sourceObj->parentUID = destObj->parentUID;
			}
			
			if (_draggingFromType == E_DT_WORLD_OBJECT) {
				gw->removeVisObject(sourceObj->uid, false);
			}
			
		}
		
		
		
		
		
		if (
			(_draggingFromType == E_DT_INV_OBJECT) ||
			(_draggingToType == E_DT_INV_OBJECT) ||
			(_draggingToType == E_DT_INV_OBJECT_PARENT) ||
			draggedIntoWorldObj
		) {
			refreshContainers(false);
		}
		
		
PERFORM_DRAG_END:		
		markerFound = false;
		isDraggingObject = false;
	}
	
	
	void removeEntity(bool isReq, int ind) {
		
		if (isReq) {
			naIntData[0] = ind;
			gameNetwork->addNetworkAction(E_NO_REM_ENT,naUintData,naIntData,naFloatData);
			return;
		}
		
		if (ind >= E_OBJ_LENGTH) {
			if (gw->removeVisObject(ind, false)) {
				setSelInd(0);
			}
		}
	}
	
	BaseObjType placeNewEnt(
		bool isReq,
		int et,
		FIVector4* cellPos,
		bool isHidden = false
	) {
		
		
		BaseObj* tmpObj = NULL;
		
		int newType = 0;
		int poolId = et;
		int xv = 1;
		int yv = 1;
		int zv = 1;
		
		int curEntId;
		
		int mf = 0;
		
		bool isRecycled = false;
		
		if (isReq) {
			naFloatData[0] = cellPos->getFX();
			naFloatData[1] = cellPos->getFY();
			naFloatData[2] = cellPos->getFZ();
			naIntData[0] = et;
			gameNetwork->addNetworkAction(E_NO_ADD_ENT,naUintData,naIntData,naFloatData);
			return - 1;
		}
		
		float bounciness = 0.0f;
		float friction = 0.9;
		float windResistance = 0.9;
		
		switch (et) {
			case E_ENTTYPE_OBJ:
				newType = getRandomObjId();
				friction = 0.1;
				windResistance = 1.0f;
				bounciness = 0.3;
			break;
			case E_ENTTYPE_MONSTER:
				newType = getRandomMonsterId();
				mf = 2;
				zv = 2;
			break;
			case E_ENTTYPE_NPC:
				newType = getRandomNPCId();
				mf = 4;
				zv = 2;
			break;
			
			case E_ENTTYPE_BULLET:
			case E_ENTTYPE_TRACE:
				
				if (waterBulletOn) {
					newType = 1103;
				}
				else {
					newType = 1139;
				}
				
				// if (et == E_ENTTYPE_TRACE) {
					
				// }
				
			break;
			
			case E_ENTTYPE_DEBRIS:
			case E_ENTTYPE_WEAPON:
				newType = 0;
				newType = 0;
			break;
			
			
		}
		
		
		if (
			(entPoolStack[poolId].maxCount == 0) ||
			(entPoolStack[poolId].entIds.size() < entPoolStack[poolId].maxCount)
		) {
			isRecycled = false;
			curEntId = gameObjCounter;
			entPoolStack[poolId].entIds.push_back(gameObjCounter);
			
		}
		else {
			
			isRecycled = true;
			curEntId = entPoolStack[poolId].entIds[
				entPoolStack[poolId].curIndex
			];
			
			entPoolStack[poolId].curIndex++;
			if (entPoolStack[poolId].curIndex == entPoolStack[poolId].maxCount) {
				entPoolStack[poolId].curIndex = 0;
			}
		}
		
		FIVector4 newPos;
		newPos.copyFrom(cellPos);
		
		if (isRecycled) {
			gw->removeVisObject(curEntId,true);
		}
		else {
			newPos.addXYZ(0.0f,0.0f,4.0f);
		}
		
		
		
		
		gw->gameObjects[curEntId] = BaseObj();
		tmpObj = &(gw->gameObjects[curEntId]);
		tmpObj->init(
			curEntId,
			0,
			newType,
			et,
			&newPos
		);
		
		if (
			(et == E_ENTTYPE_BULLET) ||
			(et == E_ENTTYPE_TRACE)
		) {
			
			if (noBounce) {
				bounciness = 0.0f;
				friction = 0.0f;
				windResistance = 1.0f;
			}
			else {
				bounciness = 0.7f;
				friction = 0.1f;
				windResistance = 1.0f;
			}
			
		}
		
		
		tmpObj->isHidden = isHidden;
		tmpObj->bounciness = bounciness;
		tmpObj->friction = friction;
		tmpObj->windResistance = windResistance;
		
		tmpObj->maxFrames = mf;
		
		BaseObjType thisObjId = curEntId;
		
		if (isRecycled) {
			gw->addVisObject(curEntId, true);
		}
		else {
			gw->addVisObject(curEntId, false);
			
			gameObjCounter++;
			
			if (isContainer[newType]) {
				fillWithRandomObjects(gameObjCounter-1, 0);
			}
		}
		
		
		
		return thisObjId;
		
	}



	void dispatchEvent(
		int button,
		int state,
		float x,
		float y,
		UIComponent* comp,
		bool automated = false,
		bool preventRefresh = false
	) {
		
		UIComponent* tempComp;
		
		
		BaseObj* tmpObj = NULL;
		GameOrgNode* tmpNode = NULL;
		
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
		
		if (comp->uid.compare("#contItemParent") == 0) {
			if (comp->jvNodeNoTemplate != NULL) {
				if (comp->jvNodeNoTemplate->HasChild("objectId")) {
					if (mouseUpEvent) {
						if (isDraggingObject) {
							if (noTravel) {
								
							}
							else {
								draggingToType = E_DT_INV_OBJECT_PARENT;
								draggingToInd = comp->jvNodeNoTemplate->Child("objectId")->number_value;
								performDrag(
									gameNetwork->isConnected,
									draggingFromInd,
									draggingFromType,
									draggingToInd,
									draggingToType,
									&(worldMarker)
								);
							}
						}
					}
				}
			}
			
			goto DISPATCH_EVENT_END;
		}
		
		if (comp->uid.compare("#contItem") == 0) {
			
			
			
			if (comp->jvNodeNoTemplate != NULL) {
				
				
				if (comp->jvNodeNoTemplate->HasChild("objectId")) {
					
					// !!
					
					if (mouseUpEvent) {
						
						if (wasDoubleClick) {
							i = comp->jvNodeNoTemplate->Child("objectId")->number_value;
							if (isContainer[gw->gameObjects[i].objectType]) {
								toggleCont(i, false);
							}
							else {
								gw->gameObjects[i].isEquipped = !(gw->gameObjects[i].isEquipped);
								if (gw->gameObjects[i].isEquipped) {
									playSoundEvent("showGUI");
								}
								else {
									playSoundEvent("hideGUI");
								}
								refreshContainers(false);
							}
							
							
						}
						else if (isDraggingObject) {
							if (noTravel) {
								
							}
							else {
								draggingToType = E_DT_INV_OBJECT;
								draggingToInd = comp->jvNodeNoTemplate->Child("objectId")->number_value;
								performDrag(
									gameNetwork->isConnected,
									draggingFromInd,
									draggingFromType,
									draggingToInd,
									draggingToType,
									&(worldMarker)
								);
							}
							
						}
					}
					else if (mouseDownEvent) {
						
						isDraggingObject = true;
						draggingFromType = E_DT_INV_OBJECT;
						draggingFromInd = comp->jvNodeNoTemplate->Child("objectId")->number_value;
					}
				}
			}
			
			goto DISPATCH_EVENT_END;
			
		}
		
		
		
		if (mouseUpEvent) {
			
			if (comp->uid.compare("ddMenu.removeEntity") == 0) {
				removeEntity(gameNetwork->isConnected, selObjInd);
			}
			else if (comp->uid.compare("ddMenu.placeEntity.npc") == 0) {
				placeNewEnt(gameNetwork->isConnected,E_ENTTYPE_NPC, &lastCellPos);
			}
			else if (comp->uid.compare("ddMenu.placeEntity.monster") == 0) {
				placeNewEnt(gameNetwork->isConnected,E_ENTTYPE_MONSTER, &lastCellPos);
			}
			else if (comp->uid.compare("ddMenu.placeEntity.object") == 0) {
				placeNewEnt(gameNetwork->isConnected,E_ENTTYPE_OBJ, &lastCellPos);
			}
			else if (comp->uid.compare("charEdit.savePose") == 0) {
				saveOrg();
			}
			else if (comp->uid.compare("charEdit.loadPose") == 0) {
				loadOrg();
			}
			else if (comp->uid.compare("$charEdit.orgOn") == 0) {
				orgOn = curValue != 0.0f;
			}
			else if (comp->uid.compare("$charEdit.pathfindingOn") == 0) {
				//pathfindingOn = curValue != 0.0f;
			}
			else if (comp->uid.compare("$charEdit.editPose") == 0) {
				editPose = curValue != 0.0f;
			}
			else if (comp->uid.compare("$charEdit.mirrorOn") == 0) {
				mirrorOn = curValue != 0.0f;
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
				if (selectedNode != NULL) {
					cout << "yqay" << comp->index << " " << boneStrings[selectedNode->nodeName] << "\n";
					
					selectedNode->orgVecs[E_OV_MATPARAMS].setFX(comp->index);
					tmpNode = getMirroredNode(selectedNode);
					if (tmpNode != NULL) {
						tmpNode->orgVecs[E_OV_MATPARAMS].setFX(comp->index);
					}
					makeDirty();
				}
			}
			else if (comp->uid.compare("#contMenu.close") == 0) {
				
				i = comp->getParent()->getChild(1)->jvNodeNoTemplate->Child("objectId")->number_value;
				
				cout << "ival " << i << "\n";
				
				playSoundEnt("leather0", NULL, 0.1);
				gw->gameObjects[i].isOpen = false;
				refreshContainers(false);
				
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
			isDraggingObject = false;
		}
	}

	StyleSheet* getNewStyleSheet(string ssName) {
		styleSheetMap[ssName].init();
		return &(styleSheetMap[ssName]);
	}

	void initStyleSheet() {
		
		int i;
		
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
		curState->setVal(E_SS_BGCOL0_R, 0.0f, 0.2f, 0.5f, 1.0f);
		curState->setVal(E_SS_BGCOL1_R, 0.0f, 0.4f, 0.8f, 0.5f);
		curState->setVal(E_SS_FGCOLTEXT0_R, 1.0f, 0.75f, 0.0f, 1.0f);
		curState->setVal(E_SS_FGCOLTEXT1_R, 1.0f, 0.75f, 0.0f, 1.0f);
		curState->setVal(E_SS_BDCOL_R, 1.0f, 0.75f, 0.0f, 1.0f);
		headerSS->compStates[E_COMP_OVER].copyFrom(& (headerSS->compStates[E_COMP_UP]) );
		headerSS->compStates[E_COMP_DOWN].copyFrom(& (headerSS->compStates[E_COMP_UP]) );

		curState = &(headerSS->compStates[E_COMP_OVER]);
		curState = &(headerSS->compStates[E_COMP_DOWN]);
		
		
		
		StyleSheet* redSS = getNewStyleSheet("redSS");
		redSS->copyFrom(mainSS);
		
		for (i = 0; i < E_COMP_TOTAL; i++) {
			curState = &(redSS->compStates[i]);
			curState->setVal(E_SS_BGCOL0_R, 1.0f, 0.2f, 0.5f, 1.0f);
			curState->setVal(E_SS_BGCOL1_R, 0.8f, 0.4f, 0.8f, 0.5f);
		}
		
		
		
		StyleSheet* greenSS = getNewStyleSheet("greenSS");
		greenSS->copyFrom(mainSS);
		
		for (i = 0; i < E_COMP_TOTAL; i++) {
			curState = &(greenSS->compStates[i]);
			curState->setVal(E_SS_BGCOL0_R, 0.0f, 1.0f, 0.5f, 1.0f);
			curState->setVal(E_SS_BGCOL1_R, 0.0f, 0.8f, 0.8f, 0.5f);
		}
		
		
		
		
		
	}

	// int requestTerIndex(int requestingBlockId)
	// {


	// 	int i;

	// 	bool foundFree = false;

	// 	float bestDis;
	// 	float testDis;
	// 	int bestInd = 0;

	// 	GameBlock *curBlock;

	// 	// find if blockId already in use
	// 	for (i = 0; i < MAX_TER_TEX; i++)
	// 	{
	// 		if (terTextures[i].usedByBlockId == requestingBlockId)
	// 		{
	// 			return i;
	// 		}
	// 	}

	// 	// find if any blockIds not in use
	// 	for (i = 0; i < MAX_TER_TEX; i++)
	// 	{
	// 		if (terTextures[i].usedByBlockId == -1)
	// 		{
	// 			bestInd = i;
	// 			foundFree = true;
	// 			break;
	// 		}
	// 	}

	// 	if (foundFree)
	// 	{

	// 	}
	// 	else
	// 	{

	// 		// find furthest blockId
	// 		for (i = 0; i < MAX_TER_TEX; i++)
	// 		{

	// 			curBlock = gw->getBlockAtId(terTextures[i].usedByBlockId);
	// 			testDis = curBlock->offsetInBlocks.distance(&(gw->camBlockPos));

	// 			if (i == 0)
	// 			{
	// 				bestInd = 0;
	// 				bestDis = testDis;
	// 			}
	// 			else
	// 			{
	// 				if (testDis > bestDis)
	// 				{
	// 					bestDis = testDis;
	// 					bestInd = i;
	// 				}
	// 			}

	// 		}

	// 	}

	// 	terTextures[bestInd].usedByBlockId = requestingBlockId;
	// 	terTextures[bestInd].alreadyBound = false;
	// 	return bestInd;


	// }


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

	~Singleton()
	{
		if (gw)
		{
			delete gw;
		}
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


	void drawBox(
		FIVector4 *v0,
		FIVector4 *v1,
		int faceFlag=2
	) {


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
	
	
	void getMaterialString() {
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
	
	bool getPrimTemplateString() {
		
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
	
	void refreshIncludeMap() {
		getMaterialString();
		getPrimTemplateString();
	}
	

	void doShaderRefresh(bool doBake)
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
				&(externalJSON["shaderParams"].jv)
			);
			
			
			
		}
		

	}





	

	void setWH(int w, int h)
	{

		baseW = w;
		baseH = h;
	}

	void sampleFBODirect(
		FBOSet *fbos,
		int offset = 0, /* write offset */
		int _minOff = 0, /* read min */
		int _maxOff = -1 /* read max */
		
	) {
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

	void unsampleFBODirect(
		FBOSet *fbos,
		int offset = 0, /* write offset */
		int _minOff = 0, /* read min */
		int _maxOff = -1 /* read max */ 
		
	) {
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



	void bindFBODirect(FBOSet *fbos, int doClear = 1)
	{
		setMatrices(fbos->width, fbos->height);
		
		fbos->bind(doClear);
		currentFBOResolutionX = fbos->width;
		currentFBOResolutionY = fbos->height;
	}

	////


	// todo: optimize this
	FBOSet* getFBOByName(string &fboName) {
		
		if (fboMap.find( fboName ) == fboMap.end()) {
			cout << "invalid key " << fboName << "\n";
			exit(0);
		}
		
		return &(fboMap[fboName]);
	}

	void sampleFBO(
		string fboName,
		int offset = 0,
		int swapFlag = -1,
		int minOff = 0,
		int maxOff = -1
	) {
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

	void unsampleFBO(
		string fboName,
		int offset = 0,
		int swapFlag = -1,
		int minOff = 0,
		int maxOff = -1
	)	{

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

	FBOSet *getFBOSet (string fboName)
	{
		return getFBOByName(fboName);
	}

	FBOWrapper *getFBOWrapper (string fboName, int offset)
	{
		FBOSet *fbos = getFBOByName(fboName);
		return fbos->getFBOWrapper(offset);
	}

	
	
	void copyFBO(string src, string dest, int num = 0)
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

	void copyFBO2(string src, string dest, int num1 = 0, int num2 = 1)
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

	void bindFBO(string fboName, int swapFlag = -1, int doClear = 1)
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

	void unbindFBO()
	{
		
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		setMatrices(baseW, baseH);

	}



	void bindShader(string shaderName)
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
	void setShaderMatrix4x4(string paramName, float *x, int count)
	{
		curShaderPtr->setShaderMatrix4x4(paramName, x, count);
	}
	void setShaderMatrix3x3(string paramName, float *x, int count)
	{
		curShaderPtr->setShaderMatrix3x3(paramName, x, count);
	}
	void setShaderArray(string paramName, float *x, int count)
	{
		curShaderPtr->setShaderArray(paramName, x, count);
	}
	
	GLint getShaderLoc(string paramName) {
		return curShaderPtr->getShaderLoc(paramName);
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
	void setShaderbtVec3(string paramName, btVector3 v)
	{
		curShaderPtr->setShaderbtVec3(paramName, v);
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


	void setShaderTBO(int multitexNumber, GLuint tbo_tex, GLuint tbo_buf)
	{
		if (shadersAreLoaded)
		{
			glActiveTexture(GL_TEXTURE0 + multitexNumber);
			glBindTexture(GL_TEXTURE_2D, tbo_tex);
			if (tbo_tex != 0) {
				glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, tbo_buf); //GL_R32F
			}
			curShaderPtr->setShaderInt(shaderTextureIds[multitexNumber] , multitexNumber);
		}
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


	void drawQuadWithCoords(
		FIVector4* p0,
		FIVector4* p1,
		FIVector4* p2,
		FIVector4* p3,
		
		float tx1,
		float ty1,
		float tx2,
		float ty2
		
	) {
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
	
	void drawQuadBounds(
		float fx1,
		float fy1,
		float fx2,
		float fy2,
		
		float fz
	) {
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
	

	void drawFSQuad()
	{
		glCallList(fsqDL);
	}
	

	void drawFSQuadOffset(
		float xOff,
		float yOff,
		float zm
	)
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

	void drawFBO(string fboName, int ind, float zm, int swapFlag = -1)
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

	void drawFBOOffsetDirect(FBOSet *fbos, int ind, float xOff, float yOff, float zm)
	{

		glBindTexture(GL_TEXTURE_2D, fbos->fbos[ind].color_tex);
		//glClearColor(0.2,0.2,0.2,0.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawFSQuadOffset(xOff, yOff, zm);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void drawFBOOffset(string fboName, int ind, float xOff, float yOff, float zm)
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
	
	
	float getTerHeightScaled(float val)
	{
		return val*heightMapMaxInCells; //mixf(0.125f,0.875f,val)
		
	}


	float getSLNormalized()
	{
		return (((float)gw->seaLevel) / 255.0f);
	}
	float getSeaHeightScaled()
	{
		return getSLNormalized()*heightMapMaxInCells;
	}

	float getHeightAtPixelPos(float x, float y, bool dd = false)
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
	
	
	void transformOrg(GameOrg* curOrg) {
		curOrg->baseNode->doTransform(this);
	}
	
	void angleToVec(FIVector4* fv, float xr, float yr) {
		fv->setFXYZ(
			cos(xr)*sin(yr),
			sin(xr)*sin(yr),
			cos(yr)
		);
		fv->normalize();
	}
	
	void vecToAngle(FIVector4* fv, FIVector4 * ta) {
		
		ta->setFXYZ(
			atan2(fv->getFY(),fv->getFX()),
			acos(fv->getFZ()),
			0.0f	
		);
	}
	
	
	void syncObjects() {
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
		
		
		
		for (i = 0; i < gameOrgs.size(); i++) {
			if (currentActor != NULL) {
				
				if (i == currentActor->orgId) {
					gameOrgs[i]->basePosition.setBTV(currentActor->getCenterPoint(0));
				}
				
			}
			
			transformOrg(gameOrgs[i]);
		}
		
		
	}

	void updateCamVals() {
		
		
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
			tempLerpPos.copyFrom(&camLerpPos);
			tempLerpPos.addXYZRef(cameraPos,-1.0f);
			tempLerpPos.multXYZ(timeDelta);
			
			amountInvalidMove = tempLerpPos.length();
			depthInvalidMove = amountInvalidMove > 0.01f;
			
			cameraPos->addXYZRef(&tempLerpPos);
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

	void moveCamera(FIVector4 *pModXYZ)
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
	
	GameOrgNode* getMirroredNode(GameOrgNode* curNode) {
		if (getCurOrg() == NULL) {
			return NULL;
		}
		GameOrg* testHuman = getCurOrg();
		
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

	

	void applyNodeChanges(GameOrgNode* _curNode, float dx, float dy) {
		
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
			
			xm = dx/100.0f;
			ym = dy/100.0f;
			
			if (shiftDown()) { // || altDown()
								
				if (lbDown) {
					curNode->orgVecs[E_OV_TBNRAD0].addXYZ(0.0f,xm,ym);
				}
				if (rbDown) {
					curNode->orgVecs[E_OV_TBNRAD1].addXYZ(0.0f,xm,ym);
				}
				if (mbDown) {
					
					curNode->orgVecs[E_OV_TBNRAD0].addXYZ(ym, 0.0f, 0.0f);
					curNode->orgVecs[E_OV_TBNRAD1].addXYZ(ym, 0.0f, 0.0f);
					
					//curNode->boneLengthScale += ym;
				}
			}
			else {
				
				if (lbDown) {
					curNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(dirMod*ym,0.0,0.0); //dirMod*ym
				}
				if (rbDown) {
					curNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(0.0,0.0,dirMod*ym);
				}
				if (mbDown) {
					curNode->orgVecs[E_OV_THETAPHIRHO].addXYZ(0.0,dirMod*ym,0.0);
				}
				
				
			}
			
			if (applyToChildren) {
				for (k = 0; k < curNode->children.size(); k++) {
					applyNodeChanges(curNode->children[k], dx, dy);
				}
			}
			
			
		}
			
		
	}
	
	
	GLfloat getCamRot(int ind) {
		
		return curCamRotation[ind];
		
	}
	

	void moveObject(float dx, float dy)
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
			(orgOn) && 
			(activeNode != NULL) &&
			editPose
			
		) {
			
			applyNodeChanges(activeNode, dx, dy);
			
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
					
					
					if (rbDown) {
						camRotation[0] -= dx*0.005f;
						camRotation[1] += dy*0.005f;
					}
					
				}
				


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



	void setCameraToElevation() {

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

	

	btVector3 getRayTo(float x, float y) {

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



	// void getRay(
	// 	float mx,
	// 	float my,
	// 	btVector3 &begPos,
	// 	btVector3 &endPos,
	// 	btVector3 &rayDir
	// ) {
	// 	float aspect = bufferDim[1]/bufferDim[0];
	// 	float myNear = clipDist[0];
	// 	float myFar = clipDist[1];


	// 	float newX = mx/bufferDim[0];
	// 	float newY = my/bufferDim[1];

	// 	newX = newX-0.5;
	// 	newY = newY-0.5;

	// 	float dx = tan(FOV*0.5f)*(newX*2.0-1.0f)/aspect;
	// 	float dy = tan(FOV*0.5f)*((1.0f-newY)*2.0-1.0f);

	// 	dx = -dx;

	// 	Vector4 p1s = Vector4(dx*myNear,dy*myNear,myNear,1.0);
	// 	Vector4 p2s = Vector4(dx*myFar,dy*myFar,myFar,1.0);

	// 	Matrix4 modelviewInverse(viewMatrixDI);

	// 	Vector4 p1 = modelviewInverse*p1s;
	// 	Vector4 p2 = modelviewInverse*p2s;

	// 	begPos = btVector3(p1.x, p1.y, p1.z);
	// 	endPos = btVector3(p2.x, p2.y, p2.z);

	// 	rayDir = (begPos-endPos);
	// 	rayDir.normalize();

	// }
	
	

	void runReport() {
		
		//mainGUI->runReport();
		
		cout << "num limbs " << limbDataDebug << "\n";
		
		// cout << "lastMouseX" << lastMouseX << "\n";
		// cout << "lastMouseY" << lastMouseY << "\n";
		
		// cout << "polyCount " << polyCount << "\n";
		
		// doTraceVecND("cameraPos ", cameraGetPosNoShake());
		// doTraceVecND("lookAtVec ", &lookAtVec);
		// cout << "\n";
		
		//cout << "Object Count: " << lastObjectCount << "\n";
		// cout << "lightCount: " << gw->lightCount << "\n";
		//cout << "TOT GPU MEM USED (MB): " << TOT_GPU_MEM_USAGE << "\n";
		// cout << "HolderSize (MB): " << holderSizeMB << "\n";
		// cout << "totalPointCount: " << totalPointCount << "\n";
	}
	
	
	
	void setFirstPerson(bool _newVal) {
		
		bool newVal = _newVal;
		
		if (currentActor == NULL) {
			newVal = false;	
		}
		
		if (firstPerson) {
			subjectDistance = lastSubjectDistance;
		}
		
		firstPerson = newVal;
		
		if (firstPerson) {
			lastSubjectDistance = subjectDistance;
		}
		
		cout << "firstPerson " << firstPerson << "\n";
	}
	
	int getCurActorUID() {
		if (currentActor == NULL) {
			return -1;
		}
		else {
			return currentActor->uid;
		}
	}
	
	void updateCS() {
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
	
	
	void getMarkerPos(int x, int y) {
		getPixData(&spaceUpPD, x, y, true, false);
		worldMarker.copyFrom(&spaceUpPD);
		lastCellPos.copyFrom(&(worldMarker));
	}
	
	
	void makeJump(int actorId, int isUp) {
		
		BaseObj* ge = &(gw->gameObjects[actorId]);
		
		if (ge->bodies.size() < 0) {
			return;
		}
		
		if (ge->isJumping) {
			return;
		}
		
		float jumpAmount = conVals[E_CONST_JUMP_AMOUNT]*ge->getMarkerMass();
		
		
		if (isUp == 1) {
			if (ge->bodies[E_BDG_CENTER].inWater) {
				
				
				if (
					gw->getCellAtCoords(
						ge->getCenterPoint(0).getX(),
						ge->getCenterPoint(0).getY(),
						ge->getCenterPoint(0).getZ() + 1.0f
					) == E_CD_EMPTY
				) {
					
					
					// at water surface
					
					ge->applyImpulse(btVector3(0.0f,0.0f,jumpAmount), false, 0);
					
					
					
				}
				else {
					
					// underwater
					
					
					ge->applyImpulse(btVector3(0.0f,0.0f,jumpAmount), false, 0);
					
					playSoundEnt(
						"bubble0",
						ge,
						0.3f,
						0.5f
					);
				}
				
				ge->isJumping = true;
				
				
			}
			else {
				if (ge->allFalling()) {
					
				}
				else {
					
					
					ge->applyImpulse(btVector3(0.0f,0.0f,jumpAmount), false, 0);
					ge->zeroZ = true;
					
					playSoundEnt(
						"jump0",
						ge,
						0.0f
					);
					
					ge->isJumping = true;
					
				}
			}
		}
		else {
			if (ge->bodies[E_BDG_CENTER].inWater) {
				ge->applyImpulse(btVector3(0.0f,0.0f,-jumpAmount), false, 0);
				
				playSoundEnt(
					"bubble0",
					ge,
					0.3f,
					0.5f
				);
			}
		}
		
		
		
	}
	
	void resetGeom() {
		int i;
		geomStep = 0;
		geomOrigOffset.setFXYZ(0.0f);
		geomPoints[0].setFXYZ(0.0f,0.0f,0.0f);
		
		for (i = 0; i < FLOATS_PER_TEMPLATE; i++) {
			paramArrGeom[i] = defaultTemplate[i];
		}
	}
	
	
	void changePose(int amount) {
		
		GameOrg* testHuman = getCurOrg();
		
		currentPose += amount;
		
		if (currentPose == E_PK_LENGTH) {
			currentPose = E_PK_T_POSE;
		}
		if (currentPose < 0) {
			currentPose = E_PK_LENGTH-1;
		}
		
		cout << "Current Pose: " << poseStrings[currentPose] << "\n";
		
		
		
		if (testHuman != NULL) {
			testHuman->targetPose = currentPose;
			
			if (editPose) {
				loadCurrentPose();
			}
			
			//testHuman->setToPose(gamePoses[currentPose]);
			//transformOrg(testHuman);
			//currentActor->wakeAll();
		}
	}
	
	void saveCurrentPose() {
		GameOrg* testHuman = getCurOrg();
		
		if (editPose) {
			
			if (testHuman != NULL) {
				testHuman->saveToFile(poseStrings[currentPose]);
				gamePoses[currentPose]->loadFromFile(poseStrings[currentPose], false);
				transformOrg(gamePoses[currentPose]);
				
				cout << "Saved Pose " << poseStrings[currentPose] << "\n";
				
			}
		}
	}
	
	
	
	void loadNonPoseData(int currentNPD) {
		GameOrg* testHuman = getCurOrg();
		
		int i;
		
		if (editPose) {
			if (testHuman != NULL) {
				
				for (i = 0; i < E_PK_LENGTH; i++) {
					//if (i != E_PK_NON_POSE) {
						currentPose = i;
						loadCurrentPose();
						
						gamePoses[currentPose]->loadFromFile(poseStrings[currentNPD], true);
						transformOrg(gamePoses[currentPose]);
						testHuman->setToPose(gamePoses[currentPose],1.0f);
						transformOrg(testHuman);
						makeDirty();
						cout << "Loaded Non Pose " << poseStrings[currentPose] << "\n";
						saveCurrentPose();
						
					//}
				}
				currentPose = E_PK_NON_POSE;
				loadCurrentPose();
			}
		}
	}
	
	
	void loadCurrentPose() {
		GameOrg* testHuman = getCurOrg();
		
		if (editPose) {
			if (testHuman != NULL) {
				
				gamePoses[currentPose]->loadFromFile(poseStrings[currentPose], false);
				transformOrg(gamePoses[currentPose]);
				testHuman->setToPose(gamePoses[currentPose],1.0f);
				transformOrg(testHuman);
				makeDirty();
				cout << "Loaded Pose " << poseStrings[currentPose] << "\n";
				
			}
		}
	}
	
	
	void processInput(unsigned char key, bool keyDown, int x, int y) {
		
		int i;
		
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
		
		GameOrg* testHuman = getCurOrg();
		
		
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
					updateHolders = true;
					getMarkerPos(x, y);
					placeNewEnt(gameNetwork->isConnected,E_ENTTYPE_NPC, &lastCellPos);
				break;
				case '2':
					updateHolders = true;
					getMarkerPos(x, y);
					placeNewEnt(gameNetwork->isConnected,E_ENTTYPE_MONSTER, &lastCellPos);
				break;
				case '3':
					updateHolders = true;
					getMarkerPos(x, y);
					placeNewEnt(gameNetwork->isConnected,E_ENTTYPE_OBJ, &lastCellPos);
				break;
				case '4':
					updateHolders = true;
					getMarkerPos(x, y);
					placeNewEnt(gameNetwork->isConnected,E_ENTTYPE_WEAPON, &lastCellPos);
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
						//isMacro = !isMacro;
						//mirrorOn = !mirrorOn;
						
						ignoreFrameLimit = !ignoreFrameLimit;
						cout << "ignoreFrameLimit: " << ignoreFrameLimit << "\n";
					break;


				// case '9':
				// 	saveAllData();
				// 	cout << "data saved\n";
				// break;
				
				case 19: //ctrl-s
					saveExternalJSON();
					saveGUIValues();
					saveCurrentPose();
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
					
					
					cout << "\n";
					cout << "pathfindingOn: " << pathfindingOn << "\n";
					cout << "updateHolders " << updateHolders << "\n";
					
					
					
				break;
				
				case 'Q':
					
					//gamePhysics->beginDrop();
				break;
				
				case 'q':
					
					targetSubjectZoom = 1.0f;
					if (!ignoreFrameLimit) {
						subjectZoom = targetSubjectZoom;
					}
					
					if (selObjInd >= E_OBJ_LENGTH) {
						if (selObjInd == actObjInd) {
							setCurrentActor(NULL);
						}
						else {
							setCurrentActor(&(gw->gameObjects[selObjInd]));
							
							playSoundEnt(
								"swimming0",
								currentActor,
								0.0f,
								0.0f,
								true
							);
							playSoundEnt(
								"walkinggravel0",
								currentActor,
								0.0f,
								0.0f,
								true
							);
							
						}
					}
					else {
						setCurrentActor(NULL);
					}
					
					break;

				
				case 'W':
					cout << "start FPS timer\n";
					fpsTest = true;
					fpsCount = 0;
					fpsTimer.start();
				break;
				
				// case 'w': // grab / throw
					
				// 	//setFirstPerson(!firstPerson);
					
					
				// 	// grab
					
					
				// break;

				case 27: // esc
					//std::exit(0);
					
					if (ddMenu->visible || contMenu->visible) {
						toggleDDMenu(x,y,false);
						contMenu->visible = false;
						closeAllContainers();
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
					
					loadGUI();
					loadValuesGUI();
				break;
				case 'r':
					
					
					
					doShaderRefresh(bakeParamsOn);
					cout << "Shaders Refreshed\n";

					
					
					break;
					
				case 'j':
				
					if (editPose) {
						resetActiveNode();
					}
					else {
						smoothMove = !smoothMove;
					}
				
					
				
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
					gridOn = 1.0 - gridOn;
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
					gameAI->getKB();
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
					//targetTimeOfDay = 1.0f-targetTimeOfDay;
					
					break;

				case 'h':
					
					// diagCount++;
					
					// if (diagCount >= 9) {
					// 	diagCount = 0;
					// }
					
					//gameFluid[E_FID_SML]->updateTBOData(false,true);
					gameFluid[E_FID_BIG]->updateTBOData(false,true);
					
					
					break;
					
				case 'y':
					
				break;
				case 't':
					testOn = !testOn;
					
				break;
				case 'T':
					testOn2 = !testOn2;
					
				break;
				// case 'o':
				// 	//rotOn = !rotOn;
				// 	break;
					

				case '\t':
				
					if (ddMenu->visible || contMenu->visible) {
						toggleDDMenu(x,y,false);
						contMenu->visible = false;
						closeAllContainers();
						
						escCount = 0;
					}
					else {
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
					}
					
					break;

				case ' ':
					
				
					//timeMod = !timeMod;
					// if (mouseState == E_MOUSE_STATE_PICKING) {
					// 	selectedEnts.cycleEnts();
					// }
					
					
				break;
				
				
				case 'C':
					if (currentPose == E_PK_NON_POSE) {
						saveCurrentPose();
						loadNonPoseData(E_PK_NON_POSE);
					}
					else {
						cout << "Error, switch to E_PK_NON_POSE\n";
					}
				
					
				break;
				case 'c':
					
					editPose = !editPose;
					
					cout << "editPose " << editPose << "\n";
					
					if (editPose) {
						loadCurrentPose();
					}
					
					//setCameraToElevation();
				
					//doShaderRefresh(bakeParamsOn);

					// noBounce = !noBounce;
					
					// if (currentActor == NULL) {
					// 	noBounce = false;
					// }
					
					// cout << "noBounce " << noBounce << "\n";
									
				
					break;
					
				case 'v':
					
					mirrorOn = !mirrorOn;
					cout << "mirrorOn " << mirrorOn << "\n";
					
					//waterBulletOn = !waterBulletOn;
					//gw->toggleVis(selectedEnts.getSelectedEnt());
					break;
				

				case 'X':
					//fogOn = 1.0 - fogOn;
					//cout << "fog on " << fogOn << "\n";
					changePose(-1);
					break;
					
				case 'x':
					changePose(1);
					
					//fxaaOn = !fxaaOn;
					//cout << "fxaaOn " << fxaaOn << "\n";
					break;

				case 'm':


					
					
					// doPathReport = true;

					medianCount++;					
					if (medianCount == 4) {
						medianCount = 0;
					}
					
					//runReport();
					
					// refreshPaths = true;
					

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

				default:
					cout << "No code for key: " << key << "\n";
					break;
			}
		}
		
		
	}
	
	
	
	
	
	
	
	

	void getPixData(FIVector4 *toVector, int _xv, int _yv, bool forceUpdate, bool isObj)
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


	float getMinGeom(int baseIndex) {
		int newIndex = baseIndex*4;
		
		return min(
			min(
				paramArrGeom[newIndex + 0],
				paramArrGeom[newIndex + 1]
			),
			paramArrGeom[newIndex + 2]
		);
		
	}
	
	
	
	void setFXYZWGeom(int baseIndex, FIVector4* baseVec) {
		int newIndex = baseIndex*4;
		int i;
		
		for (i = 0; i < 4; i++) {
			paramArrGeom[newIndex + i] = baseVec->getIndex(i);
		}
		
	}
	
	void setFXYGeom(int baseIndex, float xv, float yv) {
		int newIndex = baseIndex*4;
		paramArrGeom[newIndex + 0] = xv;
		paramArrGeom[newIndex + 1] = yv;
	}
	void setFXGeom(int baseIndex, float xv) {
		int newIndex = baseIndex*4;
		paramArrGeom[newIndex + 0] = xv;
	}
	void setFYGeom(int baseIndex, float yv) {
		int newIndex = baseIndex*4;
		paramArrGeom[newIndex + 1] = yv;
	}
	void setFZGeom(int baseIndex, float zv) {
		int newIndex = baseIndex*4;
		paramArrGeom[newIndex + 2] = zv;
	}
	void setFWGeom(int baseIndex, float wv) {
		int newIndex = baseIndex*4;
		paramArrGeom[newIndex + 3] = wv;
	}
	
	
	float getFXGeom(int baseIndex) {
		int newIndex = baseIndex*4;
		return paramArrGeom[newIndex + 0];
	}
	float getFYGeom(int baseIndex) {
		int newIndex = baseIndex*4;
		return paramArrGeom[newIndex + 1];
	}
	float getFZGeom(int baseIndex) {
		int newIndex = baseIndex*4;
		return paramArrGeom[newIndex + 2];
	}
	float getFWGeom(int baseIndex) {
		int newIndex = baseIndex*4;
		return paramArrGeom[newIndex + 3];
	}
	
	
	
	
	
	
	
	/////////////////////////////////////////
	//                                     //
	// ############## INPUT ############## //
	//                                     //
	/////////////////////////////////////////

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
	
	void keyboardUp(unsigned char key, int _x, int _y)
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
	
	void keyboardDown(unsigned char key, int _x, int _y)
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
	
	void updateCurGeom(int x, int y) {
		
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
							-roundf(
								cos(xrotrad)*yv2 + -sin(xrotrad)*xv2
							),
							-roundf(
								sin(xrotrad)*yv2 + cos(xrotrad)*xv2
							),
							roundf(curPrimMod)
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
						setFXYGeom(E_PRIMTEMP_BOUNDSMIN, roundf(-xv), roundf(-yv));
						setFXYGeom(E_PRIMTEMP_BOUNDSMAX, roundf( xv), roundf( yv));
						
						setFXYGeom(E_PRIMTEMP_VISMIN, roundf(-xv), roundf(-yv));
						setFXYGeom(E_PRIMTEMP_VISMAX, roundf( xv), roundf( yv));
						
						setFZGeom(E_PRIMTEMP_BOUNDSMIN, roundf(-curPrimMod));
						setFZGeom(E_PRIMTEMP_BOUNDSMAX, roundf( curPrimMod));
						
						setFZGeom(E_PRIMTEMP_VISMIN, roundf(-curPrimMod));
						setFZGeom(E_PRIMTEMP_VISMAX, roundf( curPrimMod));
						
					break;
					case E_GEOM_POINTS_OFFSET:
						geomOrigOffset.setFXYZ(
							roundf(xv2),
							roundf(yv2),
							roundf(curPrimMod)
						);
					break;
					case E_GEOM_POINTS_CORNER:
					
						minCornerDis = 0.1;
						maxCornerDis = getMinGeom(E_PRIMTEMP_BOUNDSMAX);
						
						setFXGeom(E_PRIMTEMP_CORNERDIS, 
							roundf(mixf(
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
							roundf(max(xv2,getFXGeom(E_PRIMTEMP_BOUNDSMIN))),
							roundf(max(yv2,getFYGeom(E_PRIMTEMP_BOUNDSMIN)))
						);
					break;
					case E_GEOM_POINTS_POS_RAD_XY:						
						setFXYGeom(E_PRIMTEMP_VISMAX, 
							roundf(min(xv2,getFXGeom(E_PRIMTEMP_BOUNDSMAX))),
							roundf(min(yv2,getFYGeom(E_PRIMTEMP_BOUNDSMAX)))
						);
					
					break;
					case E_GEOM_POINTS_NEG_RAD_Z:
						setFZGeom(E_PRIMTEMP_VISMIN, roundf(max(yv2,getFZGeom(E_PRIMTEMP_BOUNDSMIN))) );
					break;
					case E_GEOM_POINTS_POS_RAD_Z:
						setFZGeom(E_PRIMTEMP_VISMAX, roundf(min(yv2,getFZGeom(E_PRIMTEMP_BOUNDSMAX))) );
					break;
					case E_GEOM_POINTS_THICKNESS:
						setFYGeom(E_PRIMTEMP_CORNERDIS, roundf(yv));
					break;
				}
			}
			
			
		}
		
		
	}
	
	void mouseMove(int _x, int _y)
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
		
		if (ddMenu != NULL) {
			ddVis = ddMenu->visible;
		}
		
		float fx = ((float)x)*M_PI*2.0f / bufferDim[0];
		float fy = ((float)y)*M_PI / bufferDim[1];
		
		
		if (hitGUI) {
			return;
		}
		
		
		if ((highlightedLimb==-1)&&mbDown) {
			angleToVec(&lightVec, fx*2.0, fy*2.0);
			lightVecOrig.copyFrom(&lightVec);
			lightVec.setFZ(-abs(lightVec.getFZ()));
		}
		
		
		if (abDown)
		{
			
			if (bCtrl&&(mouseState == E_MOUSE_STATE_BRUSH)) {
				
			}
			else {
				moveObject((float)dx, (float)dy);
			}
			
		}
		else
		{

			if (placingGeom||RT_TRANSFORM||editPose||pathfindingOn||(mouseState != E_MOUSE_STATE_MOVE)) {
			//if (true) {
				getPixData(&mouseMovePD, x, y, false, false);
				getPixData(&mouseMoveOPD, x, y, true, true);
			}
			

			gw->updateMouseCoords(&mouseMovePD);
			
			if (pathfindingOn) {
				
				if (gameLogic->getClosestPathInd(&mouseMovePD, closestHolder) > -1) {
					
					if (pathFindingStep < 2) {
						moveNodes[pathFindingStep].copyFrom(&(mouseMovePD));
					}
				}
				
				
			}
			

			//////////////

			

			if (
				orgOn &&
				editPose
				&& (!ddVis)
			) {
				updateNearestOrgNode(false, &mouseMovePD);
			}
			else {
				if (!ddVis) {
					activeNode = NULL;
					setSelNode(NULL);
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
	
	void doSwing(BaseObj* ca) {
		
		GameOrg* curOrg;
		
		if (ca->isSwinging) {
			
		}
		else {
			
			if (ca->weaponActive) {
				ca->isSwinging = true;
				curOrg = gameOrgs[ca->orgId];
				curOrg->stepCount = 0;
				curOrg->totTime = 0;
				playSoundEnt("swing0", ca);
			}
			
			
		}
		
		
	}
	
	void mouseClick(int button, int state, int _x, int _y) {
		
		int tempInt;

		FIVector4 newPos;

		int x = _x / scaleFactor;
		int y = _y / scaleFactor;

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
		bool wasDoubleClick = false;
		
		bool ddVis = false;
		if (ddMenu != NULL) {
			ddVis = ddMenu->visible;
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
		
		
		if (abDown) {
			
		}
		else {
			muTime = myTimer.getElapsedTimeInMilliSec();
			longClick = (muTime - mdTime) > 300.0f;
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
		
		
		if (currentActor != NULL) {
			
			// if (currentActor->weaponActive) {
				
			// 	// if (lbDown) {
			// 	// 	currentActor->begSwing();
			// 	// }
				
				
			// 	//
			// }
			
			if (lbClicked) {
				doSwing(currentActor);
			}
			
		}
		
		
		
		if (lbClicked) {
			gamePhysics->lastBodyPick = NULL;
		}
		
		

		if (abClicked) {

			if (ddMenu != NULL) {
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
								
								
								if (pathfindingOn) {
									
									pathFindingStep++;
									
									if (pathFindingStep == 3) {
										pathFindingStep = 0;
										moveNodes[0].setFXYZ(0.0,0.0,0.0);
										moveNodes[1].setFXYZ(0.0,0.0,0.0);
										gameLogic->searchedForPath = false;
										gameLogic->didFindPath = false;
									}
								}
								else {
									
								}
								
								
							}
							else {
								if (isDraggingObject) {
									
									upInd = mouseUpOPD.getFW();
									
									//cout << "UP IND " << upInd << "\n";
									
									if (upInd == 0) {
										draggingToInd = 0;
										draggingToType = E_DT_NOTHING;
										performDrag(
										gameNetwork->isConnected,
											draggingFromInd,
											draggingFromType,
											draggingToInd,
											draggingToType,
											&(worldMarker)
										);
									}
									else {
										if (upInd >= E_OBJ_LENGTH) {
											
												if (isContainer[gw->gameObjects[upInd].objectType]) {
													draggingToInd = upInd;
													draggingToType = E_DT_WORLD_OBJECT;
												}
												else {
													draggingToInd = 0;
													draggingToType = E_DT_NOTHING;
												}
												
												performDrag(
													gameNetwork->isConnected,
													draggingFromInd,
													draggingFromType,
													draggingToInd,
													draggingToType,
													&(worldMarker)
												);
										}						
									}
									
									
									
								}
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
				mdTime = myTimer.getElapsedTimeInMilliSec();
				

				if (rbDown&&(!lbDown)) {
					activeObject = E_OBJ_CAMERA;
				}
				
				
				
				if (mouseState == E_MOUSE_STATE_MOVE) {
					if (
						orgOn &&
						editPose
						&& (!ddVis)
						
					) {
						findObject = !(updateNearestOrgNode(true, &mouseDownPD));
					}
					else {
						findObject = true;
					}
					
					if (findObject&&lbDown) {
						
						
						
						bestInd = mouseDownOPD.getFW();
						
						
						
						setSelInd(bestInd);
						
						//setCurrentActor(&(gw->gameObjects[bestInd]));
						
						
						
						
						
						if (selObjInd != 0) {
							if (lastObjInd == selObjInd) {
								if ( (mdTime-clickTime) < 500 ) {
									wasDoubleClick = true;
								}
							}
						}
						
						lastObjInd = selObjInd;
						clickTime = mdTime;
						
						
						draggingFromInd = 0;
						draggingFromType = E_DT_NOTHING;
						
						if (wasDoubleClick) {
							toggleCont(selObjInd, true);
						}
						
						if (bestInd >= E_OBJ_LENGTH) {
							
							isDraggingObject = true;
							//markerFound = true;
							draggingFromInd = selObjInd;
							draggingFromType = E_DT_WORLD_OBJECT;
							
							// todo: make sure bestInd exists
							
						}
						else {
							
							if (bCtrl) {
								if (bestInd <= 0) {
									
								}
								else {
									activeObject = (E_OBJ)(bestInd);
									hitObject = true;
								}
							}
							
							if (hitObject) {
								
							}
							else {
								//setCurrentActor(NULL);
							}
							
							
						}
						
					}
				}
				
				
				
				
				
				
				
				


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
	
	
	
	/////////////////////////////////////////
	//                                     //
	// ########### END INPUT ############# //
	//                                     //
	/////////////////////////////////////////
	
	
	
	
	
	


	void makeDirty() {
		
		if (currentActor != NULL) {
			currentActor->wakeAll();
		}
		
		//testHuman->gph->childrenDirty = true;
	}

	void setSelNode(GameOrgNode* newNode) {
		
		selectedNode = newNode;
		if (selectedNode != lastSelNode) {
			makeDirty();
		}
		lastSelNode = newNode;
		
	}

	



	// void worldToScreenBase(FIVector4 *sc, FIVector4 *wc) {
		
		
		
		
		
		
		
	// 	Vector4 v;
	// 	v.x = wc->getFX();
	// 	v.y = wc->getFY();
	// 	v.z = wc->getFZ();
	// 	v.w = 1.0;
		

		
		
		
		
	// 	GLdouble winX;
	// 	GLdouble winY;
	// 	GLdouble winZ;
		
		
	// 	gluProject(
	// 		v.x,// GLdouble  	objX,
	// 		v.y,// GLdouble  	objY,
	// 		v.z, // GLdouble  	objZ,
	// 		viewMatrixD, // const GLdouble *  	model,
	// 		projMatrixD, // const GLdouble *  	proj,
	// 		viewport, // const GLint *  	view,
	// 		&winX,
	// 		&winY,
	// 		&winZ
	// 	);
		
	// 	sc->setFXYZ(
	// 		(winX/bufferDim.getFX())/((float)DEF_SCALE_FACTOR),
	// 		(winY/bufferDim.getFY())/((float)DEF_SCALE_FACTOR),
	// 		1.0f - winZ/clipDist[1]
	// 	);
		

	// }

	void refreshContainers(bool onMousePos) {
		UIComponent* objCont = NULL;
		
		bool oldVis = false;
		
		if (contMenu != NULL) {
			
			cout << "refreshContainers\n";
			
			externalJSON.erase("objectData"); // mem leak?
			
			oldVis = contMenu->visible;
			contMenu->visible = anyContainerOpen();

			objCont = mainGUI->findNodeByString("objectContainer");
			//objCont->jvNodeNoTemplate->Child("dataParams")->number_value = contIndex;
			
			mainGUI->refreshNode(objCont);
			
			if (onMousePos&&(oldVis == false)) {
				
				// contMenu->dragOffset.x = 0.0f;
				// contMenu->dragOffset.y = 0.0f;
				contMenuBar = contMenu->getChild(0)->getChild(0);
				
				contMenuBar->lastDrag.x = (guiX);
				contMenuBar->lastDrag.y = min((float)(guiY), (float)( (guiWinH - contMenu->getChild(0)->resultDimInPixels.y) ));
				contMenuBar->forceDragUpdate = true;
			}
			
		}
	}

	void toggleCont(int contIndex, bool onMousePos) {
		
		if (
			isContainer[gw->gameObjects[contIndex].objectType]
		) {
			playSoundEnt("leather0", NULL, 0.1);
			gw->gameObjects[contIndex].isOpen = !(gw->gameObjects[contIndex].isOpen);
			refreshContainers(onMousePos);
		}
		else {
			playSoundEnt("bump0");	
		}
		
		
	}

	bool feetContact(BaseObj* ge) {
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


	float getShortestAngle(float begInRad, float endInRad, float amount) {
		int begInDeg = begInRad*180/M_PI;
		int endInDeg = endInRad*180/M_PI;
		
		float shortest_angle = ((((endInDeg - begInDeg) % 360) + 540) % 360) - 180;
		
		return shortest_angle * amount * M_PI / 180.0f;
	}


	void flushKeyStack() {
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
	
	
	
	void applyKeyAction(bool isReq, int actorId, uint keyFlags, float camRotX, float camRotY) {
		
		int i;
		BaseObj* ca;
		
		//depthInvalidMove
		bool charMoved;
		
		if (isReq) {
			//naFloatData[0] = ;
			naUintData[0] = keyFlags;
			naIntData[0] = actorId;
			naFloatData[0] = camRotX;
			naFloatData[1] = camRotY;
			gameNetwork->addNetworkAction(E_NO_KEY_ACTION,naUintData,naIntData,naFloatData);
			return;
		}
		
		if (actorId < 0) {
			
		}
		else {
			
			
			ca = &(gw->gameObjects[actorId]);
			
			unzipBits(keyFlags,keyMapResultUnzipped,KEYMAP_LENGTH);
			
			// for (i = 0; i < KEYMAP_LENGTH; i++) {
			// 	if (keyMapResultUnzipped[i]) {
			// 		cout << i << "\n";
			// 	}
			// }
			
			//tempVec2.setFXYZ(0.0f,0.0f,0.0);
			
			// if (firstPerson) {
			// 	ca->ang = camRotX;
			// 	ca->targAng = ca->ang;
			// }
			
			// tempVec1.setFXYZ(
			// 	cos(ca->ang),
			// 	sin(ca->ang),
			// 	0.0f
			// );
			
			
			
			if (keyMapResultUnzipped[KEYMAP_RIGHT]) {
				// if (firstPerson) {
				// 	tempVec2.addXYZ(tempVec1[1],-tempVec1[0],0.0f);
				// }
				// else {
				// 	ca->targAng += (-2.0f*M_PI*timeDelta);
				// }
				
				ca->applyAngularImpulse(btVector3(0,0,-4.0f), true, 0);
			}
			
			if (keyMapResultUnzipped[KEYMAP_LEFT]) {
				// if (firstPerson) {
				// 	tempVec2.addXYZ(-tempVec1[1],tempVec1[0],0.0f);
				// }
				// else {
				// 	ca->targAng += (2.0f*M_PI*timeDelta);
				// }
				
				ca->applyAngularImpulse(btVector3(0,0,4.0f), true, 0);
			}
			
			
			
			
			
			// btTransform tr;
			// tr.setIdentity();
			// btQuaternion quat;
			// quat.setEuler(yaw,pitch,roll); //or quat.setEulerZYX depending on the ordering you want
			// tr.setRotation(quat);

			// rigidBody->setCenterOfMassTransform(tr);
			
			
			
			
			if (keyMapResultUnzipped[KEYMAP_FIRE_PRIMARY]) {
				launchBullet(actorId, E_ENTTYPE_BULLET);
			}
			
			if (keyMapResultUnzipped[KEYMAP_GRAB_THROW]) {
				grabThrowObj(actorId);
			}
			
			if (keyMapResultUnzipped[KEYMAP_UP]) {
				makeJump(actorId,1);
			}
			
			if (keyMapResultUnzipped[KEYMAP_DOWN]) {
				makeJump(actorId,0);
			}
			
			
			ca->isWalking = false;
			
			
			if (keyMapResultUnzipped[KEYMAP_FORWARD]) {
				ca->isWalking = true;
				if (ca->hasBodies()) {
					ca->applyImpulseOtherRot(
						btVector3(0,conVals[E_CONST_WALK_AMOUNT],0)*ca->getMarkerMass(),
						ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis(),
						true,
						0
					);
				}
			}
			
			if (keyMapResultUnzipped[KEYMAP_BACKWARD]) {
				ca->isWalking = true;
				if (ca->hasBodies()) {
					ca->applyImpulseOtherRot(
						btVector3(0,-conVals[E_CONST_WALK_AMOUNT],0)*ca->getMarkerMass(),
						ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis(),
						true,
						0
					);
				}
			}
			
			
			// if (keyMapResultUnzipped[KEYMAP_FORWARD]) {
			// 	ca->isWalking = true;
			// 	if (ca->hasBodies()) {
			// 		ca->makeWalk(
			// 			btVector3(
			// 				0,
			// 				conVals[E_CONST_WALK_AMOUNT],
			// 				conVals[E_CONST_WALK_AMOUNT]
			// 			),
			// 			ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis()
			// 		);
			// 	}
			// }
			
			// if (keyMapResultUnzipped[KEYMAP_BACKWARD]) {
			// 	ca->isWalking = true;
			// 	if (ca->hasBodies()) {
			// 		ca->makeWalk(
			// 			btVector3(
			// 				0,
			// 				-conVals[E_CONST_WALK_AMOUNT],
			// 				conVals[E_CONST_WALK_AMOUNT]
			// 			),
			// 			ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis()
			// 		);
			// 	}
			// }
			
			
		}
		
		
		
		//gw->updatePhys();
		
	}
	
	
	void gatherKeyActions() {
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
			getCurActorUID(),
			zipBits(keyMapResultZipped, KEYMAP_LENGTH),
			getCamRot(0),
			getCamRot(1)
		);
	}
	
	

	void handleMovement() {
		
		
		
		
		int i;
		
		
		
		
		
		
		float xrotrad = getCamRot(0);
		float yrotrad = getCamRot(1);
		
		angleToVec(&lookAtVec,xrotrad,yrotrad);
		
		lookAtVec2D.copyFrom(&lookAtVec);
		lookAtVec2D.normalizeXY();
		
		
		
		float xmod = 0.0f;
		float ymod = 0.0f;
		float zmod = 0.0f;
		
		float tempZoom = 1.0;
		
		float tempMoveSpeed;
		
		//unsigned char curKey;
		
		isPressingMove = false;
		
		btVector3 tempBTV;
		
		amountInvalidRotate = (abs(curCamRotation[0]-camRotation[0]) + abs(curCamRotation[1]-camRotation[1]));
		depthInvalidRotate = (amountInvalidRotate > 0.001f);
		
		if (smoothMove||(
			(!firstPerson) &&
			(currentActor != NULL)	
		)) {
			curCamRotation[0] += (camRotation[0]-curCamRotation[0])*timeDelta*8.0f;
			curCamRotation[1] += (camRotation[1]-curCamRotation[1])*timeDelta*8.0f;
		}
		else {
			curCamRotation[0] = camRotation[0];
			curCamRotation[1] = camRotation[1];
		}
		
		
		
		if (currentActor == NULL) {
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
		
		
		if (currentActor != NULL) {
				
			if (firstPerson) {
				targetCameraPos.setBTV(currentActor->getCenterPoint(0));
				targetCameraPos.addXYZ(0.0f,0.0f,2.0f);
			}
			else {
				targetCameraPos.copyFrom(&lookAtVec);
				targetCameraPos.multXYZ( -(subjectDistance)*subjectZoom*tempZoom );
				
				targetCameraPos.addXYZRef(BTV2FIV(currentActor->getCenterPoint(0)));
				
			}
			
			
			if (!firstPerson) {
				if (
					keysPressed[keyMap[KEYMAP_FORWARD]] ||
					keysPressed[keyMap[KEYMAP_BACKWARD]]
				) {
					isPressingMove = true;
					if (!rbDown) {
						
						tempBTV = multByOtherRot(
							btVector3(0.0f,1.0f,0.0f),
							currentActor->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis()
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
			modXYZ.multXYZ(0.5f);
			
			moveCamera(&modXYZ);
			
		}
		
	}
	
	
	
	
	bool anyMenuVisible() {
		bool doProc = false;
		
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
				if (contMenu != NULL) {
					if (contMenu->visible){
						doProc = true;
					}
				}
				// if (pickerMenu != NULL) {
				// 	if (pickerMenu->visible){
				// 		doProc = true;
				// 	}
				// }
				if (fieldMenu != NULL) {
					if (fieldMenu->visible) {
						doProc = true;
					}
				}
				
			}
		}
		
		return doProc;
		
	}
	
	void performCamShake(BaseObj* ge, float fp) {
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
	
	void explodeBullet(BaseObj* ge) {
		
		FIVector4 newPos;
		
		
		
		newPos.setBTV(ge->getCenterPoint(0));
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
		sphereStack.back().position.setBTV(ge->getCenterPoint(0));
		sphereStack.back().curRad = 1.0f;
		sphereStack.back().maxRad = explodeRad;
		sphereStack.back().radVel = 40.0f;
		sphereStack.back().radAcc = -5.0f;
		
		
		//gameFluid[E_FID_SML]->pushExplodeBullet(true,&newPos,boolToInt(waterBulletOn));
		gameFluid[E_FID_BIG]->pushExplodeBullet(true,&newPos,boolToInt(waterBulletOn));
		
		explodeStack.push_back(ExplodeStruct());
		
		if (ge->hasBodies()) {
			explodeStack.back().pos = ge->bodies[E_BDG_CENTER].body->getCenterOfMassPosition();
			explodeStack.back().radius = 20.0f;
			explodeStack.back().power = 200.0f;
		}
		
		
		gw->removeVisObject(ge->uid, true);
		
		
		
		//ge->isHidden = true;
		
		
		
		//gw->removeVisObject(ge->uid);
	}
	
	
	void grabThrowObj(int actorId) {
		
		int res;
		
		if (actorId < 0) {
			return;
		}
		
		BaseObj* ca = &(gw->gameObjects[actorId]);
		GameOrg* curOrg = gameOrgs[ca->orgId];
		
		// ca->weaponActive = !ca->weaponActive;
		
		// return;
		
		
		if (ca->isGrabbingId >= 0) {
			// throw current obj
			
			
			
			//##
			
			// gw->gameObjects[ca->isGrabbingId].setVel(
			// 	cos(ca->ang)*20.0f,
			// 	sin(ca->ang)*20.0f,
			// 	30.0f	
			// );
			
			
			if (ca->hasBodies()) {
				gw->gameObjects[ca->isGrabbingId].applyImpulseOtherRot(
					btVector3(0.0,200,200)*gw->gameObjects[ca->isGrabbingId].getTotalMass(),
					ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis(),
					true,
					0
				);
			}
			
			
			playSoundEnt(
				"woosh0",
				ca,
				0.2f
			);
			
			ca->weaponActive = false;
			
			
			
			//gw->gameObjects[ca->isGrabbingId].setDamping(0.1f,0.9f);
			
			//##
			
			
			gw->gameObjects[ca->isGrabbingId].isGrabbedById = -1;
			ca->isGrabbingId = -1;
			
			
		}
		else {
			// find obj to pickup
			
			
			
			
			
			
			
			
			res = gw->getClosestObj(actorId, BTV2FIV(ca->getCenterPoint(0)));
			
			if (res < 0) {
				
			}
			else {
				
				curOrg->stepCount = 0;
				curOrg->totTime = 0;
				ca->isPickingUp = true;
				
				playSoundEnt(
					"scrape0",
					ca,
					0.2f
				);
				
				
				//gw->gameObjects[ca->isGrabbingId].setDamping(999.0f,999.0f);
				ca->weaponActive = true;
				ca->isGrabbingId = res;
				gw->gameObjects[res].isGrabbedById = actorId;
			}
			
			
		}
		
	}
	
	void launchBullet(int actorId, int bulletType) {
		
		int entNum;
		
		int vx;
		int vy;
		
		
		FIVector4 newCellPos;
		FIVector4 newVel;
		
		if (actorId < 0) {
			return;
		}
		
		
		
		BaseObj* ca = &(gw->gameObjects[actorId]);
		
		if (ca == NULL) {
			
		}
		else {
			
			if (ca->hasBodies()) {
			
				newCellPos.setBTV(ca->getCenterPoint(0));
				
				// vx = cos(ca->ang)*3.0f;
				// vx = vx;
				
				// vy = sin(ca->ang)*3.0f;
				// vy = vy;
				
				
				btVector3 tempBTV = multByOtherRot(
					btVector3(0.0f,3.0f,3.0f),
					ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis()
				);
				newCellPos.addXYZ(tempBTV.getX(), tempBTV.getY(), tempBTV.getZ());
				
				entNum = placeNewEnt(false, bulletType, &newCellPos);
				
				
				
				// gw->gameObjects[entNum].setVel(
				// 	cos(ca->ang)*20.0f,
				// 	sin(ca->ang)*20.0f,
				// 	30.0f
				// );
			
			
				gw->gameObjects[entNum].applyImpulseOtherRot(
					btVector3(0.0,200,200)*gw->gameObjects[entNum].getTotalMass(),
					ca->bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis(),
					true,
					0
				);
			}
			
			
			
			if (bulletType != E_ENTTYPE_TRACE) {
				playSoundEnt(
					"woosh0",
					ca,
					0.2f
				);
			}
			
			
			
		}
	}
	

	

	
	void resetActiveNode() {
		
		GameOrgNode* curNode = NULL;
		
		GameOrg* testHuman = getCurOrg();
		
		if (selectedNode == NULL) {
			
			curNode = lastSelNode;
		}
		else {
			
			curNode = selectedNode;
		}
		
		
		
		if (curNode != NULL) {
			
			
			
			if (testHuman != NULL) {
				
				
				
				testHuman->setToPose(gamePoses[E_PK_T_POSE],1.0,curNode->nodeName);
			}
			
			//curNode->rotThe = 0.0f;
			//curNode->rotPhi = 0.0f;
			//curNode->rotRho = 0.0f;
			//curNode->boneLengthScale = 1.0f;
			
			//curNode->tbnRadScale0.setFXYZ(1.0f,1.0f,1.0f);
			//curNode->tbnRadScale1.setFXYZ(1.0f,1.0f,1.0f);
			//makeDirty();
			
			
			
		}
	}
	
	bool updateNearestOrgNode(bool setActive, FIVector4* mousePosWS) {
		// tempVec3.setFXYZRef(mousePosWS);
		// tempVec3.addXYZRef(&(testHuman->basePosition),-1.0f);
		
		//worldToScreenBase(&tempVec1, mousePosWS);
		
		GameOrgNode* mirNode = NULL;
		
		if (getCurOrg() == NULL) {
			return false;
		}
		GameOrg* testHuman = getCurOrg();
		
		bestNode = NULL;
		
		int boneId;
		
		highlightedLimb2 = -1;
		highlightedLimb = -1;
		
		gamePhysics->pickBody(&mouseMoveOPD);  //mouseDownPD.getBTV(),mouseDownOPD.getBTV());
		
		if (gamePhysics->lastBodyPick == NULL) {
						
		}
		else {
			highlightedLimb = gamePhysics->lastBodyPick->limbUID;
			
			if (highlightedLimb > -1) {
				boneId = currentActor->bodies[highlightedLimb].boneId;
				
				
				if (boneId > -1) {
					bestNode = testHuman->allNodes[boneId];
					
					// if (mirrorOn) {
					// 	mirNode = getMirroredNode(bestNode);
						
					// 	if (mirNode == NULL) {
							
					// 	}
					// 	else {
					// 		mirNode->nodeName
					// 	}
						
					// }
					
					setSelNode(bestNode);
					if (setActive) {
						activeNode = bestNode;
					}
					return true;
				}
			}
			
		}
		
		
		bestNode = NULL;
		activeNode = NULL;
		setSelNode(NULL);
		return false;
		
		
		// bestNodeDis = 99999.0f;
		// findNearestOrgNode(
		// 	testHuman->baseNode,
		// 	mousePosWS//&tempVec1
		// );
		
		// //cout << "bestNodeDis " << bestNodeDis << "\n";
		
		// if (bestNodeDis >= 3.0f) {
		// 	bestNode = NULL;
		// 	activeNode = NULL;
		// 	setSelNode(NULL);
		// }
		
		// if (bestNode != NULL) {
			
		// 	setSelNode(bestNode);
		// 	if (setActive) {
		// 		activeNode = bestNode;				
		// 	}
		// }
		
		//return (bestNode != NULL);
	}
	
	// void findNearestOrgNode(
	// 	GameOrgNode* curNode,
	// 	FIVector4* mousePosWS
	// ) {
		
	// 	if (getCurOrg() == NULL) {
	// 		return;
	// 	}
	// 	GameOrg* testHuman = getCurOrg();
		
	// 	tempVec3.setFXYZRef(&(curNode->orgTrans[1]));
	// 	tempVec3.addXYZRef(&(testHuman->basePosition));
		
	// 	//worldToScreenBase(&tempVec2, &tempVec3);
	// 	float curDis = mousePosWS->distance(&tempVec3);//&tempVec2);
		
	// 	if (curDis < bestNodeDis) {
	// 		bestNodeDis = curDis;
	// 		bestNode = curNode;
	// 	}
		
	// 	int i;
		
	// 	for (i = 0; i < curNode->children.size(); i++) {
	// 		findNearestOrgNode(curNode->children[i],mousePosWS);
	// 	}
		
	// }


	// void processB64(charArr *sourceBuffer, charArr *saveBuffer)
	// {

	// 	char *buf = sourceBuffer->data;
	// 	int len = sourceBuffer->size;

	// 	if (saveBuffer != &nullBuffer)
	// 	{
	// 		if (saveBuffer->data != NULL)
	// 		{
	// 			delete[] saveBuffer->data;
	// 			saveBuffer->data = NULL;
	// 		}
	// 		saveBuffer->data = new char[len];
	// 		strncpy(saveBuffer->data, buf, len);
	// 		saveBuffer->size = len;
	// 	}

	// 	FBOSet *fbos = getFBOByName("palFBO");


	// 	//unsigned char* resultImage = new unsigned char[256*256*4];

	// 	membuf inBuffer(sourceBuffer->data, sourceBuffer->data + sourceBuffer->size);
	// 	std::istream myIS(&inBuffer);
	// 	Poco::Base64Decoder b64in(myIS);



	// 	std::ostringstream oss;
	// 	oss << b64in.rdbuf();

	// 	std::string strConst = oss.str();
	// 	const char *inString = strConst.c_str();


	// 	lodepng_decode32(&resultImage, &palWidth, &palHeight, (unsigned char *)inString, strConst.size() );

	// 	fbos->copyFromMem(0, resultImage);

	// }

	void getJVNodeByString(
		JSONValue* rootNode,
		JSONValue** resultNode,
		string stringToSplit
		//, bool dd = false
	) {
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
	
	
	void closeAllContainers() {
		BaseObj* curCont;
		
		bool oldOpen;
		bool didClose = false;
		
		for (itBaseObj iterator = gw->gameObjects.begin(); iterator != gw->gameObjects.end(); iterator++) {
			// iterator->first = key
			// iterator->second = value
			
			
			
			curCont = &(gw->gameObjects[iterator->first]);
			oldOpen = curCont->isOpen;
			
			curCont->isOpen = false;
			
			
			if (oldOpen != curCont->isOpen) {
				didClose = true;
			}
		}
		
		if (didClose) {
			playSoundEnt("leather0", NULL, 0.1);
		}
	}
	
	bool anyContainerOpen() {
		BaseObj* curCont;
		for (itBaseObj iterator = gw->gameObjects.begin(); iterator != gw->gameObjects.end(); iterator++) {
			// iterator->first = key
			// iterator->second = value
			
			curCont = &(gw->gameObjects[iterator->first]);
			
			if (curCont->isOpen) {
				return true;
			}
		}
		return false;
	}

	void cleanJVPointer(JSONValue** jv) {
		
		if (*jv != NULL) {
			delete *jv;
		}
		
		*jv = NULL;
		
	}

	string getStringForObjectId(int objectId) {
		int objType = gw->gameObjects[objectId].objectType;
		int iconNum = entIdToIcon[objType];
		
		// if (isContainer[objType]) {
		// 	return i__s(iconNum) + "& Test Container ";
		// }
		// else {
		// 	if (gw->gameObjects[objectId].isEquipped) {
		// 		return "(E) " + i__s(iconNum) + "& Test Object ";
		// 	}
		// 	else {
		// 		return i__s(iconNum) + "& Test Object ";
		// 	}
		// }
		
		if (gw->gameObjects[objectId].isEquipped) {
			return "(E) " + i__s(iconNum) + "& "+objStrings[objType]+" ";
		}
		else {
			return i__s(iconNum) + "& "+objStrings[objType]+" ";
		}
		
	}

	void getObjectData() {//int paramVal) {
		int i;
		int objectType;
		int childId;
		
		JSONValue* tempVal0;
		JSONValue* tempVal1;
		BaseObj* curCont;
		
		
		cleanJVPointer(&(externalJSON["objectData"].jv));
		
		
		externalJSON["objectData"].jv = new JSONValue(JSONObject());
		externalJSON["objectData"].jv->object_value["objects"] = new JSONValue(JSONArray());
		tempVal0 = externalJSON["objectData"].jv->object_value["objects"];
		
		for (itBaseObj iterator = gw->gameObjects.begin(); iterator != gw->gameObjects.end(); iterator++) {
			// iterator->first = key
			// iterator->second = value
			
			curCont = &(gw->gameObjects[iterator->first]);
			
			if (curCont->isOpen) {
				
				tempVal0->array_value.push_back( new JSONValue(JSONObject()) );
				
				tempVal0->array_value.back()->object_value["children"] = new JSONValue(JSONArray());
				tempVal0->array_value.back()->object_value["objectId"] = new JSONValue( ((double)(iterator->first)) );
				tempVal1 = tempVal0->array_value.back()->Child("children");
				
				
				for (i = 0; i < curCont->children.size(); i++) {
					tempVal1->array_value.push_back(new JSONValue(JSONObject()));
					childId = curCont->children[i];
					objectType = gw->gameObjects[childId].objectType;
					tempVal1->array_value.back()->object_value["objectType"] = new JSONValue( ((double)(objectType)) );
					tempVal1->array_value.back()->object_value["objectId"] = new JSONValue( ((double)(childId)) );
				}
			}
		}
	}
	
	

	JSONValue* fetchJSONData(string dataFile, bool doClean, JSONValue* params = NULL) {
		
		
		bool doLoad = false;
		bool loadRes = false;
		
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
			if (dataFile.compare("shaderParams") == 0) {
				cout << "attempted to load shaderParams\n";
			}
			else if (dataFile.compare("objectData") == 0) {
				getObjectData();//params->number_value);
			}
			else {
				cout << "load jv data "  + dataFile << "\n";
				loadRes = loadJSON(
					"..\\data\\" + dataFile,
					&((externalJSON[dataFile]).jv)
				);
				
				if (loadRes == false) {
					return NULL;	
				}
				
			}
			
			
		}
		
		
		
		return (externalJSON[dataFile]).jv;
	}

	bool processJSONFromString(
		string* sourceBuffer,
		JSONValue** destObj
	) {
		cleanJVPointer(destObj);
		
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

	bool processJSON(
		charArr *sourceBuffer,
		charArr *saveBuffer,
		JSONValue** destObj
	) {
		
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
	
	
	bool loadJSON(
		string path,
		JSONValue** destObj
	) {
		
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
	
	
	void setGUIText(
		string key,
		string stringValue,
		float floatValue = 0.0f,
		bool applyVal = false,
		bool applyString = true
	) {
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
	
	float getGUIValue(string key) {
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
	
	UIComponent* getGUIComp(string key) {
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
	
	void setGUIValue(
		string key,
		float floatValue,
		bool dispatchEvent = false,
		bool preventRefresh = false
	) {
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
	
	
	void loadValuesGUI(bool applyValues = false) {
		
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
	
	
	void saveExternalJSON() {
		
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
	
	
	void saveGUIValues() {
		
		cout << "Saving GUI Values\n";
		
		stringBuf = "";
		
		for(itUICStruct iterator = compMap.begin(); iterator != compMap.end(); iterator++) {
				
				if (iterator->second.nodeId < 0) {
					
				}
				else {
					if (iterator->first[0] ==  '$') {
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
	
	
	void updateGUI() {
		
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
	
	
	
	void beginFieldInput(string defString, int cb) {
		currentFieldString = defString;
		
		fieldCallback = cb;
		
		inputOn = true;
		fieldMenu->visible = true;
		
		if (fieldText != NULL) {
			if (currentFieldString.compare("") == 0) {
				fieldText->setText(" ");
			}
			else {
				fieldText->setText(currentFieldString);
			}
			
		}
	}
	
	void processFieldInput(unsigned char key) {
		
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
	
	GameOrg* getCurOrg() {
		if (currentActor == NULL) {
			return NULL;
		}
		if (currentActor->orgId < 0) {
			return NULL;
		}
		return gameOrgs[currentActor->orgId];
	}
	
	void endFieldInput(bool success) {
		
		if (getCurOrg() == NULL) {
			return;
		}
		GameOrg* testHuman = getCurOrg();
		
		inputOn = false;
		fieldMenu->visible = false;
		
		float tempVal;
		
		if (success) {
			switch (fieldCallback) {
				case E_FC_SAVEORG:
				
					tempVal = testHuman->baseNode->orgVecs[E_OV_THETAPHIRHO].getFZ();
				
					testHuman->baseNode->orgVecs[E_OV_THETAPHIRHO].setFZ(0.0f);
					transformOrg(testHuman);
									
					testHuman->saveToFile(currentFieldString);
					
					testHuman->baseNode->orgVecs[E_OV_THETAPHIRHO].setFZ(tempVal);
					transformOrg(testHuman);
					
				break;
				case E_FC_LOADORG:
					testHuman->loadFromFile(currentFieldString, false);
					//orientRotation();
					if (currentActor != NULL) {
						//currentActor->curRot = 1;
					}
					transformOrg(testHuman);
					makeDirty();
					
				break;
				
			}
		}
		
	}
	
	void saveOrg() {
		beginFieldInput("",E_FC_SAVEORG);
	}
	
	void loadOrg() {
		beginFieldInput("",E_FC_LOADORG);
	}
	
	
	float getConst(string conName) {
		if (constRootJS == NULL) {
			
		}
		else {
			if (constRootJS->HasChild(conName)) {
				return constRootJS->Child(conName)->number_value;
			}
			else {
				
			}
		}
		return 0.0f;
	}
	
	void loadConstants() {
		int i;
		if (loadJSON("..\\data\\constants.js", &constRootJS)) {
			
			for (i = 0; i < E_CONST_LENGTH; i++) {
				conVals[i] = getConst(constStrings[i]);
			}
			
		}
	}

	void loadGUI() {
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
		
		mapComp = getGUIComp("map.mapHolder");
		mainMenu = getGUIComp("guiHandles.mainMenu");
		ddMenu = getGUIComp("guiHandles.ddMenu");
		contMenu = getGUIComp("guiHandles.contMenu");
		//pickerMenu = getGUIComp("guiHandles.pickerMenu");
		fieldMenu = getGUIComp("guiHandles.fieldMenu");
		fieldText = getGUIComp("fieldMenu.field");
		
		// if (pickerMenu != NULL) {
		// 	pickerMenu->visible = false;
		// }
		if (mainMenu != NULL) {
			mainMenu->visible = false;
		}
		if (ddMenu != NULL) {
			ddMenu->visible = false;
		}
		if (fieldMenu != NULL) {
			fieldMenu->visible = false;
		}
		
		updateMatFlag = true;
		
		
	}


	// void loadAllData()
	// {
	// 	// if ( loadFile("..\\data\\lastJSONBuffer.js", &lastJSONBuffer) )
	// 	// {
	// 	// 	processJSON(&lastJSONBuffer, &nullBuffer, &rootObjJS);
	// 	// }

	// 	// if ( loadFile("..\\data\\lastImageBuffer.txt", &lastImageBuffer) )
	// 	// {
	// 	// 	processB64(&lastImageBuffer, &nullBuffer);
	// 	// }

	// }

	// void saveAllData()
	// {
		
		
	// 	// saveFile("..\\data\\lastJSONBuffer.js", &lastJSONBuffer);
	// 	// saveFile("..\\data\\lastImageBuffer.txt", &lastImageBuffer);
		
	// 	// cout << "All Data Saved\n";
	// }


	string loadFileString(string fnString) {
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


	std::ifstream::pos_type filesize(const char* filename)
	{
	    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	    return in.tellg(); 
	}

	bool loadFile(string fnString, charArr *dest)
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

	bool saveFileString(string fileName, string* source)
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



	float getUnderWater() {
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

	void updateAmbientSounds() {
		
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
	
	
	
	
	void frameUpdate() {
		
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
				if (currentActor != NULL) {
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
					
					
					
					if (currentActor != NULL) {
						if ((currentTick%10) == 0) {
							
							// if (rbDown) {
							// 	launchBullet(currentActor->uid, E_ENTTYPE_TRACE);
							// }
							
							
						}
					}
					
					if (currentTick > 2) {
						
						
						updateCamVals();
						
						
						
						
						// if (currentTick < 2) {
						// 	gw->update();
						// }
						
						if (currentTick < 4) {
							cameraGetPosNoShake()->setFXYZ(2048.0,2048.0,0.0);
							camLerpPos.copyFrom(cameraGetPosNoShake());
						}
						
						if (currentTick == 4) {
							setCameraToElevation();
							
							gamePhysics = new GamePhysics();
							gamePhysics->init(this);
							
						}
						
						if (
							(currentActor != NULL) &&
							currentActor->hasBodies()	
						) {
							
							if (currentActor->bodies[E_BDG_CENTER].inWater) {
								temp = clampfZO(
									currentActor->getVel(0)->length()
								)*0.25f;
								temp2 = 0.0f;
							}
							else {
								
								if (currentActor->allFalling()) {
									temp2 = 0.0f;
								}
								else {
									temp2 = clampfZO(
										currentActor->getVel(0)->length()
									);
								}
								
								temp = 0.0f;
							}
							
							
							
							updateSoundPosAndPitch(
								"swimming0",
								cameraGetPosNoShake(),
								BTV2FIV(currentActor->getCenterPoint(0)),
								temp,
								0.01
							);
							updateSoundPosAndPitch(
								"walkinggravel0",
								cameraGetPosNoShake(),
								BTV2FIV(currentActor->getCenterPoint(0)),
								temp2,
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
						
						gw->update();
						
						if (GEN_POLYS_WORLD) {
							gw->generateBlockHolder();
						}
						
						
					}
					
					
					
				}
			}
			
		}
		
		
		
		
		
		
		
		
		
		
		TRACE_ON = false;
		
		
		frameCount++;
	}
	

	
	FIVector4* cameraGetPos() {
		return &resultCameraPos;
	}	
	FIVector4* cameraGetPosNoShake() {
		return cameraPos;
	}

	float getTargetTimeOfDay() {
		return 1.0f-(lightVecOrig.getFZ() + 1.0f)*0.5f;
	}

	void updateBullets() {
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

	void display(bool doFrameRender)
	{
		
		bool noTravel = false;
		
		frameSkipCount++;
		
		
		curTime = myTimer.getElapsedTimeInMilliSec();
		smoothTime = (sin(curTime/1000.0)+1.0f)*0.5f;
		
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
		totTimePassedPhysics += curTimePassed*SPEEDUP_FACTOR;
		
		
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
				
				
				flushKeyStack();
				gatherKeyActions();
				handleMovement();
				
				
				gamePhysics->updateAll();
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
				isDraggingObject && 
				(draggingFromType != E_DT_NOTHING) &&
				((curTime-mdTime) > 300) &&
				(!editPose)
			) {
				glutSetCursor(GLUT_CURSOR_CROSSHAIR);
			
				mouseMoveVec.setFXYZ(lastMouseX,lastMouseY,0.0);
				noTravel = mouseMoveVec.distance(&mouseStart) < MAX_TRAVEL_DIS;	
				markerFound = !noTravel;
				getPixData(&worldMarker, lastMouseX, lastMouseY, true, false);
			
			}
			else {
				markerFound = (ddMenu->visible)&&(selObjInd < E_OBJ_LENGTH);
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
	
	
	
	bool gluInvertMatrix(const double m[16], float invOut[16])
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
	
	
	int getMatrixInd(int col, int row) {
		return col*4 + row;
	}
	
	void ComputeFOVProjection(
		float* result,
		float fov,
		float aspect,
		float nearDist,
		float farDist,
		bool leftHanded
	) {
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
	
	void setMatrices(int w, int h)
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
	
	void reshape(int w, int h)
	{

		setWH(w, h);

		screenWidth = w;
		screenHeight = h;
		
		setMatrices(baseW, baseH);
	}
	
	void initAllObjects() {
		int i;
		int j;
		int k;
		int itemCount = 0;
		
		for (i = 0; i < MAX_ICON_ID; i++) {
			iconToEntId[i] = 0;
		}
		
		for (i = 0; i < MAX_OBJ_TYPES; i++) {
			entIdToIcon[i] = 0;
		}
		
		
		
		itemCount = numberIcons(itemCount,0,0,11,20);
		itemCount = numberIcons(itemCount,12,0,23,15);
		itemCount = numberIcons(itemCount,24,0,35,16);
		itemCount = numberIcons(itemCount,12,16,21,20);
		itemCount = numberIcons(itemCount,22,17,35,31);
		itemCount = numberIcons(itemCount,0,21,15,31);
		itemCount = numberIcons(itemCount,16,21,21,22);
		itemCount = numberIcons(itemCount,0,32,35,35);
		itemCount = numberIcons(itemCount,0,36,15,47);
		itemCount = numberIcons(itemCount,16,36,35,47);
		
		
		for (i = 0; i < MAX_OBJ_TYPES; i++) {
			isContainer[i] = false;
			objStrings[i] = "";
		}
		for (i = 360; i <= 419; i++ ) {
			isContainer[i] = true;
		}
		for (i = 1240; i <= 1671; i++ ) {
			isContainer[i] = true;
		}
		for (i = 525; i <= 527; i++ ) {
			isContainer[i] = true;
		}
		for (i = 537; i <= 539; i++ ) {
			isContainer[i] = true;
		}
		
		
		for (i = 0; i <= 35; i++) {
			objStrings[i] = gemStrings[i%12] + " and Gold Ring";
		}
		
		for (i = 36; i <= 71; i++) {
			objStrings[i] = gemStrings[i%12] + " and Silver Ring";
		}
		
		for (i = 180; i <= 191; i++) {
			objStrings[i] = gemStrings[i%12] + " Necklace";
		}
		
		for (i = 420; i <= 431; i++) {
			objStrings[i] = gemStrings[i%12] + " Ore";
		}
		
		for (i = 432; i <= 443; i++) {
			objStrings[i] = "Polished " + gemStrings[i%12];
		}
		
		for (i = 72; i <= 419; i++) {
			
			j = i/12;
			
			if (j == 15) {
				
			}
			else {
				objStrings[i] = colorStrings[i%12] + " ";
			}
			
			
			switch (j) {
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
					objStrings[i] += "Cloak";
				break;
				case 11:
					objStrings[i] += "Plate Armor";
				break;
				case 12:
					objStrings[i] += "Leather Armor";
				break;
				case 13:
					objStrings[i] += "Buckler";
				break;
				case 14:
					objStrings[i] += "Kite Shield";
				break;
				case 15:
				
				break;
				case 16:
					objStrings[i] += "Bandana";
				break;
				case 17:
					objStrings[i] += "Boot";
				break;
				case 18:
					objStrings[i] += "Pointed Cap";
				break;
				case 19:
					objStrings[i] += "Plumed Helm";
				break;
				case 20:
					objStrings[i] += "Sailor Cap";
				break;
				case 21:
				case 22:
				case 23:
					objStrings[i] += "Book";
				break;
				case 24:
					objStrings[i] += "Bound Scroll";
				break;
				case 25:
					objStrings[i] += "Scroll";
				break;
				case 26:
				case 27:
					objStrings[i] += "Parchment";
				break;
				case 28:
				case 29:
					objStrings[i] += "Scroll";
				break;
				case 30:
					objStrings[i] += "Bag";
				break;
				case 31:
				case 32:
					objStrings[i] += "Satchel";
				break;
				case 33:
				case 34:
					objStrings[i] += "Gift Box";
				break;
			}
		}
		
		for (i = 698; i <= 907; i++) {
			objStrings[i] = colorStrings[(i+2)%14] + " Potion";
		}
		
		
		for (i = 648; i <= 697; i++) {
			
			j = (i+2)%10;
			
			k = ((i+2)/10) - 65;
			
			objStrings[i] = metalStrings[k] + " ";
			
			
			switch (j) {
				case 0:
					objStrings[i] += "Bullion";
				break;
				case 1:
					objStrings[i] += "Denarii (100)";
				break;
				case 2:
					objStrings[i] += "Denarii (10)";
				break;
				case 3:
					objStrings[i] += "Denarius";
				break;
				case 4:
					objStrings[i] += "Decima";
				break;
				case 5:
					objStrings[i] += "Cent";
				break;
				case 6:
					objStrings[i] += "Medal";
				break;
				case 7:
				case 8:
				case 9:
					objStrings[i] += "Key";
				break;
			}
		}
		
		for (i = 1084; i <= 1095; i++) {
			objStrings[i] = "Parchment";
		}
		
		
		
		for (i = 908; i <= 1083; i++) {
			
			j = (i+4)%16;
			k = ((i+4)/16) - 57;
			
			
			objStrings[i] = elementStrings[j] + " " + weaponStrings[k];
		}
		
		
		objStrings[444] = "Leather Helm";
		objStrings[445] = "Leather Helm";
		objStrings[446] = "Leather Helm";
		objStrings[447] = "Leather Helm";
		objStrings[448] = "Iron Helm";
		objStrings[449] = "Iron Helm";
		objStrings[450] = "Iron Helm";
		objStrings[451] = "Artifact Helm";
		objStrings[452] = "Artifact Helm";
		objStrings[453] = "Artifact Helm";
		objStrings[454] = "Pointed Cap";
		objStrings[455] = "Feathered Cap";
		objStrings[456] = "Cap";
		objStrings[457] = "Mask";
		objStrings[458] = "Top Hat";
		objStrings[459] = "Feathered Hat";
		objStrings[460] = "Cat Ears";
		objStrings[461] = "Rabbit Ears";
		objStrings[462] = "Headband";
		objStrings[463] = "Crown";
		objStrings[464] = "Fur Cap";
		objStrings[465] = "Cap";
		objStrings[466] = "Hat";
		objStrings[467] = "Leather Cap";
		objStrings[468] = "Tunic";
		objStrings[469] = "Tunic";
		objStrings[470] = "Fur Coat";
		objStrings[471] = "Overcoat";
		objStrings[472] = "Leather Armor";
		objStrings[473] = "Leather Armor";
		objStrings[474] = "Iron Plated Armor";
		objStrings[475] = "Iron Plated Armor";
		objStrings[476] = "Iron Plated Armor";
		objStrings[477] = "Artifact Armor";
		objStrings[478] = "Artifact Armor";
		objStrings[479] = "Artifact Armor";
		objStrings[480] = "Slipper";
		objStrings[481] = "Boot";
		objStrings[482] = "Buckled Boot";
		objStrings[483] = "Fur Boot";
		objStrings[484] = "Buckled Boot";
		objStrings[485] = "Shoe";
		objStrings[486] = "Iron Plated Boot";
		objStrings[487] = "Iron Plated Boot";
		objStrings[488] = "Iron Plated Boot";
		objStrings[489] = "Artifact Boot";
		objStrings[490] = "Artifact Boot";
		objStrings[491] = "Artifact Boot";
		objStrings[492] = "Wooden Buckler";
		objStrings[493] = "Wooden Divoted Buckler";
		objStrings[494] = "Wood and Iron Buckler";
		objStrings[495] = "Iron Buckler";
		objStrings[496] = "Iron Buckler";
		objStrings[497] = "Iron Tower Shield";
		objStrings[498] = "Wooden Heater Shield";
		objStrings[499] = "Wood and Iron Heater Shield";
		objStrings[500] = "Iron Heater Shield";
		objStrings[501] = "Decorated Heater Shield";
		objStrings[502] = "Kite Shield";
		objStrings[503] = "Decorated Kite Shield";
		objStrings[504] = "Artifact Shield";
		objStrings[505] = "Artifact Shield";
		objStrings[506] = "Artifact Shield";
		objStrings[507] = "Artifact Shield";
		objStrings[508] = "Belt";
		objStrings[509] = "Belt";
		objStrings[510] = "Belt";
		objStrings[511] = "Artifact Necklace";
		objStrings[512] = "Artifact Necklace";
		objStrings[513] = "Artifact Necklace";
		objStrings[514] = "Artifact Necklace";
		objStrings[515] = "Artifact Necklace";
		objStrings[516] = "Gold Ring";
		objStrings[517] = "Gold Ring";
		objStrings[518] = "Gold Ring";
		objStrings[519] = "Gold Ring";
		objStrings[520] = "Gold Ring";
		objStrings[521] = "Gold Ring";
		objStrings[522] = "Gold Ring";
		objStrings[523] = "Gold Ring";
		objStrings[524] = "Gold Ring";
		objStrings[525] = "Box";
		objStrings[526] = "Box";
		objStrings[527] = "Bag";
		objStrings[528] = "Silver Ring";
		objStrings[529] = "Silver Ring";
		objStrings[530] = "Silver Ring";
		objStrings[531] = "Silver Ring";
		objStrings[532] = "Silver Ring";
		objStrings[533] = "Silver Ring";
		objStrings[534] = "Silver Ring";
		objStrings[535] = "Silver Ring";
		objStrings[536] = "Silver Ring";
		objStrings[537] = "Box";
		objStrings[538] = "Box";
		objStrings[539] = "Bag";
		
		objStrings[540] = "Canine";
		objStrings[541] = "Patch of Fur";
		objStrings[542] = "Hide";
		objStrings[543] = "Claw";
		objStrings[544] = "Feather";
		objStrings[545] = "Horn";
		objStrings[546] = "Mushroom Cap";
		objStrings[547] = "Shell";
		objStrings[548] = "Bone";
		objStrings[549] = "Eyeball";
		objStrings[550] = "Tentacle";
		objStrings[551] = "Bat Wing";
		
		objStrings[552] = "Molar";
		objStrings[553] = "Patch of Fur";
		objStrings[554] = "Hide";
		objStrings[555] = "Claw";
		objStrings[556] = "Feather";
		objStrings[557] = "Horn";
		objStrings[558] = "Mushroom Cap";
		objStrings[559] = "Shell";
		objStrings[560] = "Bone";
		objStrings[561] = "Eyeball";
		objStrings[562] = "Tentacle";
		objStrings[563] = "Bat Wing";
		
		objStrings[564] = "Candle";
		objStrings[565] = "Jelly";
		objStrings[566] = "Mirror";
		objStrings[567] = "Flask";
		objStrings[568] = "Yarn";
		objStrings[569] = "Button";
		objStrings[570] = "Cloth";
		objStrings[571] = "Bell";
		objStrings[572] = "Wood";
		objStrings[573] = "Beak";
		objStrings[574] = "Tail";
		objStrings[575] = "Claw";
		
		objStrings[576] = "Candle";
		objStrings[577] = "Jelly";
		objStrings[578] = "Mirror";
		objStrings[579] = "Flask";
		objStrings[580] = "Yarn";
		objStrings[581] = "Button";
		objStrings[582] = "Cloth";
		objStrings[583] = "Bell";
		objStrings[584] = "Wood";
		objStrings[585] = "Beak";
		objStrings[586] = "Tail";
		objStrings[587] = "Claw";
		
		objStrings[588] = "Apple";
		objStrings[589] = "Lime";
		objStrings[590] = "Orange";
		objStrings[591] = "Passion Fruit";
		objStrings[592] = "Pineapple";
		objStrings[593] = "Banana";
		objStrings[594] = "Cherries";
		objStrings[595] = "Watermelon";
		objStrings[596] = "Bread";
		objStrings[597] = "Cooked Lamb Shank";
		objStrings[598] = "Cooked Egg";
		objStrings[599] = "Cooked Fish";
		objStrings[600] = "Cooked Chicken";
		objStrings[601] = "Sandwich";
		objStrings[602] = "Sliced Potato";
		objStrings[603] = "Steak";
		objStrings[604] = "Sliced Apple";
		objStrings[605] = "Sliced Lime";
		objStrings[606] = "Sliced Orange";
		objStrings[607] = "Sliced Passion Fruit";
		objStrings[608] = "Sliced Pineapple";
		objStrings[609] = "Peeled Banana";
		objStrings[610] = "Cherry";
		objStrings[611] = "Sliced Watermelon";
		objStrings[612] = "Cookie";
		objStrings[613] = "Candy";
		objStrings[614] = "Candy Cane";
		objStrings[615] = "Slice of Cake";
		objStrings[616] = "Dark Chocolate";
		objStrings[617] = "Lollipop";
		objStrings[618] = "Icecream";
		objStrings[619] = "Honey";
		objStrings[620] = "Half Cookie";
		objStrings[621] = "Candy";
		objStrings[622] = "Candy Cane";
		objStrings[623] = "Cake";
		objStrings[624] = "Milk Chocolate";
		objStrings[625] = "Lollipop";
		objStrings[626] = "Icecream";
		objStrings[627] = "Water";
		objStrings[628] = "Cheese";
		objStrings[629] = "Raw Lamb Shank";
		objStrings[630] = "Raw Egg";
		objStrings[631] = "Raw Fish";
		objStrings[632] = "Cooked Chicken";
		objStrings[633] = "Sandwich";
		objStrings[634] = "Potato";
		objStrings[635] = "Raw Steak";
		objStrings[636] = "Cut Emerald";
		objStrings[637] = "Cut Ruby";
		objStrings[638] = "Cut Sapphire";
		objStrings[639] = "Cut Amethyst";
		objStrings[640] = "Cut Beryl";
		objStrings[641] = "Cut Topaz";
		objStrings[642] = "Cut Onyx";
		objStrings[643] = "Cut Diamond";
		objStrings[644] = "Glass";
		objStrings[645] = "Pearl";
		objStrings[646] = "Ash";
		objStrings[647] = "Flint";
		
		
		
		for (i = 0; i < 3; i++) {
			j = i*16;
			objStrings[1240 + j] = "Giant Ant";
			objStrings[1242 + j] = "Giant Rat";
			objStrings[1244 + j] = "Slime";
			objStrings[1246 + j] = "Giant Larva";
			objStrings[1248 + j] = "Giant Wasp";
			objStrings[1250 + j] = "Dread Knight";
			objStrings[1252 + j] = "Carnivorous Plant";
			objStrings[1254 + j] = "Haunted Stump";
			
			objStrings[1288 + j] = "Floating Eye";
			objStrings[1290 + j] = "Gazer";
			objStrings[1292 + j] = "Skeleton";
			objStrings[1294 + j] = "Ghost";
			objStrings[1296 + j] = "Animated Fungus";
			objStrings[1298 + j] = "Necromancer";
			objStrings[1300 + j] = "Electric Eye";
			objStrings[1302 + j] = "Mimic";
			
			objStrings[1336 + j] = "Fire Elemental";
			objStrings[1338 + j] = "Wind Elemental";
			objStrings[1340 + j] = "Earth Elemental";
			objStrings[1342 + j] = "Water Elemental";
			objStrings[1344 + j] = "Golem";
			objStrings[1346 + j] = "Zombie";
			objStrings[1348 + j] = "Imp";
			objStrings[1350 + j] = "Cyclopes";
			
			objStrings[1384 + j] = "Hatchling";
			objStrings[1386 + j] = "Giant Crab";
			objStrings[1388 + j] = "Giant Snake";
			objStrings[1390 + j] = "Giant Frog";
			objStrings[1392 + j] = "Giant Snail";
			objStrings[1394 + j] = "Dark Lord";
			objStrings[1396 + j] = "Animated Armor";
			objStrings[1398 + j] = "Banshee";
		}
		
		for (i = 0; i < 4; i++) {
			j = i*20;
			
			objStrings[1432 + j] = "Female Townsperson";
			objStrings[1436 + j] = "Male Townsperson";
			objStrings[1440 + j] = "Priest";
			objStrings[1444 + j] = "Rogue";
			objStrings[1448 + j] = "Theif";
			
			objStrings[1512 + j] = "Guard";
			objStrings[1516 + j] = "Townsperson";
			objStrings[1520 + j] = "Knight";
			objStrings[1524 + j] = "Assassin";
			objStrings[1528 + j] = "Warlord";
			
			objStrings[1592 + j] = "Ninja";
			objStrings[1596 + j] = "Old Man";
			objStrings[1600 + j] = "Old Woman";
			objStrings[1604 + j] = "Paladin";
			objStrings[1608 + j] = "Wizard";
			
		}
	}

};
