#pragma once

#include <map>

#include "json.hpp"
#include "server_networking.hpp"
#include "command_parser.hpp"

using json = nlohmann::json;

typedef struct{
      json data;
} player;

class server : public serverNetworking {
      private:
            commandParser parser;
            std::map<SOCKET, player> player_pool;

            void processNewPlayer(SOCKET clientSocket) override {
                  json player_data;
                  
                  std::string welcome_message = "log:welcome !*";
                  std::string message_stats = "updatestats:5,3,5,6,4,3*";
                  std::string message_servername = "servername:Ursina*";

                  send(clientSocket, message_stats.c_str(), strlen(message_stats.c_str()), 0);
                  send(clientSocket, message_servername.c_str(), strlen(message_servername.c_str()), 0);
                  send(clientSocket, welcome_message.c_str(), strlen(welcome_message.c_str()), 0);
            }

            void parseCommand(SOCKET currSocketFd, std::string text) override {
                  std::vector<commandType> commands = parser.parse(text);

                  if(!(commands.empty())) {
                        for(auto command : commands) {
                              
                              if(command.commandId == "login") {
                                    std::cout << "[SERVER]: user joined: "+command.args[0] << "\n";

                                    sendLogMessageToEveryone("joined "+command.args[0]);
                              } else if(command.commandId == "say") {
                                    std::cout << "[SERVER]: user used say command\n";

                                    sendLogMessageToEveryone(command.args[0]+": "+command.args[1]);
                              }
                        }
                  }

                  //Send all player data again, because player lost that, then hi send new command.
                  std::string message_stats = "updatestats:5,3,5,6,4,3*";
                  std::string message_servername = "servername:Ursina*";

                  send(currSocketFd, message_stats.c_str(), strlen(message_stats.c_str()), 0);
                  send(currSocketFd, message_servername.c_str(), strlen(message_servername.c_str()), 0);
            }

      public:
            server() : serverNetworking() {
                  
            };
            
            void run() {
                  std::cout << "[SERVER] Server started \n";
                  std::cout << "[SERVER] waiting for connections...\n";
                  read();
            }

            void close() {
                  exit(1);
            }
};