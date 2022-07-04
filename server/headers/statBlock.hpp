#pragma once

#include "statstype.hpp"

class statBlock {
      private:
            stattype health;
            stattype armor;
            stattype agility;
            stattype strenght;
            stattype intelect;
      public:
            statBlock(
                  stattype hp = 0,
                  stattype arm = 0,
                  stattype agi = 0,
                  stattype str = 0,
                  stattype intel = 0
            ) {
                  health = hp;
                  armor = arm;
                  agility = agi;
                  strenght = str;
                  intelect = intel;
            }
};