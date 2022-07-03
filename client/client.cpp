#include <string>
#include <iostream>

#include "headers/game_networking.hpp"
#include "headers/console_graphics.hpp"
#include "headers/command_parser.hpp"

void BasicScreen() {
    system("CLS");

    drawRect(0, 0, 63, 2);
    drawRect(43, 0, 63, 2);
    drawRect(0, 2, 63, 17);
    drawRect(0, 2, 43, 17);

    drawRect(0, 15, 43, 17);

    //Stats bar:
    drawRect(0, 2, 7, 4); //Hp
    drawRect(7, 2, 16, 4); //Arm
    drawRect(16, 2, 25, 4); //Res
    drawRect(25, 2, 34, 4); //Str
    drawRect(34, 2, 43, 4); //Int

    safePrint("Server: ", 44, 1);
}

int main() {
    //SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_LINE_INPUT);
    BasicScreen();
    
    commandParser *parser = new commandParser();
    gameNetworking *net = new gameNetworking(parser);

    bool connect = net->connectServer("Maksim");

    if(connect) {
            while(true) {
                setCursorPos(1, 16);

                std::string command;
                std::cin >> command;

                BasicScreen();
            if(!(command.empty())) {
                net->sendMsg(command);
            }
        }
    }

    

    net->disconnect();
    net->close();
    
    return 0;
}