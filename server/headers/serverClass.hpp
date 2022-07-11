#pragma once

#include <map>
#include <Windows.h>

#include "server_networking.hpp"
#include "command_parser.hpp"
#include "playerClass.hpp"
#include "combatClasses.hpp"
#include "serverDataSender.hpp"
#include "gameLocation.hpp"
#include "mobClass.hpp"

#include "../../dependencies/threads.hpp"

struct attaclHandleType {
      Player *p;
      SOCKET socket;
      Mob mob;

      attaclHandleType(Player *_p, SOCKET _socket, Mob _mob) {
            p = _p;
            socket = _socket;
            mob = _mob;
      }
};

static void attackThreadProc(void *data) {
      attaclHandleType *ah = (attaclHandleType*)data;
      std::cout << ah->p->getName() << "\n";
      while(true) {
            int damage = ah->p->dealDamage();
            ah->mob.setHealth(ah->mob.getHealth() - damage);

            std::cout << "[SERVER]: "+ah->p->getName()+" attacked "+ah->mob.getName()+" for "+std::to_string(damage)+" damage\n";
            std::string message = "actionlog:You#dealed#"+std::to_string(damage)+"#to#"+ah->mob.getName()+"#damage*";
            send(ah->socket, message.c_str(), strlen(message.c_str()), 0);

            if(ah->mob.getHealth() < 1) {
                  ah->p->setBattleMode(false);
                  std::cout << "[SERVER]: "+ah->p->getName()+" killed "+ah->mob.getName()+"\n";
                  std::string message = "actionlog:You#killed#"+ah->mob.getName()+"#and#received#1#gold#and#5#exp*";

                  ah->p->gainExp(ah->mob.getExp());
                  ah->p->gainGold(ah->mob.getGold());

                  serverDataSender sds;
                  sds.commandPlayerGold(ah->socket, *(ah->p));
                  sds.commandPlayerExp(ah->socket, *(ah->p));

                  send(ah->socket, message.c_str(), strlen(message.c_str()), 0);
                  break;
            }

            Sleep(1500); 
      }

      delete ah;
}

class server : public serverNetworking, public serverDataSender {
      private:
            std::map<std::string, gameLocation> gameWorld;

            commandParser parser;
            std::map<SOCKET, Player> player_pool;

            void initGameWorld() {
                  gameWorld["RoyalFields"] = createGameLocation("RoyalFields")
                        .addNearLoc("FoggyRavine")
                        .addMob(Rat())
                        .addMob(Zombie());

                  gameWorld["FoggyRavine"] = createGameLocation("FoggyRavine")
                        .addNearLoc("RoyalFields")
                        .addMob(Zombie())
                        .addMob(Slime());
            }

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
                  Player p = player_pool[currSocketFd];

                  commandUpdateStats(currSocketFd, p);
                  commandServerName(currSocketFd);
                  commandPlayersOnline(currSocketFd, player_pool);
                  commandPlayerName(currSocketFd, p);
                  commandPlayerLvl(currSocketFd, p);
                  commandPlayerExp(currSocketFd, p);
                  commandPlayerClassName(currSocketFd, p);
                  commandEnergyBar(currSocketFd, p);
                  commandPlayerGold(currSocketFd, p);
                  commandPlayerHealth(currSocketFd, p);
                  commandPlayerLocation(currSocketFd, p, gameWorld);
            }
            
            void erasePlayer(SOCKET currSocketFd) override {
                  player_pool.erase(currSocketFd);
            }

            std::string getPlayerNameBySocket(SOCKET currSocketFd) {
                  return player_pool[currSocketFd].getName();
            }

            bool loginPlayer(SOCKET clientSocket, std::string _identifier) {
                  Player p;
                  getPlayerFromFile("usersdata/"+_identifier+".bin", p);

                  for(auto tmpP : player_pool) {
                        if(tmpP.second.getIdentifier() == p.getIdentifier()) {
                              std::cout << "[SERVER]: Detected double login from: "+p.getName() << "\n";
                              return false;
                        }
                  }

                  std::string welcome_message = "log:welcome !*";
                  send(clientSocket, welcome_message.c_str(), strlen(welcome_message.c_str()), 0);


                  if(p.getName() != "") {
                        player_pool[clientSocket] = p;
                  }

                  sendPlayerData(clientSocket);
                  return true;
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
                                    bool successful = loginPlayer(currSocketFd, command.args[0]);

                                    if(successful) {
                                          std::string player_name = player_pool[currSocketFd].getName();
                                          std::cout << "[SERVER]: user joined: "+player_name << "\n";

                                          sendLogMessageToEveryone("joined "+player_name);

                                          std::string message_playersonline = "playersonline:"+std::to_string(player_pool.size())+"*";
                                          for(auto player : player_pool) {
                                                send(player.first, message_playersonline.c_str(), strlen(message_playersonline.c_str()), 0);
                                          }
                                    }
                              } else if(command.commandId == "requestforloc") {
                                    Player p = player_pool[currSocketFd];

                                    std::string messageLocData = "location:"+p.getLocation();

                                    int n = 1;
                                    for(auto mob : gameWorld[p.getLocation()].getMobs()) {
                                          messageLocData += ","+std::to_string(n)+"."+mob.getName()+"("+std::to_string(mob.getLvl())+"lvl)";
                                          n++;
                                    }

                                    messageLocData += "*";
                                    send(currSocketFd, messageLocData.c_str(), strlen(messageLocData.c_str()), 0);
                              } else if(command.commandId == "looknear") {
                                    Player p = player_pool[currSocketFd];

                                    std::string message = "actionlog:Now#near#you";

                                    for(auto loc : gameWorld[p.getLocation()].getNearLoc()) {
                                          message += "#"+loc;
                                    }

                                    message += "*";
                                    send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);

                              } else if(command.commandId == "goto") {
                                    if(!player_pool[currSocketFd].getBattleMode()) {
                                          Player p = player_pool[currSocketFd];

                                          bool canGo = false;

                                          for(auto loc : gameWorld[p.getLocation()].getNearLoc()) {
                                                if(command.args[0] == loc) {
                                                      canGo = true;
                                                      break;
                                                }
                                          }

                                          if(canGo) {
                                                std::cout << "[SERVER]: "+p.getName()+" went to "+command.args[0]+"\n";
                                                player_pool[currSocketFd].setLocation(command.args[0]);

                                                std::string message = "actionlog:You#went#to#"+command.args[0]+"*";
                                                send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
                                                commandPlayerLocation(currSocketFd, p, gameWorld);
                                          }
                                    } else {
                                          std::string message = "actionlog:You#all#ready#in#battle#mode*";
                                          send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
                                    }
                              } else if(command.commandId == "attack") {
                                    if(!player_pool[currSocketFd].getBattleMode()) {
                                          Player p = player_pool[currSocketFd];
                                          std::string attackingMob = command.args[0];

                                          bool canAttack = false;
                                          for(auto mob : gameWorld[p.getLocation()].getMobs()) {
                                                if(mob.getName() == attackingMob) {
                                                      canAttack = true;
                                                      break;
                                                }
                                          }

                                          if(canAttack) {
                                                p.setBattleMode(true);
                                                
                                                attaclHandleType *ah= new attaclHandleType(&p, currSocketFd, mobToString(attackingMob));

                                                Thread *attackThread = new Thread(attackThreadProc, ah);
                                          } else {
                                                std::string message = "actionlog:Wrong#mob#choosen#for#attack*";
                                                send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
                                          }
                                    } else {
                                          std::string message = "actionlog:You#all#ready#in#battle#mode*";
                                          send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
                                    }
                              } else if(command.commandId == "disconnect") {
                                    std::cout << "[SERVER]: user disconnected: "+command.args[0] << "\n";

                                    sendLogMessageToEveryone("disconnected "+command.args[0]);
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

                                    std::cout << "["+player_username+"]: "+out+"\n";
                                    sendLogMessageToEveryone(player_username+": "+text);
                                    sendPlayerData(currSocketFd);
                              }
                        }
                  }
            }

      public:
            server() : serverNetworking(), serverDataSender() {
                  initGameWorld();
            }
            
            void run() {
                  std::cout << "[SERVER] Server started \n";
                  std::cout << "[SERVER] waiting for connections...\n";
                  read();
            }

            void close() {
                  exit(1);
            }
};