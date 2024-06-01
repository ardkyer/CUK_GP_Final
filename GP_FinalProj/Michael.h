#ifndef MICHAEL_H
#define MICHAEL_H

#include "Map.h"
#include "MovingMonster.h"

class Michael : public Map {
public:
    Michael();
    void SpawnMonsters() override;
};

#endif // MICHAEL_H
