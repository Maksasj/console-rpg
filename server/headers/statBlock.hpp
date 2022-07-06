#pragma once

#include "statstype.hpp"

struct statBlock {
            stattype health;
            stattype armor;
            stattype agility;
            stattype strenght;
            stattype intelect;

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

            statBlock& operator += (const statBlock& rhs) {
                  health += rhs.health;
                  armor += rhs.armor;
                  agility += rhs.agility;
                  strenght += rhs.strenght;
                  intelect += rhs.intelect;
                  return *this;
            }

};