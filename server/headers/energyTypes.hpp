#pragma once

#include <cstdint>

typedef uint_fast16_t energyType;

class energyBlock {
      private:
            energyType maxMana;
            energyType mana;

            energyType maxEnergy;
            energyType energy;

      public:
            energyBlock() {
                  maxMana = 0;
                  mana = 0;
                  maxEnergy = 0;
                  energy = 0;
            }

            energyBlock(energyType _maxMana, energyType _maxEnergy) {
                  maxMana = _maxMana;
                  mana = _maxMana;
                  maxEnergy = _maxEnergy;
                  energy = _maxEnergy;
            }

            energyType getMana() {
                  return mana;
            }

            energyType getMaxMana() {
                  return maxMana;
            }

            energyType getEnergy() {
                  return energy;
            }

            energyType getMaxEnergy() {
                  return maxEnergy;
            }

            void setMaxMana(energyType value) {
                  maxMana = value;
            }

            void setMaxEnergy(energyType value) {
                  maxEnergy = value;
            }

            void setMana(energyType value) {
                  if(value > maxMana) {
                        mana = maxMana;
                        return;
                  }

                  mana = value;
            }

            void setEnergy(energyType value) {
                  if(value > maxEnergy) {
                        energy = maxEnergy;
                        return;
                  }

                  energy = value;
            }
};