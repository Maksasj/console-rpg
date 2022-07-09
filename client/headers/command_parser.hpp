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
            logs *actionlogger;

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
            commandParser(logs *l, logs *_actionlogger) {
                  logger = l;
                  actionlogger = _actionlogger;
            }

            void parse(std::string commandstr) {
                  std::vector<commandType> commands = explodeCommand(commandstr);

                  for(auto command : commands) {
                        if(command.commandId == "updatestats") {
                              safePrintC("HP: "+command.args[0], cgRed, 1, 3);
                              safePrintC("ARM: "+command.args[1], cgYellow, 1, 4);
                              safePrintC("AGI: "+command.args[2], cgYellow, 1, 5);
                              safePrintC("STR: "+command.args[3], cgRed, 1, 6);
                              safePrintC("INT: "+command.args[4], cgLightBlue, 1, 7);
                        } else if(command.commandId == "log") {
                              std::string out = "";

                              for(auto letter : command.args[0]) {
                                    if(letter == '#') {
                                          out += " ";
                                    } else {
                                          out += letter;
                                    }
                              }

                              logger->insertLog(out);
                              logger->drawLog();
                        } else if(command.commandId == "actionlog") {
                              std::string out = "";

                              for(auto letter : command.args[0]) {
                                    if(letter == '#') {
                                          out += " ";
                                    } else {
                                          out += letter;
                                    }
                              }

                              actionlogger->insertLog(out);
                              actionlogger->drawLog();
                        } else if(command.commandId == "updateenergy") {
                              safePrintC(command.args[0]+"/"+command.args[1], cgBlue, 14, 4);
                              safePrintC(command.args[2]+"/"+command.args[3], cgYellow, 16, 5);
                              
                        } else if(command.commandId == "updategold") {
                              safePrintC(command.args[0], cgYellow, 14, 7);
      
                        } else if(command.commandId == "updatehealth") {
                              safePrintC(command.args[0]+"/"+command.args[1], cgRed, 16, 3);
      
                        } else if(command.commandId == "servername") {
                              safePrintC(command.args[0], cgGreen, 52, 1);

                        } else if(command.commandId == "playername") {
                              safePrint(command.args[0], 1, 1);
                              
                        } else if(command.commandId == "classname") {
                              safePrintC(command.args[0], cgRed, 9, 1);

                        } else if(command.commandId == "playerlvl") {
                              safePrint("LVL: "+command.args[0], 17, 1);

                        } else if(command.commandId == "playerexp") {
                              safePrintC("EXP: "+command.args[0]+"/"+command.args[1], cgGreen, 26, 1);
                              
                        } else if(command.commandId == "playersonline") {
                              safePrintC(command.args[0], cgGreen, 68, 1);
                              
                        } else if(command.commandId == "location") {
                              safePrint(command.args[0], 22, 3);

                              for(int x = 1; x < command.args.size(); x++) {
                                    safePrint(command.args[x], 22, 3 + x);
                              }
                              
                        } else if(command.commandId == "doublelogin") {
                              clearConsole
                              std::cout << "Double login detected, this account currenly connected!";
                        }
                  }
            }
};