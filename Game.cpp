#include "Game.h"
#include "Plants.h"
#include "Zombies.h"
#include "constants.h"
#include <fstream>
#include <iostream>

/* totally not PVZ game
- Seems like creative liberty when main calls here with invalid planting
commands, I cout a msg and abort the planting attempt (keep the game running)
- Gameover logic: Loss condition handled in moveObject() when zombie reaches column 0
- Win condition handled in nextTurn() when spawn queue is empty and no zombies remain
*/

Game::Game() {
  // Initialize necessary game variables
  // Initialize the board with nullptrs
  board.resize(BOARD_ROWS, std::vector<GameObject *>(BOARD_COLS, nullptr));
  sun = INITIAL_SUN;
  turn = 0;
  gameOver = false;
}

Game::~Game() {
  // Free all memory in the board vector to prevent leaks
  for (int r = 0; r < BOARD_ROWS; r++) {
    for (int c = 0; c < BOARD_COLS; c++) {
      if (board[r][c] != nullptr) {
        delete board[r][c]; // calling delete on allocated GameObjects
      }
    }
  }
  // spawnQueue cleans itself up
}

void Game::loadLevel(std::string filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open level file " << filename << std::endl;
    return;
  }

  std::string line;
  while (file >> line) {
    // we can use >> like this here as long as we consume single lines properly
    if (line == "WAVE") {
      int turn, row;
      std::string zombieType;
      file >> turn >> zombieType >> row;
      Wave thisWave = {turn, zombieType, row};
      spawnQueue.push_back(thisWave);

    } else if (line == "SUN") {
      // Changes default starting Sun of player
      int cheatSun;
      file >> cheatSun;
      sun = cheatSun;
    }
  }
  std::cout << "Level loaded. Spawn queue size: " << spawnQueue.size()
            << std::endl;
} // this reads in the level file, we trust the file writer to form the file

void Game::nextTurn() {
  turn++;

  // 1. Spawn Phase: Check spawnQueue and spawn all eligible zombies
  // Use while loop to spawn all waves that should spawn on or before this turn
  while (!spawnQueue.empty()) {
    Wave currentWave = spawnQueue.front();
    if (currentWave.turn <= turn) {
      // validate row is in bounds and spawn location is empty
      if (isValid(currentWave.row, 9) && getObjectAt(currentWave.row, 9) == nullptr) {
        spawnZombie(currentWave.zombieType, currentWave.row);
        spawnQueue.pop_front();
      } else if (!isValid(currentWave.row, 9)) {
        // invalid spawn location in level file, skip this wave
        std::cerr << "Warning: Invalid spawn row " << currentWave.row << " in level file. Skipping wave." << std::endl;
        spawnQueue.pop_front();
      } else {
        // spawn location occupied, wait for next turn
        break;
      }
    } else {
      // this wave isn't ready yet
      break;
    }
  }

  // 2. Action Phase: Iterate through board -> call act() on every object
  for (int r = 0; r < BOARD_ROWS; r++) {
    for (int c = 0; c < BOARD_COLS; c++) {
      GameObject *obj = getObjectAt(r, c);
      if (obj != nullptr) {
        obj->act(*this);
      }
    }
  }

  // 3. Cleanup Phase: Remove dead objects (HP <= 0)
  for (int r = 0; r < BOARD_ROWS; r++) {
    for (int c = 0; c < BOARD_COLS; c++) {
      GameObject *obj = getObjectAt(r, c);
      if (obj != nullptr && obj->getHP() <= 0) {
        delete obj;
        board[r][c] = nullptr;
      }
    }
  }

  // 4. Check win condition: no more zombies on board and no more spawns
  int zombieCount = 0;
  for (int r = 0; r < BOARD_ROWS; r++) {
    for (int c = 0; c < BOARD_COLS; c++) {
      GameObject *obj = getObjectAt(r, c);
      if (obj != nullptr) {
        std::string symbol = obj->getSymbol();
        if (symbol == "Z" || symbol == "B" || symbol == "V") {
          zombieCount++;
        }
      }
    }
  }

//   if (!gameOver && spawnQueue.empty() && zombieCount == 0) {
//     gameOver = true;
//   }
}

void Game::plant(std::string type, int r, int c) {
  // Planting logic implementation:
  // 1. Check if 'c' is the Spawn Zone (Column 9). If so, print error and return.
  if (c == 9) {
    std::cout << "Cannot plant in zombie spawn zone!" << std::endl;
    return;
  }

  // 2. Check if coordinates (r, c) are valid.
  // 3. Check if the tile is empty.
  if (!isValid(r, c) || getObjectAt(r, c) != nullptr) {
    std::cout << "Invalid placement!" << std::endl;
    return;
  }

  if (type == "Sunflower" || type == "SunFlower") {
    if (sun < SUNFLOWER_COST) {
      std::cout << "Not enough sun!"
                << std::endl;
      return;
    }
    Sunflower *sunflower = new Sunflower(r, c);
    board[r][c] = sunflower;
    sun -= SUNFLOWER_COST;
  } else if (type == "Peashooter" || type == "PeaShooter") {
    if (sun < PEASHOOTER_COST) {
      std::cout << "Not enough sun!"
                << std::endl;
      return;
    }
    Peashooter *peashooter = new Peashooter(r, c);
    board[r][c] = peashooter;
    sun -= PEASHOOTER_COST;
  } else if (type == "WallNut" || type == "WallNut") {
    if (sun < WALLNUT_COST) {
      std::cout << "Not enough sun!"
                << std::endl;
      return;
    }
    WallNut *wallNut = new WallNut(r, c);
    board[r][c] = wallNut;
    sun -= WALLNUT_COST;
  } else {
    std::cout << "Unknown plant!" << std::endl;
  }
}

void Game::moveObject(GameObject *obj, int newR, int newC) {
  // Move object to new position (primarily used for zombies)
  // Zombie class ensures this is only called with a legal move
  // Also checks if zombie reaches column 0 to trigger game over

  if (!isValid(newR, newC)) {
    std::cerr << "Error: Cannot move object to invalid position (" << newR << ", " << newC << ")" << std::endl;
    return;
  }

  int oldR = obj->getRow();
  int oldC = obj->getCol();
  board[oldR][oldC] = nullptr;
  obj->setPosition(newR, newC);
  board[newR][newC] = obj;

  if (obj->getSymbol() == "Z" || obj->getSymbol() == "B" ||
      obj->getSymbol() == "V") {
    // if it's a zombie class, we check if it's in end-game zone
    if (newC == 0) {
      gameOver = true;
    }
  }
}

// Spawn zombie implementation
void Game::spawnZombie(std::string type, int row) {
  // Double-check bounds for safety
  if (!isValid(row, 9)) {
    std::cerr << "Error: Cannot spawn zombie at invalid position (" << row << ", 9)" << std::endl;
    return;
  }

  if (type == "Zombie") {
    Zombie *zombie = new Zombie(row, 9, BASIC_ZOMBIE_HP, "Zombie", "Z");
    board[row][9] = zombie;
  } else if (type == "BucketHeadZombie" || type == "BucketHead") {
    BucketHeadZombie *bucketHeadZombie = new BucketHeadZombie(row, 9);
    board[row][9] = bucketHeadZombie;
  } else if (type == "PoleVaultZombie" || type == "PoleVault") {
    PoleVaultZombie *poleVaultZombie = new PoleVaultZombie(row, 9);
    board[row][9] = poleVaultZombie;
  } else {
    std::cerr << "Warning: Unknown zombie type '" << type << "'" << std::endl;
  }
}

GameObject *Game::getObjectAt(int r, int c) {
  if (!isValid(r, c))
    return nullptr;
  return board[r][c];
}

bool Game::isValid(int r, int c) {
  return r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS;
}

void Game::addSun(int amount) { sun += amount; }

// Helper: Simple ASCII Draw: Provided for Autograder - DO NOT EDIT
void Game::draw() {
  std::cout << "Sun: " << sun << " | Turn: " << turn << std::endl;
  std::cout << "   0  1  2  3  4  5  6  7  8  9 " << std::endl;
  for (int r = 0; r < BOARD_ROWS; r++) {
    std::cout << r << " ";
    for (int c = 0; c < BOARD_COLS; c++) {
      std::cout << "[";
      if (board[r][c])
        std::cout << board[r][c]->getSymbol();
      else
        std::cout << " ";
      std::cout << "]";
    }
    std::cout << std::endl;
  }
}
void Game::debugPrint() {
  for (int r = 0; r < BOARD_ROWS; r++) {
    for (int c = 0; c < BOARD_COLS; c++) {
      GameObject *obj = board[r][c];
      if (obj != nullptr) {
        std::cout << r << " " << c << " " << obj->getName() << " "
                  << obj->getHP() << std::endl;
      }
    }
  }
  std::cout << "Sun: " << sun << std::endl;
  std::cout << "Turn: " << turn << std::endl;
}
