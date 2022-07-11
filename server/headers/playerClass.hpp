#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdint>

#include "../../dependencies/sstring.hpp"

#include "statstype.hpp"
#include "combatClasses.hpp"
#include "playerProgressionTypes.hpp"
#include "moneyTypes.hpp"
#include "energyTypes.hpp"

typedef uint_fast16_t HealthType;

class Player {
      private:
            HealthType health;
            HealthType maxHealth;

            sstring<64> identifier;
            sstring<20> name;

            sstring<20> gameLocation;

            lvlType lvl; // exp proggresion 100, 200, 400, 800, 1600 3200 6400 12800....
            expType exp;
            expType expToNexLvl;

            combatClass cclass;
            energyBlock energy;

            Gold gold;

            bool battleMode;
      public:
            Player() {}

            Player(std::string _name, std::string _identifier, combatClass _cclass) {
                  name = _name;
                  identifier = _identifier;
                  cclass = _cclass;

                  lvl = 1;
                  exp = 0;
                  expToNexLvl = 100;

                  gold = 0;

                  gameLocation = std::string("RoyalFields");
                  battleMode = false;

                  health = cclass.getBaseStats().health;
                  maxHealth = cclass.getBaseStats().health;

                  energy = energyBlock(cclass.getBaseStats().intelect*2, cclass.getBaseStats().agility);
            }

            damageType dealDamage() {
                  return cclass.dealDamage();
            }

            void setBattleMode(bool _value) {
                  battleMode = _value;
            }

            bool getBattleMode() {
                  return battleMode;
            }

            void gainExp(expType amount) {
                  if((exp + amount) >= expToNexLvl) {
                        lvl++;
                        exp = 0;
                        expToNexLvl *= 2;
                  } else {
                        exp += amount;
                  }
            }

            std::string getLocation() {
                  return gameLocation.to_string();
            }

            void setLocation(std::string _loc) {
                  gameLocation = _loc; 
            }

            energyBlock getEnergyBlock() {
                  return energy;
            }

            expType getExp() {
                  return exp;
            }

            expType getExpToNextLvl() {
                  return expToNexLvl;
            }

            lvlType getLvl() {
                  return lvl;
            }

            Gold getGold() {
                  return gold;
            }

            std::string getClassName() {
                  return cclass.getClassName();
            }

            HealthType getHealth() {
                  return health;
            }

            HealthType getMaxHealth() {
                  return maxHealth;
            }

            stattype getHp() {
                  return cclass.getBaseStats().health;
            }

            stattype getArm() {
                  return cclass.getBaseStats().armor;
            }

            stattype getAgi() {
                  return cclass.getBaseStats().agility;
            }

            stattype getStr() {
                  return cclass.getBaseStats().strenght;
            }

            stattype getInt() {
                  return cclass.getBaseStats().intelect;
            }

            std::string getName() {
                  return name.to_string();
            }

            std::string getIdentifier() {
                  return identifier.to_string();
            }

            void gainGold(Gold _gold) {
                  gold += _gold;
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