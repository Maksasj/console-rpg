#include "headers/serverClass.hpp"
#include "headers/playerClass.hpp"

#include "../dependencies/threads.hpp"
#include "../dependencies/sha256.hpp"

static void proc(void *data) {
      int *d = (int*)data;

      while(true) {
            std::cout << "test "+std::to_string((*d))+"\n";
            //printf("number "+(*d).std::to_string()+". lol \n");
      }
}

int main() {
	//Player pl1("naonfapo", "Maksasj");
	//savePlayerToFile("player.bin", pl1);

      Player pl;
      getPlayerFromFile("player.bin", pl);
	std::cout << pl.name.to_string();

	return 0;
}

 	

/*
int main(void) {
	//std::string output1 = sha256("maksasj");
	//std::cout << output1 << "\n";
	const Player pl1("naonfapo", "Maksasj");
	Save("player.bin", pl1);

	//Player pl;
	//getPlayerFromFile("player", pl);
	//printf((pl.getName()).c_str());
	//std::cout << "\n" << pl.getName() << "\n";

	int *a = new int(69);
	Thread<int> testThread(proc, a);

	while(true) {
		//do stuff
	}
	//testThread.deleteThread();

	
	server Server;
	
	Server.run();
	Server.close();

	return 0;
}
*/