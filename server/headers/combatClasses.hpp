#pragma once

#include <string>
#include "statBlock.hpp"

class combatClass {
      private:
            statBlock baseStats;

      public:
            combatClass() {
                  baseStats = statBlock(0, 0, 0, 0, 0);
            };
            
            combatClass(statBlock _baseStats) {
                  baseStats = _baseStats;
            }

            statBlock getBaseStats() {
                  return baseStats;
            }

            void increaseStats(statBlock value) {
                  baseStats += value;
            }

};

class Warrior: public combatClass {
      public:
            Warrior() : combatClass(statBlock(15, 4, 1, 4, 3)) { // hp, arm, agi, str, intel

            }
};

class Mage: public combatClass {
      public:
            Mage() : combatClass(statBlock(7, 1, 2, 1, 9)) { // hp, arm, agi, str, intel

            }
};

class Cleric: public combatClass {
      public:
            Cleric() : combatClass(statBlock(12, 2, 3, 2, 5)) { // hp, arm, agi, str, intel

            }
};

class Rogue: public combatClass {
      public:
            Rogue() : combatClass(statBlock(13, 3, 6, 3, 1)) { // hp, arm, agi, str, intel

            }
};