#ifndef GAME_H
#define GAME_H

#include <random>
#include "player.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include "SDL.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  
  void Run(Player &player, Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  
  int GetScore() const;
  int GetSize() const;

  void WelcomeMessage(Player &player, Renderer &renderer);
  std::vector<results> ReadResults();
  void ShowResults(std::vector<results> &resultsvec, Renderer &renderer);
  void ShowResults(results &playerResults, Renderer &renderer);
  results PlayerResults(Player &player);
  std::vector<results> SortResults(std::vector<results> &sortResults);
  std::vector<results> UpdateResults(std::vector<results> &gameResults, results &playerResults, Renderer &renderer);
  void WriteResults(std::vector<results> &resultsvec);

  // CP3/8. Path to results file used by both ReadResults() and WriteResults() function.
  const std::string resultsFileName{"../src/results.txt"};

  std::vector<results> resultsvec;

 private:
  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
};

#endif