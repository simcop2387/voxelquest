
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif


int RUN_COUNT;

bool ND_TRACE_OFF = false;


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

const static int MAX_LIMB_DATA_IN_BYTES = 65536;

const static bool POLYS_FOR_CELLS = false;
const static bool DO_VOXEL_WRAP = true;
//const static bool POLY_COLLISION = false;
const static bool VOXEL_COLLISION = true;

const static bool GEN_DEBRIS = false;
const static int  MAX_DEBRIS = 0;
const static bool GEN_COLLISION = false;
//const static bool GEN_POLYS_WORLD = false;

const static bool SINGLE_THREADED = false;
const static bool DO_RANDOMIZE = false;

// no greater than 8 unless shader changed (primIdList[8])
const static int MAX_PRIM_IDS = 16;
const static int MAX_PRIMTEST = 8;

const static int MAX_DEPTH_PEELS = 4;

double STEP_TIME_IN_SEC;

const static float OFFSET_X[4] = {-0.5,0.5,0.5,-0.5};
const static float OFFSET_Y[4] = {-0.5,-0.5,0.5,0.5};

const static int MAX_SPLASHES = 8;
const static int MAX_EXPLODES = 8;

//const static bool DO_CONNECT = true;
const static bool DO_SHADER_DUMP = false;
bool EDIT_POSE = false;

// warning: changing this changes the size of saved poses
// should be a one time change, then revereted to 1.0 after save
const static float ORG_SCALE_DELTA = 1.0f;

// base scale applied to base org generation
// only change this after changing ORG_SCALE_DELTA from 1.0
const static float ORG_SCALE_BASE = 0.5f;





// const static int DEF_WIN_W = 1440;
// const static int DEF_WIN_H = 720;

// qqqq

#define STREAM_RES 1

#ifdef STREAM_RES
	const static int DEF_WIN_W = 1920; //2048;//
	const static int DEF_WIN_H = 1080; //1024;//
#else
	const static int DEF_WIN_W = 1440;//1536;
	const static int DEF_WIN_H = 720;//768;
#endif


const static int DEF_SCALE_FACTOR = 1;
const static int RENDER_SCALE_FACTOR = 4;


int TOT_POINT_COUNT = 0;

const static bool DO_POINTS = true;
const static int MAX_THREADS = 7;
const static int NORM_RAD = 2;
const static int AO_RAD = 1;
const static int MAX_HOLDER_LOAD_COUNT = 512;
const static int VOXELS_PER_CELL = 16;
const static int CELLS_PER_HOLDER = 16;
const static int MAX_PDPOOL_SIZE = MAX_THREADS;
// const static int MAX_TBOPOOL_SIZE = 8;





const static float SPHEREMAP_SCALE_FACTOR = 0.5f; // lower is faster

const static int DEF_VOL_SIZE = 64;

const static bool USE_SPHERE_MAP = false;

const static float TIME_DELTA = 1.0f/60.0f;

const static int THREAD_DATA_COUNT = 16;

const static float MASS_PER_LIMB = 0.1f;

// #define DEBUG_BOUNDS 1

const static float explodeRad = 5.0f;

float RAND_COUNTER = 25.0f;
float RAND_COUNTER2 = 25.0f;


const static int GROUP_ID_NONEXISTENT = -3;
const static int GROUP_ID_UNMARKED = -2;
const static int GROUP_ID_UNMARKED_IDEAL = -1;

const static int FLUID_UNIT_MIN = -1;
const static int FLUID_UNIT_MAX = 16384;

const static int MAX_LAYERS = 2;
const static int MAX_MIP_LEV = 1; // min of 1




const static int MAX_KEYS = 256;

//const static int NUM_PRIM_LAYERS = 1;

const static int MAX_LIGHTS = 24;
const static int MAX_EVAL_LIGHTS = 128;
const static int FLOATS_PER_LIGHT = 12;



const static float UI_SCALE_FACTOR = 1.0f;

const static int MAX_PLANT_GEN = 16;

const static int MAX_TER_TEX = 9;

const static bool RT_TRANSFORM = false;



const static int MAX_NODE_DIRS = 6;
const static int MAX_NODE_VALS = 4;
const static int TOT_NODE_VALS =
	MAX_NODE_DIRS * MAX_NODE_VALS;

const static int TOT_MAP_DIRS = 4;

const static int MAX_BLOCK_STACK = 10;
const static int MAX_UI_LAYERS = 4;

float ZERO_FLOATS[16] = {
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f
};

// solid, water, air
const static float CUBE_POINTS[42] = {
		0,0,0,
		1,0,1,
		0,0,1,
		0,1,1,
		0,1,0,
		1,1,0,
		1,0,0,
		
		
		1,1,1,
		1,0,0,
		1,1,0,
		0,1,0,
		0,1,1,
		0,0,1,
		1,0,1
		
		
		
};





// solid, water, air
const static bool PROC_MATRIX[3][3] = {
		{false, true, true},
		{false, false, true},
		{false, false, false}
};

bool TEMP_DEBUG;

const static int MAX_OBJ_TYPES = 1673;
const static int ITEMS_PER_COL = 48;
const static int ITEMS_PER_ROW = 36;
const static int MAX_ICON_ID = ITEMS_PER_COL*ITEMS_PER_ROW;
const static int EMPTY_OBJECT_TYPE = 1672;
const static int EMPTY_OBJECT_ID = 1;

enum E_DIR_SPECS_SIGN {
	E_DIR_XP,
	E_DIR_XM,
	E_DIR_YP,
	E_DIR_YM,
	E_DIR_ZP,
	E_DIR_ZM,
	E_DIR_MP_LENGTH
};


const static int NUM_ORIENTATIONS = 6;
const static float DIR_VECS[NUM_ORIENTATIONS][3] = {
	{1.0f, 0.0f, 0.0f},
	{-1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, -1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, -1.0f}
};
const static int DIR_VECS_I[NUM_ORIENTATIONS][3] = {
	{1, 0, 0},
	{-1, 0, 0},
	{0, 1, 0},
	{0, -1, 0},
	{0, 0, 1},
	{0, 0, -1}
};


const static int NUM_PLANAR_ORIENTATIONS = 4;
const static int BASE_MOVEABLE_Z = 2;
const static int MAX_MOVEABLE_Z = BASE_MOVEABLE_Z*2 + 1;
const static int NUM_MOVEABLE_DIRS_ONE_AWAY = NUM_PLANAR_ORIENTATIONS*3;

const static int NUM_MOVEABLE_DIRS = MAX_MOVEABLE_Z*NUM_PLANAR_ORIENTATIONS;// 12 20 24
const static int DIR_VECS_MOVE[NUM_MOVEABLE_DIRS][3] = {
	
	{ 1,  0,  -2},
	{-1,  0,  -2},
	{ 0,  1,  -2},
	{ 0, -1,  -2},
	
	{ 1,  0, -1},
	{-1,  0, -1},
	{ 0,  1, -1},
	{ 0, -1, -1},
	
	
	{ 1,  0,  0},
	{-1,  0,  0},
	{ 0,  1,  0},
	{ 0, -1,  0},
	
	
	
	
	{ 1,  0,  1},
	{-1,  0,  1},
	{ 0,  1,  1},
	{ 0, -1,  1},
		
	{ 1,  0,  2},
	{-1,  0,  2},
	{ 0,  1,  2},
	{ 0, -1,  2}
	
	
	
	
	
	// ,
	// { 1,  0,  -3},
	// {-1,  0,  -3},
	// { 0,  1,  -3},
	// { 0, -1,  -3},
	
	// { 1,  0,  3},
	// {-1,  0,  3},
	// { 0,  1,  3},
	// { 0, -1,  3}
	
	
	
	// ,{ 1,  1,  1},
	// { 1, -1,  1},
	// {-1,  1,  1},
	// {-1, -1,  1},
	
	// { 1,  1,  0},
	// { 1, -1,  0},
	// {-1,  1,  0},
	// {-1, -1,  0},
	
	// { 1,  1, -1},
	// { 1, -1, -1},
	// {-1,  1, -1},
	// {-1, -1, -1}
	
	
};


float ALL_ROT[16*NUM_ORIENTATIONS*NUM_ORIENTATIONS*NUM_ORIENTATIONS];
const static int ROT_MAP[36] = {
	
	// x, inc theta
	//-------------
	E_DIR_XP,// x+ -> x+
	E_DIR_XM,// x- -> x-
	E_DIR_ZP,// y+ -> z+
	E_DIR_ZM,// y- -> z-
	E_DIR_YM,// z+ -> y-
	E_DIR_YP,// z- -> y+
	
	// x, dec theta
	//-------------
	E_DIR_XP,// x+ -> x+
	E_DIR_XM,// x- -> x-
	E_DIR_ZM,// y+ -> z-
	E_DIR_ZP,// y- -> z+
	E_DIR_YP,// z+ -> y+
	E_DIR_YM,// z- -> y-
	
	// y, inc theta
	//-------------
	E_DIR_ZP,// x+ -> z+
	E_DIR_ZM,// x- -> z-
	E_DIR_YP,// y+ -> y+
	E_DIR_YM,// y- -> y-
	E_DIR_XM,// z+ -> x-
	E_DIR_XP,// z- -> x+
	
	// y, dec theta
	//-------------
	E_DIR_ZM,// x+ -> z-
	E_DIR_ZP,// x- -> z+
	E_DIR_YP,// y+ -> y+
	E_DIR_YM,// y- -> y-
	E_DIR_XP,// z+ -> x+
	E_DIR_XM,// z- -> x-
	
	// z, inc theta
	//-------------
	E_DIR_YP,// x+ -> y+
	E_DIR_YM,// x- -> y-
	E_DIR_XM,// y+ -> x-
	E_DIR_XP,// y- -> x+
	E_DIR_ZP,// z+ -> z+
	E_DIR_ZM,// z- -> z-
	
	// z, dec theta
	//-------------
	E_DIR_YM,// x+ -> y-
	E_DIR_YP,// x- -> y+
	E_DIR_XP,// y+ -> x+
	E_DIR_XM,// y- -> x-
	E_DIR_ZP,// z+ -> z+
	E_DIR_ZM// z- -> z-
	
};

int totalPointCount;


bool LAST_COMPILE_ERROR = false;

char *BUF_NAMES[] =
{
	"ublock0",
	"ublock1",
	"ublock2",
	"ublock3",
	"ublock4",
	"ublock5",
	"ublock6",
	"ublock7",
	"ublock8",
	"ublock9",
	"ublock10",
	"ublock11",
	"ublock12",
	"ublock13"
};

const static int R_CHANNEL = 0;
const static int G_CHANNEL = 1;
const static int B_CHANNEL = 2;
const static int A_CHANNEL = 3;

const static int MAX_VG_FBOS = 2;
int CUR_VG_FBO = 0;

const static int MAX_VGT_FBOS = 2;
int CUR_VGT_FBO = 0;

const static float M_PI = 3.14159265359;

const static float TBDIR_ARR[] = {
	0.0f,
	M_PI*0.5f,
	M_PI,
	M_PI*1.5f
};


int PAGE_COUNT = 0;
// set to 0 to disable
int MAX_HOLDERS = 2048;



float MAX_GPU_MEM = 4096.0f;
float VERTEX_MEM_USAGE = 0.0f;
float TOT_GPU_MEM_USAGE = 0.0f;

float MAX_CPU_MEM = 4096.0f;
float TOT_CPU_MEM_USAGE = 0.0f;

long long ENT_COUNTER = 0;

bool TRACE_ON = false;
//#define DEBUG_MODE 1







#include <SDKDDKVer.h>


#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
#include <list>
#include <cstddef>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <ios>
#include <algorithm>
#include <cstdlib>
#include <thread>
#include <mutex>
//#include <atomic>

#include <iomanip>
#include <map>
//#include <ctime>


#include <stdlib.h>
#include <math.h>
#include <tchar.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>
#include <conio.h>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/freeglut.h>
#pragma comment(lib, "glew32.lib")




float fract(float val) {
	return (val - floor(val));
}





// note - check deprecated source folder
// make sure to put it back in source
// if using poco or web services

// #ifdef USE_POCO

// #include "Poco/Net/HTTPServer.h"
// #include "Poco/Net/HTTPRequestHandler.h"
// #include "Poco/Net/HTTPRequestHandlerFactory.h"
// #include "Poco/Net/HTTPServerParams.h"
// #include "Poco/Net/HTTPServerRequest.h"
// #include "Poco/Net/HTTPServerResponse.h"
// #include "Poco/Net/HTTPServerParams.h"
// #include "Poco/Net/ServerSocket.h"
// #include "Poco/Net/WebSocket.h"
// #include "Poco/Net/NetException.h"
// #include "Poco/Util/ServerApplication.h"
// #include "Poco/Util/Option.h"
// #include "Poco/Util/OptionSet.h"
// #include "Poco/Util/HelpFormatter.h"
// #include "Poco/Format.h"
// #include "Poco/Runnable.h"
// #include "Poco/ThreadPool.h"





// using Poco::Net::ServerSocket;
// using Poco::Net::WebSocket;
// using Poco::Net::WebSocketException;
// using Poco::SystemException;
// using Poco::Net::HTTPRequestHandler;
// using Poco::Net::HTTPRequestHandlerFactory;
// using Poco::Net::HTTPServer;
// using Poco::Net::HTTPServerRequest;
// using Poco::Net::HTTPResponse;
// using Poco::Net::HTTPServerResponse;
// using Poco::Net::HTTPServerParams;
// using Poco::Timestamp;
// using Poco::Runnable;
// using Poco::ThreadPool;
// using Poco::Util::ServerApplication;
// using Poco::Util::Application;
// using Poco::Util::Option;
// using Poco::Util::OptionSet;
// using Poco::Util::HelpFormatter;



// #endif

//#include "Poco/Base64Decoder.h"
//using Poco::Base64Decoder;






class HPClock
{
public:
	HPClock();

	HPClock(const HPClock& other);
	HPClock& operator=(const HPClock& other);

	~HPClock();

	/// Resets the initial reference time.
	void reset();

	/// Returns the time in ms since the last call to reset or since 
	/// the HPClock was created.
	unsigned long int getTimeMilliseconds();

	/// Returns the time in us since the last call to reset or since 
	/// the Clock was created.
	unsigned long int getTimeMicroseconds();
private:
	struct HPClockData* m_data;
};



template <class T>
const T& HPClockMin(const T& a, const T& b) 
{
  return a < b ? a : b ;
}


#ifdef __CELLOS_LV2__
#include <sys/sys_time.h>
#include <sys/time_util.h>
#include <stdio.h>
#endif

#if defined (SUNOS) || defined (__SUNOS__) 
#include <stdio.h> 
#endif

#if defined(WIN32) || defined(_WIN32)

#define B3_USE_WINDOWS_TIMERS
#define WIN32_LEAN_AND_MEAN
#define NOWINRES
#define NOMCX
#define NOIME 

#ifdef _XBOX
	#include <Xtl.h>
#else //_XBOX
	#include <windows.h>
#endif //_XBOX

#include <time.h>


#else //_WIN32
#include <sys/time.h>
#endif //_WIN32



struct HPClockData
{

#ifdef B3_USE_WINDOWS_TIMERS
	LARGE_INTEGER mClockFrequency;
	DWORD mStartTick;
	LONGLONG mPrevElapsedTime;
	LARGE_INTEGER mStartTime;
#else
#ifdef __CELLOS_LV2__
	uint64_t	mStartTime;
#else
	struct timeval mStartTime;
#endif
#endif //__CELLOS_LV2__

};

///The HPClock is a portable basic clock that measures accurate time in seconds, use for profiling.
HPClock::HPClock()
{
	m_data = new HPClockData;
#ifdef B3_USE_WINDOWS_TIMERS
	QueryPerformanceFrequency(&m_data->mClockFrequency);
#endif
	reset();
}

HPClock::~HPClock()
{
	delete m_data;
}

HPClock::HPClock(const HPClock& other)
{
	m_data = new HPClockData;
	*m_data = *other.m_data;
}

HPClock& HPClock::operator=(const HPClock& other)
{
	*m_data = *other.m_data;
	return *this;
}


	/// Resets the initial reference time.
void HPClock::reset()
{
#ifdef B3_USE_WINDOWS_TIMERS
	QueryPerformanceCounter(&m_data->mStartTime);
	m_data->mStartTick = GetTickCount();
	m_data->mPrevElapsedTime = 0;
#else
#ifdef __CELLOS_LV2__

	typedef uint64_t  ClockSize;
	ClockSize newTime;
	//__asm __volatile__( "mftb %0" : "=r" (newTime) : : "memory");
	SYS_TIMEBASE_GET( newTime );
	m_data->mStartTime = newTime;
#else
	gettimeofday(&m_data->mStartTime, 0);
#endif
#endif
}

/// Returns the time in ms since the last call to reset or since 
/// the HPClock was created.
unsigned long int HPClock::getTimeMilliseconds()
{
#ifdef B3_USE_WINDOWS_TIMERS
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	LONGLONG elapsedTime = currentTime.QuadPart - 
		m_data->mStartTime.QuadPart;
		// Compute the number of millisecond ticks elapsed.
	unsigned long msecTicks = (unsigned long)(1000 * elapsedTime / 
		m_data->mClockFrequency.QuadPart);
		// Check for unexpected leaps in the Win32 performance counter.  
	// (This is caused by unexpected data across the PCI to ISA 
		// bridge, aka south bridge.  See Microsoft KB274323.)
		unsigned long elapsedTicks = GetTickCount() - m_data->mStartTick;
		signed long msecOff = (signed long)(msecTicks - elapsedTicks);
		if (msecOff < -100 || msecOff > 100)
		{
			// Adjust the starting time forwards.
			LONGLONG msecAdjustment = HPClockMin(msecOff * 
				m_data->mClockFrequency.QuadPart / 1000, elapsedTime - 
				m_data->mPrevElapsedTime);
			m_data->mStartTime.QuadPart += msecAdjustment;
			elapsedTime -= msecAdjustment;

			// Recompute the number of millisecond ticks elapsed.
			msecTicks = (unsigned long)(1000 * elapsedTime / 
				m_data->mClockFrequency.QuadPart);
		}

		// Store the current elapsed time for adjustments next time.
		m_data->mPrevElapsedTime = elapsedTime;

		return msecTicks;
#else

#ifdef __CELLOS_LV2__
		uint64_t freq=sys_time_get_timebase_frequency();
		double dFreq=((double) freq) / 1000.0;
		typedef uint64_t  ClockSize;
		ClockSize newTime;
		SYS_TIMEBASE_GET( newTime );
		//__asm __volatile__( "mftb %0" : "=r" (newTime) : : "memory");

		return (unsigned long int)((double(newTime-m_data->mStartTime)) / dFreq);
#else

		struct timeval currentTime;
		gettimeofday(&currentTime, 0);
		return (currentTime.tv_sec - m_data->mStartTime.tv_sec) * 1000 + 
			(currentTime.tv_usec - m_data->mStartTime.tv_usec) / 1000;
#endif //__CELLOS_LV2__
#endif
}

	/// Returns the time in us since the last call to reset or since 
	/// the Clock was created.
unsigned long int HPClock::getTimeMicroseconds()
{
#ifdef B3_USE_WINDOWS_TIMERS
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		LONGLONG elapsedTime = currentTime.QuadPart - 
			m_data->mStartTime.QuadPart;

		// Compute the number of millisecond ticks elapsed.
		unsigned long msecTicks = (unsigned long)(1000 * elapsedTime / 
			m_data->mClockFrequency.QuadPart);

		// Check for unexpected leaps in the Win32 performance counter.  
		// (This is caused by unexpected data across the PCI to ISA 
		// bridge, aka south bridge.  See Microsoft KB274323.)
		unsigned long elapsedTicks = GetTickCount() - m_data->mStartTick;
		signed long msecOff = (signed long)(msecTicks - elapsedTicks);
		if (msecOff < -100 || msecOff > 100)
		{
			// Adjust the starting time forwards.
			LONGLONG msecAdjustment = HPClockMin(msecOff * 
				m_data->mClockFrequency.QuadPart / 1000, elapsedTime - 
				m_data->mPrevElapsedTime);
			m_data->mStartTime.QuadPart += msecAdjustment;
			elapsedTime -= msecAdjustment;
		}

		// Store the current elapsed time for adjustments next time.
		m_data->mPrevElapsedTime = elapsedTime;

		// Convert to microseconds.
		unsigned long usecTicks = (unsigned long)(1000000 * elapsedTime / 
			m_data->mClockFrequency.QuadPart);

		return usecTicks;
#else

#ifdef __CELLOS_LV2__
		uint64_t freq=sys_time_get_timebase_frequency();
		double dFreq=((double) freq)/ 1000000.0;
		typedef uint64_t  ClockSize;
		ClockSize newTime;
		//__asm __volatile__( "mftb %0" : "=r" (newTime) : : "memory");
		SYS_TIMEBASE_GET( newTime );

		return (unsigned long int)((double(newTime-m_data->mStartTime)) / dFreq);
#else

		struct timeval currentTime;
		gettimeofday(&currentTime, 0);
		return (currentTime.tv_sec - m_data->mStartTime.tv_sec) * 1000000 + 
			(currentTime.tv_usec - m_data->mStartTime.tv_usec);
#endif//__CELLOS_LV2__
#endif 
}







struct charArr {
	long size;
	char *data;
};

#ifdef WIN32
//#include <windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#else
// #include <sys/time.h>
#endif


// #if defined(_DEBUG)
// #include <crtdbg.h>
// #endif





// #include "q3.h"

// #include <chrono>

// typedef std::chrono::high_resolution_clock hr_clock;
// typedef std::chrono::nanoseconds clock_freq;
// const float kClockDivisor = 1.0f / (float)std::chrono::duration_cast<clock_freq>( std::chrono::seconds( 1 ) ).count( );

// class Clock {
// public:
// 	Clock( ) {
// 		m_start = hr_clock::now( );
// 		m_stop = hr_clock::now( );
// 	}

// 	float Start( ) {
// 		m_start = hr_clock::now( );
// 		return std::chrono::duration_cast<clock_freq>( m_start - m_stop ).count( ) * kClockDivisor;
// 	}
// 	void Stop( ) {
// 		m_stop = hr_clock::now( );
// 	}

// private:
//   hr_clock::time_point m_start;
//   hr_clock::time_point m_stop;
// };




using namespace std;


#define _USE_MATH_DEFINES

typedef unsigned int uint;


static inline int newcasecmp(const char *s1, const char *s2, size_t n)
{
	int lc1  = 0;
	int lc2  = 0;

	while (n--)
	{
		lc1 = towlower (*s1);
		lc2 = towlower (*s2);

		if (lc1 != lc2)
			return (lc1 - lc2);

		if (!lc1)
			return 0;

		++s1;
		++s2;
	}

	return 0;
}

// todo: this define of wcsncasecmp should not be here

//Win32 incompatibilities
#if defined(WIN32) && !defined(__GNUC__)
static inline bool isnan(double x) {
	return x != x;
}
static inline bool isinf(double x) {
	return !isnan(x) && isnan(x - x);
}
#endif

// Linux compile fix - from quaker66
#ifdef __GNUC__
// #include <cstring>
// #include <cstdlib>
#endif

// Mac compile fixes - from quaker66, Lion fix by dabrahams
#if defined(__APPLE__) && __DARWIN_C_LEVEL < 200809L || (defined(WIN32) && defined(__GNUC__))
#include <wctype.h>
#include <wchar.h>

#endif

// Simple function to check a string 's' has at least 'n' characters
static inline bool simplejson_wcsnlen(const char *s, size_t n) {
	if (s == 0)
		return false;

	const char *save = s;
	while (n-- > 0)
	{
		if (*(save++) == 0) return false;
	}

	return true;
}

struct membuf : std::streambuf
{
	membuf(char *begin, char *end) {
		this->setg(begin, begin, end);
	}
};



bool PROG_ACTIVE = true;

string SPACE_BUFFER[] = {" ", "  ", "   ", "    ", "     ", "      ", "       ", "        ", "         ", "          ", "           ", "            ", "             ", "              ", "               ", "                ", "                 ", "                  ", "                   ", "                    ", "                     ", "                      ", "                       ", "                        ", "                         ", "                          ", "                           ", "                            ", "                             ", "                              ", "                               ", "                                ", "                                 ", "                                  ", "                                   ", "                                    ", "                                     ", "                                      ", "                                       ", "                                        ", "                                         ", "                                          ", "                                           ", "                                            ", "                                             ", "                                              ", "                                               ", "                                                ", "                                                 ", "                                                  ", "                                                   ", "                                                    ", "                                                     ", "                                                      ", "                                                       ", "                                                        ", "                                                         ", "                                                          ", "                                                           ", "                                                            ", "                                                             ", "                                                              ", "                                                               ", "                                                                ", "                                                                 ", "                                                                  ", "                                                                   ", "                                                                    ", "                                                                     ", "                                                                      ", "                                                                       ", "                                                                        ", "                                                                         ", "                                                                          ", "                                                                           ", "                                                                            ", "                                                                             ", "                                                                              ", "                                                                               ", "                                                                                ", "                                                                                 ", "                                                                                  ", "                                                                                   ", "                                                                                    ", "                                                                                     ", "                                                                                      ", "                                                                                       ", "                                                                                        ", "                                                                                         ", "                                                                                          ", "                                                                                           ", "                                                                                            ", "                                                                                             ", "                                                                                              ", "                                                                                               ", "                                                                                                ", "                                                                                                 ", "                                                                                                  ", "                                                                                                   ", "                                                                                                    "};

#define glError() { \
		GLenum err = glGetError(); \
		while (err != GL_NO_ERROR) { \
			printf("glError: %s caught at %s:%u", \
						 (char*)gluErrorString(err), __FILE__, __LINE__); \
			err = glGetError(); \
			exit(-1); \
		} \
	}























// ########
	



// struct variable to store OpenGL info
struct glInfo
{
    std::string vendor;
    std::string renderer;
    std::string version;
    std::string glslVersion;
    std::vector <std::string> extensions;
    int redBits;
    int greenBits;
    int blueBits;
    int alphaBits;
    int depthBits;
    int stencilBits;
    int maxTextureSize;
    int maxLights;
    int maxAttribStacks;
    int maxModelViewStacks;
    int maxProjectionStacks;
    int maxClipPlanes;
    int maxTextureStacks;

    // ctor, init all members
    glInfo() : redBits(0), greenBits(0), blueBits(0), alphaBits(0), depthBits(0),
               stencilBits(0), maxTextureSize(0), maxLights(0), maxAttribStacks(0),
               maxModelViewStacks(0), maxClipPlanes(0), maxTextureStacks(0) {}

    void getInfo(unsigned int param=0);         // extract info
    void printSelf();                           // print itself
    bool isExtensionSupported(const std::string& ext); // check if a extension is supported
};



// WGL specific extensions for v3.0+ //////////////////////////////////////////
#ifdef _WIN32
//#include <windows.h>
#ifndef WGLGETEXTENSIONSSTRINGARB_DEF
#define WGLGETEXTENSIONSSTRINGARB_DEF
typedef const char* (WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC hdc);
PFNWGLGETEXTENSIONSSTRINGARBPROC    pwglGetExtensionsStringARB = 0;
#define wglGetExtensionsStringARB  pwglGetExtensionsStringARB
#endif
#endif

// version 2.0 or greater
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C



///////////////////////////////////////////////////////////////////////////////
// extract openGL info
// This function must be called after GL rendering context opened.
///////////////////////////////////////////////////////////////////////////////
void glInfo::getInfo(unsigned int param)
{
    std::string str;

    // get vendor string
    str = (const char*)glGetString(GL_VENDOR);
    this->vendor = str;             // check NULL return value

    // get renderer string
    str = (const char*)glGetString(GL_RENDERER);
    this->renderer = str;           // check NULL return value

    // get version string
    str = (const char*)glGetString(GL_VERSION);
    this->version = str;            // check NULL return value

    // get all extensions as a string
    str = (const char*)glGetString(GL_EXTENSIONS);

    // split extensions
    if(str.size() > 0)
    {
        char* str2 = new char[str.size() + 1];
        strcpy(str2, str.c_str());
        char* tok = strtok(str2, " ");
        while(tok)
        {
            this->extensions.push_back(tok);    // put a extension into struct
            tok = strtok(0, " ");               // next token
        }
        delete [] str2;
    }

    // get GLSL version string (v2.0+)
    str = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    if(str.size() > 0)
        this->glslVersion = str;
    else
    {
        // "GL_SHADING_LANGUAGE_VERSION" token is added later (v2.0) after the
        // first GLSL included in OpenGL (v1.5). If "GL_SHADING_LANGUAGE_VERSION"
        // is invalid token but "GL_ARB_shading_language_100" is supported, then
        // the GLSL version should be 1.0.rev.51
        if(isExtensionSupported("GL_ARB_shading_language_100"))
            glslVersion = "1.0.51"; // the first GLSL version
        else
            glslVersion = "";
    }

    // get WGL specific extensions for v3.0+
#ifdef _WIN32 //===========================================
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
    if(wglGetExtensionsStringARB && param)
    {
        str = (const char*)wglGetExtensionsStringARB((HDC)param);
        if(str.size() > 0)
        {
            char* str2 = new char[str.size() + 1];
            strcpy(str2, str.c_str());
            char* tok = strtok(str2, " ");
            while(tok)
            {
                this->extensions.push_back(tok);    // put a extension into struct
                tok = strtok(0, " ");               // next token
            }
            delete [] str2;
        }
    }
#endif //==================================================

    // sort extension by alphabetical order
    std::sort(this->extensions.begin(), this->extensions.end());

    // get number of color bits
    glGetIntegerv(GL_RED_BITS, &this->redBits);
    glGetIntegerv(GL_GREEN_BITS, &this->greenBits);
    glGetIntegerv(GL_BLUE_BITS, &this->blueBits);
    glGetIntegerv(GL_ALPHA_BITS, &this->alphaBits);

    // get depth bits
    glGetIntegerv(GL_DEPTH_BITS, &this->depthBits);

    // get stecil bits
    glGetIntegerv(GL_STENCIL_BITS, &this->stencilBits);

    // get max number of lights allowed
    glGetIntegerv(GL_MAX_LIGHTS, &this->maxLights);

    // get max texture resolution
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &this->maxTextureSize);

    // get max number of clipping planes
    glGetIntegerv(GL_MAX_CLIP_PLANES, &this->maxClipPlanes);

    // get max modelview and projection matrix stacks
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &this->maxModelViewStacks);
    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &this->maxProjectionStacks);
    glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &this->maxAttribStacks);

    // get max texture stacks
    glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &this->maxTextureStacks);
}



///////////////////////////////////////////////////////////////////////////////
// check if the video card support a certain extension
///////////////////////////////////////////////////////////////////////////////
bool glInfo::isExtensionSupported(const std::string& ext)
{
    // search corresponding extension
    std::vector<std::string>::const_iterator iter = this->extensions.begin();
    std::vector<std::string>::const_iterator endIter = this->extensions.end();

    while(iter != endIter)
    {
        if(ext == *iter)
            return true;
        else
            ++iter;
    }
    return false;
}



///////////////////////////////////////////////////////////////////////////////
// print OpenGL info to screen and save to a file
///////////////////////////////////////////////////////////////////////////////
void glInfo::printSelf()
{
    std::stringstream ss;

    ss << std::endl; // blank line
    ss << "OpenGL Driver Info" << std::endl;
    ss << "==================" << std::endl;
    ss << "Vendor: " << this->vendor << std::endl;
    ss << "Version: " << this->version << std::endl;
    ss << "GLSL Version: " << this->glslVersion << std::endl;
    ss << "Renderer: " << this->renderer << std::endl;

    ss << std::endl;
    ss << "Color Bits(R,G,B,A): (" << this->redBits << ", " << this->greenBits
       << ", " << this->blueBits << ", " << this->alphaBits << ")\n";
    ss << "Depth Bits: " << this->depthBits << std::endl;
    ss << "Stencil Bits: " << this->stencilBits << std::endl;

    ss << std::endl;
    ss << "Max Texture Size: " << this->maxTextureSize << "x" << this->maxTextureSize << std::endl;
    ss << "Max Lights: " << this->maxLights << std::endl;
    ss << "Max Clip Planes: " << this->maxClipPlanes << std::endl;
    ss << "Max Modelview Matrix Stacks: " << this->maxModelViewStacks << std::endl;
    ss << "Max Projection Matrix Stacks: " << this->maxProjectionStacks << std::endl;
    ss << "Max Attribute Stacks: " << this->maxAttribStacks << std::endl;
    ss << "Max Texture Stacks: " << this->maxTextureStacks << std::endl;

    ss << std::endl;
    ss << "Total Number of Extensions: " << this->extensions.size() << std::endl;
    ss << "==============================" << std::endl;
    for(unsigned int i = 0; i < this->extensions.size(); ++i)
        ss << this->extensions.at(i) << std::endl;

    ss << "======================================================================" << std::endl;

    std::cout << ss.str() << std::endl;
}




// function pointers for PBO Extension
// Windows needs to get function pointers from ICD OpenGL drivers,
// because opengl32.dll does not support extensions higher than v1.1.
#ifdef _WIN32
PFNGLGENBUFFERSARBPROC pglGenBuffersARB = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC pglBindBufferARB = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC pglBufferDataARB = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC pglMapBufferARB = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB = 0;                   // unmap VBO procedure
// #define glGenBuffersARB           pglGenBuffersARB
// #define glBindBufferARB           pglBindBufferARB
// #define glBufferDataARB           pglBufferDataARB
// #define glBufferSubDataARB        pglBufferSubDataARB
// #define glDeleteBuffersARB        pglDeleteBuffersARB
// #define glGetBufferParameterivARB pglGetBufferParameterivARB
// #define glMapBufferARB            pglMapBufferARB
// #define glUnmapBufferARB          pglUnmapBufferARB
#endif


// function pointers for WGL_EXT_swap_control
#ifdef _WIN32
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef int (WINAPI * PFNWGLGETSWAPINTERVALEXTPROC) (void);
PFNWGLSWAPINTERVALEXTPROC pwglSwapIntervalEXT = 0;
PFNWGLGETSWAPINTERVALEXTPROC pwglGetSwapIntervalEXT = 0;
#define wglSwapIntervalEXT      pwglSwapIntervalEXT
#define wglGetSwapIntervalEXT   pwglGetSwapIntervalEXT
#endif




// typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
// extern PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;

// static inline void init_EXT_Vsync()
// {
// wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)extgl_GetProcAddress( "wglSwapIntervalEXT" );
// }





const static GLenum bufNames[] = {
	GL_COLOR_ATTACHMENT0_EXT,
	GL_COLOR_ATTACHMENT1_EXT,
	GL_COLOR_ATTACHMENT2_EXT,
	GL_COLOR_ATTACHMENT3_EXT,
	GL_COLOR_ATTACHMENT4_EXT,
	GL_COLOR_ATTACHMENT5_EXT,
	GL_COLOR_ATTACHMENT6_EXT,
	GL_COLOR_ATTACHMENT7_EXT
};






// data for a fullscreen quad (this time with texture coords)
GLfloat vertexDataQuad[] = {
//  X     Y     Z           U     V     
   1.0f, 1.0f, 0.0f, 1.0f,       1.0f, 1.0f, 0.0f, 0.0f, // vertex 0
  -1.0f, 1.0f, 0.0f, 1.0f,       0.0f, 1.0f, 0.0f, 0.0f, // vertex 1
   1.0f,-1.0f, 0.0f, 1.0f,       1.0f, 0.0f, 0.0f, 0.0f, // vertex 2
  -1.0f,-1.0f, 0.0f, 1.0f,       0.0f, 0.0f, 0.0f, 0.0f  // vertex 3
}; // 4 vertices with 8 components (floats) each
GLuint indexDataQuad[] = {
    0,1,2, // first triangle
    2,1,3  // second triangle
};








// ??????????????????????????????





#define NUMRAYS 500
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Z 5

#define COLLISION_RADIUS 0.0f


#define BT_LINE_BATCH_SIZE 512
#if defined(BT_USE_DOUBLE_PRECISION)
#define btglLoadMatrix glLoadMatrixd
#define btglMultMatrix glMultMatrixd
#define btglColor3 glColor3d
#define btglVertex3 glVertex3d
#else
#define btglLoadMatrix glLoadMatrixf
#define btglMultMatrix glMultMatrixf
#define btglColor3 glColor3f
#define btglVertex3 glVertex3d
#endif

// #ifndef M_PI
// #define M_PI       btScalar(3.14159265358979323846)
// #endif

// #ifndef M_PI_2
// #define M_PI_2     btScalar(1.57079632679489661923)
// #endif

// #ifndef M_PI_4
// #define M_PI_4     btScalar(0.785398163397448309616)
// #endif

#ifndef M_PI_8
#define M_PI_8     0.5 * M_PI_4
#endif

//#define NUM_LEGS_GA 6
//#define BODYPART_COUNT_GA 2 * NUM_LEGS_GA + 1
//#define JOINT_COUNT_GA BODYPART_COUNT_GA - 1



class btConvexShape;
class btCollisionShape;
class btShapeHull;
class btDiscreteDynamicsWorld;
class MyShapeDrawer;
class btDynamicsWorld;
class btRigidBody;
class btBroadphaseInterface;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;





#include "../CommonInterfaces/CommonGraphicsAppInterface.h"
#include "../CommonInterfaces/CommonRenderInterface.h"
#include "../CommonInterfaces/CommonExampleInterface.h"
#include "../CommonInterfaces/CommonGUIHelperInterface.h"
#include "../CommonInterfaces/CommonRigidBodyBase.h"


#include "btBulletDynamicsCommon.h"

// #include "Bullet3Common/b3Vector3.h"
// #include "Bullet3Common/b3Scalar.h"

// #include "LinearMath/btVector3.h"
// #include "LinearMath/btAlignedObjectArray.h"
// #include "LinearMath/btTransformUtil.h"



#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/CollisionShapes/btPolyhedralConvexShape.h"
#include "BulletCollision/CollisionShapes/btConvexPolyhedron.h"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.h"

// included within btBulletDynamicsCommon.h
// #include "LinearMath/btIDebugDraw.h"
// #include "LinearMath/btDefaultMotionState.h"
// #include "LinearMath/btTransform.h"
// #include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
// #include "BulletCollision/CollisionShapes/btBoxShape.h"
// #include "BulletCollision/CollisionShapes/btSphereShape.h"
// #include "BulletCollision/CollisionShapes/btConeShape.h"
// #include "BulletCollision/CollisionShapes/btCylinderShape.h"
// #include "BulletCollision/CollisionShapes/btTetrahedronShape.h"
// #include "BulletCollision/CollisionShapes/btCompoundShape.h"
// #include "BulletCollision/CollisionShapes/btCapsuleShape.h"
// #include "BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
// #include "BulletCollision/CollisionShapes/btUniformScalingShape.h"
// #include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"
// #include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
// #include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
// #include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
// #include "BulletDynamics/Dynamics/btRigidBody.h"

#include "../OpenGLWindow/OpenGL2Include.h"
#include "../OpenGLWindow/SimpleCamera.h"
#include "../OpenGLWindow/GLInstanceGraphicsShape.h"

#include "../Benchmarks/TaruData.h"
//#include "../Benchmarks/landscapeData.h"






/////







// ??????????????????????????????






/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/









struct ShapeCache
{
	struct Edge { btVector3 n[2];int v[2]; };
	ShapeCache(btConvexShape* s) : m_shapehull(s) {}
	btShapeHull					m_shapehull;
	btAlignedObjectArray<Edge>	m_edges;
};

void OGL_displaylist_register_shape(btCollisionShape * shape);
void OGL_displaylist_clean();



///










class GlDrawcallback : public btTriangleCallback
{

public:

	bool	m_wireframe;

	GlDrawcallback()
		:m_wireframe(false)
	{
	}

	virtual void processTriangle(btVector3* triangle, int partId, int triangleIndex)
	{

		// (void)triangleIndex;
		// (void)partId;


		if (m_wireframe)
		{
			// glBegin(GL_LINES);
			// //glColor3f(1, 0, 0);
			// glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			// glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			// //glColor3f(0, 1, 0);
			// glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			// glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			// // /glColor3f(0, 0, 1);
			// glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			// glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			// glEnd();
		} else
		{
			//glBegin(GL_TRIANGLES);
			//glColor3f(1, 1, 1);
			
			//cout << "f\n";
			
			
			btVector3 normal = (triangle[2]-triangle[0]).cross(triangle[1]-triangle[0]);
			normal.normalize();
			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			
			glVertex3f(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			glVertex3f(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			glVertex3f(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			
			// cout << triangle[0].getX() << " " << triangle[0].getY() << " " << triangle[0].getZ() << "\n";
			// cout << triangle[1].getX() << " " << triangle[1].getY() << " " << triangle[1].getZ() << "\n";
			// cout << triangle[2].getX() << " " << triangle[2].getY() << " " << triangle[2].getZ() << "\n";
			// cout << normal.getX() << " " << normal.getY() << " " << normal.getZ() << "\n";
			// cout << "\n";
			
			// normal = (triangle[0]-triangle[2]).cross(triangle[1]-triangle[2]);
			// normal.normalize();
			// glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			
			// glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			// glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			// glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			//glEnd();
		}
	}
};

class TriangleGlDrawcallback : public btInternalTriangleIndexCallback
{
public:
	virtual void internalProcessTriangleIndex(btVector3* triangle,int partId,int  triangleIndex)
	{
		(void)triangleIndex;
		(void)partId;



		//glBegin(GL_TRIANGLES);//LINES);
		//glColor3f(1, 0, 0);
		glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
		glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
		//glColor3f(0, 1, 0);
		glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
		glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
		// /glColor3f(0, 0, 1);
		glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
		glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
		//glEnd();
	}
};


// ???????????








struct OGLRenderer : public CommonRenderInterface
{
    int m_width;
    int m_height;
    SimpleCamera	m_camera;
    
    OGLRenderer(int width, int height);
    
    virtual void init();
    
    virtual void updateCamera(int upAxis);
    
	virtual const CommonCameraInterface* getActiveCamera() const;
	virtual CommonCameraInterface* getActiveCamera();
	virtual void setActiveCamera(CommonCameraInterface* cam);

	virtual void resize(int width, int height)
	{
		m_width = width;
		m_height = height;
	}

    virtual void removeAllInstances();
    
    
    virtual void writeSingleInstanceColorToCPU(float* color, int srcIndex);
    virtual void writeSingleInstanceColorToCPU(double* color, int srcIndex);
    virtual void getCameraViewMatrix(float viewMat[16]) const;
    virtual void getCameraProjectionMatrix(float projMat[16]) const;

    
    virtual void renderScene();
    
    virtual int getScreenWidth()
    {
        return m_width;
    }
    virtual int getScreenHeight()
    {
        return m_height;
    }
	virtual int	registerTexture(const unsigned char* texels, int width, int height)
	{
		return -1;
	}
    virtual int registerGraphicsInstance(int shapeIndex, const double* position, const double* quaternion, const double* color, const double* scaling);
    
    virtual int registerGraphicsInstance(int shapeIndex, const float* position, const float* quaternion, const float* color, const float* scaling);
    
    virtual void drawLines(const float* positions, const float color[4], int numPoints, int pointStrideInBytes, const unsigned int* indices, int numIndices, float pointDrawSize);
    
    virtual void drawLine(const float from[4], const float to[4], const float color[4], float lineWidth);
    
    virtual int registerShape(const float* vertices, int numvertices, const int* indices, int numIndices,int primitiveType=B3_GL_TRIANGLES, int textureIndex=-1);
    
    virtual void writeSingleInstanceTransformToCPU(const float* position, const float* orientation, int srcIndex);
    
    virtual void writeSingleInstanceTransformToCPU(const double* position, const double* orientation, int srcIndex);
    
    virtual void writeTransforms();
    
    virtual void drawLine(const double from[4], const double to[4], const double color[4], double lineWidth);
    
    virtual void drawPoint(const float* position, const float color[4], float pointDrawSize);
    
    virtual void drawPoint(const double* position, const double color[4], double pointDrawSize);
    
    virtual void updateShape(int shapeIndex, const float* vertices);
    
    virtual void enableBlend(bool blend);

	virtual struct	GLInstanceRendererInternalData* getInternalData()
	{
		return 0;
	}
    
};







OGLRenderer::OGLRenderer(int width, int height)
 :m_width(width),
 m_height(height)
{
    
}

void OGLRenderer::init()
{
}

const CommonCameraInterface* OGLRenderer::getActiveCamera() const
{
	return &m_camera;
}
CommonCameraInterface* OGLRenderer::getActiveCamera()
{
	return &m_camera;
}
void OGLRenderer::setActiveCamera(CommonCameraInterface* cam)
{
	b3Assert(0);//not supported yet
}

void OGLRenderer::updateCamera(int upAxis)
{
    float projection[16];
    float view[16];
    m_camera.setAspectRatio((float)m_width/(float)m_height);
    m_camera.update();
    m_camera.getCameraProjectionMatrix(projection);
    m_camera.getCameraViewMatrix(view);
    GLfloat projMat[16];
    GLfloat viewMat[16];
    for (int i=0;i<16;i++)
    {
        viewMat[i] = view[i];
        projMat[i] = projection[i];
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(projMat);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(viewMat);
}

void OGLRenderer::removeAllInstances()
{
}


void OGLRenderer::writeSingleInstanceColorToCPU(float* color, int srcIndex)
{
}
void OGLRenderer::writeSingleInstanceColorToCPU(double* color, int srcIndex)
{
    
}
void OGLRenderer::getCameraViewMatrix(float viewMat[16]) const
{
    b3Assert(0);
}
void OGLRenderer::getCameraProjectionMatrix(float projMat[16]) const
{
    b3Assert(0);
    
}


void OGLRenderer::renderScene()
{
    //cout << "asdfasdf\n";
}
    
    


int OGLRenderer::registerGraphicsInstance(int shapeIndex, const double* position, const double* quaternion, const double* color, const double* scaling)
{
    return 0;
}

int OGLRenderer::registerGraphicsInstance(int shapeIndex, const float* position, const float* quaternion, const float* color, const float* scaling)
{
    return 0;
}

void OGLRenderer::drawLines(const float* positions, const float color[4], int numPoints, int pointStrideInBytes, const unsigned int* indices, int numIndices, float pointDrawSize)
{
    int pointStrideInFloats = pointStrideInBytes/4;
    for (int i=0;i<numIndices;i+=2)
    {
        int index0 = indices[i];
        int index1 = indices[i+1];
        
        b3Vector3 fromColor = b3MakeVector3(color[0],color[1],color[2]);
        b3Vector3 toColor = b3MakeVector3(color[0],color[1],color[2]);
        
        b3Vector3 from= b3MakeVector3(positions[index0*pointStrideInFloats],positions[index0*pointStrideInFloats+1],positions[index0*pointStrideInFloats+2]);
        b3Vector3 to= b3MakeVector3(positions[index1*pointStrideInFloats],positions[index1*pointStrideInFloats+1],positions[index1*pointStrideInFloats+2]);
        
        // glBegin(GL_LINES);
        // glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
        // glVertex3d(from.getX(), from.getY(), from.getZ());
        // glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
        // glVertex3d(to.getX(), to.getY(), to.getZ());
        // glEnd();
        
    }
}

void OGLRenderer::drawLine(const float from[4], const float to[4], const float color[4], float lineWidth)
{
        // glBegin(GL_LINES);
        // glColor3f(color[0],color[1],color[2]);
        // glVertex3d(from[0],from[1],from[2]);
        // glVertex3d(to[0],to[1],to[2]);
        // glEnd();
}
int OGLRenderer::registerShape(const float* vertices, int numvertices, const int* indices, int numIndices,int primitiveType, int textureIndex)
{
    return 0;
}

void OGLRenderer::writeSingleInstanceTransformToCPU(const float* position, const float* orientation, int srcIndex)
{
}
void OGLRenderer::writeSingleInstanceTransformToCPU(const double* position, const double* orientation, int srcIndex)
{
}
void OGLRenderer::writeTransforms()
{
}


void OGLRenderer::drawLine(const double from[4], const double to[4], const double color[4], double lineWidth)
{
    
}
void OGLRenderer::drawPoint(const float* position, const float color[4], float pointDrawSize)
{
}
void OGLRenderer::drawPoint(const double* position, const double color[4], double pointDrawSize)
{
}

void OGLRenderer::updateShape(int shapeIndex, const float* vertices)
{
}

void OGLRenderer::enableBlend(bool blend)
{
}




// ??????????????

struct MyOGLAppInternalData
{
	GLuint m_fontTextureId;
	GLuint m_largeFontTextureId;
	
};

struct MyOGLApp : public CommonGraphicsApp
{
protected:
	struct MyOGLAppInternalData*	m_data;

public:
	MyOGLApp(const char* title, int width, int height) {
		m_renderer = new OGLRenderer(width,height);		
	}
	virtual ~MyOGLApp() {}

	virtual void drawGrid(DrawGridData data=DrawGridData()) {}
	virtual void setUpAxis(int axis) {}
	virtual int getUpAxis() const {return 2;}
	
	virtual void swapBuffer() {}
	virtual void drawText( const char* txt, int posX, int posY) {}
	virtual void setBackgroundColor(float red, float green, float blue) {}
	virtual int	registerCubeShape(float halfExtentsX,float halfExtentsY, float halfExtentsZ, int textureIndex = -1,  float textureScaling = 1)
	{
		cout << "a\n";
		return 0;
	}
	virtual int	registerGraphicsUnitSphereShape(EnumSphereLevelOfDetail lod, int textureId=-1)
	{
		cout << "b\n";
		return 0;
	}
    virtual void drawText3D( const char* txt, float posX, float posZY, float posZ, float size) {}
    virtual void registerGrid(int xres, int yres, float color0[4], float color1[4]) {
    	cout << "c\n";
    }
    
    virtual struct	GLInstanceRendererInternalData* getInternalData()
    {
    	cout << "d\n";
    	return 0;
    }
    
};






static btVector4 sColors[4] =
{
	btVector4(0.3,0.3,1,1),
	btVector4(0.6,0.6,1,1),
	btVector4(0,1,0,1),
	btVector4(0,1,1,1),
	//btVector4(1,1,0,1),
};


struct MyDebugVec3
{
	MyDebugVec3(const btVector3& org)
	:x(org.x()),
	y(org.y()),
	z(org.z())
	{
	}

	float x;
	float y;
	float z;
};
class MyDebugDrawer : public btIDebugDraw
{
	CommonGraphicsApp* m_glApp;
	int m_debugMode;

    btAlignedObjectArray<MyDebugVec3> m_linePoints;
    btAlignedObjectArray<unsigned int> m_lineIndices;
    btVector3 m_currentLineColor;
	DefaultColors m_ourColors;

public:

	MyDebugDrawer(CommonGraphicsApp* app)
		: m_glApp(app)
		,m_debugMode(btIDebugDraw::DBG_DrawWireframe|btIDebugDraw::DBG_DrawAabb),
		m_currentLineColor(-1,-1,-1)
	{
		
		
	}
	virtual DefaultColors	getDefaultColors() const	
	{	
		return m_ourColors;
	}
	///the default implementation for setDefaultColors has no effect. A derived class can implement it and store the colors.
	virtual void setDefaultColors(const DefaultColors& colors) 
	{
		m_ourColors = colors;
	}


	virtual void drawLine(const btVector3& from1,const btVector3& to1,const btVector3& color1)
	{
        //float from[4] = {from1[0],from1[1],from1[2],from1[3]};
        //float to[4] = {to1[0],to1[1],to1[2],to1[3]};
        //float color[4] = {color1[0],color1[1],color1[2],color1[3]};
		//m_glApp->m_instancingRenderer->drawLine(from,to,color);
		if (m_currentLineColor!=color1 || m_linePoints.size() >= BT_LINE_BATCH_SIZE)
        {
            flushLines();
            m_currentLineColor = color1;
        }
		MyDebugVec3 from(from1);
		MyDebugVec3 to(to1);
			
		m_linePoints.push_back(from);
		m_linePoints.push_back(to);

		m_lineIndices.push_back(m_lineIndices.size());
		m_lineIndices.push_back(m_lineIndices.size());

	}

	virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
	{
        drawLine(PointOnB,PointOnB+normalOnB,color);
	}
     

	virtual void reportErrorWarning(const char* warningString)
	{
	}

	virtual void draw3dText(const btVector3& location,const char* textString)
	{
	}

	virtual void setDebugMode(int debugMode)
	{
		m_debugMode = debugMode;
	}

	virtual int		getDebugMode() const
	{
		return m_debugMode;
	}

    virtual void flushLines()
	{
	    int sz = m_linePoints.size();
	    if (sz)
        {
			float debugColor[4];
		debugColor[0] = m_currentLineColor.x();
		debugColor[1] = m_currentLineColor.y();
		debugColor[2] = m_currentLineColor.z();
		debugColor[3] = 1.f;
		m_glApp->m_renderer->drawLines(&m_linePoints[0].x,debugColor,
														 m_linePoints.size(),sizeof(MyDebugVec3),
														 &m_lineIndices[0],
														 m_lineIndices.size(),
														 1);
            m_linePoints.clear();
            m_lineIndices.clear();
        }
	}

};




struct MyConvertPointerSizeT
{
	union 
	{
			const void* m_ptr;
			size_t m_int;
	};
};

bool shapePointerCompareFunc(const btCollisionObject* colA, const btCollisionObject* colB)
{
	MyConvertPointerSizeT a,b;
	a.m_ptr = colA->getCollisionShape();
	b.m_ptr = colB->getCollisionShape();
	return (a.m_int<b.m_int);
}

struct MyGLHelperInternalData
{
	struct CommonGraphicsApp* m_glApp;
	class MyDebugDrawer* m_debugDraw;
	MyShapeDrawer* m_gl2ShapeDrawer;
};














// %%%%%%%%%%%%%%%%%%%%%%%%%


/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/














struct CustFilterCallback : public btOverlapFilterCallback
{
	
	// CustFilterCallback()
	// {
	// }
	// virtual ~CustFilterCallback()
	// {
	// }
	
	// return true when pairs need collision
	virtual bool needBroadphaseCollision(
		btBroadphaseProxy* proxy0,
		btBroadphaseProxy* proxy1) const
	{
		
		if (EDIT_POSE) {
			return false;
		}
		
		bool collides = 
		(
			((proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0) &&
			((proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask) != 0)
		);


		
		btCollisionObject* colObj0 = (btCollisionObject*)(proxy0->m_clientObject);
		btCollisionObject* colObj1 = (btCollisionObject*)(proxy1->m_clientObject);
		
		
		
		// if (colObj0->bodyUID != colObj1->bodyUID) {
		// 	cout << colObj0->bodyUID << " " << colObj1->bodyUID << "\n";
		// }
		
		collides = collides && (
			(colObj0->bodyUID != colObj1->bodyUID) &&
			(colObj0->bodyUID > -2) &&
			(colObj1->bodyUID > -2) &&
			(colObj0->heldByUID != colObj1->bodyUID) &&
			(colObj1->heldByUID != colObj0->bodyUID)
		);
		
		
		
		return collides;
	}
};




class BenchmarkDemo : public CommonRigidBodyBase
{

	//keep the collision shapes, for deletion/cleanup

	btAlignedObjectArray<class RagDoll*>	m_ragdolls;
	
	//btOverlapFilterCallback * filterCallback;
	
	btVector3 initOffset;
	
	int	m_benchmark;

	void myinit()
	{
		//??
	}

	void setCameraDistance(btScalar dist)
	{
	}
	void createTest1();
	void createTest2();
	void createTest3();
	void createTest4();
	void createTest5();
	void createTest6();
	void createTest7();
	void createTest8();

	
	void createWall(const btVector3& offsetPosition,int stackSize,const btVector3& boxSize);
	void createPyramid(const btVector3& offsetPosition,int stackSize,const btVector3& boxSize);
	void createTowerCircle(const btVector3& offsetPosition,int stackSize,int rotSize,const btVector3& boxSize);
	//void createLargeMeshBody();


	class SpuBatchRaycaster* m_batchRaycaster;
	class btThreadSupportInterface* m_batchRaycasterThreadSupport;

	void castRays();
	void initRays();

	public:

	
	BenchmarkDemo(struct GUIHelperInterface* helper, int benchmark)
	:CommonRigidBodyBase(helper),
	m_benchmark(benchmark)
	{
	}
	virtual ~BenchmarkDemo()
	{
		exitPhysics();
	}
	
	btDiscreteDynamicsWorld* getWorld();
	
	btRigidBody* createRigidBodyMask(
		btScalar mass, const btTransform& startTransform, btCollisionShape* shape, int maskFrom, int maskTo
	);
	
	void removeRigidBody(btRigidBody* body);
	
	void updateGraphicsObjects();
	
	void beginDrop(float x, float y, float z);
	
	// btRigidBody* bodyPick(const btVector3& rayFromWorld, const btVector3& rayToWorld);
	
	void initPhysics();

	void exitPhysics();

	void stepSimulation(float deltaTime);

	void resetCamera()
	{
		float dist = 120;
		float pitch = 52;
		float yaw = 35;
		float targetPos[3]={0,10.46,0};
		m_guiHelper->resetCamera(dist,pitch,yaw,targetPos[0],targetPos[1],targetPos[2]);
	}
};

// btRigidBody* BenchmarkDemo::bodyPick(const btVector3& rayFromWorld, const btVector3& rayToWorld) {
	
// 	if (m_dynamicsWorld==0) {
// 		cout << "world not ready\n";
// 		return NULL;
// 	}

// 	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFromWorld, rayToWorld);

// 	m_dynamicsWorld->rayTest(rayFromWorld, rayToWorld, rayCallback);
// 	if (rayCallback.hasHit())
// 	{

// 		btVector3 pickPos = rayCallback.m_hitPointWorld;
// 		btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
// 		if (body)
// 		{
// 			//other exclusions?
// 			if (!(body->isStaticObject() || body->isKinematicObject()))
// 			{
				
// 				return body;
				
// 				// m_pickedBody = body;
// 				// m_savedState = m_pickedBody->getActivationState();
// 				// m_pickedBody->setActivationState(DISABLE_DEACTIVATION);
// 				// //printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());
// 				// btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
// 				// btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body, localPivot);
// 				// m_dynamicsWorld->addConstraint(p2p, true);
// 				// m_pickedConstraint = p2p;
// 				// btScalar mousePickClamping = 30.f;
// 				// p2p->m_setting.m_impulseClamp = mousePickClamping;
// 				// //very weak constraint for picking
// 				// p2p->m_setting.m_tau = 0.001f;
// 			}
// 		}
		
		
// 		// m_oldPickingPos = rayToWorld;
// 		// m_hitPos = pickPos;
// 		// m_oldPickingDist = (pickPos - rayFromWorld).length();
// 	}
// 	return NULL;
	
// }


class btRaycastBar2
{
public:
	btVector3 source[NUMRAYS];
	btVector3 dest[NUMRAYS];
	btVector3 direction[NUMRAYS];
	btVector3 hit[NUMRAYS];
	btVector3 normal[NUMRAYS];
	struct GUIHelperInterface* m_guiHelper;
	
	int frame_counter;
	int ms;
	int sum_ms;
	int sum_ms_samples;
	int min_ms;
	int max_ms;

#ifdef USE_BT_CLOCK
	btClock frame_timer;
#endif //USE_BT_CLOCK

	btScalar dx;
	btScalar min_x;
	btScalar max_x;
	btScalar max_y;
	btScalar sign;

	btRaycastBar2 ()
	{
		m_guiHelper = 0;
		ms = 0;
		max_ms = 0;
		min_ms = 9999;
		sum_ms_samples = 0;
		sum_ms = 0;
	}



	btRaycastBar2 (btScalar ray_length, btScalar z,btScalar max_y,struct GUIHelperInterface* guiHelper)
	{
		m_guiHelper = guiHelper;
		frame_counter = 0;
		ms = 0;
		max_ms = 0;
		min_ms = 9999;
		sum_ms_samples = 0;
		sum_ms = 0;
		dx = 10.0;
		min_x = 0;
		max_x = 0;
		this->max_y = max_y;
		sign = 1.0;
		btScalar dalpha = 2*SIMD_2_PI/NUMRAYS;
		for (int i = 0; i < NUMRAYS; i++)
		{
			btScalar alpha = dalpha * i;
			// rotate around by alpha degrees y 
			btQuaternion q(btVector3(0.0, 1.0, 0.0), alpha);
			direction[i] = btVector3(1.0, 0.0, 0.0);
			direction[i] = quatRotate(q , direction[i]);
			direction[i] = direction[i] * ray_length;
			
			
			source[i] = btVector3(min_x, max_y, z);
			dest[i] = source[i] + direction[i];
			dest[i][1]=-1000;
			normal[i] = btVector3(1.0, 0.0, 0.0);
		}
	}

	void move (btScalar dt)
	{
		if (dt > btScalar(1.0/60.0))
			dt = btScalar(1.0/60.0);
		for (int i = 0; i < NUMRAYS; i++)
		{
			source[i][0] += dx * dt * sign;
			dest[i][0] += dx * dt * sign;
		}
		if (source[0][0] < min_x)
			sign = 1.0;
		else if (source[0][0] > max_x)
			sign = -1.0;
	}

	void cast (btCollisionWorld* cw)
	{
#ifdef USE_BT_CLOCK
		frame_timer.reset ();
#endif //USE_BT_CLOCK

#ifdef BATCH_RAYCASTER
		if (!gBatchRaycaster)
			return;

		gBatchRaycaster->clearRays ();
		for (int i = 0; i < NUMRAYS; i++)
		{
			gBatchRaycaster->addRay (source[i], dest[i]);
		}
		gBatchRaycaster->performBatchRaycast ();
		for (int i = 0; i < gBatchRaycaster->getNumRays (); i++)
		{
				const SpuRaycastTaskWorkUnitOut& out = (*gBatchRaycaster)[i];
				hit[i].setInterpolate3(source[i],dest[i],out.hitFraction);
				normal[i] = out.hitNormal;
				normal[i].normalize ();
		}
#else
		for (int i = 0; i < NUMRAYS; i++)
		{
			btCollisionWorld::ClosestRayResultCallback cb(source[i], dest[i]);
			
			cw->rayTest (source[i], dest[i], cb);
			if (cb.hasHit ())
			{
				hit[i] = cb.m_hitPointWorld;
				normal[i] = cb.m_hitNormalWorld;
				normal[i].normalize ();
			} else {
				hit[i] = dest[i];
				normal[i] = btVector3(1.0, 0.0, 0.0);
			}

		}
#ifdef USE_BT_CLOCK
		ms += frame_timer.getTimeMilliseconds ();
#endif //USE_BT_CLOCK
		frame_counter++;
		if (frame_counter > 50)
		{
			min_ms = ms < min_ms ? ms : min_ms;
			max_ms = ms > max_ms ? ms : max_ms;
			sum_ms += ms;
			sum_ms_samples++;
			btScalar mean_ms = (btScalar)sum_ms/(btScalar)sum_ms_samples;
			printf("%d rays in %d ms %d %d %f\n", NUMRAYS * frame_counter, ms, min_ms, max_ms, mean_ms);
			ms = 0;
			frame_counter = 0;
		}
#endif
	}

	void draw ()
	{
		
		if (m_guiHelper)
		{
			btAlignedObjectArray<unsigned int> indices;
			btAlignedObjectArray<btVector3FloatData> points;
			
			
			float lineColor[4]={1,0.4,.4,1};
			
			for (int i = 0; i < NUMRAYS; i++)
			{
				btVector3FloatData s,h;
				for (int w=0;w<4;w++)
				{
					s.m_floats[w] = source[i][w];
					h.m_floats[w] = hit[i][w];
				}
				
				points.push_back(s);
				points.push_back(h);
				indices.push_back(indices.size());
				indices.push_back(indices.size());
			}

			m_guiHelper->getRenderInterface()->drawLines(&points[0].m_floats[0],lineColor,points.size(),sizeof(btVector3),&indices[0],indices.size(),1);
		}
													 
#if 0
		glDisable (GL_LIGHTING);
		glColor3f (0.0, 1.0, 0.0);
		glBegin (GL_LINES);
		int i;

		for (i = 0; i < NUMRAYS; i++)
		{
			glVertex3f (source[i][0], source[i][1], source[i][2]);
			glVertex3f (hit[i][0], hit[i][1], hit[i][2]);
		}
		glEnd ();
		glColor3f (1.0, 1.0, 1.0);
		glBegin (GL_LINES);
		for (i = 0; i < NUMRAYS; i++)
		{
			glVertex3f (hit[i][0], hit[i][1], hit[i][2]);
			glVertex3f (hit[i][0] + normal[i][0], hit[i][1] + normal[i][1], hit[i][2] + normal[i][2]);
		}
		glEnd ();
		glColor3f (0.0, 1.0, 1.0);
		glBegin (GL_POINTS);
		for ( i = 0; i < NUMRAYS; i++)
		{
			glVertex3f (hit[i][0], hit[i][1], hit[i][2]);
		}
		glEnd ();
		glEnable (GL_LIGHTING);
#endif //USE_GRAPHICAL_BENCHMARK

	}
};


static btRaycastBar2 raycastBar;

btDiscreteDynamicsWorld* BenchmarkDemo::getWorld() {
	return m_dynamicsWorld;
}

void BenchmarkDemo::stepSimulation(float deltaTime)
{
	if (m_dynamicsWorld)
	{
		m_dynamicsWorld->stepSimulation(deltaTime);
	}
	
	if (m_benchmark==7)
	{
		castRays();

		raycastBar.draw();
	
	}

}

void BenchmarkDemo::removeRigidBody(btRigidBody* body) {
	m_dynamicsWorld->removeRigidBody(body);
}


btRigidBody* BenchmarkDemo::createRigidBodyMask(
	btScalar mass, const btTransform& startTransform, btCollisionShape* shape, int maskFrom, int maskTo
) {

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic) {
		shape->calculateLocalInertia(mass,localInertia);
	}
		

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	//add the body to the dynamics world 
	m_dynamicsWorld->addRigidBody(body, maskFrom, maskTo); //
	
	return body;
}


void BenchmarkDemo::updateGraphicsObjects() {
	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}


void BenchmarkDemo::beginDrop(float x, float y, float z) {
	initOffset = btVector3(x,y,z);
	createTest5();
	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

void BenchmarkDemo::initPhysics()
{
	cout << "BenchmarkDemo:initPhysics()\n";
	
	
	m_guiHelper->setUpAxis(2);

	setCameraDistance(btScalar(100.));

	///collision configuration contains default setup for memory, collision setup
	btDefaultCollisionConstructionInfo cci;
	cci.m_defaultMaxPersistentManifoldPoolSize = 32768;
	m_collisionConfiguration = new btDefaultCollisionConfiguration(cci);

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
	
	m_dispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);



	///the maximum size of the collision world. Make sure objects stay within these boundaries
	///Don't make the world AABB size too large, it will harm simulation quality and performance
	btVector3 worldAabbMin(-16384,-16384,-16384);
	btVector3 worldAabbMax(16384,16384,16384);
	
//	btHashedOverlappingPairCache* pairCache = new btHashedOverlappingPairCache();
//	m_broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,3500,pairCache);
//	m_broadphase = new btSimpleBroadphase();
	m_broadphase = new btDbvtBroadphase();
	

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	
	
	m_solver = sol;

	btDiscreteDynamicsWorld* dynamicsWorld;
	m_dynamicsWorld = dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	

	///the following 3 lines increase the performance dramatically, with a little bit of loss of quality
	m_dynamicsWorld->getSolverInfo().m_solverMode |=SOLVER_ENABLE_FRICTION_DIRECTION_CACHING; //don't recalculate friction values each frame
	dynamicsWorld->getSolverInfo().m_numIterations = 5; //few solver iterations 
	//m_defaultContactProcessingThreshold = 0.f;//used when creating bodies: body->setContactProcessingThreshold(...);
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);
	

	m_dynamicsWorld->setGravity(btVector3(0,0,-10));
	
	
	
	

	// if (m_benchmark<5)
	// {
	// 	///create a few basic rigid bodies
	// 	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(250.),btScalar(250.),btScalar(50.)));
	// //	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),0);
		
	// 	m_collisionShapes.push_back(groundShape);

	// 	btTransform groundTransform;
	// 	groundTransform.setIdentity();
	// 	groundTransform.setOrigin(btVector3(0,0,-50));

	// 	//We can also use DemoApplication::createRigidBody, but for clarity it is provided here:
	// 	{
	// 		btScalar mass(0.);

	// 		//rigidbody is dynamic if and only if mass is non zero, otherwise static
	// 		bool isDynamic = (mass != 0.f);

	// 		btVector3 localInertia(0,0,0);
	// 		if (isDynamic)
	// 			groundShape->calculateLocalInertia(mass,localInertia);

	// 		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	// 		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	// 		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
	// 		btRigidBody* body = new btRigidBody(rbInfo);

	// 		//add the body to the dynamics world
	// 		m_dynamicsWorld->addRigidBody(body);
	// 	}
	// }

	// switch (m_benchmark)
	// {
	// 	case 1:
	// 		{
	// 			createTest1();
	// 			break;
	// 		}
	// 	case 2:
	// 		{
	// 			createTest2();
	// 			break;
	// 		}
	// 	case 3:
	// 		{
	// 			createTest3();
	// 			break;
	// 		}
	// 	case 4:
	// 		{
	// 			createTest4();
	// 			break;
	// 		}
	// 	case 5:
	// 		{
	// 			createTest5();
	// 			break;
	// 		}
	// 	case 6:
	// 	{
	// 		createTest6();
	// 		break;
	// 	}
	// 	case 7:
	// 	{
	// 		createTest7();
	// 		break;
	// 	}
	// 	case 8:
	// 	{
	// 		createTest8();
	// 		break;
	// 	}


	// default:
	// 	{
	// 	}			
	// }

	

}
	

void BenchmarkDemo::createTest1()
{
	// 3000
	int size = 8;
	const float cubeSize = 1.0f;
	float spacing = cubeSize;
	btVector3 pos(0.0f, cubeSize * 2,0.f);
	float offset = -size * (cubeSize * 2.0f + spacing) * 0.5f;

	btBoxShape* blockShape = new btBoxShape(btVector3(cubeSize-COLLISION_RADIUS,cubeSize-COLLISION_RADIUS,cubeSize-COLLISION_RADIUS));
	btVector3 localInertia(0,0,0);
	float mass = 2.f;
	blockShape->calculateLocalInertia(mass,localInertia);
	
	btTransform trans;
	trans.setIdentity();

	for(int k=0;k<47;k++) {
		for(int j=0;j<size;j++) {
			pos[2] = offset + (float)j * (cubeSize * 2.0f + spacing);
			for(int i=0;i<size;i++) {
				pos[0] = offset + (float)i * (cubeSize * 2.0f + spacing);

				trans.setOrigin(pos);
				btRigidBody* cmbody;
				cmbody= createRigidBody(mass,trans,blockShape);
			}
		}
		offset -= 0.05f * spacing * (size-1);
//		spacing *= 1.01f;
		pos[1] += (cubeSize * 2.0f + spacing);
	}
}


///////////////////////////////////////////////////////////////////////////////
// Pyramid 3

void BenchmarkDemo::createWall(const btVector3& offsetPosition,int stackSize,const btVector3& boxSize)
{
	
	btBoxShape* blockShape = new btBoxShape(btVector3(boxSize[0]-COLLISION_RADIUS,boxSize[1]-COLLISION_RADIUS,boxSize[2]-COLLISION_RADIUS));

	float mass = 1.f;
	btVector3 localInertia(0,0,0);
	blockShape->calculateLocalInertia(mass,localInertia);

//	btScalar  diffX = boxSize[0] * 1.0f;
	btScalar  diffY = boxSize[1] * 1.0f;
	btScalar  diffZ = boxSize[2] * 1.0f;

	btScalar  offset = -stackSize * (diffZ * 2.0f) * 0.5f;
	btVector3 pos(0.0f, diffY, 0.0f);

	btTransform trans;
	trans.setIdentity();

	while(stackSize) {
		for(int i=0;i<stackSize;i++) {
			pos[2] = offset + (float)i * (diffZ * 2.0f);

		trans.setOrigin(offsetPosition + pos);
		createRigidBody(mass,trans,blockShape);
		
		}
		offset += diffZ;
		pos[1] += (diffY * 2.0f);
		stackSize--;
	}
}

void BenchmarkDemo::createPyramid(const btVector3& offsetPosition,int stackSize,const btVector3& boxSize)
{
	btScalar space = 0.0001f;
	
	btVector3 pos(0.0f, boxSize[1], 0.0f);

	btBoxShape* blockShape = new btBoxShape(btVector3(boxSize[0]-COLLISION_RADIUS,boxSize[1]-COLLISION_RADIUS,boxSize[2]-COLLISION_RADIUS));
	btTransform trans;
	trans.setIdentity();

	btScalar mass = 1.f;
	btVector3 localInertia(0,0,0);
	blockShape->calculateLocalInertia(mass,localInertia);

	
	btScalar diffX = boxSize[0]*1.02f;
	btScalar diffY = boxSize[1]*1.02f;
	btScalar diffZ = boxSize[2]*1.02f;
	
	btScalar offsetX = -stackSize * (diffX * 2.0f + space) * 0.5f;
	btScalar offsetZ = -stackSize * (diffZ * 2.0f + space) * 0.5f;
	while(stackSize) {
		for(int j=0;j<stackSize;j++) {
			pos[2] = offsetZ + (float)j * (diffZ * 2.0f + space);
			for(int i=0;i<stackSize;i++) {
				pos[0] = offsetX + (float)i * (diffX * 2.0f + space);
				trans.setOrigin(offsetPosition + pos);
				this->createRigidBody(mass,trans,blockShape);


			}
		}
		offsetX += diffX;
		offsetZ += diffZ;
		pos[1] += (diffY * 2.0f + space);
		stackSize--;
	}

}

 const btVector3 rotate( const btQuaternion& quat, const btVector3 & vec ) {
    float tmpX, tmpY, tmpZ, tmpW;
    tmpX = ( ( ( quat.getW() * vec.getX() ) + ( quat.getY() * vec.getZ() ) ) - ( quat.getZ() * vec.getY() ) );
    tmpY = ( ( ( quat.getW() * vec.getY() ) + ( quat.getZ() * vec.getX() ) ) - ( quat.getX() * vec.getZ() ) );
    tmpZ = ( ( ( quat.getW() * vec.getZ() ) + ( quat.getX() * vec.getY() ) ) - ( quat.getY() * vec.getX() ) );
    tmpW = ( ( ( quat.getX() * vec.getX() ) + ( quat.getY() * vec.getY() ) ) + ( quat.getZ() * vec.getZ() ) );
    return btVector3(
        ( ( ( ( tmpW * quat.getX() ) + ( tmpX * quat.getW() ) ) - ( tmpY * quat.getZ() ) ) + ( tmpZ * quat.getY() ) ),
        ( ( ( ( tmpW * quat.getY() ) + ( tmpY * quat.getW() ) ) - ( tmpZ * quat.getX() ) ) + ( tmpX * quat.getZ() ) ),
        ( ( ( ( tmpW * quat.getZ() ) + ( tmpZ * quat.getW() ) ) - ( tmpX * quat.getY() ) ) + ( tmpY * quat.getX() ) )
    );
}

void BenchmarkDemo::createTowerCircle(const btVector3& offsetPosition,int stackSize,int rotSize,const btVector3& boxSize)
{

	btBoxShape* blockShape = new btBoxShape(btVector3(boxSize[0]-COLLISION_RADIUS,boxSize[1]-COLLISION_RADIUS,boxSize[2]-COLLISION_RADIUS));

	btTransform trans;
	trans.setIdentity();

	float mass = 1.f;
	btVector3 localInertia(0,0,0);
	blockShape->calculateLocalInertia(mass,localInertia);


	float radius = 1.3f * rotSize * boxSize[0] / SIMD_PI;

	// create active boxes
	btQuaternion rotY(0,1,0,0);
	float posY = boxSize[1];

	for(int i=0;i<stackSize;i++) {
		for(int j=0;j<rotSize;j++) {
		

			trans.setOrigin(offsetPosition+  rotate(rotY,btVector3(0.0f , posY, radius)));
			trans.setRotation(rotY);
			createRigidBody(mass,trans,blockShape);

			rotY *= btQuaternion(btVector3(0,1,0),SIMD_PI/(rotSize*btScalar(0.5)));
		}

		posY += boxSize[1] * 2.0f;
		rotY *= btQuaternion(btVector3(0,1,0),SIMD_PI/(float)rotSize);
	}
	
}

void BenchmarkDemo::createTest2()
{
	setCameraDistance(btScalar(50.));
	const float cubeSize = 1.0f;

	createPyramid(btVector3(-20.0f,0.0f,0.0f),12,btVector3(cubeSize,cubeSize,cubeSize));
	createWall(btVector3(-2.0f,0.0f,0.0f),12,btVector3(cubeSize,cubeSize,cubeSize));
	createWall(btVector3(4.0f,0.0f,0.0f),12,btVector3(cubeSize,cubeSize,cubeSize));
	createWall(btVector3(10.0f,0.0f,0.0f),12,btVector3(cubeSize,cubeSize,cubeSize));
	createTowerCircle(btVector3(25.0f,0.0f,0.0f),8,24,btVector3(cubeSize,cubeSize,cubeSize));
	
}




class RagDoll
{
	enum
	{
		BODYPART_PELVIS = 0,
		BODYPART_SPINE,
		BODYPART_HEAD,

		BODYPART_LEFT_UPPER_LEG,
		BODYPART_LEFT_LOWER_LEG,

		BODYPART_RIGHT_UPPER_LEG,
		BODYPART_RIGHT_LOWER_LEG,

		BODYPART_LEFT_UPPER_ARM,
		BODYPART_LEFT_LOWER_ARM,

		BODYPART_RIGHT_UPPER_ARM,
		BODYPART_RIGHT_LOWER_ARM,

		BODYPART_COUNT
	};

	enum
	{
		JOINT_PELVIS_SPINE = 0,
		JOINT_SPINE_HEAD,

		JOINT_LEFT_HIP,
		JOINT_LEFT_KNEE,

		JOINT_RIGHT_HIP,
		JOINT_RIGHT_KNEE,

		JOINT_LEFT_SHOULDER,
		JOINT_LEFT_ELBOW,

		JOINT_RIGHT_SHOULDER,
		JOINT_RIGHT_ELBOW,

		JOINT_COUNT
	};

	btDynamicsWorld* m_ownerWorld;
	btCollisionShape* m_shapes[BODYPART_COUNT];
	btRigidBody* m_bodies[BODYPART_COUNT];
	btTypedConstraint* m_joints[JOINT_COUNT];

	btRigidBody* createRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
	{
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic) {
			shape->calculateLocalInertia(mass,localInertia);
		}
		

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_ownerWorld->addRigidBody(body);

		return body;
	}

public:
	RagDoll (btDynamicsWorld* ownerWorld, const btVector3& positionOffset,btScalar scale)
		: m_ownerWorld (ownerWorld)
	{
		// Setup the geometry
		m_shapes[BODYPART_PELVIS] = new btCapsuleShape(btScalar(0.15)*scale, btScalar(0.20)*scale);
		m_shapes[BODYPART_SPINE] = new btCapsuleShape(btScalar(0.15)*scale, btScalar(0.28)*scale);
		m_shapes[BODYPART_HEAD] = new btCapsuleShape(btScalar(0.10)*scale, btScalar(0.05)*scale);
		m_shapes[BODYPART_LEFT_UPPER_LEG] = new btCapsuleShape(btScalar(0.07)*scale, btScalar(0.45)*scale);
		m_shapes[BODYPART_LEFT_LOWER_LEG] = new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.37)*scale);
		m_shapes[BODYPART_RIGHT_UPPER_LEG] = new btCapsuleShape(btScalar(0.07)*scale, btScalar(0.45)*scale);
		m_shapes[BODYPART_RIGHT_LOWER_LEG] = new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.37)*scale);
		m_shapes[BODYPART_LEFT_UPPER_ARM] = new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.33)*scale);
		m_shapes[BODYPART_LEFT_LOWER_ARM] = new btCapsuleShape(btScalar(0.04)*scale, btScalar(0.25)*scale);
		m_shapes[BODYPART_RIGHT_UPPER_ARM] = new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.33)*scale);
		m_shapes[BODYPART_RIGHT_LOWER_ARM] = new btCapsuleShape(btScalar(0.04)*scale, btScalar(0.25)*scale);

		// Setup all the rigid bodies
		btTransform offset; offset.setIdentity();
		offset.setOrigin(positionOffset);

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.), btScalar(1.), btScalar(0.)));
		m_bodies[BODYPART_PELVIS] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_PELVIS]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.), btScalar(1.2), btScalar(0.)));
		m_bodies[BODYPART_SPINE] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_SPINE]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.), btScalar(1.6), btScalar(0.)));
		m_bodies[BODYPART_HEAD] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_HEAD]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(-0.18), btScalar(0.65), btScalar(0.)));
		m_bodies[BODYPART_LEFT_UPPER_LEG] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_UPPER_LEG]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(-0.18), btScalar(0.2), btScalar(0.)));
		m_bodies[BODYPART_LEFT_LOWER_LEG] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_LOWER_LEG]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.18), btScalar(0.65), btScalar(0.)));
		m_bodies[BODYPART_RIGHT_UPPER_LEG] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_UPPER_LEG]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.18), btScalar(0.2), btScalar(0.)));
		m_bodies[BODYPART_RIGHT_LOWER_LEG] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_LOWER_LEG]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(-0.35), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,M_PI_2);
		m_bodies[BODYPART_LEFT_UPPER_ARM] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_UPPER_ARM]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(-0.7), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,M_PI_2);
		m_bodies[BODYPART_LEFT_LOWER_ARM] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_LOWER_ARM]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.35), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,-M_PI_2);
		m_bodies[BODYPART_RIGHT_UPPER_ARM] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_UPPER_ARM]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.7), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,-M_PI_2);
		m_bodies[BODYPART_RIGHT_LOWER_ARM] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_LOWER_ARM]);

		// Setup some damping on the m_bodies
		for (int i = 0; i < BODYPART_COUNT; ++i)
		{
			m_bodies[i]->setDamping(btScalar(0.05), btScalar(0.85));
			m_bodies[i]->setDeactivationTime(btScalar(0.8));
			m_bodies[i]->setSleepingThresholds(btScalar(1.6), btScalar(2.5));
		}

		// Now setup the constraints
		btHingeConstraint* hingeC;
		btConeTwistConstraint* coneC;

		btTransform localA, localB;

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.15), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.15), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_SPINE], localA, localB);
		hingeC->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
		m_joints[JOINT_PELVIS_SPINE] = hingeC;
		m_ownerWorld->addConstraint(m_joints[JOINT_PELVIS_SPINE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,M_PI_2); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.30), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_2); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_HEAD], localA, localB);
		coneC->setLimit(M_PI_4, M_PI_4, M_PI_2);
		m_joints[JOINT_SPINE_HEAD] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_SPINE_HEAD], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,-M_PI_4*5); localA.setOrigin(scale*btVector3(btScalar(-0.18), btScalar(-0.10), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,-M_PI_4*5); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_LEFT_UPPER_LEG], localA, localB);
		coneC->setLimit(M_PI_4, M_PI_4, 0);
		m_joints[JOINT_LEFT_HIP] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_HIP], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_LEFT_UPPER_LEG], *m_bodies[BODYPART_LEFT_LOWER_LEG], localA, localB);
		hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
		m_joints[JOINT_LEFT_KNEE] = hingeC;
		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_KNEE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,M_PI_4); localA.setOrigin(scale*btVector3(btScalar(0.18), btScalar(-0.10), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_4); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_RIGHT_UPPER_LEG], localA, localB);
		coneC->setLimit(M_PI_4, M_PI_4, 0);
		m_joints[JOINT_RIGHT_HIP] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_HIP], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_RIGHT_UPPER_LEG], *m_bodies[BODYPART_RIGHT_LOWER_LEG], localA, localB);
		hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
		m_joints[JOINT_RIGHT_KNEE] = hingeC;
		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_KNEE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,M_PI); localA.setOrigin(scale*btVector3(btScalar(-0.2), btScalar(0.15), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_2); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_LEFT_UPPER_ARM], localA, localB);
		coneC->setLimit(M_PI_2, M_PI_2, 0);
		m_joints[JOINT_LEFT_SHOULDER] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_SHOULDER], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_LEFT_UPPER_ARM], *m_bodies[BODYPART_LEFT_LOWER_ARM], localA, localB);
		hingeC->setLimit(btScalar(-M_PI_2), btScalar(0));
		m_joints[JOINT_LEFT_ELBOW] = hingeC;
		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_ELBOW], true);



		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,0); localA.setOrigin(scale*btVector3(btScalar(0.2), btScalar(0.15), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_2); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_RIGHT_UPPER_ARM], localA, localB);
		coneC->setLimit(M_PI_2, M_PI_2, 0);
		m_joints[JOINT_RIGHT_SHOULDER] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_SHOULDER], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_RIGHT_UPPER_ARM], *m_bodies[BODYPART_RIGHT_LOWER_ARM], localA, localB);
		hingeC->setLimit(btScalar(-M_PI_2), btScalar(0));
		m_joints[JOINT_RIGHT_ELBOW] = hingeC;
		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_ELBOW], true);
	}

	virtual	~RagDoll ()
	{
		int i;

		// Remove all constraints
		for ( i = 0; i < JOINT_COUNT; ++i)
		{
			m_ownerWorld->removeConstraint(m_joints[i]);
			delete m_joints[i]; m_joints[i] = 0;
		}

		// Remove all bodies and shapes
		for ( i = 0; i < BODYPART_COUNT; ++i)
		{
			m_ownerWorld->removeRigidBody(m_bodies[i]);
			
			delete m_bodies[i]->getMotionState();

			delete m_bodies[i]; m_bodies[i] = 0;
			delete m_shapes[i]; m_shapes[i] = 0;
		}
	}
};

void BenchmarkDemo::createTest3()
{
	setCameraDistance(btScalar(50.));

	int size = 16;

	float sizeX = 1.f;
	float sizeY = 1.f;

	//int rc=0;

	btScalar scale(3.5);
	btVector3 pos(0.0f, sizeY, 0.0f);
	while(size) {
		float offset = -size * (sizeX * 6.0f) * 0.5f;
		for(int i=0;i<size;i++) {
			pos[0] = offset + (float)i * (sizeX * 6.0f);

				RagDoll* ragDoll = new RagDoll (m_dynamicsWorld,pos,scale);
				m_ragdolls.push_back(ragDoll);
		}

		offset += sizeX;
		pos[1] += (sizeY * 7.0f);
		pos[2] -= sizeX * 2.0f;
		size--;
	}

}
void BenchmarkDemo::createTest4()
{
	setCameraDistance(btScalar(50.));

	int size = 8;
	const float cubeSize = 1.5f;
	float spacing = cubeSize;
	btVector3 pos(0.0f, cubeSize * 2, 0.0f);
	float offset = -size * (cubeSize * 2.0f + spacing) * 0.5f;

	btConvexHullShape* convexHullShape = new btConvexHullShape();

	btScalar scaling(1);

	convexHullShape->setLocalScaling(btVector3(scaling,scaling,scaling));

	for (int i=0;i<TaruVtxCount;i++)
	{
		btVector3 vtx(TaruVtx[i*3],TaruVtx[i*3+1],TaruVtx[i*3+2]);
		convexHullShape->addPoint(vtx*btScalar(1./scaling));
	}

	//this will enable polyhedral contact clipping, better quality, slightly slower
	//convexHullShape->initializePolyhedralFeatures();

	btTransform trans;
	trans.setIdentity();

	float mass = 1.f;
	btVector3 localInertia(0,0,0);
	convexHullShape->calculateLocalInertia(mass,localInertia);

	for(int k=0;k<15;k++) {
		for(int j=0;j<size;j++) {
			pos[2] = offset + (float)j * (cubeSize * 2.0f + spacing);
			for(int i=0;i<size;i++) {
				pos[0] = offset + (float)i * (cubeSize * 2.0f + spacing);
				trans.setOrigin(pos);
				createRigidBody(mass,trans,convexHullShape);		
			}
		}
		offset -= 0.05f * spacing * (size-1);
		spacing *= 1.01f;
		pos[1] += (cubeSize * 2.0f + spacing);
	}
}


///////////////////////////////////////////////////////////////////////////////
// LargeMesh

// int LandscapeVtxCount[] = {
// 	Landscape01VtxCount,
// 	Landscape02VtxCount,
// 	Landscape03VtxCount,
// 	Landscape04VtxCount,
// 	Landscape05VtxCount,
// 	Landscape06VtxCount,
// 	Landscape07VtxCount,
// 	Landscape08VtxCount,
// };

// int LandscapeIdxCount[] = {
// 	Landscape01IdxCount,
// 	Landscape02IdxCount,
// 	Landscape03IdxCount,
// 	Landscape04IdxCount,
// 	Landscape05IdxCount,
// 	Landscape06IdxCount,
// 	Landscape07IdxCount,
// 	Landscape08IdxCount,
// };

// btScalar *LandscapeVtx[] = {
// 	Landscape01Vtx,
// 	Landscape02Vtx,
// 	Landscape03Vtx,
// 	Landscape04Vtx,
// 	Landscape05Vtx,
// 	Landscape06Vtx,
// 	Landscape07Vtx,
// 	Landscape08Vtx,
// };

// btScalar *LandscapeNml[] = {
// 	Landscape01Nml,
// 	Landscape02Nml,
// 	Landscape03Nml,
// 	Landscape04Nml,
// 	Landscape05Nml,
// 	Landscape06Nml,
// 	Landscape07Nml,
// 	Landscape08Nml,
// };

// btScalar* LandscapeTex[] = {
// 	Landscape01Tex,
// 	Landscape02Tex,
// 	Landscape03Tex,
// 	Landscape04Tex,
// 	Landscape05Tex,
// 	Landscape06Tex,
// 	Landscape07Tex,
// 	Landscape08Tex,
// };

// unsigned short  *LandscapeIdx[] = {
// 	Landscape01Idx,
// 	Landscape02Idx,
// 	Landscape03Idx,
// 	Landscape04Idx,
// 	Landscape05Idx,
// 	Landscape06Idx,
// 	Landscape07Idx,
// 	Landscape08Idx,
// };

// void BenchmarkDemo::createLargeMeshBody()
// {
// 	btTransform trans;
// 	trans.setIdentity();

// 	for(int i=0;i<8;i++) {

// 		btTriangleIndexVertexArray* meshInterface = new btTriangleIndexVertexArray();
// 		btIndexedMesh part;

// 		part.m_vertexBase = (const unsigned char*)LandscapeVtx[i];
// 		part.m_vertexStride = sizeof(btScalar) * 3;
// 		part.m_numVertices = LandscapeVtxCount[i];
// 		part.m_triangleIndexBase = (const unsigned char*)LandscapeIdx[i];
// 		part.m_triangleIndexStride = sizeof( short) * 3;
// 		part.m_numTriangles = LandscapeIdxCount[i]/3;
// 		part.m_indexType = PHY_SHORT;

// 		meshInterface->addIndexedMesh(part,PHY_SHORT);

// 		bool	useQuantizedAabbCompression = true;
// 		btBvhTriangleMeshShape* trimeshShape = new btBvhTriangleMeshShape(meshInterface,useQuantizedAabbCompression);
// 		btVector3 localInertia(0,0,0);
// 		trans.setOrigin(btVector3(0,-25,0));

// 		btRigidBody* body = createRigidBody(0,trans,trimeshShape);
// 		body->setFriction (btScalar(0.9));
		
// 	}
	
// }


void BenchmarkDemo::createTest5()
{
	//setCameraDistance(btScalar(250.));
	btVector3 boxSize(1.5f,1.5f,1.5f);
	float boxMass = 1.0f;
	float sphereRadius = 1.5f;
	float sphereMass = 1.0f;
	float capsuleHalf = 2.0f;
	float capsuleRadius = 1.0f;
	float capsuleMass = 1.0f;

	btRigidBody* tempBody = NULL;

	{
		int size = 10;
		int height = 10;

		const float cubeSize = boxSize[0];
		float spacing = 2.0f;
		btVector3 pos(0.0f, 0.0f, 20.0f);
		float offset = -size * (cubeSize * 2.0f + spacing) * 0.5f;
		
		int numBodies = 0;

		for(int k=0;k<height;k++) {
			for(int j=0;j<size;j++) {
				pos[1] = offset + (float)j * (cubeSize * 2.0f + spacing);
				for(int i=0;i<size;i++) {
					pos[0] = offset + (float)i * (cubeSize * 2.0f + spacing);
					btVector3 bpos = btVector3(0,0,300) + btVector3(5.0f,5.0f,1.0f)*pos + initOffset;
					int idx = rand() % 9;
					btTransform trans;
					trans.setIdentity();
					trans.setOrigin(bpos);

					switch(idx) {
						case 0:case 1:case 2:
						{
							float r = 0.5f * (idx+1);
							btBoxShape* boxShape = new btBoxShape(boxSize*r);
							tempBody = createRigidBody(boxMass*r,trans,boxShape);
						}
						break;

						case 3:case 4:case 5:
						{
							float r = 0.5f * (idx-3+1);
							btSphereShape* sphereShape = new btSphereShape(sphereRadius*r);
							tempBody = createRigidBody(sphereMass*r,trans,sphereShape);
						}
						break;

						case 6:case 7:case 8:
						{
							float r = 0.5f * (idx-6+1);
							btCapsuleShape* capsuleShape = new btCapsuleShape(capsuleRadius*r,capsuleHalf*r);
							tempBody = createRigidBody(capsuleMass*r,trans,capsuleShape);
						}
						break;
					}
					
					tempBody->bodyUID = -1;
					tempBody->limbUID = -1;
					
					numBodies++;
				}
			}
			offset -= 0.05f * spacing * (size-1);
			spacing *= 1.1f;
			pos[1] += (cubeSize * 2.0f + spacing);
		}
	}

	//createLargeMeshBody();
}
void BenchmarkDemo::createTest6()
{
	setCameraDistance(btScalar(250.));

	btVector3 boxSize(1.5f,1.5f,1.5f);

	btConvexHullShape* convexHullShape = new btConvexHullShape();

	for (int i=0;i<TaruVtxCount;i++)
	{
		btVector3 vtx(TaruVtx[i*3],TaruVtx[i*3+1],TaruVtx[i*3+2]);
		convexHullShape->addPoint(vtx);
	}

	btTransform trans;
	trans.setIdentity();

	float mass = 1.f;
	btVector3 localInertia(0,0,0);
	convexHullShape->calculateLocalInertia(mass,localInertia);


	{
		int size = 10;
		int height = 10;

		const float cubeSize = boxSize[0];
		float spacing = 2.0f;
		btVector3 pos(0.0f, 20.0f, 0.0f);
		float offset = -size * (cubeSize * 2.0f + spacing) * 0.5f;
		
	
		for(int k=0;k<height;k++) {
			for(int j=0;j<size;j++) {
				pos[2] = offset + (float)j * (cubeSize * 2.0f + spacing);
				for(int i=0;i<size;i++) {
					pos[0] = offset + (float)i * (cubeSize * 2.0f + spacing);
					btVector3 bpos = btVector3(0,25,0) + btVector3(5.0f,1.0f,5.0f)*pos;
					trans.setOrigin(bpos);
					
					createRigidBody(mass,trans,convexHullShape);
				}
			}
			offset -= 0.05f * spacing * (size-1);
			spacing *= 1.1f;
			pos[1] += (cubeSize * 2.0f + spacing);
		}
	}


	//createLargeMeshBody();
}


void BenchmarkDemo::initRays()
{
	raycastBar = btRaycastBar2 (2500.0, 0,50.0,m_guiHelper);
}



void BenchmarkDemo::castRays()
{
	raycastBar.cast (m_dynamicsWorld);
}

void BenchmarkDemo::createTest7()
{
	
	createTest6();
	setCameraDistance(btScalar(150.));
	initRays();
}


void BenchmarkDemo::createTest8()
{
	setCameraDistance(btScalar(250.));
	btVector3 boxSize(1.5f,1.5f,1.5f);
	float boxMass = 1.0f;
	float sphereRadius = 1.5f;
	float sphereMass = 1.0f;
	float capsuleHalf = 2.0f;
	float capsuleRadius = 1.0f;
	float capsuleMass = 1.0f;

	{
		int size = 10;
		int height = 10;

		const float cubeSize = boxSize[0];
		float spacing = 2.0f;
		btVector3 pos(0.0f, 20.0f, 0.0f);
		float offset = -size * (cubeSize * 2.0f + spacing) * 0.5f;
		
		int numBodies = 0;

		for(int k=0;k<height;k++) {
			for(int j=0;j<size;j++) {
				pos[2] = offset + (float)j * (cubeSize * 2.0f + spacing);
				for(int i=0;i<size;i++) {
					pos[0] = offset + (float)i * (cubeSize * 2.0f + spacing);
					btVector3 bpos = btVector3(0,25,0) + btVector3(5.0f,1.0f,5.0f)*pos;
					int idx = rand() % 9;
					btTransform trans;
					trans.setIdentity();
					trans.setOrigin(bpos);

					switch(idx) {
						case 0:case 1:case 2:
						{
							float r = 0.5f * (idx+1);
							btBoxShape* boxShape = new btBoxShape(boxSize*r);
							createRigidBody(boxMass*r,trans,boxShape);
						}
						break;

						case 3:case 4:case 5:
						{
							float r = 0.5f * (idx-3+1);
							btSphereShape* sphereShape = new btSphereShape(sphereRadius*r);
							createRigidBody(sphereMass*r,trans,sphereShape);
						}
						break;

						case 6:case 7:case 8:
						{
							float r = 0.5f * (idx-6+1);
							btCapsuleShape* capsuleShape = new btCapsuleShape(capsuleRadius*r,capsuleHalf*r);
							createRigidBody(capsuleMass*r,trans,capsuleShape);
						}
						break;
					}

					numBodies++;
				}
			}
			offset -= 0.05f * spacing * (size-1);
			spacing *= 1.1f;
			pos[1] += (cubeSize * 2.0f + spacing);
		}
	}

	//createLargeMeshBody();
}


void BenchmarkDemo::exitPhysics()
{
	int i;

	for (i=0;i<m_ragdolls.size();i++)
	{
		RagDoll* doll = m_ragdolls[i];
		delete doll;
	}
	m_ragdolls.clear();

	CommonRigidBodyBase::exitPhysics();

	
}



btVector3 NET_MASKS[256];
void initNetMasks() {
	btVector3 nmDirs[8];
	
	int i;
	int j;
	int k;
	
	int ii;
	int jj;
	int kk;
	
	int tot;
	
	for (i = 0; i < 256; i++) {
		NET_MASKS[i] = btVector3(0.0f,0.0f,0.0f);
	}
	
	
	for (k = -1; k <= 1; k += 2) {
		for (j = -1; j <= 1; j += 2) {
			for (i = -1; i <= 1; i += 2) {
				ii = (i + 1)/2;
				jj = (j + 1)/2;
				kk = (k + 1)/2;
				
				nmDirs[ii+jj*2+kk*4] = btVector3(i,j,k);
				
			}	
		}
	}
	
	for (i = 0; i < 8; i++) {
		nmDirs[i].normalize();
	}
	
	
	for (i = 0; i < 256; i++) {
		tot = 0;
		for (j = 0; j < 8; j++) {
			if ((i&(1<<j)) > 0) {
				NET_MASKS[i] += nmDirs[j];
				tot++;
			}
		}
		
		if (
			(
				abs(NET_MASKS[i].getX()) +
				abs(NET_MASKS[i].getY()) +
				abs(NET_MASKS[i].getZ())		
			) == 0.0
		) {
			NET_MASKS[i] = btVector3(0.0f,0.0f,0.0f);
		}
		else {
			NET_MASKS[i].normalize();
			
			 // sqrt(0.5)/2 //0.35355339059f
			
			switch (tot) {
				
				case 1:
				case 7:
					NET_MASKS[i] = NET_MASKS[i] * (1.0f-0.43301270189f);//1.73205080757f / 4.0f; // sqrt(3)/4
				break;
				case 2:
				case 6:
					NET_MASKS[i] = NET_MASKS[i] * 1.41421356237f / 4.0f; // sqrt(2)/4
				break;
				// case 3:
				// 	NET_MASKS[i] = NET_MASKS[i] * -(1.0f-0.43301270189f);
				// break;
				default:
					NET_MASKS[i] = btVector3(0.0f,0.0f,0.0f);
				break;
				
			}
			
			if (tot > 4) {
				NET_MASKS[i] = -(NET_MASKS[i]);
			}
			
		}
		
		
		
		
		
	}
	
	
}

bool replaceStr(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

// struct CommonExampleInterface*    BenchmarkCreateFunc(struct CommonExampleOptions& options)
// {
// 	return new BenchmarkDemo(options.m_guiHelper,options.m_option);
// }







