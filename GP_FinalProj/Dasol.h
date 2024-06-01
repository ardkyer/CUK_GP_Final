#ifndef DASOL_H
#define DASOL_H

#include "Map.h"
#include "MovingMonster.h"

class Dasol : public Map {
public:
    Dasol();
    void SpawnMonsters() override;
};

#endif // DASOL_H
