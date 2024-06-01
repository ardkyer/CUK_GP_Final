#ifndef SOPHIEBARA_H
#define SOPHIEBARA_H

#include "Map.h"
#include "MovingMonster.h"

class Sophiebara : public Map {
public:
    Sophiebara();
    void SpawnMonsters() override;
};

#endif // SOPHIEBARA_H
