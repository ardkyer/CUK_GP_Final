#include "GameClass.h"
#include "Nicols1.h"
#include "RangedMonster.h"
#include "globals.h"
#include <algorithm>

Nicols1::Nicols1() {
    // ��� �ε�
    SDL_Surface* temp_surface = IMG_Load("../../Resource/Map/Nicols1.png");
    texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(texture_, NULL, NULL, &source_rectangle_.w, &source_rectangle_.h);
    destination_rectangle_ = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    // ĳ���� �ε�
    SDL_Surface* flight_sheet_surface = IMG_Load("../../Resource/Character/Isacc.png");
    g_flight_sheet_texture_up = SDL_CreateTextureFromSurface(g_renderer, flight_sheet_surface);
    g_flight_sheet_texture_down = SDL_CreateTextureFromSurface(g_renderer, flight_sheet_surface);
    g_flight_sheet_texture_left = SDL_CreateTextureFromSurface(g_renderer, flight_sheet_surface);
    g_flight_sheet_texture_right = SDL_CreateTextureFromSurface(g_renderer, flight_sheet_surface);
    SDL_FreeSurface(flight_sheet_surface);

    // ��Ż �ε�
    SDL_Surface* portal_surface = IMG_Load("../../Resource/Map/portal.png");
    SDL_Surface* resized_portal_surface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
    SDL_Rect resized_portal_rect = { 0, 0, 100, 100 };
    SDL_BlitScaled(portal_surface, NULL, resized_portal_surface, &resized_portal_rect);
    SDL_FreeSurface(portal_surface);

    portal_texture = SDL_CreateTextureFromSurface(g_renderer, resized_portal_surface);
    SDL_FreeSurface(resized_portal_surface);

    portal_rect_N1toH = { 0, (WINDOW_HEIGHT - 100) / 2, 100, 100 };
    portal_rect_N1toN2 = { WINDOW_WIDTH - 100, (WINDOW_HEIGHT - 100) / 2, 100, 100 };
    portal_rect_N1toV = { (WINDOW_WIDTH - 100) / 2, 0, 100, 100 };

    // ���� �̹��� �ε�
    SDL_Surface* rock_surface = IMG_Load("../../Resource/Map/rock.png");
    rock_texture = SDL_CreateTextureFromSurface(g_renderer, rock_surface);
    SDL_FreeSurface(rock_surface);

    // Initialize rock
    rockRect = { WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50, 100, 100 }; // Center of the map

    // ���� ����
    SpawnMonsters();
}

Nicols1::~Nicols1() {
    SDL_DestroyTexture(texture_);
    SDL_DestroyTexture(g_flight_sheet_texture_up);
    SDL_DestroyTexture(g_flight_sheet_texture_down);
    SDL_DestroyTexture(g_flight_sheet_texture_left);
    SDL_DestroyTexture(g_flight_sheet_texture_right);
    SDL_DestroyTexture(portal_texture);
    SDL_DestroyTexture(rock_texture); // ���� �ؽ�ó ����
    delete rangedMonster; // Free the memory allocated for the RangedMonster
}

void Nicols1::SpawnMonsters() {
    rangedMonster = new RangedMonster(WINDOW_WIDTH - 128, 0); // Position at the top-right corner
}

void Nicols1::Update(float deltaTime) {
    const float moveSpeed = 500.0f;

    // �Է� ���¿� ���� �̵�
    if (g_move_left) {
        g_player_destination_rect.x -= static_cast<int>(moveSpeed * deltaTime);
        g_player_direction = PlayerDirection::LEFT;
    }
    if (g_move_right) {
        g_player_destination_rect.x += static_cast<int>(moveSpeed * deltaTime);
        g_player_direction = PlayerDirection::RIGHT;
    }
    if (g_move_up) {
        g_player_destination_rect.y -= static_cast<int>(moveSpeed * deltaTime);
        g_player_direction = PlayerDirection::UP;
    }
    if (g_move_down) {
        g_player_destination_rect.y += static_cast<int>(moveSpeed * deltaTime);
        g_player_direction = PlayerDirection::DOWN;
    }

    // ������ ��踦 ����� �ʵ��� ����
    g_player_destination_rect.x = std::max(0, std::min(WINDOW_WIDTH - g_player_destination_rect.w - 40, g_player_destination_rect.x));
    g_player_destination_rect.y = std::max(0, std::min(WINDOW_HEIGHT - g_player_destination_rect.h - 40, g_player_destination_rect.y));

    // ������ �浹 Ȯ��
    SDL_Rect reducedRockRect = {
    rockRect.x + rockRect.w * 0.2, // ���� ���� 10% ���̱�
    rockRect.y + rockRect.h * 0.2, // ���� ���� 30% ���̱�
    rockRect.w * 0.3,              // ���� 70%�� ���̱�
    rockRect.h * 0.4               // ���̸� 40%�� ���̱�
    };

    if (SDL_HasIntersection(&g_player_destination_rect, &reducedRockRect)) {
        if (g_move_left) g_player_destination_rect.x += static_cast<int>(moveSpeed * deltaTime);
        if (g_move_right) g_player_destination_rect.x -= static_cast<int>(moveSpeed * deltaTime);
        if (g_move_up) g_player_destination_rect.y += static_cast<int>(moveSpeed * deltaTime);
        if (g_move_down) g_player_destination_rect.y -= static_cast<int>(moveSpeed * deltaTime);
    }

    // ��Ż�� ĳ���� �浹 Ȯ��
    if (SDL_HasIntersection(&g_player_destination_rect, &portal_rect_N1toH)) {
        g_current_game_phase = PHASE_Hall;
        g_player_destination_rect = { 600, WINDOW_HEIGHT / 2, 100, 100 };
        g_player_direction = PlayerDirection::DOWN;
    }
    if (SDL_HasIntersection(&g_player_destination_rect, &portal_rect_N1toV)) {
        g_current_game_phase = PHASE_Virtus;
        g_player_destination_rect = { WINDOW_WIDTH / 2, 400, 100, 100 };
        g_player_direction = PlayerDirection::UP;
    }
    if (SDL_HasIntersection(&g_player_destination_rect, &portal_rect_N1toN2)) {
        g_current_game_phase = PHASE_Nicols2;
        g_player_destination_rect = { 110, WINDOW_HEIGHT / 2, 100, 100 };
        g_player_direction = PlayerDirection::UP;
    }

    // ���Ϳ� �÷��̾� �浹 Ȯ��
    rangedMonster->Update(deltaTime, g_player_destination_rect);

    if (rangedMonster->CheckCollisionWithPlayer(g_player_destination_rect)) {
        g_player_health--;
        if (g_player_health <= 0) {
            // ���� ���� ó��
            g_current_game_phase = PHASE_Entrance;
            g_player_destination_rect = { WINDOW_WIDTH / 2, 110, 100, 100 };
            g_player_direction = PlayerDirection::DOWN;
            g_player_health = 5;
            delete rangedMonster; // Free the memory allocated for the RangedMonster
            SpawnMonsters(); // Reset monsters when health is depleted and player respawns
        }
    }

    // ����ü�� ���� �浹 Ȯ��
    for (auto& projectile : rangedMonster->GetProjectiles()) {
        if (projectile.active && SDL_HasIntersection(&projectile.rect, &reducedRockRect)) {
            projectile.active = false; // Deactivate projectile on collision
        }
    }
}

void Nicols1::Render() {
    SDL_SetRenderDrawColor(g_renderer, 0, 255, 255, 0);
    SDL_RenderClear(g_renderer);

    // ���
    SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);

    // ĳ����
    SDL_Rect flightRect = g_player_destination_rect;
    SDL_RenderCopy(g_renderer, g_flight_sheet_texture_up, NULL, &flightRect);

    // �÷��̾� ü�� �׸���
    for (int i = 0; i < g_player_health; i++) {
        SDL_Rect healthRect = { 10 + i * 20, 10, 20, 20 };
        SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(g_renderer, &healthRect);
    }

    // ��Ż �׸���
    SDL_RenderCopy(g_renderer, portal_texture, NULL, &portal_rect_N1toH);
    SDL_RenderCopy(g_renderer, portal_texture, NULL, &portal_rect_N1toN2);
    SDL_RenderCopy(g_renderer, portal_texture, NULL, &portal_rect_N1toV);

    // ���� �׸���
    SDL_RenderCopy(g_renderer, rock_texture, NULL, &rockRect);

    // ���� �׸���
    rangedMonster->Render();

    SDL_RenderPresent(g_renderer);
}

void Nicols1::HandleEvents() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            g_flag_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                g_move_left = true;
                break;
            case SDLK_RIGHT:
                g_move_right = true;
                break;
            case SDLK_UP:
                g_move_up = true;
                break;
            case SDLK_DOWN:
                g_move_down = true;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                g_move_left = false;
                break;
            case SDLK_RIGHT:
                g_move_right = false;
                break;
            case SDLK_UP:
                g_move_up = false;
                break;
            case SDLK_DOWN:
                g_move_down = false;
                break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_RIGHT) {
                g_current_game_phase = PHASE_Nicols2;
            }
            break;
        }
    }
}