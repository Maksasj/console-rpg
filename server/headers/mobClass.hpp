#pragma once;

#include "statBlock.hpp"

class Mob {
      private:

            int health;

            std::string name;
            int lvl;
            statBlock stats;
      public:
            Mob() {
                  health = 0;
                  name = "Undefined";
                  lvl = 0;
                  stats = statBlock(0, 0, 0, 0, 0);
            }

            Mob(std::string _name, int _lvl, statBlock _stats) {
                  name = _name;
                  lvl = _lvl;
                  stats = _stats;
                  health = stats.health;
            }

            int getHealth() {
                  return health;
            }

            void setHealth(int value) {
                  health = value;
            }

            int getLvl() {
                  return lvl;
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