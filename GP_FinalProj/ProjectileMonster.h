// ProjectileMonster.h
#ifndef PROJECTILEMONSTER_H
#define PROJECTILEMONSTER_H
#include "Monster.h"
class ProjectileMonster : public Monster {
public:
    ProjectileMonster(int x, int y, float velX, float velY);
    void Update(float deltaTime);  // �Լ� ���� ����
    bool IsOutOfBounds() const;

private:
    float velX;
    float velY;
};
#endif // PROJECTILEMONSTER_H