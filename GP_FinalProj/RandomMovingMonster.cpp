#include "RandomMovingMonster.h"
#include <cstdlib>

RandomMovingMonster::RandomMovingMonster(int x, int y) : Monster(x, y) {}

void RandomMovingMonster::Update(int playerX, int playerY) {
    // ������ �������� ������
    int direction = rand() % 4;
    switch (direction) {
    case 0: x++; break; // ������
    case 1: x--; break; // ����
    case 2: y++; break; // �Ʒ���
    case 3: y--; break; // ����
    }
}
