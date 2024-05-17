// Nicols1.cpp
#include "GameClass.h"
#include "Nicols1.h"
#define NOMINMAX
#include <windows.h>
#include <atlstr.h>
#include <string>
#include <iomanip>
#include <algorithm>

Nicols1::Nicols1()
{
    // ��� �ε�
    SDL_Surface* temp_surface = IMG_Load("../../Resource/Map/Nicols1.png");
    texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(texture_, NULL, NULL, &source_rectangle_.w, &source_rectangle_.h);
    source_rectangle_.x = 0;
    source_rectangle_.y = 0;
    destination_rectangle_.x = 0;
    destination_rectangle_.y = 0;
    destination_rectangle_.w = WINDOW_WIDTH;
    destination_rectangle_.h = WINDOW_HEIGHT;

    // ����� �ε�
    SDL_Surface* flight_sheet_surface_up = IMG_Load("../../Resource/Character/Isacc.png");
    g_flight_sheet_texture_up = SDL_CreateTextureFromSurface(g_renderer, flight_sheet_surface_up);
    SDL_FreeSurface(flight_sheet_surface_up);

    SDL_Surface* flight_sheet_surface_down = IMG_Load("../../Resource/Character/Isacc.png");
    g_flight_sheet_texture_down = SDL_CreateTextureFromSurface(g_renderer, flight_sheet_surface_down);
    SDL_FreeSurface(flight_sheet_surface_down);

    SDL_Surface* flight_sheet_surface_left = IMG_Load("../../Resource/Character/Isacc.png");
    g_flight_sheet_texture_left = SDL_CreateTextureFromSurface(g_renderer, flight_sheet_surface_left);
    SDL_FreeSurface(flight_sheet_surface_left);

    SDL_Surface* flight_sheet_surface_right = IMG_Load("../../Resource/Character/Isacc.png");
    g_flight_sheet_texture_right = SDL_CreateTextureFromSurface(g_renderer, flight_sheet_surface_right);
    SDL_FreeSurface(flight_sheet_surface_right);

    // ��Ż �ε�
    SDL_Surface* portal_surface = IMG_Load("../../Resource/Map/portal.png");

    // ��Ż ũ�� ����
    int portal_width = 100;
    int portal_height = 100;
    SDL_Surface* resized_portal_surface = SDL_CreateRGBSurface(0, portal_width, portal_height, 32, 0, 0, 0, 0);
    SDL_Rect resized_portal_rect = { 0, 0, portal_width, portal_height };
    SDL_BlitScaled(portal_surface, NULL, resized_portal_surface, &resized_portal_rect);
    SDL_FreeSurface(portal_surface);

    portal_texture = SDL_CreateTextureFromSurface(g_renderer, resized_portal_surface);
    SDL_FreeSurface(resized_portal_surface);

    portal_rect_N1toH.w = portal_width;
    portal_rect_N1toH.h = portal_height;

    portal_rect_N1toN2.w = portal_width;
    portal_rect_N1toN2.h = portal_height;
    
    portal_rect_N1toV.w = portal_width;
    portal_rect_N1toV.h = portal_height;
    
    // ��Ż ��ġ ����(��)
    portal_rect_N1toH.x = 0;
    portal_rect_N1toH.y = (WINDOW_HEIGHT - portal_rect_N1toH.h) / 2;

    // ��Ż ��ġ ����(��)
    portal_rect_N1toV.x = (WINDOW_WIDTH - portal_rect_N1toV.w) /2;
    portal_rect_N1toV.y = 0;

    // ��Ż ��ġ ����(��)
    portal_rect_N1toN2.x = 700;
    portal_rect_N1toN2.y = (WINDOW_HEIGHT - portal_rect_N1toN2.h) / 2;


    // Ŭ��ŷ ����
    is_cloaking = false;
    cloaking_alpha = 255;
    is_cloaking_on = false;
    cloaking_time = 0.0f;
    cloaking_duration = 1.0f; // Ŭ��ŷ ���� �ð� (��)
}

Nicols1::~Nicols1()
{
    // Free textures
    SDL_DestroyTexture(texture_);
    SDL_DestroyTexture(g_flight_sheet_texture_up);
    SDL_DestroyTexture(g_flight_sheet_texture_down);
    SDL_DestroyTexture(g_flight_sheet_texture_left);
    SDL_DestroyTexture(g_flight_sheet_texture_right);

}

void Nicols1::Update(float deltaTime)
{
    const float moveSpeed = 500.0f; // �ʴ� �̵��� �ȼ� ��

    // �Է� ���¿� ���� �̵�
    if (g_move_left) {
        g_player_destination_rect.x -= moveSpeed * deltaTime;
        g_player_direction = PlayerDirection::LEFT;
    }
    if (g_move_right) {
        g_player_destination_rect.x += moveSpeed * deltaTime;
        g_player_direction = PlayerDirection::RIGHT;
    }
    if (g_move_up) {
        g_player_destination_rect.y -= moveSpeed * deltaTime;
        g_player_direction = PlayerDirection::UP;
    }
    if (g_move_down) {
        g_player_destination_rect.y += moveSpeed * deltaTime;
        g_player_direction = PlayerDirection::DOWN;
    }

    // ������ ��踦 ����� �ʵ��� ����
    g_player_destination_rect.x = std::max(0, std::min(WINDOW_WIDTH - g_player_destination_rect.w - 40, g_player_destination_rect.x));
    g_player_destination_rect.y = std::max(0, std::min(WINDOW_HEIGHT - g_player_destination_rect.h - 40, g_player_destination_rect.y));

    // Ŭ��ŷ ���� ������Ʈ
    if (is_cloaking)
    {
        if (is_cloaking_on)
        {
            cloaking_time += deltaTime;
            cloaking_alpha = std::max(0, 255 - static_cast<int>((cloaking_time / cloaking_duration) * 255));
            if (cloaking_time >= cloaking_duration)
            {
                is_cloaking = false;
                cloaking_time = 0.0f;
            }
        }
        else
        {
            cloaking_time += deltaTime;
            cloaking_alpha = std::min(255, static_cast<int>((cloaking_time / cloaking_duration) * 255));
            if (cloaking_time >= cloaking_duration)
            {
                is_cloaking = false;
                cloaking_time = 0.0f;
            }
        }
    }
    // ��Ż�� ĳ���� �浹 Ȯ��(hall)
    if (SDL_HasIntersection(&g_player_destination_rect, &portal_rect_N1toH))
    {
        // ���� ���� �÷��̾� ��ġ ����
        g_current_game_phase = PHASE_Hall;
        g_player_destination_rect = { 600, WINDOW_HEIGHT / 2, 100, 100 };
        g_player_direction = PlayerDirection::DOWN;

    }
    // ��Ż�� ĳ���� �浹 Ȯ��(V)
    if (SDL_HasIntersection(&g_player_destination_rect, &portal_rect_N1toV))
    {
        // ���� ���� �÷��̾� ��ġ ����
        g_current_game_phase = PHASE_Virtus;
        g_player_destination_rect = { WINDOW_WIDTH / 2, 400, 100, 100 };
        g_player_direction = PlayerDirection::UP;

    }
    // ��Ż�� ĳ���� �浹 Ȯ��(N2)
    if (SDL_HasIntersection(&g_player_destination_rect, &portal_rect_N1toN2))
    {
        // ���� ���� �÷��̾� ��ġ ����
        g_current_game_phase = PHASE_Nicols2;
        g_player_destination_rect = { 110, WINDOW_HEIGHT / 2, 100, 100 };
        g_player_direction = PlayerDirection::UP;

    }
}

void Nicols1::Render()
{
    // ������ �ʱ�ȭ
    SDL_SetRenderDrawColor(g_renderer, 0, 255, 255, 0);
    SDL_RenderClear(g_renderer); // clear the renderer to the draw color

    // ���
    SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);

    // �����
    SDL_Rect flightRect = g_player_destination_rect;

    switch (g_player_direction) {
    case PlayerDirection::UP:
    case PlayerDirection::DOWN:
    case PlayerDirection::LEFT:
    case PlayerDirection::RIGHT:
        break;
    }

    SDL_Texture* flightTexture = nullptr;

    switch (g_player_direction) {
    case PlayerDirection::UP:
        flightTexture = g_flight_sheet_texture_up;
        break;
    case PlayerDirection::DOWN:
        flightTexture = g_flight_sheet_texture_down;
        break;
    case PlayerDirection::LEFT:
        flightTexture = g_flight_sheet_texture_left;
        break;
    case PlayerDirection::RIGHT:
        flightTexture = g_flight_sheet_texture_right;
        break;
    }

    // Ŭ��ŷ ����
    if (flightTexture != nullptr)
    {
        SDL_SetTextureAlphaMod(flightTexture, cloaking_alpha);
        SDL_RenderCopy(g_renderer, flightTexture, NULL, &flightRect);
    }

    // ��Ż �׸���
    SDL_SetTextureAlphaMod(portal_texture, cloaking_alpha);
    SDL_RenderCopy(g_renderer, portal_texture, NULL, &portal_rect_N1toH);
    SDL_RenderCopy(g_renderer, portal_texture, NULL, &portal_rect_N1toN2);
    SDL_RenderCopy(g_renderer, portal_texture, NULL, &portal_rect_N1toV);
    

    // ������ ����
    SDL_RenderPresent(g_renderer);
}

void Nicols1::HandleEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
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

            case SDLK_c:
                if (!is_cloaking)
                {
                    is_cloaking = true;
                    is_cloaking_on = true;
                    cloaking_time = 0.0f;
                }
                break;
            case SDLK_v:
                if (!is_cloaking)
                {
                    is_cloaking = true;
                    is_cloaking_on = false;
                    cloaking_time = 0.0f;
                }
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