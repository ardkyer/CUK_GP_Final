#include "Michael.h"
#include <random>

Michael::Michael()
    : Map("../../Resource/Map/Michael.png") {
    // 스테이지 초기 시작 시에는 몬스터를 생성하지 않음
}

void Michael::SpawnMonsters() {
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
