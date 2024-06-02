#include "Gameover.h"
#include "globals.h"
#include "GamePhases.h"
#include <SDL_image.h>

Gameover::Gameover() {
    // ��� �̹��� �ε�
    SDL_Surface* backgroundSurface = IMG_Load("../../Resource/Map/GameOver.png");
    backgroundTexture = SDL_CreateTextureFromSurface(g_renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // ��� �̹��� ũ�� ����
    backgroundRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
}

Gameover::~Gameover() {
    SDL_DestroyTexture(backgroundTexture);
}

void Gameover::Update(float deltaTime) {
    // Gameover ȭ�鿡���� Ư���� ������Ʈ ������ �����ϴ�.
}

void Gameover::Render() {
    SDL_RenderCopy(g_renderer, backgroundTexture, nullptr, &backgroundRect);
}

void Gameover::HandleEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        // ���콺 ��ư�� ������ PHASE_Intro�� ��ȯ
        g_current_game_phase = PHASE_Intro;
    }
}