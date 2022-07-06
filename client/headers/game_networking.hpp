#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
//#include <windows.h>
//#include <stdio.h>

#include <string>
#include <iostream>
#include <limits>
#include <ctime>
#include <codecvt>
#include <locale>
#include <functional>
#include <type_traits>

#include "command_parser.hpp"

#define DEFAULT_PORT 2000
#define DEFAULT_BUFLEN 2048

HANDLE receiveThread;
SOCKET CURRENT_SOCKET;

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;

std::string to_string(std::wstring wstr)
{
    return strconverter.to_bytes(wstr);
}

std::wstring to_wstring(std::string str)
{
    return strconverter.from_bytes(str);
}

std::wstring getWideStringFromString(std::string string){	
	int sz = MultiByteToWideChar(CP_UTF8, 0, &string[0], -1, NULL, 0);
	std::wstring res(sz, 0);
	MultiByteToWideChar(CP_UTF8, 0, &string[0], -1, &res[0], sz);
	return res;
}

std::string getStringFromWideString(std::wstring wstring){
	int sz = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], -1, 0, 0, 0, 0);
	std::string res(sz, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstring[0], -1, &res[0], sz, 0, 0);
	return res;
}


DWORD WINAPI receiveMessagesProc(LPVOID lpParam){

      commandParser *parser = (commandParser*)lpParam;

      // pass in the socket for this client to receive on  
      SOCKET clientSock = CURRENT_SOCKET;
      char recvbuf[DEFAULT_BUFLEN];
      int recvbuflen = DEFAULT_BUFLEN;
      int rtnVal;
      
      while(1){
            // clear the recv buffer 
            ZeroMemory(recvbuf, recvbuflen);
            rtnVal = recv(clientSock, (char *)recvbuf, recvbuflen, 0);

            if(rtnVal > 0){	
                  // get curr time
                  std::time_t timeNow = std::time(NULL);
                  std::tm* ptm = std::localtime(&timeNow);
      
                  WCHAR buff[64];
                  std::wcsftime(buff, 64, L"%m-%d-%Y_%H:%M:%S", ptm);
                  std::wstring currTime = std::wstring(buff);
                  
                  std::string multiByteMsg = std::string(recvbuf);
                  //std::cout << multiByteMsg << "\n";
                  //std::wstring res = getWideStringFromString(multiByteMsg);
                  
                  parser->parse(multiByteMsg);
            }else if(rtnVal == 0){
                  printf("connection closed.\n");
                  exit(1);
            }else{
                  printf("recv failed: %d\n", WSAGetLastError());
                  exit(1);
            }
      }
      
      return 0;
}


class gameNetworking {
      public: 
            // variables for networking
            WSADATA wsaData;
            int iResult;

            std::string username;
            std::string identifier;

            SOCKET connectSocket;

            commandParser *parser;

            gameNetworking(commandParser *_parser) {
                  parser = _parser;
            }
            
            void setPlayerData(std::string _username, std::string _identifier) {
                  username = _username;
                  identifier = _identifier;
            }

            bool connectServer(std::string _init_command) { // try to connect to server		

                  // get the ip addr (need to validate it!)		
                  std::string ip = std::string("127.0.0.1");
                  const char* theIp = ip.c_str();

                  iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
                  if(iResult != 0){
                        printf("WSAStartup failed: %d\n", iResult);
                        return 0;
                  }
                  
                  // start socket stuff
                  connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                  if(connectSocket == INVALID_SOCKET){
                        printf("socket() failed\n");
                        WSACleanup();
                        return 0;
                  }
                  
                  struct sockaddr_in servAddr;
                  ZeroMemory(&servAddr, sizeof(servAddr));
                  servAddr.sin_family = AF_INET;
                  
                  int size = sizeof(servAddr);
                  iResult = WSAStringToAddressA((LPSTR)theIp, AF_INET, NULL, (struct sockaddr *)&servAddr, &size);
                  if(iResult != 0){
                        printf("getaddrinfo failed: %d\n", iResult);
                        WSACleanup();
                        return 0;
                  } else {
                        servAddr.sin_port = htons(DEFAULT_PORT);
                        
                        // CONNECT TO THE SERVER
                        iResult = connect(connectSocket, (struct sockaddr *)&servAddr, sizeof(servAddr));
                        if(iResult == SOCKET_ERROR){
                              closesocket(connectSocket);
                              connectSocket = INVALID_SOCKET;
                        }else{
                              // create thread to receive messages 
                              CURRENT_SOCKET = connectSocket;
                              
                              // refer to the window handler of the chat page (the current hwnd is the connection page)

                              receiveThread = CreateThread(NULL, 0, receiveMessagesProc, (void*)parser, 0, 0);
                              send(connectSocket, _init_command.c_str(), strlen(_init_command.c_str()), 0);	
                              return 1;
                        }
                  }

                  return 0;
            }

            void disconnect() {
                  closesocket(connectSocket);
                  DWORD exitCode;
                  if(GetExitCodeThread(receiveThread, &exitCode) != 0){
                        TerminateThread(receiveThread, exitCode);
                  }

                  iResult = shutdown(connectSocket, SD_SEND);
                  if(iResult == SOCKET_ERROR){
                        printf("shutdown failed: %d\n", WSAGetLastError());
                        closesocket(connectSocket);
                        WSACleanup();
                        return;
                  }
                  closesocket(connectSocket);
            }

            void close() {
                  DWORD exitCode;
                  if(GetExitCodeThread(receiveThread, &exitCode) != 0){
                        TerminateThread(receiveThread, exitCode);
                  }
                  
                  // gotta close socket here 
                  iResult = shutdown(connectSocket, SD_SEND);
                  if(iResult == SOCKET_ERROR){
                        printf("shutdown failed: %d\n", WSAGetLastError());
                        closesocket(connectSocket);
                        WSACleanup();
                        return;
                  }
                  closesocket(connectSocket);
                  WSACleanup();
                  PostQuitMessage(0);
            }

            void sendMsg(std::string text) {
                  send(connectSocket, text.c_str(), strlen(text.c_str()), 0);	
            }

};