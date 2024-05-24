#include "Minimap.h"
#include <SDL_image.h>
#include <iostream>

Minimap::Minimap(SDL_Renderer* renderer) : renderer(renderer), currentMapIndex(0) {
    // �� �ؽ�ó �ε�
    SDL_Surface* mapSurface = IMG_Load("../../Resource/Map/minimap.png");
    mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    SDL_FreeSurface(mapSurface);

    // �÷��̾� �ؽ�ó �ε�
    SDL_Surface* playerSurface = IMG_Load("../../Resource/Character/player_icon.png");
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);

    // �÷��̾� Rect �ʱ�ȭ
    playerRect = { 0, 0, 16, 16 }; // �÷��̾� ������ ũ�� ����

    // �̴ϸ��� �� ��ġ�� �����ϴ� Rect �ʱ�ȭ
    mapRects.push_back({ 10, 140, 32, 32 }); // start ��ġ
    mapRects.push_back({ 10, 115, 32, 32 });
    mapRects.push_back({ 10, 95, 32, 32 });
    mapRects.push_back({ 50, 95, 32, 32 });
    mapRects.push_back({ 52, 52, 32, 32 });
    mapRects.push_back({ 52, 94, 32, 32 });
    mapRects.push_back({ 94, 10, 32, 32 });
    mapRects.push_back({ 94, 52, 32, 32 });
    mapRects.push_back({ 94, 94, 32, 32 });
    mapRects.push_back({ 136, 10, 32, 32 });
    mapRects.push_back({ 136, 52, 32, 32 });
    mapRects.push_back({ 136, 94, 32, 32 });

    playerRect.x = mapRects[currentMapIndex].x;
    playerRect.y = mapRects[currentMapIndex].y;
}

Minimap::~Minimap() {
    SDL_DestroyTexture(mapTexture);
    SDL_DestroyTexture(playerTexture);
}

void Minimap::Render(int playerX, int playerY) {
    // �̴ϸ� ������
    SDL_Rect mapRect = { 0, 0, 160, 160 }; // �̴ϸ� ũ�� ����
    SDL_RenderCopy(renderer, mapTexture, NULL, &mapRect);

    // �÷��̾� ������ ������
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
}

void Minimap::UpdatePlayerPosition(int mapIndex) {
    if (mapIndex < 0 || mapIndex >= mapRects.size()) {
        std::cerr << "Invalid map index" << std::endl;
        return;
    }
    currentMapIndex = mapIndex;
    playerRect.x = mapRects[mapIndex].x;
    playerRect.y = mapRects[mapIndex].y;
}
