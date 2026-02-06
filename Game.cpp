#include "Game.h"
#include "Plants.h"
#include "Zombies.h"
#include "constants.h"
#include <iostream>
#include <fstream>
#include <iomanip>

Game::Game() {
    // TODO: Initialize necessary game variables
}

Game::~Game() {
    // TODO: Free all memory in the board vector to prevent leaks!
}

void Game::loadLevel(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open level file " << filename << std::endl;
        return;
    }
    
    std::string line;
    while (file >> line) {
        if (line == "WAVE") {

            // TODO: read and store in spawnQueue;

        } else if (line == "SUN") {
            
            // TODO: changes default starting Sun of player

        }
    }
    std::cout << "Level loaded. Spawn queue size: " << spawnQueue.size() << std::endl;
}

void Game::nextTurn() {
    turn++;
    std::cout << "Processing Turn " << turn << "..." << std::endl;
    
    // TODO:
    // 1. Check spawnQueue. If turn matches, spawn zombie.
    // 2. Iterate through board -> call act() on every object.
    // 3. Handle collisions (if act() doesn't do it).
    // 4. Cleanup dead objects (hp <= 0).
}

void Game::plant(std::string type, int r, int c) {
    // TODO: Implement the planting logic
    // 1. Check if coordinates (r, c) are valid.
    // 2. Check if the tile is empty.
    // 3. Check if 'c' is the Spawn Zone (Column 9). If so, print error and return.

    // 4. Check if player has enough sun.
    // 5. If all good, create the object, deduct sun, and place it on board.
}

// Provided for Autograder - DO NOT EDIT
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

void Game::moveObject(GameObject* obj, int newR, int newC) {

    // TODO: Implement movement object
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


// TODO: Implement other declard functions