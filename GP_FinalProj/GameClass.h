// GameClass.h
#pragma once

#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void InitGame();
void ClearGame();

// Game Phases
const int PHASE_Entrance = 0;
const int PHASE_KimSuHwan = 1;
const int PHASE_Hall = 2;
const int PHASE_Nicols1 = 3;
const int PHASE_Dasol = 4;
const int PHASE_Sophiebara = 5;
const int PHASE_Michael = 6;
const int PHASE_LastBoss = 7;


extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern Uint32 g_frame_per_sec;
extern bool g_move_left;
extern bool g_move_right;
extern bool g_move_up;
extern bool g_move_down;
extern int g_player_health;

// Game variables
enum class PlayerDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

extern SDL_Rect g_player_destination_rect;
extern PlayerDirection g_player_direction;

// Ŭ��ŷ ����
extern bool is_cloaking;
extern int cloaking_alpha;
extern bool is_cloaking_on;
extern float cloaking_time;
extern float cloaking_duration;