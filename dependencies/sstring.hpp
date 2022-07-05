#pragma once;

#include <string>

template<int _size>
class sstring {
      private:
            char data[_size];
            int Size = 0;
      public:
            sstring() { }

            sstring(std::string _string) {
                  int x = 0;
                  for(auto letter : _string) {
                        data[x] = letter;
                        x++;
                  }
                  Size = x;
            } 

            std::string to_string() {
                  std::string out;

                  for(int x = 0; x < Size; x++) {
                        out += data[x];
                  }

                  return out;
            }

            int size() {
                  return Size;
            }

            template<typename T>
            T operator=(std::string const& other) {
                  T s(other);
                  return s;
            }
};