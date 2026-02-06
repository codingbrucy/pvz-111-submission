#include "Plants.h"
#include "Game.h"   
#include "constants.h"

// 🌸plants
/*
- Game will check for sun and tile before planting, so constructors will only plant.
*/
Plant::Plant(int r, int c, int hp, int cost, std::string name, std::string sym)
    : GameObject(r, c, hp, name, sym), cost(cost) {}


Sunflower::Sunflower(int r, int c)
    : Plant(r, c, DEFAULT_PLANT_HP, SUNFLOWER_COST, "Sunflower", "S") {
    cooldown = 0;
}

void Sunflower::act(Game& game) {
    cooldown++;
    if (cooldown >= SUNFLOWER_COOLDOWN) {

        cooldown = 0;
        game.addSun(SUNFLOWER_INCOME);
    }
}

Peashooter::Peashooter(int r, int c)
    : Plant(r, c, DEFAULT_PLANT_HP, PEASHOOTER_COST, "Peashooter", "P") {
}

void Peashooter::act(Game& game) {
    // scan its row left to right for a zombie
    // it only shoots to the right
    for(int c = col + 1; c < BOARD_COLS; c++){
        GameObject* object = game.getObjectAt(row, c);
        if(object && (object->getSymbol() == "Z" || object->getSymbol() == "B" || object->getSymbol() == "V")){
            object->takeDamage(PEASHOOTER_DMG);
            // Cleanup phase will handle deletion of dead objects
            return;
        }
    }
    // if no zombie is found, do nothing
    return;
}

WallNut::WallNut(int r, int c)
    : Plant(r, c, WALLNUT_HP, WALLNUT_COST, "WallNut", "W") {
}

void WallNut::act(Game& game) {
    // do nothing
    (void)game;
    return;
}