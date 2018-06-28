#pragma once

#include "Vector2f.h"

constexpr const int MAX_TILE_NUM = 100;
constexpr const int TILE_SIZE = 22;

constexpr const int NO_MOVE = 0;
constexpr const int UP    = 0x0001;
constexpr const int RIGHT = 0x0010;
constexpr const int DOWN  = 0x0100;
constexpr const int LEFT  = 0x1000;

constexpr const Vector2f NO_MOVEMENT;
constexpr const Vector2f UP_MOVEMENT(0.0f, 1.f);
constexpr const Vector2f RIGHT_MOVEMENT(1.0f, 0.0f);
constexpr const Vector2f DOWN_MOVEMENT(0.0f, -1.0f);
constexpr const Vector2f LEFT_MOVEMENT(-1.0f, 0.0f);

constexpr const Vector2f START_PLAYER_POS(12.f * TILE_SIZE, 22.f * TILE_SIZE);
constexpr const Vector2f START_GHOST_POS(13.f * TILE_SIZE, 13.f * TILE_SIZE);