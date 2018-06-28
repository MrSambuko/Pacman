#pragma once


#include "Vector2f.h"

struct SDL_Surface;
union SDL_Event;
class Drawer;
class Avatar;
class World;
class Ghost;

class Pacman
{
public:
	static Pacman* Create(Drawer* aDrawer);

	bool Update(const SDL_Event* event, float aTime);
	void Draw() const;

private:
	Pacman(Drawer* aDrawer);
	bool Init() const;
	bool updateInput(const SDL_Event* event);
	void MoveAvatar() const;
	bool CheckEndGameCondition() const;

	Drawer* myDrawer;

	float myGhostGhostCounter;

	int myLives;
	int myScore;
	int myFps;

	Vector2f myNextMovement;
	int myDirection;

	Avatar* myAvatar;
	Ghost* myGhost;
	World* myWorld;

};