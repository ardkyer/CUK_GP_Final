#include "Map.h"

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
    }
    monsters.clear();
}

void Map::Update(float deltaTime) {
    player_.Update(deltaTime);

    // 몬스터 생성 타이머 업데이트
    if (!monstersSpawned) {
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnDelay) {
            SpawnMonsters();  // 몬스터 생성
            spawnTimer = 0;  // 타이머 리셋
            monstersSpawned = true;
        }
    }

    bool allMonstersDefeated = !monsters.empty();
    for (auto it = monsters.begin(); it != monsters.end();) {
        (*it)->Update(deltaTime, player_.GetRect());
        if ((*it)->CheckCollisionWithPlayer(player_.GetRect())) {
            delete* it;
            it = monsters.erase(it);
        }
        else {
            ++it;
            allMonstersDefeated = false;
        }
    }

    if (allMonstersDefeated && monsters.empty()) {
        g_phase_transition_timer = 2.0f; // 단계 전환 타이머 시작
    }
}

void Map::Render() {
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    SDL_RenderClear(g_renderer);
    SDL_RenderCopy(g_renderer, texture_, NULL, &destination_rectangle_);
    for (auto monster : monsters) {
        monster->Render();
    }
    SDL_RenderPresent(g_renderer);
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
    }
    monsters.clear();
    spawnTimer = 0;  // 스폰 타이머 리셋
    monstersSpawned = false; // 몬스터 생성 여부 리셋
}

void Map::LoadBackground(const char* path) {
    SDL_Surface* temp_surface = IMG_Load(path);
    texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(texture_, NULL, NULL, &source_rectangle_.w, &source_rectangle_.h);
    destination_rectangle_ = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
}
