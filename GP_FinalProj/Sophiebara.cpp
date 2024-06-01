#include "Sophiebara.h"
#include <random>

Sophiebara::Sophiebara()
    : Map("../../Resource/Map/Sophiebara.png") {
    // �������� �ʱ� ���� �ÿ��� ���͸� �������� ����
}

void Sophiebara::SpawnMonsters() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, WINDOW_WIDTH - 128);
    std::uniform_int_distribution<> disY(0, WINDOW_HEIGHT - 128);

    for (int i = 0; i < 5; ++i) {
        int x = disX(gen);
        int y = disY(gen);
        monsters.push_back(new MovingMonster(x, y));
    }
}
