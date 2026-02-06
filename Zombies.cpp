#include "Zombies.h"
#include "Game.h"
#include "constants.h"

Zombie::Zombie(int r, int c, int hp, std::string name, std::string sym)
    : GameObject(r, c, hp, name, sym) {}

// The "Standard" Behavior
void Zombie::act(Game& game) {
    GameObject* object = game.getObjectAt(row, col - 1);
    if(object){
        std::string symbol = object->getSymbol();
        if(symbol == "S" || symbol == "P" || symbol == "W") {// plant? eat it
            object->takeDamage(ZOMBIE_DMG);
            // Cleanup phase will handle deletion of dead objects
        }
        return; // if it has any object to its left, this zombie don't move
    }

    game.moveObject(this, row, col - 1);
}

BucketHeadZombie::BucketHeadZombie(int r, int c)
    : Zombie(r, c, 3 * BASIC_ZOMBIE_HP, "BucketHead", "B") {
        // we call Zombie() with 3*hp and leave everything else the same
}
void BucketHeadZombie::act(Game& game) {
    Zombie::act(game);// we call the default behavior
}

PoleVaultZombie::PoleVaultZombie(int r, int c)
    : Zombie(r, c, BASIC_ZOMBIE_HP, "PoleVault", "V") {
}

void PoleVaultZombie::act(Game& game) {// so many edge cases here
   // PoleVault is special, first it checks if a plant is one tile to its left 
    GameObject* left1 = game.getObjectAt(row, col - 1);
    if (left1 && (left1->getSymbol() == "S" || left1->getSymbol() == "P" || left1->getSymbol() == "W")) {
        // it can jump if jumped is false and it PV is not in col 1 AAANND if left2 is empty
        if(this->col != 1 && !this->Jumped && game.getObjectAt(row, col - 2) == nullptr){
            // it can jump
            game.moveObject(this, row, col - 2);
            this->Jumped = true;
        }
        return;
    }
    // if the jumping condition is not met(for any reason), it will act like a normal zombie
    Zombie::act(game);
}