#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>

#include "../../dependencies/sstring.hpp"

#include "statstype.hpp"
#include "combatClasses.hpp"

struct Player {
      //combatClass player_class;
      sstring<64> identifier;
      sstring<20> name;
      combatClass cclass;

      Player() {}

      Player(std::string _name, std::string _identifier, combatClass _cclass) {
            name = _name;
            identifier = _identifier;
            cclass = _cclass;
      }

      stattype getHealth() {
            return cclass.getBaseStats().health;
      }

      stattype getArmor() {
            return cclass.getBaseStats().armor;
      }

      stattype getAgility() {
            return cclass.getBaseStats().agility;
      }

      stattype getStrenght() {
            return cclass.getBaseStats().strenght;
      }

      stattype getIntelect() {
            return cclass.getBaseStats().intelect;
      }

      std::string getName() {
            return name.to_string();
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