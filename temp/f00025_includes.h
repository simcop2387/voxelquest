
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif


int RUN_COUNT;


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

const static bool GEN_COLLISION = false;
const static bool GEN_POLYS_HOLDER = false;
const static bool GEN_POLYS_WORLD = false;

const static bool SINGLE_THREADED = false;
const static bool DO_RANDOMIZE = false;

// no greater than 8 unless shader changed
const static int MAX_PRIM_IDS = 8;
const static int MAX_PRIMTEST = 8;

const static int MAX_DEPTH_PEELS = 4;

const static float OFFSET_X[4] = {-0.5,0.5,0.5,-0.5};
const static float OFFSET_Y[4] = {-0.5,-0.5,0.5,0.5};

const static int MAX_SPLASHES = 8;
const static int MAX_EXPLODES = 8;

//const static bool DO_CONNECT = true;
const static bool DO_SHADER_DUMP = false;


const static int DEF_WIN_W = 1440;
const static int DEF_WIN_H = 720;

const static int DEF_VOL_SIZE = 128;

const static int DEF_SCALE_FACTOR = 2;
const static int RENDER_SCALE_FACTOR = 2;
const static float SPHEREMAP_SCALE_FACTOR = 0.5f; // lower is faster

const static bool USE_SPHERE_MAP = false;

const static float TIME_DELTA = 1.0f/60.0f;

const static int THREAD_DATA_COUNT = 16;

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

const static bool DO_POINTS = true;


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

int PAGE_COUNT = 0;
// set to 0 to disable
int MAX_HOLDERS = 2048;


float MAX_GPU_MEM = 2560.0f;
float TOT_GPU_MEM_USAGE = 0.0f;

float MAX_CPU_MEM = 4096.0f;
float TOT_CPU_MEM_USAGE = 0.0f;

long long ENT_COUNTER = 0;

bool TRACE_ON = false;
//#define DEBUG_MODE 1











//??????????????


// #if defined(USE_SIMD)
// #include "use_simd.h"
// #else
// #include "use_scalar.h"
// #endif

// #include <swizzle/glsl/vector.h>
// #include <swizzle/glsl/matrix.h>
//#include <swizzle/glsl/texture_functions.h>

// typedef swizzle::glsl::vector< float_type, 2 > vec2;
// typedef swizzle::glsl::vector< float_type, 3 > vec3;
// typedef swizzle::glsl::vector< float_type, 4 > vec4;

// static_assert(sizeof(vec2) == sizeof(float_type[2]), "Too big");
// static_assert(sizeof(vec3) == sizeof(float_type[3]), "Too big");
// static_assert(sizeof(vec4) == sizeof(float_type[4]), "Too big");

// typedef swizzle::glsl::matrix< swizzle::glsl::vector, vec4::scalar_type, 2, 2> mat2;
// typedef swizzle::glsl::matrix< swizzle::glsl::vector, vec4::scalar_type, 3, 3> mat3;
// typedef swizzle::glsl::matrix< swizzle::glsl::vector, vec4::scalar_type, 4, 4> mat4;


// //! A really, really simplistic sampler using SDLImage
// struct SDL_Surface;
// class sampler2D : public swizzle::glsl::texture_functions::tag
// {
// public:
//     enum WrapMode
//     {
//         Clamp,
//         Repeat,
//         MirrorRepeat
//     };

//     typedef const vec2& tex_coord_type;

//     sampler2D(const char* path, WrapMode wrapMode);
//     ~sampler2D();
//     vec4 sample(const vec2& coord);

// private:
//     SDL_Surface *m_image;
//     WrapMode m_wrapMode;

//     // do not allow copies to be made
//     sampler2D(const sampler2D&);
//     sampler2D& operator=(const sampler2D&);
// };










// this where the magic happens...
// namespace glsl_sandbox
// {
//     // a nested namespace used when redefining 'inout' and 'out' keywords
//     namespace ref
//     {
// #ifdef CXXSWIZZLE_VECTOR_INOUT_WRAPPER_ENABLED
//         typedef swizzle::detail::vector_inout_wrapper<vec2> vec2;
//         typedef swizzle::detail::vector_inout_wrapper<vec3> vec3;
//         typedef swizzle::detail::vector_inout_wrapper<vec4> vec4;
// #else
//         typedef vec2& vec2;
//         typedef vec3& vec3;
//         typedef vec4& vec4;
// #endif
//         typedef ::float_type& float_type;
//     }

//     namespace in
//     {
//         typedef const ::vec2& vec2;
//         typedef const ::vec3& vec3;
//         typedef const ::vec4& vec4;
//         typedef const ::float_type& float_type;
//     }

//     #include <swizzle/glsl/vector_functions.h>

//     // constants shaders are using
//     float_type time = 1;
//     vec2 mouse(0, 0);
//     vec2 resolution;

//     // constants some shaders from shader toy are using
//     vec2& iResolution = resolution;
//     float_type& iGlobalTime = time;
//     vec2& iMouse = mouse;

//     //sampler2D diffuse("diffuse.png", sampler2D::Repeat);
//     //sampler2D specular("specular.png", sampler2D::Repeat);

//     struct fragment_shader
//     {
//         vec2 gl_FragCoord;
//         vec4 gl_FragColor;
//         void operator()(void);
//     };

//     // change meaning of glsl keywords to match sandbox
//     #define uniform extern
//     #define in in::
//     #define out ref::
//     #define inout ref::
//     #define main fragment_shader::operator()
//     #define float float_type   
//     #define bool bool_type
    
//     #pragma warning(push)
//     #pragma warning(disable: 4244) // disable return implicit conversion warning
//     #pragma warning(disable: 4305) // disable truncation warning
    
//     //#include "shaders/sampler.frag"
//     //#include "shaders/leadlight.frag"
//     //#include "shaders/terrain.frag"
//     //#include "shaders/complex.frag"
//     //#include "shaders/road.frag"
//     //#include "shaders/gears.frag"
//     //#include "shaders/water_turbulence.frag"
//     //#include "shaders/sky.frag"

//     // be a dear a clean up
//     #pragma warning(pop)
//     #undef bool
//     #undef float
//     #undef main
//     #undef in
//     #undef out
//     #undef inout
//     #undef uniform
// }

//??????????????













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
#include <ctime>


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




struct charArr {
	long size;
	char *data;
};

#ifdef WIN32
#include <windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#else
#include <sys/time.h>
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
#include <windows.h>
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


#include "../CommonInterfaces/CommonGraphicsAppInterface.h"
#include "../CommonInterfaces/CommonRenderInterface.h"
#include "../CommonInterfaces/CommonExampleInterface.h"
#include "../CommonInterfaces/CommonGUIHelperInterface.h"
#include "../CommonInterfaces/CommonRigidBodyBase.h"


#include "btBulletDynamicsCommon.h"
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Z 5

#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"





// struct MyGUIHelper : public GUIHelperInterface
// {
// 	MyGUIHelper() {}
// 	virtual ~MyGUIHelper() {}

// 	virtual void createRigidBodyGraphicsObject(btRigidBody* body,const btVector3& color){}

// 	virtual void createCollisionObjectGraphicsObject(btCollisionObject* obj,const btVector3& color) {}

// 	virtual void createCollisionShapeGraphicsObject(btCollisionShape* collisionShape){}

// 	virtual void syncPhysicsToGraphics(const btDiscreteDynamicsWorld* rbWorld){}

// 	virtual void render(const btDiscreteDynamicsWorld* rbWorld) {}

// 	virtual void createPhysicsDebugDrawer( btDiscreteDynamicsWorld* rbWorld){}

// 	virtual int registerGraphicsShape(const float* vertices, int numvertices, const int* indices, int numIndices) { return -1; }

// 	virtual int registerGraphicsInstance(int shapeIndex, const float* position, const float* quaternion, const float* color, const float* scaling) { return -1;}

// 	virtual Common2dCanvasInterface* get2dCanvasInterface()
// 	{
// 		return 0;
// 	}
	
// 	virtual CommonParameterInterface* getParameterInterface()
// 	{
// 		return 0;
// 	}

// 	virtual CommonRenderInterface* getRenderInterface()
// 	{
// 		return 0;
// 	}
	
// 	virtual CommonGraphicsApp* getAppInterface()
// 	{
// 		return 0;
// 	}


// 	virtual void setUpAxis(int axis)
// 	{
// 	}
// 	virtual void resetCamera(float camDist, float pitch, float yaw, float camPosX,float camPosY, float camPosZ)
// 	{
// 	}

// 	virtual void autogenerateGraphicsObjects(btDiscreteDynamicsWorld* rbWorld) 
// 	{
// 	}
    
// 	virtual void drawText3D( const char* txt, float posX, float posZY, float posZ, float size)
// 	{
// 	}
	
// };


//class CommonExampleInterface*    BasicExampleCreateFunc(struct CommonExampleOptions& options);

struct BasicExample : public CommonRigidBodyBase
{
	BasicExample(struct GUIHelperInterface* helper)
		:CommonRigidBodyBase(helper)
	{
		
	}
	
	virtual ~BasicExample(){}
	virtual void initPhysics();
	virtual void renderScene();
	//virtual btDiscreteDynamicsWorld* getDynamicsWorld();
	
	void resetCamera()
	{
		float dist = 41;
		float pitch = 52;
		float yaw = 35;
		float targetPos[3]={0,0.46,0};
		m_guiHelper->resetCamera(dist,pitch,yaw,targetPos[0],targetPos[1],targetPos[2]);
	}
};

// btDiscreteDynamicsWorld* BasicExample::getDynamicsWorld() {
// 	return m_dynamicsWorld;
// }

void BasicExample::initPhysics()
{
	m_guiHelper->setUpAxis(2);

	createEmptyDynamicsWorld();
	
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);

	if (m_dynamicsWorld->getDebugDrawer())
		m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe+btIDebugDraw::DBG_DrawContactPoints);

	m_dynamicsWorld->setGravity(btVector3(0,-10,0));

	///create a few basic rigid bodies
	btBoxShape* groundShape = createBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));
	

	//groundShape->initializePolyhedralFeatures();
//	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),50);
	
	m_collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,-50,0));

	{
		btScalar mass(0.);
		createRigidBody(mass,groundTransform,groundShape, btVector4(0,0,1,1));
	}


	{
		//create a few dynamic rigidbodies
		// Re-using the same collision is better for memory usage and performance

		btBoxShape* colShape = createBoxShape(btVector3(1,1,1));
		

		//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		m_collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);


		for (int k=0;k<ARRAY_SIZE_Y;k++)
		{
			for (int i=0;i<ARRAY_SIZE_X;i++)
			{
				for(int j = 0;j<ARRAY_SIZE_Z;j++)
				{
					startTransform.setOrigin(btVector3(
										btScalar(2.0*i),
										btScalar(20+2.0*k),
										btScalar(2.0*j)));

			
					createRigidBody(mass,startTransform,colShape);
					

				}
			}
		}
	}

	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}


void BasicExample::renderScene()
{
	// m_guiHelper->syncPhysicsToGraphics(m_dynamicsWorld);
	// m_guiHelper->render(m_dynamicsWorld);
	CommonRigidBodyBase::renderScene();
}

// CommonExampleInterface* BasicExampleCreateFunc(CommonExampleOptions& options)
// {
// 	return new BasicExample(options.m_guiHelper);
// }




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

#include "../OpenGLWindow/OpenGL2Include.h"

class btConvexShape;
class btCollisionShape;
class btShapeHull;
class btDiscreteDynamicsWorld;

#include "LinearMath/btAlignedObjectArray.h"
#include "LinearMath/btVector3.h"

#include "BulletCollision/CollisionShapes/btShapeHull.h"


struct ShapeCache
{
	struct Edge { btVector3 n[2];int v[2]; };
	ShapeCache(btConvexShape* s) : m_shapehull(s) {}
	btShapeHull					m_shapehull;
	btAlignedObjectArray<Edge>	m_edges;
};

void OGL_displaylist_register_shape(btCollisionShape * shape);
void OGL_displaylist_clean();

#include "BulletCollision/CollisionShapes/btPolyhedralConvexShape.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btConeShape.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletCollision/CollisionShapes/btTetrahedronShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btUniformScalingShape.h"
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletCollision/CollisionShapes/btConvexPolyhedron.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btDefaultMotionState.h"

///
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "LinearMath/btTransformUtil.h"
#include "LinearMath/btIDebugDraw.h"

#include "Bullet3Common/b3Scalar.h"
#include "../OpenGLWindow/GLInstanceGraphicsShape.h"
//#include "../OpenGLWindow/SimpleOpenGL2Renderer.h"

// @@@@@







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


class GlDrawcallback : public btTriangleCallback
{

public:

	bool	m_wireframe;

	GlDrawcallback()
		:m_wireframe(false)
	{
	}

	virtual void processTriangle(btVector3* triangle,int partId, int triangleIndex)
	{

		(void)triangleIndex;
		(void)partId;


		if (m_wireframe)
		{
			glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			glColor3f(0, 1, 0);
			glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			glColor3f(0, 0, 1);
			glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			glEnd();
		} else
		{
			glBegin(GL_TRIANGLES);
			//glColor3f(1, 1, 1);
			
			
			glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());

			glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			glEnd();
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


		glBegin(GL_TRIANGLES);//LINES);
		glColor3f(1, 0, 0);
		glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
		glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
		glColor3f(0, 1, 0);
		glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
		glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
		glColor3f(0, 0, 1);
		glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
		glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
		glEnd();
	}
};


// ???????????





#include "../CommonInterfaces/CommonRenderInterface.h"
#include "../OpenGLWindow/SimpleCamera.h"
#include "Bullet3Common/b3Vector3.h"


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

	virtual void	resize(int width, int height)
	{
		m_width = width;
		m_height = height;
	}

    virtual void removeAllInstances();
    
    
    virtual void writeSingleInstanceColorToCPU(float* color, int srcIndex);
    virtual void writeSingleInstanceColorToCPU(double* color, int srcIndex);
    virtual void	getCameraViewMatrix(float viewMat[16]) const;
    virtual void	getCameraProjectionMatrix(float projMat[16]) const;

    
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
void	OGLRenderer::getCameraViewMatrix(float viewMat[16]) const
{
    b3Assert(0);
}
void	OGLRenderer::getCameraProjectionMatrix(float projMat[16]) const
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
    glLineWidth(pointDrawSize);
    for (int i=0;i<numIndices;i+=2)
    {
        int index0 = indices[i];
        int index1 = indices[i+1];
        
        b3Vector3 fromColor = b3MakeVector3(color[0],color[1],color[2]);
        b3Vector3 toColor = b3MakeVector3(color[0],color[1],color[2]);
        
        b3Vector3 from= b3MakeVector3(positions[index0*pointStrideInFloats],positions[index0*pointStrideInFloats+1],positions[index0*pointStrideInFloats+2]);
        b3Vector3 to= b3MakeVector3(positions[index1*pointStrideInFloats],positions[index1*pointStrideInFloats+1],positions[index1*pointStrideInFloats+2]);
        
        glBegin(GL_LINES);
        glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
        glVertex3d(from.getX(), from.getY(), from.getZ());
        glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
        glVertex3d(to.getX(), to.getY(), to.getZ());
        glEnd();
        
    }
}

void OGLRenderer::drawLine(const float from[4], const float to[4], const float color[4], float lineWidth)
{
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(color[0],color[1],color[2]);
        glVertex3d(from[0],from[1],from[2]);
        glVertex3d(to[0],to[1],to[2]);
        glEnd();
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





#define BT_LINE_BATCH_SIZE 512


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


	virtual void	drawLine(const btVector3& from1,const btVector3& to1,const btVector3& color1)
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

	virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
	{
        drawLine(PointOnB,PointOnB+normalOnB,color);
	}
     

	virtual void	reportErrorWarning(const char* warningString)
	{
	}

	virtual void	draw3dText(const btVector3& location,const char* textString)
	{
	}

	virtual void	setDebugMode(int debugMode)
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


class MyShapeDrawer;

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











 
