#pragma once

#include <iostream>
#include <fstream>
#include "combatClasses.hpp"

class Player {
      private:
            //combatClass player_class;
            std::string identifier;
            std::string name;

      public:
            Player() {}

            Player(std::string _name, std::string _identifier) {
                  name = _name;
                  identifier = _identifier;
                  //player_class = _class;
            }
};

void savePlayerToFile(const std::string& file_name, Player& data) {
  std::ofstream out(file_name.c_str());
  out.write(reinterpret_cast<char*>(&data), sizeof(Player));
  out.close();
}

void getPlayerFromFile(const std::string& file_name, Player& data) {
  std::ifstream in(file_name.c_str());
  in.read(reinterpret_cast<char*>(&data), sizeof(Player));
  in.close();
}