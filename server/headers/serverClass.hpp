#pragma once

#include <map>

#include "server_networking.hpp"
#include "command_parser.hpp"
#include "playerClass.hpp"
#include "combatClasses.hpp"

class server : public serverNetworking {
      private:
            commandParser parser;
            std::map<SOCKET, Player> player_pool;

            void registerPlayer(std::string name, std::string _identifier, std::string pclass) {           
                  Player p;

                  if(pclass == "warrior") {
                        p = Player(name, _identifier, Warrior());
                  } else if(pclass == "rogue") {
                        p = Player(name, _identifier, Rogue());
                  } else if(pclass == "cleric") {
                        p = Player(name, _identifier, Cleric());
                  } else if(pclass == "mage") {
                        p = Player(name, _identifier, Mage());
                  }
                  
                  savePlayerToFile("usersdata/"+_identifier+".bin", p);
            }

            void sendPlayerData(SOCKET currSocketFd) {
                  std::string message_stats = "updatestats:"
                        +std::to_string(player_pool[currSocketFd].getHealth())+","
                        +std::to_string(player_pool[currSocketFd].getArmor())+","
                        +std::to_string(player_pool[currSocketFd].getAgility())+","
                        +std::to_string(player_pool[currSocketFd].getStrenght())+","
                        +std::to_string(player_pool[currSocketFd].getIntelect())+"*";

                  std::string message_servername = "servername:Ursina*";
                  std::string message_playersonline = "playersonline:"+std::to_string(player_pool.size())+"*";

                  send(currSocketFd, message_stats.c_str(), strlen(message_stats.c_str()), 0);
                  send(currSocketFd, message_servername.c_str(), strlen(message_servername.c_str()), 0);
                  send(currSocketFd, message_playersonline.c_str(), strlen(message_playersonline.c_str()), 0);
            }
            
            void erasePlayer(SOCKET currSocketFd) override {
                  player_pool.erase(currSocketFd);
            }

            std::string getPlayerNameBySocket(SOCKET currSocketFd) {
                  return player_pool[currSocketFd].getName();
            }

            void loginPlayer(SOCKET clientSocket, std::string _identifier) {
                  std::string welcome_message = "log:welcome !*";
                  send(clientSocket, welcome_message.c_str(), strlen(welcome_message.c_str()), 0);

                  Player p;
                  getPlayerFromFile("usersdata/"+_identifier+".bin", p);

                  if(p.name.to_string() != "") {
                        player_pool[clientSocket] = p;
                  }

                  sendPlayerData(clientSocket);
            }
            
            void parseCommand(SOCKET currSocketFd, std::string text) override {
                  std::vector<commandType> commands = parser.parse(text);

                  if(!(commands.empty())) {
                        for(auto command : commands) {
                              
                              if(command.commandId == "requesfordata") {
                                    sendPlayerData(currSocketFd);
                              } else if(command.commandId == "register") {
                                    std::cout << "[SERVER]: new user registered, username:"+command.args[0]+"\n";
                                    registerPlayer(command.args[0], command.args[1], command.args[2]);

                              } else if(command.commandId == "login") {
                                    loginPlayer(currSocketFd, command.args[0]);

                                    std::string player_name = player_pool[currSocketFd].name.to_string();
                                    std::cout << "[SERVER]: user joined: "+player_name << "\n";

                                    sendLogMessageToEveryone("joined "+player_name);
                              } else if(command.commandId == "disconnect") {
                                    std::cout << "[SERVER]: user disconnected: "+command.args[0] << "\n";

                                    sendLogMessageToEveryone("disconnected "+command.args[0]);
                                    //loginPlayer(currSocketFd, command.args[0]);
                              } else if(command.commandId == "say") {
                                    std::string player_username = player_pool[currSocketFd].getName();
                                    std::string text = command.args[0];

                                    std::string out = "";
      
                                    for(auto letter : text) {
                                          if(letter == '#') {
                                                out += " ";
                                          } else {
                                                out += letter;
                                          }
                                    }

                                    std::cout << "[SERVER]: ["+player_username+"]: "+out+"\n";
                                    sendLogMessageToEveryone(player_username+": "+text);
                                    sendPlayerData(currSocketFd);
                              }
                        }
                  }
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