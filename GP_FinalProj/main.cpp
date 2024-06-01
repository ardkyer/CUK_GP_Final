#include "GameClass.h"
#include "Entrance.h"
#include "KimSuHwan.h"
#include "Hall.h"
#include "globals.h"
#include "GamePhases.h"
#include "Minimap.h"
#include "Health.h"
#include "Chisam.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <SDL_image.h> // Include SDL_image push test

void SetPlayerToCenter(Player& player) {
    player.SetPosition(WINDOW_WIDTH / 2 - player.GetRect().w / 2, WINDOW_HEIGHT / 2 - player.GetRect().h / 2);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG); // Initialize SDL_image

    TTF_Font* font = TTF_OpenFont("../../Resource/YEONGJUSeonbi.ttf", 24); // 적절한 경로로 변경
    g_window = SDL_CreateWindow("Dungeon of Catholic (Beta)", 500, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

    InitGame();

    Minimap minimap(g_renderer);
    Health health(g_renderer);

    Entrance entrance;
    KimSuHwan kimsuhwan;
    Hall hall;
    Player player;
    Chisam chisam(WINDOW_WIDTH / 20, WINDOW_HEIGHT / 20, g_renderer, font);

    SetPlayerToCenter(player);

    while (g_flag_running) {
        Uint32 cur_time_ms = SDL_GetTicks();
        float deltaTime = (cur_time_ms - g_last_time_ms) / 1000.0f;
        if (cur_time_ms - g_last_time_ms < (1000 / g_frame_per_sec))
            continue;

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                g_flag_running = false;
            }
            chisam.HandleEvents(e);

            // 현재 맵의 몬스터 목록을 player.HandleEvents에 전달
            switch (g_current_game_phase) {
            case PHASE_Entrance:
                player.HandleEvents(e, entrance.GetMonsters());
                break;
            case PHASE_KimSuHwan:
                player.HandleEvents(e, kimsuhwan.GetMonsters());
                break;
            case PHASE_Hall:
                player.HandleEvents(e, hall.GetMonsters());
                break;
            }
        }

        player.Update(deltaTime);

        if (g_player_health <= 0) {
            g_current_game_phase = PHASE_Entrance;
            g_player_health = 5;
            entrance.ResetMonsters();
            SetPlayerToCenter(player);
        }

        // 단계 전환 타이머 업데이트
        if (g_phase_transition_timer > 0) {
            g_phase_transition_timer -= deltaTime;
            if (g_phase_transition_timer <= 0) {
                // 다음 단계로 전환
                switch (g_current_game_phase) {
                case PHASE_Entrance:
                    g_current_game_phase = PHASE_KimSuHwan;
                    kimsuhwan.ResetMonsters();  // 다음 맵 초기화
                    break;
                case PHASE_KimSuHwan:
                    g_current_game_phase = PHASE_Hall;
                    hall.ResetMonsters();  // 다음 맵 초기화
                    break;
                case PHASE_Hall:
                    g_current_game_phase = PHASE_Entrance;
                    entrance.ResetMonsters();  // 다시 처음으로 돌아옴
                    break;
                }
                SetPlayerToCenter(player); // 맵 전환 후 플레이어 위치 중앙으로 설정
            }
        }
        else {
            bool showMinimapAndHealth = true;

            switch (g_current_game_phase) {
            case PHASE_Entrance:
                minimap.UpdatePlayerPosition(0);
                entrance.Update(deltaTime);
                entrance.Render();
                chisam.Render();
                break;
            case PHASE_KimSuHwan:
                minimap.UpdatePlayerPosition(1);
                kimsuhwan.Update(deltaTime);
                kimsuhwan.Render();
                break;
            case PHASE_Hall:
                minimap.UpdatePlayerPosition(2);
                hall.Update(deltaTime);
                hall.Render();
                break;
            }

            player.Render();

            if (showMinimapAndHealth) {
                minimap.Render(g_player_destination_rect.x, g_player_destination_rect.y);
                health.Render();
            }

            SDL_RenderPresent(g_renderer);
            g_last_time_ms = cur_time_ms;
        }
    }

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);

    ClearGame();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
