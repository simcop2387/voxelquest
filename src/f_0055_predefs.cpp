




//UNIT -> PAGE -> GROUP
float timeValSec;
float timeVal;
float curFPS;
float totalFPS = 0.0f;
int frameCount = 0;
int giGUI_IDS;
Timer timer;
Timer globalTimer;
Vector3 gvLightDir;

const static int fpsNumFrames = 				100;
const static int giSEA_LEVEL =	 				260;
const static float gfSEA_LEVEL =				(float)giSEA_LEVEL;
const static int giNUM_THREADS = 				8;
const static int giLOAD_RAD = 					16;
const static int giRENDER_RAD = 				16;
const static int giUNITS_PER_PAGE = 			128;
const static int giPAGES_PER_GROUP[3] = 		{64,4,64};
const static int giMIP_LEVELS =					1;
const static int giMIP_SCALE =					2;
const static int giMIN_MIP = 					0;
const static int giRAY_STEPS =					16;
const static int giTEXTURE_SIZE =				32;
const static int giNUM_TEXTURES =				4;
const static int giMATERIAL_SLOTS =				256;
const static int giNUM_MATERIALS = 				20;
const static int giNUM_THREAD_PARAMS =			8;
const static int giUNITS_PER_GROUP[3] = 		{giPAGES_PER_GROUP[0]*giUNITS_PER_PAGE, 	giPAGES_PER_GROUP[1]*giUNITS_PER_PAGE, 	giPAGES_PER_GROUP[2]*giUNITS_PER_PAGE};
const static float gfCLIP_DIST = 				2000.0;
const static float gfTEXTURE_SIZE =				(float)giTEXTURE_SIZE;
const static float gfMIP_LEVELS = 				(float)giMIP_LEVELS;
const static float gfUNITS_PER_PAGE = 			(float)giUNITS_PER_PAGE;
const static float gfPAGES_PER_GROUP[3] = 		{(float)giPAGES_PER_GROUP[0],  	(float)giPAGES_PER_GROUP[1],		(float)giPAGES_PER_GROUP[2]};
const static float gfUNITS_PER_GROUP[3] = 		{(float)giUNITS_PER_GROUP[0], 		(float)giUNITS_PER_GROUP[1], 		(float)giUNITS_PER_GROUP[2]};
const static float gfRAY_STEPS =				(float)giRAY_STEPS;
const static float gfMATERIAL_SLOTS =			(float)giMATERIAL_SLOTS;
const static int static_imods[6] = {-1,1,0,0,0,0};
const static int static_jmods[6] = {0,0,-1,1,0,0};
const static int static_kmods[6] = {0,0,0,0,-1,1};
const static int static_ADJ_NEIGHBOR_COUNT = 27;
const static int static_octantsX[8] = 	{0,1,0,1,0,1,0,1};
const static int static_octantsY[8] = 	{0,0,1,1,0,0,1,1};
const static int static_octantsZ[8] = 	{0,0,0,0,1,1,1,1};
const static int static_nxmods[static_ADJ_NEIGHBOR_COUNT] = 	{
									-1, 0, 1,
									-1, 0, 1,
									-1, 0, 1,
									
									-1, 0, 1,
									-1, 0, 1,
									-1, 0, 1,

									-1, 0, 1,
									-1, 0, 1,
									-1, 0, 1
								};
const static int static_nymods[static_ADJ_NEIGHBOR_COUNT] = 	{
									-1, -1, -1,
									0, 0, 0,
									1, 1, 1, 

									-1, -1, -1,
									0, 0, 0,
									1, 1, 1, 

									-1, -1, -1,
									0, 0, 0,
									1, 1, 1
								};
const static int static_nzmods[static_ADJ_NEIGHBOR_COUNT] = 	{
									-1, -1, -1,
									-1, -1, -1,
									-1, -1, -1,

									0, 0, 0,
									0, 0, 0,
									0, 0, 0,

									1, 1, 1,
									1, 1, 1,
									1, 1, 1

								};
const static int static_faceOffsets[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

struct GameMaterial {
	float r[giMATERIAL_SLOTS];
	float g[giMATERIAL_SLOTS];
	float b[giMATERIAL_SLOTS];
};

GameMaterial mainMaterials[giNUM_MATERIALS];

bool ptBusy[giNUM_THREADS];
float windowSize[2] = {1024.0f, 1024.0f};
pthread_t threadStore[giNUM_THREADS];

OffsetGrid* oGrid = new OffsetGrid;
