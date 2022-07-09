#pragma once

#include <string>
#include "statBlock.hpp"
#include "../../dependencies/sstring.hpp"

class combatClass {
      private:
            statBlock baseStats;
            sstring<20> name;

      public:
            combatClass() {
                  baseStats = statBlock(0, 0, 0, 0, 0);
                  name = std::string("None");
            };
            
            combatClass(std::string class_name, statBlock _baseStats) {
                  baseStats = _baseStats;
                  name = class_name;
            }

            statBlock getBaseStats() {
                  return baseStats;
            }

            void increaseStats(statBlock value) {
                  baseStats += value;
            }

            std::string getClassName() {
                  return name.to_string();
            }

};

class Warrior: public combatClass {
      public:
            Warrior() : combatClass("Warrior", statBlock(15, 4, 1, 4, 3)) { // hp, arm, agi, str, intel

            }
};

class Mage: public combatClass {
      public:
            Mage() : combatClass("Mage", statBlock(7, 1, 2, 1, 9)) { // hp, arm, agi, str, intel

            }
};

class Cleric: public combatClass {
      public:
            Cleric() : combatClass("Cleric", statBlock(12, 2, 3, 2, 5)) { // hp, arm, agi, str, intel

            }
};

class Rogue: public combatClass {
      public:
            Rogue() : combatClass("Rogue", statBlock(13, 3, 6, 3, 1)) { // hp, arm, agi, str, intel

            }
};