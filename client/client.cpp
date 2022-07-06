#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <iostream>

#include "headers/game_networking.hpp"
#include "headers/console_graphics.hpp"
#include "headers/command_parser.hpp"
#include "headers/clientClockWidget.hpp"

#include "../dependencies/sha256.hpp"
#include "../dependencies/threads.hpp"

std::vector<std::string> explode(std::string const & s, char delim) {
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim); ) {
        result.push_back(std::move(token));
    }

    return result;
}

void BasicScreen() {
    system("CLS");

    drawRect(0, 0, 80, 2);
    drawRect(43, 0, 80, 2);
    drawRect(0, 2, 80, 17);
    drawRect(0, 2, 43, 17);

    drawRect(0, 15, 43, 17);

    //Stats bar:
    drawRect(0, 2, 7, 4); //Hp
    drawRect(7, 2, 16, 4); //Arm
    drawRect(16, 2, 25, 4); //Res
    drawRect(25, 2, 34, 4); //Str
    drawRect(34, 2, 43, 4); //Int

    safePrint("Server: ", 44, 1);
    safePrint("online: ", 60, 1);
}

void updateBasicScreen(logs *logger) {
    BasicScreen();
    logger->drawLog();
}

typedef struct {
    std::string name;
    std::string identifier;
} playerData;

//i need function that will delete all space before text
std::string deleteSpace(std::string text) {
    std::string out;
    bool firstWord = false;

    for(auto letter : text) {
        if(firstWord == true) {
            out += letter;
        } else {
            if(letter != ' ') {
                out += letter;
                firstWord = true;
            }
        }
    }

    return out;
}

//delete specifc symbol from string function
std::string deleteSymbol(std::string text, char symbol) {
    std::string out;

    for(auto letter : text) {
        if(letter != symbol) {
            out += letter;
        }
    }

    return out;
}


int main() {
    playerData PlayerData;

    logs *logger = new logs();
    commandParser *parser = new commandParser(logger);
    gameNetworking *net = new gameNetworking(parser);

    bool connect = false;
    std::cout << "Choose server, use [connect] ['servername']\n";
    std::cout << "      - Ursina \n";
    while(true) {
        std::string initCommand, serverName;
        std::cin >> initCommand >> serverName;

        if(initCommand == "connect") {
            if(serverName == "Ursina") {
                connect = net->connectServer("connect:client*");

                if(connect) {
                    break;
                } else {
                    std::cout << "Connection error \n";
                    continue;
                }
            }
        }
    }

    std::cout << "Type: [login, regiser], [username],[password], [warrior | cleric | mage | rogue ] \n";

    while(true) {
        std::string init_command;
        std::string command = "", name = "", password = "", pclass = "";
        std::cin >> command;

        if(command == "login") {
            std::cin >> name >> password;
            init_command += command+" "+name+" "+password;
        } else if(command == "register") {
            std::cin >> name >> password >> pclass;
            init_command += command+" "+name+" "+password+" "+pclass;
        }
        
        std::vector<std::string> parsed_command = explode(init_command, ' ');

        if(parsed_command.size() == 0) {
            std::cout << "Error: type [login, regiser], [username],[password], [warrior | ] \n";
        } else {
            if(parsed_command[0] == "login") {
                BasicScreen();
                std::string tmp_name = parsed_command[1];
                std::string tmp_password = parsed_command[2];

                std::string tmp_identifier = sha256(tmp_name+tmp_password);

                PlayerData = {tmp_name, tmp_identifier};

                net->setPlayerData(tmp_name, tmp_identifier);
                net->sendMsg("login:"+tmp_identifier+"*");
                
                if(connect) {
                    break;
                }
            } else if(parsed_command[0] == "register") {
                std::string tmp_name = parsed_command[1];
                std::string tmp_password = parsed_command[2];
                std::string tmp_class = parsed_command[3];

                std::string tmp_identifier = sha256(tmp_name+tmp_password);

                PlayerData = {tmp_name, tmp_identifier};

                connect = net->connectServer("register:"+tmp_name+","+tmp_identifier+","+tmp_class+"*");
                
                if(connect) {
                    std::cout << "You successfully registered, now just use login \n";
                }
            } else {
                std::cout << "Error: available keywords [login, register] \n";
            }
        }
    }

	Thread<int> clockThread(clock, new int(69));

    if(connect) {
        while(true) {
            bool commandHasBeenExecuted = false;
            setCursorPos(1, 16);

            std::string command;
            std::cin >> command;
            
            updateBasicScreen(logger);

            if(command == "say") {
                std::string out = "";
                std::string text;
                std::getline (std::cin, text);

                text = deleteSpace(text);
                
                text = deleteSymbol(text, '#');
                text = deleteSymbol(text, ',');
                text = deleteSymbol(text, '*');

                for(auto letter : text) {
                    if(letter == ' ') {
                        out += "#";
                    } else {
                        out += letter;
                    }
                }

                net->sendMsg("say:"+out+"*");
                commandHasBeenExecuted = true;
            } else if(command == "disconnect") {
                //net->sendMsg("disconnect:*");
                //net->disconnect();
                commandHasBeenExecuted = true;
                exit(0);
            }

            if(!commandHasBeenExecuted) {
                net->sendMsg("requesfordata:*");
            }
        }
    }

    net->close();
    
    return 0;
}