#pragma once

#include <string>
#include "statBlock.hpp"

class combatClass {
      private:
            statBlock baseStats;

      public:
            combatClass(statBlock _baseStats) {
                  baseStats = _baseStats;
            }

            statBlock getBaseStats() {
                  return baseStats;
            }

};

class Warrior: public combatClass {
      public:
            Warrior() : combatClass(statBlock(15, 3, 1, 4, 3)) { // hp, arm, agi, str, intel

            }
};