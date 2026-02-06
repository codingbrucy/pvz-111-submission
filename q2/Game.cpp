#include "Game.h"
#include "Plants.h"
#include "Zombies.h"
#include "constants.h"
#include <iostream>
#include <fstream>

/* totally not PVZ game
- Seems like creative liberty when main calls here with invalid planting commands, I cout a msg and abort the planting attempt (keep the game running)
- Gameover logic is handled in both Zombie::act() and PoleVaultZombie::act()
*/

Game::Game() {
    // TODO: Initialize necessary game variables
    // Initialize the board with nullptrs
    board.resize(BOARD_ROWS, std::vector<GameObject*>(BOARD_COLS, nullptr));
    sun = INITIAL_SUN;
    turn = 0;
    gameOver = false;
    
}

Game::~Game() {
    // TODO: Free all memory in the board vector to prevent leaks!
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
            
            // TODO: changes default starting Sun of player
            int cheatSun;
            file >> cheatSun;
            sun = cheatSun;
        }
    }
    std::cout << "Level loaded. Spawn queue size: " << spawnQueue.size() << std::endl;
}// this reads in the level file, we trust the file writer to form the file

void Game::nextTurn() {
    turn++;
    std::cout << "Processing Turn " << turn << "..." << std::endl;
    
    // TODO:
    // 1. Check spawnQueue. If turn matches, spawn zombie.
    if(!spawnQueue.empty()){
        Wave currentWave = spawnQueue.front();
        if(currentWave.turn >= turn){
            // we should attempt a spawn
            if(getObjectAt(currentWave.row, 9) == nullptr){
                spawnZombie(currentWave.zombieType, currentWave.row);
                spawnQueue.pop_front();
            }
            //else we wait
        }
        // if no more spawn we don't spawn
    }
    
    // 2. Iterate through board -> call act() on every object.
    int zombieCount = 0;
    for(int r = 0; r < BOARD_ROWS; r++){
        for(int c = 0; c < BOARD_COLS; c++){
            GameObject* obj = getObjectAt(r, c);
            if(obj != nullptr){
                obj->act(*this);
                // obj will either be the same object or null. so we don't call getObjectAt() again
                if(obj != nullptr){
                    std::string symbol = obj->getSymbol();
                    if(symbol == "Z" || symbol == "B" || symbol == "V"){
                        zombieCount++;
                    }
                }
            }
        }
    }
    // 3. Handle collisions (if act() doesn't do it) , ✌🏻 act() does it all
    // 4. Cleanup dead objects (hp <= 0). ✌🏻 act() does it all
    // also, by handling death logic in act(), if a plant dies, PV can jump there, this is kinda important and I don't know how graders eval this
    
    // 5. Check win condition: if spawn queue is empty and no zombies remain, player wins
    if (!gameOver && spawnQueue.empty() && zombieCount == 0) {
        gameOver = true;
    }
}

void Game::plant(std::string type, int r, int c) {
    // TODO: Implement the planting logic
    // 1. Check if coordinates (r, c) are valid.
    // 2. Check if the tile is empty.
    // 3. Check if 'c' is the Spawn Zone (Column 9). If so, print error and return.
    if(!isValid(r, c)|| getObjectAt(r, c) != nullptr || c==9 ){
        std::cout << "Invalid planting command! Nothing is planted." << std::endl;
        return;
    }
    if(type == "Sunflower" || type=="SunFlower"){
        if (sun < SUNFLOWER_COST){
            std::cout << "Not enough sun to plant Sunflower! Nothing is planted." << std::endl;
            return;
        }
        Sunflower* sunflower = new Sunflower(r, c);
        board[r][c] = sunflower;
        sun -= SUNFLOWER_COST;
    }
    else if(type == "Peashooter" || type=="PeaShooter"){
        if (sun < PEASHOOTER_COST){
            std::cout << "Not enough sun to plant Peashooter! Nothing is planted." << std::endl;
            return;
        }
        Peashooter* peashooter = new Peashooter(r, c);
        board[r][c] = peashooter;
        sun -= PEASHOOTER_COST;
    }
    else if(type == "WallNut" || type=="WallNut"){
        if (sun < WALLNUT_COST){
            std::cout << "Not enough sun to plant WallNut! Nothing is planted." << std::endl;
            return;
        }
        WallNut* wallNut = new WallNut(r, c);
        board[r][c] = wallNut;
        sun -= WALLNUT_COST;
    }

}

void Game::moveObject(GameObject* obj, int newR, int newC) {
    // move Object might as well be called moveZombie, because we don't have a shovel
    // I have made sure in zombie class that this will only be called with a legal move
    int oldR = obj->getRow();
    int oldC = obj->getCol();
    board[oldR][oldC] = nullptr;
    obj->setPosition(newR, newC);
    board[newR][newC] = obj;
    if(obj->getSymbol() == "Z" || obj->getSymbol() == "B" || obj->getSymbol() == "V") {
        // if it's a zombie class, we check if it's in end-game zone
        if(newC == 0){
            gameOver = true;
        }
    }
}


// TODO: Implement spawnZombie too
void Game::spawnZombie(std::string type, int row) {
    // we make sure in nextTurn() that the spawn is legal, so we don't need to check here
    if(type == "Zombie"){
        Zombie* zombie = new Zombie(row, 9, BASIC_ZOMBIE_HP, "Zombie", "Z");
        board[row][9] = zombie;
    }
    else if(type == "BucketHeadZombie"|| type == "BucketHead"){
        BucketHeadZombie* bucketHeadZombie = new BucketHeadZombie(row,9);
        board[row][9] = bucketHeadZombie;
    }
    else if(type == "PoleVaultZombie" || type =="PoleVault"){
        PoleVaultZombie* poleVaultZombie = new PoleVaultZombie(row,9);
        board[row][9] = poleVaultZombie;
    }
}

GameObject* Game::getObjectAt(int r, int c) {
    if (!isValid(r, c)) return nullptr;
    return board[r][c];
}

bool Game::isValid(int r, int c) {
    return r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS;
}

void Game::addSun(int amount) {
    sun += amount;
}

void Game::removeObjectAt(int r, int c) {
    if (!isValid(r, c)) return;
    if (board[r][c] == nullptr) return;
    
    delete board[r][c];
    board[r][c] = nullptr;
}
// Helper: Simple ASCII Draw: Provided for Autograder - DO NOT EDIT
void Game::draw() {
    std::cout << "Sun: " << sun << " | Turn: " << turn << std::endl;
    std::cout << "   0  1  2  3  4  5  6  7  8  9 " << std::endl;
    for (int r = 0; r < BOARD_ROWS; r++) {
        std::cout << r << " ";
        for (int c = 0; c < BOARD_COLS; c++) {
            std::cout << "[";
            if (board[r][c]) std::cout << board[r][c]->getSymbol();
            else std::cout << " ";
            std::cout << "]";
        }
        std::cout << std::endl;
    }
}
void Game::debugPrint() {
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            GameObject* obj = board[r][c];
            if (obj != nullptr) {
                std::cout << r << " " << c << " " 
                          << obj->getName() << " " 
                          << obj->getHP() << std::endl;
            }
        }
    }
    std::cout << "Sun: " << sun << std::endl;
    std::cout << "Turn: " << turn << std::endl;
}
