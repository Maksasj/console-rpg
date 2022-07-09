#pragma once

#include <string>

#include "server_networking.hpp"
#include "playerClass.hpp"
#include "gameLocation.hpp"

struct serverDataSender {
      serverDataSender() {}

      void commandUpdateStats(SOCKET currSocketFd, Player p) {
            std::string message = "updatestats:"
                  +std::to_string(p.getHp())+","
                  +std::to_string(p.getArm())+","
                  +std::to_string(p.getAgi())+","
                  +std::to_string(p.getStr())+","
                  +std::to_string(p.getInt())+"*";
            send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
      }

      void commandServerName(SOCKET currSocketFd) {
            std::string message = "servername:Ursina*";
            send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
      }

      void commandPlayersOnline(SOCKET currSocketFd, std::map<SOCKET, Player> _player_pool) {
            std::string message = "playersonline:"+std::to_string(_player_pool.size())+"*";
            send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
      }

      void commandPlayerName(SOCKET currSocketFd, Player p)  {
            std::string message = "playername:"+p.getName()+"*";
            send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
      }

      void commandPlayerLvl(SOCKET currSocketFd, Player p)  {
            std::string message = "playerlvl:"+std::to_string(p.getLvl())+"*";
            send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
      }

      void commandPlayerExp(SOCKET currSocketFd, Player p)  {
            std::string message = "playerexp:"+std::to_string(p.getExp())+","+std::to_string(p.getExpToNextLvl())+"*";
            send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
      }

      void commandPlayerClassName(SOCKET currSocketFd, Player p)  {
            std::string message = "classname:"+p.getClassName()+"*";
            send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
      }

      void commandEnergyBar(SOCKET currSocketFd, Player p)  {
            std::string message = "updateenergy:"
                  +std::to_string(p.getEnergyBlock().getMana())+","
                  +std::to_string(p.getEnergyBlock().getMaxMana())+","
                  +std::to_string(p.getEnergyBlock().getEnergy())+","
                  +std::to_string(p.getEnergyBlock().getMaxEnergy())+"*";
            send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
      }

      void commandPlayerGold(SOCKET currSocketFd, Player p)  {
            std::string message = "updategold:"+std::to_string(p.getGold())+"*";
            send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
      }

      void commandPlayerHealth(SOCKET currSocketFd, Player p)  {
            std::string message = "updatehealth:"
                  +std::to_string(p.getHealth())+","
                  +std::to_string(p.getMaxHealth())+"*";
            send(currSocketFd, message.c_str(), strlen(message.c_str()), 0);
      }

      void commandPlayerLocation(SOCKET currSocketFd, Player p, std::map<std::string, gameLocation> gameWorld) {
            std::string messageLocData = "location:"+p.getLocation();

            int n = 1;
            for(auto mob : gameWorld[p.getLocation()].getMobs()) {
                  messageLocData += ","+std::to_string(n)+"."+mob.getName()+"("+std::to_string(mob.getLvl())+"lvl)";
                  n++;
            }

            messageLocData += "*";
            send(currSocketFd, messageLocData.c_str(), strlen(messageLocData.c_str()), 0);
      }
};