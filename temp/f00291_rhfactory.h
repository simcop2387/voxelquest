// f00291_rhfactory.h
//

#include "f00291_rhfactory.e"
#define LZZ_INLINE inline
RequestHandlerFactory::RequestHandlerFactory (WebSocketServer * _ws_ptr)
                                                        {
		ws_ptr = _ws_ptr;
	}
HTTPRequestHandler * RequestHandlerFactory::createRequestHandler (HTTPServerRequest const & request)
        {
		Application& app = Application::instance();
		app.logger().information("Request from " 
			+ request.clientAddress().toString()
			+ ": "
			+ request.getMethod()
			+ " "
			+ request.getURI()
			+ " "
			+ request.getVersion());
			
		for (HTTPServerRequest::ConstIterator it = request.begin(); it != request.end(); ++it)
		{
			app.logger().information(it->first + ": " + it->second);
		}

		doTrace("REQUEST HANDLER CREATED");

		return new WebSocketRequestHandler(ws_ptr);
	}
#undef LZZ_INLINE
 
