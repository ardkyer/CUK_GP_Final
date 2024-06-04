#include "Tutorial.h"
#include "MovingMonster.h"
#include "RangedMonster.h"
#include "ChargingMonster.h"
#include <SDL_image.h>
#include <random>

Tutorial::Tutorial()
    : Map("../../Resource/Map/Tutorial.png", 3) {  // maxMonsters ����
    spawnDelay = 1.0f;  // �ʱ� ���� ���� ���� �ð� ����
}

void Tutorial::SpawnMonsters() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, WINDOW_WIDTH - 128);
    std::uniform_int_distribution<> disY(0, WINDOW_HEIGHT - 128);

    for (int i = 0; i < 1; i++) {
        int x = disX(gen);
        int y = disY(gen);
        monsters.push_back(new MovingMonster(x, y));
    }
    for (int i = 0; i < 1; i++) {
        int x = disX(gen);
        int y = disY(gen);
        monsters.push_back(new RangedMonster(x, y));
    }
    for (int i = 0; i < 1; i++) {
        int x = disX(gen);
        int y = disY(gen);
        monsters.push_back(new ChargingMonster(x, y));
    }

}

void Tutorial::SpawnMonster() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, WINDOW_WIDTH - 128);
    std::uniform_int_distribution<> disY(0, WINDOW_HEIGHT - 128);

    if (monsters.size() < maxMonsters) {
        int x = disX(gen);
        int y = disY(gen);
        if (monsters.size() % 2 == 0) {
            monsters.push_back(new MovingMonster(x, y));
        }
        else {
            monsters.push_back(new RangedMonster(x, y));
        }
    }
}

void Tutorial::HandleEvent(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_c) {
            // CŰ�� ������ Entrance ������ �̵�
            g_current_game_phase = PHASE_Entrance;
        }
    }
}
