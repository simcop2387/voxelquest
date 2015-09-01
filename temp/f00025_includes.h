
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif


int RUN_COUNT;


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


const static float OFFSET_X[4] = {-0.5,0.5,0.5,-0.5};
const static float OFFSET_Y[4] = {-0.5,-0.5,0.5,0.5};

const static int MAX_SPLASHES = 8;
const static int MAX_EXPLODES = 8;

//const static bool DO_CONNECT = true;
const static bool DO_SHADER_DUMP = true;

const static int DEF_WIN_W = 1920;
const static int DEF_WIN_H = 1080;
const static int DEF_SCALE_FACTOR = 1;
const static int RENDER_SCALE_FACTOR = 1;


const static float SPHEREMAP_SCALE_FACTOR = 1.0f;
const static bool USE_SPHERE_MAP = true;

const static float TIME_DELTA = 1.0f/60.0f;
const static int MAX_THREADS = 8;

// #define DEBUG_BOUNDS 1

const static float explodeRad = 5.0f;

float RAND_COUNTER = 0.0f;
float RAND_COUNTER2 = 0.0f;

typedef unsigned int PRIM_FORMAT;

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


#if defined(_DEBUG)
#include <crtdbg.h>
#endif

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


 
