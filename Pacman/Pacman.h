#pragma once


#include "Vector2f.h"

struct SDL_Surface;
class Drawer;
class Avatar;
class World;
class Ghost;

class Pacman
{
public:
	static Pacman* Create(Drawer* aDrawer);

	bool Update(float aTime);
	bool Draw() const;

private:
	Pacman(Drawer* aDrawer);
	bool Init() const;
	bool UpdateInput();
	void MoveAvatar() const;
	bool CheckEndGameCondition() const;

	Drawer* myDrawer;

	float myGhostGhostCounter;

	int myLives;
	int myScore;
	int myFps;

	Vector2f myNextMovement;

	Avatar* myAvatar;
	Ghost* myGhost;
	World* myWorld;

};