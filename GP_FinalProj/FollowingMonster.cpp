#include "FollowingMonster.h"
#include <cmath>

FollowingMonster::FollowingMonster(int x, int y) : Monster(x, y) {}

void FollowingMonster::Update(int playerX, int playerY) {
    // ������ ���� �˰���
    if (playerX > x) x++;
    else if (playerX < x) x--;

    if (playerY > y) y++;
    else if (playerY < y) y--;
}
