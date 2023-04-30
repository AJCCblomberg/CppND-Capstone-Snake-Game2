#include "game.h"
#include <iostream>
#include "SDL.h"
#include <fstream>



Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Player &player, Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  bool paused = false; // ADDED

  while (running) {


    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      //renderer.UpdateWindowTitle(score, frame_count);
      std::string name = player.GetName();
      renderer.UpdateWindowTitle(name, score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }

  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }


// CP2. Welcome message
void Game::WelcomeMessage(Player &player, Renderer &renderer) {
  std::string namestr = "Welcome " + player.GetName() + " and good luck!";

  const char *namechr = namestr.c_str();
  const char *titlechr = "Snake Game";

  renderer.MessageBox(titlechr, namechr);
}

// CP3. Read results
std::vector<results> Game::ReadResults() {
  std::string line;

  results playerResults;

  std::ifstream filestream(resultsFileName);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      linestream >> playerResults.name >> playerResults.score >> playerResults.size;
      resultsvec.push_back(playerResults);
    }
  }
  if (resultsvec.empty()) {
      playerResults.name = "NA";
      playerResults.score = "0";
      playerResults.size = "1";
      resultsvec.push_back(playerResults);
    }

  return resultsvec;
}

// CP4. Show best results
void Game::ShowResults(std::vector<results> &resultsvec, Renderer &renderer) {
  const std::string messagestr = "Player: " + resultsvec[0].name + "\n" 
                               + "Score: " + resultsvec[0].score + "\n"
                               + "Size: " + resultsvec[0].size;

  const char *messagechr = messagestr.c_str();
  const char *titlechr = "Previous best result";

  renderer.MessageBox(titlechr, messagechr);
}

// CP6. Show player results
void Game::ShowResults(results &playerResults, Renderer &renderer) {
  const std::string messagestr = "Player: " + playerResults.name + "\n" 
                               + "Score: " + playerResults.score + "\n"
                               + "Size: " + playerResults.size;

  const char *messagechr = messagestr.c_str();
  const char *titlechr = "Player result";

  renderer.MessageBox(titlechr, messagechr);
}

// CP6. Show player results
// The PlayerResults() function creates the results struct based on the results of the player.
results Game::PlayerResults(Player &player) {
    results playerResults;

    playerResults.name = player.GetName();
    playerResults.score = std::to_string(score);
    playerResults.size = std::to_string(snake.size);

    return playerResults;
}


// CP7. Congratulate if in top 3
// Lambda function
bool compareScore(const results &a, const results &b) {
    return std::stoi(a.score) > std::stoi(b.score);
}

// CP7. Congratulate if in top 3
// The SortResults() function sorts the previous and player results.
// The sort function uses the lambda function.
std::vector<results> Game::SortResults(std::vector<results> &sortResults) {
  std::sort(sortResults.begin(), sortResults.end(), compareScore);

  return sortResults;
}

// CP7. Congratulate if in top 3
// The UpdateResults() function checks if the player made the top 3 and it creates the vector struct which is used by the WriteResults() function.
// It also uses the SortResults() function to make sure that the top 3 is correct after the player has played the game.
std::vector<results> Game::UpdateResults(std::vector<results> &gameResults, results &playerResults, Renderer &renderer) {

  std::string messagestr;

  int i = 0;
  for (auto result : gameResults) {
    if (playerResults.score >= result.score && i == 0) {
      messagestr = "Perfect score you have won the 1st price";
      if (gameResults.size() == 3) {
        gameResults.pop_back();
      }
      gameResults.push_back(playerResults);
      gameResults = SortResults(gameResults);
      break;
    } else if (playerResults.score >= result.score && i == 1) {
      messagestr = "Congratulations you are 2nd! Now lets try to get 1st.";
      if (gameResults.size() == 3) {
        gameResults.pop_back();
      }
      gameResults.push_back(playerResults);
      gameResults = SortResults(gameResults);
      break;
    } else if (playerResults.score >= result.score && i == 2) {
      messagestr = "Congratulations you are 3rd!";
      if (gameResults.size() == 3) {
        gameResults.pop_back();
      }
      gameResults.push_back(playerResults);
      gameResults = SortResults(gameResults);
      break;
    } else if (playerResults.score < result.score && i == 2) {
      messagestr = "Too bad you didn't made the top 3, better luck next time!";
    }
    ++i;
  }

  const char *messagechr = messagestr.c_str();
  const char *titlechr = "The End";

  renderer.MessageBox(titlechr, messagechr);

  return gameResults;
}

// CP8. Write results
// Function to write the top 3 results to the results.txt file.
void Game::WriteResults(std::vector<results> &resultsvec) {

  std::ofstream resultsfile;
  resultsfile.open(resultsFileName);
  for (auto player : resultsvec) {
    resultsfile << player.name << " " << player.score << " " << player.size << std::endl;
  }

  resultsfile.close();
}