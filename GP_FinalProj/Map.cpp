#include "Map.h"
#include "globals.h" // globals.h 포함
#include "Entrance.h"  // Entrance 클래스 포함
#include "MovingMonster.h"
#include "RangedMonster.h"
#include "ChargingMonster.h"
#include <random>

Map::Map(const char* backgroundPath) : destination_rectangle_{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT }, spawnTimer(0.0f), spawnDelay(5.0f), monstersSpawned(false) {
    LoadBackground(backgroundPath);
}

Map::~Map() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
    for (auto& monster : monsters) {
        delete monster;
        monster = nullptr;
    }
    monsters.clear();
}

void Map::Update(float deltaTime) {
    player_.Update(deltaTime);

    // 몬스터 생성 타이머 업데이트
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnDelay && !monstersSpawned) {
        SpawnMonsters();  // 몬스터 생성
        monstersSpawned = true;  // 몬스터 생성 플래그 설정
    }

    bool allMonstersDefeated = true;
    for (auto it = monsters.begin(); it != monsters.end();) {
        (*it)->Update(deltaTime, player_.GetRect());
        if ((*it)->CheckCollisionWithPlayer(player_.GetRect())) {
            delete* it;
            it = monsters.erase(it);
           
            // 새로운 몬스터 생성 및 추가
            if (monsters.size() < 7) {
                dynamic_cast<Map*>(this)->SpawnMonster();  // 새로운 몬스터 생성
            }
        }
        else {
            ++it;
            allMonstersDefeated = false;
        }
    }

    if (allMonstersDefeated && monsters.empty() && monstersSpawned) {
        g_phase_transition_timer = 2.0f;
    }
}


void Map::Render() {
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    SDL_RenderClear(g_renderer);
    SDL_RenderCopy(g_renderer, texture_, NULL, &destination_rectangle_);
    for (auto monster : monsters) {
        monster->Render();
    }
}

void Map::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            g_flag_running = false;
        }
    }
}

void Map::ResetMonsters() {
    for (auto& monster : monsters) {
        delete monster;
        monster = nullptr;
    }
    monsters.clear();
    monstersSpawned = false;  // 리셋 시 몬스터 생성 플래그 초기화
    spawnTimer = 0.0f;  // 타이머 리셋

    // 플레이어 위치 중앙으로 설정
    player_.SetPosition(WINDOW_WIDTH / 2 - player_.GetRect().w / 2, WINDOW_HEIGHT / 2 - player_.GetRect().h / 2);
}

void Map::LoadBackground(const char* path) {
    SDL_Surface* temp_surface = IMG_Load(path);
    texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(texture_, NULL, NULL, &source_rectangle_.w, &source_rectangle_.h);
    destination_rectangle_ = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
}

void Map::SpawnMonster() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, WINDOW_WIDTH - 128);
    std::uniform_int_distribution<> disY(0, WINDOW_HEIGHT - 128);


    if (g_current_game_phase == PHASE_Hall) {
        if (monsters.size() < 7) {
            int x = disX(gen);
            int y = disY(gen);
            monsters.push_back(new ChargingMonster(x, y));
        }
    }
    else {
        if (monsters.size() < 7) {
            int x = disX(gen);
            int y = disY(gen);
            if (g_kill_count % 3 == 0) {
                monsters.push_back(new RangedMonster(x, y));
            }
            else {
                monsters.push_back(new MovingMonster(x, y));
            }
        }
    }


}
