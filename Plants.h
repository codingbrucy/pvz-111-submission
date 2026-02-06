#ifndef PLANTS_H
#define PLANTS_H

#include "GameObject.h"
#include "constants.h"

// --- Intermediate Base Class ---
class Plant : public GameObject {
protected:
    int cost;

public:
    // Pass 'hp' and 'cost' up to this constructor
    Plant(int r, int c, int hp, int cost, std::string name, std::string sym);
    
    int getCost() const { return cost; }
    
};

// --- Concrete Classes ---

class Sunflower : public Plant {
private:
    int cooldown;
public:
    Sunflower(int r, int c); // Constructor sets cost = 50, hp = 30
    void act(Game& game) override;
};

class Peashooter : public Plant {
public:
    Peashooter(int r, int c); // Constructor sets cost = 100, hp = 100
    void act(Game& game) override;
};

class WallNut : public Plant {
public:
    WallNut(int r, int c); // Constructor sets cost = 50, hp = 300
    // does it act? 
    void act(Game& game) override;
};

#endif