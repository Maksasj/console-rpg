#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <stdexcept>
#include <iterator>
#include <sstream>

#include "console_graphics.hpp"

using namespace std;

ostream& formatDateTime(ostream& out, const tm& t, const char* fmt) {
  const time_put<char>& dateWriter = use_facet<time_put<char> >(out.getloc());
  int n = strlen(fmt);
  if (dateWriter.put(out, out, ' ', &t, fmt, fmt + n).failed()) {
    throw runtime_error("failure to format date time");
  }
  return out;
}

string dateTimeToString(const tm& t, const char* format) {
  stringstream s;
  formatDateTime(s, t, format);
  return s.str();
}

tm now() {
  time_t now = time(0);
  return *localtime(&now);
}

static void clock(void *data) {
      int *d = (int*)data;

      while(true) {
            std::string s = dateTimeToString(now(), " %H:%M:%S");
            safePrintC(s, cgGreen, 71, 1);
      }
}