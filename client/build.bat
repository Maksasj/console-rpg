g++ -Wall -g -c -std=c++14 client.cpp
g++ client.o -lws2_32 -lmswsock -static-libstdc++ -static-libgcc -o client.exe