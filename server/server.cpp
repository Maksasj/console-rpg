#include "headers/serverClass.hpp"
#include "headers/playerClass.hpp"

#include "../dependencies/threads.hpp"
#include "../dependencies/sha256.hpp"

static void proc(void *data) {
      //int *d = (int*)data;

      while(true) {
            //std::cout << "test "+std::to_string((*d))+"\n";
            //printf("number "+(*d).std::to_string()+". lol \n");
      }
}

int main(void) {
	int *a = new int(69);
	Thread testThread(proc, a);

	server Server;
	Server.run();
	Server.close();

	return 0;
}