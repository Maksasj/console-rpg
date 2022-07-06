#pragma once

#include <stdlib.h>
#include <string>
#include <windows.h>

#define setCursorPos(gc_X, gc_Y) COORD p = { (SHORT)gc_X, (SHORT)gc_Y };\
            SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), p );


#define cgRed FOREGROUND_RED | FOREGROUND_INTENSITY
#define cgBlue FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define cgGreen FOREGROUND_GREEN | FOREGROUND_INTENSITY

#define cgLightBlue FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define cgPink FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define cgYellow FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY

void setWindowSize(int Width, int Height) {
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
}

void setPoint(char sym, int X, int Y) {
	DWORD written = 0;
	WriteConsoleOutputCharacterA(GetStdHandle(STD_OUTPUT_HANDLE), (std::string(1, sym).c_str()), 1, { (SHORT)X, (SHORT)Y }, &written);
}

void setPointC(char sym, WORD color, int X, int Y) {
   WORD attribute = color ;
   DWORD written;

   ::WriteConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), &attribute, 1, { (SHORT)X, (SHORT)Y }, &written);
   ::WriteConsoleOutputCharacterA(GetStdHandle(STD_OUTPUT_HANDLE), (std::string(1, sym).c_str()), 1, { (SHORT)X, (SHORT)Y }, &written);
}

void fill(char sym, int X, int Y, int X2, int Y2) {
      for(int x = X; x < X2; x++) {
            for(int y = Y; y < Y2; y++) {
                  setPoint(sym, x, y);
            }
      }
}

void drawRect(int X, int Y, int X2, int Y2) {
	for(int x = X; x <= X2; x++) {
		if(x == X || x == X2) {
			setPoint('+', x, Y);
			setPoint('+', x, Y2);
		} else {
			setPoint('-', x, Y);
			setPoint('-', x, Y2);
		}
	}

	for(int y = Y; y <= Y2; y++) {
		if(y == Y || y == Y2) {
			setPoint('+', X, y);
			setPoint('+', X2, y);
		} else {
			setPoint('|', X, y);
			setPoint('|', X2, y);
		}
	}
}

void drawRect(int X, int Y, int X2, int Y2, WORD color) {
	for(int x = X; x <= X2; x++) {
		if(x == X || x == X2) {
			setPointC('+', color, x, Y);
			setPointC('+', color, x, Y2);
		} else {
			setPointC('-', color, x, Y);
			setPointC('-', color, x, Y2);
		}
	}

	for(int y = Y; y <= Y2; y++) {
		if(y == Y || y == Y2) {
			setPointC('+', color, X, y);
			setPointC('+', color, X2, y);
		} else {
			setPointC('|', color, X, y);
			setPointC('|', color, X2, y);
		}
	}
}

void safePrint(std::string text, int X, int Y) {
	for(auto letter : text) {
		setPoint(letter, X, Y);
		X++;
	}
}

void safePrintC(std::string text, WORD color, int X, int Y) {
	for(auto letter : text) {
		setPointC(letter, color, X, Y);
		X++;
	}
}