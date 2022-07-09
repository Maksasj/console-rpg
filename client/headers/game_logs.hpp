#pragma once

#include <string>
#include <deque>

#include "console_graphics.hpp"

typedef struct{
      std::string text;
} gameLogType;

typedef struct{
      int x = 0;
      int y = 0;
} vec2;

class logs {
      private:
            int log_length;
            std::deque<gameLogType> log_deq;

            vec2 offset;

      public:
            logs(vec2 _offset, int len) {
                  offset = _offset;
                  log_length = len;
            };

            void insertLog(std::string text) {
                  log_deq.push_back({text});

                  while(log_deq.size() > log_length) {
                        log_deq.pop_front();
                  }
            }

            void drawLog() {
                  int _offset = 0;
                  for(auto log : log_deq) {
                        safePrint(log.text, offset.x, offset.y + _offset);
                        _offset++;
                  }
            }
};