#pragma once;

#include "statBlock.hpp"
#include "moneyTypes.hpp"
#include "playerProgressionTypes.hpp"
#include "statstype.hpp"

class Mob {
      private:
            stattype health;
            lvlType lvl;

            expType expOnDeath;
            Gold goldOnDeath;

            std::string name;
            
            statBlock stats;
      public:
            Mob() {
                  health = 0;
                  name = "Undefined";
                  lvl = 0;
                  stats = statBlock(0, 0, 0, 0, 0);

                  expOnDeath = 0;
                  goldOnDeath = 0;
            }

            Mob(std::string _name, int _lvl, statBlock _stats) {
                  name = _name;
                  lvl = _lvl;
                  stats = _stats;
                  health = stats.health;

                  expOnDeath = lvl*2;

                  if((lvl - 5) <= 0) {
                        goldOnDeath = 0;
                  } else {
                        goldOnDeath = lvl - 5;
                  }
            }

            stattype getHealth() {
                  return health;
            }

            void setHealth(stattype value) {
                  health = value;
            }

            lvlType getLvl() {
                  return lvl;
            }

            Gold getGold() {
                  return goldOnDeath;
            }

            expType getExp() {
                  return expOnDeath;
            }

            std::string getName() {
                  return name;
            }

            statBlock getStats() {
                  return stats;
            }

            
};

struct Rat : public Mob {
      Rat() : Mob("Rat", 1, statBlock(3, 1, 1, 1, 1)) {};
};

struct Zombie : public Mob {
      Zombie() : Mob("Zombie", 3, statBlock(3, 3, 3, 3, 3)) {};
};

struct Slime : public Mob {
      Slime() : Mob("Slime", 4, statBlock(3, 3, 3, 3, 3)) {};
};

Mob mobToString(std::string mobStr) {
      if(mobStr == "Rat") {
            return Rat();
      } else if(mobStr == "Zombie") {
            return Zombie();
      } else if(mobStr == "Slime") {
            return Slime();
      }

      return Mob("None", 99, statBlock(0, 0, 0, 0, 0));
}