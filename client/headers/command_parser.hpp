#pragma once

#include <string>
#include <iostream>

#include <deque>
#include <vector>

#include "console_graphics.hpp"
#include "game_logs.hpp"

typedef struct{
      std::string commandId;
      std::vector<std::string> args;
} commandType;

class commandParser {
      private:
            logs *logger;

            std::deque<char> stringToDequeTTTTT(std::string text) {
                  std::deque<char> out;

                  for(auto letter : text) {
                        out.push_back(letter);
                  }

                  return out;
            }


            std::vector<commandType> explodeCommand(std::string init_command) {
                  std::vector<commandType> commandTypeVector;
                  std::vector<std::string> commands_str;

                  std::string tmp_command_init;
                  for(auto letter : init_command) {
                        if(letter != '*') {
                              tmp_command_init.push_back(letter);
                        } else {
                              commands_str.push_back(tmp_command_init);
                              tmp_command_init = "";
                        }
                  }

                  for(auto command : commands_str) {
                        std::string commandId;
                        std::vector<std::string> args;

                        std::deque<char> sym_deq = stringToDequeTTTTT(command);

                        while(true) {
                              char sym = sym_deq[0];
                              sym_deq.pop_front();

                              if(sym != ':') {
                                    commandId.push_back(sym);
                              } else {
                                    break;
                              }

                              if(sym_deq.size() < 1) {
                                    break;
                              }
                        }
                        
                        if(commandId != "" && sym_deq.size() != 0) {
                              std::string tmp_arg;

                              while(true) {
                                    char sym = sym_deq[0];
                                    sym_deq.pop_front();

                                    if(sym != ',') {
                                          tmp_arg.push_back(sym);
                                    } else {
                                          args.push_back(tmp_arg);
                                          tmp_arg = "";
                                    }

                                    if(sym_deq.size() < 1) {
                                          if(tmp_arg != "") {
                                                args.push_back(tmp_arg);
                                          }
                                          break;
                                    }
                              }
                        }

                        commandTypeVector.push_back({ commandId, args });
                  }
                  

                  return commandTypeVector;
            }

      public:
            commandParser() {
                  logger = new logs();
            }

            void parse(std::string commandstr) {
                  std::vector<commandType> commands = explodeCommand(commandstr);

                  for(auto command : commands) {
                        if(command.commandId == "updatestats") {
                              safePrintC("HP: "+command.args[0], cgRed, 1, 3);
                              safePrintC("ARM: "+command.args[1], cgYellow, 8, 3);
                              safePrintC("RES: "+command.args[2], cgYellow, 17, 3);
                              safePrintC("STR: "+command.args[3], cgRed, 26, 3);
                              safePrintC("INT: "+command.args[4], cgLightBlue, 35, 3);
                        } else if(command.commandId == "log") {
                              //safePrint("log", 3, 15);
                              logger->insertLog(command.args[0]);
                              logger->drawLog();
                        } else if(command.commandId == "servername") {
                              safePrintC(command.args[0], cgGreen, 52, 1);
                        }
                  }
            }
};