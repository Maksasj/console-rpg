g++ -Wall -w -g -c -std=c++14 server.cpp -o server.o
g++ server.o -lws2_32 -lmswsock -static-libstdc++ -static-libgcc -o server.exe