#include "player.h"
#include <iostream>


// constructor
Player::Player() : _name("NA") {}
Player::Player(std::string name) : _name(name) {}

// getter/setter
std::string Player::GetName() const {
    return _name;
}

// CP1. Ask players nickname
void Player::SetName() {
    std::string name;
    std::cout << "Please enter nickname: " << std::endl;
    std::cin >> name;
    _name = name;
}