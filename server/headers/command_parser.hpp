#pragma once

#include <string>
#include <iostream>

#include <deque>
#include <vector>

typedef struct{
      std::string commandId;
      std::vector<std::string> args;
} commandType;

class commandParser {
      private:
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
            commandParser() { }

            std::vector<commandType> parse(std::string commandstr) {
                  std::vector<commandType> commands = explodeCommand(commandstr);
                  return commands;
            }
};