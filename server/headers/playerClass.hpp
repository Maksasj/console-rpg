#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>

#include "../../dependencies/sstring.hpp"

#include "combatClasses.hpp"

struct Player {
      //combatClass player_class;
      sstring<64> identifier;
      sstring<20> name;

      Player() {}

      Player(std::string _name, std::string _identifier) {
            name = _name;
            identifier = _identifier;
      }

      std::string getName() {
            return name.to_string();
      }

      void setName(std::string n) {
            name = n;
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