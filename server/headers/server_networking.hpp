#pragma once

#define _WIN32_WINNT 0x501 

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>

#define DEFAULT_PORT 2000
#define DEFAULT_BUFLEN 2048

class serverNetworking {
      private:
            virtual void processNewPlayer(SOCKET clientSocket) {};
            virtual void parseCommand(SOCKET currSocketFd, std::string text) {};

            SOCKET constructSocket(){
                  SOCKET sockRtnVal = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                  if(sockRtnVal == INVALID_SOCKET){
                        printf("socket() failed\n");
                        WSACleanup();
                        exit(1);
                  }
                  return sockRtnVal;
            }

            // checks for SOCKET_ERROR 
            // this error can come up when trying to bind, listen, send 
            void socketErrorCheck(int returnValue, SOCKET socketToClose, const char* action){
                  const char *actionAttempted = action;
                  if(returnValue == SOCKET_ERROR){
                        printf("socket error. %s failed with error: %d\n", actionAttempted, WSAGetLastError());
                  }else{
                        // everything's fine, probably...
                        return;
                  }
                  closesocket(socketToClose);
                  WSACleanup();
                  exit(1);
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

            WSADATA wsaData;
            
            int iResult;
            int sendResult;
            char recvbuf[DEFAULT_BUFLEN] = {0};
            int recvbuflen = DEFAULT_BUFLEN;
            
            SOCKET serverSocket = INVALID_SOCKET;
            SOCKET clientSocket = INVALID_SOCKET;
            
            struct sockaddr_in servAddr;
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen;
            
            // for select() 
            fd_set activeFdSet;
            fd_set readFdSet;
            
            // keep array of sockets 
            std::vector<SOCKET> socketsArray;

            std::vector<SOCKET> newSocketArray;

      public:
            serverNetworking() {
                  iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
                  if(iResult != 0){
                        printf("WSAStartup failed: %d\n", iResult);
                        return;
                  }
	
                  // create socket for server
                  serverSocket = constructSocket();
                  //assert(serverSocket != 1);
                  
                  // set up server address 
                  ZeroMemory(&servAddr, sizeof(servAddr));
                  servAddr.sin_family = AF_INET;
                  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
                  servAddr.sin_port = htons(DEFAULT_PORT);
                  
                  // bind the server socket to the address 
                  iResult = bind(serverSocket, (struct sockaddr *)&servAddr, sizeof(servAddr));
                  socketErrorCheck(iResult, serverSocket, "bind");

                  // listen on the socket for connections
                  iResult = listen(serverSocket, SOMAXCONN);
                  socketErrorCheck(iResult, serverSocket, "listen");

                  // initialize active socket set (and add the server socket to the set)
                  FD_ZERO(&activeFdSet);
                  FD_SET(serverSocket, &activeFdSet);
                  socketsArray.push_back(serverSocket);
                  
                  // keep new empty vector in case we need to update socketsArray (i.e. client disconnects)
                  newSocketArray = socketsArray;
            }

            ~serverNetworking() {
                  WSACleanup();
            }

            void read() {
                  while(true) {
                        // block until input comes in 
                        readFdSet = activeFdSet;
                        iResult = select(FD_SETSIZE, &readFdSet, NULL, NULL, NULL);
                        socketErrorCheck(iResult, serverSocket, "select");
                        
                        // service all the sockets that have input! 
                        for(int i = 0; i < (int)socketsArray.size(); i++){
                        
                              // grab the socket fd at this index 
                              SOCKET currSocketFd = socketsArray.at(i);
                              
                              if(FD_ISSET(currSocketFd, &readFdSet)){
                                    // if active socket, check if it's a new connection or one with data 	
                                    if(currSocketFd == serverSocket){
                                          // this is a new connection 
                                          clientAddrLen = sizeof(clientAddr);
                                          clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
                                          if(clientSocket == INVALID_SOCKET){
                                                printf("accept failed with error: %d\n", WSAGetLastError());
                                                closesocket(clientSocket);
                                                WSACleanup();
                                                return;
                                          }
                                          // add them to the set
                                          FD_SET(clientSocket, &activeFdSet);
                                          
                                          // update the sockets array 
                                          newSocketArray.push_back(clientSocket); // HERE WE REGISTER NEW CONNECTION
                                          processNewPlayer(clientSocket);
                                    } else { // handle communication between already established client
                                          ZeroMemory(recvbuf, recvbuflen); // clear the recv buffer first 
                                          
                                          // get the new data 
                                          iResult = recv(currSocketFd, recvbuf, recvbuflen, 0);
                                          
                                          if(iResult > 0){			
                                                std::string user_message = std::string(recvbuf);

                                                parseCommand(currSocketFd, user_message);
                                                std::cout << "[SERVER]: Received message: " << user_message << "\n";

                                                ZeroMemory(recvbuf, recvbuflen);
                                          } else if (iResult == 0){
                                                // note that iResult needs to be 0 in order for a connection to close. 
                                                          
                                                // if you reach this point, connection needs to be shutdown 
                                                iResult = shutdown(currSocketFd, SD_SEND);
                                                socketErrorCheck(iResult, currSocketFd, "shutdown");
                                                
                                                closesocket(currSocketFd);
                                                FD_CLR(currSocketFd, &activeFdSet);

                                                
                                                std::vector<SOCKET> tempArr; 
                                                
                                                // add the server socket to the front!
                                                tempArr.push_back(serverSocket);
                                                
                                                for(auto sock: newSocketArray){
                                                      if((SOCKET)sock != currSocketFd && (SOCKET)sock != serverSocket){
                                                            tempArr.push_back((SOCKET)sock);
                                                      }
                                                }
                                                newSocketArray.assign(tempArr.begin(), tempArr.end());
                                                
                                          } else {
                                                printf("recv failed: %d\n", WSAGetLastError());
                                                closesocket(currSocketFd);
                                                FD_CLR(currSocketFd, &activeFdSet);
                                                WSACleanup();
                                                return;
                                          }
                                    }	
                              }
                        }

                        // update sockets array so that the next loop will use this updated array 
                        socketsArray.assign(newSocketArray.begin(), newSocketArray.end());
                  }
            }

            void sendLogMessageToEveryone(std::string messagee) {
                  for(int j = 1; j < (int)socketsArray.size(); j++){
                        std::string message = "log:"+messagee+"*";

                        SOCKET sockFd = (SOCKET)socketsArray.at(j);
                        sendResult = send(sockFd, message.c_str(), strlen(message.c_str()), 0);
                  }
            }
};