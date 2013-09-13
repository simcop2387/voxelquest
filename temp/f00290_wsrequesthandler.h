// f00290_wsrequesthandler.h
//

#include "f00290_wsrequesthandler.e"
#define LZZ_INLINE inline
WebSocketRequestHandler::WebSocketRequestHandler (WebSocketServer * _ws_ptr)
                                                          {

		ws_ptr = _ws_ptr;
	}
WebSocketRequestHandler::~ WebSocketRequestHandler ()
                                   {
		
		
		
	}
void WebSocketRequestHandler::handleRequest (HTTPServerRequest & request, HTTPServerResponse & response)
        {

		int flags;
		int n;

		//JSONValue *value;

		if (ws_ptr->isWorking) {
			// drop the request, client should only send one request
			// at a time until it receives a response message
			return;
		}


		ws_ptr->isWorking = true;
		ws_ptr->dataReady = false;
		if (ws_ptr->recMessage != NULL) {
			delete ws_ptr->recMessage;
			ws_ptr->recMessage = NULL;
		}

		Application& app = Application::instance();

		
		
		try
		{


			WebSocket ws(request, response);
			app.logger().information("WebSocket connection established.");
			//char buffer[1024];
			
			
			
			do
			{
				n = ws.receiveFrame(ws_ptr->recBuffer, ws_ptr->MAX_FRAME_SIZE, flags);
				app.logger().information(Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)));
				

				if (n > 0) {

					ws_ptr->recBuffer[n+1] = '\0';
					ws_ptr->recBufferLength = n; 

					if (ws_ptr->recBuffer[0] == '{') {
						ws_ptr->isJSON = true;

						ws_ptr->recMessage = JSON::Parse(ws_ptr->recBuffer);

						if (ws_ptr->recMessage == NULL) {
							doTrace("Invalid JSON\n\n");
							ws_ptr->isWorking = false;
							
						}
						else {
							doTrace("");
							doTrace("Valid JSON");
							doTrace("");
							//ws_ptr->isWorking = false;
							ws_ptr->dataReady = true;
						}

					}
					else {
						ws_ptr->isJSON = false;
						ws_ptr->dataReady = true;

					}

					
					

				}
				//01234567890123
				//{"cc":"SENDING"
				
				/*ws_ptr->recBuffer[7] = 'R';
				ws_ptr->recBuffer[8] = 'E';
				ws_ptr->recBuffer[9] = 'C';
				ws_ptr->recBuffer[10] = '_';
				ws_ptr->recBuffer[11] = '_';
				ws_ptr->recBuffer[12] = 'O';
				ws_ptr->recBuffer[13] = 'K';
				*/
				//ws.sendFrame(ws_ptr->recBuffer, n, flags);
				//1234567890123456
				//{"cc":"REC__OK"}
				ws.sendFrame(ws_ptr->okBuffer,16,flags);

			}
			while ( (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE) && PROG_ACTIVE );
			app.logger().information("WebSocket connection closed.");

			

			


		}
		catch (WebSocketException& exc)
		{

			app.logger().log(exc);
			switch (exc.code())
			{
			case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
				response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
				// fallthrough
			case WebSocket::WS_ERR_NO_HANDSHAKE:
			case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
			case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
				response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
				response.setContentLength(0);
				response.send();
				break;
			}
		}


		

		


		

	}
#undef LZZ_INLINE
 
