#ifndef ZOMBIES_H
#define ZOMBIES_H

#include "GameObject.h"
// 🧟‍♂️ Zombie 🧟‍♂️
/*
- special zombie constructor only takes row,col. (hp,name and symbol are handled internally)
- Normal zombie constructor takes row,col,hp,name and symbol.
- BucketHeadZombie is a normal zombie with 3x hp
- PoleVaultZombie is a normal zombie when jump condition is not met (which is rigorously checked in PoleVaultZombie::act())
- plant dying logic is handled in Zombie::act(), which is funky to me
- collision is handled partialy in Zombie::act (by assuming if a zombie is on zombie's left, it is a collision),
                         and partialy in Game::nextTurn(), with left to right iteration
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