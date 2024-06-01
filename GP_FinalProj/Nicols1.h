#ifndef NICOLS1_H
#define NICOLS1_H

#include "Map.h"
#include "MovingMonster.h"

class Nicols1 : public Map {
public:
    Nicols1();
    void SpawnMonsters() override;
};

#endif // NICOLS1_H
