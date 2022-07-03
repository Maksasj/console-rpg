#pragma once

#include <string>
#include <deque>

#include "console_graphics.hpp"

typedef struct{
      std::string text;
} gameLogType;

class logs {
      private:
            static const int log_length = 10;
            std::deque<gameLogType> log_deq;

      public:
            logs() {};

            void insertLog(std::string text) {
                  log_deq.push_back({text});

                  while(log_deq.size() > log_length) {
                        log_deq.pop_front();
                  }
            }

            void drawLog() {
                  int offset = 0;
                  for(auto log : log_deq) {
                        safePrint(log.text, 44, 3 + offset);
                        offset++;
                  }
            }
};