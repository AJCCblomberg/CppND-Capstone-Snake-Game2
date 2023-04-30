#include <iostream>
#include "renderer.h"
#include "controller.h"
#include "player.h"
#include "game.h"

#include <string>
#include <vector>

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Player player;
  Controller controller;
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Game game(kGridWidth, kGridHeight);

  // CP1. Ask players nickname
  // Created Player class to set and get the players name which is used by other functions to personalize the game.
  player.SetName();

  // CP2. Welcome message
  // Created a WelcomeMessage() function.
  game.WelcomeMessage(player, renderer);

  // CP3. Read results
  // Created a ReadResults() function to read the previous top 3 results from the results.txt file.
  std::vector<results> gameResults = game.ReadResults();

  // CP4. Show best results
  // Created a ShowResults() function to show the previous best result.
  game.ShowResults(gameResults, renderer);

  // CP5. Run game
  // Added the player to the Run() function to show the players name in the window title
  game.Run(player, controller, renderer, kMsPerFrame);

  // CP6. Show player results
  // Created the PlayerResults() function to obtain the player results in a struct
  // Overloaded the ShowResults() function to show the player results.
  results playerResults = game.PlayerResults(player);
  game.ShowResults(playerResults, renderer);

  // CP7. Congratulate if in top 3
  // Created the UpdateResults() function to check if player made the top 3 and to create the vector struct which is used by the WriteResults() function.
  std::vector<results> updatedGameResults = game.UpdateResults(gameResults, playerResults, renderer);

  // CP8. Write results
  // Created the WriteResults() function to write the top 3 results to the results.txt file.
  game.WriteResults(updatedGameResults);

  std::cout << "Game has terminated successfully!\n";

  return 0;
}