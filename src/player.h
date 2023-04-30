#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

// struct to store players results
struct results {
  std::string name;
  std::string score;
  std::string size;
};

// CP1. Player class to set and get the players name which is used by other functions to personalize the game.
class Player {
 public:
    // constructor
    Player();
    Player(std::string name);

    // getter/setter
    std::string GetName()  const;
    void SetName();
    

 private:
    std::string _name;
};

#endif