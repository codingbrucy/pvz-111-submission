#ifndef ZOMBIES_H
#define ZOMBIES_H

#include "GameObject.h"
// 🧟‍♂️ Zombie 🧟‍♂️
/*
- Special zombie constructor only takes row,col. (hp,name and symbol are handled internally)
- Normal zombie constructor takes row,col,hp,name and symbol.
- BucketHeadZombie is a normal zombie with 3x hp
- PoleVaultZombie is a normal zombie when jump condition is not met (which is rigorously checked in PoleVaultZombie::act())
- Plant damage is applied in Zombie::act(), but deletion of dead objects is handled in Game::nextTurn() cleanup phase
- Collision is handled in Zombie::act() by checking if there's an object to the left (zombie or plant)
*/
class Zombie : public GameObject {
public:
    Zombie(int r, int c, int hp, std::string name, std::string sym);
    virtual void act(Game& game) override;
};

class BucketHeadZombie : public Zombie {
public:
    BucketHeadZombie(int r, int c);
    void act(Game& game) override;
};

class PoleVaultZombie : public Zombie {
public:
    bool Jumped = false;
    PoleVaultZombie(int r, int c);
    void act(Game& game) override;
    
};
 
#endif