#pragma once

#include <string>
#include <vector>

#include "mobClass.hpp"

class gameLocation {
      private:
            std::string name;
            std::vector<Mob> mobs;

            std::vector<std::string> nearLocations; //like graph

      public:
            gameLocation() {};

            std::string getName() {
                  return name;
            }

            gameLocation(std::string _name) {
                  name = _name;
            }

            std::vector<Mob> getMobs() {
                  return mobs;
            }

            std::vector<std::string> getNearLoc() {
                  return nearLocations;
            }

            gameLocation addMob(Mob mob) {
                  mobs.push_back(mob);
                  return *this;
            }

            gameLocation addNearLoc(std::string loc) {
                  nearLocations.push_back(loc);
                  return *this;
            }

            gameLocation setName(std::string _name) {
                  name = _name;
                  return *this;
            }
};

gameLocation createGameLocation(std::string name) {
      return gameLocation(name);
}