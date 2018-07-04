#pragma once

#include <array>

#include "Vector2f.h"

struct SDL_Surface;
union SDL_Event;
class Drawer;
class Avatar;
class World;
class Ghost;

enum GameState
{
	GAMEPLAY,
	WIN_SCREEN,
	LOSE_SCREEN
};

class Pacman
{
public:
	static Pacman* Create(Drawer* aDrawer);
	~Pacman();

	bool Update(const SDL_Event* event, float aTime);
	void Draw() const;

	void GetAvatarPosition(int* __restrict aPosX, int* __restrict aPosY) const;
	void GetAvatarDirection(int* __restrict aDirX, int* __restrict aDirY) const;
	void GetRedGhostPos(int* __restrict aPosX, int* __restrict aPosY) const;

private:
	Pacman(Drawer* aDrawer);
	bool Init() const;
	bool updateInput(const SDL_Event* event);
	void MoveAvatar() const;
	bool CheckEndGameCondition() const;

	void SpawnGhosts(float aTime);
	void UpdateAvatar(float aTime) const;
	void UpdateGhosts(float aTime);
	void CheckIntersections();

	void Reset();

private:
	GameState myState;
	Drawer* myDrawer;

	float myGhostSwitchStateTimer;
	float myGhostSpawnTimer;

	int myLives;
	int myScore;
	int myFps;

	Vector2f myNextMovement;
	int myDirection;

	Avatar* myAvatar;
	std::array<Ghost*, 4> myGhosts;
	World* myWorld;

};