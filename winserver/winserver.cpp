
#include "stdafx.h"
#include "winserver.h"

#include <winsock2.h>
#include <windows.h>
//#include <list>
#include <string>

#pragma comment(lib,"ws2_32.lib")

#define IDC_EDIT_IN		101
#define IDC_EDIT_OUT		102
#define IDC_MAIN_BUTTON		103
#define WM_SOCKET		104

int nPort=27015;

HWND hEditIn=NULL;
HWND hEditOut=NULL;
//char szHistory[10000];
sockaddr sockAddrClient;



const static int NA_SIZE_IN_BYTES = 32;
//const static int MAX_FRAMES = 256;
const static int MAX_CLIENTS = 3;
const static int FRAME_SIZE_IN_BYTES = 256;
const static int TOT_BUFFER_SIZE = FRAME_SIZE_IN_BYTES*256;

enum E_NET_OPS {
	E_NO_TERMINAL,
	E_NO_ADD_ENT,
	E_NO_LAUNCH_BULLET,
	E_NO_MOVE_ENT,
	E_NO_JUMP,
	E_NO_LENGTH	
};






struct ClientData {
	char recvbuf[TOT_BUFFER_SIZE];
	char sendbuf[TOT_BUFFER_SIZE];
	
	int recvPosInBytes;
	int recvConsumedInBytes;
	
	int sendPosInBytes;
	int sendConsumedInBytes;
	
	bool hasRead;
};

ClientData clientData[MAX_CLIENTS];

int numClients=0;
SOCKET Socket[MAX_CLIENTS];
SOCKET ServerSocket=NULL;

LRESULT CALLBACK WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass,sizeof(WNDCLASSEX));
	wClass.cbClsExtra=NULL;
	wClass.cbSize=sizeof(WNDCLASSEX);
	wClass.cbWndExtra=NULL;
	wClass.hbrBackground=(HBRUSH)COLOR_WINDOW;
	wClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wClass.hIcon=NULL;
	wClass.hIconSm=NULL;
	wClass.hInstance=hInst;
	wClass.lpfnWndProc=(WNDPROC)WinProc;
	wClass.lpszClassName="Window Class";
	wClass.lpszMenuName=NULL;
	wClass.style=CS_HREDRAW|CS_VREDRAW;

	if(!RegisterClassEx(&wClass))
	{
		int nResult=GetLastError();
		MessageBox(NULL,
			"Window class creation failed\r\nError code:",
			"Window Class Failed",
			MB_ICONERROR);
	}

	HWND hWnd=CreateWindowEx(NULL,
			"Window Class",
			"Winsock Async Server",
			WS_OVERLAPPEDWINDOW,
			200,
			200,
			640,
			480,
			NULL,
			NULL,
			hInst,
			NULL);

	if(!hWnd)
	{
		int nResult=GetLastError();

		MessageBox(NULL,
			"Window creation failed\r\nError code:",
			"Window Creation Failed",
			MB_ICONERROR);
	}

    ShowWindow(hWnd,nShowCmd);

	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));

	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void checkBufferLengths() {
	
	int roundedPos;
	int basePos;
	int i;
	int n;
	
	
	
	for (n = 0; n < numClients; n++) {
		
		if (
			clientData[n].sendConsumedInBytes > (TOT_BUFFER_SIZE/2)
		) {
			roundedPos = (clientData[n].sendConsumedInBytes/NA_SIZE_IN_BYTES)*NA_SIZE_IN_BYTES-NA_SIZE_IN_BYTES;
			
			basePos = clientData[n].sendConsumedInBytes-roundedPos;
			
			for (i = 0; i <= (clientData[n].sendPosInBytes - clientData[n].sendConsumedInBytes); i++) {
				clientData[n].sendbuf[basePos + i] = clientData[n].sendbuf[clientData[n].sendConsumedInBytes+i];
			}
			
			clientData[n].sendConsumedInBytes -= roundedPos;
			clientData[n].sendPosInBytes -= roundedPos;
		}
		
		if (clientData[n].recvConsumedInBytes > (TOT_BUFFER_SIZE/2)) {
			roundedPos = (clientData[n].recvConsumedInBytes/NA_SIZE_IN_BYTES)*NA_SIZE_IN_BYTES-NA_SIZE_IN_BYTES;
			
			basePos = clientData[n].recvConsumedInBytes-roundedPos;
			
			for (i = 0; i <= (clientData[n].recvPosInBytes - clientData[n].recvConsumedInBytes); i++) {
				clientData[n].recvbuf[basePos + i] = clientData[n].recvbuf[clientData[n].recvConsumedInBytes+i];
			}
			
			clientData[n].recvConsumedInBytes -= roundedPos;
			clientData[n].recvPosInBytes -= roundedPos;
		}
		
	}
	
}

LRESULT CALLBACK WinProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	
	int n;
	int i;
	int totSize;
	
	int dataRecvSize;
	int dataSendSize;
	//bool allRead;
	//bool enoughToSend;
	
	switch(msg)
    {
		case WM_COMMAND:
			switch(LOWORD(wParam))
            {
				case IDC_MAIN_BUTTON:
				{
					// char szBuffer[1024];
					// ZeroMemory(szBuffer,sizeof(szBuffer));

					// SendMessage(hEditOut,
					// 	WM_GETTEXT,
					// 	sizeof(szBuffer),
					// 	reinterpret_cast<LPARAM>(szBuffer));
					// for(n = 0; n < numClients; n++)
					// {
					// 	send(Socket[n],szBuffer,strlen(szBuffer),0);
					// }

					// SendMessage(hEditOut,WM_SETTEXT,NULL,(LPARAM)"");
				}
				break;
			}
			break;
		case WM_CREATE: 
		{
			
			
			
			for (n = 0; n < MAX_CLIENTS; n++) {
				clientData[n].recvPosInBytes = 0;
				clientData[n].recvConsumedInBytes = 0;
				
				
				clientData[n].sendPosInBytes = 0;
				clientData[n].sendConsumedInBytes = 0;
				
				
				clientData[n].hasRead = false;
			}
			

			// Create incoming message box
			hEditIn=CreateWindowEx(WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD|WS_VISIBLE|ES_MULTILINE|
				ES_AUTOVSCROLL|ES_AUTOHSCROLL,
				50,
				120,
				400,
				200,
				hWnd,
				(HMENU)IDC_EDIT_IN,
				GetModuleHandle(NULL),
				NULL);
			if(!hEditIn)
			{
				MessageBox(hWnd,
					"Could not create incoming edit box.",
					"Error",
					MB_OK|MB_ICONERROR);
			}
			HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEditIn,
					WM_SETFONT,
					(WPARAM)hfDefault,
					MAKELPARAM(FALSE,0));
			SendMessage(hEditIn,
					WM_SETTEXT,
					NULL,
					(LPARAM)"Waiting for client to connect...");

			// Create outgoing message box
			hEditOut=CreateWindowEx(WS_EX_CLIENTEDGE,
						"EDIT",
						"",
						WS_CHILD|WS_VISIBLE|ES_MULTILINE|
						ES_AUTOVSCROLL|ES_AUTOHSCROLL,
						50,
						50,
						400,
						60,
						hWnd,
						(HMENU)IDC_EDIT_IN,
						GetModuleHandle(NULL),
						NULL);
			if(!hEditOut)
			{
				MessageBox(hWnd,
					"Could not create outgoing edit box.",
					"Error",
					MB_OK|MB_ICONERROR);
			}

			SendMessage(hEditOut,
					WM_SETFONT,
					(WPARAM)hfDefault,
					MAKELPARAM(FALSE,0));
			SendMessage(hEditOut,
					WM_SETTEXT,
					NULL,
					(LPARAM)"Type message here...");

			// Create a push button
			HWND hWndButton=CreateWindow( 
					    "BUTTON",
						"Send",
						WS_TABSTOP|WS_VISIBLE|
						WS_CHILD|BS_DEFPUSHBUTTON,
						50,
						330,
						75,
						23,
						hWnd,
						(HMENU)IDC_MAIN_BUTTON,
						GetModuleHandle(NULL),
						NULL);
			
			SendMessage(hWndButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));

			WSADATA WsaDat;
			int nResult=WSAStartup(MAKEWORD(2,2),&WsaDat);
			if(nResult!=0)
			{
				MessageBox(hWnd,
					"Winsock initialization failed",
					"Critical Error",
					MB_ICONERROR);
				SendMessage(hWnd,WM_DESTROY,NULL,NULL);
				break;
			}

			ServerSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			if(ServerSocket==INVALID_SOCKET)
			{
				MessageBox(hWnd,
					"Socket creation failed",
					"Critical Error",
					MB_ICONERROR);
				SendMessage(hWnd,WM_DESTROY,NULL,NULL);
				break;
			}

			SOCKADDR_IN SockAddr;
			SockAddr.sin_port=htons(nPort);
			SockAddr.sin_family=AF_INET;
			SockAddr.sin_addr.s_addr=htonl(INADDR_ANY);

			if(bind(ServerSocket,(LPSOCKADDR)&SockAddr,sizeof(SockAddr))==SOCKET_ERROR)
			{
				MessageBox(hWnd,"Unable to bind socket","Error",MB_OK);
				SendMessage(hWnd,WM_DESTROY,NULL,NULL);
				break;
			}

			nResult=WSAAsyncSelect(ServerSocket,
					hWnd,
					WM_SOCKET,
					(FD_CLOSE|FD_ACCEPT|FD_READ));
			if(nResult)
			{
				MessageBox(hWnd,
					"WSAAsyncSelect failed",
					"Critical Error",
					MB_ICONERROR);
				SendMessage(hWnd,WM_DESTROY,NULL,NULL);
				break;
			}

			if(listen(ServerSocket,SOMAXCONN)==SOCKET_ERROR)
			{
				MessageBox(hWnd,
					"Unable to listen!",
					"Error",
					MB_OK);
				SendMessage(hWnd,WM_DESTROY,NULL,NULL);
				break;
			}
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			shutdown(ServerSocket,SD_BOTH);
			closesocket(ServerSocket);
			WSACleanup();
			return 0;
		}
		break;

		case WM_SOCKET:
		{
			switch(WSAGETSELECTEVENT(lParam))
			{
				case FD_READ:
				{
					
					checkBufferLengths();
					
					
					// read all incoming data
					
					for (n = 0; n < MAX_CLIENTS; n++) {
						clientData[n].hasRead = false;
					}
					
					for(n = 0; n < numClients; n++) {
						
						dataRecvSize = recv(
							Socket[n],
							(char*)( &(clientData[n].recvbuf[clientData[n].recvPosInBytes]) ),
							FRAME_SIZE_IN_BYTES,
							0
						);
						
						
						if (dataRecvSize < 0) { //(dataRecvSize == -1);
							
						}
						else {
							clientData[n].recvPosInBytes += dataRecvSize;	
							clientData[n].hasRead = true;
						}
						
						
						// zip up all client data into one stream
						if (
							(clientData[n].recvPosInBytes - clientData[n].recvConsumedInBytes) >= NA_SIZE_IN_BYTES
						) {
							totSize = (
								(
									clientData[n].recvPosInBytes -
									clientData[n].recvConsumedInBytes
								)/NA_SIZE_IN_BYTES
							)*NA_SIZE_IN_BYTES;
							
							for (i = 0; i < numClients; i++) {
								memcpy(
									&(clientData[i].sendbuf[ clientData[i].sendPosInBytes]),
									&(clientData[n].recvbuf[
										clientData[n].recvConsumedInBytes	
									]),
									totSize
								);
								
								clientData[i].sendPosInBytes += totSize;
							}
							
							
							
							clientData[n].recvConsumedInBytes += totSize;
							
						}
						
					}
					
					
					
					for(n = 0; n < numClients; n++) {
						if (clientData[n].hasRead) {
							
							if (
								(clientData[n].sendPosInBytes - clientData[n].sendConsumedInBytes) >= NA_SIZE_IN_BYTES	
							) {
								
								dataSendSize = send(
									Socket[n],
									(char*)(
										&(clientData[n].sendbuf[
											clientData[n].sendConsumedInBytes	
										])
									),
									(clientData[n].sendPosInBytes - clientData[n].sendConsumedInBytes),
									0
								);
								
								if (dataSendSize >= 0) {
									clientData[n].sendConsumedInBytes += 
										(clientData[n].sendPosInBytes - clientData[n].sendConsumedInBytes);
										//dataSendSize;
								}
								
							}
							
							
							
						}
					}
					
					
					
					
					
					
				}
				break;

				case FD_CLOSE:
				{
					// MessageBox(hWnd,
					// 	"Client closed connection",
					// 	"Connection closed!",
					// 	MB_ICONINFORMATION|MB_OK);
				}
				break;

				case FD_ACCEPT:
				{
					if(numClients<MAX_CLIENTS)
					{
						int size=sizeof(sockaddr);
						Socket[numClients]=accept(wParam,&sockAddrClient,&size);                
						if (Socket[numClients]==INVALID_SOCKET)
						{
							int nret = WSAGetLastError();
							WSACleanup();
						}
						SendMessage(hEditIn,
							WM_SETTEXT,
							NULL,
							(LPARAM)"Client connected!");
						}
						numClients++;
					}
				break;
    			}   
			}
		}
    
    return DefWindowProc(hWnd,msg,wParam,lParam);
}
