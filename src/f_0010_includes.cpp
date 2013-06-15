/*************************************************************************
 * 
 * 
 * Copyright [2000] - [2012] Gavan Woolery 
 * All Rights Reserved.
 * 
 * NOTICE:  All information contained herein is, and remains
 * the sole property of Gavan Woolery. Dissemination of this information
 * or reproduction of this material is strictly forbidden unless prior
 * written permission is obtained from Gavan Woolery.
 *
 *
 *************************************************************************/

//./mergefiles.sh;cmake -DCMAKE_BUILD_TYPE=Release CMakeLists.txt;make clean;make;./voxelquest

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include <sstream>
#include <math.h>
#include <vector>
#include <assert.h>
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>

#if ( (defined(__MACH__)) && (defined(__APPLE__)) )   
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glext.h>
#endif

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Format.h"



 using Poco::Net::ServerSocket;
 using Poco::Net::WebSocket;
 using Poco::Net::WebSocketException;
 using Poco::Net::HTTPRequestHandler;
 using Poco::Net::HTTPRequestHandlerFactory;
 using Poco::Net::HTTPServer;
 using Poco::Net::HTTPServerRequest;
 using Poco::Net::HTTPResponse;
 using Poco::Net::HTTPServerResponse;
 using Poco::Net::HTTPServerParams;
 using Poco::Timestamp;
 using Poco::ThreadPool;
 using Poco::Util::ServerApplication;
 using Poco::Util::Application;
 using Poco::Util::Option;
 using Poco::Util::OptionSet;
 using Poco::Util::HelpFormatter;


/*
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::tcp;
*/

using namespace std;


#define _USE_MATH_DEFINES

typedef unsigned int uint;
