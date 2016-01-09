// f00332_gamenetwork.h
//

#include "f00332_gamenetwork.e"
#define LZZ_INLINE inline
int const GameNetwork::FRAME_SIZE_IN_BYTES;
int const GameNetwork::TOT_BUFFER_SIZE;
GameNetwork::GameNetwork ()
                      {
		
	}
void GameNetwork::init (Singleton * _singleton)
                                         {
		singleton = _singleton;
		ConnectSocket = INVALID_SOCKET;
		isConnected = false;
		
		recvPosInBytes = 0;
		sendPosInBytes = 0;
		
		recvConsumedInBytes = 0;
		sendConsumedInBytes = 0;
		
	}
void GameNetwork::getIntFloatLen (int opCode, int * uintLen, int * intLen, int * floatLen)
                                                                                  {
		
		*uintLen = 0;
		*intLen = 0;
		*floatLen = 0;
		
		
		switch (opCode) {
			case E_NO_TERMINAL:
				
			break;
			case E_NO_KEY_ACTION:
				*uintLen = 1;
				*intLen = 1;
				*floatLen = 2;
			break;
			case E_NO_ADD_ENT:
				*intLen = 1;
				*floatLen = 3;
			break;
			case E_NO_REM_ENT:
				*intLen = 1;
			break;
			case E_NO_DRAG_ENT:
				*intLen = 4;
				*floatLen = 3;
			break;
			default:
				cout << "Unsupported net operation\n";
			break;
			
		}
	}
void GameNetwork::addNetworkAction (int opCode, uint * naUintData, int * naIntData, float * naFloatData)
          {
		char* curDataPtr;
		
		int i;
		int pos;
		
		int uintLen = 0;
		int intLen = 0;
		int floatLen = 0;
		getIntFloatLen(opCode, &uintLen, &intLen, &floatLen);
		
		if (!isConnected) {
			return;
		}
		
		netSendStack.push_back(NetworkAction());
		curDataPtr = &(netSendStack.back().data[0]);
		
		curDataPtr[0] = opCode;
		
		pos = 4;
		
		for (i = 0; i < uintLen; i++) {
			uintPtr[i] = toUintPtr(&(curDataPtr[pos])); pos += 4;
			*(uintPtr[i]) = naUintData[i];
		}
		for (i = 0; i < intLen; i++) {
			intPtr[i] = toIntPtr(&(curDataPtr[pos])); pos += 4;
			*(intPtr[i]) = naIntData[i];
		}
		for (i = 0; i < floatLen; i++) {
			floatPtr[i] = toFloatPtr(&(curDataPtr[pos])); pos += 4;
			*(floatPtr[i]) = naFloatData[i];
		}
		
		
	}
void GameNetwork::addNetworkActionForSend (NetworkAction * na)
          {
		char* curDataPtr;
		int i;
		
		curDataPtr = &(sendbuf[sendPosInBytes]);
		
		for (i = 0; i < NA_SIZE_IN_BYTES; i++) {
			curDataPtr[i] = na->data[i];
		}
		
		sendPosInBytes += NA_SIZE_IN_BYTES;
		
	}
void GameNetwork::applyNetworkActions ()
                                   {
		
		
		int i;
		int pos;
		int opCode;
		char* curDataPtr;
		
		while ( (recvPosInBytes - recvConsumedInBytes) >= NA_SIZE_IN_BYTES) {
			
			
			curDataPtr = &(recvbuf[recvConsumedInBytes]);
			
			pos = 4;
			
			opCode = curDataPtr[0];
			
			int intLen = 0;
			int floatLen = 0;
			int uintLen = 0;
			getIntFloatLen(opCode, &uintLen, &intLen, &floatLen);
			
			for (i = 0; i < uintLen; i++) {
				uintPtr[i] = toUintPtr(&(curDataPtr[pos])); pos += 4;
			}
			for (i = 0; i < intLen; i++) {
				intPtr[i] = toIntPtr(&(curDataPtr[pos])); pos += 4;
			}
			for (i = 0; i < floatLen; i++) {
				floatPtr[i] = toFloatPtr(&(curDataPtr[pos])); pos += 4;
				
				if (i < 4) {
					tempVecs[0].setIndex(i,*(floatPtr[i]));
				}
				else {
					tempVecs[1].setIndex(i-4,*(floatPtr[i]));
				}
			}
			
			switch(opCode) {
				case E_NO_TERMINAL:
					
				break;
				case E_NO_KEY_ACTION:
					singleton->applyKeyAction(
						false,
						*(intPtr[0]),
						*(uintPtr[0]),
						*(floatPtr[0]),
						*(floatPtr[1])
					);
				break;
				case E_NO_ADD_ENT:
					singleton->gem->placeNewEnt(false, *(intPtr[0]), &(tempVecs[0]));
				break;
				case E_NO_REM_ENT:
					singleton->gem->removeEntity(false,*(intPtr[0]));
				break;
				case E_NO_DRAG_ENT:
					singleton->gem->performDrag(
						false,
						*(intPtr[0]),
						*(intPtr[1]),
						*(intPtr[2]),
						*(intPtr[3]),
						&(tempVecs[0])
					);
				break;
				
			}
			
			
			recvConsumedInBytes += NA_SIZE_IN_BYTES;
			
		}
		
	}
int GameNetwork::socketConnect (bool doConnect)
                                          {
		int iResult;
		
		struct addrinfo *result = NULL,
		                *ptr = NULL,
		                hints;
		
		if (doConnect) {
			// Initialize Winsock
			iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
			if (iResult != 0) {
			    printf("WSAStartup failed with error: %d\n", iResult);
			    return 1;
			}

			ZeroMemory( &hints, sizeof(hints) );
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			// Resolve the server address and port
			iResult = getaddrinfo("192.168.1.107", DEFAULT_PORT, &hints, &result); //argv[1] //localhost
			if ( iResult != 0 ) {
			    printf("getaddrinfo failed with error: %d\n", iResult);
			    WSACleanup();
			    return 1;
			}

			// Attempt to connect to an address until one succeeds
			for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

			    // Create a SOCKET for connecting to server
			    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
			        ptr->ai_protocol);
			    if (ConnectSocket == INVALID_SOCKET) {
			        printf("socket failed with error: %ld\n", WSAGetLastError());
			        WSACleanup();
			        return 1;
			    }

			    // Connect to server.
			    iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			    if (iResult == SOCKET_ERROR) {
			        closesocket(ConnectSocket);
			        ConnectSocket = INVALID_SOCKET;
			        continue;
			    }
			    break;
			}

			freeaddrinfo(result);

			if (ConnectSocket == INVALID_SOCKET) {
			    printf("Unable to connect to server!\n");
			    WSACleanup();
			    return 1;
			}
			
			isConnected = true;
			
		}
		else {
			// shutdown the connection since no more data will be sent
			iResult = shutdown(ConnectSocket, SD_SEND);
			if (iResult == SOCKET_ERROR) {
			    printf("shutdown failed with error: %d\n", WSAGetLastError());
			    closesocket(ConnectSocket);
			    WSACleanup();
			    return 1;
			}

			
			

			// cleanup
			closesocket(ConnectSocket);
			WSACleanup();
			
			isConnected = false;
		}
		
		
		

		return 0;
		
	}
void GameNetwork::socketRecv ()
                          {
		
		int iResult;
		
		iResult = recv(
			ConnectSocket, 
			&(recvbuf[recvPosInBytes]), 
			FRAME_SIZE_IN_BYTES, 
			0
		);
		if ( iResult > 0 ) {
			recvPosInBytes += iResult;
			//printf("Bytes received: %d\n", iResult);
		}
		else if ( iResult == 0 ) {
			printf("Connection closed\n");
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
		}
	}
void GameNetwork::socketSend ()
                          {//char* bufToSend, int totSize) {
		//sendbuf, sendPosInBytes - sendConsumedInBytes
	
		int iResult;
		
		//cout << "Sending...\n";
		
		// Send an initial buffer
		iResult = send(
			ConnectSocket,
			&(sendbuf[sendConsumedInBytes]),
			sendPosInBytes - sendConsumedInBytes,
			0
		);
		
		if (iResult >= 0) {
			sendConsumedInBytes += 
				sendPosInBytes - sendConsumedInBytes;
				//iResult;
		}
		
		if (iResult == SOCKET_ERROR) {
		    printf("send failed with error: %d\n", WSAGetLastError());
		    closesocket(ConnectSocket);
		    WSACleanup();
		}

		//printf("Bytes Sent: %ld\n", iResult);
		
		
	}
void GameNetwork::checkBufferLengthSend ()
                                     {
		
		int roundedPos;
		int basePos;
		int i;
		
		if (sendConsumedInBytes > (TOT_BUFFER_SIZE/2)) {
			roundedPos = (sendConsumedInBytes/NA_SIZE_IN_BYTES)*NA_SIZE_IN_BYTES-NA_SIZE_IN_BYTES;
			
			basePos = sendConsumedInBytes-roundedPos;
			
			for (i = 0; i <= (sendPosInBytes - sendConsumedInBytes); i++) {
				sendbuf[basePos + i] = sendbuf[sendConsumedInBytes+i];
			}
			
			sendConsumedInBytes -= roundedPos;
			sendPosInBytes -= roundedPos;
		}
		
	}
void GameNetwork::checkBufferLengthRecv ()
                                     {
		
		int roundedPos;
		int basePos;
		int i;
		
		
		if (recvConsumedInBytes > (TOT_BUFFER_SIZE/2)) {
			roundedPos = (recvConsumedInBytes/NA_SIZE_IN_BYTES)*NA_SIZE_IN_BYTES-NA_SIZE_IN_BYTES;
			
			basePos = recvConsumedInBytes-roundedPos;
			
			for (i = 0; i <= (recvPosInBytes - recvConsumedInBytes); i++) {
				recvbuf[basePos + i] = recvbuf[recvConsumedInBytes+i];
			}
			
			recvConsumedInBytes -= roundedPos;
			recvPosInBytes -= roundedPos;
		}
		
	}
void GameNetwork::flushNetworkActions ()
                                   {
		
		int i;
		
		// if nothing to send, add in an empty action
		if (
			((sendPosInBytes - sendConsumedInBytes) == 0) &&
			(netSendStack.size() == 0)
		) {
			addNetworkAction(E_NO_TERMINAL, NULL, NULL, NULL);	
		}
		
		//cout << "netSendStack.size() " << netSendStack.size() << "\n";
		
		for (i = 0; i < netSendStack.size(); i++) {
			addNetworkActionForSend(&(netSendStack[i]));
		}
		
		netSendStack.clear();
	}
void GameNetwork::updateSend ()
                          {
		checkBufferLengthSend();
		socketSend();
	}
void GameNetwork::updateRecv ()
                          {
		checkBufferLengthRecv();
		socketRecv();
	}
#undef LZZ_INLINE
 
