#include "Ending.h"
#include "globals.h"
#include <SDL_image.h>

Ending::Ending() {
    // ��� �̹��� �ε�
    SDL_Surface* backgroundSurface = IMG_Load("../../Resource/Map/Ending.png");
    backgroundTexture = SDL_CreateTextureFromSurface(g_renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // ��� �̹��� ũ�� ����
    backgroundRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
}

Ending::~Ending() {
    SDL_DestroyTexture(backgroundTexture);
}

void Ending::Update(float deltaTime) {
    // Ending ȭ�鿡���� Ư���� ������Ʈ ������ �����ϴ�.
}

void Ending::Render() {
    SDL_RenderCopy(g_renderer, backgroundTexture, nullptr, &backgroundRect);
}
