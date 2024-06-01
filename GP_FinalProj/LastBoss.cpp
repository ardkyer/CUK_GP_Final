#include "LastBoss.h"
#include "GameClass.h"
#include <SDL_image.h>

LastBoss::LastBoss()
    : Map("../../Resource/Map/LastBoss.png"), showBossIntro(true), bossIntroTotalTimer(4.0f), currentFrame(0), frameDuration(1.0f), frameTimer(0.0f), bossMonster(nullptr) {
    SpawnMonsters();

    // 보스전 인트로 이미지 로드
    for (int i = 1; i <= 4; ++i) {
        std::string path = "../../Resource/Intro/boss_intro_frame" + std::to_string(i) + ".png";
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (surface) {
            bossIntroFrames.push_back(SDL_CreateTextureFromSurface(g_renderer, surface));
            SDL_FreeSurface(surface);
        }
    }
}

LastBoss::~LastBoss() {
    for (auto texture : bossIntroFrames) {
        SDL_DestroyTexture(texture);
    }
    delete bossMonster;
}

void LastBoss::SpawnMonsters() {
    int bossX = (WINDOW_WIDTH - 128) / 2;
    int bossY = (WINDOW_HEIGHT - 128) / 2; // 보스 몬스터 위치 중앙 조정
    bossMonster = new BossMonster(bossX, bossY);
    monsters.push_back(bossMonster);
}

void LastBoss::Update(float deltaTime) {
    if (showBossIntro) {
        bossIntroTotalTimer -= deltaTime;
        frameTimer += deltaTime;
        if (frameTimer >= frameDuration) {
            frameTimer = 0.0f;
            currentFrame++;
            if (currentFrame >= bossIntroFrames.size()) {
                showBossIntro = false;
                return;
            }
        }
        return;
    }

    Map::Update(deltaTime); // 일반 맵 업데이트 호출
}

void LastBoss::Render() {
    if (showBossIntro) {
        SDL_RenderCopy(g_renderer, bossIntroFrames[currentFrame], NULL, &destination_rectangle_);
        SDL_RenderPresent(g_renderer);
        return;
    }

    Map::Render(); // 일반 맵 렌더링 호출

    // 보스 HP 표시
    if (bossMonster) {
        RenderBossHP();
    }
}

void LastBoss::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            g_flag_running = false;
        }

        if (showBossIntro && (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYDOWN)) {
            frameTimer = 0.0f;
            currentFrame++;
            if (currentFrame >= bossIntroFrames.size()) {
                showBossIntro = false;
            }
            return;
        }

        Map::HandleEvents(); // 일반 맵 이벤트 처리 호출
    }
}

void LastBoss::RenderBossHP() {
    int bossHP = bossMonster->getHP();
    int maxHP = 20;

    SDL_Rect hpBarBg = { (WINDOW_WIDTH - 400) / 2, 20, 400, 40 };
    SDL_SetRenderDrawColor(g_renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(g_renderer, &hpBarBg);

    SDL_Rect hpBarValue = { (WINDOW_WIDTH - 400) / 2, 20, 400 * bossHP / maxHP, 40 };
    SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(g_renderer, &hpBarValue);

    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(g_renderer, &hpBarBg);
}

bool LastBoss::IsShowBossIntro() const {
    return showBossIntro;
}
