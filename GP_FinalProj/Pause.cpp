// Pause.cpp
#include "Pause.h"
#include "globals.h"
#include "GamePhases.h"
#include <SDL_image.h>

Pause::Pause() {
    LoadTextures();

    backgroundRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    continueRect = { WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 150, 200, 50 };
    retryRect = { WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 50, 200, 50 };
    quitRect = { WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 50, 200, 50 };
}

Pause::~Pause() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(continueTexture);
    SDL_DestroyTexture(retryTexture);
    SDL_DestroyTexture(quitTexture);
}

void Pause::LoadTextures() {
    backgroundTexture = IMG_LoadTexture(g_renderer, "../../Resource/Map/Pause_bg.png");
    continueTexture = IMG_LoadTexture(g_renderer, "../../Resource/Map/Pause_continue.png");
    retryTexture = IMG_LoadTexture(g_renderer, "../../Resource/Map/Pause_retry.png");
    quitTexture = IMG_LoadTexture(g_renderer, "../../Resource/Map/Pause_gamequit.png");
}

void Pause::Update(float deltaTime) {
    // �Ͻ����� ȭ�鿡���� ������Ʈ ������ �ʿ����� �ʽ��ϴ�.
}

void Pause::Render() {
    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(backgroundTexture, 128);
    SDL_RenderCopy(g_renderer, backgroundTexture, NULL, &backgroundRect);
        
    SDL_RenderCopy(g_renderer, continueTexture, NULL, &continueRect);
    SDL_RenderCopy(g_renderer, retryTexture, NULL, &retryRect);
    SDL_RenderCopy(g_renderer, quitTexture, NULL, &quitRect);
}

void Pause::HandleEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            int x = event.button.x;
            int y = event.button.y;

            if (x >= continueRect.x && x <= continueRect.x + continueRect.w &&
                y >= continueRect.y && y <= continueRect.y + continueRect.h) {
                // ����ϱ� ��ư Ŭ�� �� ���� ������� ���ư��ϴ�.
                g_current_game_phase = g_paused_game_phase;
            }
            else if (x >= retryRect.x && x <= retryRect.x + retryRect.w &&
                y >= retryRect.y && y <= retryRect.y + retryRect.h) {
                // ����� ��ư Ŭ�� �� Intro ������� ���ư��� ������ �ʱ�ȭ�մϴ�.
                g_current_game_phase = PHASE_Intro;
                g_reset_game = true; // ���� �ʱ�ȭ �÷��� ����
            }
            else if (x >= quitRect.x && x <= quitRect.x + quitRect.w &&
                y >= quitRect.y && y <= quitRect.y + quitRect.h) {
                // ���� ��ư Ŭ�� �� ������ �����մϴ�.
                g_flag_running = false;
            }
        }
    }
    else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            // ESC Ű�� ������ ���� ������� ���ư��ϴ�.
            g_current_game_phase = g_paused_game_phase;
        }
    }
}