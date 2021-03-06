#pragma once

#include "Vector2f.h"

constexpr const int MAX_TILE_NUM = 30;
constexpr const int TILE_SIZE = 22;

constexpr const int NO_MOVE = 0;
constexpr const int UP    = 0x0001;
constexpr const int RIGHT = 0x0010;
constexpr const int DOWN  = 0x0100;
constexpr const int LEFT  = 0x1000;

constexpr const Vector2f NO_DIRECTION;
constexpr const Vector2f UP_DIRECTION(0.0f, -1.f);
constexpr const Vector2f RIGHT_DIRECTION(1.0f, 0.0f);
constexpr const Vector2f DOWN_DIRECTION(0.0f, 1.0f);
constexpr const Vector2f LEFT_DIRECTION(-1.0f, 0.0f);

constexpr const int START_PLAYER_TILE_X = 12;
constexpr const int START_PLAYER_TILE_Y = 22;
constexpr const int START_GHOST_TILE_X = 13;
constexpr const int START_GHOST_TILE_Y = 13;
constexpr const Vector2f START_PLAYER_POS(static_cast<float>(START_PLAYER_TILE_X*TILE_SIZE), static_cast<float>(START_PLAYER_TILE_Y*TILE_SIZE));
constexpr const Vector2f START_GHOST_POS(static_cast<float>(START_GHOST_TILE_X*TILE_SIZE), static_cast<float>(START_GHOST_TILE_Y*TILE_SIZE));

constexpr int X_OFFSET = 220;
constexpr int Y_OFFSET = 60;

constexpr const float GHOST_SPEED = 60.f;
constexpr const float DEAD_GHOST_SPEED = 120.f;
constexpr const float PLAYER_SPEED = 90.f;

constexpr extern const float EPSILON = 0.000001f;

constexpr const int TOP_LEFT_X = 0;
constexpr const int TOP_LEFT_Y = 0;
constexpr const int TOP_RIGHT_X = 25;
constexpr const int TOP_RIGHT_Y = 0;
constexpr const int BOTTOM_LEFT_X = 0;
constexpr const int BOTTOM_LEFT_Y = 28;
constexpr const int BOTTOM_RIGHT_X = 25;
constexpr const int BOTTOM_RIGHT_Y = 28;
