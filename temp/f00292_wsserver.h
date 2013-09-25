// f00292_wsserver.h
//

#include "f00292_wsserver.e"
#define LZZ_INLINE inline
WebSocketServer::WebSocketServer ()
  : _helpRequested (false)
        {
		//recMessage = NULL;
		dataReady = false;
		isWorking = false;
		isJSON = false;
		//recBufferLength = 0;
		
		MAX_FRAME_SIZE = 16777216; //16 MB
		recBuffer.data = new char[MAX_FRAME_SIZE];
		okBuffer.data = "{\"cc\":\"REC__OK\"}";
		okBuffer.size = 16;
		
	}
WebSocketServer::~ WebSocketServer ()
        {
		if (recBuffer.data != NULL) {
			delete[] recBuffer.data;
		}
	}
void WebSocketServer::initialize (Application & self)
        {
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}
void WebSocketServer::uninitialize ()
        {
		ServerApplication::uninitialize();
	}
void WebSocketServer::defineOptions (OptionSet & options)
        {
		ServerApplication::defineOptions(options);
		
		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false));
	}
void WebSocketServer::handleOption (std::string const & name, std::string const & value)
        {
		ServerApplication::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
	}
void WebSocketServer::displayHelp ()
        {
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A sample HTTP server supporting the WebSocket protocol.");
		helpFormatter.format(std::cout);
	}
int WebSocketServer::main (std::vector <std::string> const & args)
        {





		// get parameters from configuration file
		unsigned short port = (unsigned short) config().getInt("WebSocketServer.port", 9980);
		
		// set-up a server socket
		ServerSocket svs(port);
		// set-up a HTTPServer instance
		HTTPServer srv(new RequestHandlerFactory(this), svs, new HTTPServerParams);
		// start the HTTPServer
		srv.start();
		// wait for CTRL-C or kill
		//waitForTerminationRequest();
		// Stop the HTTPServer

		glutMainLoop();
		
		srv.stop();


		return Application::EXIT_OK;

	}
#undef LZZ_INLINE
 
