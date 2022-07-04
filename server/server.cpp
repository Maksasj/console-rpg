#include "headers/serverClass.hpp"
#include "../dependencies/threads.hpp"
#include "headers/playerClass.hpp"

static void proc(void *data) {
      int *d = (int*)data;

      while(true) {
            std::cout << "test "+std::to_string((*d))+"\n";
            //printf("number "+(*d).std::to_string()+". lol \n");
      }
}

int main(void) {
	Player pl("naonfapo", "Maksasj");
	/*
	int *a = new int(69);
	Thread<int> testThread(proc, a);

	while(true) {
		//do stuff
	}
	//testThread.deleteThread();
	*/
	
	server Server;
	
	Server.run();
	Server.close();

	return 0;
}